#include "fx.h"
#include "preferences.h"


FXDEFMAP (preferences) preferencesMap[] =
{
    //________Message_Type_____________________ID____________Message_Handler_______  
	FXMAPFUNC (SEL_COMMAND, preferences::ID_SAVE, preferences::onSave),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_COMMAND_CHANGE, preferences::onCommandChange),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_FILETYPE_CHANGE, preferences::onFileTypeChange), 
	FXMAPFUNC (SEL_COMMAND, preferences::ID_NEW_COMMAND, preferences::onNewCommand), 
	FXMAPFUNC (SEL_COMMAND, preferences::ID_REMOVE_COMMAND, preferences::onRemoveCommand),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_MIME_APP, preferences::onOpenMimeApp),
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

   //commandsPop = new FXScrollPane (this,30,PACK_UNIFORM_HEIGHT);
   // commandsPop = (FXScrollPane*)new FXMenuPane (this,20);
   // commandsMenu = new FXOptionMenu (commandsPane, dynamic_cast<FXMenuPane *>(commandsPop), LAYOUT_TOP | FRAME_RAISED | FRAME_THICK | JUSTIFY_HZ_APART | ICON_AFTER_TEXT);
   
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
	command_container ct;

	


	while (1)
	{
	    string res = conf->getnextnode ();
	    if (res == "")
		break;

	    //new FXOption (commandsPop, res.c_str (), NULL, this, ID_COMMAND_CHANGE, JUSTIFY_HZ_APART | ICON_AFTER_TEXT);
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

	    commandsRescan->setCheck(ct.rescan);
	    commandsCapture->setCheck(ct.capture);
	    commandsTextfield->setText (ct.exec.c_str ());
            commandsCombo->setCurrentItem (commandsCombo->getNumItems () - 1);
	    currentCommandName=ct.name;
	    string str="command type: " + ct.type;
	    commandsType->setText(str.c_str());
	    string shorticonname;
	    
	    
	    	   
	   // commandsIcon->setIcon(objmanager->osicons[]);
	
	
	   map < string, FXIcon * >::iterator iter;
	   
    iconsList->appendItem("",NULL);
    for (iter = objmanager->osicons.begin (); iter != objmanager->osicons.end (); iter++)
    {
	FXIcon *icon = iter->second;
	iconsList->appendItem(iter->first.c_str(),icon);
    }	
	if(ct.icon!="")
	iconsList->setCurrentItem(iconsList->findItem(ct.icon.c_str()));
	
	new FXLabel (commandsPane, "");
	new FXLabel (commandsPane, "");
	new FXSeparator(commandsPane);
	new FXButton (commandsPane, "New Command", NULL, this, ID_NEW_COMMAND, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);
	newCommandEdit = new FXTextField (commandsPane, 20);

	new FXButton (commandsPane, "Remove", NULL, this, ID_REMOVE_COMMAND, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);

    }

    filetypepane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXLabel (filetypepane, "File types settings", NULL, LAYOUT_LEFT);
    new FXButton (buttons, "File types Settings", NULL, switcher, FXSwitcher::ID_OPEN_FOURTH, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);



    if (conf->openxpath ("/OpenspaceConfig/file_types") != -1)
    {
	filetype_container *ct;

	filetypepop = new FXPopup (this);

	FXVerticalFrame *frame = new FXVerticalFrame (filetypepane, LAYOUT_FILL_X | FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);

	while (1)
	{
	    string res = conf->getnextnode ();
	    if (res == "")
		break;

	    ct = new   filetype_container;


	    new FXOption (filetypepop, res.c_str (), NULL, this, ID_FILETYPE_CHANGE, JUSTIFY_HZ_APART | ICON_AFTER_TEXT);

	    ct->name = res;
	    configure conflocal = *conf;
	    string exec = conflocal.readonestring ("/OpenspaceConfig/file_types/" + res + "/default");

	    ct->command = exec;

	    /*       FXIconSource *icos=new FXIconSource(getApp()); 

	       string path=conf->readonestring("/OpenspaceConfig/path") + "ico/" + conf->readonestring("/OpenspaceConfig/file_types/" +res +"/icon") ;
	       FXString fil=path.c_str();

	       ct->icon=icos->loadIcon(fil);
	       if(ct->icon)
	       {

	       ct->icon->create();
	       new FXButton (ct->frame,"",ct->icon,this,777,FRAME_THICK,0,0,0,0,0,0,0,0);
	       }

	       FXComboBox *cbox=new FXComboBox (ct->frame, 20);
	       configure conflocal2=*conf;                          
	       if(conflocal2.openxpath("/OpenspaceConfig/file_types/" + res + "/commands/command")!=-1)
	       {
	       while(1)
	       {
	       string command=conflocal2.getnextstring();
	       if(command=="")
	       break;
	       cbox->appendItem(command.c_str());

	       }
	       }    


	     */

	    filetype_vec.push_back (ct);

	}

	filetypestring = ct->name;

	filetypedefaultbox = new FXComboBox (frame, 20);
	configure conflocal2 = *conf;
	if (conflocal2.openxpath ("/OpenspaceConfig/commands") != -1)
	{
	    while (1)
	    {
		string command = conflocal2.getnextnode ();
		if (command == "")
		    break;
		filetypedefaultbox->appendItem (command.c_str ());

	    }
	}

	filetypemenu = new FXOptionMenu (filetypepane, filetypepop, LAYOUT_TOP | FRAME_RAISED | FRAME_THICK | JUSTIFY_HZ_APART | ICON_AFTER_TEXT);
	//filetypemenu->setCurrentNo(filetypemenu->getNumOptions()-1);

    }

}


preferences::~preferences ()
{
}

long preferences::onSave (FXObject * sender, FXSelector sel, void *)
{
    FXTRACE ((5, "save\n"));
    
    this->onCommandChange(NULL,0,NULL);

    map < string, command_container >::iterator iter;

    for (iter = commandsMap.begin (); iter != commandsMap.end (); iter++)
    {
	command_container ct = iter->second;
	
	string com = ct.name;
	string value = ct.exec;
	if (!conf->saveonestring ("/OpenspaceConfig/commands/" + com + "/exec", value))
	{
	    conf->addstring ("/OpenspaceConfig/commands", com, "");
	    if(ct.exec!="")
	    conf->addstring ("/OpenspaceConfig/commands/" + com, "exec", value);
	}
	string options;
	if (ct.capture)
	    options += " capture";
	if (ct.rescan)
	    options += " rescan";

	if (options != "")
	{
	    if (!conf->saveonestring ("/OpenspaceConfig/commands/" + com + "/options", options))
		conf->addstring ("/OpenspaceConfig/commands/" + com, "options", options);
	}

	if (ct.text != "")
	{
	    if (!conf->saveonestring ("/OpenspaceConfig/commands/" + com + "/text", ct.text))
		conf->addstring ("/OpenspaceConfig/commands/" + com, "text", ct.text);
	}


	if (ct.icon != "")
	{
	    if (!conf->saveonestring ("/OpenspaceConfig/commands/" + com + "/icon", ct.icon))
		conf->addstring ("/OpenspaceConfig/commands/" + com, "icon", ct.icon);
	}

	   fxmessage("\n");
	   fxmessage(value.c_str());
	   fxmessage("\n");
	   fxmessage(com.c_str());
	 
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



    vector < filetype_container * >::iterator iter;


    for (iter = filetype_vec.begin (); iter != filetype_vec.end (); iter++)
    {
	filetype_container *ct = *iter;
	if (ct->name == filetypestring)
	{
	    ct->command = filetypedefaultbox->getText ().text ();
	    fxmessage (ct->name.c_str ());
	    break;
	}

    }

    filetypestring = filetypemenu->getCurrent ()->getText ().text ();
    fxmessage (filetypestring.c_str ());
    for (iter = filetype_vec.begin (); iter != filetype_vec.end (); iter++)
    {
	filetype_container *ct = *iter;
	if (ct->name == filetypestring)
	{
	    int pos = filetypedefaultbox->findItem (ct->command.c_str ());
	    filetypedefaultbox->setCurrentItem (pos);
	    break;
	}

    }



//string def=conf->readonestring("/OpenspaceConfig/file_types/" + filetypestring + "/default" );




/*
vector<filetype_container*>::iterator iter;

	for (iter=filetype_vec.begin();iter != filetype_vec.end(); iter++)
	{
		filetype_container*ct=*iter;
		ct->frame->hide();
		if(ct->name==filetypemenu->getCurrent()->getText().text())
		{
		ct->frame->show();
		}
	
	}

filetypepane->recalc();
*/

}
long preferences::onNewCommand (FXObject * sender, FXSelector sel, void *)
{
/*
    fxmessage ("command");

    string command_name = newcommandedit->getText ().text ();
    fxmessage (command_name.c_str ());
    command_container *ct = new command_container;
    ct->frame = new FXVerticalFrame (commandspane, LAYOUT_FILL_X | FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
    FXOption *op = new FXOption (commandspop, command_name.c_str (), NULL, this, ID_COMMAND_CHANGE, JUSTIFY_HZ_APART | ICON_AFTER_TEXT);
    op->create ();
    ct->name = command_name;
    ct->textfield = new FXTextField (ct->frame, 20);
    ct->vv = new FXGroupBox (ct->frame, "Options", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0);
    ct->rescancheck = new FXCheckButton (ct->vv, "rescan", NULL, 0, JUSTIFY_LEFT | JUSTIFY_TOP | ICON_BEFORE_TEXT | LAYOUT_SIDE_TOP);
    ct->capturecheck = new FXCheckButton (ct->vv, "capture", NULL, 0, JUSTIFY_LEFT | JUSTIFY_TOP | ICON_BEFORE_TEXT | LAYOUT_SIDE_TOP);
    ct->frame->create ();

    ct->frame->hide ();
    command_vec.push_back (ct);
*/
}

long preferences::onRemoveCommand (FXObject * sender, FXSelector sel, void *)
{
/*
    string command = "/OpenspaceConfig/commands/";
    command += commandsmenu->getCurrent ()->getText ().text ();
//command+="/exec";
    fxmessage (command.c_str ());


    conf->removestring (command);

    vector < command_container * >::iterator iter;

    for (iter = command_vec.begin (); iter != command_vec.end (); iter++)
    {
	command_container *ct = *iter;
	ct->frame->hide ();
	if (ct->name == commandsmenu->getCurrent ()->getText ().text ())
	{
	    delete ct->frame;
	    delete ct;
	    command_vec.erase (iter);
	    delete commandsmenu->getCurrent ();
	    commandsmenu->setCurrentNo (0);
	    break;
	}

    }

    command_container *ct = *command_vec.begin ();
    ct->frame->show ();
*/

}

long preferences::onOpenMimeApp (FXObject * sender, FXSelector sel, void *)
{
 
    if (mimeapp->shown ())
	mimeapp->hide ();

    else
	mimeapp->show (PLACEMENT_OWNER);
}
