#include "fx.h"
#include "preferences.h"
#include "MimeType.h"


FXDEFMAP (preferences) preferencesMap[] =
{
    //________Message_Type_____________________ID____________Message_Handler_______  
	FXMAPFUNC (SEL_COMMAND, preferences::ID_SAVE, preferences::onSave),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_COMMAND_CHANGE, preferences::onCommandChange),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_FILETYPE_CHANGE, preferences::onFileTypeChange), 
	FXMAPFUNC (SEL_COMMAND, preferences::ID_NEW_COMMAND, preferences::onNewCommand), 
	FXMAPFUNC (SEL_COMMAND, preferences::ID_REMOVE_COMMAND, preferences::onRemoveCommand),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_MIME_APP, preferences::onOpenMimeApp),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_ADD_FILETYPE, preferences::onAddFiletype),
	FXMAPFUNCS (SEL_COMMAND, preferences::ID_ADD_COMMAND_ADDITIONAL,preferences::ID_DEL_COMMAND_ADDITIONAL, preferences::onAdditionalCommandChange),
};


FXIMPLEMENT (preferences, FXDialogBox, preferencesMap, ARRAYNUMBER (preferencesMap)) preferences::preferences (FXWindow * owner):FXDialogBox (owner, "Preferences", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4)
{

    mimeapp=new MimeApp(this);
    objmanager=objectmanager::instance(getApp());
    FXVerticalFrame *vertical = new FXVerticalFrame (this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);
    FXHorizontalFrame *horizontal = new FXHorizontalFrame (vertical, LAYOUT_FILL_X | LAYOUT_FILL_Y);
    FXVerticalFrame *buttons = new FXVerticalFrame (horizontal, LAYOUT_LEFT | LAYOUT_FILL_Y | FRAME_SUNKEN | PACK_UNIFORM_WIDTH | PACK_UNIFORM_HEIGHT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    FXSwitcher *switcher = new FXSwitcher (horizontal, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);


    new FXButton (vertical, "Save", NULL, this, preferences::ID_SAVE);
    new FXButton (vertical, "Auto configure", NULL, this, preferences::ID_MIME_APP);


    FXVerticalFrame *mainpane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXLabel (mainpane, "Main settings", NULL, LAYOUT_LEFT);
    new FXButton (buttons, "Main Settings", NULL, switcher, FXSwitcher::ID_OPEN_FIRST, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);

    new FXLabel (mainpane, "width:");
    mainwindow_width = new FXTextField (mainpane, 5);
    mainwindow_width->setText (conf->readonestring ("/OpenspaceConfig/mainwindow/width").c_str ());
    new FXLabel (mainpane, "height:");
    mainwindow_height = new FXTextField (mainpane, 5);
    mainwindow_height->setText (conf->readonestring ("/OpenspaceConfig/mainwindow/height").c_str ());

//getShell()->getWidth()
//getShell()->getHeight()


    FXVerticalFrame *pluginspane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXLabel (pluginspane, "Plugins settings", NULL, LAYOUT_LEFT);
    new FXButton (buttons, "Plugins Settings", NULL, switcher, FXSwitcher::ID_OPEN_SECOND, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);

    commandsPane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXLabel (commandsPane, "Commands settings", NULL, LAYOUT_LEFT);

    new FXButton (buttons, "Commands Settings", NULL, switcher, FXSwitcher::ID_OPEN_THIRD, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);

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
	   
		
		fxmessage("aa=%s\n",res.c_str());
		
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
	iconsList->appendItem(iter->first.c_str(),icon);
    }	
	if(ctlast->icon!="")
	iconsList->setCurrentItem(iconsList->findItem(ctlast->icon.c_str()));
	

	new FXSeparator(commandsPane);
	FXHorizontalFrame *hfr=new FXHorizontalFrame (commandsPane, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXButton (hfr, "Remove", NULL, this, ID_REMOVE_COMMAND, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);
	new FXButton (hfr, "New Command", NULL, this, ID_NEW_COMMAND, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);
	newCommandEdit = new FXTextField (hfr, 20);

	

    }


    filetypePane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXLabel (filetypePane, "File types settings", NULL, LAYOUT_LEFT);
    new FXButton (buttons, "File types Settings", NULL, switcher, FXSwitcher::ID_OPEN_FOURTH, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);



    if (conf->openxpath ("/OpenspaceConfig/file_types") != -1)
    {
	filetype_container *ctlast;
	new FXLabel(filetypePane,"File Type:");
	fileTypeList=new FXListBox (filetypePane, this, ID_FILETYPE_CHANGE);
	fileTypeList->setNumVisible(30);
	
	new FXLabel(filetypePane,"default command:");
	fileTypeDefaultBox = new FXListBox (filetypePane);
	fileTypeDefaultBox->setNumVisible(30);
	
	FXHorizontalFrame* hzframe=new FXHorizontalFrame (filetypePane, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	FXVerticalFrame* vframe0=new FXVerticalFrame (hzframe, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(vframe0,"additional commands:");
        additionalCommands=new FXList (vframe0,NULL, 0,LIST_NORMAL| LAYOUT_FIX_WIDTH, 0, 0,250);
	additionalCommands->setNumVisible(5);
	FXVerticalFrame* vframe=new FXVerticalFrame (hzframe, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(vframe,"\n");
	new FXLabel(vframe,"\n");
	new FXArrowButton(vframe,this,ID_ADD_COMMAND_ADDITIONAL,FRAME_RAISED|FRAME_THICK|ARROW_LEFT);
	new FXArrowButton(vframe,this,ID_DEL_COMMAND_ADDITIONAL,FRAME_RAISED|FRAME_THICK|ARROW_RIGHT);
	FXVerticalFrame* vframe1=new FXVerticalFrame (hzframe, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	new FXLabel(vframe1,"all available commands:");
	additionalCommandsAvailable=new FXList (vframe1,NULL, 0,LIST_NORMAL| LAYOUT_FIX_WIDTH, 0, 0,250);
	additionalCommandsAvailable->setNumVisible(5);

	
	allMime=new FXComboBox (filetypePane,60);
	allMime->setNumVisible(30);
	MimeType::__initialize();
	map<string,string>::iterator iter0;
	for(iter0=MimeType::mimeMap.begin();iter0!=MimeType::mimeMap.end();iter0++)
	{
	allMime->appendItem(xml2mime(iter0->second).c_str());
	}
	
	new FXButton (filetypePane, "Add", NULL, this, preferences::ID_ADD_FILETYPE);
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
			 ct.name = res+"/"+res2;
			 fileTypeList->appendItem(xml2mime(ct.name).c_str (),objmanager->osicons["unknown"]);	    
	   		 
	   		 ct.command = conf->readonestring ("/OpenspaceConfig/file_types/" + res + "/types/"+res2+"/default");
	   		 ct.icon=conf->readonestring("/OpenspaceConfig/file_types/" + res + "/types/"+res2+"/icon") ;
	   		 
			 configure conflocal2 = *conf;
	   		 if(conflocal2.openxpath("/OpenspaceConfig/file_types/" + res+"/types/"+res2 + "/commands/command")!=-1)
	      		 {
			 string command;
	      			 while(conflocal2.getnextstring(command))
	      			 {
	      			  ct.commands.push_back(command.c_str());
				  fxmessage("\n\nCOMMAND=%s\n",command.c_str());

	     			 }
	    		 }  
			 filetypesMap[ct.name]=ct;  
			
		}
		
	   }	
	    filetype_container ct;
	    ctlast=&ct;
	    ct.name = res;
	    fileTypeList->appendItem(xml2mime(ct.name) .c_str (),objmanager->osicons["unknown"]);	    
	    
	    ct.command = conf->readonestring ("/OpenspaceConfig/file_types/" + res + "/default");
	    ct.icon=conf->readonestring("/OpenspaceConfig/file_types/" +res +"/icon") ;
	    
	    
	    configure conflocal2 = *conf;
	    if(conflocal2.openxpath("/OpenspaceConfig/file_types/" + ct.name + "/commands/command")!=-1)
	       {
	       string command;
	      	 while(conflocal2.getnextstring(command))
	      	 {
	      	 
	     	  ct.commands.push_back(command.c_str());
		  fxmessage("\n\nCOMMAND=%s\n",command.c_str());

	     	 }
	       }    
	filetypesMap[ct.name]=ct;
	}

	filetypestring = ctlast->name;

	
	
	fileTypeDefaultBox->appendItem ("",objmanager->osicons["execute"]);
	
	if (conf->openxpath ("/OpenspaceConfig/commands") != -1)
	{
	string command;
	    while (conf->getnextnode (command))
	    {
		
		fileTypeDefaultBox->appendItem (command.c_str (),objmanager->osicons["execute"]);
		additionalCommandsAvailable->appendItem (command.c_str ());

	    }
	}
	
	vector<string>::iterator iter;
	for(iter=ctlast->commands.begin();iter!=ctlast->commands.end();iter++)
	{
	additionalCommands->appendItem(iter->c_str());
	}
	                         
	       
	
	fileTypeList->setCurrentItem (fileTypeList->getNumItems () - 1);
	fileTypeDefaultBox->setCurrentItem(fileTypeDefaultBox->findItem(ctlast->command.c_str()));
	

	
    }

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

long preferences::onSave (FXObject * sender, FXSelector sel, void *)
{
    FXTRACE ((5, "save\n"));
    
    this->onCommandChange(NULL,0,NULL);
    this->onFileTypeChange(NULL,0,NULL);

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

/*
    vector < filetype_container * >::iterator iter2;

    for (iter2 = filetype_vec.begin (); iter2 != filetype_vec.end (); iter2++)
    {

	filetype_container *ct = *iter2;
	conf->saveonestring ("/OpenspaceConfig/file_types/" + ct->name + "/default", ct->command);
	fxmessage (ct->name.c_str ());
	fxmessage (" ");
	fxmessage (ct->command.c_str ());
	fxmessage ("\n");

    }


*/
    conf->saveonestring ("/OpenspaceConfig/mainwindow/width", mainwindow_width->getText ().text ());
    conf->saveonestring ("/OpenspaceConfig/mainwindow/height", mainwindow_height->getText ().text ());


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
//ct_prev->icon=

filetype_container ct=filetypesMap[mime2xml(fileTypeList->getItem (fileTypeList->getCurrentItem ()).text ())];
fileTypeDefaultBox->setCurrentItem(fileTypeDefaultBox->findItem(ct.command.c_str()));
additionalCommands->clearItems();


vector<string>::iterator iter;
	for(iter=ct.commands.begin();iter!=ct.commands.end();iter++)
	{
	additionalCommands->appendItem(iter->c_str());
	}
	

currentFileType=ct.name;

}
long preferences::onNewCommand (FXObject * sender, FXSelector sel, void *)
{

    string command_name = newCommandEdit->getText ().text ();
    if(command_name=="")
    return 0;
    
    fxmessage (command_name.c_str ());
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
 
    if (mimeapp->shown ())
	mimeapp->hide ();

    else
	mimeapp->show (PLACEMENT_OWNER);
}
