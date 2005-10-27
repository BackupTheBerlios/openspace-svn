#include "fx.h"
#include "preferences.h"
#include "MimeType.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

#include "FXDLL.h"
#include "filelist_base.h"
#include <fstream>
#include <sstream>
#include "config.h"

FXDEFMAP (preferences) preferencesMap[] =
{
    //________Message_Type_____________________ID____________Message_Handler_______  
	//FXMAPFUNC (SEL_COMMAND, preferences::ID_SAVE, preferences::onSave),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_COMMAND_CHANGE, preferences::onCommandChange),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_FILETYPE_CHANGE, preferences::onFileTypeChange), 
	FXMAPFUNC (SEL_COMMAND, preferences::ID_SHUTTER_CHANGE, preferences::onShutterChange),	
	FXMAPFUNC (SEL_COMMAND, preferences::ID_TOOLBAR_CHANGE, preferences::onToolbarChange),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_VFS_CHANGE, preferences::onVfsChange),	
	FXMAPFUNC (SEL_COMMAND, preferences::ID_NEW_COMMAND, preferences::onNewCommand), 
	FXMAPFUNC (SEL_COMMAND, preferences::ID_REMOVE_COMMAND, preferences::onRemoveCommand),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_NEW_SHUTTER, preferences::onNewShutter), 
	FXMAPFUNC (SEL_COMMAND, preferences::ID_REMOVE_SHUTTER, preferences::onRemoveShutter),
	FXMAPFUNCS (SEL_COMMAND, preferences::ID_MIME_APP, preferences::ID_MIME_APP_AUTO, preferences::onOpenMimeApp),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_ADD_FILETYPE, preferences::onAddFiletype),
	FXMAPFUNCS (SEL_COMMAND, preferences::ID_CHOOSE_COLOR,preferences::ID_CHOOSE_BACKCOLOR, preferences::onChooseColor),
	FXMAPFUNCS (SEL_COMMAND, preferences::ID_ADD_COMMAND_ADDITIONAL,preferences::ID_DEL_COMMAND_ADDITIONAL, preferences::onAdditionalCommandChange),
	FXMAPFUNCS (SEL_COMMAND, preferences::ID_ADD_BUTTON_COMMAND,preferences::ID_DEL_BUTTON_COMMAND, preferences::onAddButtonCommand),
	FXMAPFUNCS (SEL_COMMAND, preferences::ID_ADD_SHUTTER_COMMAND,preferences::ID_DEL_SHUTTER_COMMAND, preferences::onAddShutterCommand),
	FXMAPFUNCS (SEL_COMMAND, preferences::ID_ADD_HEADER,preferences::ID_DEL_HEADER, preferences::onAddHeader),
	FXMAPFUNC(SEL_CLOSE,0,preferences::close),
	FXMAPFUNCS (SEL_COMMAND, preferences::ID_DOWNLOAD_INSTALL_CMD_PLUGIN,preferences::ID_DOWNLOAD_INSTALL_VFS_PLUGIN, preferences::downloadInstallPlugin),
	FXMAPFUNCS (SEL_COMMAND, preferences::ID_UPDATE_CMD_PLUGIN_LIST,preferences::ID_UPDATE_VFS_PLUGIN_LIST, preferences::updatePluginList),
	
	
	
	
};

long preferences::close (FXObject * sender, FXSelector sel, void *ptr)
{

if(saveconfiguration)
this->save();

FXMessageBox about (this, "restart", "restart openspace to apply changes", NULL, MBOX_OK | DECOR_ALL);
about.execute ();

FXDialogBox::onCmdAccept(sender,sel,ptr);

}


FXIMPLEMENT (preferences, FXDialogBox, preferencesMap, ARRAYNUMBER (preferencesMap))


long preferences::downloadInstallPlugin (FXObject * sender, FXSelector sel, void *)
{
FXushort id=FXSELID(sel);
string file;

	if(id==ID_DOWNLOAD_INSTALL_CMD_PLUGIN)
	file=string(FXFile::getUserDirectory ("").text ()) +"/.openspace/plugins/cmddialog/commandPluginsList.txt";
	else
	file=string(FXFile::getUserDirectory ("").text ()) +"/.openspace/plugins/filelist/vfsPluginsList.txt";
	
	if(FXFile::exists(file.c_str()))
	{
	
	std::string line;
   	std::ifstream infile (file.c_str());

  		  while (std::getline (infile, line))
   		 {
		string name;
		string download;
		std::stringstream parser (line);	
		parser >> name;
		parser >> download;
		
		if(id==ID_DOWNLOAD_INSTALL_CMD_PLUGIN)
			if(name==availableCommandPluginsList->getItem(availableCommandPluginsList->getCurrentItem()).text())
			{
				if(name=="")
				return 0;
				
			string cmd="cd "+ string(FXFile::getUserDirectory ("").text ()) +"/.openspace/plugins/cmddialog/ && wget -N --connect-timeout=5  "+ download;
			system(cmd.c_str());

			
			 command_container ct=commandsMap[name];
   			 if(ct.name!="") // already exists
   			 return 0;
    
   			  ct.name=name;
  			  ct.rescan=false;
  			  ct.capture=false;
			  ct.type="PLUGIN";
   			  commandsCombo->appendItem(ct.name.c_str());
    			  commandsMap[name]=ct;   
 		          commandsCombo->setCurrentItem (commandsCombo->getNumItems () - 1);
   			  this->onCommandChange(NULL,0,NULL);
    
  			  fileTypeDefaultBox->appendItem(ct.name.c_str());
  			  additionalCommands->appendItem(ct.name.c_str());
			
			break;
			}
		else
			if(name==availableVfsPluginsList->getItem(availableVfsPluginsList->getCurrentItem()).text())
			{
				if(name=="")
				return 0;
			
			string cmd="cd "+ string(FXFile::getUserDirectory ("").text ()) +"/.openspace/plugins/filelist/ && wget -N --connect-timeout=5  "+ download;
			system(cmd.c_str());

			string plugin_path = FXFile::getUserDirectory ("").text ()+string("/.openspace/plugins/filelist/libfilelist")+ name+".so";

    
   			 	void *dllhandle = fxdllOpen (plugin_path.c_str ());
    				if (dllhandle)
    				{
	
				filelist_base *(*gg) (void);
				gg = (filelist_base * (*)(void)) fxdllSymbol (dllhandle, "get_filelist");
				filelist_base *fb = gg ();	
				
				vfs v=fb->setup();
				
				conf->removestring ("/OpenspaceConfig/filelist/"+name);
				conf->addstring("/OpenspaceConfig/filelist",name,"");
				conf->addstring("/OpenspaceConfig/filelist/"+name,"type",v.type);
				conf->addstring("/OpenspaceConfig/filelist/"+name,"properties","");
				
				
				
				vector <vfsheader_container>::iterator iter;
				
					for(iter=v.vfsheaders.begin();iter!=v.vfsheaders.end();iter++)
					{
					conf->addstring("/OpenspaceConfig/filelist/"+name+"/properties",iter->name,"");
					conf->addstring("/OpenspaceConfig/filelist/"+name+"/properties/"+ iter->name,"width",iter->width);
					conf->addstring("/OpenspaceConfig/filelist/"+name+"/properties/"+ iter->name,"type",iter->type);
					}
				vfsList->appendItem(name.c_str());
			
				}

			}	
			
		
		}
	}


}
long preferences::updatePluginList (FXObject * sender, FXSelector sel, void *)
{
string cmd;

FXushort id=FXSELID(sel);

	if(id==ID_UPDATE_CMD_PLUGIN_LIST)
	cmd="cd "+ string(FXFile::getUserDirectory ("").text ()) +"/.openspace/plugins/cmddialog/ && wget -N http://openspace.linux.pl/files/"+string(PACKAGE_VERSION)+"/x86/commandPluginsList.txt";
	else
	cmd="cd "+ string(FXFile::getUserDirectory ("").text ()) +"/.openspace/plugins/filelist/ && wget -N http://openspace.linux.pl/files/"+string(PACKAGE_VERSION)+"/x86/vfsPluginsList.txt";
	
	
system(cmd.c_str());
string file;
	if(id==ID_UPDATE_CMD_PLUGIN_LIST)
	file=string(FXFile::getUserDirectory ("").text ()) +"/.openspace/plugins/cmddialog/commandPluginsList.txt";
	else
	file=string(FXFile::getUserDirectory ("").text ()) +"/.openspace/plugins/filelist/vfsPluginsList.txt";
	
	
	if(FXFile::exists(file.c_str()))
	{
	
	std::string line;
   	std::ifstream infile (file.c_str());

  		while (std::getline (infile, line))
   		{
		string name;
		string download;
		std::stringstream parser (line);	
		parser >> name;
		parser >> download;
			if(id==ID_UPDATE_CMD_PLUGIN_LIST)
			{
			availableCommandPluginsList->clearItems();
			availableCommandPluginsList->appendItem(name.c_str());
			}
			else
			{
			availableVfsPluginsList->clearItems();
			availableVfsPluginsList->appendItem(name.c_str());
			}
		}
	}

}



preferences::preferences (FXWindow * owner):FXDialogBox (owner, "Preferences", DECOR_ALL, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4)
{

  
  typingSpeed =getApp()->getTypingSpeed();
  clickSpeed  =getApp()->getClickSpeed();
  scrollSpeed =getApp()->getScrollSpeed();
  scrollDelay =getApp()->getScrollDelay();
  blinkSpeed  =getApp()->getBlinkSpeed();
  animSpeed   =getApp()->getAnimSpeed();
  menuPause   =getApp()->getMenuPause();
  tooltipPause=getApp()->getTooltipPause();
  tooltipTime =getApp()->getTooltipTime();
  dragDelta   =getApp()->getDragDelta();
  wheelLines  =getApp()->getWheelLines();


  target_typingspeed.connect(typingSpeed);
  target_clickspeed.connect(clickSpeed);
  target_scrollspeed.connect(scrollSpeed);
  target_scrolldelay.connect(scrollDelay);
  target_blinkspeed.connect(blinkSpeed);
  target_animspeed.connect(animSpeed);
  target_menupause.connect(menuPause);
  target_tooltippause.connect(tooltipPause);
  target_tooltiptime.connect(tooltipTime);
  target_dragdelta.connect(dragDelta);
  target_wheellines.connect(wheelLines);


    mimeapp=NULL;
    saveconfiguration=true;
    
    objmanager=objectmanager::instance(getApp());
    colordlg=new FXColorDialog(this,"Color Dialog");
    
    FXVerticalFrame *vertical = new FXVerticalFrame (this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);
    FXHorizontalFrame *horizontal = new FXHorizontalFrame (vertical, LAYOUT_FILL_X | LAYOUT_FILL_Y);
    FXVerticalFrame *buttons = new FXVerticalFrame (horizontal, LAYOUT_LEFT | LAYOUT_FILL_Y | FRAME_SUNKEN | PACK_UNIFORM_WIDTH | LAYOUT_FIX_HEIGHT, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0);
    FXSwitcher *switcher = new FXSwitcher (horizontal, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);


    FXVerticalFrame *mainpane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    new FXLabel (mainpane, "Main settings", NULL, LAYOUT_LEFT);
    new FXButton (buttons, "Main Settings", NULL, switcher, FXSwitcher::ID_OPEN_FIRST, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);



    new FXButton (mainpane, "Semi-Auto configure", NULL, this, preferences::ID_MIME_APP);
    new FXButton (mainpane, "Full-Auto configure", NULL, this, preferences::ID_MIME_APP_AUTO);

    new FXLabel (mainpane, "width:");
    mainwindow_width = new FXTextField (mainpane, 5);
    mainwindow_width->setText (conf->readonestring ("/OpenspaceConfig/mainwindow/width").c_str ());
    new FXLabel (mainpane, "height:");
    mainwindow_height = new FXTextField (mainpane, 5);
    mainwindow_height->setText (conf->readonestring ("/OpenspaceConfig/mainwindow/height").c_str ());
    
    
    new FXLabel (mainpane, "left directory:");
    leftdir = new FXTextField (mainpane, 50);
    leftdir->setText(conf->readonestring ("/OpenspaceConfig/leftdir/dir").c_str ());

    new FXLabel (mainpane, "right directory (in single directory window mode this is default window):");
    rightdir = new FXTextField (mainpane, 50);
    rightdir->setText(conf->readonestring ("/OpenspaceConfig/rightdir/dir").c_str ());
    
    new FXLabel (mainpane, "default new opened directory:");
    defaultdir = new FXTextField (mainpane, 50);
    defaultdir->setText(conf->readonestring ("/OpenspaceConfig/defaultdir/dir").c_str ());
    
    
    new FXLabel (mainpane, "maximum size of image file for generating thumbnails (in Bytes)");
    thumbsize = new FXTextField (mainpane, 50);
    thumbsize->setText(conf->readonestring ("/OpenspaceConfig/filelist/local/thumbs/size").c_str ());
    
    new FXLabel (mainpane, "icons theme:");
    iconsTheme=new FXListBox (mainpane);
    iconsTheme->setNumVisible(5);
    
    new FXLabel (mainpane, "font 1");
    font1 = new FXTextField (mainpane, 50);
    font1->setText(conf->readonestring ("/OpenspaceConfig/fonts/captionfont").c_str ());
    
    new FXLabel (mainpane, "font 2");
    font2 = new FXTextField (mainpane, 50);
    font2->setText(conf->readonestring ("/OpenspaceConfig/fonts/captionfont1").c_str ());    
    
    new FXLabel (mainpane, "font 3");
    font3 = new FXTextField (mainpane, 50);
    font3->setText(conf->readonestring ("/OpenspaceConfig/fonts/captionfont2").c_str ());
    
    

 string iconsdir=conf->readonestring ("/OpenspaceConfig/path") +"/icons";
	    struct dirent *dp;
	    DIR *dirp;
dirp = opendir (iconsdir.c_str ());
	    if(dirp)
	    while ((dp = readdir (dirp)) != NULL)
	    {
		if (dp->d_name[0] != '.' || (dp->d_name[1] != '\0' && (dp->d_name[1] != '.' || dp->d_name[2] != '\0')))
		{
		string name=dp->d_name;
		iconsTheme->appendItem(name.c_str());   
		}
	    }
	    closedir (dirp);
iconsdir=FXFile::getUserDirectory ("").text () +string("/.openspace/icons");

dirp = opendir (iconsdir.c_str ());
	    if(dirp)
	    while ((dp = readdir (dirp)) != NULL)
	    {
		if (dp->d_name[0] != '.' || (dp->d_name[1] != '\0' && (dp->d_name[1] != '.' || dp->d_name[2] != '\0')))
		{
		string name=dp->d_name;
		iconsTheme->appendItem(name.c_str());   
		}
	    }
	    closedir (dirp);

iconsTheme->setCurrentItem(iconsTheme->findItem(conf->readonestring ("/OpenspaceConfig/icons_theme").c_str()));

//getShell()->getWidth()
//getShell()->getHeight()


//======================================================BUTTONS===========================================================

 FXVerticalFrame *buttonsPane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
   
    new FXButton (buttons, "Menu and Buttons Settings", NULL, switcher, FXSwitcher::ID_OPEN_SECOND, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);

 new FXLabel (buttonsPane, "Buttons in configurable toolbox:", NULL, LAYOUT_LEFT);

	toolbarList=new FXListBox (buttonsPane, this, ID_TOOLBAR_CHANGE);
        toolbarList->setNumVisible(10);

	buttonsList=new FXList (buttonsPane,NULL, 0,LIST_NORMAL| LAYOUT_FIX_WIDTH, 0, 0,250);
	buttonsList->setNumVisible(5);

     
	       	
	     if(conf->openxpath("/OpenspaceConfig/toolbars")!=-1)
	       {
	       string commandstr;
	      	 while(conf->getnextnode(commandstr))
	      	 {     	 
	     	 toolbarList->appendItem(commandstr.c_str());
		 buttonsList->clearItems();
		 
		 configure conflocal=*conf;		 
		 
		  if(conflocal.openxpath("/OpenspaceConfig/toolbars/"+commandstr+"/command")!=-1)
	     	  {
	      	 string commandstr2;
	      		 while(conflocal.getnextstring(commandstr2))
	      		 {     	 
			 string icon=conflocal.readonestring ("/OpenspaceConfig/commands/" + commandstr2 + "/icon");
	     		 buttonsList->appendItem(commandstr2.c_str(),objmanager->osicons[icon]);
			 toolbarVector.push_back(toolbar_container(commandstr,commandstr2));
			 }
	     	  }
		 
		 
	     	 }
	       }	  
toolbarList->setCurrentItem (toolbarList->getNumItems () - 1);	       
	        
	FXHorizontalFrame *buttonsHframe = new FXHorizontalFrame (buttonsPane, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);  

	new FXLabel(buttonsHframe," ");
	new FXLabel(buttonsHframe," ");
	new FXArrowButton(buttonsHframe,this,ID_ADD_BUTTON_COMMAND,FRAME_RAISED|FRAME_THICK|ARROW_UP);
	new FXArrowButton(buttonsHframe,this,ID_DEL_BUTTON_COMMAND,FRAME_RAISED|FRAME_THICK|ARROW_DOWN);
	
	new FXLabel (buttonsPane, "All available commands:", NULL, LAYOUT_LEFT);
	
	additionalCommandsAvailableForButtons=new FXList (buttonsPane,NULL, 0,LIST_NORMAL| LAYOUT_FIX_WIDTH, 0, 0,250);
	additionalCommandsAvailableForButtons->setNumVisible(8);
	additionalCommandsAvailableForButtons->appendItem("SEPARATOR");
FXHorizontalFrame *buttonsHframe2 = new FXHorizontalFrame (buttonsPane, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);  

	new FXLabel(buttonsHframe2," ");
	new FXLabel(buttonsHframe2," ");
new FXArrowButton(buttonsHframe2,this,ID_DEL_SHUTTER_COMMAND,FRAME_RAISED|FRAME_THICK|ARROW_UP);
new FXArrowButton(buttonsHframe2,this,ID_ADD_SHUTTER_COMMAND,FRAME_RAISED|FRAME_THICK|ARROW_DOWN);
new FXLabel (buttonsPane, "Shutter:", NULL, LAYOUT_LEFT);


       shutterList=new FXListBox (buttonsPane, this, ID_SHUTTER_CHANGE);
       shutterList->setNumVisible(10);

shutterCommands=new FXList (buttonsPane,NULL, 0,LIST_NORMAL| LAYOUT_FIX_WIDTH, 0, 0,250);
shutterCommands->setNumVisible(5);

	new FXSeparator(buttonsPane);
	FXHorizontalFrame *hfr=new FXHorizontalFrame (buttonsPane, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXButton (hfr, "Remove shutter", NULL, this, ID_REMOVE_SHUTTER, FRAME_RAISED | ICON_ABOVE_TEXT );
	new FXButton (hfr, "New shutter", NULL, this, ID_NEW_SHUTTER, FRAME_RAISED | ICON_ABOVE_TEXT);
	newShutterEdit = new FXTextField (hfr, 20);


 if(conf->openxpath("/OpenspaceConfig/shutter")!=-1)
	       {
	       string commandstr;
	      	 while(conf->getnextnode(commandstr))
	      	 {     	 
	     	 shutterList->appendItem(commandstr.c_str());
		 shutterCommands->clearItems();
		 
		 configure conflocal=*conf;		 
		 
		  if(conflocal.openxpath("/OpenspaceConfig/shutter/"+commandstr+"/command")!=-1)
	     	  {
	      	 string commandstr2;
	      		 while(conflocal.getnextstring(commandstr2))
	      		 {     	 
	     		 shutterCommands->appendItem(commandstr2.c_str());
			 shutterVector.push_back(shutter_container(commandstr,commandstr2));
			 }
	     	  }
		 
		 
	     	 }
	       }	
	       
shutterList->setCurrentItem (shutterList->getNumItems () - 1);


//======================================================COMMANDS===========================================================


    FXVerticalFrame *commandPluginsPane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    
    new FXButton (buttons, "Command plugins Settings", NULL, switcher, FXSwitcher::ID_OPEN_THIRD, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);

 new FXLabel (commandPluginsPane, "installed plugins");
commandPluginsList=new FXListBox (commandPluginsPane, this, ID_COMMANDPLUGIN_CHANGE);
commandPluginsList->setNumVisible(30);


FXPacker *vv = new FXGroupBox (commandPluginsPane, "available plugins - download from internet", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0); 

hfr=new FXHorizontalFrame (vv, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
availableCommandPluginsList=new FXListBox (hfr);
availableCommandPluginsList->setNumVisible(30);
new FXButton (hfr, "Download and Install", NULL, this, ID_DOWNLOAD_INSTALL_CMD_PLUGIN, FRAME_RAISED | ICON_ABOVE_TEXT );
new FXButton (vv, "Update available plugins list", NULL, this, ID_UPDATE_CMD_PLUGIN_LIST, FRAME_RAISED | ICON_ABOVE_TEXT );



string plugin_path = string(PATH_LIBDIR) + "/openspace/plugins/cmddialog";




	    dirp = opendir (plugin_path.c_str ());
	    if(dirp)
	    while ((dp = readdir (dirp)) != NULL)
	    {
		if (dp->d_name[0] != '.' || (dp->d_name[1] != '\0' && (dp->d_name[1] != '.' || dp->d_name[2] != '\0')))
		{
		string name=dp->d_name;
			if(name.substr(name.length()-3,3)==".so")
			{
			name=name.substr(0,name.length()-3);
			name=name.substr(3,name.length()-3);
			commandPluginsList->appendItem(name.c_str());
			}
			
		    
		}
	    }

	closedir (dirp);

plugin_path = FXFile::getUserDirectory ("").text ()+string("/.openspace/plugins/cmddialog");
	
	dirp = opendir (plugin_path.c_str ());
	    if(dirp)
	    while ((dp = readdir (dirp)) != NULL)
	    {
		if (dp->d_name[0] != '.' || (dp->d_name[1] != '\0' && (dp->d_name[1] != '.' || dp->d_name[2] != '\0')))
		{
		string name=dp->d_name;
			if(name.substr(name.length()-3,3)==".so")
			{
			name=name.substr(0,name.length()-3);
			name=name.substr(3,name.length()-3);
			commandPluginsList->appendItem(name.c_str());
			}
			
		    
		}
	    }

	    closedir (dirp);

    




    commandsPane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXLabel (commandsPane, "Commands settings", NULL, LAYOUT_LEFT);

    new FXButton (buttons, "Commands Settings", NULL, switcher, FXSwitcher::ID_OPEN_FOURTH, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);

    commandsCombo=new FXListBox (commandsPane, this, ID_COMMAND_CHANGE);
    new FXLabel (commandsPane, "icon: ", NULL, LAYOUT_LEFT);
    iconsList=new FXListBox (commandsPane);
    commandsCombo->setNumVisible(30);
    iconsList->setNumVisible(30);
    new FXLabel (commandsPane, "exec: ", NULL, LAYOUT_LEFT);
    commandsTextfield = new FXTextField (commandsPane, 20);
    
    
    
    new FXLabel (commandsPane, "shortuct mask: ", NULL, LAYOUT_LEFT);
    shortcutMaskList=new FXListBox (commandsPane);
    shortcutMaskList->setNumVisible(3);
    shortcutMaskList->appendItem("");
    shortcutMaskList->appendItem("control");
    shortcutMaskList->appendItem("alt");
    
    
    new FXLabel (commandsPane, "shortuct: ", NULL, LAYOUT_LEFT);
    shortcutList=new FXListBox (commandsPane);
    shortcutList->setNumVisible(30);
    
    
   	map<string,unsigned int>::iterator s_iter;
	
	shortcutList->appendItem("");
	for(s_iter=objmanager->key_map.begin();s_iter!=objmanager->key_map.end();s_iter++)
	{
	shortcutList->appendItem(s_iter->first.c_str());
	}
	
    new FXLabel (commandsPane, "text: ", NULL, LAYOUT_LEFT);
    commandsTextfieldText = new FXTextField (commandsPane, 20);
    commandsType=new FXLabel (commandsPane, "", NULL, LAYOUT_LEFT);
    
    
    vv = new FXGroupBox (commandsPane, "Options", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0);
    commandsRescan = new FXCheckButton (vv, "rescan", NULL, 0, JUSTIFY_LEFT | JUSTIFY_TOP | ICON_BEFORE_TEXT | LAYOUT_SIDE_TOP);
    commandsCapture = new FXCheckButton (vv, "capture", NULL, 0, JUSTIFY_LEFT | JUSTIFY_TOP | ICON_BEFORE_TEXT | LAYOUT_SIDE_TOP);
   

    if (conf->openxpath ("/OpenspaceConfig/commands") != -1)
    {
	

	

command_container *ctlast;
string res;
	while (conf->getnextnode (res))
	{
		
	    command_container ct;
	    ctlast=&ct;
	    commandsCombo->appendItem(res.c_str (),objmanager->osicons["execute"]);
	    ct.name = res;
	    configure conflocal = *conf;
	    ct.exec = conflocal.readonestring ("/OpenspaceConfig/commands/" + res + "/exec");
	    
	    string options = conflocal.readonestring ("/OpenspaceConfig/commands/" + res + "/options");

	    string::size_type pos = options.find ("capture");
	    if (pos != string::npos)
		ct.capture=true;
	    else
		ct.capture=false;


	    pos = options.find ("rescan");
	    if (pos != string::npos)
		ct.rescan=true;
	    else
		ct.rescan=false;
		
		ct.text=conflocal.readonestring ("/OpenspaceConfig/commands/" + res + "/text");
		ct.icon=conflocal.readonestring ("/OpenspaceConfig/commands/" + res + "/icon");
		ct.type=conflocal.readonestring ("/OpenspaceConfig/commands/" + res + "/type");
		ct.key=conflocal.readonestring ("/OpenspaceConfig/commands/" + res + "/key");
		ct.key_mask=conflocal.readonestring ("/OpenspaceConfig/commands/" + res + "/key_mask");
		
	    commandsMap[ct.name]=ct;

	}

	    commandsRescan->setCheck(ctlast->rescan);
	    commandsCapture->setCheck(ctlast->capture);
	    commandsTextfield->setText (ctlast->exec.c_str ());
            commandsCombo->setCurrentItem (commandsCombo->getNumItems () - 1);
	    currentCommandName=ctlast->name;
	    
	    string str;
	    if(ctlast->type=="")
	    str="command type: EXTERNAL";
	    else
	    str="command type: " + ctlast->type;
	    
	    commandsType->setText(str.c_str());
	    string shorticonname;
	    
	    
	
	   map < string, FXIcon * >::iterator iter;
	   
    iconsList->appendItem("",NULL);
    for (iter = objmanager->osicons.begin (); iter != objmanager->osicons.end (); iter++)
    {
	FXIcon *icon = iter->second;
	if(icon!=NULL)
	iconsList->appendItem(iter->first.c_str(),icon);
    }	
	if(ctlast->icon!="")
	iconsList->setCurrentItem(iconsList->findItem(ctlast->icon.c_str()));
	
	if(ctlast->key!="")
	shortcutList->setCurrentItem(shortcutList->findItem(ctlast->key.c_str()));

	if(ctlast->key_mask!="")
	shortcutMaskList->setCurrentItem(shortcutMaskList->findItem(ctlast->key_mask.c_str()));
	
	
	
	new FXSeparator(commandsPane);
	FXHorizontalFrame *hfr=new FXHorizontalFrame (commandsPane, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	removeCommandButton=new FXButton (hfr, "Remove", NULL, this, ID_REMOVE_COMMAND, FRAME_RAISED | ICON_ABOVE_TEXT );
	new FXButton (hfr, "New Command", NULL, this, ID_NEW_COMMAND, FRAME_RAISED | ICON_ABOVE_TEXT);
	newCommandEdit = new FXTextField (hfr, 20);

	if(ctlast->type=="INTERNAL" || ctlast->type=="PLUGIN")
	{
	commandsTextfield->disable();
	commandsRescan->disable();
	commandsCapture->disable();
	removeCommandButton->disable();
	}

    }


    filetypePane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    
    new FXButton (buttons, "File types Settings", NULL, switcher, FXSwitcher::ID_OPEN_FIFTH, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);



    if (conf->openxpath ("/OpenspaceConfig/file_types") != -1)
    {
	filetype_container *ctlast;
	new FXLabel(filetypePane,"File Type:");
	fileTypeList=new FXListBox (filetypePane, this, ID_FILETYPE_CHANGE);
	fileTypeList->setNumVisible(30);
	
	
	FXGroupBox *filetypeGroup=new FXGroupBox(filetypePane,"commands",GROUPBOX_TITLE_CENTER|FRAME_RIDGE|LAYOUT_FILL_X);
	
	
	new FXLabel(filetypeGroup,"default command:");
	fileTypeDefaultBox = new FXListBox (filetypeGroup);
	fileTypeDefaultBox->setNumVisible(30);
	
	
	
	FXHorizontalFrame* hzframe=new FXHorizontalFrame (filetypeGroup, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	FXVerticalFrame* vframe0=new FXVerticalFrame (hzframe, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(vframe0,"additional commands:");
        additionalCommands=new FXList (vframe0,NULL, 0,LIST_NORMAL| LAYOUT_FIX_WIDTH, 0, 0,250);
	additionalCommands->setNumVisible(5);
	FXVerticalFrame* vframe=new FXVerticalFrame (hzframe, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(vframe,"\n");
	new FXLabel(vframe,"\n");
	new FXArrowButton(vframe,this,ID_ADD_COMMAND_ADDITIONAL,FRAME_RAISED|FRAME_THICK|ARROW_LEFT);
	new FXArrowButton(vframe,this,ID_DEL_COMMAND_ADDITIONAL,FRAME_RAISED|FRAME_THICK|ARROW_RIGHT);
	FXVerticalFrame* vframe1=new FXVerticalFrame (hzframe, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(vframe1,"all available commands:");
	additionalCommandsAvailable=new FXList (vframe1,NULL, 0,LIST_NORMAL| LAYOUT_FIX_WIDTH, 0, 0,250);
	additionalCommandsAvailable->setNumVisible(5);

	filetypeGroup=new FXGroupBox(filetypePane,"colors and icon",GROUPBOX_TITLE_CENTER|FRAME_RIDGE|LAYOUT_FILL_X);
	FXHorizontalFrame* hf0=new FXHorizontalFrame (filetypeGroup, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 10, 10);
	
	iconsList2=new FXListBox (hf0);
        iconsList2->setNumVisible(30);
	//new FXLabel(hf0,"  ");
	vframe1=new FXVerticalFrame (hf0, 0 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(vframe1,"text color:");
	colorbutton=new FXButton(vframe1,"Color",NULL,this,ID_CHOOSE_COLOR);
	vframe1=new FXVerticalFrame (hf0, 0 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(vframe1,"back color:");
	backcolorbutton=new FXButton(vframe1,"Back Color",NULL,this,ID_CHOOSE_BACKCOLOR);
	
	
	
	filetypeGroup=new FXGroupBox(filetypePane,"add new file type",GROUPBOX_TITLE_CENTER|FRAME_RIDGE|LAYOUT_FILL_X);
	
	allMime=new FXComboBox (filetypeGroup,60);
	allMime->setNumVisible(30);
	new FXButton (filetypeGroup, "Add", NULL, this, preferences::ID_ADD_FILETYPE);
	
	MimeType::__initialize();
	map<string,string>::iterator iter0;
	for(iter0=MimeType::mimeMap.begin();iter0!=MimeType::mimeMap.end();iter0++)
	{
		if(allMime->findItem(xml2mime(iter0->second).c_str())==-1)
		{	
		allMime->appendItem(xml2mime(iter0->second).c_str());
		}
	}
	
	 
	
	
	

	string res;
	while (conf->getnextnode (res))
	{

	
	    configure conflocal = *conf;
	   if(conflocal.openxpath ("/OpenspaceConfig/file_types/"+res+"/types") != -1)
	   {
	   string res2;
	    while (conflocal.getnextnode (res2))
		{
	 	 
			
			 filetype_container ct;
			 ct.load( res+"/"+res2);
			 fileTypeList->appendItem(xml2mime(ct.name).c_str (),objmanager->osicons["unknown"]);	    
	   		 
	   		 
			 filetypesMap[ct.name]=ct;  
			
		}
		
	   }	
	    filetype_container ct;
	    ct.load( res);
	    ctlast=&ct;
	    fileTypeList->appendItem(xml2mime(ct.name) .c_str (),objmanager->osicons["unknown"]);	    
	    
	   
	filetypesMap[ct.name]=ct;
	}

	filetypestring = ctlast->name;

	 map < string, FXIcon * >::iterator iter2;
	 iconsList2->appendItem("",NULL);
    	for (iter2 = objmanager->osicons.begin (); iter2 != objmanager->osicons.end (); iter2++)
    	{
	FXIcon *icon = iter2->second;
	if(icon!=NULL && (iter2->first.length()<4 || iter2->first.substr(0,4)!="big_"))
	iconsList2->appendItem(iter2->first.c_str(),icon);
    	}	
	if(ctlast->icon!="")
	iconsList2->setCurrentItem(iconsList2->findItem(ctlast->icon.c_str()));
	
	
	
	fileTypeDefaultBox->appendItem ("",objmanager->osicons["execute"]);
	
	if (conf->openxpath ("/OpenspaceConfig/commands") != -1)
	{
	string command;
	    while (conf->getnextnode (command))
	    {
		
		fileTypeDefaultBox->appendItem (command.c_str (),objmanager->osicons["execute"]);
		string icon=conf->readonestring ("/OpenspaceConfig/commands/" + command+ "/icon");
		additionalCommandsAvailable->appendItem (command.c_str ());
		additionalCommandsAvailableForButtons->appendItem (command.c_str (),objmanager->osicons[icon]);
	    }
	}
	
	vector<string>::iterator iter;
	for(iter=ctlast->commands.begin();iter!=ctlast->commands.end();iter++)
	{
	additionalCommands->appendItem(iter->c_str());
	}
	                         
	       
	
	fileTypeList->setCurrentItem (fileTypeList->getNumItems () - 1);
	fileTypeDefaultBox->setCurrentItem(fileTypeDefaultBox->findItem(ctlast->command.c_str()));
	
        setAllColor(colorbutton,readcolor(ctlast->color));
	setAllColor(backcolorbutton,readcolor2(ctlast->backcolor));
	
    }






  new FXButton (buttons, "Speed and delays", NULL, switcher, FXSwitcher::ID_OPEN_SIXTH, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);


  FXHorizontalFrame *hframe = new FXHorizontalFrame(switcher,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0,0,0);

  FXMatrix* matrix = new FXMatrix(hframe,3,LAYOUT_FILL_Y|MATRIX_BY_COLUMNS,0,0,0,0,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING);

  const FXuint spinnerstyle=FRAME_SUNKEN|FRAME_THICK;

  new FXLabel(matrix,"Typing Speed\t\tTyping Speed",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  FXSpinner *spinner=new FXSpinner(matrix,4,&target_typingspeed,FXDataTarget::ID_VALUE,spinnerstyle);
  spinner->setRange(1,9999);
  spinner->setIncrement(500);
  new FXLabel(matrix,"ms",NULL,LAYOUT_CENTER_Y);


  new FXLabel(matrix,"Double Click Speed\t\tDouble Click Speed",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  spinner=new FXSpinner(matrix,4,&target_clickspeed,FXDataTarget::ID_VALUE,spinnerstyle);
  spinner->setRange(1,9999);
  spinner->setIncrement(100);
  new FXLabel(matrix,"ms",NULL,LAYOUT_CENTER_Y);

  new FXLabel(matrix,"Scroll Speed\t\tScroll Speed",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  spinner=new FXSpinner(matrix,4,&target_scrollspeed,FXDataTarget::ID_VALUE,spinnerstyle);
  spinner->setRange(1,9999);
  spinner->setIncrement(10);
  new FXLabel(matrix,"ms",NULL,LAYOUT_CENTER_Y);

  new FXLabel(matrix,"Scroll Delay\t\tScroll Delay",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  spinner=new FXSpinner(matrix,4,&target_scrolldelay,FXDataTarget::ID_VALUE,spinnerstyle);
  spinner->setRange(1,9999);
  spinner->setIncrement(100);
  new FXLabel(matrix,"ms",NULL,LAYOUT_CENTER_Y);

  new FXLabel(matrix,"Cursor Blink Speed\t\tCursor Blink Speed",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  spinner=new FXSpinner(matrix,4,&target_blinkspeed,FXDataTarget::ID_VALUE,spinnerstyle);
  spinner->setRange(1,9999);
  spinner->setIncrement(100);
  new FXLabel(matrix,"ms",NULL,LAYOUT_CENTER_Y);

  new FXLabel(matrix,"Animation Speed\t\tAnimation Speed",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  new FXSpinner(matrix,4,&target_animspeed,FXDataTarget::ID_VALUE,spinnerstyle);
  spinner->setRange(1,9999);
  spinner->setIncrement(1);
  new FXLabel(matrix,"ms",NULL,LAYOUT_CENTER_Y);

  new FXLabel(matrix,"Cascade Menu Popup Delay\t\tAmount of delay before cascading menu is shown",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  spinner=new FXSpinner(matrix,4,&target_menupause,FXDataTarget::ID_VALUE,spinnerstyle);
  spinner->setRange(1,9999);
  spinner->setIncrement(100);
  new FXLabel(matrix,"ms",NULL,LAYOUT_CENTER_Y);

  new FXLabel(matrix,"Tooltip Popup Delay\t\tAmount of Delay before tooltip is shown ",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  spinner=new FXSpinner(matrix,4,&target_tooltippause,FXDataTarget::ID_VALUE,spinnerstyle);
  spinner->setRange(1,9999);
  spinner->setIncrement(100);
  new FXLabel(matrix,"ms",NULL,LAYOUT_CENTER_Y);

  new FXLabel(matrix,"Tooltip Time\t\tTime that tooltips are shown",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  spinner=new FXSpinner(matrix,4,&target_tooltiptime,FXDataTarget::ID_VALUE,spinnerstyle);
  spinner->setRange(1,9999);
  spinner->setIncrement(500);
  new FXLabel(matrix,"ms",NULL,LAYOUT_CENTER_Y);

  new FXLabel(matrix,"Drag Delta\t\tMinimum distance considered a mouse move",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  spinner=new FXSpinner(matrix,4,&target_dragdelta,FXDataTarget::ID_VALUE,spinnerstyle);
  spinner->setRange(1,9999);
  spinner->setIncrement(1);
  new FXFrame(matrix,FRAME_NONE);

  new FXLabel(matrix,"Wheel Lines",NULL,LAYOUT_RIGHT|LAYOUT_CENTER_Y);
  spinner=new FXSpinner(matrix,4,&target_wheellines,FXDataTarget::ID_VALUE,spinnerstyle);
  spinner->setRange(1,100);





    vfsPane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXLabel (vfsPane, "VFS settings", NULL, LAYOUT_LEFT);

    new FXButton (buttons, "Virtual File System Settings", NULL, switcher, FXSwitcher::ID_OPEN_SEVENTH, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);

    
    
        vfsList=new FXListBox (vfsPane, this, ID_VFS_CHANGE);
        vfsList->setNumVisible(10);
        
	    
    
        FXHorizontalFrame *hzframe=new FXHorizontalFrame (vfsPane, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	FXVerticalFrame *vframe0=new FXVerticalFrame (hzframe, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(vframe0,"visible headers:");
        headersList=new FXList (vframe0,NULL, 0,LIST_NORMAL| LAYOUT_FIX_WIDTH, 0, 0,250);
	headersList->setNumVisible(5);
	FXVerticalFrame* vframe=new FXVerticalFrame (hzframe, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(vframe,"\n");
	new FXLabel(vframe,"\n");
	new FXArrowButton(vframe,this,ID_ADD_HEADER,FRAME_RAISED|FRAME_THICK|ARROW_LEFT);
	new FXArrowButton(vframe,this,ID_DEL_HEADER,FRAME_RAISED|FRAME_THICK|ARROW_RIGHT);
	FXVerticalFrame *vframe1=new FXVerticalFrame (hzframe, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(vframe1,"all available headers:");
	availableHeadersList=new FXList (vframe1,NULL, 0,LIST_NORMAL| LAYOUT_FIX_WIDTH, 0, 0,250);
	availableHeadersList->setNumVisible(5);

 new FXLabel(vfsPane,""); 
 new FXSeparator(vfsPane);
 new FXLabel(vfsPane,"");

vv = new FXGroupBox (vfsPane, "available plugins - download from internet", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0); 

hfr=new FXHorizontalFrame (vv, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
availableVfsPluginsList=new FXListBox (hfr);
availableVfsPluginsList->setNumVisible(30);
new FXButton (hfr, "Download and Install", NULL, this, ID_DOWNLOAD_INSTALL_VFS_PLUGIN, FRAME_RAISED | ICON_ABOVE_TEXT );
new FXButton (vv, "Update available plugins list", NULL, this, ID_UPDATE_VFS_PLUGIN_LIST, FRAME_RAISED | ICON_ABOVE_TEXT );  
  
    
    
    if (conf->openxpath ("/OpenspaceConfig/filelist") != -1)
	{
	string command;
	    while (conf->getnextnode (command))
	    {
	    vfsList->appendItem(command.c_str());
	    }
	}    
    
actualvfs="";
       
	this->onVfsChange(NULL,0,NULL);


}

long preferences::onVfsChange (FXObject * sender, FXSelector sel, void *)
{

if(actualvfs!="")//not first run, save
{
conf->removestring ("/OpenspaceConfig/filelist/"+actualvfs+"/headers");
conf->addstring("/OpenspaceConfig/filelist/"+actualvfs,"headers","");

for (int c = 0; c < headersList->getNumItems (); c++)
	    {
		
		string cmd= headersList->getItemText (c).text ();
		conf->addstring("/OpenspaceConfig/filelist/"+actualvfs+"/headers","header",cmd);  
	    }	


}

actualvfs=vfsList->getItem(vfsList->getCurrentItem()).text();
availableHeadersList->clearItems();
if (conf->openxpath ("/OpenspaceConfig/filelist/"+actualvfs+"/properties") != -1)
	{
	string command;
	    while (conf->getnextnode (command))
	    {
	    	if(command!="name")
		availableHeadersList->appendItem (command.c_str ());
	    }
	}
headersList->clearItems();
if (conf->openxpath ("/OpenspaceConfig/filelist/"+actualvfs+"/headers/header") != -1)
	{
	string command;
	    while (conf->getnextstring (command))
	    {
		headersList->appendItem (command.c_str ());
	    }
	}



}

long preferences::onAddButtonCommand (FXObject * sender, FXSelector sel, void *)
{

FXushort id=FXSELID(sel);
string actual_toolbar=toolbarList->getItem(toolbarList->getCurrentItem()).text();

	if(id==ID_ADD_BUTTON_COMMAND)
	{
	
	
	string cmd=additionalCommandsAvailableForButtons->getItemText(additionalCommandsAvailableForButtons->getCurrentItem()).text();
	toolbarVector.push_back(toolbar_container(actual_toolbar,cmd));
	buttonsList->appendItem(cmd.c_str());
	}
	else
	{
	
	string cmd=buttonsList->getItemText(buttonsList->getCurrentItem()).text();
	buttonsList->removeItem(buttonsList->getCurrentItem());
	
	vector <toolbar_container>::iterator iter;
	
	for(iter=toolbarVector.begin();iter!=toolbarVector.end(); iter++)
		{
			if(iter->command==cmd && iter->toolbar==actual_toolbar)
			{
			toolbarVector.erase(iter);
			break;
			}
		}
			
	}


}

long preferences::onAddHeader (FXObject * sender, FXSelector sel, void *)
{

FXushort id=FXSELID(sel);

	if(id==ID_ADD_HEADER)
	{
	headersList->appendItem(availableHeadersList->getItemText(availableHeadersList->getCurrentItem()));
	}
	else
	{
	headersList->removeItem(headersList->getCurrentItem());
	}



}

long preferences::onAddShutterCommand (FXObject * sender, FXSelector sel, void *)
{

FXushort id=FXSELID(sel);
string actual_shutter=shutterList->getItem(shutterList->getCurrentItem()).text();

	if(id==ID_ADD_SHUTTER_COMMAND)
	{
	
	
	string cmd=additionalCommandsAvailableForButtons->getItemText(additionalCommandsAvailableForButtons->getCurrentItem()).text();
		if(cmd!="SEPARATOR")
		{
		shutterVector.push_back(shutter_container(actual_shutter,cmd));
		shutterCommands->appendItem(cmd.c_str());
		}
	}
	else
	{
	
	string cmd=shutterCommands->getItemText(shutterCommands->getCurrentItem()).text();
	shutterCommands->removeItem(shutterCommands->getCurrentItem());
	
	vector <shutter_container>::iterator iter;
	
	for(iter=shutterVector.begin();iter!=shutterVector.end(); iter++)
		{
			if(iter->command==cmd && iter->shutter==actual_shutter)
			{
			shutterVector.erase(iter);
			break;
			}
		}
			
	}



}


void preferences::setAllColor(FXButton* button,FXColor color)
{
 button->setBaseColor(color);
 button->setBorderColor(color);
 button->setShadowColor(color);
 button->setHiliteColor(color);
 button->setTextColor(color);
 button->setBackColor(color);
}

 long preferences::onChooseColor (FXObject * sender, FXSelector sel, void *)
 {

 if(!colordlg->execute())
return 0;

  
 FXColor color=colordlg->getRGBA();
 FXushort id=FXSELID(sel);
 if(id==ID_CHOOSE_COLOR)
 setAllColor(colorbutton,color);
 else
 setAllColor(backcolorbutton,color);
 

 }	

long preferences::onAddFiletype (FXObject * sender, FXSelector sel, void *)
{
string mime=allMime->getText().text();

filetype_container ct=filetypesMap[mime];
    if(ct.name!="") // already exists
    return 0;
  
  ct.name=mime;  
  filetypesMap[mime]=ct;    

fileTypeList->appendItem(ct.name.c_str (),objmanager->osicons["unknown"]);  
this->onFileTypeChange(NULL,0,NULL);  
}

preferences::~preferences ()
{
}

long preferences::save (void)
{
    FXTRACE ((5, "save\n"));
    
    this->onCommandChange(NULL,0,NULL);
    this->onFileTypeChange(NULL,0,NULL);
    this->onVfsChange(NULL,0,NULL);

    map < string, command_container >::iterator iter;

    for (iter = commandsMap.begin (); iter != commandsMap.end (); iter++)
    {
	command_container ct = iter->second;	
	ct.save();		 
    }

map < string, filetype_container >::iterator iter2;

    for (iter2 = filetypesMap.begin (); iter2 != filetypesMap.end (); iter2++)
    {
	filetype_container ct = iter2->second;
	ct.save();
    }

    conf->saveonestring ("/OpenspaceConfig/mainwindow/width", mainwindow_width->getText ().text ());
    conf->saveonestring ("/OpenspaceConfig/mainwindow/height", mainwindow_height->getText ().text ());

conf->removestring ("/OpenspaceConfig/button_commands");
conf->addstring("/OpenspaceConfig","button_commands","");

for (int c = 0; c < buttonsList->getNumItems (); c++)
	    {		
		  string cmd= buttonsList->getItemText (c).text ();
		  conf->addstring("/OpenspaceConfig/button_commands","command",cmd);
	    }


conf->removestring ("/OpenspaceConfig/shutter");
conf->addstring("/OpenspaceConfig","shutter","");

vector <shutter_container>::iterator shutter_iter;
	
	for(shutter_iter=shutterVector.begin();shutter_iter!=shutterVector.end(); shutter_iter++)
		{
		string res;
			if(!conf->readonestring("/OpenspaceConfig/shutter/"+shutter_iter->shutter,res))
			conf->addstring("/OpenspaceConfig/shutter",shutter_iter->shutter,"");
				
			conf->addstring("/OpenspaceConfig/shutter/"+shutter_iter->shutter,"command",shutter_iter->command);
	
		}

conf->removestring ("/OpenspaceConfig/toolbars");
conf->addstring("/OpenspaceConfig","toolbars","");

vector <toolbar_container>::iterator toolbar_iter;
	
	for(toolbar_iter=toolbarVector.begin();toolbar_iter!=toolbarVector.end(); toolbar_iter++)
		{
		string res;
			if(!conf->readonestring("/OpenspaceConfig/toolbars/"+toolbar_iter->toolbar,res))
			conf->addstring("/OpenspaceConfig/toolbars",toolbar_iter->toolbar,"");
				
			conf->addstring("/OpenspaceConfig/toolbars/"+toolbar_iter->toolbar,"command",toolbar_iter->command);
	
		}



conf->saveonestring ("/OpenspaceConfig/leftdir/dir",leftdir->getText().text());
conf->saveonestring ("/OpenspaceConfig/rightdir/dir",rightdir->getText().text());
conf->saveonestring ("/OpenspaceConfig/defaultdir/dir",defaultdir->getText().text());
conf->saveonestring ("/OpenspaceConfig/filelist/local/thumbs/size",thumbsize->getText().text());
conf->saveonestring ("/OpenspaceConfig/icons_theme",iconsTheme->getItem(iconsTheme->getCurrentItem()).text());


conf->saveonestring ("/OpenspaceConfig/fonts/captionfont",font1->getText().text());
conf->saveonestring ("/OpenspaceConfig/fonts/captionfont1",font2->getText().text());
conf->saveonestring ("/OpenspaceConfig/fonts/captionfont2",font3->getText().text());


conf->saveonestring ("/OpenspaceConfig/speed_delay/typing_speed",ntos(typingSpeed));
conf->saveonestring ("/OpenspaceConfig/speed_delay/click_speed",ntos(clickSpeed));
conf->saveonestring ("/OpenspaceConfig/speed_delay/scroll_speed",ntos(scrollSpeed));
conf->saveonestring ("/OpenspaceConfig/speed_delay/scroll_delay",ntos(scrollDelay));
conf->saveonestring ("/OpenspaceConfig/speed_delay/blink_speed",ntos(blinkSpeed));
conf->saveonestring ("/OpenspaceConfig/speed_delay/anim_speed",ntos(animSpeed));
conf->saveonestring ("/OpenspaceConfig/speed_delay/menu_pause",ntos(menuPause));
conf->saveonestring ("/OpenspaceConfig/speed_delay/tooltip_pause",ntos(tooltipPause));
conf->saveonestring ("/OpenspaceConfig/speed_delay/tooltip_time",ntos(tooltipTime));
conf->saveonestring ("/OpenspaceConfig/speed_delay/drag_delta",ntos(dragDelta));
conf->saveonestring ("/OpenspaceConfig/speed_delay/wheel_lines",ntos(wheelLines));
}

long preferences::onCommandChange (FXObject * sender, FXSelector sel, void *)
{

command_container *ct_prev=&commandsMap[currentCommandName];
ct_prev->exec=commandsTextfield->getText().text();
ct_prev->capture=commandsCapture->getCheck();
ct_prev->rescan=commandsRescan->getCheck();
ct_prev->text=commandsTextfieldText->getText().text();
ct_prev->icon=iconsList->getItem(iconsList->getCurrentItem()).text();
ct_prev->key=shortcutList->getItem(shortcutList->getCurrentItem()).text();
ct_prev->key_mask=shortcutMaskList->getItem(shortcutMaskList->getCurrentItem()).text();

command_container ct=commandsMap[commandsCombo->getItem (commandsCombo->getCurrentItem ()).text ()];

commandsRescan->setCheck(ct.rescan);
commandsCapture->setCheck(ct.capture);
commandsTextfield->setText (ct.exec.c_str ());
commandsTextfieldText->setText (ct.text.c_str ());
           
	    string str;
	    if(ct.type=="")
	    str="command type: EXTERNAL";
	    else
	    str="command type: " + ct.type;
	    
	    commandsType->setText(str.c_str());


	if(ct.type=="INTERNAL" || ct.type=="PLUGIN")
	{
	commandsTextfield->disable();
	commandsRescan->disable();
	commandsCapture->disable();
	removeCommandButton->disable();
	}
	else
	{
	commandsTextfield->enable();
	commandsRescan->enable();
	commandsCapture->enable();
	removeCommandButton->enable();
	}
iconsList->setCurrentItem(iconsList->findItem(ct.icon.c_str()));
shortcutList->setCurrentItem(shortcutList->findItem(ct.key.c_str()));
shortcutMaskList->setCurrentItem(shortcutMaskList->findItem(ct.key_mask.c_str()));


currentCommandName=ct.name;

}


long preferences::onFileTypeChange (FXObject * sender, FXSelector sel, void *)
{

filetype_container *ct_prev=&filetypesMap[currentFileType];
ct_prev->command=fileTypeDefaultBox->getItem(fileTypeDefaultBox->getCurrentItem()).text();

ct_prev->commands.clear();
for(int i=0;i<additionalCommands->getNumItems ();i++)
	{
	ct_prev->commands.push_back(additionalCommands->getItemText(i).text());
	}

FXchar buffer[60];
fxnamefromcolor(buffer,colorbutton->getBackColor());

ct_prev->color=buffer;

fxnamefromcolor(buffer,backcolorbutton->getBackColor());

ct_prev->backcolor=buffer;
ct_prev->icon=iconsList2->getItem(iconsList2->getCurrentItem()).text();

filetype_container ct=filetypesMap[mime2xml(fileTypeList->getItem (fileTypeList->getCurrentItem ()).text ())];
fileTypeDefaultBox->setCurrentItem(fileTypeDefaultBox->findItem(ct.command.c_str()));
additionalCommands->clearItems();

iconsList2->setCurrentItem(iconsList2->findItem(ct.icon.c_str()));

vector<string>::iterator iter;
	for(iter=ct.commands.begin();iter!=ct.commands.end();iter++)
	{
	additionalCommands->appendItem(iter->c_str());
	}

 setAllColor(colorbutton,readcolor(ct.color));
 setAllColor(backcolorbutton,readcolor2(ct.backcolor));
	

currentFileType=ct.name;

}

long preferences::onShutterChange (FXObject * sender, FXSelector sel, void *)
{
string actual_shutter=shutterList->getItem(shutterList->getCurrentItem()).text();
shutterCommands->clearItems();
vector <shutter_container>::iterator iter;
	for(iter=shutterVector.begin();iter!=shutterVector.end();iter++)
	{
		if(iter->shutter==actual_shutter)
	shutterCommands->appendItem(iter->command.c_str());
	}

}

long preferences::onToolbarChange (FXObject * sender, FXSelector sel, void *)
{
string actual_toolbar=toolbarList->getItem(toolbarList->getCurrentItem()).text();
buttonsList->clearItems();
vector <toolbar_container>::iterator iter;
	for(iter=toolbarVector.begin();iter!=toolbarVector.end();iter++)
	{
		if(iter->toolbar==actual_toolbar)
		{
		string icon=conf->readonestring ("/OpenspaceConfig/commands/" + iter->command + "/icon");
		buttonsList->appendItem(iter->command.c_str(),objmanager->osicons[icon] );
		}
	}

}


bool preferences::validateName(string name)
{
	if(name=="")return false;

	int p,k;
	FXRex identifier("[0-9a-zA-Z]*");
        identifier.match(name.c_str(),name.size(),&p,&k);
	

	if(k!=name.size())
	{
	FXMessageBox error (this, "error", "Incorrect characters in name", NULL, MBOX_OK | DECOR_TITLE | DECOR_BORDER);
	error.execute();
	return false;
	}
	else
	{
	return true;
	}
    
	
}

long preferences::onNewShutter (FXObject * sender, FXSelector sel, void *)
{
string shutter_name = newShutterEdit->getText ().text ();
    if(!validateName(shutter_name))
    return 0;

shutterList->appendItem(shutter_name.c_str());

}

long preferences::onRemoveShutter (FXObject * sender, FXSelector sel, void *)
{

    string shutter =  shutterList->getItem (shutterList->getCurrentItem ()).text ();
    int cur=shutterList->getCurrentItem ();
    shutterList->setCurrentItem (0);
    shutterList->removeItem (cur);    
    this->onShutterChange(NULL,0,NULL);
    
    
       string res = "/OpenspaceConfig/shutter/"+shutter;
       conf->removestring (res);


vector <shutter_container>::iterator shutter_iter;
	
	for(shutter_iter=shutterVector.begin();shutter_iter!=shutterVector.end(); shutter_iter++)
		{
			if(shutter_iter->shutter==shutter)
			{
				shutterVector.erase(shutter_iter);
			break;
			}
		}


}


long preferences::onNewCommand (FXObject * sender, FXSelector sel, void *)
{

    string command_name = newCommandEdit->getText ().text ();
    if(!validateName(command_name))
    return 0;
    
    command_container ct=commandsMap[command_name];
    if(ct.name!="") // already exists
    return 0;
    
    ct.name=command_name;
    ct.rescan=false;
    ct.capture=false;
    commandsCombo->appendItem(ct.name.c_str());
    commandsMap[command_name]=ct;   
    commandsCombo->setCurrentItem (commandsCombo->getNumItems () - 1);
    this->onCommandChange(NULL,0,NULL);
    
    fileTypeDefaultBox->appendItem(ct.name.c_str());
    additionalCommands->appendItem(ct.name.c_str());


}

long preferences::onRemoveCommand (FXObject * sender, FXSelector sel, void *)
{

    string command =  commandsCombo->getItem (commandsCombo->getCurrentItem ()).text ();
    int cur=commandsCombo->getCurrentItem ();
    commandsCombo->setCurrentItem (0);
    commandsCombo->removeItem (cur);    
    this->onCommandChange(NULL,0,NULL);
    
    
       string res = "/OpenspaceConfig/commands/"+command;
       conf->removestring (res);


}

long preferences:: onAdditionalCommandChange( FXObject * sender, FXSelector sel, void *)
{
FXushort id=FXSELID(sel);
filetype_container *ct_prev=&filetypesMap[currentFileType];
	if(id==ID_ADD_COMMAND_ADDITIONAL)
	{
	ct_prev->commands.push_back(additionalCommandsAvailable->getItemText(additionalCommandsAvailable->getCurrentItem()).text());
	additionalCommands->appendItem(additionalCommandsAvailable->getItemText(additionalCommandsAvailable->getCurrentItem()));
	}
	else
	{
		vector <string>::iterator iter = find(ct_prev->commands.begin(), ct_prev->commands.end(),additionalCommands->getItemText( additionalCommands->getCurrentItem()).text());
		if(iter != ct_prev->commands.end())
		{
		ct_prev->commands.erase(iter);
		}
	additionalCommands->removeItem(additionalCommands->getCurrentItem());
	
	


	
	}

}

long preferences::onOpenMimeApp (FXObject * sender, FXSelector sel, void *)
{
if(mimeapp==NULL)
{
mimeapp=new MimeApp(this);


FXushort id=FXSELID(sel);

	if(id==ID_MIME_APP_AUTO)
	{	
	mimeapp->doAutomaticConfiguration();
	}
	else
	{
	mimeapp->create();
	mimeapp->execute (PLACEMENT_OWNER);
	}


saveconfiguration=false;
this->handle (this, FXSEL (SEL_CLOSE, 0), NULL);
}

   
	
}
