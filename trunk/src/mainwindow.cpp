#include <string>
#include <vector>
#include <map>
using namespace std;


 #include "fx.h"
// #include "FXPNGIcon.h"
 #include "thread_elem.h"
 #include "filelist.h"
 #include "mainwindow.h"
 #include "sharedobjects.h"
 #include "informationpanel.h"
 #include "configure.h"
 #include "oslistitem.h"



 #ifdef WIN32
   #define SEPARATOR "\\"
 #else
#include <dlfcn.h>
   #define SEPARATOR "/"
 #endif


using namespace FX;







FXDEFMAP(MainWindow) MainWindowMap[]=
     {
     //________Message_Type_____________________ID____________Message_Handler_______  
     FXMAPFUNC(SEL_COMMAND, MainWindow::ID_DIRCH,   MainWindow::onChangeDir),
    // FXMAPFUNC(SEL_ENTER, MainWindow::ID_DIRCH,   MainWindow::onListNextDir),
    // FXMAPFUNC(SEL_LEAVE, MainWindow::ID_DIRCH,   MainWindow::onListNextDir),
     FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, MainWindow::ID_DIR,   MainWindow::onListDirs),
     FXMAPFUNC(SEL_COMMAND, MainWindow::ID_DIR,   MainWindow::onOpenDir),
     FXMAPFUNC(SEL_COMMAND, MainWindow::ID_CONFIGURE,   MainWindow::onConfigure),     
     FXMAPFUNC(SEL_COMMAND, MainWindow::ID_ABOUT,   MainWindow::onAbout),
     FXMAPFUNCS(SEL_LEFTBUTTONRELEASE,MainWindow::ID_TOLEFT, MainWindow::ID_TORIGHT,   MainWindow::onChangeList),
     FXMAPFUNCS(SEL_COMMAND, MainWindow::ID_OVERWRITE, MainWindow::ID_SKIP_ALL,   MainWindow::onOverwrite),     
     FXMAPFUNC(SEL_COMMAND, MainWindow::ID_NEWFRAME,   MainWindow::onNewFrame),
     FXMAPFUNC(SEL_COMMAND, MainWindow::ID_NEW_NETWORK,   MainWindow::onNewNetworkFrame),     
     FXMAPFUNCS(SEL_COMMAND, 666,667,   MainWindow::onNotify),
     FXMAPFUNC(SEL_TIMEOUT,MainWindow::ID_TIMER,MainWindow::onTimer),
     FXMAPFUNCS(SEL_COMMAND,MainWindow::ID_CHANGE_VIEW_SMALL,MainWindow::ID_CHANGE_VIEW_DETAILS,MainWindow::onChangeView),
     FXMAPFUNC(SEL_COMMAND,MainWindow::ID_CANCEL,MainWindow::cancel), 
     FXMAPFUNC(SEL_COMMAND,MainWindow::ID_COMMANDS_SHOW,MainWindow::commandsShow),
     FXMAPFUNC(SEL_UPDATE,0,MainWindow::update),  
     };    
     FXIMPLEMENT(MainWindow,FXMainWindow,MainWindowMap,ARRAYNUMBER(MainWindowMap))


//load icon from file and put in the array
void MainWindow::loadicon(string src)
{
static unsigned int counter=0;


FXFileStream stream;

	osicons[counter]=new FXGIFIcon(getApp(),NULL);
		if(stream.open(src.c_str(),FXStreamLoad))
		{
		osicons[counter]->loadPixels(stream);
		stream.close();
		osicons[counter]->create();
		}
counter++;

}


 // read color definied as FF FF FF and return FXColor  
FXColor readcolor(string col)
{
if(col=="")
return FXRGB(0,0,0);
	FXString color=col.c_str();
	int cc=0;	
	int z1=0;
	FXString rgbcolor[3];			
			while(z1!=-1)
			{
			z1=color.find(" ",0);			
			int i=0;
			if(z1==-1)
			rgbcolor[cc].append(color.text(),color.length());
			else
			rgbcolor[cc].append(color.text(),z1);
		
			color.remove(0,z1+1);
			cc++;		
			}	
							
	return FXRGB(FXIntVal(rgbcolor[0]),FXIntVal(rgbcolor[1]),FXIntVal(rgbcolor[2]));
			
}


//the same as above but defualt color is white
 FXColor readcolor2(string col)
{
if(col=="")
return FXRGB(255,255,255);
	FXString color=col.c_str();
	int cc=0;	
	int z1=0;
	FXString rgbcolor[3];			
			while(z1!=-1)
			{
			z1=color.find(" ",0);			
			int i=0;
			if(z1==-1)
			rgbcolor[cc].append(color.text(),color.length());
			else
			rgbcolor[cc].append(color.text(),z1);
		
			color.remove(0,z1+1);
			cc++;		
			}	
							
	return FXRGB(FXIntVal(rgbcolor[0]),FXIntVal(rgbcolor[1]),FXIntVal(rgbcolor[2]));
			
}


string MainWindow::parseDir(string dir)
{
	if(dir=="{homedir}")
	return FXFile::getHomeDirectory().text();
	else
	return dir;
}
     
//-----MAIN WINDOW---------------------------------------------------------------------------------------------------------------------------         
     
MainWindow::MainWindow(FXApp * a) : FXMainWindow(a,"openspace",NULL,NULL,DECOR_ALL|LAYOUT_FIX_WIDTH,0,0,600,400,0,0)
{


  new FXToolTip(getApp(),TOOLTIP_NORMAL);
pane=NULL;
filemenu=NULL;


conf=new configure();
pref=new preferences(this);

string res=conf->readonestring("/OpenspaceConfig/version");
FXTRACE((1,"VERSION %s\n",res.c_str()));

int w=atoi(conf->readonestring("/OpenspaceConfig/mainwindow/width").c_str());
int h=atoi(conf->readonestring("/OpenspaceConfig/mainwindow/height").c_str());

resize(w,h);
string s=conf->readonestring("/OpenspaceConfig/path") + "icons/directory.gif";

		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/directory.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/big_directory.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/unknown.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/big_unknown.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/left.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/close.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/right.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/plus.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/execute.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/network.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/password.gif"); //10
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/user.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/smallicons.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/bigicons.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/details.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/pattern.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/executable.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/symlink.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/foxmini.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/foxbig.gif");
		loadicon(conf->readonestring("/OpenspaceConfig/path") + "icons/configure.gif"); //20
osicons[21]=NULL;



// read icons for file types
if(conf->openxpath("/OpenspaceConfig/file_types")!=-1)
{
 
 while(1)
 {
 string res=conf->getnextnode();
 	if(res=="")break;
 string res2=conf->readonestring("/OpenspaceConfig/file_types/" + res + "/icon");
 string colorstr=conf->readonestring("/OpenspaceConfig/file_types/" + res + "/color");
 FXColor color=readcolor(colorstr);
 
  string backcolorstr=conf->readonestring("/OpenspaceConfig/file_types/" + res + "/backcolor");
 FXColor backcolor=readcolor2(backcolorstr);
 
 	if(res2!="")
	{
	FXFileStream stream;
	string path=conf->readonestring("/OpenspaceConfig/path") + "/icons/" + res2;
	FXIcon *osicon=NULL;
	//string name=res2;
	//if(name.substr(name.length()-3,3)=="gif")
	osicon=new FXGIFIcon(getApp(),NULL);
	//else if(name.substr(name.length()-3,3)=="png")
	//osicon=new FXPNGIcon(getApp(),NULL);
	
		if(stream.open(path.c_str(),FXStreamLoad))
		{
		osicon->loadPixels(stream);
		stream.close();
		osicon->create();
		file_type_settings[res]=new file_type(osicon,color,backcolor);
		}
	
 	

	}


 }


}






FXVerticalFrame *ff=new FXVerticalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y);

controlframe=new FXVerticalFrame(ff,LAYOUT_FILL_X|FRAME_THICK,0,0,0,0,0,0,0,0);


splitter = new FXSplitter(ff,LAYOUT_FILL_X|SPLITTER_TRACKING|LAYOUT_FILL_Y);

left=new FXVerticalFrame(splitter,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|LAYOUT_FIX_WIDTH,0,0,w/2);
right=new FXVerticalFrame(splitter,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN);

leftcontrolframe=new FXVerticalFrame(left,LAYOUT_FILL_X);
leftframe=new FXVerticalFrame(left,LAYOUT_FILL_X|LAYOUT_FILL_Y);

rightcontrolframe=new FXVerticalFrame(right,LAYOUT_FILL_X);
rightframe=new FXVerticalFrame(right,LAYOUT_FILL_X|LAYOUT_FILL_Y);

FXHorizontalFrame *buttonsframe=new FXHorizontalFrame(controlframe,LAYOUT_FILL_X|FRAME_THICK,0,0,0,0,0,0,0,0);
new FXButton (buttonsframe,"",osicons[7],this,MainWindow::ID_COMMANDS_SHOW,FRAME_THICK,0,0,0,0,0,0,0,0);
new FXButton (buttonsframe,"",osicons[0],this,MainWindow::ID_NEWFRAME,FRAME_THICK,0,0,0,0,0,0,0,0);
new FXButton (buttonsframe,"",osicons[9],this,MainWindow::ID_NEW_NETWORK,FRAME_THICK,0,0,0,0,0,0,0,0);
new FXSeparator(buttonsframe,SEPARATOR_NONE );
new FXSeparator(buttonsframe,SEPARATOR_NONE );
new FXButton (buttonsframe,"",osicons[12],this,MainWindow::ID_CHANGE_VIEW_SMALL,FRAME_THICK,0,0,0,0,0,0,0,0);
new FXButton (buttonsframe,"",osicons[13],this,MainWindow::ID_CHANGE_VIEW_BIG, FRAME_THICK,0,0,0,0,0,0,0,0);
new FXButton (buttonsframe,"",osicons[14],this,MainWindow::ID_CHANGE_VIEW_DETAILS,FRAME_THICK,0,0,0,0,0,0,0,0);
new FXSeparator(buttonsframe,SEPARATOR_NONE );
new FXSeparator(buttonsframe,SEPARATOR_NONE );
new FXButton (buttonsframe,"\tconfiguration",osicons[20],this,MainWindow::ID_CONFIGURE,FRAME_THICK,0,0,0,0,0,0,0,0);
new FXButton (buttonsframe,"",osicons[18],this,MainWindow::ID_ABOUT,FRAME_THICK,0,0,0,0,0,0,0,0);


		
		string dir=parseDir(conf->readonestring("/OpenspaceConfig/leftdir/dir"));		
		string type=conf->readonestring("/OpenspaceConfig/leftdir/type");
		pathtype pt(dir,type);		
		left_frame=new Frame(leftcontrolframe,leftframe,pt,this,-1,&thread_vec,&file_type_settings,osicons);
		
		dir=parseDir(conf->readonestring("/OpenspaceConfig/rightdir/dir"));
		type=conf->readonestring("/OpenspaceConfig/rightdir/type");
		pathtype pt2(dir,type);
		right_frame=new Frame(rightcontrolframe,rightframe,pt2,this,-1,&thread_vec,&file_type_settings,osicons);
					

left_frame->f->filelist_opposite=right_frame->f;
right_frame->f->filelist_opposite=left_frame->f;

infoframe=new FXVerticalFrame(ff,LAYOUT_FILL_X);
networkframe=NULL;

getApp()->addTimeout(this,ID_TIMER,200);

}

//---------------------------------------------------- 
// open configure window
long MainWindow::onConfigure(FXObject * sender,FXSelector sel,void*)
{
FXTRACE((5,"CONFIGURE\n"));
	if(pref->shown())
	pref->hide();
	else
	pref->show(PLACEMENT_OWNER);
}


//about
long MainWindow::onAbout(FXObject * sender,FXSelector sel,void*)
{
  FXMessageBox about(this,"About Openspace","Openspace File Browser V0.0.1 ALPHA\n\nUsing the FOX C++ GUI Library (http://www.fox-tookit.org)\n\nCopyright (C) Mateusz Dworak (compbatant@t-nas.org)",osicons[19],MBOX_OK|DECOR_TITLE|DECOR_BORDER);
  about.execute();
} 
 
 
 //change view type: big/small/detailes 
long MainWindow::onChangeView(FXObject * sender,FXSelector sel,void*)
{
FXushort id = FXSELID(sel);


filelist *f;
			if(left_frame->f->active)
			f=left_frame->f;
			else		
			f=right_frame->f;

	if(id==ID_CHANGE_VIEW_SMALL)
	{
	f->setFont(f->captionfont1);
	f->setListStyle(ICONLIST_EXTENDEDSELECT|ICONLIST_MINI_ICONS|ICONLIST_COLUMNS);
	}
	else if(id==ID_CHANGE_VIEW_BIG)
	{
	f->setFont(f->captionfont);
	f->setListStyle(ICONLIST_EXTENDEDSELECT|ICONLIST_BIG_ICONS|ICONLIST_COLUMNS);
	}
	else if(id=ID_CHANGE_VIEW_DETAILS)
	{
	f->setFont(f->captionfont1);
	f->setListStyle(ICONLIST_EXTENDEDSELECT|ICONLIST_DETAILED|ICONLIST_COLUMNS);
	}
}   


//new frame
long MainWindow::onNewFrame(FXObject * sender,FXSelector,void* ptr)
{
string dir;
string type;
string str_server;
string str_user;
	if(networkframe==NULL)
 	{
	dir=parseDir(conf->readonestring("/OpenspaceConfig/defaultdir/dir"));
	type=conf->readonestring("/OpenspaceConfig/defaultdir/type");	
	}
	else
	{
	dir="/";
	type="scp";
	str_server=server->getText().text();
	str_user=user->getText().text();
	networkframe->hide();
	delete networkframe;
	networkframe=NULL;
 	controlframe->recalc();
	}
	
	
pathtype pt(dir,type,str_server,str_user);

Frame *fr=new Frame(controlframe,leftframe,pt,this,0,&thread_vec,&file_type_settings,osicons);
fr->frame->create();
fr->hf->create();
controlframe->recalc();
}

//show or hide frame where we can set login/pass/etc for connecting do remote filesystem
 long MainWindow::onNewNetworkFrame(FXObject * sender,FXSelector,void*)
 {
 	if(networkframe==NULL)
 	{
 
 networkframe=new FXHorizontalFrame(controlframe,LAYOUT_FILL_X|FRAME_THICK,0,0,0,0,0,0,0,0);
 new FXLabel(networkframe,"server:");
 server=new FXTextField(networkframe, 20);
 new FXLabel(networkframe,"user:",osicons[11]);
 user=new FXTextField(networkframe, 20);
 new FXLabel(networkframe,"pass:",osicons[10]);
 password=new FXTextField(networkframe, 20);
 
FXComboBox* combobox=new FXComboBox(networkframe,4,NULL,0,FRAME_THICK|LAYOUT_SIDE_TOP|COMBOBOX_STATIC);
  combobox->setNumVisible(2);
  combobox->appendItem("scp");
  combobox->appendItem("ftp");
 
 new FXButton (networkframe,"connect",NULL,this,MainWindow::ID_NEWFRAME,FRAME_RAISED,0,0,0,0,0,0,0,0);
 
 networkframe->create();
 networkframe->recalc();
 	}
	else
	{
	networkframe->hide();
	delete networkframe;
	networkframe=NULL;
 	controlframe->recalc();
	}

 
 }


//----------------------------------------------------   
 
MainWindow::~MainWindow()
{
delete pref;
delete conf;
}

//----------------------------------------------------   
 
void MainWindow::create()
{   
	FXMainWindow::create();
	show(PLACEMENT_SCREEN);	
}
     

//---------------------------------------------------- 
//show or hide frame containing informaitions about copying/moving files
long MainWindow::commandsShow(FXObject * sender,FXSelector,void* ptr)
{
	if(infoframe->shown())
	infoframe->hide();
	else
	infoframe->show();
}

//executed when we change directory clicking directory name in popoup window 
long MainWindow::onChangeDir(FXObject * sender,FXSelector,void* ptr)
{


filelist *fil=current_frame->f;
FXTRACE((5,"CHANGE DIR\n",fil->path.c_str()));
FXMenuCommand *mc=(FXMenuCommand *)sender;
FXEvent* event=(FXEvent*)ptr;
FXWindow *win=(FXWindow *)mc->getParent();

int x=win->getX();
int y=win->getY();



string p=fil->path;

string current_path=p;
current_path.append(SEPARATOR);
current_path.append(mc->getText().text());

current_frame->f->path=current_path;

current_frame->f->opendir(current_path);
current_frame->generate_menu(current_path,this);
popupDir(current_frame->f,current_path,x,y);	



}



//popup with dir names when we click right button in dir path
int MainWindow::popupDir(filelist *current_filelist, string path , int x, int y)
{

vector<string> vec;
	for(int i=0;i<current_filelist->getNumItems();i++)
	{
		os_ListItem* oslistitem=(os_ListItem*)current_filelist->getItem(i);
			if(oslistitem->osf.type&FOLDER)
			{
			string name=oslistitem->osf.name;
				if(name!="." && name!=".." && name[0]!='.')
				vec.push_back (name);
				
			}
	
	}

/*

	if(current_filelist->fb->osopendir(path)==-1)
	{
	return 0;
	}





	while(1)
	{
	osfile os_file=current_filelist->fb->osreaddir();
		if(os_file.name=="")
		break;
	
		if(os_file.type&FOLDER && os_file.name!="." && os_file.name!="..")
		vec.push_back (os_file.name);		
	}

*/
unsigned int indx;

if(vec.size()>0)
{
sort(vec.begin(), vec.end());
	if(filemenu)
	{
	filemenu->popdown();
	delete filemenu;
	filemenu=NULL;
}

filemenu=new FXMenuPane(this);
	for (indx = 0; indx < vec.size(); indx++)
	{
	new FXMenuCommand(filemenu,vec[indx].c_str(),osicons[0],this,MainWindow::ID_DIRCH);
	} 

filemenu->create();
filemenu->popup(NULL,x,y,0,0);



}

}

//currently nothing ;p
long MainWindow::onListNextDir(FXObject * sender,FXSelector sel,void* ptr)
{

}


//popup with directory names when we click right button in path
long MainWindow::onListDirs(FXObject * sender,FXSelector,void* ptr)
{
FXTRACE((5,"LIST DIRS\n"));

FXButton *bt=(FXButton*)sender;

box *boxel=(box*)bt->getUserData();
box *boxbackup=boxel;

current_frame=boxel->fr;

if(boxel->fr!=right_frame && boxel->fr!=left_frame)
return 0;


string path="";

while(boxel->prebutton!=NULL)
	{	
	boxel=(box*)boxel->prebutton->getUserData();
	}
	

	while(boxel!=NULL)
	{
	
	
	
		if(boxel==boxbackup)
		{
		path.append(boxel->bt->getText().text());
		break;
		}
	path.append(boxel->bt->getText().text());
	boxel=(box*)boxel->nextbutton->getUserData();
	}



current_frame->f->path=path;
current_frame->f->opendir(path);


FXEvent* event=(FXEvent*)ptr;

int x=event->root_x;
int y=event->root_y;

popupDir(current_frame->f,path,x,y);

}


//change position or close frame
long MainWindow::onChangeList(FXObject * sender,FXSelector sel,void* ptr)
{
FXButton *bt=(FXButton*)sender;
box *boxel=(box*)bt->getUserData();


FXushort id = FXSELID(sel);
int pos=-1;
	if(id==ID_TOCLOSE)
	{
	boxel->fr->f->fb->quit();
	delete boxel->fr->hf;
	delete boxel->fr->frame;
        sender=NULL;
	return 1;
	}
	else
	{
	
		if(id==ID_TORIGHT)
		{
		pos=1;
		}
	

	}



if(pos==-1)
{
boxel->fr->frame->reparent(leftframe);
boxel->fr->hf->reparent(leftcontrolframe);


left_frame->hf->reparent(controlframe);
left_frame->toleft->show();
left_frame->toright->show();
left_frame->toclose->show();
left_frame->frame->hide();
left_frame=boxel->fr;
left_frame->frame->show();
}
else
{
boxel->fr->frame->reparent(rightframe);
boxel->fr->hf->reparent(rightcontrolframe);


right_frame->hf->reparent(controlframe);
right_frame->toleft->show();
right_frame->toright->show();
right_frame->toclose->show();
right_frame->frame->hide();
right_frame=boxel->fr;
right_frame->frame->show();
}


left_frame->f->filelist_opposite=right_frame->f;
right_frame->f->filelist_opposite=left_frame->f;


boxel->fr->toleft->hide();
boxel->fr->toright->hide();
boxel->fr->toclose->hide();

rightframe->recalc();
leftframe->recalc();

}



//notify from filelist
long MainWindow::onNotify(FXObject * sender,FXSelector sel,void* ptr)
{
FXTRACE((5,"NOTIFY\n"));

FXushort id = FXSELID(sel);
	if(id==667)	//directory change, for example user clicked double on direcotry, or clicked with 3rd button to go to parent dir
	{
	string *file_ptr=(string*)ptr;
	string  file=*file_ptr;
	FXTRACE((5,"change to directory %s\n",file.c_str()));
	delete file_ptr;
	

	string dir;
	string type;
	string str_server=file;
	
	dir="/";
	type="tar";

	pathtype pt(dir,type,str_server);
	Frame *fr;
	
	
	if(left_frame->f->active)
	fr=new Frame(leftcontrolframe,leftframe,pt,this,-1,&thread_vec,&file_type_settings,osicons);
	else	
	fr=new Frame(rightcontrolframe,rightframe,pt,this,-1,&thread_vec,&file_type_settings,osicons);
	
	if(left_frame->f->active)
	{
	
	left_frame->hf->reparent(controlframe);
	left_frame->toleft->show();
	left_frame->toright->show();
	left_frame->toclose->show();
	left_frame->frame->hide();
	left_frame=fr;
	left_frame->frame->show();
	}
	else
	{
	right_frame->hf->reparent(controlframe);
	right_frame->toleft->show();
	right_frame->toright->show();
	right_frame->toclose->show();
	right_frame->frame->hide();
	right_frame=fr;
	right_frame->frame->show();
	}
	
	controlframe->recalc();
	
	
	
	fr->frame->create();
	fr->hf->create();
	
	
	left_frame->f->filelist_opposite=right_frame->f;
	right_frame->f->filelist_opposite=left_frame->f;


	rightframe->recalc();
	leftframe->recalc();
	
	
	
	
	}
	else
	{
	

		if(left_frame->f->active)
		left_frame->generate_menu(left_frame->f->path,this);
		else	
		right_frame->generate_menu(right_frame->f->path,this);	
	}

}

//pressed button in path
long MainWindow::onOpenDir(FXObject * sender,FXSelector,void* ptr)
{
FXTRACE((5,"OPEN DIR\n"));
FXButton *bt=(FXButton*)sender;

box *boxel=(box*)bt->getUserData();
box *boxbackup=boxel;

if(boxel->fr!=right_frame && boxel->fr!=left_frame)
return 0;

string path="";
	while(boxel->prebutton!=NULL)
	{	
	boxel=(box*)boxel->prebutton->getUserData();
	}

	while(boxel!=NULL)
	{
		if(boxel==boxbackup)
		{
		break;
		}
	
	path.append(boxel->bt->getText().text());
	boxel=(box*)boxel->nextbutton->getUserData();
	}
if(path=="")
path="/";
	
boxbackup->fr->f->path=path;
boxbackup->fr->f->opendir(path);
boxbackup->fr->generate_menu(path,this);

}





//executed cyclically to update informations about progress of copying/moving/deleteing files
//using vector of special objects thread_elem to read from them information and to write request
//for example stoping copying.
long MainWindow::onTimer(FXObject*,FXSelector,void*)
{


vector<thread_elem*>::iterator iter;

	unsigned int indx;

	for (iter=thread_vec.begin();iter != thread_vec.end(); iter++)
	{
	
	bool end=false;
	informationpanel *infp;
	
	thread_elem * telem=*iter;
	
	if(telem->mutex.trylock())
		{
	
		if(telem->gui==NULL)
		{
		
		infp=new informationpanel(infoframe,telem->command);
		telem->gui=(void*)infp;
		
			if(telem->command=="copy" || telem->command=="move" || telem->command=="remove")
			{
			infp->bu->setUserData(telem);
			infp->bu->setSelector(MainWindow::ID_CANCEL);
			infp->bu->setTarget(this);
			
			}
			else if(telem->command=="execute")
			{
			
				
				string show_commands=conf->readonestring("/OpenspaceConfig/show_commands");
			
				if(show_commands=="true")
				{
					FXFont *font=new FXFont(getApp(),"Arial",8);
					if(font)
					{
					font->create();
					infp->lab0->setFont(font);
					}
			
				infp->lab0->setText(telem->src->c_str());			
				infp->frame->recalc();
				}
				else
				infp->frame->hide();
				
			}
			else if(telem->command=="init")
			{
				string text="connecting ... ";
				infp->lab0->setText(text.c_str());
				infp->frame->recalc();
			}
		
		}

			
		
		
		
			//popup with question about skipping or overwriting file
			if(telem->question==true)
			{
			telem->question=false;
			telem->pane=new FXMenuPane(this);
			FXButton *but= new FXButton(telem->pane,"overwrite",NULL,this,ID_OVERWRITE);
				  but->setUserData(telem);
				  but= new FXButton(telem->pane,"skip",NULL,this,ID_SKIP);
				  but->setUserData(telem);
			          but= new FXButton(telem->pane,"skip all",NULL,this,ID_SKIP_ALL);
				  but->setUserData(telem);
			          but= new FXButton(telem->pane,"overwrite all",NULL,this,ID_OVERWRITE_ALL);
				  but->setUserData(telem);
				  
				  
			telem->pane->create();
			telem->pane->popup(NULL,this->getX()+300,this->getY()+300,but->getWidth(),100);
			
			
			}
			
		
		//when command is copy or move update information about progress
		if(telem->command=="copy" || telem->command=="move")
		{
		infp=(informationpanel*)telem->gui;
		
		string info="total size: " + numtostring(telem->total_size);
		infp->lab0->setText(info.c_str());		
		
		info="Current file: " + telem->act_file_name + " size: " + numtostring(telem->file_size);		
		infp->lab1->setText(info.c_str());	
			
		double pct2=100*(double)telem->act_total_size/(double)telem->total_size;	
		infp->pbar2->setProgress((int)pct2);	
			
		double pct=100*(double)telem->act_file_size/(double)telem->file_size;	
		infp->pbar->setProgress((int)pct);	
			
			
		double diff=difftime(time(NULL),*telem->tim);
		
		
		info="time elapsed: " + ntos((unsigned int)diff) + " copied size: ";
		info.append(numtostring(telem->act_file_size));
		
		if(diff==0)diff=0.001;
		double speed=(double)telem->act_total_size/diff;
		
		info.append(" Speed: " + numtostring((unsigned int)speed) +  "/s");
	
		unsigned int eta=(unsigned int)((diff*(double)telem->total_size)/(double)telem->act_total_size);
		eta=eta-(unsigned int)diff;

		info.append(" ETA: " + ntos(eta));
		infp->lab2->setText(info.c_str());
		}
		else if(telem->command=="remove")
		{
		infp=(informationpanel*)telem->gui;
			string info="Removing: " + telem->act_file_name;
			FXTRACE((5,"remove file %s\n",telem->act_file_name.c_str()));
			
			infp->lab0->setText(info.c_str());
			infp->frame->recalc();
		}
		
		else if(telem->command=="execute")
		{
			string::size_type pos=telem->options.find("capture");
			if (pos != string::npos)
			{
			
			char readbuf[80];
			int k=0;
				while(read(telem->p[0], readbuf, 1)>0 && k++<2000)
				{
				 string s="";
				 s=s+readbuf[0];
		
					 if(s=="!")
						 telem->end=true;
				 	 else	 
						 telem->dst=telem->dst+readbuf[0];
				  	
				
				 }
			
				
			}
		
		}
		end=telem->end;
		telem->mutex.unlock();
		if(end)		//when telem ended
		{
				if(telem->command=="init")
				{
				FXTRACE((5,"INIT \n"));
				filelist * fil=(filelist *)telem->filel;
				fil->init();
				}
				
				
				
		if(telem->command=="copy")
		{		
			if(left_frame->f->active)
			right_frame->f->refresh();
			else		
			left_frame->f->refresh();
		}
		else if(telem->command=="remove")
		{		
			if(left_frame->f->active)
			left_frame->f->refresh();
			else		
			right_frame->f->refresh();
		}
		else if(telem->command=="move")
		{		
			right_frame->f->refresh();
			left_frame->f->refresh();			
		}
		
		string options=telem->options; // get special options
		
		string::size_type pos=options.find("rescan"); // when we need to rescan directory after command finish
		if (pos != string::npos)
		{
		
			if(left_frame->f->active)
			left_frame->f->refresh();
			else		
			right_frame->f->refresh();
			
			
		}
		
		 pos=telem->options.find("capture");  // when we need to capture data from standard output from executed program
			if (pos != string::npos)
			{
			telem->pane=new FXMenuPane(this);
			FXLabel *lab= new FXLabel(telem->pane,telem->dst.c_str());
			
			telem->pane->create();
			telem->pane->popup(NULL,this->getX(),this->getY(),lab->getWidth(),lab->getHeight());
			}
				

			//cleanup
			delete ((informationpanel*)telem->gui);
			telem->gui=NULL;
			infoframe->recalc();
			
			
			//erase from vector and delete telem;	
			thread_vec.erase(iter);
			delete telem;
			
			break;
				
	
		}
	
	}			
	
	} //end for




getApp()->addTimeout(this,ID_TIMER,200);
return 1;

}


//when we choose in popup if we want overwrite or skip
//notify about this filelist plugin using telem
long MainWindow::onOverwrite(FXObject * sender,FXSelector sel,void*)
{

	FXushort id = FXSELID(sel);
	FXButton *but=(FXButton*)sender;
	thread_elem* te=(thread_elem*)but->getUserData();
	

	te->mutex.lock();
	
		
		if(id==ID_OVERWRITE)
		{
		te->overwrite=true;
		}
		else if(id==ID_SKIP)
		{
		te->overwrite=false;
		}
		else if(ID_OVERWRITE_ALL)
		{
		te->overwrite=true;
		te->all=true;
		}
		else if(id==ID_SKIP_ALL)
		{
		te->overwrite=false;
		te->all=true;
		}
		
	te->pane->hide();
	delete te->pane;
	te->answer=true;
	te->mutex.unlock();
	
	
}


//NEED TO CHANGE THIS
long MainWindow::update(FXObject * sender,FXSelector,void*)
{
 	left->setWidth(this->getWidth()/2);
}


//telem is canceled
long  MainWindow::cancel(FXObject * sender,FXSelector,void*)
{
	FXButton *but=(FXButton*)sender;
	thread_elem* te=(thread_elem*)but->getUserData();
	te->mutex.lock();
	te->cancel=true;
	te->mutex.unlock();

}





//-----FRAME---------------------------------------------------------------------------------------------------------------------------         


Frame::Frame(
	FXComposite *cp,
	FXComposite *p,
	pathtype pt,
	FXObject *tgt,
	int position=0,
	vector<thread_elem*>*thread_vec=NULL,
	map<string,file_type*>*file_type_settings=NULL,
	FXGIFIcon **specialicons=NULL
)
{

this->pathdir=pt.dir;
this->type=pt.type;

string path=pathdir;

hf=new FXHorizontalFrame(cp,LAYOUT_FILL_X|FRAME_THICK,0,0,0,0, 0,0,0,0, 0,0);

pathdir=path;

//generate_menu(path,tgt);

int z=0;	
	 FXButton *prebutton=NULL;	
	 FXButton *nextbutton=NULL;
 
toleft=new FXButton (hf,"",specialicons[4],tgt,MainWindow::ID_TOLEFT,FRAME_THICK,0,0,0,0,0,0,0,0);
toleft->setUserData(new box(this,NULL,NULL,NULL));
toclose=new FXButton (hf,"",specialicons[5],tgt,MainWindow::ID_TOCLOSE,FRAME_THICK,0,0,0,0,0,0,0,0);
toclose->setUserData(new box(this,NULL,NULL,NULL));
toright=new FXButton (hf,"",specialicons[6],tgt,MainWindow::ID_TORIGHT,FRAME_THICK,0,0,0,0,0,0,0,0);
toright->setUserData(new box(this,NULL,NULL,NULL));
//new FXLabel (hf," ");

if(position!=0)
{
toleft->hide();
toclose->hide();
toright->hide();
}
		while(z!=-1)
		{
			string path_element="";
			z=path.find(SEPARATOR,1);
				if(z==-1)
				path_element.append(path);
				else
				path_element.append(path.substr(0,z));
			path.erase(0,z);	
					
				
	     		FXButton *bt=new FXButton (hf,path_element.c_str(),NULL,tgt,MainWindow::ID_DIR,FRAME_THICK,0,0,0,0,0,0,0,0);
			bt->setBackColor(FXRGB(145,134,201));
			if(prebutton!=NULL)
			{
			 box *boxel=(box*)prebutton->getUserData();
			 boxel->nextbutton=bt;
			}
			else
			firstbutton=bt;
			
			 bt->setUserData(new box(this,prebutton,bt,NULL));
			 prebutton=bt;
		}



	
	this->position=position;

	frame =new FXVerticalFrame(p,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN,0,0,0,0, 0,0,0,0, 0,0);	
	f=new filelist(frame,pt,thread_vec,file_type_settings,specialicons);

	
		
	if(position==0)
	{
	frame->hide();	
	}


	}


//generate buttons path for given path
void Frame::generate_menu(string path,FXObject *tgt)
	{
	FXTRACE((5,"GENERATE MENU path %s ",path.c_str()));
		if(path[0]=='/' && path[1]=='/')
		path.erase(0,1);
	
		box* boxel=(box*)firstbutton->getUserData();
		box* boxbackup=boxel;
		while(boxel)
		{
	
		box * boxel_del=boxel;
			if(boxel->nextbutton!=NULL)
			boxel=(box*)boxel->nextbutton->getUserData();
			else
			boxel=NULL;
	
		delete boxel_del->bt;
		delete boxel_del;
		boxel_del->bt=NULL;
		boxel_del=NULL;
	
		}
	
	this->pathdir=path;
	int z=0;	
	 FXButton *prebutton=NULL;	
	 FXButton *nextbutton=NULL;
		while(z!=-1)
		{
			string path_element="";
			z=path.find(SEPARATOR,1);
				if(z==-1)
				path_element.append(path);
				else
				path_element.append(path.substr(0,z));
			path.erase(0,z);	
					
	     		FXButton *bt=new FXButton (hf,path_element.c_str(),NULL,tgt,MainWindow::ID_DIR,FRAME_THICK,0,0,0,0,0,0,0,0);
			bt->setBackColor(FXRGB(145,134,201));
			bt->create();
			if(prebutton!=NULL)
			{
			 box *boxel=(box*)prebutton->getUserData();
			 boxel->nextbutton=bt;
			}
			else
			firstbutton=bt;
			
			
			 bt->setUserData(new box(this,prebutton,bt,NULL));
			 prebutton=bt;
		}
	
	}
