
#include <string>
#include <map>
#include <vector>
#include <list>


using namespace std;

#include "MimeType.h"

#include "pathtype.h"
#include "fx.h"
#include "osfile.h"
#include "FXDLL.h"
#include "filelist.h"
#include "cmddialog.h"
#include <fcntl.h>
#include <sys/vfs.h>
#include <fxkeys.h>


#include <time.h>


#include "sharedobjects.h"

#include "oslistitem.h"
#include "shutter.h"

#ifdef WIN32
#define SEPARATOR "\\"
#else
#include <dlfcn.h>
#define SEPARATOR "/"
#endif
#include <sys/mount.h>

FXDEFMAP (filelist) filelistMap[] =
{
FXMAPFUNC (SEL_FOCUSIN, filelist::ID_ICO, filelist::setFocus),
	FXMAPFUNC (SEL_FOCUSOUT, filelist::ID_ICO, filelist::lostFocus),
	FXMAPFUNC (SEL_DOUBLECLICKED, filelist::ID_ICO, filelist::openfile),
	FXMAPFUNC (SEL_CLICKED, filelist::ID_ICO, filelist::click),
	FXMAPFUNC (SEL_MIDDLEBUTTONPRESS, filelist::ID_ICO, filelist::gotoparentdir),
	FXMAPFUNC (SEL_RIGHTBUTTONRELEASE, filelist::ID_ICO, filelist::onPopup),
	FXMAPFUNCS (SEL_COMMAND, filelist::ID_LAST, filelist::ID_LAST + 50, filelist::file_operation),
	FXMAPFUNCS (SEL_COMMAND, filelist::ID_LAST+51, filelist::ID_LAST + 100, filelist::key_shortcut),
	FXMAPFUNC (SEL_COMMAND, filelist::ID_TEXTFIELD_RUN, filelist::texfield_run),	
	FXMAPFUNC (SEL_COMMAND, filelist::ID_HEADER_CHANGE, filelist::onCmdHeader),
	FXMAPFUNC (SEL_COMMAND, filelist::ID_SORT_CHANGE, filelist::onCmdHeader),
	FXMAPFUNC (SEL_LEAVE, filelist::ID_HEADER_CHANGE, filelist::onCmdResize),	
	FXMAPFUNC (SEL_COMMAND, cmddialog::ID_COMMAND, filelist::onCommand),
	FXMAPFUNC (SEL_COMMAND, cmddialog::ID_CANCEL_COMMAND, filelist::onCommandCancel),
	FXMAPFUNC (SEL_DRAGGED, 0, filelist::onDragged),
	FXMAPFUNC (SEL_DND_ENTER, 0, filelist::onDNDEnter),
	FXMAPFUNC (SEL_DND_LEAVE, 0, filelist::onDNDLeave),
	FXMAPFUNC (SEL_DND_DROP, 0, filelist::onDNDDrop),
	FXMAPFUNC (SEL_DND_MOTION, 0, filelist::onDNDMotion),
	FXMAPFUNC (SEL_DND_REQUEST, 0, filelist::onDNDRequest),
	FXMAPFUNC (SEL_BEGINDRAG, 0, filelist::onBeginDrag),
	FXMAPFUNC (SEL_ENDDRAG, 0, filelist::onEndDrag),
	//FXMAPFUNC (SEL_CLIPBOARD_LOST, 0, filelist::onClipboardLost), 
	//FXMAPFUNC (SEL_CLIPBOARD_GAINED, 0, filelist::onClipboardGained), 
	FXMAPFUNC (SEL_CLIPBOARD_REQUEST, 0, filelist::onClipboardRequest),
		
	};

FXIMPLEMENT (filelist, FXIconList, filelistMap, ARRAYNUMBER (filelistMap))
     bool filelist::ascend = true;
     bool filelist::strcase = false;




//-----FILELIST----------------------------------------------------------------------------------------------------------------------------------------- 






filelist::filelist (FXComposite * p, pathtype pt):
FXIconList (p, this, ID_ICO, LAYOUT_FILL_X | LAYOUT_FILL_Y | ICONLIST_EXTENDEDSELECT | ICONLIST_COLUMNS)
{
objmanager=objectmanager::instance(getApp());
 
FXScrollArea::vertical->setArrowColor(FXRGB(255, 255, 255));
FXScrollArea::vertical->setShadowColor(objmanager->maincolor);
FXScrollArea::vertical->setHiliteColor(FXRGB(255, 255, 255));
FXScrollArea::vertical->setBorderColor(FXRGB(255, 255, 255));
FXScrollArea::vertical->setBackColor(objmanager->maincolor);

    flags |= FLAG_ENABLED | FLAG_DROPTARGET;
    popupmenu = NULL;
    sortpop = NULL;

   

    dropaction = DRAG_MOVE;   

   
    setKeys();
    setFont (objmanager->captionfont2);
    specialicons = objmanager->specialicons;


    thumb_size = 0;
    this->path = pt.dir;
    this->type = pt.type;

    if (type == "local")
    {
	string size = conf->readonestring ("/OpenspaceConfig/filelist/local/thumbs/size");
	thumb_size = atoi (size.c_str ());
    }

    string style = conf->readonestring ("/OpenspaceConfig/filelist/" + this->type + "/style");
    if (style == "big icons")
    {
	this->setListStyle (ICONLIST_EXTENDEDSELECT | ICONLIST_BIG_ICONS | ICONLIST_COLUMNS);
	setFont (objmanager->captionfont);
    }
    else if (style == "small icons")
	this->setListStyle (ICONLIST_EXTENDEDSELECT | ICONLIST_MINI_ICONS | ICONLIST_COLUMNS);
    else if (style == "detailed")
	this->setListStyle (ICONLIST_EXTENDEDSELECT | ICONLIST_DETAILED | ICONLIST_COLUMNS);


    sort_nr = 0;


    notifyparent = getShell ();

    active = false;

	FXDockSite *docksite = new FXDockSite (p, LAYOUT_SIDE_TOP | LAYOUT_FILL_X);
	FXToolBarShell *dragshell1 = new FXToolBarShell (this, FRAME_RAISED);
	toolbar = new FXToolBar (docksite, dragshell1, LAYOUT_DOCK_NEXT | LAYOUT_SIDE_TOP | LAYOUT_FILL_X | FRAME_RAISED);
	new FXToolBarGrip (toolbar, toolbar, FXToolBar::ID_TOOLBARGRIP, TOOLBARGRIP_DOUBLE);


        toolbar2 = new FXToolBar (docksite, dragshell1,LAYOUT_DOCK_NEXT | LAYOUT_SIDE_TOP | FRAME_RAISED);
        new FXToolBarGrip (toolbar2, toolbar2, FXToolBar::ID_TOOLBARGRIP, TOOLBARGRIP_SINGLE);


int command_num=0;


 if(conf->openxpath("/OpenspaceConfig/toolbars")!=-1)
	       {
	       string commandstr;
	      	 while(conf->getnextnode(commandstr))
	      	 {     	 
	     	 

		 
		 configure conflocal=*conf;		 
		 
		  if(conflocal.openxpath("/OpenspaceConfig/toolbars/"+commandstr+"/command")!=-1)
	     	  {
	      	 string res;
	      		 while(conflocal.getnextstring(res))
	      		 {     	 
				
		
			FXToolBar *toolb;
			if(commandstr=="toolbar1")
			toolb=toolbar;
			else
			toolb=toolbar2;
			
			if(res=="SEPARATOR")
			{
			 new FXSeparator (toolb, SEPARATOR_NONE);
   			 new FXSeparator (toolb, SEPARATOR_GROOVE);
   			 new FXSeparator (toolb, SEPARATOR_NONE);
			}
			else
			{
			string name=conf->readonestring ("/OpenspaceConfig/commands/"+res+"/icon");
				if(name=="")
				new FXButton (toolb, res.c_str (), NULL, this, ID_LAST + command_num, FRAME_RAISED | LAYOUT_TOP | LAYOUT_LEFT | BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
				else
				{
				string cmd_txt=conf->readonestring ("/OpenspaceConfig/commands/"+res+"/text");
				if(cmd_txt=="")
				cmd_txt=res;
				cmd_txt="\t"+cmd_txt;
			
				new FXButton (toolb, cmd_txt.c_str(), objmanager->osicons[name], this, ID_LAST + command_num, FRAME_RAISED | LAYOUT_TOP | LAYOUT_LEFT | BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
				}
			
				button_commands_tab.push_back(res);
		       		command_num++;
			 }	
			 }
	     	  }
		 
		 
	     	 }
	       }	 


if (conf->openxpath ("/OpenspaceConfig/button_commands/command") != -1)
    {

    string res;
	while (conf->getnextstring (res))
	{
		   	

	    
	}
  }


	


    string plugin_path = string(PATH_LIBDIR) + "/openspace/plugins/filelist/libfilelist" + this->type;


#ifdef WIN32
    plugin_path += ".dll";
#else
    plugin_path += ".so";
#endif

    if(!FXFile::exists(plugin_path.c_str()))
	{
	plugin_path = FXFile::getUserDirectory ("").text ()+string("/.openspace/plugins/filelist/libfilelist")+ this->type;
	#ifdef WIN32
	plugin_path += ".dll";
	#else
	plugin_path += ".so";
	#endif
	}
    
    void *dllhandle = fxdllOpen (plugin_path.c_str ());
    if (dllhandle)
    {
	
	filelist_base *(*gg) (void);
	gg = (filelist_base * (*)(void)) fxdllSymbol (dllhandle, "get_filelist");
	fb = gg ();

	FXIconListSortFunc sortfunc = filelist::cmp;
	setSortFunc (sortfunc);

//label=new FXLabel (p,this->path.c_str());
	info = new FXLabel (p, fb->info ().c_str ());
	

	//bottomframe = new FXHorizontalFrame (toolbar, LAYOUT_FILL_X | FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
	textfield = new FXTextField (toolbar, 30, this, filelist::ID_TEXTFIELD_RUN);

	dial = NULL;
	processing = false;
	this->pt = new pathtype (pt);
	
	
    vector_name.push_back ("name");
    string wi;

    if ((wi = conf->readonestring ("/OpenspaceConfig/filelist/"+type+"/properties/name/width")) != "")
	vector_width.push_back (atoi (wi.c_str ()));
    else
	vector_width.push_back (100);

    vector_type.push_back (0);


    int counter = 1;

   // display_size = conf->readonestring ("/OpenspaceConfig/filelist/"+type+"/properties/name/display");

    if (conf->openxpath ("/OpenspaceConfig/filelist/"+type+"/headers/header") != -1)
    {
    string res;
	while (conf->getnextstring (res))
	{


	    if ((wi = conf->readonestring ("/OpenspaceConfig/filelist/"+type+"/properties/" + res + "/width")) != "")
		vector_width.push_back (atoi (wi.c_str ()));
	    else
		vector_width.push_back (40);


	    vector_name.push_back (res);

	    string fieldtype=conf->readonestring ("/OpenspaceConfig/filelist/"+type+"/properties/" + res + "/type");
	    	    
	    if (fieldtype == "size")
		vector_type.push_back (1);
	    else if(fieldtype == "date")
	    	vector_type.push_back (2);	
	    else
		vector_type.push_back (0);

	    counter++;
	}
    }
	
	
	
	//if(type!="local")
	if (1)
	{
	    processing = true;
	    thread_elem *el = new thread_elem (fb, "init", "none");
	    start_thread (el);
	}
	else
	    init ();


    }
}

long filelist::texfield_run (FXObject * obj, FXSelector sel, void *ptr)
{

system(textfield->getText ().text());

}

filelist::~filelist ()
{
	



    delete sortpop;
    FXTRACE ((5, "destruct\n"));

}

//--------------------------------------------------------------------
//initializing fuction, get availible headers from filelist plugin
bool filelist::init ()
{



    if (!processing)
	fb->init (&vector_name,*pt, conf);

    for (int i = 0; i < vector_name.size (); i++)
    {

	appendHeader (vector_name[i].c_str (), NULL, vector_width[i]);
	unsigned int test = vector_type[i];
	header_vec.push_back (vector_name[i]);
    }


    sortpop = new FXPopup (this);

    int indx;

    for (indx = 0; indx < header_vec.size (); indx++)
    {
	new FXOption (sortpop, header_vec[indx].c_str (), NULL, this, ID_SORT_CHANGE, FRAME_THICK | JUSTIFY_HZ_APART | ICON_AFTER_TEXT);
    }

    new FXOption (sortpop, "file type", NULL, this, ID_SORT_CHANGE, FRAME_THICK | JUSTIFY_HZ_APART | ICON_AFTER_TEXT);

    sortmenu = new FXOptionMenu (toolbar, sortpop, LAYOUT_TOP |FRAME_THICK | JUSTIFY_HZ_APART | ICON_AFTER_TEXT);

    if (processing)
	sortmenu->create ();

    toolbar->recalc ();
    processing = false;
    
    string initialdir=fb->getinitialdir();
    if(this->path=="/" && initialdir!="/")
    {
    this->path=initialdir;
    //notifyparent->handle (this, FXSEL (SEL_COMMAND, 666), NULL);
    
    }
    
    return opendir (this->path);
    

}


//return default command for given filename

string filelist::getdefaultcommand (string name)
{

    string ext = getfiletype (name);
    string key = "";
    if (ext != "")
    {
	string str = "/types/";
	ext = ext.replace (ext.find ("/"), 1, str);
	key = conf->readonestring ("/OpenspaceConfig/file_types/" + ext + "/default");
    }

    if (key == "")
    {
	ext = ext.substr (0, ext.find ("/"));
	key = conf->readonestring ("/OpenspaceConfig/file_types/" + ext + "/default");
    }


   
	return key;
}

//return file type for given name
string filelist::getfiletype (string name)
{

    //std::transform (name.begin (), name.end (), name.begin (), std::tolower);

    string r = MimeType::getMimeFromName (name);

    //printf("Name: '%s' MimeType: '%s'\n", name.c_str(), r.c_str());

    return r;

}

	 
 

//return command with replaced {f} with valid path
string filelist::resolvecommand(string command,string name)
{
string res;
	if (command != "")
	    res = conf->readonestring ("/OpenspaceConfig/commands/" + command + "/exec");
	else
	    return "";

	if (res == "INTERNAL")
	{
	    FXTRACE ((5, "INTERNAL"));
	    return "INTERNAL";
	}
	else if(res == "VFS")
	{
	 FXTRACE ((5, "VFS"));
	    return "VFS";
	}
	
	string fullname;
	
	if(this->type=="local")
		fullname = "\"" + returnpath(name) + "\"";
	else
	{
		fullname = "\"/tmp/openspace/" + name + "\"";
	}	
	
	int pos = res.find ("{f}");
	res.replace (pos, fullname.length (), fullname);
	return res;


}



void filelist::create ()
{

    FXIconList::create ();
    if (!deleteType)
    {
	deleteType = getApp ()->registerDragType (deleteTypeName);
    }
    if (!urilistType)
    {
	urilistType = getApp ()->registerDragType (urilistTypeName);
    }

}

long filelist::key_shortcut (FXObject * obj, FXSelector sel, void *ptr)
{

    FXushort id = FXSELID (sel);
    
    string com_name;

    com_name = key_commands_tab[id - ID_LAST-51];

    runCommand(com_name);

}

//--------------------------------------------------------------------------------
//bind keys
long filelist::setKeys (void)
{
FXAccelTable *table = getShell ()->getAccelTable ();
int counter=0;
	if (conf->openxpath ("/OpenspaceConfig/commands") != -1)
  	  {
		string res;
		while (conf->getnextnode (res))
		{
		configure conflocal = *conf;
	   	string key = conflocal.readonestring ("/OpenspaceConfig/commands/" + res + "/key");
		string key_mask = conflocal.readonestring ("/OpenspaceConfig/commands/" + res + "/key_mask");
			if(key!="")
			{
			unsigned int mask=0;
			
			if(key_mask=="control")
				mask=CONTROLMASK;
			else if(key_mask=="alt")
				mask=ALTMASK;
			
			table->addAccel (MKUINT ((FXushort)objmanager->key_map[key],mask ), this, FXSEL (SEL_COMMAND, ID_LAST+51+counter));
			key_commands_tab.push_back(res);
			counter++;
			}
		}
	  }	
	    
	    


/*    
    table->addAccel (MKUINT (KEY_a, CONTROLMASK), this, FXSEL (SEL_COMMAND, filelist::ID_SELECT_ALL));
    table->addAccel (MKUINT (KEY_c, CONTROLMASK), this, FXSEL (SEL_COMMAND, filelist::ID_CLIP_COPY));
    table->addAccel (MKUINT (KEY_x, CONTROLMASK), this, FXSEL (SEL_COMMAND, filelist::ID_CLIP_CUT));
    table->addAccel (MKUINT (KEY_v, CONTROLMASK), this, FXSEL (SEL_COMMAND, filelist::ID_CLIP_PASTE));
    table->addAccel (MKUINT (KEY_F5, 0), this, FXSEL (SEL_COMMAND, filelist::ID_REFRESH));
    table->addAccel (MKUINT (KEY_Delete, 0), this, FXSEL (SEL_COMMAND, filelist::ID_REMOVE));
*/
}

//copy/move/remove function
void filelist::copymoveremove (string com_name)
{

	if(com_name=="remove")
	{
	FXMessageBox about (this, "?", "Are you sure to remove selected files?", NULL, MBOX_YES_NO | DECOR_TITLE | DECOR_BORDER);
    
		if(about.execute ()==MBOX_CLICKED_NO)
		return;
	}
	
	
    int selit = 0;

    for (int c = 0; c < getNumItems (); c++)
    {
	if (isItemSelected (c))
	    selit++;
    }


    vector<string> src;
    for (int c = 0; c < getNumItems (); c++)
    {
	if (isItemSelected (c))
	{
	    src.push_back( returnpath(getItemText (c).text ()));
	}


    }

    string options;
    filelist_base *fil = fb;


    if (this->type == "local" && com_name != "remove")
    {
	options = "download";
	fil = filelist_opposite->fb;
    }
    else  //type other than local
	options = "upload";



    if(this->type=="local" || filelist_opposite->type=="local" || com_name=="remove")
    {
    FXTRACE ((5, "copy/move/remove"));
    thread_elem *el = new thread_elem (fil, com_name, options, src,filelist_opposite->path);
    start_thread (el);
    }
    else
    {
    FXMessageBox about (this, "error", "copying/moving beetwen VFS not implemented (yet ;p)", NULL, MBOX_OK | DECOR_TITLE | DECOR_BORDER);
    about.execute ();
    }	

  
}

//opendir
bool filelist::opendir (string dir)
{


//label->setText(dir.c_str());
    


    if (fb->osopendir (dir) == -1)
    {
    return false;
    }	
clearItems ();


    for (int indx = 0; indx < icon_vec.size (); indx++)
    {
	icon_vec[indx]->detach ();
	delete icon_vec[indx];
    }

    icon_vec.erase (icon_vec.begin (), icon_vec.end ());



    int count = 0;
    while (1)
    {

	count++;
	osfile os_file = fb->osreaddir ();

	if (os_file.name == "")
	    break;

	FXColor color;
	FXColor backcolor;
	FXIcon *icon = NULL;
	FXIcon *icon2 = NULL;

	
	if (os_file.type & FOLDER)
	{
	    color = readcolor (conf->readonestring ("/OpenspaceConfig/file_types/COMMON/types/dir/color"));
	    backcolor = readcolor2 (conf->readonestring ("/OpenspaceConfig/file_types/COMMON/types/dir/backcolor"));
	    icon = specialicons[1];
	    icon2 = specialicons[0];
	}
	else
	{

	    string ext = getfiletype (os_file.name);
	    file_type *filet = NULL;
	    if (ext != "")
	    {
		filet = objmanager->file_type_settings[ext];
		if (filet == NULL)
		{
		    ext = ext.substr (0, ext.find ("/"));

		    filet = objmanager->file_type_settings[ext];
		}

	    }




	    if (filet)
	    {
		icon2 = filet->icon;
		icon = filet->bigicon;
		color = filet->color;
		backcolor = filet->backcolor;
	    }
	    else
	    {
	  
		icon = specialicons[3];
		icon2 = specialicons[2];
		color = readcolor (conf->readonestring ("/OpenspaceConfig/file_types/COMMON/types/all/color"));
		backcolor = readcolor2 (conf->readonestring ("/OpenspaceConfig/file_types/COMMON/types/all/backcolor"));
	    }
		
	    string graphtype=ext.substr(0,5);
			
	    if (graphtype=="image" && os_file.size < thumb_size && (this->getListStyle () & ICONLIST_BIG_ICONS) )
	    {

		string file = dir+"/" +os_file.name;
		FXString fil = file.c_str ();
		FXIconSource *source = new FXIconSource (getApp ());
		FXIcon *ico = NULL;

		ico = source->loadScaledIcon (fil, 64);

		if (ico)
		{
		
		    ico->create ();
		    icon_vec.push_back (ico);
		    icon = ico;
		    icon2 = ico;
		}
	
		
	    }


	}

	if (os_file.type & EXECUTABLE && !(os_file.type & FOLDER))
	{
	    color = readcolor (conf->readonestring ("/OpenspaceConfig/file_types/COMMON/types/executable/color"));
	    backcolor = readcolor2 (conf->readonestring ("/OpenspaceConfig/file_types/COMMON/types/executable/backcolor"));
	}


	appendItem (new os_ListItem (this, os_file, color, backcolor, icon, icon2));


    }

    sortItems ();

    struct statfs stats;

    string siz;
    if (statfs (dir.c_str (), &stats) != -1)
    {


	unsigned long size = stats.f_bsize / 1024 * stats.f_bavail * 1024;
	siz = numtostring (size);


    }


    string inf = "files: " + ntos (count) + "   free: " + siz;
    info->setText (inf.c_str ());
 return true;
}


long filelist::setFocus (FXObject * obj, FXSelector sel, void *ptr)
{

setKeys();
    active = true;
    filelist_opposite->active = false;
    filelist_opposite->toolbar->hide();
    filelist_opposite->toolbar2->hide();		
    toolbar->show();
    toolbar->recalc();	
    toolbar2->show();
    toolbar2->recalc();
    setBackColor (FXRGB (255, 255, 255));
    chdir (path.c_str ());
}

long filelist::lostFocus (FXObject * obj, FXSelector sel, void *ptr)
{
//active=false;
    setBackColor (FXRGB (220, 220, 220));


}




void *filelist::thread_func (void *data)
{

    thread_elem *el = (thread_elem *) data;
    filelist_base *fb = (filelist_base *) el->fb;

    FXASSERT ((5, "THREAD :) %s\n", el->command.c_str ()));

    filelist *filel = (filelist *) el->filel;

    if (el->command == "copy")
    {
	fb->copy (el);
    }
    else if (el->command == "move")
    {
	fb->move (el);
    }
    else if (el->command == "remove")
    {
	fb->remove (el);
    }
    else if (el->command == "mkdir")
    {
	//fb->mkdir(el->src,0); 
	sleep (5);
    }
    else if (el->command == "execute")
    {
    string command=el->dst;
    	if(command=="")//command don't need files as parametr
	{
	system (el->src[0].c_str ());
	}
	else
	{
	
		bool exportfile=false;
		
		if( ((filelist*)el->filel)->type!="local" && ((filelist*)el->filel)->type!="search")
		exportfile=true;
			
		bool simple_command=true;
		string::size_type pos = command.find ("{F}");
		if (pos != string::npos)
		{
		simple_command=false;
		}
		else
		{
		pos = command.find ("{f}");
		}

		string tmpdir;
		string destdir;
		string fbdir;
		if(((filelist*)el->filel)->type!="local")
		{
		tmpdir=FXFile::time ("%S%H%M%d%m%y", FXFile::now()).text ();
		destdir=string("/tmp/openspace/") + tmpdir;
		FXFile::createDirectory(destdir.c_str(),0);
		}
		
		string Flist;
		vector <string> Flist_vector;
		vector <FXTime> Flist_vector_time;
		vector <FXTime>::iterator time_iter;
		
		
	 	 vector <string>::iterator iter;
		 for (iter = el->src.begin (); iter != el->src.end (); iter++)
 		 {
		 	
			string tmpfile=*iter;
				if(exportfile)
				{
				string name=FXFile::name(iter->c_str()).text();
				fbdir=FXFile::directory(iter->c_str()).text();
				tmpfile=destdir+"/"+name;
			
				thread_elem *el2 = new thread_elem (fb, "copy", "upload", *iter,destdir);
				fb->copy (el2);
				delete el2;
				}
			if(simple_command)
			{	
				FXTime t1=FXFile::modified(tmpfile.c_str());
				string f="\""+tmpfile+"\"";
				string exec=command.replace (pos, f.length (),f );	
				system (exec.c_str ());
				if(exportfile)
				{	
				FXTime t2=FXFile::modified(tmpfile.c_str());
					if(t1!=t2)
					{
					thread_elem *el2 = new thread_elem (fb, "copy", "download", tmpfile,fbdir);
					fb->copy (el2);
					delete el2;
					}
				
				}
				
				
			}
			else
			{
			Flist=Flist+ " \""+tmpfile+"\" ";
				if(exportfile)
				{
				Flist_vector.push_back(tmpfile);
				FXTime t1=FXFile::modified(tmpfile.c_str());
				Flist_vector_time.push_back(t1);
				}
			}
		 
		 }
			if(!simple_command)
			{
			string exec=command.replace (pos, Flist.length (), Flist);	
				system (exec.c_str ());	
				
				time_iter=Flist_vector_time.begin();
				vector <string> copysrc;
				
				for (iter = Flist_vector.begin (); iter != Flist_vector.end (); iter++)
 		 		{
					FXTime t2=FXFile::modified(iter->c_str());
					if(t2!=*time_iter)
					{
					copysrc.push_back(*iter);
					
					}
					time_iter++;
				}
				
				
				if(copysrc.size()>0)
				{
				thread_elem *el2 = new thread_elem (fb, "copy", "download",copysrc,fbdir);
				fb->copy (el2);
				delete el2;
				
				}
				
				
			}
		 
		 
		if(exportfile)
		{
		FXFile::remove(destdir.c_str());
		}
		
		
	
	
	}
   				
	

    }
    else if (el->command == "init")
    {

	fb->init (&filel->vector_name, (*(filel->pt)), conf);

    }


    el->end = true;
}


int filelist::runCommand(string command)
{


string command_type=conf->readonestring ("/OpenspaceConfig/commands/" + command + "/type");


/*
    int k = getCurrentItem ();
    os_ListItem *oslistitem = (os_ListItem *) getItem (k);
    string dir=returnpath(getItemText(k).text());
    string name = oslistitem->osf.name;	
*/

    if (command_type == "VFS")	//internal command, open new window for virtual file system
    {
		int c = getCurrentItem ();
		string *file = new string (returnpath(getItemText (c).text ()));
		notifyparent->handle (this, FXSEL (SEL_COMMAND, 667), (void *) file);
		return 0;
    }
    else if (command_type == "EXTERNAL" || command_type=="")
    {


	string res = conf->readonestring ("/OpenspaceConfig/commands/" + command + "/exec");
	string options = conf->readonestring ("/OpenspaceConfig/commands/" + command + "/options");
	if (res == "")
	    return 0;
	    
       string exec=res;
    
  		bool simple_command=true;
		int pos = exec.find ("{F}");
		if (pos != -1)
		simple_command=false;
		else
		pos = exec.find ("{f}");
		
	    if (pos == -1)//command don't need any selected files
	    {
	
		thread_elem *el = new thread_elem (fb, "execute", options, res);
		start_thread (el);
	    }
	    else
	    {
	    	vector<string> src;
	
		
	    	for (int i = 0; i < getNumItems (); i++)
		{
	    		if (isItemSelected (i))
	   		 {
		 		 os_ListItem *oslistitem = (os_ListItem *) getItem (i);
				 string name = oslistitem->osf.name;
				 string fullname = returnpath(name);
				 
				 if(simple_command==true)
				 {
				 thread_elem *el = new thread_elem (fb, "execute", options,fullname,exec);
	  	 		 start_thread (el);
				 }
				 else			
				 src.push_back(fullname);
				 
				 				 
			 }
	
		 }
		 
		 if(simple_command==false && src.size()>0) 
		 {
	   	   thread_elem *el = new thread_elem (fb, "execute", options,src,exec);
	  	   start_thread (el);
		 }
	    
	    }


    }

    else if (command_type == "INTERNAL")
    {


	if (command == "other")
	{
	    path = filelist_opposite->path;
	    //label->setText(path.c_str());
	    notifyparent->handle (this, FXSEL (SEL_COMMAND, 666), NULL);
	    opendir (filelist_opposite->path);
	}
	else if (command == "copy" || command == "move" || command == "remove")
	{
	    copymoveremove (command);
	}
	else if (command == "new_file")
	{

	    string sr = "touch " + returnpath("newfile");
	    system (sr.c_str ());
	    refresh ();
	}
	else if (command == "totalsize")
	{

	    unsigned long size = 0;
	    for (int c = 0; c < getNumItems (); c++)
	    {
		if (isItemSelected (c))
		{
		    string sr = returnpath(getItemText (c).text ());
		    if (FXFile::isDirectory (sr.c_str ()))
			sr.append (SEPARATOR);
		    fb->totalsize (sr, size);

		}


	    }


	    string inf = "total size: " + numtostring (size);
	    info->setText (inf.c_str ());
	}
	else if (command == "umount")
	{
	    int c = getCurrentItem ();
	    string sr = returnpath(getItemText (c).text ());
	    umount2 (sr.c_str (), MNT_FORCE);
	}
	else if (command == "refresh")
	{
		refresh ();
	} 
	else if (command == "select_all")
	{
		for (int i = 0; i < getNumItems (); i++)
		{
	  	  string name = getItem (i)->getText ().text ();
	   		 if (name != "." && name != "..")
			 selectItem (i);
		}
	}    
	else if (command == "home")
	{
		 opendir(FXFile::getHomeDirectory ().text ());
   		 path=FXFile::getHomeDirectory ().text ();
  		 notifyparent->handle (this, FXSEL (SEL_COMMAND, 666), NULL);
	} 
	else if (command == "dirup")
	{
		 this->handle (this, FXSEL (SEL_MIDDLEBUTTONPRESS, ID_ICO), NULL);
	} 
	else if (command == "change_view_mini_icons")
	{
	setFont (objmanager->captionfont2);
	setListStyle (ICONLIST_EXTENDEDSELECT | ICONLIST_MINI_ICONS | ICONLIST_COLUMNS);
	refresh ();
	conf->saveonestring ("/OpenspaceConfig/filelist/" + this->type + "/style","small icons");
    	}
    	else if (command == "change_view_big_icons")
	{
	setFont (objmanager->captionfont);
	setListStyle (ICONLIST_EXTENDEDSELECT | ICONLIST_BIG_ICONS | ICONLIST_COLUMNS);
	refresh ();
	conf->saveonestring ("/OpenspaceConfig/filelist/" + this->type + "/style","big icons");
   	}
	else if (command == "change_view_detailed")
	{
	setFont (objmanager->captionfont2);
	setListStyle (ICONLIST_EXTENDEDSELECT | ICONLIST_DETAILED | ICONLIST_COLUMNS);
	refresh ();
	conf->saveonestring ("/OpenspaceConfig/filelist/" + this->type + "/style","detailed");
    	}
	else if(command == "clipboard_copy" || command == "clipboard_cut")
	{
		  FXDragType types[1];
   		  types[0] = urilistType;
    
  		  if (acquireClipboard (types, 1))
    		  {
   

  			 if(command == "clipboard_copy")
   			 {
   			 dropaction=DRAG_COPY;
   			 }
   			 else if(command == "clipboard_cut")
   			 {
   			 dropaction=DRAG_MOVE;
   			 }
    
    
    			 //FXEvent *event = (FXEvent *) ptr;
   			  //handleDrag (event->root_x, event->root_y, dropaction);
     
			filelist_opposite->dropaction=dropaction;

			dragfiles = FXString::null;
		for (int i = 0; i < getNumItems (); i++)
		{
	   	 if (isItemSelected (i))
	  	  {
			os_ListItem *oslistitem = (os_ListItem *) getItem (i);
			string name = oslistitem->osf.name;
			string fullname =returnpath(name);
			if (name != "." && name != "..")
			{
			    if (!dragfiles.empty ())
				dragfiles += "\r\n";
			    dragfiles += FXURL::fileToURL (fullname.c_str ());
		    
			}
	   	 }
		}

   	      }
 
	}
	else if(command == "clipboard_paste")
	{
		dropData (true);
	}
	else if(command == "switch_on_or_two_panel_mode")
	{
	
		if (filelist_opposite->getWidth () == 0)
		maximize = true;

  		 maximize = !maximize;

  		 if (maximize)
   		 {
			conf->saveonestring ("/OpenspaceConfig/panels", "single");
   		 }
   		 else
   		 {
			conf->saveonestring ("/OpenspaceConfig/panels", "double");
   		 }
   	 notifyparent->handle (this, FXSEL (SEL_COMMAND, 668), NULL);
	
	}
	else if(command == "filter_files")
	{

		for (int i = 0; i < getNumItems (); i++)
		{
	  	  deselectItem (i);
		}


		FXRex identifier (textfield->getText ());

		for (int i = 0; i < getNumItems (); i++)
		{
	 	   os_ListItem *oslistitem = (os_ListItem *) getItem (i);
	   	   string name = oslistitem->osf.name;
	   	 	if (identifier.match (name.c_str ()))
	   	 	{
			selectItem (i);
	   		}
		}

       }
       else if(command == "goto_dir")
       {
	opendir (textfield->getText ().text ());
	path = textfield->getText ().text ();
	notifyparent->handle (this, FXSEL (SEL_COMMAND, 666), NULL);
       }
       else if (command == "get_dir")
       {
	textfield->setText (path.c_str ());
       }


	
	
	
	
    }

    else if (command_type == "PLUGIN")
    {
	



	string plugin_path = string(PATH_LIBDIR) + "/openspace/plugins/cmddialog/lib" + command;
#ifdef WIN32
	plugin_path += ".dll";
#else
	plugin_path += ".so";
#endif

	if(!FXFile::exists(plugin_path.c_str()))
	{
	plugin_path = FXFile::getUserDirectory ("").text ()+string("/.openspace/plugins/cmddialog")+"/lib" + command;
	#ifdef WIN32
	plugin_path += ".dll";
	#else
	plugin_path += ".so";
	#endif
	}


	void *dllhandle = fxdllOpen (plugin_path.c_str ());

	fxmessage (fxdllError ().text ());

	if (dllhandle)
	{

	    int selit = 0;

	    for (int c = 0; c < getNumItems (); c++)
	    {
		if (isItemSelected (c))
		    selit++;
	    }

	
	    vector < string > src;
	    for (int c = 0; c < getNumItems (); c++)
	    {
		if (isItemSelected (c))
		{
		    src.push_back(returnpath(getItemText (c).text ()));
		}
	    }
	
	    cmddialog *(*gg) (FXWindow *, filelist_base * fb, vector < string > src);
	    gg = (cmddialog * (*)(FXWindow *, filelist_base * fb, vector < string > src)) fxdllSymbol (dllhandle, "get_cmddialog");
	    dial = gg (this, fb, src);
	    dial->create ();

	    dial->show (PLACEMENT_OWNER);

	}


    }
    


}

//double click on file/direcotry in filelist
long filelist::openfile (FXObject * sender, FXSelector, void *)
{

    if (processing)
	return 0;

    int k = getCurrentItem ();
    os_ListItem *oslistitem = (os_ListItem *) getItem (k);
  
    if (oslistitem->osf.type & FOLDER)
    {
	string dir = this->path;

	if (getItemText (k) == "..")
	{
	    dir = FXFile::upLevel (dir.c_str ()).text ();

	}
	else if (getItemText (k) == ".")
	{
	    dir = dir;
	}
	else
	{
	    dir=returnpath(getItemText(k).text());
	}

	;
	//label->setText(path.c_str());
	
	if(opendir (dir))
	{
	path = dir;
	notifyparent->handle (this, FXSEL (SEL_COMMAND, 666), NULL);
  	
	}  
    }
    else
    {    
    	string name = oslistitem->osf.name;
	string command = getdefaultcommand (name);
	
	if(command!="")
	runCommand(command);	
        else if (oslistitem->osf.type & EXECUTABLE)
   	{
    	    string command=returnpath(oslistitem->osf.name);
	    command+=" &";
	    system (command.c_str ());
   	}
	else
	{
	runCommand(conf->readonestring ("/OpenspaceConfig/file_types/COMMON/types/all/default"));
	
	}
			
    }

}

//----------------------------------------------------  
// go up dir, 3rd button of mouse 
long filelist::gotoparentdir (FXObject *, FXSelector, void *)
{
    if (processing)
	return 0;

   string pathnew = FXFile::upLevel (path.c_str ()).text ();
    
    if(opendir (pathnew));
    {
    this->path = pathnew;
    notifyparent->handle (this, FXSEL (SEL_COMMAND, 666), NULL);
    
    }
}


//----------------------------------------------------   
//poup menu
long filelist::onPopup (FXObject *, FXSelector, void *ptr)
{

    if (processing)
	return 0;

commands_tab.clear();

    FXEvent *event = (FXEvent *) ptr;
    selectitem ();

    int k = getCurrentItem ();

    if (popupmenu)
    {
	delete popupmenu;
	popupmenu = NULL;
    }

    popupmenu = new FXMenuPane (this);



    FXShutter *shutterFrame = new shutter (popupmenu, this);
    FXShutterItem *shutterItem;



    int command_num = 0;


    if (conf->openxpath ("/OpenspaceConfig/shutter") != -1)
    {
	string res;
	while (conf->getnextnode (res))
	{


	    configure conflocal = *conf;
	    //shutterItem = new FXShutterItem(shutterFrame,res.c_str(),NULL,      FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,0,0,0,0,0,0);
	    // shutterItem = new FXShutterItem(shutterFrame,res.c_str(),osicons[8],FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,0,0,0,0,0,0);
	    shutterItem = new FXShutterItem (shutterFrame, res.c_str (), objmanager->osicons["execute"], FRAME_NONE | LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	    FXButton *but = shutterItem->getButton ();
	    FXVerticalFrame *content = shutterItem->getContent ();

	    but->setButtonStyle (BUTTON_TOOLBAR);
	    but->setIconPosition (ICON_BEFORE_TEXT);
	    but->setJustify (JUSTIFY_LEFT);
	    but->setBackColor (objmanager->maincolor);
	    //but->setBackColor (readcolor (conf->readonestring ("/OpenspaceConfig/colors/maincolor")));
	    content->setBackColor (getApp ()->getShadowColor ());
	    if (conflocal.openxpath ("/OpenspaceConfig/shutter/" + res + "/command") != -1)
	    {
	    string res;
		while (conflocal.getnextstring (res))
		{
	

		    string comm_s;
		    if (res == "special")
		    {
			int sp = 0;
			list < string > supported_commands;
			list < string >::iterator nums_iter;

			for (int i = 0; i < getNumItems (); i++)
			{
			    if (isItemSelected (i))
			    {
				sp++;
				os_ListItem *oslistitem = (os_ListItem *) getItem (i);
				string ext = getfiletype (oslistitem->osf.name);
				string rep;

				string str = "/types/";
				ext = ext.replace (ext.find ("/"), 1, str);
				rep = "file_types/" + ext;

				if (oslistitem->osf.type & FOLDER)
				{
				    rep = "file_types/COMMON/types/dir";
				    ext = "dir";
				}
				configure conflocal3 = *conf;

				int open = 0;

				if (ext != "" && conflocal3.openxpath ("/OpenspaceConfig/" + rep + "/commands/command") != -1)
				{
				    open++;
				       string res;
					while (conflocal3.getnextstring (res))
					{


					if (sp == 1)
					{
					    supported_commands.push_back (res);
					}
					else	//sp >1
					{
					    nums_iter = find (supported_commands.begin (), supported_commands.end (), res);
					    if (nums_iter == supported_commands.end ())
					    {
						supported_commands.remove (res);
					    }
					}


				    }
				}




				ext = ext.substr (0, ext.find ("/"));
				rep = "file_types/" + ext;

				if (ext != "" && conflocal3.openxpath ("/OpenspaceConfig/" + rep + "/commands/command") != -1)
				{
				    open++;
				        string res;
				   while (conflocal3.getnextstring (res))
				   {


					if (sp == 1)
					{
					    supported_commands.push_back (res);
					}
					else	//sp >1
					{
					    nums_iter = find (supported_commands.begin (), supported_commands.end (), res);
					    if (nums_iter == supported_commands.end ())
					    {
						supported_commands.remove (res);
					    }
					}


				    }
				}

				if (open == 0)
				{
				    supported_commands.clear ();
				}


			    }
			}





			while (supported_commands.size () > 0)
			{

			    string res = supported_commands.front ();
			    supported_commands.pop_front ();

			    configure conflocal2 = conflocal;
			    string command_text = conflocal2.readonestring ("/OpenspaceConfig/commands/" + res + "/text");
			    if(command_text=="")command_text=res;
		
			    new FXButton (shutterItem->getContent (), command_text.c_str (), 0, this, ID_LAST + command_num, FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT | BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
			    commands_tab.push_back(res.c_str ());
			    command_num++;

			}

			continue;
		    }

		    configure conflocal2 = conflocal;
		    string command_text = conflocal2.readonestring ("/OpenspaceConfig/commands/" + res + "/text");
		    if(command_text=="")command_text=res;
	
		    new FXButton (shutterItem->getContent (), command_text.c_str (), 0, this, ID_LAST + command_num, FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT | BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
		    commands_tab.push_back(res.c_str ());
		    command_num++;

		}

	    }

	}
    }



    int height_size = ((shutter *) shutterFrame)->getheight ();
    popupmenu->create ();



    popup_x = event->root_x - 5;
    popup_y = event->root_y - 5;

    popupmenu->popup (NULL, popup_x, popup_y, 90, height_size);
    getApp ()->runModalWhileShown (popupmenu);
    popupmenu->setFocus ();
    return 1;


}

//----------------------------------------------------  
// choose command from popoup menu
long filelist::file_operation (FXObject * obj, FXSelector sel, void *ptr)
{
    if (processing)
	return 0;
	
	

    FXushort id = FXSELID (sel);
    
    string com_name;

    if(((FXWindow*)obj)->getParent()==toolbar2 || ((FXWindow*)obj)->getParent()==toolbar)
    {
    com_name = button_commands_tab[id - ID_LAST];
    }
    
    else
    {   
    com_name = commands_tab[id - ID_LAST];
    }
    
    if(popupmenu)
  	   popupmenu->popdown ();
    
    runCommand(com_name);

}


//execute function(copying files etc) as thread 
void filelist::start_thread (thread_elem * te)
{

    te->filel = (void *) this;

   objmanager->thread_vec.push_back (te);


    if (te->command == "execute" && te->options == "capture")
    {
	pipe (te->p);
	fcntl (te->p[0], O_NONBLOCK);

	if (fork () == 0)	//child
	{
	    close (te->p[0]);
	    dup2 (te->p[1], fileno (stdout));

	    //close(te->p[1]);


	    //if(execl("/bin/ls","ls",NULL)== -1)perror("execl");
		
	    system (te->src[0].c_str ());
	    /*
	       ;
	     */
	    string end = "!";
	    write (te->p[1], end.c_str (), end.size ());
	    _exit (0);
	}
	else
	{
	    close (te->p[1]);
	}


    }
    else
    {

	pthread_t t;
	pthread_attr_t attr;
	pthread_attr_init (&attr);
	pthread_create (&t, &attr, filelist::thread_func, (void *) te);
	pthread_attr_destroy (&attr);
    }

}

//thread function



//----------------------------------------------------   

void filelist::refresh (void)
{
    if (processing)
	return;
    opendir (path);
}

//----------------------------------------------------   
//select item using right mouse button
void filelist::selectitem ()
{
    if (processing)
	return;

    int num = 0;
    for (int u = 0; u < getNumItems (); u++)
	if (isItemSelected (u))
	    num++;

    if (num <= 1)
    {
	FXint x, y;
	FXuint state;
	getCursorPosition (x, y, state);

	int item = getItemAt (x, y);

	if (getCurrentItem () >= 0)
	    deselectItem (getCurrentItem ());

	if (item >= 0)
	{
	    setCurrentItem (item);
	    if (!selectItem (item));
	}

    }


}

//----------------------------------------------------  
//function for sorting files in filelist
FXint filelist::cmp (const FXIconItem * pa, const FXIconItem * pb)
{


    os_ListItem *a = (os_ListItem *) pa;
    os_ListItem *b = (os_ListItem *) pb;

    filelist *fl = (filelist *) a->list;

    if (b->osf.type & FOLDER && !(a->osf.type & FOLDER))
	return 1;
    else if (a->osf.type & FOLDER && !(b->osf.type & FOLDER))
	return -1;

/*
 register FXint diff=b->osf.type|FOLDER - a->osf.type|FOLDER;	
	 if(diff) 
		return diff;
		*/
    register const unsigned char *p;
    register const unsigned char *q;

    if (fl->sort_nr == 0)	//name
    {
	p = (const unsigned char *) a->getText ().text ();
	q = (const unsigned char *) b->getText ().text ();
    }
    else if (fl->sort_nr == -1)	//extension sorting
    {
	string exta = filelist::getfiletype (a->getText ().text ());
	if (exta == "")
	    exta = "zzzzz";
	string extb = filelist::getfiletype (b->getText ().text ());
	if (exta == "")
	    extb = "zzzzz";


	p = (const unsigned char *) exta.c_str ();
	q = (const unsigned char *) extb.c_str ();

    }
    else
    {
	p = (const unsigned char *) a->osf.attrib[fl->sort_nr-1].c_str ();
	q = (const unsigned char *) b->osf.attrib[fl->sort_nr-1].c_str ();
	
    }
    
    
    if (fl->vector_type[fl->sort_nr] == 1)	//size      
    {

	if (a->osf.size > b->osf.size)
	    if (ascend)
		return 1;
	    else
		return -1;
	else if (ascend)
	    return -1;
	else
	    return 1;

    }
    else if(fl->vector_type[fl->sort_nr] == 2)  //date
    {
    
    
       struct tm tm1;
       struct tm tm2;

       strptime((const char*)p, "%H:%M %d/%m/%y", &tm1);
       strptime((const char*)q, "%H:%M %d/%m/%y", &tm2);
    time_t t1=mktime(&tm1);
    time_t t2=mktime(&tm2);
   
    
      if(t1>t2) 
      	if(ascend)
      	  return 1;
	else  
	  return -1;
      else  if(t1<t2)	
      	if(ascend)
      	  return -1;
	else  
	  return 1;
      else 
      	return 0;		      
    } 
    else			//string
    {

	while (1)
	{

	    if (!*p)
		break;
	    if (!*q)
		break;

	    if (strcase)
	    {
		if (*p > *q)
		    if (ascend)
			return 1;
		    else
			return -1;
		if (*p < *q)
		    if (ascend)
			return -1;
		    else
			return 1;
	    }
	    else
	    {
		if (tolower (*p) > tolower (*q))
		    if (ascend)
			return 1;
		    else
			return -1;
		if (tolower (*p) < tolower (*q))
		    if (ascend)
			return -1;
		    else
			return 1;
	    }

	    p++;
	    q++;
	}
	return 0;

    }




}


//--------------------------------------------------------------------

long filelist::onCmdResize(FXObject * sender, FXSelector sel, void *ptr)
{

//FXIconList::onHeaderResize(sender, sel,ptr);

for(int i=0;i<getNumHeaders();i++)
	{
	string header=getHeaderText(i).text();
	int width=getHeaderSize(i);
	conf->saveonestring("/OpenspaceConfig/filelist/"+this->type+"/properties/"+header+"/width",ntos(width));
	
	}
}



//--------------------------------------------------------------------
//click on filelist header (change sorting)
long filelist::onCmdHeader (FXObject *, FXSelector sel, void *ptr)
{
    if (processing)
	return 0;


    FXASSERT ((5, "CMDHEADER\n"));
    FXushort id = FXSELID (sel);
    int num;

    if (id == ID_SORT_CHANGE)
    {
	FXOption *option = sortmenu->getCurrent ();
	string sort = option->getText ().text ();
	if (sort == "file type")
	{

	    num = -1;
	}
	else
	    num = sortmenu->getCurrentNo ();
    }
    else
    {
	num = (int) ptr;
    }

    if (num == sort_nr)
    {
	ascend = !ascend;
    }
    else
    {
	sort_nr = num;
    }


    sortItems ();
}

//execute command from cmddialog plugin and hide this plugin window
long filelist::onCommand (FXObject *, FXSelector, void *ptr)
{

    if (dial->exec () != -1)
    {
	dial->hide ();
	//delete dial;
    }
    refresh ();


}

//cancel cmddialog
long filelist::onCommandCancel (FXObject *, FXSelector, void *ptr)
{
    dial->hide ();
    delete dial;
}

//update info about selected files
long filelist::click (FXObject *, FXSelector, void *ptr)
{
    int count = 0;
    long unsigned int size = 0;
    os_ListItem *os;
    for (int i = 0; i < getNumItems (); i++)
    {
	if (isItemSelected (i))
	{

	    count++;
	    os = (os_ListItem *) getItem (i);
	    size += os->osf.size;

	}
    }
    string inf = "selected: " + ntos (count) + " size: " + numtostring (size);
    if (count == 1)
    {
	inf = inf + " " + os->osf.name;
    }


    info->setText (inf.c_str ());

}


    


//===================================================================================================
//Below only drag and drop functions:

// Handle drag-and-drop enter, remember current directory
long filelist::onDNDEnter (FXObject * sender, FXSelector sel, void *ptr)
{

    FXIconList::onDNDEnter (sender, sel, ptr);
    //orgdirectory=getDirectory();
    return 1;
}


// Handle drag-and-drop leave, restore current directory prior to drag
long filelist::onDNDLeave (FXObject * sender, FXSelector sel, void *ptr)
{
    FXIconList::onDNDLeave (sender, sel, ptr);
    // getApp()->removeTimeout(this,ID_OPENTIMER);
    stopAutoScroll ();
    // setDirectory(orgdirectory);
    return 1;
}


// Handle drag-and-drop motion
long filelist::onDNDMotion (FXObject * sender, FXSelector sel, void *ptr)
{
    FXEvent *event = (FXEvent *) ptr;
    FXint index = -1;

    // Cancel open up timer
    // getApp()->removeTimeout(this,ID_OPENTIMER);

    // Start autoscrolling
    if (startAutoScroll (event, FALSE))
	return 1;

    // Give base class a shot
    if (FXIconList::onDNDMotion (sender, sel, ptr))
	return 1;

    // Dropping list of filenames
    if (offeredDNDType (FROM_DRAGNDROP, urilistType))
    {

	// Drop in the background
	//  dropdirectory=getDirectory();

	// What is being done (move,copy,link)
	dropaction = inquireDNDAction ();
/*
    // We will open up a folder if we can hover over it for a while
    index=getItemAt(event->win_x,event->win_y);
    if(0<=index && isItemDirectory(index)){

      // Set open up timer
      getApp()->addTimeout(this,ID_OPENTIMER,700);

      // Directory where to drop, or directory to open up
      dropdirectory=getItemPathname(index);
      }
*/
	// See if dropdirectory is writable
	// if(FXFile::isWritable(dropdirectory)){
	//  FXTRACE((100,"accepting drop on %s\n",dropdirectory.text()));
	acceptDrop (DRAG_ACCEPT);
	//   }
	return 1;
    }
    return 0;
}


// Handle drag-and-drop drop
long filelist::onDNDDrop (FXObject * sender, FXSelector sel, void *ptr)
{
    // Cancel open up timer
//  getApp()->removeTimeout(this,ID_OPENTIMER);

    // Stop scrolling
    stopAutoScroll ();


    // Perhaps target wants to deal with it
    if (FXIconList::onDNDDrop (sender, sel, ptr))
	return 1;
    dropData (false);


    return 0;
}


// Somebody wants our dragged data
long filelist::onDNDRequest (FXObject * sender, FXSelector sel, void *ptr)
{
    FXEvent *event = (FXEvent *) ptr;
    FXuchar *data;
    FXuint len;

    // Perhaps the target wants to supply its own data
    if (FXIconList::onDNDRequest (sender, sel, ptr))
	return 1;

    // Return list of filenames as a uri-list
    if (event->target == urilistType)
    {
	if (!dragfiles.empty ())
	{
	    len = dragfiles.length ();
	    FXMEMDUP (&data, dragfiles.text (), FXuchar, len);
	    setDNDData (FROM_DRAGNDROP, event->target, data, len);
	}
	return 1;
    }

    // Delete selected files
    if (event->target == deleteType)
    {
	FXTRACE ((100, "Delete files not yet implemented\n"));
	return 1;
    }

    return 0;
}

string filelist::returnpath(string dirname)
{
	string ret;
	if(path==SEPARATOR)
	ret=SEPARATOR + dirname;
	else
	ret=path + SEPARATOR + dirname;
	
	return ret;
}

// Start a drag operation
long filelist::onBeginDrag (FXObject * sender, FXSelector sel, void *ptr)
{
    register FXint i;
    if (FXIconList::onBeginDrag (sender, sel, ptr))
	return 1;
    if (beginDrag (&urilistType, 1))
    {
	dragfiles = FXString::null;
	for (i = 0; i < getNumItems (); i++)
	{
	    if (isItemSelected (i))
	    {

		os_ListItem *oslistitem = (os_ListItem *) getItem (i);
		string name = oslistitem->osf.name;
		string fullname = returnpath(name);
		if (name != "." && name != "..")
		{
		    if (!dragfiles.empty ())
			dragfiles += "\r\n";

		    dragfiles += FXURL::fileToURL (fullname.c_str ());
		}
	    }
	}
	return 1;
    }
    return 0;
}


// End drag operation
long filelist::onEndDrag (FXObject * sender, FXSelector sel, void *ptr)
{
    if (FXIconList::onEndDrag (sender, sel, ptr))
	return 1;
    endDrag ((didAccept () != DRAG_REJECT));
    setDragCursor (getDefaultCursor ());
    dragfiles = FXString::null;
    return 1;
}


// Dragged stuff around
long filelist::onDragged (FXObject * sender, FXSelector sel, void *ptr)
{
    FXEvent *event = (FXEvent *) ptr;
    FXDragAction action;
    if (FXIconList::onDragged (sender, sel, ptr))
	return 1;
    action = DRAG_MOVE;
    if (event->state & CONTROLMASK)
	action = DRAG_COPY;
    if (event->state & SHIFTMASK)
	action = DRAG_MOVE;
    if (event->state & ALTMASK)
	action = DRAG_LINK;
    handleDrag (event->root_x, event->root_y, action);
    if (didAccept () != DRAG_REJECT)
    {
	if (action == DRAG_MOVE)
	    setDragCursor (getApp ()->getDefaultCursor (DEF_DNDMOVE_CURSOR));
	else if (action == DRAG_LINK)
	    setDragCursor (getApp ()->getDefaultCursor (DEF_DNDLINK_CURSOR));
	else
	    setDragCursor (getApp ()->getDefaultCursor (DEF_DNDCOPY_CURSOR));
    }
    else
    {
	setDragCursor (getApp ()->getDefaultCursor (DEF_DNDSTOP_CURSOR));
    }
    return 1;
}





//----------------------------------------------------  


// We now really do have the clipboard, keep clipped text
long filelist::onClipboardGained (FXObject * sender, FXSelector sel, void *ptr)
{
    FXIconList::onClipboardGained (sender, sel, ptr);
    return 1;
}


// We lost the clipboard, free clipped text
long filelist::onClipboardLost (FXObject * sender, FXSelector sel, void *ptr)
{
    FXIconList::onClipboardLost (sender, sel, ptr);
    dragfiles=FXString::null;
    //clipped.clear();
    return 1;
}


// Somebody wants our clipped text
long filelist::onClipboardRequest (FXObject * sender, FXSelector sel, void *ptr)
{
    FXEvent *event = (FXEvent *) ptr;
    FXuchar *data;
    FXuint len;
    len = dragfiles.length ();
    FXMEMDUP (&data, dragfiles.text (), FXuchar, len);
    setDNDData (FROM_CLIPBOARD, event->target, data, len);
    return 0;
}


void filelist::dropData (bool clipboard)
{


    FXuchar *data;
    FXuint len;
    FXDNDOrigin origin;
    if (clipboard)
	origin = FROM_CLIPBOARD;
    else
	origin = FROM_DRAGNDROP;

    // Get uri-list of files being dropped
    if (getDNDData (origin, urilistType, data, len))
    {
  
	FXRESIZE (&data, FXuchar, len + 1);
	data[len] = '\0';
	FXchar *p, *q;
	p = q = (FXchar *) data;

	vector < string > src;

	while (*p)
	{
	    while (*q && *q != '\r')
		q++;
	    FXString url (p, q - p);
	    FXString filesrc (FXURL::fileFromURL (url));
	    src.push_back (filesrc.text ());
	    if (*q == '\r')
		q += 2;
	    p = q;
	}


	string com_name;
	if (dropaction == DRAG_MOVE)
	    com_name = "move";
	else if (dropaction == DRAG_COPY)
	    com_name = "copy";

    filelist_base *fil = fb;
    string options;
    if (this->type == "local" && filelist_opposite->dragfiles!=FXString::null)
    {
     options = "upload";
     fil = filelist_opposite->fb;

    }
    else 
    {
    options = "download";
    }
    
    if(this->type=="local" || filelist_opposite->type=="local")
    {
	FXTRACE ((5, "copy/move/remove"));
	thread_elem *el = new thread_elem (fil, com_name, options, src, path);
	start_thread (el);
    }
    else
    {
    FXMessageBox about (this, "error", "copying/moving beetwen VFS not implemented (yet ;p)", NULL, MBOX_OK | DECOR_TITLE | DECOR_BORDER);
    about.execute ();
    }	


	FXFREE (&data);

    }
dropaction=DRAG_MOVE;
filelist_opposite->dropaction=dropaction=DRAG_MOVE;

}

