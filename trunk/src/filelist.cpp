
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
	FXMAPFUNCS (SEL_COMMAND, filelist::ID_SELECT_ALL, filelist::ID_REFRESH, filelist::keyPress),
	FXMAPFUNC (SEL_DOUBLECLICKED, filelist::ID_ICO, filelist::openfile),
	FXMAPFUNC (SEL_CLICKED, filelist::ID_ICO, filelist::click),
	FXMAPFUNC (SEL_MIDDLEBUTTONPRESS, filelist::ID_ICO, filelist::gotoparentdir),
	FXMAPFUNC (SEL_COMMAND, filelist::ID_PARENTDIR, filelist::gotoparentdir),
	FXMAPFUNC (SEL_RIGHTBUTTONRELEASE, filelist::ID_ICO, filelist::onPopup),
	FXMAPFUNCS (SEL_COMMAND, filelist::ID_TEXTFIELD_REG, filelist::ID_TEXTFIELD_GET, filelist::parseTextField),
	FXMAPFUNCS (SEL_COMMAND, filelist::ID_LAST, filelist::ID_LAST + 50, filelist::file_operation),
	FXMAPFUNC (SEL_COMMAND, filelist::ID_HEADER_CHANGE, filelist::onCmdHeader),
	FXMAPFUNC (SEL_COMMAND, filelist::ID_SORT_CHANGE, filelist::onCmdHeader),
	FXMAPFUNC (SEL_COMMAND, filelist::ID_MAXIMIZE, filelist::onMaximize),
	FXMAPFUNC (SEL_COMMAND, cmddialog::ID_COMMAND, filelist::onCommand),
	FXMAPFUNC (SEL_COMMAND, cmddialog::ID_CANCEL_COMMAND, filelist::onCommandCancel),
	FXMAPFUNC (SEL_COMMAND, filelist::ID_HOME, filelist::onGoHome),
	FXMAPFUNC (SEL_DRAGGED, 0, filelist::onDragged),
	FXMAPFUNC (SEL_DND_ENTER, 0, filelist::onDNDEnter),
	FXMAPFUNC (SEL_DND_LEAVE, 0, filelist::onDNDLeave),
	FXMAPFUNC (SEL_DND_DROP, 0, filelist::onDNDDrop),
	FXMAPFUNC (SEL_DND_MOTION, 0, filelist::onDNDMotion),
	FXMAPFUNC (SEL_DND_REQUEST, 0, filelist::onDNDRequest),
	FXMAPFUNC (SEL_BEGINDRAG, 0, filelist::onBeginDrag),
	FXMAPFUNC (SEL_ENDDRAG, 0, filelist::onEndDrag),
	FXMAPFUNCS (SEL_COMMAND, filelist::ID_CLIP_COPY,filelist::ID_CLIP_CUT, filelist::onCmdCopySel),
	FXMAPFUNC (SEL_COMMAND, filelist::ID_CLIP_PASTE, filelist::onCmdPasteSel), 
	//FXMAPFUNC (SEL_CLIPBOARD_LOST, 0, filelist::onClipboardLost), 
	//FXMAPFUNC (SEL_CLIPBOARD_GAINED, 0, filelist::onClipboardGained), 
	FXMAPFUNC (SEL_CLIPBOARD_REQUEST, 0, filelist::onClipboardRequest),
	FXMAPFUNCS (SEL_COMMAND, filelist::ID_CHANGE_VIEW_SMALL, filelist::ID_CHANGE_VIEW_DETAILS, filelist::onChangeView),
	
	};

FXIMPLEMENT (filelist, FXIconList, filelistMap, ARRAYNUMBER (filelistMap))
     bool filelist::ascend = true;
     bool filelist::strcase = false;



// Handle drag-and-drop enter, remember current directory
     long filelist::onDNDEnter (FXObject * sender, FXSelector sel, void *ptr)
{
    fxmessage ("DRAAAAAAAAAAAGING");
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
	// fxmessage("AKCPETUJEMY\n\n");
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



// Start a drag operation
long filelist::onBeginDrag (FXObject * sender, FXSelector sel, void *ptr)
{
    fxmessage ("\nBEGIN DRAG\n");
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
		string fullname = path + SEPARATOR + name;
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



//return file type for given name
string getfiletype (string name)
{

    //std::transform (name.begin (), name.end (), name.begin (), std::tolower);

    string r = MimeType::getMimeFromName (name);

    //printf("Name: '%s' MimeType: '%s'\n", name.c_str(), r.c_str());

    return r;

}

//----------------------------------------------------  


// We now really do have the clipboard, keep clipped text
long filelist::onClipboardGained (FXObject * sender, FXSelector sel, void *ptr)
{
    FXIconList::onClipboardGained (sender, sel, ptr);
    fxmessage ("AA\n");
    return 1;
}


// We lost the clipboard, free clipped text
long filelist::onClipboardLost (FXObject * sender, FXSelector sel, void *ptr)
{
    FXIconList::onClipboardLost (sender, sel, ptr);
    fxmessage ("BB\n");
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
    fxmessage ("CC\n");

    len = dragfiles.length ();
    FXMEMDUP (&data, dragfiles.text (), FXuchar, len);
    setDNDData (FROM_CLIPBOARD, event->target, data, len);
//ZMIANA
    return 0;
}

long filelist::onCmdCopySel (FXObject *, FXSelector sel, void *ptr)
{
    
    fxmessage ("COPY");
    FXDragType types[1];
    types[0] = urilistType;
    
 
    if (acquireClipboard (types, 1))
    {
   
    FXushort id = FXSELID (sel);

    if (id == ID_CLIP_COPY)
    {
    fxmessage("TYLKO KOPIUJ");
    dropaction=DRAG_COPY;
    }
    else if (id == ID_CLIP_CUT)
    {
    fxmessage("TYLKO PRZENIES");
    dropaction=DRAG_MOVE;
    }
    
    
     //FXEvent *event = (FXEvent *) ptr;
     //handleDrag (event->root_x, event->root_y, dropaction);
     
filelist_opposite->dropaction=dropaction;

	fxmessage ("ok");
	dragfiles = FXString::null;
	for (int i = 0; i < getNumItems (); i++)
	{
	    if (isItemSelected (i))
	    {
		os_ListItem *oslistitem = (os_ListItem *) getItem (i);
		string name = oslistitem->osf.name;
		string fullname = path + SEPARATOR + name;
		if (name != "." && name != "..")
		{
		    if (!dragfiles.empty ())
			dragfiles += "\r\n";
		    dragfiles += FXURL::fileToURL (fullname.c_str ());
		     fxmessage("\nplik: ");
	   	 fxmessage(fullname.c_str());
	    	fxmessage("\n");
		    
		}
	    }
	}

    }
    return 1;
}


// Paste
long filelist::onCmdPasteSel (FXObject *, FXSelector sel, void *)
{



    fxmessage ("PASTE");
    dropData (true);
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
fxmessage("KOPIUJEMY");
    // Get uri-list of files being dropped
    if (getDNDData (origin, urilistType, data, len))
    {
fxmessage("TAK");    
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
     
	fxmessage("DAJEMY OPOSITA \n");
    }
    else 
    {
    options = "download";
    }
    
    if(this->type=="local" || filelist_opposite->type=="local" || com_name=="romove")
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

//-----FILELIST----------------------------------------------------------------------------------------------------------------------------------------- 




//return default command for given filename
// if resolve true return command with replaced {f} with valid path
string filelist::getdefaultcommand (string name, bool resolve = true)
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

    string res;

    if (key == "")
    {
	key = conf->readonestring ("/OpenspaceConfig/file_types_speical/all/default");
    }

    if (resolve)
    {

	if (key != "")
	    res = conf->readonestring ("/OpenspaceConfig/commands/" + key + "/exec");
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
	string fullname = "\"" + path + SEPARATOR + name + "\"";

	int pos = res.find ("{f}");
	res.replace (pos, fullname.length (), fullname);
	return res;
    }
    else
	return key;
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

//--------------------------------------------------------------------------------
//bind keys
long filelist::setKeys (void)
{

    FXAccelTable *table = getShell ()->getAccelTable ();
    table->addAccel (MKUINT (KEY_a, CONTROLMASK), this, FXSEL (SEL_COMMAND, filelist::ID_SELECT_ALL));
    table->addAccel (MKUINT (KEY_c, CONTROLMASK), this, FXSEL (SEL_COMMAND, filelist::ID_CLIP_COPY));
    table->addAccel (MKUINT (KEY_x, CONTROLMASK), this, FXSEL (SEL_COMMAND, filelist::ID_CLIP_CUT));
    table->addAccel (MKUINT (KEY_v, CONTROLMASK), this, FXSEL (SEL_COMMAND, filelist::ID_CLIP_PASTE));
    table->addAccel (MKUINT (KEY_F5, 0), this, FXSEL (SEL_COMMAND, filelist::ID_REFRESH));
    table->addAccel (MKUINT (KEY_Delete, 0), this, FXSEL (SEL_COMMAND, filelist::ID_REMOVE));

}


filelist::filelist (FXComposite * p, pathtype pt):
FXIconList (p, this, ID_ICO, LAYOUT_FILL_X | LAYOUT_FILL_Y | ICONLIST_EXTENDEDSELECT | ICONLIST_COLUMNS)
{



    flags |= FLAG_ENABLED | FLAG_DROPTARGET;
    popupmenu = NULL;
    sortpop = NULL;

    setKeys();

    dropaction = DRAG_MOVE;


   

    objmanager=objectmanager::instance(getApp());

    setFont (objmanager->captionfont2);

    fxmessage (pt.server.c_str ());

    specialicons = objmanager->specialicons;
    //osicons = objmanager->osicons;

    //this->file_type_settings = objmanager->file_type_settings;
    //this->thread_vec = objmanager->thread_vec;
/*
int pos=path.find(":");
fxmessage("PATH=");
fxmessage(path.c_str());
fxmessage("=PATH\n");

this->path=path.substr(pos+1,path.length());
this->type=path.substr(0,pos);
*/

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


if (conf->openxpath ("/OpenspaceConfig/button_commands/command") != -1)
    {
    fxmessage("OTWARTE\n\n");
	while (1)
	{
	    string res = conf->getnextstring ();
	    fxmessage("BB\n\n");
	    if (res == "")
	    break;
	   	string name=conf->readonestring ("/OpenspaceConfig/commands/"+res+"/icon");
		string shortname=name.substr (0,name.length () - 4);
		//fxmessage("\nUUUUUUUUU=%s %s\n",res.c_str(),shortname.c_str());
		
		
		
			string comm_s;
			  configure conflocal2 = *conf;
			    string res2 = conflocal2.readonestring ("/OpenspaceConfig/commands/" + res + "/exec");
			    if (res2 == "INTERNAL")
				comm_s = "IC_";
			    else if (res2 == "PLUGIN")
				comm_s = "PL_";
			    else if(res2 == "VFS")
			    	comm_s = "VF_";
			    else
				comm_s = "EC_";

			    comm_s.append (res);
			if(name=="")
			new FXButton (toolbar2, res.c_str (), NULL, this, ID_LAST + command_num, FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT | BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
			else
			new FXButton (toolbar2, "", objmanager->osicons[shortname], this, ID_LAST + command_num, FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT | BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
			
			
			button_commands_tab.push_back(comm_s.c_str ());
		        command_num++;

	    
	}
  }




    string plugin_path = conf->readonestring ("/OpenspaceConfig/path") + "plugins/filelist/libfilelist" + this->type;


#ifdef WIN32
    plugin_path += ".dll";
#else
    plugin_path += ".so";
#endif
    fxmessage (plugin_path.c_str ());
    fxmessage ("\n");
    fxmessage (fxdllError ().text ());
    fxmessage ("\n");
    void *dllhandle = fxdllOpen (plugin_path.c_str ());
    if (dllhandle)
    {
	
	filelist_base *(*gg) (void);
	gg = (filelist_base * (*)(void)) fxdllSymbol (dllhandle, "get_filelist");
	fb = gg ();
	fxmessage ("ZALADOWANY :)");





	FXIconListSortFunc sortfunc = filelist::cmp;
	setSortFunc (sortfunc);

//label=new FXLabel (p,this->path.c_str());
	info = new FXLabel (p, fb->info ().c_str ());
	

	//bottomframe = new FXHorizontalFrame (toolbar, LAYOUT_FILL_X | FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
	textfield = new FXTextField (toolbar, 30, this, filelist::ID_TEXTFIELD_REG);
	
	new FXButton (toolbar, "", objmanager->osicons["pattern"], this, filelist::ID_TEXTFIELD_REG, BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXButton (toolbar, "Go", 0, this, filelist::ID_TEXTFIELD_GO, BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXButton (toolbar, "Get", 0, this, filelist::ID_TEXTFIELD_GET, BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);

    new FXSeparator (toolbar, SEPARATOR_NONE);
    new FXSeparator (toolbar, SEPARATOR_GROOVE);
    new FXSeparator (toolbar, SEPARATOR_NONE);
	
	if (conf->readonestring ("/OpenspaceConfig/panels") == "single")
	    new FXButton (toolbar, "", objmanager->osicons["min"], this, filelist::ID_MAXIMIZE, BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
	else
	new FXButton (toolbar, "", objmanager->osicons["max"], this, filelist::ID_MAXIMIZE, BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
  
    new FXSeparator (toolbar, SEPARATOR_NONE);
    new FXSeparator (toolbar, SEPARATOR_GROOVE);
    new FXSeparator (toolbar, SEPARATOR_NONE);
    
    new FXButton (toolbar, "", objmanager->osicons["bigicons"], this, filelist::ID_CHANGE_VIEW_BIG, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXButton (toolbar, "", objmanager->osicons["smallicons"], this, filelist::ID_CHANGE_VIEW_SMALL, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXButton (toolbar, "", objmanager->osicons["details"], this, filelist::ID_CHANGE_VIEW_DETAILS, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
  
    new FXSeparator (toolbar, SEPARATOR_NONE);
    new FXSeparator (toolbar, SEPARATOR_GROOVE);
    new FXSeparator (toolbar, SEPARATOR_NONE);
    
    new FXButton (toolbar, "", objmanager->osicons["reload"], this, filelist::ID_REFRESH, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXButton (toolbar, "", objmanager->osicons["home"], this, filelist::ID_HOME, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXButton (toolbar, "", objmanager->osicons["dirup"], this, filelist::ID_PARENTDIR, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
   
    new FXSeparator (toolbar, SEPARATOR_NONE);
    new FXSeparator (toolbar, SEPARATOR_GROOVE);
    new FXSeparator (toolbar, SEPARATOR_NONE);
   
    new FXButton (toolbar, "", objmanager->osicons["copy"], this, filelist::ID_CLIP_COPY, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXButton (toolbar, "", objmanager->osicons["cut"], this, filelist::ID_CLIP_CUT, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXButton (toolbar, "", objmanager->osicons["paste"], this, filelist::ID_CLIP_PASTE, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);


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
	while (1)
	{
	    string res = conf->getnextstring ();
	    if (res == "")
		break;

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

filelist::~filelist ()
{

    delete sortpop;
    FXTRACE ((5, "destruct\n"));

}

//--------------------------------------------------------------------
//initializing fuction, get availible headers from filelist plugin
void filelist::init ()
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
	new FXOption (sortpop, header_vec[indx].c_str (), NULL, this, ID_SORT_CHANGE, JUSTIFY_HZ_APART | ICON_AFTER_TEXT);
    }

    new FXOption (sortpop, "extension", NULL, this, ID_SORT_CHANGE, JUSTIFY_HZ_APART | ICON_AFTER_TEXT);

    sortmenu = new FXOptionMenu (toolbar, sortpop, LAYOUT_TOP | FRAME_RAISED | FRAME_THICK | JUSTIFY_HZ_APART | ICON_AFTER_TEXT);

    if (processing)
	sortmenu->create ();

    toolbar->recalc ();
    processing = false;
    opendir (this->path);


}


//button pressed
long filelist::keyPress (FXObject * sender, FXSelector sel, void *ptr)
{
    FXTRACE ((5, "KEY PRESSED\n"));

    if (processing)
	return 0;


    FXushort id = FXSELID (sel);

    if (id == ID_SELECT_ALL)
    {
	for (int i = 0; i < getNumItems (); i++)
	{
	    string name = getItem (i)->getText ().text ();
	    if (name != "." && name != "..")
		selectItem (i);
	}
    }
    else if (id == ID_REFRESH)
    {
	refresh ();
    }
    else if (id == ID_REMOVE)
    {
	copymoveremove ("remove");
    }
}

//copy/move/remove function
void filelist::copymoveremove (string com_name)
{
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
	    src.push_back( path + SEPARATOR + getItemText (c).text ());
	}


    }

    string options;
    filelist_base *fil = fb;


    if (this->type == "local" && com_name != "remove")
    {
	options = "download";
	fil = filelist_opposite->fb;
	fxmessage("DAJEMY OPOSITA \n");
    }
    else  //type other than local
	options = "upload";



    if(this->type=="local" || filelist_opposite->type=="local" || com_name=="romove")
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
void filelist::opendir (string dir)
{



//label->setText(dir.c_str());
    clearItems ();


    if (fb->osopendir (dir) == -1)
	return;




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
	    color = readcolor (conf->readonestring ("/OpenspaceConfig/file_types_special/dir/color"));
	    backcolor = readcolor2 (conf->readonestring ("/OpenspaceConfig/file_types_special/dir/backcolor"));
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
		icon = filet->icon;
		icon2 = filet->icon;
		color = filet->color;
		backcolor = filet->backcolor;
	    }
	    else
	    {
		icon = specialicons[3];
		icon2 = specialicons[2];
		color = readcolor (conf->readonestring ("/OpenspaceConfig/file_types_special/all/color"));
		backcolor = readcolor2 (conf->readonestring ("/OpenspaceConfig/file_types_special/all/backcolor"));
	    }


	    if (os_file.size < thumb_size && (this->getListStyle () & ICONLIST_BIG_ICONS))
	    {


		string file = dir + SEPARATOR + os_file.name;
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
		else
		{

		    /*
		       FXStream stream;
		       osicons[3]->savePixels(stream);      
		       getData              
		       ico= new FXIcon (getApp(),NULL);
		       ico->loadPixels(stream);
		       ico->create();
		       stream.close();
		     */
		    //ico=osicons[3];
		    /*
		       ico= new FXIcon (getApp(),NULL);
		       ico->setData(osicons[3]->getData(),osicons[3]->getOptions(),osicons[3]->getWidth(),osicons[3]->getHeight());


		       icon_vec.push_back(ico);

		       ico->create();

		       FXDCWindow dc(ico);
		       dc.setForeground(FXRGB(0,255,0));
		       dc.setBackground(FXRGB(255,0,0));
		       dc.fillRectangle(0,0,32,32);
		       dc.drawRectangle(2,2,31,31);
		       dc.drawLine(0,0,20,20);

		       icon=ico;
		       icon2=ico;
		     */

		}

	    }


	}

	if (os_file.type & EXECUTABLE && !(os_file.type & FOLDER))
	{
	    color = readcolor (conf->readonestring ("/OpenspaceConfig/file_types_special/executable/color"));
	    backcolor = readcolor2 (conf->readonestring ("/OpenspaceConfig/file_types_special/executable/backcolor"));
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
}


long filelist::setFocus (FXObject * obj, FXSelector sel, void *ptr)
{

fxmessage("\n FOCUS\n");
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


//when we enter string in the text field and execute command for regexp select etc
long filelist::parseTextField (FXObject * sender, FXSelector sel, void *)
{

    if (processing)
	return 0;

    FXushort id = FXSELID (sel);

    if (id == ID_TEXTFIELD_REG)
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
    else if (id == ID_TEXTFIELD_GO)
    {
	opendir (textfield->getText ().text ());
	path = textfield->getText ().text ();
	notifyparent->handle (this, FXSEL (SEL_COMMAND, 666), NULL);
    }
    else if (id == ID_TEXTFIELD_GET)
    {
	textfield->setText (path.c_str ());
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
	    //int k=dir.contains(SEPARATOR);
	    dir = FXFile::upLevel (dir.c_str ()).text ();
	    //if(k>2)dir.append(SEPARATOR); 
	}
	else if (getItemText (k) == ".")
	{
	    dir = dir;
	}
	else
	    dir = dir + SEPARATOR + getItemText (k).text ();






	path = dir;
	//label->setText(path.c_str());
	notifyparent->handle (this, FXSEL (SEL_COMMAND, 666), NULL);
	opendir (dir);
    }
    else
    {
	string dir = this->path;
	dir.append (SEPARATOR);
	dir.append (getItemText (k).text ());




	string name = oslistitem->osf.name;

	string res = getdefaultcommand (name);
	string key = getdefaultcommand (name, false);


	if (res == "VFS")	//internal command, open new window for virtual file system
	{
		int c = getCurrentItem ();
		string *file = new string (path + SEPARATOR + getItemText (c).text ());
		notifyparent->handle (this, FXSEL (SEL_COMMAND, 667), (void *) file);
	}
	
	if (res != "")
	{
	    string options = conf->readonestring ("/OpenspaceConfig/commands/" + key + "/options");
	    vector<string> src;
	    src.push_back(res);
	    thread_elem *el = new thread_elem (fb, "execute", options, src);
	    start_thread (el);
	}
	else if (oslistitem->osf.type & EXECUTABLE)
	{
	    dir.append (" &");
	    system (dir.c_str ());
	}
	else
	{
	    string str = conf->readonestring ("/OpenspaceConfig/file_types_special/all/default");
	    if (str != "")
	    {
		string str2 = conf->readonestring ("/OpenspaceConfig/commands/" + str + "/exec");
		if (str2 != "")
		{
		    string options = conf->readonestring ("/OpenspaceConfig/commands/" + str + "/options");
		    string fullname = "\"" + path + SEPARATOR + name + "\"";

		    string::size_type pos = str2.find ("{f}");
		    if (pos != string::npos)
		    {
			str2.replace (pos, fullname.length (), fullname);
			vector<string> src;
			src.push_back(str2);
			thread_elem *el = new thread_elem (fb, "execute", options,  src);
			start_thread (el);
		    }
		}
	    }
	}


    }

}

//----------------------------------------------------  
// go up dir, 3rd button of mouse 
long filelist::gotoparentdir (FXObject *, FXSelector, void *)
{
    if (processing)
	return 0;

    path = FXFile::upLevel (path.c_str ()).text ();
    //label->setText(path.c_str());
    notifyparent->handle (this, FXSEL (SEL_COMMAND, 666), NULL);
    opendir (path);


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

	while (1)
	{
	    string res = conf->getnextnode ();
	    if (res == "")
		break;

	    configure conflocal = *conf;
	    //shutterItem = new FXShutterItem(shutterFrame,res.c_str(),NULL,      FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,0,0,0,0,0,0);
	    // shutterItem = new FXShutterItem(shutterFrame,res.c_str(),osicons[8],FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,0,0,0,0,0,0);
	    shutterItem = new FXShutterItem (shutterFrame, res.c_str (), objmanager->osicons["execute"], FRAME_NONE | LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	    FXButton *but = shutterItem->getButton ();
	    FXVerticalFrame *content = shutterItem->getContent ();

	    but->setButtonStyle (BUTTON_TOOLBAR);
	    but->setIconPosition (ICON_BEFORE_TEXT);
	    but->setJustify (JUSTIFY_LEFT);
	    but->setBackColor (FXRGB (145, 134, 201));
	    content->setBackColor (getApp ()->getShadowColor ());
	    if (conflocal.openxpath ("/OpenspaceConfig/shutter/" + res + "/command") != -1)
	    {

		while (1)
		{
		    string res = conflocal.getnextstring ();
		    if (res == "")
			break;

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
				getdefaultcommand (oslistitem->osf.name);
				string ext = getfiletype (oslistitem->osf.name);
				string rep;

				string str = "/types/";
				ext = ext.replace (ext.find ("/"), 1, str);
				rep = "file_types/" + ext;

				if (oslistitem->osf.type & FOLDER)
				{
				    rep = "file_types_special/dir";
				    ext = "dir";
				}
				configure conflocal3 = *conf;

				int open = 0;

				if (ext != "" && conflocal3.openxpath ("/OpenspaceConfig/" + rep + "/commands/command") != -1)
				{
				    open++;
				    while (1)
				    {

					string res = conflocal3.getnextstring ();
					if (res == "")
					    break;


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
				    while (1)
				    {

					string res = conflocal3.getnextstring ();
					if (res == "")
					    break;


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
			    string res2 = conflocal2.readonestring ("/OpenspaceConfig/commands/" + res + "/exec");
			    string command_text = conflocal2.readonestring ("/OpenspaceConfig/commands/" + res + "/text");
			    if(command_text=="")command_text=res;
			    if (res2 == "INTERNAL")
				comm_s = "IC_";
			    else if (res2 == "PLUGIN")
				comm_s = "PL_";
			    else if(res2 == "VFS")
			    	comm_s = "VF_";	
			    else
				comm_s = "EC_";

			    comm_s.append (res);

			    new FXButton (shutterItem->getContent (), command_text.c_str (), 0, this, ID_LAST + command_num, FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT | BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
			    commands_tab.push_back(comm_s.c_str ());
			    command_num++;

			}

			continue;
		    }

		    configure conflocal2 = conflocal;
		    string res2 = conflocal2.readonestring ("/OpenspaceConfig/commands/" + res + "/exec");
		    string command_text = conflocal2.readonestring ("/OpenspaceConfig/commands/" + res + "/text");
		    if(command_text=="")command_text=res;
		    if (res2 == "INTERNAL")
			comm_s = "IC_";
		    else if (res2 == "PLUGIN")
			comm_s = "PL_";
		    else if(res2 == "VFS")
			comm_s = "VF_";	
		    else
			comm_s = "EC_";

		    comm_s.append (res);

		    new FXButton (shutterItem->getContent (), command_text.c_str (), 0, this, ID_LAST + command_num, FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT | BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
		    commands_tab.push_back(comm_s.c_str ());
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
    
    string com_type;
    string key;
    string com_name;
    
    if(((FXWindow*)obj)->getParent()==toolbar2)
    {
    com_type.append (button_commands_tab[id - ID_LAST].c_str (), 2);
    com_name = button_commands_tab[id - ID_LAST];
    }
    else
    {   
    com_type.append (commands_tab[id - ID_LAST].c_str (), 2);
    com_name = commands_tab[id - ID_LAST];
    }
    
    com_name.erase (0, 3);
    

    //int k=getCurrentItem();
    //string filename=path + SEPARATOR + getItemText(k).text();     

    if (com_type == "EC")
    {
	key = "EXTERNAL_COMMANDS";


	string res = conf->readonestring ("/OpenspaceConfig/commands/" + com_name + "/exec");
	string options = conf->readonestring ("/OpenspaceConfig/commands/" + com_name + "/options");
	if (res == "")
	    return 0;


	/*thread_elem* el=new thread_elem(fb,"execute",new string(path));
	   start_thread(el);
	 */
	bool anyselected = false;

	for (int i = 0; i < getNumItems (); i++)
	{
	    if (isItemSelected (i))
	    {
		anyselected = true;
		os_ListItem *oslistitem = (os_ListItem *) getItem (i);
		string name = oslistitem->osf.name;

		string fullname = path + SEPARATOR + name;

		int pos = res.find ("{f}");
		string comm = res;
		if (pos != -1)
		{

		    comm.replace (pos, fullname.length (), fullname);
		}
		vector<string> src;
		src.push_back(comm);
		thread_elem *el = new thread_elem (fb, "execute", options,src);
		start_thread (el);
	    }
	}

	//if none element is selected and command doesnt need selected file to execute
	if (!anyselected)
	{
	    int pos = res.find ("{f}");
	    if (pos == -1)
	    {
	    vector<string> src;
	    src.push_back(res);
		thread_elem *el = new thread_elem (fb, "execute", options, src);
		start_thread (el);
	    }
	}





    }

    else if (com_type == "IC")
    {


	if (com_name == "other")
	{
	    path = filelist_opposite->path;
	    //label->setText(path.c_str());
	    notifyparent->handle (this, FXSEL (SEL_COMMAND, 666), NULL);
	    opendir (filelist_opposite->path);
	}
	else if (com_name == "copy" || com_name == "move" || com_name == "remove")
	{




	    copymoveremove (com_name);


	}
	else if (com_name == "new_file")
	{

	    string sr = "touch " + path + SEPARATOR + "newfile";
	    system (sr.c_str ());
	    refresh ();
	}
	else if (com_name == "totalsize")
	{

	    unsigned long size = 0;
	    for (int c = 0; c < getNumItems (); c++)
	    {
		if (isItemSelected (c))
		{
		    string sr = path + SEPARATOR + getItemText (c).text ();
		    if (FXFile::isDirectory (sr.c_str ()))
			sr.append (SEPARATOR);
		    fb->totalsize (sr, size);

		}


	    }


	    string inf = "total size: " + numtostring (size);
	    info->setText (inf.c_str ());
	}
	else if (com_name == "umount")
	{
	    int c = getCurrentItem ();
	    string sr = path + SEPARATOR + getItemText (c).text ();
	    umount2 (sr.c_str (), MNT_FORCE);
	    fxmessage ("umount");
	    fxmessage (sr.c_str ());
	}
	    

    }

    else if (com_type == "PL")
    {
	FXASSERT ((5, "PLUGIN\n"));
	string res = conf->readonestring ("/OpenspaceConfig/plugins/cmddialog/" + com_name);

	if (res == "")
	    return 0;

	if(popupmenu)
	popupmenu->popdown ();


	string plugin_path = conf->readonestring ("/OpenspaceConfig/path") + "plugins/cmddialog/lib" + res;
#ifdef WIN32
	plugin_path += ".dll";
#else
	plugin_path += ".so";
#endif

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
		    src.push_back(path + SEPARATOR + getItemText (c).text ());
		}
	    }
	

	    cmddialog *(*gg) (FXWindow *, filelist_base * fb, vector < string > src);
	    gg = (cmddialog * (*)(FXWindow *, filelist_base * fb, vector < string > src)) fxdllSymbol (dllhandle, "get_cmddialog");
	    dial = gg (this, fb, src);
	    dial->create ();





	    //dial->param();


	    dial->show (PLACEMENT_OWNER);

	}


    }
    else if (com_type == "VF")
    {
      	int c = getCurrentItem ();
	string *file = new string (path + SEPARATOR + getItemText (c).text ());
	notifyparent->handle (this, FXSEL (SEL_COMMAND, 667), (void *) file);
       
    }




	if(popupmenu)
  	   popupmenu->popdown ();


}


//execute function(copying files etc) as thread 
void filelist::start_thread (thread_elem * te)
{

    te->filel = (void *) this;

   objmanager->thread_vec.push_back (te);


    if (te->command == "execute" && te->options == "capture")
    {
	fxmessage ("capture");
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
	//int n;
	//n = 1;
	//ioctl(pipefd[0], FIOBIO, &n);
	//el->pipe = popen(el->src->c_str(), "r");
	system (el->src[0].c_str ());

    }
    else if (el->command == "init")
    {

	fb->init (&filel->vector_name, (*(filel->pt)), conf);

    }


    el->end = true;
}


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
	string exta = getfiletype (a->getText ().text ());
	if (exta == "")
	    exta = "zzzzz";
	string extb = getfiletype (b->getText ().text ());
	if (exta == "")
	    extb = "zzzzz";


	p = (const unsigned char *) exta.c_str ();
	q = (const unsigned char *) extb.c_str ();

    }
    else
    {
  //  fxmessage("\nnr=%d",fl->sort_nr);
    

	p = (const unsigned char *) a->osf.attrib[fl->sort_nr-1].c_str ();
	q = (const unsigned char *) b->osf.attrib[fl->sort_nr-1].c_str ();
	//fxmessage((const char*)p);
    	//fxmessage((const char*)q);
	
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
   
   //if(ascend)
   //fxmessage("A");
    
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
	if (sort == "extension")
	{

	    num = -1;
	}
	else
	    num = sortmenu->getCurrentNo ();
	fxmessage (sort.c_str ());

    }
    else
    {
	num = (int) ptr;
    }
fxmessage("nr=%d\n",num);
    if (num == sort_nr)
    {
	ascend = !ascend;
	fxmessage("zmiana trybu\n");
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

long filelist::onMaximize (FXObject * sender, FXSelector, void *)
{

    if (filelist_opposite->getWidth () == 0)
	maximize = true;

    fxmessage ("MAX");
    maximize = !maximize;
    FXButton *bt = (FXButton *) sender;
    if (maximize)
    {
	fxmessage ("zapis single");
	bt->setIcon (objmanager->osicons["min"]);
	conf->saveonestring ("/OpenspaceConfig/panels", "single");
    }
    else
    {
	fxmessage ("zapis double");
	bt->setIcon (objmanager->osicons["max"]);
	conf->saveonestring ("/OpenspaceConfig/panels", "double");
    }
    notifyparent->handle (this, FXSEL (SEL_COMMAND, 668), NULL);
}

    //change view type: big/small/detailes 
long filelist::onChangeView (FXObject * sender, FXSelector sel, void *)
{
    FXushort id = FXSELID (sel);

    if (id == ID_CHANGE_VIEW_SMALL)
    {
	setFont (objmanager->captionfont2);
	setListStyle (ICONLIST_EXTENDEDSELECT | ICONLIST_MINI_ICONS | ICONLIST_COLUMNS);
	refresh ();
    }

    else if (id == ID_CHANGE_VIEW_BIG)
    {
	setFont (objmanager->captionfont);
	setListStyle (ICONLIST_EXTENDEDSELECT | ICONLIST_BIG_ICONS | ICONLIST_COLUMNS);
	refresh ();
    }

    else if (id = ID_CHANGE_VIEW_DETAILS)
    {
	setFont (objmanager->captionfont2);
	setListStyle (ICONLIST_EXTENDEDSELECT | ICONLIST_DETAILED | ICONLIST_COLUMNS);
	refresh ();
    }
}

long filelist::onGoHome (FXObject * sender, FXSelector, void *)
{
    opendir(FXFile::getHomeDirectory ().text ());
}
