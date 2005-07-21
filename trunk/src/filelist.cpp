
#include <string>
#include <map>
#include <vector>
#include <list>
#include <algorithm>

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

//#include "filelist_local.h"

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
    FXMAPFUNC (SEL_RIGHTBUTTONRELEASE, filelist::ID_ICO, filelist::onPopup),
    FXMAPFUNCS (SEL_COMMAND, filelist::ID_TEXTFIELD_REG, filelist::ID_TEXTFIELD_GET, filelist::parseTextField),
    FXMAPFUNCS (SEL_COMMAND, filelist::ID_LAST, filelist::ID_LAST + 50, filelist::file_operation),
    FXMAPFUNC (SEL_COMMAND, filelist::ID_HEADER_CHANGE, filelist::onCmdHeader),
    FXMAPFUNC (SEL_COMMAND, filelist::ID_SORT_CHANGE, filelist::onCmdHeader),
    FXMAPFUNC (SEL_COMMAND, cmddialog::ID_COMMAND, filelist::onCommand), FXMAPFUNC (SEL_COMMAND, cmddialog::ID_CANCEL_COMMAND, filelist::onCommandCancel)};

FXIMPLEMENT (filelist, FXIconList, filelistMap, ARRAYNUMBER (filelistMap))
     bool filelist::ascend = true;
     bool filelist::strcase = false;


//return file type for given name
     string getfiletype (string name)
{

  transform (name.begin (), name.end (), name.begin (), tolower);

  string r = MimeType::getMimeFromName (name);

 //printf("Name: '%s' MimeType: '%s'\n", name.c_str(), r.c_str());

  return r;

}

//----------------------------------------------------  



//-----FILELIST----------------------------------------------------------------------------------------------------------------------------------------- 




//return default command for given filename
// if resolve true return command with replaced {f} with valid path
string
filelist::getdefaultcommand (string name, bool resolve = true)
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
      string fullname = "\"" + path + SEPARATOR + name + "\"";

      int pos = res.find ("{f}");
      res.replace (pos, fullname.length (), fullname);
      return res;
    }
  else
    return key;
}



//--------------------------------------------------------------------------------

filelist::filelist (FXComposite * p, pathtype pt, vector < thread_elem * >*thread_vec, map < string, file_type * >*file_type_settings, FXGIFIcon ** specialicons):
FXIconList (p, this, ID_ICO, LAYOUT_FILL_X | LAYOUT_FILL_Y | ICONLIST_EXTENDEDSELECT | ICONLIST_COLUMNS)
{

  popupmenu = NULL;
  sortpop = NULL;


//bind keys
  FXAccelTable *table = getShell ()->getAccelTable ();
  table->addAccel (MKUINT (KEY_a, CONTROLMASK), this, FXSEL (SEL_COMMAND, filelist::ID_SELECT_ALL));
  table->addAccel (MKUINT (KEY_F5, 0), this, FXSEL (SEL_COMMAND, filelist::ID_REFRESH));
  table->addAccel (MKUINT (KEY_Delete, 0), this, FXSEL (SEL_COMMAND, filelist::ID_REMOVE));

  captionfont = new FXFont (getApp (), "arial", 9, FONTWEIGHT_BOLD);
  captionfont->create ();

  captionfont1 = new FXFont (getApp (), "times", 8);
  captionfont1->create ();
  fxmessage (pt.server.c_str ());

  osicons = specialicons;

  this->file_type_settings = file_type_settings;
  this->thread_vec = thread_vec;
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
      setFont (captionfont);
    }
  else if (style == "small icons")
    this->setListStyle (ICONLIST_EXTENDEDSELECT | ICONLIST_MINI_ICONS | ICONLIST_COLUMNS);
  else if (style == "detailed")
    this->setListStyle (ICONLIST_EXTENDEDSELECT | ICONLIST_DETAILED | ICONLIST_COLUMNS);


  sort_nr = 0;


  notifyparent = getShell ();

  active = false;


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
      fxmessage ("zalADOWANY :)");
      filelist_base *(*gg) (void);
      gg = (filelist_base * (*)(void)) fxdllSymbol (dllhandle, "get_filelist");
      fb = gg ();






      FXIconListSortFunc sortfunc = filelist::cmp;
      setSortFunc (sortfunc);

//label=new FXLabel (p,this->path.c_str());
      info = new FXLabel (p, fb->info ().c_str ());

      bottomframe = new FXHorizontalFrame (p, LAYOUT_FILL_X | FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
      textfield = new FXTextField (bottomframe, 30, this, filelist::ID_TEXTFIELD_REG);

      new FXButton (bottomframe, "", osicons[15], this, filelist::ID_TEXTFIELD_REG, BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
      new FXButton (bottomframe, "Go", 0, this, filelist::ID_TEXTFIELD_GO, BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
      new FXButton (bottomframe, "Get", 0, this, filelist::ID_TEXTFIELD_GET, BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);


      dial = NULL;
      this->pt = new pathtype (pt);

      processing = true;
      thread_elem *el = new thread_elem (fb, "init", "none");
      start_thread (el);



    }
}

filelist::~filelist ()
{

  delete sortpop;
  FXTRACE ((5, "destruct\n"));

}

//--------------------------------------------------------------------
//initializing fuction, get availible headers from filelist plugin
void
filelist::init (void)
{
  processing = false;
  for (int i = 0; i < vector_name.size (); i++)
    {
      fxmessage (vector_name[i].c_str ());
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

  sortmenu = new FXOptionMenu (bottomframe, sortpop, LAYOUT_TOP | FRAME_RAISED | FRAME_THICK | JUSTIFY_HZ_APART | ICON_AFTER_TEXT);
  sortmenu->create ();
  bottomframe->recalc ();
  opendir (this->path);

}


//button pressed
long
filelist::keyPress (FXObject * sender, FXSelector sel, void *ptr)
{
  FXTRACE ((5, "KEY PRESSED\n"));

  if (processing)
    return 0;

  filelist *fil;
  if (active)
    fil = this;
  else
    fil = filelist_opposite;



  FXushort id = FXSELID (sel);

  if (id == ID_SELECT_ALL)
    {
      for (int i = 0; i < fil->getNumItems (); i++)
	{
	  string name = getItem (i)->getText ().text ();
	  if (name != "." && name != "..")
	    fil->selectItem (i);
	}
    }
  else if (id == ID_REFRESH)
    {
      fil->refresh ();
    }
  else if (id == ID_REMOVE)
    {
      fil->copymoveremove ("remove");
    }
}

//copy/move/remove function
void
filelist::copymoveremove (string com_name)
{
  int selit = 0;

  for (int c = 0; c < getNumItems (); c++)
    {
      if (isItemSelected (c))
	selit++;
    }

  string *srclist = new string[selit + 1];

  int ind = 0;
  for (int c = 0; c < getNumItems (); c++)
    {
      if (isItemSelected (c))
	{
	  srclist[ind] = path + SEPARATOR + getItemText (c).text ();
	  ind++;
	}


    }
  srclist[ind] = "";

  string options;
  filelist_base *fil = fb;


  if (this->type == "local" && com_name != "remove")
    {
      options = "upload";
      fil = filelist_opposite->fb;
    }
  else
    options = "download";


  FXTRACE ((5, "copy/move/remove"));
  thread_elem *el = new thread_elem (fil, com_name, options, srclist,
				     filelist_opposite->path);
  start_thread (el);
}

//opendir
void
filelist::opendir (string dir)
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
	  icon = osicons[1];
	  icon2 = osicons[0];
	}
      else
	{

	  string ext = getfiletype (os_file.name);
	  file_type *filet = NULL;
	  if (ext != "")
	    {
	      filet = (*file_type_settings)[ext];
	      if (filet == NULL)
		{
		  ext = ext.substr (0, ext.find ("/"));

		  filet = (*file_type_settings)[ext];
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
	      icon = osicons[3];
	      icon2 = osicons[2];
	      color = readcolor (conf->readonestring ("/OpenspaceConfig/file_types_special/all/color"));
	      backcolor = readcolor2 (conf->readonestring ("/OpenspaceConfig/file_types_special/all/backcolor"));
	    }


	  if (os_file.size < thumb_size)
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


long
filelist::setFocus (FXObject * obj, FXSelector sel, void *ptr)
{

  active = true;
  filelist_opposite->active = false;

  setBackColor (FXRGB (255, 255, 255));
  chdir (path.c_str ());
}

long
filelist::lostFocus (FXObject * obj, FXSelector sel, void *ptr)
{
//active=false;
  setBackColor (FXRGB (220, 220, 220));


}


//when we enter string in the text field and execute command for regexp select etc
long
filelist::parseTextField (FXObject * sender, FXSelector sel, void *)
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
long
filelist::openfile (FXObject * sender, FXSelector, void *)
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


      if (res == "INTERNAL")	//internal command, open new window for virtual file system
	{
	  if (key == "open_tar_bz2")
	    {
	      int c = getCurrentItem ();
	      string *file = new string (path + SEPARATOR + getItemText (c).text ());
	      notifyparent->handle (this, FXSEL (SEL_COMMAND, 667), (void *) file);
	    }
	  return 0;
	}
      if (res != "")
	{
	  string options = conf->readonestring ("/OpenspaceConfig/commands/" + key + "/options");
	  thread_elem *el = new thread_elem (fb, "execute", options, new string (res));
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

		      thread_elem *el = new thread_elem (fb, "execute", options,
							 new string (str2));
		      start_thread (el);
		    }
		}
	    }
	}


    }

}

//----------------------------------------------------  
// go up dir, 3rd button of mouse 
long
filelist::gotoparentdir (FXObject *, FXSelector, void *)
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
long
filelist::onPopup (FXObject *, FXSelector, void *ptr)
{

  if (processing)
    return 0;



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
	  shutterItem = new FXShutterItem (shutterFrame, res.c_str (), osicons[8], FRAME_NONE | LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

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
			      
			      int open=0;
	 
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
				
				if(open==0)
				{
				supported_commands.clear();
				}
		

			    }
			}





		      while (supported_commands.size () > 0)
			{

			  string res = supported_commands.front ();
			  supported_commands.pop_front ();

			  configure conflocal2 = conflocal;
			  string res2 = conflocal2.readonestring ("/OpenspaceConfig/commands/" + res + "/exec");
			  if (res2 == "INTERNAL")
			    comm_s = "IC_";
			  else if (res2 == "PLUGIN")
			    comm_s = "PL_";
			  else
			    comm_s = "EC_";

			  comm_s.append (res);

			  new FXButton (shutterItem->getContent (),
					res.c_str (), 0, this, ID_LAST + command_num, FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT | BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
			  commands_tab[command_num++] = comm_s.c_str ();


			}

		      continue;
		    }

		  configure conflocal2 = conflocal;
		  string res2 = conflocal2.readonestring ("/OpenspaceConfig/commands/" + res + "/exec");
		  if (res2 == "INTERNAL")
		    comm_s = "IC_";
		  else if (res2 == "PLUGIN")
		    comm_s = "PL_";
		  else
		    comm_s = "EC_";

		  comm_s.append (res);

		  new FXButton (shutterItem->getContent (), res.c_str (), 0,
				this, ID_LAST + command_num, FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT | BUTTON_TOOLBAR, 0, 0, 0, 0, 0, 0, 0, 0);
		  commands_tab[command_num++] = comm_s.c_str ();

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
long
filelist::file_operation (FXObject * obj, FXSelector sel, void *ptr)
{
  if (processing)
    return 0;

  FXushort id = FXSELID (sel);


  string com_type;
  com_type.append (commands_tab[id - ID_LAST].c_str (), 2);

  string com_name = commands_tab[id - ID_LAST];
  com_name.erase (0, 3);
  string key;

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
	      thread_elem *el = new thread_elem (fb, "execute", options,
						 new string (comm));
	      start_thread (el);
	    }
	}

      //if none element is selected and command doesnt need selected file to execute
      if (!anyselected)
	{
	  int pos = res.find ("{f}");
	  if (pos == -1)
	    {
	      thread_elem *el = new thread_elem (fb, "execute", options,
						 new string (res));
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
      else if (com_name == "open_tar_bz2")
	{
	  int c = getCurrentItem ();
	  string *file = new string (path + SEPARATOR + getItemText (c).text ());
	  notifyparent->handle (this, FXSEL (SEL_COMMAND, 667), (void *) file);
	}



    }

  else if (com_type == "PL")
    {
      FXASSERT ((5, "PLUGIN\n"));
      string res = conf->readonestring ("/OpenspaceConfig/plugins/cmddialog/" + com_name);

      if (res == "")
	return 0;

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

	  string *srclist = new string[selit + 1];

	  int ind = 0;
	  for (int c = 0; c < getNumItems (); c++)
	    {
	      if (isItemSelected (c))
		{
		  srclist[ind] = path + SEPARATOR + getItemText (c).text ();
		  ind++;
		}


	    }
	  srclist[ind] = "";


	  cmddialog *(*gg) (FXWindow *, filelist_base * fb, string * src);
	  gg = (cmddialog * (*)(FXWindow *, filelist_base * fb, string * src)) fxdllSymbol (dllhandle, "get_cmddialog");
	  dial = gg (this, fb, srclist);
	  dial->create ();





	  //dial->param();


	  dial->show (PLACEMENT_OWNER);

	}


    }





  popupmenu->popdown ();


}


//execute function(copying files etc) as thread 
void
filelist::start_thread (thread_elem * te)
{

  te->filel = (void *) this;

  thread_vec->push_back (te);


  if (te->command == "execute" && te->options == "capture")
    {
      fxmessage ("capture");
      pipe (te->p);
      fcntl (te->p[0], O_NONBLOCK);

      if (fork () == 0)		//child
	{
	  close (te->p[0]);
	  dup2 (te->p[1], fileno (stdout));

	  //close(te->p[1]);


	  //if(execl("/bin/ls","ls",NULL)== -1)perror("execl");

	  system (te->src->c_str ());
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
void *
filelist::thread_func (void *data)
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
      system (el->src->c_str ());

    }
  else if (el->command == "init")
    {

      fb->init (&filel->vector_name, &filel->vector_type, &filel->vector_width, (*(filel->pt)), conf);

    }


  el->end = true;
}


//----------------------------------------------------   

void
filelist::refresh (void)
{
  if (processing)
    return;
  opendir (path);
}

//----------------------------------------------------   
//select item using right mouse button
void
filelist::selectitem ()
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
FXint
filelist::cmp (const FXIconItem * pa, const FXIconItem * pb)
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

  if (fl->sort_nr == 0)		//name
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

      p = (const unsigned char *) a->osf.attrib[fl->sort_nr].c_str ();
      q = (const unsigned char *) b->osf.attrib[fl->sort_nr].c_str ();
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
  else				//string
    {

      while (1)
	{
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
long
filelist::onCmdHeader (FXObject *, FXSelector sel, void *ptr)
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
long
filelist::onCommand (FXObject *, FXSelector, void *ptr)
{

  if (dial->exec () != -1)
    {
      dial->hide ();
      //delete dial;
    }
  refresh ();


}

//cancel cmddialog
long
filelist::onCommandCancel (FXObject *, FXSelector, void *ptr)
{
  dial->hide ();
  delete dial;
}

//update info about selected files
long
filelist::click (FXObject *, FXSelector, void *ptr)
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
