#include "CommandsFileTypesContainers.h"
#include "sharedobjects.h"


filetype_container::filetype_container(string name,string command):name(name),command(command)
{
}

void filetype_container::save(void)
{
fxmessage("\nNAME=%s COMMAND=%s\n",name.c_str(),command.c_str());

if(name=="")
return;

	string::size_type pos=name.find("/");
	string path;
	string mime_major;
	string mime_minor;
	if (pos != string::npos)
	{
		mime_major=name.substr(0,pos);	
		mime_minor=name.substr(pos+1);
		string ret;
				if(conf->readonestring ("/OpenspaceConfig/file_types/"+ mime_major,ret)==false)
				conf->addstring ("/OpenspaceConfig/file_types", mime_major,"");
				
				if(conf->readonestring ("/OpenspaceConfig/file_types/"+ mime_major+"/types",ret)==false)
				conf->addstring ("/OpenspaceConfig/file_types/" + mime_major ,"types","");	
		
		if(conf->readonestring ("/OpenspaceConfig/file_types/"+ mime_major+"/types/"+mime_minor,ret)==false)
				conf->addstring ("/OpenspaceConfig/file_types/" + mime_major+"/types" ,mime_minor,"");	
		
		path="/OpenspaceConfig/file_types/" + mime_major + "/types/"+mime_minor;
		
	

			
	}
	else  // major mime type: audio,video,etc
	{
	string ret;
	if(conf->readonestring ("/OpenspaceConfig/file_types/"+ name,ret)==false)
			conf->addstring ("/OpenspaceConfig/file_types/" ,name,"");	
	
	path="/OpenspaceConfig/file_types/" + name;
	}
	
	if(command!="")
	  if(!conf->saveonestring (path+"/default",command))
			{
			conf->addstring(path ,"default",command);
			}
	
	
		
	
	if(commands.size()>0)
	{
	conf->removestring("/OpenspaceConfig/file_types/" + mime_major + "/types/"+mime_minor+"/commands");
	vector <string>::iterator iter;
	conf->addstring("/OpenspaceConfig/file_types/" + mime_major + "/types/"+mime_minor,"commands","");
		for(iter=commands.begin();iter!=commands.end();iter++)
		{
		conf->addstring("/OpenspaceConfig/file_types/" + mime_major + "/types/"+mime_minor+"/commands","command",iter->c_str());
		}
	
	}
	
		
/*
	if ( conflocal3.openxpath ("/OpenspaceConfig/" + rep + "/commands/command") != -1)
				{
		
				    while (1)
				    {

					string res = conflocal3.getnextstring ();
					if (res == "")
					    break;
*/
}

command_container::command_container(string name, string exec):name(name),exec(exec)
{
}

void command_container::save(void)
{

string com = name;
	string value = exec;
	if(type!="INTERNAL" && type!="PLUGIN")
	{
		if (!conf->saveonestring ("/OpenspaceConfig/commands/" + com + "/exec", value))
		{
	   	 conf->addstring ("/OpenspaceConfig/commands", com, "");
	   	 if(exec!="")
	   	 conf->addstring ("/OpenspaceConfig/commands/" + com, "exec", value);
		}
	}
	string options;
	if (capture)
	    options += " capture";
	if (rescan)
	    options += " rescan";

	if (options != "")
	{
	    if (!conf->saveonestring ("/OpenspaceConfig/commands/" + com + "/options", options))
		conf->addstring ("/OpenspaceConfig/commands/" + com, "options", options);
	}

	if (text != "")
	{
	    if (!conf->saveonestring ("/OpenspaceConfig/commands/" + com + "/text", text))
		conf->addstring ("/OpenspaceConfig/commands/" + com, "text", text);
	}


	if (icon != "")
	{
	    if (!conf->saveonestring ("/OpenspaceConfig/commands/" + com + "/icon", icon))
		conf->addstring ("/OpenspaceConfig/commands/" + com, "icon", icon);
	}



}

