#include "fx.h"
#include "../../thread_elem.h"
#include "OSFXFile.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>




#define FOX_THREAD_SAFE

struct fxdirent:dirent
{
    char buffer[256];
};



// Read bytes
static long fullread (int fd, unsigned char *ptr, long len)
{
    long nread;
#ifdef EINTR
    do
    {
	nread = read (fd, ptr, len);
    }
    while (nread < 0 && errno == EINTR);
#else
    nread = read (fd, ptr, len);
#endif
    return nread;
}


// Write bytes
static long fullwrite (int fd, const unsigned char *ptr, long len)
{
    long nwritten, ntotalwritten = 0;
    while (len > 0)
    {
	nwritten = write (fd, ptr, len);
	if (nwritten < 0)
	{
#ifdef EINTR
	    if (errno == EINTR)
		continue;
#endif
	    return -1;
	}
	ntotalwritten += nwritten;
	ptr += nwritten;
	len -= nwritten;
    }
    return ntotalwritten;
}



// Copy ordinary file
static FXbool copyfile (const FXString & oldfile, const FXString & newfile, thread_elem * te)
{
    unsigned char buffer[4096];
    struct stat status;
    long nread, nwritten;
    int src, dst;
    FXbool ok = FALSE;
    if ((src = open (oldfile.text (), O_RDONLY)) >= 0)
    {
	if (::stat (oldfile.text (), &status) == 0)
	{


	    te->act_file_name = oldfile.text ();
	    te->act_file_size = 1;
	    te->file_size = status.st_size;



	    if ((dst = open (newfile.text (), O_WRONLY | O_CREAT | O_TRUNC, status.st_mode)) >= 0)
	    {
		while (1)
		{
		    nread = fullread (src, buffer, sizeof (buffer));
		    if (nread < 0)
			goto err;
		    if (nread == 0)
			break;




		    if (te->cancel == true)
		    {
			fxmessage ("CANCEL CANCEL CANCEL CANCEL !!!\n\n\n\n\n");
			close (dst);
			close (src);
			return FALSE;
		    }

		    te->act_file_size += nread;
		    te->act_total_size += nread;



		    nwritten = fullwrite (dst, buffer, nread);
		    if (nwritten < 0)
			goto err;
		}
		ok = TRUE;
	      err:close (dst);
	    }
	}
	close (src);
    }
    return ok;
}


// To search visited inodes
struct inodelist
{
    ino_t st_ino;
    inodelist *next;
};


// Forward declararion
static FXbool copyrec (const FXString & oldfile, const FXString & newfile, FXbool overwrite, inodelist * inodes, thread_elem * te);


// Copy directory
static FXbool copydir (const FXString & oldfile, const FXString & newfile, FXbool overwrite, struct stat &parentstatus, inodelist * inodes, thread_elem * te)
{
    FXString oldchild, newchild;
    struct stat status;
    inodelist *in, inode;
    struct dirent *dp;
    DIR *dirp;

    // See if visited this inode already
    for (in = inodes; in; in = in->next)
    {
	if (in->st_ino == parentstatus.st_ino)
	    return TRUE;
    }

    // Try make directory, if none exists yet
    if (mkdir (newfile.text (), parentstatus.st_mode | S_IWUSR) != 0 && errno != EEXIST)
	return FALSE;

    // Can we stat it
    if (::lstat (newfile.text (), &status) != 0 || !S_ISDIR (status.st_mode))
	return FALSE;

    // Try open directory to copy
    dirp = opendir (oldfile.text ());
    if (!dirp)
	return FALSE;

    // Add this to the list
    inode.st_ino = status.st_ino;
    inode.next = inodes;

    // Copy stuff
#ifdef FOX_THREAD_SAFE
    struct fxdirent dirresult;
    while (!readdir_r (dirp, &dirresult, &dp) && dp)
    {
#else
    while ((dp = readdir (dirp)) != NULL)
    {
#endif
	if (dp->d_name[0] != '.' || (dp->d_name[1] != '\0' && (dp->d_name[1] != '.' || dp->d_name[2] != '\0')))
	{
	    oldchild = oldfile;
	    if (!ISPATHSEP (oldchild[oldchild.length () - 1]))
		oldchild.append (PATHSEP);
	    oldchild.append (dp->d_name);
	    newchild = newfile;
	    if (!ISPATHSEP (newchild[newchild.length () - 1]))
		newchild.append (PATHSEP);
	    newchild.append (dp->d_name);
	    if (!copyrec (oldchild, newchild, overwrite, &inode, te))
	    {
		closedir (dirp);
		return FALSE;
	    }
	}
    }

    // Close directory
    closedir (dirp);

    // Success
    return TRUE;
}




// Recursive copy
static FXbool copyrec (const FXString & oldfile, const FXString & newfile, FXbool overwrite, inodelist * inodes, thread_elem * te)
{
    struct stat status1, status2;

    // Old file or directory does not exist
    if (::lstat (oldfile.text (), &status1) != 0)
	return FALSE;

    // If target is not a directory, remove it if allowed
    if (::lstat (newfile.text (), &status2) == 0)
    {
	if (!S_ISDIR (status2.st_mode))
	{

	    if (te->all == false)
	    {

		te->question = true;
		while (te->answer != true)
		    usleep (5000);
		te->answer = false;

	    }
	    if (!te->overwrite)
		return TRUE;	//changed
	    FXTRACE ((100, "unlink(%s)\n", newfile.text ()));
	    if (::unlink (newfile.text ()) != 0)
		return FALSE;
	}
    }

    // Source is direcotory: copy recursively
    if (S_ISDIR (status1.st_mode))
    {
	return copydir (oldfile, newfile, overwrite, status1, inodes, te);
    }

    // Source is regular file: copy block by block
    if (S_ISREG (status1.st_mode))
    {
	FXTRACE ((100, "copyfile(%s,%s)\n", oldfile.text (), newfile.text ()));
	return copyfile (oldfile, newfile, te);
    }

    // Source is fifo: make a new one
    if (S_ISFIFO (status1.st_mode))
    {
	FXTRACE ((100, "mkfifo(%s)\n", newfile.text ()));
	return::mkfifo (newfile.text (), status1.st_mode);
    }

    // Source is device: make a new one
    if (S_ISBLK (status1.st_mode) || S_ISCHR (status1.st_mode) || S_ISSOCK (status1.st_mode))
    {
	FXTRACE ((100, "mknod(%s)\n", newfile.text ()));
	return::mknod (newfile.text (), status1.st_mode, status1.st_rdev) == 0;
    }

    // Source is symbolic link: make a new one
    if (S_ISLNK (status1.st_mode))
    {
	FXString lnkfile = FXFile::symlink (oldfile);
	FXTRACE ((100, "symlink(%s,%s)\n", lnkfile.text (), newfile.text ()));
	return::symlink (lnkfile.text (), newfile.text ()) == 0;
    }

    // This shouldn't happen
    return FALSE;
}





//====================================OSFXFile================================================================




// Copy file
FXbool OSFXFile::copy (const FXString & oldfile, const FXString & newfile, thread_elem * te, FXbool overwrite)
{
    if (newfile != oldfile)
    {

#ifndef WIN32
	return copyrec (oldfile, newfile, overwrite, NULL, te);
#else
	return copyrec (oldfile, newfile, overwrite);	// No symlinks, so no need to check if directories are visited already
#endif
    }
    return FALSE;
}


// Remove file or directory
FXbool OSFXFile::remove (const FXString & file, thread_elem * te)
{


    //if(te->mutex.trylock())
    //{
    te->act_file_name = file.text ();
    //te->mutex.unlock();
    //}
#ifndef WIN32
    struct stat status;
    if (::lstat (file.text (), &status) == 0)
    {
	if (S_ISDIR (status.st_mode))
	{
	    DIR *dirp =::opendir (file.text ());
	    if (dirp)
	    {
		FXString child;
		struct dirent *dp;
#ifdef FOX_THREAD_SAFE
		struct fxdirent dirresult;
		while (!readdir_r (dirp, &dirresult, &dp) && dp)
		{
#else
		while ((dp = readdir (dirp)) != NULL)
		{
#endif
		    if (dp->d_name[0] != '.' || (dp->d_name[1] != '\0' && (dp->d_name[1] != '.' || dp->d_name[2] != '\0')))
		    {
			child = file;
			if (!ISPATHSEP (child[child.length () - 1]))
			    child.append (PATHSEP);
			child.append (dp->d_name);
			if (!OSFXFile::remove (child, te))
			{
			    ::closedir (dirp);
			    return FALSE;
			}
		    }
		}
		::closedir (dirp);
	    }
	    FXTRACE ((100, "rmdir(%s)\n", file.text ()));
	    te->act_file_name = file.text ();
	    return::rmdir (file.text ()) == 0;
	}
	else
	{
	    FXTRACE ((100, "unlink(%s)\n", file.text ()));
	    te->act_file_name = file.text ();
	    return::unlink (file.text ()) == 0;
	}
    }
    return FALSE;
#else
    DWORD atts;
    if ((atts = GetFileAttributes (file.text ())) != 0xffffffff)
    {
	if (atts & FILE_ATTRIBUTE_DIRECTORY)
	{
	    WIN32_FIND_DATA ffData;
	    HANDLE hFindFile;
	    hFindFile = FindFirstFile ((file + PATHSEPSTRING + "*").text (), &ffData);	// FIXME we may want to formalize the "walk over directory" in a few API's here also...
	    if (hFindFile != INVALID_HANDLE_VALUE)
	    {
		FXString child;
		do
		{
		    if (ffData.cFileName[0] != '.' && (ffData.cFileName[1] != '\0' && (ffData.cFileName[1] != '.' || ffData.cFileName[2] != '\0')))
		    {
			child = file;
			if (!ISPATHSEP (child[child.length () - 1]))
			    child.append (PATHSEP);
			child.append (ffData.cFileName);
			if (!OSFXFile::remove (child))
			{
			    FindClose (hFindFile);
			    return FALSE;
			}
		    }
		}
		while (FindNextFile (hFindFile, &ffData));
		FindClose (hFindFile);
	    }
	    FXTRACE ((100, "RemoveDirectory(%s)\n", file.text ()));
	    return RemoveDirectory (file.text ()) != 0;
	}
	else
	{
	    FXTRACE ((100, "DeleteFile(%s)\n", file.text ()));
	    return DeleteFile (file.text ()) != 0;
	}
    }
    return FALSE;
#endif
}


// Rename or move file, or copy and delete old if different file systems
FXbool OSFXFile::move (const FXString & oldfile, const FXString & newfile, thread_elem * te, FXbool overwrite)
{

    if (newfile != oldfile)
    {
#ifndef WIN32
	if (!FXFile::exists (oldfile))
	    return FALSE;
	if (FXFile::exists (newfile))
	{

	    if (te->all == false)
	    {

		te->question = true;
		while (te->answer != true)
		    usleep (5000);
		te->answer = false;

	    }
	    if (!te->overwrite)
		return TRUE;	//changed
	    if (!OSFXFile::remove (newfile, te))
		return FALSE;
	}
	FXTRACE ((100, "rename(%s,%s)\n", oldfile.text (), newfile.text ()));
	if (::rename (oldfile.text (), newfile.text ()) == 0)
	    return TRUE;
	if (errno != EXDEV)
	    return FALSE;
	if (OSFXFile::copy (oldfile, newfile, te))
	{
	    return OSFXFile::remove (oldfile, te);
	}
#else
	if (!FXFile::exists (oldfile))
	    return FALSE;
	if (FXFile::exists (newfile))
	{
	    if (!overwrite)
		return FALSE;
	    if (!OSFXFile::remove (newfile, te))
		return FALSE;
	}
	FXTRACE ((100, "MoveFile(%s,%s)\n", oldfile.text (), newfile.text ()));
	if (::MoveFile (oldfile.text (), newfile.text ()) != 0)
	    return TRUE;
	if (GetLastError () != ERROR_NOT_SAME_DEVICE)
	    return FALSE;
	if (OSFXFile::copy (oldfile, newfile, te))
	{
	    return OSFXFile::remove (oldfile, te);
	}
#endif
    }
    return FALSE;
}
