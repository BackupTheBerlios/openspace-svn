
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

#include <string>
#include <vector>
#include <iostream>

using namespace std;


OSVirtualFileSystemInfo OSVfsSftp::setup (void)
{
	OSVirtualFileSystemInfo v;
	
	v.vfsheaders.push_back(OSVirtualFileSystemHeader("name"));
	v.information="SFTP file list - default plugin";
	v.version="1";
	
    return v;	
}

int OSVfsSftp::init (long id, std::vector<std::string> *vector_name, OSPathType pt, OSConfigure * conf)
{
    this->id=id;
    fields = (*vector_name);



    dirsftp=NULL;

    
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
        //M auth=auth_kbdint(session);
        if(auth==SSH_AUTH_ERROR){
            fprintf(stderr,"authenticating with keyb-interactive: %s\n",
                    ssh_get_error(session));
            return -1;
        }
    }
    if(auth!=SSH_AUTH_SUCCESS){
        password=getpass("Password : ");
        if(ssh_userauth_password(session,NULL,password) != SSH_AUTH_SUCCESS){
            fprintf(stderr,"Authentication failed: %s\n",ssh_get_error(session));
            ssh_disconnect(session);
            return -1;
        }
        memset(password,0,strlen(password));
    }
    ssh_say(1,"Authentication success\n");


     //do_sftp(session);    
    //if(!sftp)
        //do_cleanup();
    

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
    
    //fxmessage("name=%s type=%x\n",file->name,file->permissions);
    
    sftp_attributes_free(file);
    return os_file;
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
  
    if(dirsftp)
    sftp_dir_close(dirsftp);   

    
    /* opening a directory */
  
    dirsftp=sftp_opendir(sftp,(char*)dir.c_str());
    
    if(!dirsftp) 
    {
        ssh_say(0,"Directory not opened(%s)\n",ssh_get_error(session));
        return -1;
    }


    /*
    
     reading the whole directory, file by file 
    while((file=sftp_readdir(sftp,dir))){
        ssh_say(0,"%30s(%.8lo) : %.5d.%.5d : %.10lld bytes\n",file->name,file->permissions,file->uid,file->gid,file->size);
        sftp_attributes_free(file);
    }
     when file=NULL, an error has occured OR the directory listing is end of file 
    if(!sftp_dir_eof(dir)){
        ssh_say(0,"error : %s\n",ssh_get_error(session));
        return -1;
    }
    if(sftp_dir_close(dir)){
        ssh_say(0,"Error : %s\n",ssh_get_error(session));
        return -1;
    }

*/

}

int OSVfsSftp::mkdir (std::string dir, int mode)
{
  
}

int OSVfsSftp::copy ( OSThreadExec * te)
{

}

int OSVfsSftp::move (OSThreadExec* te)
{

}


int OSVfsSftp::remove (OSThreadExec* te)
{



}

int OSVfsSftp::rename (std::string orgname, std::string newname)
{

}


int OSVfsSftp::copymove (OSThreadExec* te, bool copy)
{

  
}



void OSVfsSftp::totalsize (std::string path, unsigned long &size)
{

   
}

std::string OSVfsSftp::info (void)
{
    return "";
}


int OSVfsSftp::mode (std::string file)
{
    
}

bool OSVfsSftp::mode (std::string file, unsigned int mod, bool recursive)
{
   
}

std::string OSVfsSftp::owner (std::string file)
{
    
}
std::string OSVfsSftp::group (std::string file)
{
  
}

bool OSVfsSftp::owner (std::string file, std::string ownername, bool recursive)
{
   
}

bool OSVfsSftp::group (std::string file, std::string groupname, bool recursive)
{
   
}

std::string OSVfsSftp::symlink (std::string path)
{
  
}
bool OSVfsSftp::symlink (std::string src, std::string dst)
{

}
bool OSVfsSftp::hardlink (std::string src, std::string dst)
{
 
}


int OSVfsSftp::quit (void)
{
    ssh_disconnect(session);
    return 0;
}

EXPORTFUNCTION OSVirtualFileSystemBase* get_filelist (void)
{
    FXTRACE ((5, "PLUGIN LOAD\n"));
    return new OSVfsSftp ();
}
