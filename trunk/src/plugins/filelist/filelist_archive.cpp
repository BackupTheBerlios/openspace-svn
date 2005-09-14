#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif
#include "filelist_archive.h"


vfs filelist_archive::setup (void)
{
	vfs v;
	
	v.vfsheaders.push_back(vfsheader_container("name"));
	v.information="tar.bz2 and tar.gz archive - default plugin";
	v.version="1";
	v.type="vfs";
return v;	
	
}


int filelist_archive::osopendir (string dir)
{
fxmessage("OPENDIR");
this->dir=dir;
iter=files.begin();
}
osfile filelist_archive::osreaddir (void)
{

osfile os_file;

while(iter!=files.end())
{
os_file.name=*iter;
	string modfile=os_file.name.substr(0,os_file.name.length () - 1);
	//fxmessage("\nMOD=%s",modfile.c_str());
	string::size_type pos=modfile.rfind("/");
	if (pos != string::npos)
	{
		string dirf="/"+os_file.name.substr(0,pos);
		
		//fxmessage("\nDIR=%s  DIRF=%s",dir.c_str(),dirf.c_str()); 
		if(dirf!=dir)
		{
		iter++;
		continue;
		}
		else
		os_file.name=os_file.name.substr(pos+1);
		
		
		
	}
	else if(dir!="/")
	{
	iter++;
	continue;
	}

	    os_file.type = 0;
	    os_file.size = 300;
	  
	    
	    string s = os_file.name.substr (os_file.name.length () - 1, 1);

	    if (s == "/")
	    {
	        os_file.name = os_file.name.substr (0,os_file.name.length () - 1);
		os_file.type = FOLDER;
	    }
	    else
		os_file.type = 0;
	    
	    iter++;
	    //fxmessage("\nN=%s",os_file.name.c_str());  
	    return os_file;
}		    


    
    os_file.name = "";
    return os_file;

}
int filelist_archive::mkdir (string dir, int mode)
{
}
int filelist_archive::copy (thread_elem * te)
{
filesoperation("copy",te);
}

int filelist_archive::move (thread_elem * te)
{
filesoperation("move",te);
}
int filelist_archive::remove (thread_elem * te)
{
filesoperation("remove",te);
}

int filelist_archive::filesoperation(string typeoperation,thread_elem * te)
{

fxmessage("ARCHIVE COPY");

 vector < string >::iterator iter;
 
	string::size_type pos = te->options.find ("upload");
	if (pos != string::npos && typeoperation!="remove")
	{
		
		string file_list;
		vector <string> file_list_vec;
   		 for (iter = te->src.begin (); iter != te->src.end(); iter++)
   		 {
			string sr = (*iter);
			file_list_vec.push_back(sr);
			sr=sr.substr(1,sr.length()-1);
			file_list+=" \"" +  sr + "\"";
			
			
		 }	
			if(dir=="/")
			{
			string command="cd " +te->dst +" && tar xf" + type + " " + archive_filename + " " + file_list;
			system(command.c_str());
			fxmessage("\nCOMMAND=%s\n",command.c_str());
			}
			else
			{
			
			string tmpdir;
			string destdir;
			tmpdir=FXFile::time ("%S%H%M%d%m%y", FXFile::now()).text ();
			destdir=string("/tmp/openspace/tmp") + tmpdir;
			FXFile::createDirectory(destdir.c_str(),0);
			
			
			string command="cd " +destdir +" && tar xf" + type + " " + archive_filename + " " + file_list;
			system(command.c_str());
			fxmessage("\nCOMMAND=%s\n",command.c_str());
				for (iter = te->src.begin (); iter != te->src.end(); iter++)
   				{	
				string sr = (*iter);
				sr=sr.substr(1,sr.length()-1);
				string tmpfile=destdir+"/"+sr;
				string d=te->dst.c_str() + string("/")+FXFile::name(iter->c_str()).text();
				FXFile::move(tmpfile.c_str(),d.c_str());				
				}	
			FXFile::remove(destdir.c_str());	
			}
		
		if(typeoperation=="move")
		{
		
		thread_elem *el2 = new thread_elem (this, "remove", "", file_list_vec);
		this->remove(el2);
		delete el2;
		
		}
		
		
	}
	else //download
	{
		string decompress,compress,archive_filename_decompressed;
		if(type=="z")
		{
			decompress="gunzip";
			compress="gzip";
			archive_filename_decompressed=archive_filename.substr(0,archive_filename.length()-3);
		}
		else //bzip2
		{
			decompress="bunzip2";
			compress="bzip2";
			archive_filename_decompressed=archive_filename.substr(0,archive_filename.length()-4);
		}
		
		string command=decompress + " " + archive_filename;
		system(command.c_str());
		
		if(typeoperation=="remove")
		{
			string file_list;
		
   			 for (iter = te->src.begin (); iter != te->src.end(); iter++)
   			 {
			string sr = (*iter);
			sr=sr.substr(1,sr.length()-1);
			file_list+=" \"" +  sr + "\"";	
			
			vector <string>::iterator searchit=find(files.begin(), files.end(), sr);
				if (files.end()!=searchit)// Search the list.
					{
					files.erase(searchit);	
					}
					
			 }	
			command="tar --delete -f " + archive_filename_decompressed + " " +file_list;
			fxmessage("COMMAND=%s\n",command.c_str());
			system(command.c_str());
		
		}
		else
		{
		
			 for (iter = te->src.begin (); iter != te->src.end(); iter++)
   			 {
			 string file;
			 string sr = (*iter);
			 string dir=FXFile::directory(sr.c_str()).text();
			 

			 
			 
				if(this->dir=="/")
				{
				file=FXFile::name(sr.c_str()).text();
				
					if (files.end()==find(files.begin(), files.end(), file))// Search the list.
					{
					string dirtest=dir+"/"+FXFile::name(file.c_str()).text();
					fxmessage("\n\nDIRTEST=%s\n\n",dirtest.c_str());
					
						
						if(FXFile::isDirectory(dirtest.c_str()))						
						files.push_back(file+"/");
						else
						files.push_back(file);	
					}
					else	
					{
					command="cd " +dir + " && tar --delete -f " + archive_filename_decompressed + " " +file;
					fxmessage("COMMAND=%s\n",command.c_str());
					system(command.c_str());
					}
				
				command="cd " +dir + " && tar rf " + archive_filename_decompressed + " " +file;
				system(command.c_str());
				fxmessage("COMMAND=%s\n",command.c_str());
				}
				else
				{
			 			
				string reldir=this->dir.substr(1,this->dir.length()-1);
				file=reldir +"/"+ FXFile::name(sr.c_str()).text();	
				
				string tmpdir;
				string destdir;
				tmpdir=FXFile::time ("%S%H%M%d%m%y", FXFile::now()).text ();
				destdir=string("/tmp/openspace/tmp") + tmpdir;
			
				FXFile::createDirectory(destdir.c_str(),0);
				command="cd "+destdir + " && mkdir -p " + reldir;
				system(command.c_str());
				string d=destdir+"/" + reldir + "/" + FXFile::name(sr.c_str()).text();
				
				if(typeoperation=="copy")
				FXFile::copy(sr.c_str(),d.c_str());
				else if(typeoperation=="move")
				{
				fxmessage("MMMMMMMMMMMMMMMMMMMMMMMM");
				FXFile::move(sr.c_str(),d.c_str());
				}

				
				
					if (files.end()==find(files.begin(), files.end(), file))// Search the list.
					{
					string dirtest=dir+"/"+FXFile::name(file.c_str()).text();
					fxmessage("\n\nDIRTEST=%s\n\n",dirtest.c_str());
					
						
						if(FXFile::isDirectory(dirtest.c_str()))						
						files.push_back(file+"/");
						else
						files.push_back(file);	
					}
					else
					{	
					command="cd " +destdir + " && tar --delete -f " + archive_filename_decompressed + " " +file;
					fxmessage("COMMAND=%s\n",command.c_str());
					system(command.c_str());
					}
				
				command="cd " + destdir + " && tar rf " + archive_filename_decompressed + " " +file;
				system(command.c_str());
				fxmessage("COMMAND=%s\n",command.c_str());
				
	
				FXFile::remove(destdir.c_str());
				
				}
				
							
		 	}
		}	
		command=compress + " " + archive_filename_decompressed;
		system(command.c_str());
		fxmessage("COMMAND=%s\n",command.c_str());	
			
	}
	
te->end=true;


}


int filelist_archive::rename (string orgname, string newname)
{
}
int filelist_archive::init (vector < string > *vector_name, pathtype pt, configure * conf)
{
archive_filename = pt.server;


string  command;

if ((archive_filename.length () >= 6 && archive_filename.substr (archive_filename.length () - 6, 6) == "tar.gz") ||(archive_filename.length () >= 3 && archive_filename.substr (archive_filename.length () - 3, 3) == "tgz"))
type="z";
else
type="j";

command="tar tf" + type + " " + archive_filename;
fxmessage("COMMAND=%s\n",command.c_str());

pipe = popen(command.c_str(), "r");

if (pipe != NULL)
    {
	string line;
	while (fgets (readbuf, sizeof (readbuf), pipe) != NULL)
	{


	    string path = readbuf;
	    path = path.substr (0, path.length () - 1);
	    files.push_back(path);
	   
	}

	pclose (pipe);
    }
    
iter=files.begin();

}
int filelist_archive::mode (string file)
{
}
string filelist_archive::owner (string file)
{
return "";
}
string filelist_archive::group (string file)
{
return "";
}
bool filelist_archive::mode (string file, unsigned int, bool recursive)
{
}
bool filelist_archive::owner (string file, string, bool recursive)
{
}
bool filelist_archive::group (string file, string, bool recursive)
{
}
string filelist_archive::info (void)
{
return "";
}
void filelist_archive::totalsize (string path, unsigned long &size)
{
}
string filelist_archive::symlink (string path)
{
return "";
}
bool filelist_archive::symlink (string src, string dst)
{
}
bool filelist_archive::hardlink (string src, string dst)
{
}

int filelist_archive::quit (void)
{
}
EXPORTFUNCTION filelist_base *get_filelist (void)
{
    FXTRACE ((5, "PLUGIN LOAD\n"));
    return new filelist_archive ();
}
