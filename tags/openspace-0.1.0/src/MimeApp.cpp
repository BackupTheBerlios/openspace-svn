#include "fx.h"
#include "MimeApp.h"
#include "sharedobjects.h"
#include "sharedFunctions.h"

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
	FXMAPFUNCS (SEL_COMMAND, MimeApp::ID_NEXT,MimeApp::ID_PREVIOUS, MimeApp::onNextPrevious),
	FXMAPFUNC (SEL_CLOSE,0, MimeApp::onClose),
};


FXIMPLEMENT (MimeApp, FXDialogBox, MimeAppMap, ARRAYNUMBER (MimeAppMap)) MimeApp::MimeApp (FXWindow * owner):FXDialogBox (owner, "MimeApp", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4)
{


    FXVerticalFrame *vertical = new FXVerticalFrame (this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);
    
    mime_label=new FXLabel(vertical,"");
    programsbox= new FXComboBox (vertical, 20, NULL, 0, FRAME_THICK | LAYOUT_SIDE_TOP | COMBOBOX_STATIC);
    programsbox->setNumVisible(10);
    
    FXHorizontalFrame *hori = new FXHorizontalFrame (vertical, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);
    FXButton *but1 = new FXButton (hori, "PREVIOUS", NULL, this,ID_PREVIOUS);
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


long MimeApp::onClose (FXObject * sender, FXSelector sel, void *ptr)
{
string program=programsbox->getText().text();
string mime=mime_label->getText().text();	
save(mime,program);

FXDialogBox::onCmdAccept(sender,sel,ptr);

}


MimeApp::~MimeApp ()
{
}

void MimeApp::doAutomaticConfiguration(void)
{
vector <string>::iterator iter;
			
		for (iter=mime_vector.begin(); iter != mime_vector.end(); iter++)
    		{
			fill(*iter);
			save(mime_label->getText().text(),programsbox->getText().text());
		}
	

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
	
	string program=programsbox->getText().text();
	string mime=mime_label->getText().text();	
	save(mime,program);
	
	
	fill(*iter);


}
void MimeApp::fill(string tmp)
{
	string mime, program;
	mime = tmp.substr (0, tmp.rfind (":"));
	program = tmp.substr (tmp.rfind (":") + 1);

	mime_label->setText(mime.c_str());
	
	programsbox->clearItems();
	
	std::stringstream parser (program);
	string prog;
	while(parser >> prog)
	{
	
	string command="which " + prog + " >/dev/null 2>/dev/null";
		if(system(command.c_str())==0)
		{
		programsbox->appendItem(prog.c_str());
		}

	
	}
	programsbox->appendItem("");


}

void MimeApp::save(string mime, string program)
{

if(program=="")
return;

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
		string cmd = conf->readonestring ("/OpenspaceConfig/commands/" + reg + "/exec"); 
		
		if(reg=="" || cmd!=command_exec)
		{
		
		filetype_container ct=filetype_container();
		ct.load(mime);
		ct.command=command_name;
		ct.save();			
		}
		else
		return;
	}

	command_container cm=command_container(command_name,command_exec);
	cm.save();

}
