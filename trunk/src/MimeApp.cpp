#include "fx.h"
#include "MimeApp.h"
#include "sharedobjects.h"
#include "CommandsFileTypesContainers.h"

#ifdef WIN32
#define SEPARATOR "\\"
#else
#include <dlfcn.h>
#define SEPARATOR "/"
#endif

#ifndef PATH_CFG
#define PATH_CFG        "/usr/local/etc/"
#endif

using namespace std;

FXDEFMAP (MimeApp) MimeAppMap[] =
{
    //________Message_Type_____________________ID____________Message_Handler_______  
	FXMAPFUNC (SEL_COMMAND, MimeApp::ID_SAVE, MimeApp::onSave),
	FXMAPFUNCS (SEL_COMMAND, MimeApp::ID_NEXT,MimeApp::ID_PREVIOUS, MimeApp::onNextPrevious),
};


FXIMPLEMENT (MimeApp, FXDialogBox, MimeAppMap, ARRAYNUMBER (MimeAppMap)) MimeApp::MimeApp (FXWindow * owner):FXDialogBox (owner, "MimeApp", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4)
{


    FXVerticalFrame *vertical = new FXVerticalFrame (this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);
    
    mime_label=new FXLabel(vertical,"");
    programsbox= new FXComboBox (vertical, 5, NULL, 0, FRAME_THICK | LAYOUT_SIDE_TOP | COMBOBOX_STATIC);
    
    
    FXHorizontalFrame *hori = new FXHorizontalFrame (vertical, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);
    FXButton *but1 = new FXButton (hori, "PREVIOUS", NULL, this,ID_PREVIOUS);
    FXButton *but2 = new FXButton (hori, "SAVE", NULL, this,ID_SAVE);
    FXButton *but3 = new FXButton (hori, "NEXT", NULL, this,ID_NEXT);

 string file = string(PATH_CFG) + string(SEPARATOR) + string("mimeapp");
 ifstream ifs (file.c_str(), ios::in);

    while (!ifs.eof ())
    {
	string tmp;
	getline (ifs, tmp);

	if (tmp == "" || tmp[0] == '#')
	    continue;
	mime_vector.push_back(mime2xml(tmp));
    }
    ifs.close ();
 iter = mime_vector.begin ();
 fill(*iter);

}



MimeApp::~MimeApp ()
{
}



long MimeApp::onSave (FXObject * sender, FXSelector sel, void *)
{
    FXTRACE ((5, "save\n"));
	string program=programsbox->getText().text();
	string mime=mime_label->getText().text();
	
	save(mime,program);

}

long MimeApp::onNextPrevious(FXObject * sender, FXSelector sel, void *)
{

	FXushort id = FXSELID(sel);

	if(id==ID_NEXT && iter+1 != mime_vector.end())
	{
	iter++;
	}
	else if(id==ID_PREVIOUS && iter != mime_vector.begin())
	{
		iter--;
	}
	
	fill(*iter);


}
void MimeApp::fill(string tmp)
{

	//string tmp=*iter;
	string mime, program;
	mime = tmp.substr (0, tmp.rfind (":"));
	program = tmp.substr (tmp.rfind (":") + 1);

	mime_label->setText(mime.c_str());

	
	//fxmessage(mime.c_str());
	
	programsbox->clearItems();
	
	std::stringstream parser (program);
	string prog;
	while(parser >> prog)
	{
	
	string command="which " + prog + " >/dev/null 2>/dev/null";
		if(system(command.c_str())==0)
		{
		//fxmessage(" ");
		//fxmessage(prog.c_str());
		programsbox->appendItem(prog.c_str());
		}

	
	}
	//fxmessage("\n");


}

void MimeApp::save(string mime, string program)
{

if(program=="")
return;

	//fxmessage("save=%s\n",mime.c_str());
	string::size_type pos=mime.find("/");
	
	string mime_major;
	string mime_minor;
	string command_name="open_";
	string command_exec=program + " {f}" ;
	
	string reg2;
	
	if (pos != string::npos)
	{
		mime_major=mime.substr(0,pos);	
		mime_minor=mime.substr(pos+1);
		//fxmessage("MIME=%s--%s\n",mime_major.c_str(),mime_minor.c_str());
		
	
		command_name+=mime_major+"_"+mime_minor;
	
		reg2 = conf->readonestring ("/OpenspaceConfig/file_types/" + mime_major + "/default");
		string command_major = conf->readonestring ("/OpenspaceConfig/commands/" + reg2 + "/exec");
		
		if(reg2=="" || (command_major!=command_exec && reg2!=command_name))
		{
		filetype_container ct=filetype_container();
		ct.load(mime);
		ct.command=command_name;
		ct.save();	
			
		}
		else
		return;
	}
	else // major mime type: audio,video,etc
	{
		command_name+=mime;
		string reg = conf->readonestring ("/OpenspaceConfig/file_types/" + mime + "/default");
		if(reg=="" || reg!=command_name)
		{
		filetype_container ct=filetype_container();
		ct.load(mime);
		ct.command=command_name;
		ct.save();			
		}
		else
		return;
	}

	if(conf->readonestring ("/OpenspaceConfig/commands/" + command_name+"/default")=="")
	{
	conf->addstring ("/OpenspaceConfig/commands", command_name, "");
	conf->addstring ("/OpenspaceConfig/commands/" + command_name, "exec", command_exec);
	}

}
