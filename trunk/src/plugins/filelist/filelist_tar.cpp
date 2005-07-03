
#ifdef WIN32
  #define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
   #define EXPORTFUNCTION extern "C"
#endif

#include <fx.h>
#include "filelist_tar.h"
#include "../../sharedobjects.h"
#include "OSFXFile.h"

 #ifdef WIN32
   #define SEPARATOR "\\"
 #else
#include <dlfcn.h>
   #define SEPARATOR "/"
 #endif

#include <fstream>
#include <sstream>

vector<string> name;

int filelist_tar::init(string **name,unsigned int **type,unsigned int **width,pathtype pt,configure *conf)
{
server=pt.server;
configure conflocal=*conf;
int size=conflocal.countxpath("/OpenspaceConfig/filelist/tar/headers/header")+1;

fields=new string[size];
fieldsnum=size;

(*name)=new string[size]; 
(*type)=new unsigned int[size];	
(*width)=new unsigned int[size];	
string wi;
(*name)[0]="name";
fields[0]="name";
if((wi=conflocal.readonestring("/OpenspaceConfig/filelist/tar/properties/name/width"))!="")
(*width)[0]=atoi(wi.c_str());
else
(*width)[0]=100;
(*type)[0]=0;
int counter=1;

display_size=conflocal.readonestring("/OpenspaceConfig/filelist/tar/properties/name/display");	
				
		if(conflocal.openxpath("/OpenspaceConfig/filelist/tar/headers/header")!=-1)
		{
			while(1)
		 	{
		 	string res=conflocal.getnextstring();
			if(res=="")break;
			
			if((wi=conflocal.readonestring("/OpenspaceConfig/filelist/tar/properties/" + res + "/width"))!="")
			{
						
			(*width)[counter]=atoi(wi.c_str());
			}
			else
			(*width)[counter]=40;
		
			(*name)[counter]=res;
			fields[counter]=res;
				if(res=="size"){(*type)[counter]=1;}
				else{(*type)[counter]=0;}
				
			
			counter++;
			}
		}



 return size;
}


osfile filelist_tar::osreaddir()
{
fxmessage("READ\n");
	osfile os_file;
	
	
	  if ( pipe != NULL)
          {
   	  string line;
                if(fgets(readbuf, sizeof(readbuf), pipe)!= NULL)
		{
		
			
			string path=readbuf;
			path=path.substr(0,path.length()-1);
			string s=path.substr(path.length()-1,1);
			fxmessage(s.c_str());
			
			if(s=="/")
			os_file.type=FOLDER;
			else
			os_file.type=0;
			
				os_file.name=path;
				os_file.size=300;			
				os_file.attrib=new string[fieldsnum];
				os_file.attrib[0]=numtostring(os_file.size);
 				os_file.attrib[1]="";
				return os_file;
			
		}
	
	pclose(pipe);	
	  }
os_file.name="";
return os_file;	
	
	
}


int filelist_tar::osopendir(string dir)
{
fxmessage("OPEN\n");
this->dir=dir;
string command="tar tjf " + server ;

pipe = popen(command.c_str(), "r");

}

int filelist_tar::mkdir(string dir,int mode)
{

}

int filelist_tar::copy(thread_elem* te)
{
	
}

int filelist_tar::move(thread_elem* te)
{
	
}


int filelist_tar::remove(thread_elem* te)
{

}

int filelist_tar::rename(string orgname,string newname)
{

}


int filelist_tar::copymove(thread_elem* te,bool copy)
{


}



void  filelist_tar::totalsize(string path,unsigned long &  size)
{
	
 }
 
string filelist_tar::info(void)
{  
}

 
unsigned int filelist_tar::mode(string file)
{
}

bool filelist_tar::mode(string file,unsigned int mod,bool recursive)
{

}

int filelist_tar::quit(void)
{
return 0;
}
 
EXPORTFUNCTION filelist_base *get_filelist (void) { fxmessage("zwracamy"); return new filelist_tar(); }


