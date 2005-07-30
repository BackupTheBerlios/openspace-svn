#include "fx.h"
#include "preferences.h"


FXDEFMAP (preferences) preferencesMap[] =
{
    //________Message_Type_____________________ID____________Message_Handler_______  
	FXMAPFUNC (SEL_COMMAND, preferences::ID_SAVE, preferences::onSave),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_COMMAND_CHANGE, preferences::onCommandChange),
	FXMAPFUNC (SEL_COMMAND, preferences::ID_FILETYPE_CHANGE, preferences::onFileTypeChange), 
	FXMAPFUNC (SEL_COMMAND, preferences::ID_NEW_COMMAND, preferences::onNewCommand), 
	FXMAPFUNC (SEL_COMMAND, preferences::ID_REMOVE_COMMAND, preferences::onRemoveCommand),};


FXIMPLEMENT (preferences, FXDialogBox, preferencesMap, ARRAYNUMBER (preferencesMap)) preferences::preferences (FXWindow * owner):FXDialogBox (owner, "Preferences", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4)
{


    FXVerticalFrame *vertical = new FXVerticalFrame (this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);
    FXHorizontalFrame *horizontal = new FXHorizontalFrame (vertical, LAYOUT_FILL_X | LAYOUT_FILL_Y);
    FXVerticalFrame *buttons = new FXVerticalFrame (horizontal, LAYOUT_LEFT | LAYOUT_FILL_Y | FRAME_SUNKEN | PACK_UNIFORM_WIDTH | PACK_UNIFORM_HEIGHT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    FXSwitcher *switcher = new FXSwitcher (horizontal, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);


    new FXButton (vertical, "Save", NULL, this, preferences::ID_SAVE);


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





    commandsmainpane = new FXVerticalFrame (switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    new FXLabel (commandsmainpane, "Commands settings", NULL, LAYOUT_LEFT);
    commandspane = new FXVerticalFrame (commandsmainpane, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    staticcommandpane = new FXVerticalFrame (commandsmainpane, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);


    new FXButton (buttons, "Commands Settings", NULL, switcher, FXSwitcher::ID_OPEN_THIRD, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);



    if (conf->openxpath ("/OpenspaceConfig/commands") != -1)
    {
	command_container *ct;

	commandspop = new FXPopup (this);


	while (1)
	{
	    string res = conf->getnextnode ();
	    if (res == "")
		break;

	    ct = new command_container;
	    ct->frame = new FXVerticalFrame (commandspane, LAYOUT_FILL_X | FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);


	    new FXOption (commandspop, res.c_str (), NULL, this, ID_COMMAND_CHANGE, JUSTIFY_HZ_APART | ICON_AFTER_TEXT);

	    ct->name = res;
	    configure conflocal = *conf;
	    string exec = conflocal.readonestring ("/OpenspaceConfig/commands/" + res + "/exec");
	    ct->textfield = new FXTextField (ct->frame, 20);
	    ct->textfield->setText (exec.c_str ());



	    ct->vv = new FXGroupBox (ct->frame, "Options", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0);
	    ct->rescancheck = new FXCheckButton (ct->vv, "rescan", NULL, 0, JUSTIFY_LEFT | JUSTIFY_TOP | ICON_BEFORE_TEXT | LAYOUT_SIDE_TOP);
	    ct->capturecheck = new FXCheckButton (ct->vv, "capture", NULL, 0, JUSTIFY_LEFT | JUSTIFY_TOP | ICON_BEFORE_TEXT | LAYOUT_SIDE_TOP);

	    string options = conflocal.readonestring ("/OpenspaceConfig/commands/" + res + "/options");

	    string::size_type pos = options.find ("capture");
	    if (pos != string::npos)
		ct->capturecheck->setCheck (true);
	    else
		ct->capturecheck->setCheck (false);


	    pos = options.find ("rescan");
	    if (pos != string::npos)
		ct->rescancheck->setCheck (true);
	    else
		ct->rescancheck->setCheck (false);


	    ct->frame->hide ();
	    command_vec.push_back (ct);

	}

	ct->frame->show ();

	commandsmenu = new FXOptionMenu (staticcommandpane, commandspop, LAYOUT_TOP | FRAME_RAISED | FRAME_THICK | JUSTIFY_HZ_APART | ICON_AFTER_TEXT);

	commandsmenu->setCurrentNo (commandsmenu->getNumOptions () - 1);
	new FXButton (staticcommandpane, "New Command", NULL, this, ID_NEW_COMMAND, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);
	newcommandedit = new FXTextField (staticcommandpane, 20);

	new FXButton (staticcommandpane, "Remove", NULL, this, ID_REMOVE_COMMAND, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y);

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

	    ct = new filetype_container;


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




    vector < command_container * >::iterator iter;

    for (iter = command_vec.begin (); iter != command_vec.end (); iter++)
    {
	command_container *ct = *iter;
	string com = ct->name;
	string value = ct->textfield->getText ().text ();
	if (!conf->saveonestring ("/OpenspaceConfig/commands/" + com + "/exec", value))
	{
	    conf->addstring ("/OpenspaceConfig/commands", com, "");
	    conf->addstring ("/OpenspaceConfig/commands/" + com, "exec", value);
	}
	string options;
	if (ct->capturecheck->getCheck ())
	    options += "capture";
	if (ct->rescancheck->getCheck ())
	    options += " rescan";

	if (options != "")
	{
	    if (!conf->saveonestring ("/OpenspaceConfig/commands/" + com + "/options", options))
		conf->addstring ("/OpenspaceConfig/commands/" + com, "options", options);
	}

	/*fxmessage("\n");
	   fxmessage(value.c_str());
	   fxmessage("\n");
	   fxmessage(com.c_str());
	 */
    }


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



    conf->saveonestring ("/OpenspaceConfig/mainwindow/width", mainwindow_width->getText ().text ());
    conf->saveonestring ("/OpenspaceConfig/mainwindow/height", mainwindow_height->getText ().text ());
//conf->addstring("/OpenspaceConfig/mainwindow","test","zawartosc");

}

long preferences::onCommandChange (FXObject * sender, FXSelector sel, void *)
{

    vector < command_container * >::iterator iter;

    for (iter = command_vec.begin (); iter != command_vec.end (); iter++)
    {
	command_container *ct = *iter;
	ct->frame->hide ();
	if (ct->name == commandsmenu->getCurrent ()->getText ().text ())
	{
	    ct->frame->show ();
	}

    }
    commandspane->recalc ();
    commandsmainpane->recalc ();

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

}

long preferences::onRemoveCommand (FXObject * sender, FXSelector sel, void *)
{
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


}
