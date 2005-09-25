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

FXDEFMAP (preferences) preferencesMap[] =
{
    //________Message_Type_____________________ID____________Message_Handler_______  
	//FXMAPFUNC (SEL_COMMAND, preferences::ID_SAVE, preferences::onSave),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_COMMAND_CHANGE, preferences::onCommandChange),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_FILETYPE_CHANGE, preferences::onFileTypeChange), 
	FXMAPFUNC (SEL_COMMAND, preferences::ID_SHUTTER_CHANGE, preferences::onShutterChange),	
	FXMAPFUNC (SEL_COMMAND, preferences::ID_VFS_CHANGE, preferences::onVfsChange),	
	FXMAPFUNC (SEL_COMMAND, preferences::ID_NEW_COMMAND, preferences::onNewCommand), 
	FXMAPFUNC (SEL_COMMAND, preferences::ID_REMOVE_COMMAND, preferences::onRemoveCommand),
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

FXMessageBox about (this, "restart", "restart openspace to apply changes", NULL, MBOX_OK | DECOR_TITLE | DECOR_BORDER);
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
			string cmd="cd "+ string(FXFile::getUserDirectory ("").text ()) +"/.openspace/plugins/cmddialog/ && wget -nc "+ download;
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
			string cmd="cd "+ string(FXFile::getUserDirectory ("").text ()) +"/.openspace/plugins/filelist/ && wget -nc "+ download;
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
	cmd="cd "+ string(FXFile::getUserDirectory ("").text ()) +"/.openspace/plugins/cmddialog/ && wget -nc http://openspace.linux.pl/files/0.1.0/x86/commandPluginsList.txt";
	else
	cmd="cd "+ string(FXFile::getUserDirectory ("").text ()) +"/.openspace/plugins/filelist/ && wget -nc http://openspace.linux.pl/files/0.1.0/x86/vfsPluginsList.txt";
	
	
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
			availableCommandPluginsList->appendItem(name.c_str());
			}
			else
			{
			availableVfsPluginsList->appendItem(name.c_str());
			}
		}
	}

}



preferences::preferences (FXWindow * owner):FXDialogBox (owner, "Preferences", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4)
{

    mimeapp=NULL;
    saveconfiguration=true;
    
    objmanager=objectmanager::instance(getApp());
    colordlg=new FXColorDialog(this,"Color Dialog");
    
    FXVerticalFrame *vertical = new FXVerticalFrame (this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);
    FXHorizontalFrame *horizontal = new FXHorizontalFrame (vertical, LAYOUT_FILL_X | LAYOUT_FILL_Y);
    FXVerticalFrame *buttons = new FXVerticalFrame (horizontal, LAYOUT_LEFT | LAYOUT_FILL_Y | FRAME_SUNKEN | PACK_UNIFORM_WIDTH | LAYOUT_FIX_HEIGHT, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0);
    FXSwitcher *switcher = new FXSwitcher (horizontal, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);


    //new FXButton (vertical, "Save", NULL, this, preferences::ID_SAVE);
    new FXButton (vertical, "Semi-Auto configure", NULL, this, preferences::ID_MIME_APP);
    new FXButton (vertical, "Full-Auto configure", NULL, this, preferences::ID_MIME_APP_AUTO);

    FXVerticalFrame *mainpane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    new FXLabel (mainpane, "Main settings", NULL, LAYOUT_LEFT);
    new FXButton (buttons, "Main Settings", NULL, switcher, FXSwitcher::ID_OPEN_FIRST, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);

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

 FXVerticalFrame *buttonsPane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXLabel (buttonsPane, "Menu and Buttons settings", NULL, LAYOUT_LEFT);
    new FXButton (buttons, "Menu and Buttons Settings", NULL, switcher, FXSwitcher::ID_OPEN_SECOND, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);

	buttonsList=new FXList (buttonsPane,NULL, 0,LIST_NORMAL| LAYOUT_FIX_WIDTH, 0, 0,250);
	buttonsList->setNumVisible(5);

	    if(conf->openxpath("/OpenspaceConfig/button_commands/command")!=-1)
	       {
	       string commandstr;
	      	 while(conf->getnextstring(commandstr))
	      	 {     	 
	     	 buttonsList->appendItem(commandstr.c_str());
	     	 }
	       }	 
	FXHorizontalFrame *buttonsHframe = new FXHorizontalFrame (buttonsPane, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);  

	new FXLabel(buttonsHframe," ");
	new FXLabel(buttonsHframe," ");
	new FXArrowButton(buttonsHframe,this,ID_ADD_BUTTON_COMMAND,FRAME_RAISED|FRAME_THICK|ARROW_UP);
	new FXArrowButton(buttonsHframe,this,ID_DEL_BUTTON_COMMAND,FRAME_RAISED|FRAME_THICK|ARROW_DOWN);
	additionalCommandsAvailableForButtons=new FXList (buttonsPane,NULL, 0,LIST_NORMAL| LAYOUT_FIX_WIDTH, 0, 0,250);
	additionalCommandsAvailableForButtons->setNumVisible(8);
FXHorizontalFrame *buttonsHframe2 = new FXHorizontalFrame (buttonsPane, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);  

	new FXLabel(buttonsHframe2," ");
	new FXLabel(buttonsHframe2," ");
new FXArrowButton(buttonsHframe2,this,ID_DEL_SHUTTER_COMMAND,FRAME_RAISED|FRAME_THICK|ARROW_UP);
new FXArrowButton(buttonsHframe2,this,ID_ADD_SHUTTER_COMMAND,FRAME_RAISED|FRAME_THICK|ARROW_DOWN);

       shutterList=new FXListBox (buttonsPane, this, ID_SHUTTER_CHANGE);
       shutterList->setNumVisible(10);

shutterCommands=new FXList (buttonsPane,NULL, 0,LIST_NORMAL| LAYOUT_FIX_WIDTH, 0, 0,250);
shutterCommands->setNumVisible(5);

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





    FXVerticalFrame *commandPluginsPane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXLabel (commandPluginsPane, "Command plugins settings", NULL, LAYOUT_LEFT);
    new FXButton (buttons, "Command plugins Settings", NULL, switcher, FXSwitcher::ID_OPEN_THIRD, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);

 new FXLabel (commandPluginsPane, "installed plugins");
commandPluginsList=new FXListBox (commandPluginsPane, this, ID_COMMANDPLUGIN_CHANGE);
commandPluginsList->setNumVisible(30);

 new FXLabel (commandPluginsPane, "available plugins - download from internet");
 
availableCommandPluginsList=new FXListBox (commandPluginsPane);
availableCommandPluginsList->setNumVisible(30);

new FXButton (commandPluginsPane, "Download and Install", NULL, this, ID_DOWNLOAD_INSTALL_CMD_PLUGIN, FRAME_RAISED | ICON_ABOVE_TEXT );
new FXButton (commandPluginsPane, "Update available plugins list", NULL, this, ID_UPDATE_CMD_PLUGIN_LIST, FRAME_RAISED | ICON_ABOVE_TEXT );



string plugin_path = conf->readonestring ("/OpenspaceConfig/path") + "plugins/cmddialog";




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
    new FXLabel (commandsPane, "text: ", NULL, LAYOUT_LEFT);
    commandsTextfieldText = new FXTextField (commandsPane, 20);
    commandsType=new FXLabel (commandsPane, "", NULL, LAYOUT_LEFT);
    commandsIcon=new FXLabel (commandsPane, "", NULL, LAYOUT_LEFT);
    
    FXPacker *vv = new FXGroupBox (commandsPane, "Options", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0);
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
	    commandsMap[ct.name]=ct;

	}

	    commandsRescan->setCheck(ctlast->rescan);
	    commandsCapture->setCheck(ctlast->capture);
	    commandsTextfield->setText (ctlast->exec.c_str ());
            commandsCombo->setCurrentItem (commandsCombo->getNumItems () - 1);
	    currentCommandName=ctlast->name;
	    string str="command type: " + ctlast->type;
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
	

	new FXSeparator(commandsPane);
	FXHorizontalFrame *hfr=new FXHorizontalFrame (commandsPane, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXButton (hfr, "Remove", NULL, this, ID_REMOVE_COMMAND, FRAME_RAISED | ICON_ABOVE_TEXT );
	new FXButton (hfr, "New Command", NULL, this, ID_NEW_COMMAND, FRAME_RAISED | ICON_ABOVE_TEXT);
	newCommandEdit = new FXTextField (hfr, 20);

	

    }


    filetypePane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXLabel (filetypePane, "File types settings", NULL, LAYOUT_LEFT);
    new FXButton (buttons, "File types Settings", NULL, switcher, FXSwitcher::ID_OPEN_FIFTH, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);



    if (conf->openxpath ("/OpenspaceConfig/file_types") != -1)
    {
	filetype_container *ctlast;
	new FXLabel(filetypePane,"File Type:");
	fileTypeList=new FXListBox (filetypePane, this, ID_FILETYPE_CHANGE);
	fileTypeList->setNumVisible(30);
	
	new FXLabel(filetypePane,"default command:");
	fileTypeDefaultBox = new FXListBox (filetypePane);
	fileTypeDefaultBox->setNumVisible(30);
	
	iconsList2=new FXListBox (filetypePane);
        iconsList2->setNumVisible(30);
	
	FXHorizontalFrame* hzframe=new FXHorizontalFrame (filetypePane, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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

	new FXLabel(filetypePane,"color:");
	colorbutton=new FXButton(filetypePane,"Color",NULL,this,ID_CHOOSE_COLOR);
	new FXLabel(filetypePane,"back color:");
	backcolorbutton=new FXButton(filetypePane,"Back Color",NULL,this,ID_CHOOSE_BACKCOLOR);
	new FXSeparator(filetypePane);
	allMime=new FXComboBox (filetypePane,60);
	allMime->setNumVisible(30);
	new FXButton (filetypePane, "Add", NULL, this, preferences::ID_ADD_FILETYPE);
	
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
		additionalCommandsAvailable->appendItem (command.c_str ());
		additionalCommandsAvailableForButtons->appendItem (command.c_str ());
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


    vfsPane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXLabel (vfsPane, "VFS settings", NULL, LAYOUT_LEFT);

    new FXButton (buttons, "Virtual File System Settings", NULL, switcher, FXSwitcher::ID_OPEN_SIXTH, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);

    
    
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



 new FXLabel (vfsPane, "available plugins - download from internet");
 
availableVfsPluginsList=new FXListBox (vfsPane);
availableVfsPluginsList->setNumVisible(30);
    
new FXButton (vfsPane, "Download and Install", NULL, this, ID_DOWNLOAD_INSTALL_VFS_PLUGIN, FRAME_RAISED | ICON_ABOVE_TEXT );
new FXButton (vfsPane, "Update available plugins list", NULL, this, ID_UPDATE_VFS_PLUGIN_LIST, FRAME_RAISED | ICON_ABOVE_TEXT );

    
    
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

	if(id==ID_ADD_BUTTON_COMMAND)
	{
	buttonsList->appendItem(additionalCommandsAvailableForButtons->getItemText(additionalCommandsAvailableForButtons->getCurrentItem()));
	}
	else
	{
	buttonsList->removeItem(buttonsList->getCurrentItem());
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
	shutterVector.push_back(shutter_container(actual_shutter,cmd));
	shutterCommands->appendItem(cmd.c_str());
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



conf->saveonestring ("/OpenspaceConfig/leftdir/dir",leftdir->getText().text());
conf->saveonestring ("/OpenspaceConfig/rightdir/dir",rightdir->getText().text());
conf->saveonestring ("/OpenspaceConfig/defaultdir/dir",defaultdir->getText().text());
conf->saveonestring ("/OpenspaceConfig/filelist/local/thumbs/size",thumbsize->getText().text());
conf->saveonestring ("/OpenspaceConfig/icons_theme",iconsTheme->getItem(iconsTheme->getCurrentItem()).text());



}

long preferences::onCommandChange (FXObject * sender, FXSelector sel, void *)
{

command_container *ct_prev=&commandsMap[currentCommandName];
ct_prev->exec=commandsTextfield->getText().text();
ct_prev->capture=commandsCapture->getCheck();
ct_prev->rescan=commandsRescan->getCheck();
ct_prev->text=commandsTextfieldText->getText().text();
ct_prev->icon=iconsList->getItem(iconsList->getCurrentItem()).text();

command_container ct=commandsMap[commandsCombo->getItem (commandsCombo->getCurrentItem ()).text ()];

commandsRescan->setCheck(ct.rescan);
commandsCapture->setCheck(ct.capture);
commandsTextfield->setText (ct.exec.c_str ());
commandsTextfieldText->setText (ct.text.c_str ());

iconsList->setCurrentItem(iconsList->findItem(ct.icon.c_str()));

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

ct_prev->color=ntos(colorbutton->getBackColor());
ct_prev->backcolor=ntos(backcolorbutton->getBackColor());
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

long preferences::onNewCommand (FXObject * sender, FXSelector sel, void *)
{

    string command_name = newCommandEdit->getText ().text ();
    if(command_name=="")
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
