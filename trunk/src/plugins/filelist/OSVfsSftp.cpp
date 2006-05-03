
#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif

#include "OSVfsSftp.h"
#include "OSFXFile.h"
#include "../../OSVirtualFileSystemHeader.h"
#include "../../OSVirtualFileSystemInfo.h"
#include "../../OSThreadExec.h"
#include "../../OSFile.h"
#include "../../OSUtils.h"

#ifdef WIN32
#define SEPARATOR "\\"
#else
#include <dlfcn.h>
#define SEPARATOR "/"
#endif

#include <fx.h>
#include <errno.h>
#include <fstream>
#include <sstream>
#include <sys/mount.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

#include <sys/stat.h>
#include <fcntl.h>


#include <string>
#include <vector>
#include <iostream>

using namespace std;


OSVirtualFileSystemInfo OSVfsSftp::setup (void)
{
	OSVirtualFileSystemInfo v;
	
	v.vfsheaders.push_back(OSVirtualFileSystemHeader("name"));
	v.vfsheaders.push_back(OSVirtualFileSystemHeader("mode"));
	v.vfsheaders.push_back(OSVirtualFileSystemHeader("owner"));
	v.vfsheaders.push_back(OSVirtualFileSystemHeader("group"));
	v.vfsheaders.push_back(OSVirtualFileSystemHeader("size","size"));
	v.vfsheaders.push_back(OSVirtualFileSystemHeader("accessed","date"));
	v.vfsheaders.push_back(OSVirtualFileSystemHeader("modified","date"));	
	v.information="SFTP file list - default plugin";
	v.version="1";
	
    return v;	
}

int OSVfsSftp::init (long id, std::vector<std::string> *vector_name, OSPathType pt, OSConfigure * conf)
{
    this->id=id;
    fields = (*vector_name);

    fieldsnum = vector_name->size ();


    dirsftp=NULL;
    sftp=NULL;

    
    SSH_OPTIONS *options;
    int auth=0;
    char *password;
    char *banner;
    int state;
    char buf[10];
    char hash[MD5_DIGEST_LEN];

    options=options_new();
    if(!options)
    {
        fprintf(stderr,"Error : %s\n",ssh_get_error(NULL));
        return -1;
    } 
      
    options_set_username(options,(char*)pt.user.c_str());
    options_set_host(options,(char*)pt.server.c_str());
    if(pt.port!="")
    options_set_port(options,atoi(pt.port.c_str()));
    session=ssh_connect(options);
    
    if(!session)
    {
        fprintf(stderr,"Connection failed : %s\n",ssh_get_error(NULL));
        return -1;
    }

    state=ssh_is_server_known(session);
    switch(state){
        case SSH_SERVER_KNOWN_OK:
            break; /* ok */
        case SSH_SERVER_KNOWN_CHANGED:
            fprintf(stderr,"Host key for server changed : server's one is now :\n");
            ssh_get_pubkey_hash(session,hash);
            ssh_print_hexa("Public key hash",(unsigned char*)hash,MD5_DIGEST_LEN);
            fprintf(stderr,"For security reason, connection will be stopped\n");
            ssh_disconnect(session);
            return -1;
        case SSH_SERVER_FOUND_OTHER:
            fprintf(stderr,"The host key for this server was not found but an other type of key exists.\n");
            fprintf(stderr,"An attacker might change the default server key to confuse your client"
                "into thinking the key does not exist\n"
                "We advise you to rerun the client with -d or -r for more safety.\n");
                ssh_disconnect(session);
              return -1;
        case SSH_SERVER_NOT_KNOWN:
            fprintf(stderr,"The server is unknown. Do you trust the host key ?\n");
            ssh_get_pubkey_hash(session,hash);
            ssh_print_hexa("Public key hash",(unsigned char*)hash,MD5_DIGEST_LEN);
            fgets(buf,sizeof(buf),stdin);
            if(strncasecmp(buf,"yes",3)!=0){
                ssh_disconnect(session);
                return -1;
            }
            fprintf(stderr,"This new key will be written on disk for further usage. do you agree ?\n");
            fgets(buf,sizeof(buf),stdin);
            if(strncasecmp(buf,"yes",3)==0){
                if(ssh_write_knownhost(session))
                    fprintf(stderr,"error %s\n",ssh_get_error(session));
            }
            
            break;
        case SSH_SERVER_ERROR:
            fprintf(stderr,"%s",ssh_get_error(session));
            ssh_disconnect(session);
            return -1;
    }

    /* no ? you should :) */
    auth=ssh_userauth_autopubkey(session);
    if(auth==SSH_AUTH_ERROR){
        fprintf(stderr,"Authenticating with pubkey: %s\n",ssh_get_error(session));
        return -1;
    }
    banner=ssh_get_issue_banner(session);
    if(banner){
        printf("%s\n",banner);
        free(banner);
    }
    
    
    if(auth!=SSH_AUTH_SUCCESS){
      
      
      
      
    auth=ssh_userauth_kbdint(session,NULL,NULL);
    char *name,*instruction,*prompt,*ptr;
    char buffer[128];
    int i,n;
    char echo;
    while (auth==SSH_AUTH_INFO){
        name=ssh_userauth_kbdint_getname(session);
        instruction=ssh_userauth_kbdint_getinstruction(session);
        n=ssh_userauth_kbdint_getnprompts(session);
        if(strlen(name)>0)
            printf("%s\n",name);
        if(strlen(instruction)>0)
            printf("%s\n",instruction);
        for(i=0;i<n;++i){
            prompt=ssh_userauth_kbdint_getprompt(session,i,&echo);
            if(echo){
                printf("%s",prompt);
                fgets(buffer,sizeof(buffer),stdin);
                buffer[sizeof(buffer)-1]=0;
                if((ptr=strchr(buffer,'\n')))
                    *ptr=0;
                ssh_userauth_kbdint_setanswer(session,i,buffer);
                memset(buffer,0,strlen(buffer));
            } else {
                //ptr=getpass(prompt);
                ssh_userauth_kbdint_setanswer(session,i,(char*)pt.password.c_str());
            }
        }
        auth=ssh_userauth_kbdint(session,NULL,NULL);
    }
      
      
      
      
      
        if(auth==SSH_AUTH_ERROR){
            fprintf(stderr,"authenticating with keyb-interactive: %s\n",
                    ssh_get_error(session));
            return -1;
        }
    }
    
    
    
    
    
    
    
    
    if(auth!=SSH_AUTH_SUCCESS){
 cout<<pt.password<<endl;
         if(ssh_userauth_password(session,NULL,(char*)pt.password.c_str()) != SSH_AUTH_SUCCESS){
            fprintf(stderr,"Authentication failed: %s\n",ssh_get_error(session));
            ssh_disconnect(session);
            return -1;
        }
        //memset(password,0,strlen(password));
    }
    
    
    
    
    
    
    
    ssh_say(1,"Authentication success\n");


   

    sftp=sftp_new(session);
    
    if(!sftp)
    {
        ssh_say(0,"sftp error initialising channel : %s\n",ssh_get_error(session));
        return -1;
    }
    if(sftp_init(sftp))
    {
        ssh_say(0,"error initialising sftp : %s\n",ssh_get_error(session));
        return -1;
    }
    /* the connection is made */
    


	if(sftp)
	version=sftp->server_version;
	
	
	string info="version " + ntos(version);
	fxmessage("%s\n",info.c_str());


    return 0;
}


OSFile OSVfsSftp::osreaddir ()
{

    OSFile os_file;

    SFTP_ATTRIBUTES *file=sftp_readdir(sftp,dirsftp);
    
    if(file)
    {
    os_file.name=file->name;
    	if((file->permissions & S_IFMT) == S_IFDIR)
	os_file.type = FOLDER;
	else
        os_file.type = 0;
	
	os_file.size=file->size;
	
	
	
	
    for (int i = 0; i < fieldsnum - 1; i++)
    {

	if (fields[i + 1] == "size")
	    os_file.attrib.push_back(numtostring (os_file.size));
	else if (fields[i + 1] == "owner")
	    os_file.attrib.push_back(ntos(file->uid));
	else if (fields[i + 1] == "group")
	    os_file.attrib.push_back(ntos(file->gid));       
	else if (fields[i + 1] == "accessed")
	    os_file.attrib.push_back(FXFile::time ("%H:%M %d/%m/%y", file->atime).text());
	else if (fields[i + 1] == "modified")
 	    os_file.attrib.push_back(FXFile::time ("%H:%M %d/%m/%y", file->mtime).text());  
	else if (fields[i + 1] == "mode")
	{

	    std::string str = "---------";	 

		(file->permissions & S_IRUSR) ? str[0] = 'r' : str[0] = '-';
		(file->permissions & S_IWUSR) ? str[1] = 'w' : str[1] = '-';
		(file->permissions & S_IXUSR) ? str[2] = 'x' : str[2] = '-';
		(file->permissions & S_IRGRP) ? str[3] = 'r' : str[3] = '-';
		(file->permissions & S_IWGRP) ? str[4] = 'w' : str[4] = '-';
		(file->permissions & S_IXGRP) ? str[5] = 'x' : str[5] = '-';
		(file->permissions & S_IROTH) ? str[6] = 'r' : str[6] = '-';
		(file->permissions & S_IWOTH) ? str[7] = 'w' : str[7] = '-';
		(file->permissions & S_IXOTH) ? str[8] = 'x' : str[8] = '-';

	    os_file.attrib.push_back(str);

	}
	

    }
	
	
    
    sftp_attributes_free(file);
    return os_file;
    }

    if(!sftp_dir_eof(dirsftp)){
        ssh_say(0,"error : %s\n",ssh_get_error(session));
        //return;
    }

   if(sftp_dir_close(dirsftp)){
        ssh_say(0,"Error : %s\n",ssh_get_error(session));
        //return;
    }

    os_file.name = "";
    return os_file;


}


std::string OSVfsSftp::getinitialdir( void )
{

       char* path="./";    
       return string(sftp_canonicalize_path(sftp, path));
}



int OSVfsSftp::osopendir (std::string dir)
{
    
    /* opening a directory */
  
    dirsftp=sftp_opendir(sftp,(char*)dir.c_str());
    
    if(!dirsftp) 
    {
        ssh_say(0,"Directory not opened(%s)\n",ssh_get_error(session));
        return -1;
    }
    else
    {
    this->dir=dir;
    }


}

int OSVfsSftp::mkdir (std::string dir, int mode)
{

  string file=this->dir+"/"+dir;
  
  return sftp_mkdir(sftp, (char*)file.c_str(), NULL);
}

int OSVfsSftp::copy ( OSThreadExec * te)
{
copymove(te,true);
}

int OSVfsSftp::move (OSThreadExec* te)
{
copymove(te,false);
}


int OSVfsSftp::remove (OSThreadExec* te)
{
std::vector < std::string >::iterator iter;

for ( iter = te->src.begin (); iter != te->src.end(); iter++ )
        {
	removeRecursive( *iter, te );	
	}

}

int OSVfsSftp::rename (std::string orgname, std::string newname)
{
return sftp_rename(sftp,(char*)orgname.c_str(),(char*)newname.c_str());
}




int OSVfsSftp::uploadFile (string from, string to, OSThreadExec* te)
{

//fxmessage("from %s to %s\n",from.c_str(),to.c_str());


te->act_file_name=from;
te->act_file_size=0;
te->file_size=FXFile::size(from.c_str());


fxmessage("TO=%s\n",to.c_str());

SFTP_ATTRIBUTES * attr=sftp_stat (sftp,(char*)to.c_str ());

	if (attr)
	{
	sftp_attributes_free(attr);
	
	    if (te->all == false)
	    {

		te->question = true;
		while (te->answer != true)
		    usleep (5000);
		te->answer = false;

	    }
	    if (!te->overwrite)
		return -1;	
	    if (this->removeRecursive( to, te )==-1)
		return -1;
	}
	

	


		SFTP_FILE *dst;
		int src;
		int len=1;
    		char data[131072]; //128KB
						
		src=open(from.c_str(),O_RDONLY,NULL);
		
    		dst=sftp_open(sftp,(char*)to.c_str(),O_WRONLY|O_CREAT,NULL);
		if(dst)
		{
		
		
		
			while((len=read(src,data,131072)) > 0)
			{
       				if(sftp_write(dst,data,len)!=len)
				{
            			ssh_say(0,"error writing %d bytes : %s\n",len,ssh_get_error(session));
            			return -1;
        			}
			te->act_total_size+=len;
			te->act_file_size+=len;
			if ( te->cancel == true )
        		return -1;
			
			}
		sftp_file_close(dst);
		close(src);
    		}	
}


int OSVfsSftp::downloadFile (string from, string to, OSThreadExec* te)
{

te->act_file_name=from;
te->act_file_size=0;


if (FXFile::exists (to.c_str()))
	{

	    if (te->all == false)
	    {

		te->question = true;
		while (te->answer != true)
		    usleep (5000);
		te->answer = false;

	    }
	    if (!te->overwrite)
		return -1;	
	    if (!OSFXFile::remove (to.c_str(), te))
		return -1;
	}




		SFTP_FILE *src;
		int dst;
		int len=1;
    		char data[131072]; //128KB
		
    		src=sftp_open(sftp,(char*)from.c_str(),O_RDONLY,NULL);
		if(src)
		{
				
		dst=open(to.c_str(),O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR+S_IWUSR);
		
			while((len=sftp_read(src,data,131072)) > 0)
			{
       				if(write(dst,data,len)!=len)
				{
            			ssh_say(0,"error writing %d bytes : %s\n",len,ssh_get_error(session));
            			return -1;
        			}
			te->act_total_size+=len;
			te->act_file_size+=len;
			if ( te->cancel == true )
        		return -1;
			}
		sftp_file_close(src);
		close(dst);
  		}	
	
}




int OSVfsSftp::copymove (OSThreadExec* te, bool copy)
{

unsigned long size = 0;

std::vector < std::string >::iterator iter;

fxmessage(te->options.c_str());

std::string::size_type pos = te->options.find ( "upload" );
    if ( pos == std::string::npos )
    {


	for ( iter = te->src.begin (); iter != te->src.end(); iter++ )
        {
            std::string sr = ( *iter );
            if ( FXFile::isDirectory ( sr.c_str () ) )
                sr.append ( "/" );
            local_totalsize ( sr, size );
        }

        te->total_size = size;


	for (iter = te->src.begin (); iter != te->src.end(); iter++)
  	{
	
	goLocalRecursive( *iter, this->dir, te );	
		
	
    
    	}
    }
    else
    {
    
    
    	for ( iter = te->src.begin (); iter != te->src.end(); iter++ )
        {
            totalsize ( *iter, size );
        }

        te->total_size = size;
	
    
    	for (iter = te->src.begin (); iter != te->src.end(); iter++)
  	{

	goRecursive( *iter, te->dst, te );
		
    	}
    }	



  
}



void OSVfsSftp::totalsize (std::string path, unsigned long &size)
{

//fxmessage("%s\n",path.c_str());

SFTP_ATTRIBUTES * attr=sftp_stat (sftp,(char*)path.c_str ());
SFTP_DIR *dirsftp;

 if(attr) 
 {
  if((attr->permissions & S_IFMT) == S_IFDIR)
  {

  dirsftp=sftp_opendir(sftp,(char*)path.c_str());

   if(dirsftp)
   { 

    SFTP_ATTRIBUTES *file;
    
    while(file=sftp_readdir(sftp,dirsftp))
    {

    	if (file->name[0] != '.' || (file->name[1] != '\0' && (file->name[1] != '.' || file->name[2] != '\0')))
    	{
   
    		std::string filename = path;
		filename.append ("/");
		filename.append (file->name);
 
    	        totalsize (filename, size);
	}
	
    }
   sftp_dir_close (dirsftp);
   }
  }
  else
  {
  size += (unsigned int) attr->size;
  }    
 sftp_attributes_free(attr);
 } 


}

void OSVfsSftp::goLocalRecursive ( std::string path, std::string dstdir, OSThreadExec *te )
{

    if ( te->cancel == true )
        return ;

    if ( FXFile::isDirectory ( path.c_str () ) )
    {
        struct stat status;
        struct dirent *dp;
        DIR *dirp;

        
	
        


        dirp = opendir ( path.c_str () );
	
	this->dir=dstdir;
	
	dstdir = dstdir +"/"+ FXFile::name( path.c_str() ).text();
	this->mkdir( FXFile::name( path.c_str() ).text(),0);

	//fxmessage("MKDIR = %s\n",dstdir.c_str());

        while ( ( dp = readdir ( dirp ) ) != NULL )
        {
            if ( dp->d_name[ 0 ] != '.' || ( dp->d_name[ 1 ] != '\0' && ( dp->d_name[ 1 ] != '.' || dp->d_name[ 2 ] != '\0' ) ) )
            {

                std::string file = path + "/";
                file.append ( dp->d_name );
                goLocalRecursive( file, dstdir, te );
            }
        }

        closedir ( dirp );
    }
    else
    {
        
		std::string ds = dstdir;
	   	ds.append (SEPARATOR);
	  	ds.append (FXFile::name (path.c_str()).text ());
		uploadFile(path,ds,te);

    }

}


int OSVfsSftp::removeRecursive ( std::string path, OSThreadExec *te )
{

 if ( te->cancel == true )
        return -1;
int ret=0;

SFTP_ATTRIBUTES * attr=sftp_stat (sftp,(char*)path.c_str ());
SFTP_DIR *dirsftp;

 if(attr) 
 {
  if((attr->permissions & S_IFMT) == S_IFDIR)
  {

  dirsftp=sftp_opendir(sftp,(char*)path.c_str());

   if(dirsftp)
   { 

    SFTP_ATTRIBUTES *file;
    
    while(file=sftp_readdir(sftp,dirsftp))
    {

    	if (file->name[0] != '.' || (file->name[1] != '\0' && (file->name[1] != '.' || file->name[2] != '\0')))
    	{
   
    		std::string filename = path;
		filename.append ("/");
		filename.append (file->name);
 
    	        removeRecursive(filename,te);
	}
	
    }
   sftp_dir_close (dirsftp);
   te->act_file_name=path;
   ret=sftp_rmdir(sftp,(char*)path.c_str());  
   }
  }
  else
  {
  te->act_file_name=path;
  ret=sftp_rm(sftp,(char*)path.c_str());  
  }    
 sftp_attributes_free(attr);
 } 
 
return ret; 
 
}


void OSVfsSftp::goRecursive ( std::string path, std::string dstdir, OSThreadExec *te )
{

 if ( te->cancel == true )
        return ;


SFTP_ATTRIBUTES * attr=sftp_stat (sftp,(char*)path.c_str ());
SFTP_DIR *dirsftp;

 if(attr) 
 {
  if((attr->permissions & S_IFMT) == S_IFDIR)
  {

	dstdir = dstdir +"/"+ FXFile::name( path.c_str() ).text();
	FXFile::createDirectory(dstdir.c_str(),0);

	//fxmessage("X MKDIR = %s\n",dstdir.c_str());

  dirsftp=sftp_opendir(sftp,(char*)path.c_str());

   if(dirsftp)
   { 

    SFTP_ATTRIBUTES *file;
    
    while(file=sftp_readdir(sftp,dirsftp))
    {

    	if (file->name[0] != '.' || (file->name[1] != '\0' && (file->name[1] != '.' || file->name[2] != '\0')))
    	{
   
    		std::string filename = path;
		filename.append ("/");
		filename.append (file->name);
 
    	        goRecursive(filename,dstdir,te);
	}
	
    }
   sftp_dir_close (dirsftp);
   }
  }
  else
  {
  te->file_size=attr->size;
  
  
  	downloadFile(path,dstdir+"/"+string(FXFile::name (path.c_str()).text ()),te);
  
  }    
 sftp_attributes_free(attr);
 } 
}





void OSVfsSftp::local_totalsize ( std::string path, unsigned long &size )
{

//fxmessage("%s\n",path.c_str());

    if ( FXFile::isDirectory ( path.c_str () ) )
    {
        struct stat status;
        struct dirent *dp;
        DIR *dirp;


        dirp = opendir ( path.c_str () );

        while ( ( dp = readdir ( dirp ) ) != NULL )
        {
            if ( dp->d_name[ 0 ] != '.' || ( dp->d_name[ 1 ] != '\0' && ( dp->d_name[ 1 ] != '.' || dp->d_name[ 2 ] != '\0' ) ) )
            {

                std::string file = path;
                file.append ( dp->d_name );
#ifndef WIN32
                lstat ( file.c_str (), &status );
#else
                stat ( file.c_str (), &status );
#endif
                if ( S_ISDIR ( status.st_mode ) )
                {
                    size = size + ( unsigned int ) status.st_size;
                    file.append ( "/" );
                    totalsize ( file, size );

                }
                else
                {
                    size = size + status.st_size;
                }
            }
        }

        closedir ( dirp );
    }
    else
    {
        size += FXFile::size ( path.c_str () );

    }

}











std::string OSVfsSftp::info (void)
{
string info="";
return info;
}


int OSVfsSftp::mode (std::string file)
{
  
  SFTP_ATTRIBUTES * attr=sftp_stat (sftp,(char*)file.c_str ());
        if(attr) 
	{
	return attr->permissions;
	}  

    
}

bool OSVfsSftp::mode (std::string file, unsigned int mod, bool recursive)
{

  if(!recursive)
  {

	SFTP_ATTRIBUTES * attr=sftp_stat (sftp,(char*)file.c_str ());
        if(attr) 
	{
	attr->permissions=mod;
	sftp_setstat(sftp,(char*)file.c_str (),attr);
	sftp_attributes_free(attr);
	}

  }
  else
  {
  chmodRecursive(file,mod);
  }



    
}

void OSVfsSftp::chmodRecursive(string path, unsigned int mod)
{



SFTP_ATTRIBUTES * attr=sftp_stat (sftp,(char*)path.c_str ());
SFTP_DIR *dirsftp;

 if(attr) 
 {
  if((attr->permissions & S_IFMT) == S_IFDIR)
  {
	
  attr->permissions=mod;
  sftp_setstat(sftp,(char*)path.c_str (),attr);	 
  
  dirsftp=sftp_opendir(sftp,(char*)path.c_str());

   if(dirsftp)
   { 

    SFTP_ATTRIBUTES *file;
    
    while(file=sftp_readdir(sftp,dirsftp))
    {

    	if (file->name[0] != '.' || (file->name[1] != '\0' && (file->name[1] != '.' || file->name[2] != '\0')))
    	{
   
    		std::string filename = path;
		filename.append ("/");
		filename.append (file->name);
 
    	        chmodRecursive(filename,mod);
	}
	
    }
   sftp_dir_close (dirsftp);
   }
  }
  else
  {
	attr->permissions=mod;
	sftp_setstat(sftp,(char*)path.c_str (),attr);  
  
  }    
 sftp_attributes_free(attr);
 } 



}

std::string OSVfsSftp::owner (std::string file)
{

	SFTP_ATTRIBUTES * attr=sftp_stat (sftp,(char*)file.c_str ());
        if(attr) 
	{
	string owner=ntos(attr->uid);
	sftp_attributes_free(attr);	
	return owner;
	}

return "";   
}
std::string OSVfsSftp::group (std::string file)
{
return "";   
}

bool OSVfsSftp::owner (std::string file, std::string ownername, bool recursive)
{
   
}

bool OSVfsSftp::group (std::string file, std::string groupname, bool recursive)
{
   
}

std::string OSVfsSftp::symlink (std::string path)
{
  return "";
}
bool OSVfsSftp::symlink (std::string src, std::string dst)
{

}
bool OSVfsSftp::hardlink (std::string src, std::string dst)
{
 
}


int OSVfsSftp::quit (void)
{
    if(sftp)
    ssh_disconnect(session);
    return 0;
}

EXPORTFUNCTION OSVirtualFileSystemBase* get_filelist (void)
{
    FXTRACE ((5, "PLUGIN LOAD\n"));
    return new OSVfsSftp ();
}
