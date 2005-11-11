#include "config.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

#include "fx.h" 
// #include "FXPNGIcon.h"
#include "OSThreadExec.h"
#include "OSFileList.h"
#include "OSObjectManager.h"
#include "mainwindow.h"
#include "informationpanel.h"
#include "OSConfigure.h"
#include "OSFileListItem.h"

#include "FirstRun.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#ifdef WIN32
#define SEPARATOR "\\"
#else /*  */
#include <dlfcn.h>
#define SEPARATOR "/"
#endif /*  */
using namespace FX;
FXDEFMAP ( MainWindow ) MainWindowMap[] =
    {

        //________Message_Type_____________________ID____________Message_Handler_______
        FXMAPFUNC ( SEL_COMMAND, MainWindow::ID_DIRCH, MainWindow::onChangeDir ),
        // FXMAPFUNC(SEL_ENTER, MainWindow::ID_DIRCH,   MainWindow::onListNextDir),
        // FXMAPFUNC(SEL_LEAVE, MainWindow::ID_DIRCH,   MainWindow::onListNextDir),
        FXMAPFUNC ( SEL_RIGHTBUTTONRELEASE, MainWindow::ID_DIR, MainWindow::onListDirs ),
        FXMAPFUNC ( SEL_COMMAND, MainWindow::ID_DIR, MainWindow::onOpenDir ),
        FXMAPFUNC ( SEL_COMMAND, MainWindow::ID_CONFIGURE, MainWindow::onOpenConfigure ),
        FXMAPFUNC ( SEL_COMMAND, MainWindow::ID_ABOUT, MainWindow::onAbout ),
        FXMAPFUNCS ( SEL_LEFTBUTTONRELEASE, MainWindow::ID_TOLEFT, MainWindow::ID_TORIGHT, MainWindow::onChangeList ),
        FXMAPFUNCS ( SEL_COMMAND, MainWindow::ID_OVERWRITE, MainWindow::ID_SKIP_ALL, MainWindow::onOverwrite ),
        FXMAPFUNC ( SEL_COMMAND, MainWindow::ID_NEWFRAME, MainWindow::onNewFrame ),
        FXMAPFUNC ( SEL_COMMAND, MainWindow::ID_NEW_NETWORK, MainWindow::onNewNetworkFrame ),
        FXMAPFUNC ( SEL_COMMAND, MainWindow::ID_NEW_SEARCH, MainWindow::onNewSearchFrame ),
        FXMAPFUNCS ( SEL_COMMAND, 666, 668, MainWindow::onNotify ),
        FXMAPFUNC ( SEL_TIMEOUT, MainWindow::ID_TIMER, MainWindow::onTimer ),
        FXMAPFUNC ( SEL_COMMAND, MainWindow::ID_COMMANDS_SHOW, MainWindow::commandsShow ),
        FXMAPFUNC ( SEL_CONFIGURE, 0, MainWindow::onConfigure ),
        FXMAPFUNC ( SEL_COMMAND, MainWindow::ID_CANCEL, MainWindow::cancel ),
        FXMAPFUNC ( SEL_UPDATE, 0, MainWindow::onUpdate ) };

FXIMPLEMENT ( MainWindow, FXMainWindow, MainWindowMap, ARRAYNUMBER ( MainWindowMap ) )


//-----MAIN WINDOW---------------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow ( FXApp * a ) : FXMainWindow ( a, "openspace", NULL, NULL, DECOR_ALL | LAYOUT_FIX_WIDTH, 0, 0, 600, 400, 0, 0 )
{
    conf = new configure ();

    if ( conf->loadconfig() )
    {
        if ( conf->readonestring ( "/OpenspaceConfig/version" ) == PACKAGE_VERSION )
        {


            objmanager = objectmanager::instance( getApp() );

            new FXToolTip ( getApp (), TOOLTIP_NORMAL );
            pane = NULL;
            filemenu = NULL;
            pref = NULL;

            string fontdesc = conf->readonestring ( "/OpenspaceConfig/fonts/normalfont" );
            FXFont *font = new FXFont( getApp(), fontdesc.c_str() );
            font->create();
            getApp() ->setNormalFont( font );


            getApp() ->setTypingSpeed( FXIntVal( conf->readonestring ( "/OpenspaceConfig/speed_delay/typing_speed" ).c_str () ) );
            getApp() ->setClickSpeed( FXIntVal( conf->readonestring ( "/OpenspaceConfig/speed_delay/click_speed" ).c_str () ) );
            getApp() ->setScrollSpeed( FXIntVal( conf->readonestring ( "/OpenspaceConfig/speed_delay/scroll_speed" ).c_str () ) );
            getApp() ->setScrollDelay( FXIntVal( conf->readonestring ( "/OpenspaceConfig/speed_delay/scroll_delay" ).c_str () ) );
            getApp() ->setBlinkSpeed( FXIntVal( conf->readonestring ( "/OpenspaceConfig/speed_delay/blink_speed" ).c_str () ) );
            getApp() ->setAnimSpeed( FXIntVal( conf->readonestring ( "/OpenspaceConfig/speed_delay/anim_speed" ).c_str () ) );
            getApp() ->setMenuPause( FXIntVal( conf->readonestring ( "/OpenspaceConfig/speed_delay/menu_pause" ).c_str () ) );
            getApp() ->setTooltipPause( FXIntVal( conf->readonestring ( "/OpenspaceConfig/speed_delay/tooltip_pause" ).c_str () ) );
            getApp() ->setTooltipTime( FXIntVal( conf->readonestring ( "/OpenspaceConfig/speed_delay/tooltip_time" ).c_str () ) );
            getApp() ->setDragDelta( FXIntVal( conf->readonestring ( "/OpenspaceConfig/speed_delay/drag_delta" ).c_str () ) );
            getApp() ->setWheelLines( FXIntVal( conf->readonestring ( "/OpenspaceConfig/speed_delay/wheel_lines" ).c_str () ) );

            getApp() ->setBaseColor( fxcolorfromname( conf->readonestring ( "/OpenspaceConfig/colors/base" ).c_str() ) );
            getApp() ->setBorderColor( fxcolorfromname( conf->readonestring ( "/OpenspaceConfig/colors/border" ).c_str() ) );
            getApp() ->setForeColor( fxcolorfromname( conf->readonestring ( "/OpenspaceConfig/colors/fore" ).c_str() ) );
            getApp() ->setHiliteColor( fxcolorfromname( conf->readonestring ( "/OpenspaceConfig/colors/hilite" ).c_str() ) );
            getApp() ->setShadowColor( fxcolorfromname( conf->readonestring ( "/OpenspaceConfig/colors/shadow" ).c_str() ) );
            getApp() ->setSelforeColor( fxcolorfromname( conf->readonestring ( "/OpenspaceConfig/colors/selfore" ).c_str() ) );
            getApp() ->setSelbackColor( fxcolorfromname( conf->readonestring ( "/OpenspaceConfig/colors/selback" ).c_str() ) );
            getApp() ->setTipforeColor( fxcolorfromname( conf->readonestring ( "/OpenspaceConfig/colors/tipfore" ).c_str() ) );
            getApp() ->setTipbackColor( fxcolorfromname( conf->readonestring ( "/OpenspaceConfig/colors/tipback" ).c_str() ) );
            getApp() ->setSelMenuTextColor( fxcolorfromname( conf->readonestring ( "/OpenspaceConfig/colors/selmenutext" ).c_str() ) );
            getApp() ->setSelMenuBackColor( fxcolorfromname( conf->readonestring ( "/OpenspaceConfig/colors/selmenuback" ).c_str() ) );

            string iconsdir = conf->readonestring ( "/OpenspaceConfig/path" ) + "icons/" + conf->readonestring ( "/OpenspaceConfig/icons_theme" );

            if ( !FXFile::exists( iconsdir.c_str() ) )
                iconsdir = FXFile::getUserDirectory ( "" ).text () + string( "/.openspace/icons/" ) + conf->readonestring ( "/OpenspaceConfig/icons_theme" );

            iconsdir += string( "/" );

            loadicons( iconsdir );


            string res = conf->readonestring ( "/OpenspaceConfig/version" );
            FXTRACE ( ( 1, "\n openspace version %s\n\n", res.c_str () ) );
            int w = atoi ( conf->readonestring ( "/OpenspaceConfig/mainwindow/width" ).c_str () );
            int h = atoi ( conf->readonestring ( "/OpenspaceConfig/mainwindow/height" ).c_str () );
            resize ( w, h );

            // read icons for file types
            if ( conf->openxpath ( "/OpenspaceConfig/file_types" ) != -1 )
            {
                string res;
                while ( conf->getnextnode ( res ) )
                {
                    loadMimeSettings ( "/OpenspaceConfig/file_types/" + res, res );
                    configure conflocal = *conf;
                    if ( conflocal.openxpath ( "/OpenspaceConfig/file_types/" + res + "/types" ) != -1 )
                    {
                        string res2;
                        while ( conflocal.getnextnode ( res2 ) )
                        {
                            loadMimeSettings ( "/OpenspaceConfig/file_types/" + res + "/types/" + res2, res + "/" + res2 );
                        }
                    }
                }
            }




            topdock = new FXDockSite ( this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X );
            bottomdock = new FXDockSite ( this, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X );
            leftdock = new FXDockSite ( this, LAYOUT_SIDE_LEFT | LAYOUT_FILL_Y );
            rightdock = new FXDockSite ( this, LAYOUT_SIDE_RIGHT | LAYOUT_FILL_Y );



            FXToolBarShell *dragshell1 = new FXToolBarShell ( this, FRAME_RAISED );
            FXToolBar *toolbar = new FXToolBar ( topdock, dragshell1, LAYOUT_DOCK_NEXT | LAYOUT_SIDE_TOP | FRAME_RAISED );
            new FXToolBarGrip ( toolbar, toolbar, FXToolBar::ID_TOOLBARGRIP, TOOLBARGRIP_SINGLE );




            FXVerticalFrame *ff = new FXVerticalFrame ( this, LAYOUT_FILL_X | LAYOUT_FILL_Y );
            controlframe = new FXVerticalFrame ( ff, LAYOUT_FILL_X | FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );
            splitter = new FXSplitter ( ff, LAYOUT_FILL_X | SPLITTER_TRACKING | LAYOUT_FILL_Y );
            left = new FXVerticalFrame ( splitter, LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_NONE, 0, 0, w / 2, 0, 0, 0, 0 );
            right = new FXVerticalFrame ( splitter, LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_NONE, 0, 0, w / 2, 0, 0, 0, 0 );
            if ( conf->readonestring ( "/OpenspaceConfig/panels" ) == "single" )
                ratio = 0;

            else
                ratio = 1.0 / 2.0;
            leftcontrolframe = new FXVerticalFrame ( left, LAYOUT_FILL_X );
            leftframe = new FXVerticalFrame ( left, LAYOUT_FILL_X | LAYOUT_FILL_Y );
            rightcontrolframe = new FXVerticalFrame ( right, LAYOUT_FILL_X );
            rightframe = new FXVerticalFrame ( right, LAYOUT_FILL_X | LAYOUT_FILL_Y );
            //   FXHorizontalFrame * buttonsframe = new FXHorizontalFrame (toolbar, LAYOUT_FILL_X | FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
            new FXButton ( toolbar, "\thide/show copy/move progress", objmanager->osicons[ "plus" ], this, MainWindow::ID_COMMANDS_SHOW, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );
            new FXButton ( toolbar, "\topen new panel", objmanager->osicons[ "directory" ], this, MainWindow::ID_NEWFRAME, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );
            new FXButton ( toolbar, "\tconnect", objmanager->osicons[ "network" ], this, MainWindow::ID_NEW_NETWORK, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );
            new FXButton ( toolbar, "\tsearch", objmanager->osicons[ "search" ], this, MainWindow::ID_NEW_SEARCH, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );

            new FXSeparator ( toolbar, SEPARATOR_NONE );
            new FXSeparator ( toolbar, SEPARATOR_GROOVE );
            new FXSeparator ( toolbar, SEPARATOR_NONE );

            new FXButton ( toolbar, "\tconfiguration", objmanager->osicons[ "configure" ], this, MainWindow::ID_CONFIGURE, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );
            new FXButton ( toolbar, "\tabout", objmanager->osicons[ "foxmini" ], this, MainWindow::ID_ABOUT, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );
            toolbar->dock( rightdock );

            string dir = parseDir ( conf->readonestring ( "/OpenspaceConfig/leftdir/dir" ) );
            string type = conf->readonestring ( "/OpenspaceConfig/leftdir/type" );
            pathtype pt ( dir, type );
            left_frame = new Frame ( leftcontrolframe, leftframe, pt, this, -1 );
            left_frame->f->toolbar->dock( topdock );
            left_frame->f->toolbar->hide();
            left_frame->f->toolbar2->dock( rightdock );
            left_frame->f->toolbar2->hide();

            dir = parseDir ( conf->readonestring ( "/OpenspaceConfig/rightdir/dir" ) );
            type = conf->readonestring ( "/OpenspaceConfig/rightdir/type" );
            pathtype pt2 ( dir, type );
            right_frame = new Frame ( rightcontrolframe, rightframe, pt2, this, -1 );

            right_frame->f->toolbar->dock( topdock );
            right_frame->f->toolbar2->dock( rightdock );

            left_frame->f->filelist_opposite = right_frame->f;
            right_frame->f->filelist_opposite = left_frame->f;
            infoframe = new FXVerticalFrame ( ff, LAYOUT_FILL_X );
            networkframe = NULL;
            searchframe = NULL;
            getApp () ->addTimeout ( this, ID_TIMER, 200 );

            if ( conf->readonestring ( "/OpenspaceConfig/first_run" ) == "true" )
            {
                conf->saveonestring ( "/OpenspaceConfig/first_run", "false" );

                FirstRun* first_run;
                first_run = new FirstRun( this );
                first_run->show ( PLACEMENT_SCREEN );
            }
        }
        else
        {
            new FXLabel( this, "Configuration file version mismatch" );
            new FXLabel( this, "Please remove .openspace/openspacerc file in your home directory" );
            conf->loadconfig( true );
            delete conf;
            conf = NULL;
        }
    }
    else //configuration file broken
    {

        new FXLabel( this, "Configuration file is broken :|" );
        new FXLabel( this, "Restoring last good configuation file, restart program to apply changes" );

        conf->loadconfig( true );
        delete conf;
        conf = NULL;
    }

}


//----------------------------------------------------
// open configure window
long MainWindow::onOpenConfigure ( FXObject * sender, FXSelector sel, void * )
{
    if ( pref == NULL )
    {
        pref = new preferences ( this );
        pref->create();
    }
    pref->show ( PLACEMENT_OWNER );
}


//about
long MainWindow::onAbout ( FXObject * sender, FXSelector sel, void * )
{

    string msg = "Openspace File Browser " + string( PACKAGE_VERSION ) + "\n\nby Mateusz Dworak (http://openspace.linux.pl)\n\nUsing the FOX C++ GUI Library (http://www.fox-tookit.org) \n\n icons by Dawn Simon";

    FXMessageBox about ( this, "About Openspace", msg.c_str(), objmanager->osicons[ "foxbig" ], MBOX_OK | DECOR_TITLE | DECOR_BORDER );
    about.execute ();
}




//new frame
long MainWindow::onNewFrame ( FXObject * sender, FXSelector, void *ptr )
{
    string dir;
    string type;
    string str_server;
    string str_user;
    string str_pass;
    string str_port;
    if ( networkframe == NULL && searchframe == NULL )
    {
        dir = parseDir ( conf->readonestring ( "/OpenspaceConfig/defaultdir/dir" ) );
        type = conf->readonestring ( "/OpenspaceConfig/defaultdir/type" );
    }
    else
    {


        if ( networkframe == NULL )  //search
        {


            string s_size_greater = search_size_greater->getText().text();
            string s_size_less = search_size_less->getText().text();
            string s_dir = search_path->getText().text();
            string s_filter = search_filefilter->getText().text();

            str_server = "find " + s_dir;

            if ( s_filter != "" )
                str_server += " -name \"" + s_filter + "\"" ;
            if ( s_size_greater != "" )
                str_server += " -size +" + s_size_greater + "k" ;
            if ( s_size_less != "" )
                str_server += " -size -" + s_size_less + "k" ;


            dir = "/";
            type = "search";

            searchframe->hide ();
            delete searchframe;
            searchframe = NULL;
        }
        else            //network
        {
            dir = "/";
            type = filelisttypecombobox->getText().text();
            str_server = server->getText ().text ();
            str_user = user->getText ().text ();
            str_pass = password->getText ().text ();
            str_port = port->getText().text();
            networkframe->hide ();
            delete networkframe;
            networkframe = NULL;
        }


        controlframe->recalc ();
    }
    pathtype pt ( dir, type, str_server, str_user, str_pass, str_port );
    Frame *fr = new Frame ( controlframe, leftframe, pt, this, 0 );

    fr->frame->create ();
    fr->hf->create ();
    fr->f->toolbar->dock( topdock );
    fr->f->toolbar->hide();
    fr->f->toolbar2->dock( rightdock );
    fr->f->toolbar2->hide();


    this->handle ( fr->toleft, FXSEL ( SEL_LEFTBUTTONRELEASE, ID_TOLEFT ), NULL );

    controlframe->recalc ();
}


//show or hide frame where we can set login/pass/etc for connecting do remote filesystem
long MainWindow::onNewNetworkFrame ( FXObject * sender, FXSelector, void * )
{
    if ( networkframe == NULL )
    {
        networkframe = new FXHorizontalFrame ( controlframe, LAYOUT_FILL_X | FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );
        new FXLabel ( networkframe, "server:" );
        server = new FXTextField ( networkframe, 20 );
        new FXLabel ( networkframe, "user:", objmanager->osicons[ "user" ] );
        user = new FXTextField ( networkframe, 20 );
        new FXLabel ( networkframe, "pass:", objmanager->osicons[ "password" ] );
        password = new FXTextField ( networkframe, 20, NULL, 0, TEXTFIELD_PASSWD | FRAME_SUNKEN | FRAME_THICK );
        new FXLabel ( networkframe, "port:", NULL, 0, TEXTFIELD_INTEGER );
        port = new FXTextField ( networkframe, 5 );

        filelisttypecombobox = new FXComboBox ( networkframe, 4, NULL, 0,
                                                FRAME_THICK | LAYOUT_SIDE_TOP | COMBOBOX_STATIC );

        int count = 0;
        if ( conf->openxpath ( "/OpenspaceConfig/filelist" ) != -1 )
        {
            string res;
            while ( conf->getnextnode ( res ) )
            {
                string filelist_type = conf->readonestring ( "/OpenspaceConfig/filelist/" + res + "/type" );

                if ( filelist_type == "network" )
                {
                    filelisttypecombobox->appendItem ( res.c_str() );
                    count++;
                }
            }
        }


        filelisttypecombobox->setNumVisible ( count );

        new FXButton ( networkframe, "connect", NULL, this, MainWindow::ID_NEWFRAME, FRAME_RAISED, 0, 0, 0, 0, 0, 0, 0, 0 );
        networkframe->create ();
        networkframe->recalc ();
    }

    else
    {
        networkframe->hide ();
        delete networkframe;
        networkframe = NULL;
        controlframe->recalc ();
    }
}


long MainWindow::onNewSearchFrame ( FXObject * sender, FXSelector, void * )
{
    if ( searchframe == NULL )
    {
        searchframe = new FXHorizontalFrame ( controlframe, LAYOUT_FILL_X | FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );
        new FXLabel ( searchframe, "path: " );
        search_path = new FXTextField ( searchframe, 20 );
        if ( left_frame->f->active )
            search_path->setText( left_frame->f->path.c_str() );
        else
            search_path->setText( right_frame->f->path.c_str() );
        new FXLabel ( searchframe, "size" );
        new FXLabel ( searchframe, "greater than(KB):" );
        search_size_greater = new FXTextField ( searchframe, 10 );
        new FXLabel ( searchframe, "less than(KB):" );
        search_size_less = new FXTextField ( searchframe, 10 );


        new FXLabel ( searchframe, "file filter: " );
        search_filefilter = new FXTextField ( searchframe, 20 );
        search_filefilter->setText( "*" );
        new FXButton ( searchframe, "search", NULL, this, MainWindow::ID_NEWFRAME, FRAME_RAISED, 0, 0, 0, 0, 0, 0, 0, 0 );
        searchframe->create ();
        searchframe->recalc ();
    }

    else
    {
        searchframe->hide ();
        delete searchframe;
        searchframe = NULL;
        controlframe->recalc ();
    }
}


//----------------------------------------------------
MainWindow::~MainWindow ()
{
    delete pref;
    delete conf;
}


//----------------------------------------------------
void MainWindow::create ()
{
    FXMainWindow::create ();
    string position;
    if ( conf )
        position = conf->readonestring ( "/OpenspaceConfig/position" );

    if ( position == "cursor" )
        show ( PLACEMENT_CURSOR );
    else
        show ( PLACEMENT_SCREEN );
}

//----------------------------------------------------
//show or hide frame containing informaitions about copying/moving files
long MainWindow::commandsShow ( FXObject * sender, FXSelector, void *ptr )
{
    if ( infoframe->shown () )
        infoframe->hide ();

    else
        infoframe->show ();
}


//executed when we change directory clicking directory name in popoup window
long MainWindow::onChangeDir ( FXObject * sender, FXSelector, void *ptr )
{
    filelist * fil = current_frame->f;
    FXTRACE ( ( 5, "CHANGE DIR\n", fil->path.c_str () ) );
    FXMenuCommand *mc = ( FXMenuCommand * ) sender;
    FXEvent *event = ( FXEvent * ) ptr;
    FXWindow *win = ( FXWindow * ) mc->getParent ();
    int x = win->getX ();
    int y = win->getY ();
    string p = fil->path;
    string current_path = p;

    if ( current_path != "/" )
        current_path.append ( SEPARATOR );

    current_path.append ( mc->getText ().text () );
    current_frame->f->path = current_path;
    current_frame->f->opendir ( current_path );
    current_frame->generate_menu ( current_path, this );
    popupDir ( current_frame->f, current_path, x, y );
}

//popup with dir names when we click right button in dir path
int MainWindow::popupDir ( filelist * current_filelist, string path, int x, int y )
{
    vector < string > vec;
    for ( int i = 0; i < current_filelist->getNumItems (); i++ )
    {
        os_ListItem *oslistitem = ( os_ListItem * ) current_filelist->getItem ( i );
        if ( oslistitem->osf.type & FOLDER )
        {
            string name = oslistitem->osf.name;
            if ( name != "." && name != ".." && name[ 0 ] != '.' )
                vec.push_back ( name );
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
    if ( vec.size () > 0 )
    {
        sort ( vec.begin (), vec.end () );
        if ( filemenu )
        {
            filemenu->popdown ();
            delete filemenu;
            filemenu = NULL;
        }
        filemenu = new FXMenuPane ( this );
        for ( indx = 0; indx < vec.size (); indx++ )
        {
            new FXMenuCommand ( filemenu, vec[ indx ].c_str (), objmanager->osicons[ "directory" ], this, MainWindow::ID_DIRCH );
        }
        filemenu->create ();
        filemenu->popup ( NULL, x, y, 0, 0 );
    }
}


//currently nothing ;p
long MainWindow::onListNextDir ( FXObject * sender, FXSelector sel, void *ptr )
{}

//popup with directory names when we click right button in path
long MainWindow::onListDirs ( FXObject * sender, FXSelector, void *ptr )
{
    FXTRACE ( ( 5, "LIST DIRS\n" ) );
    FXButton *bt = ( FXButton * ) sender;
    box *boxel = ( box * ) bt->getUserData ();
    box *boxbackup = boxel;
    current_frame = boxel->fr;
    if ( boxel->fr != right_frame && boxel->fr != left_frame )
        return 0;
    string path = "";
    while ( boxel->prebutton != NULL )
    {
        boxel = ( box * ) boxel->prebutton->getUserData ();
    }
    while ( boxel != NULL )
    {
        if ( boxel == boxbackup )
        {
            path.append ( boxel->bt->getText ().text () );
            break;
        }
        path.append ( boxel->bt->getText ().text () );
        boxel = ( box * ) boxel->nextbutton->getUserData ();
    }
    current_frame->f->path = path;
    current_frame->f->opendir ( path );
    FXEvent *event = ( FXEvent * ) ptr;
    int x = event->root_x;
    int y = event->root_y;
    popupDir ( current_frame->f, path, x, y );
}

//change position or close frame
long MainWindow::onChangeList ( FXObject * sender, FXSelector sel, void *ptr )
{
    FXButton * bt = ( FXButton * ) sender;
    box *boxel = ( box * ) bt->getUserData ();
    FXushort id = FXSELID ( sel );
    int pos = -1;
    if ( id == ID_TOCLOSE )
    {
        bool canclose = true;

        vector < thread_elem * >::iterator iter;
        for ( iter = objmanager->thread_vec.begin (); iter != objmanager->thread_vec.end (); iter++ )
        {
            thread_elem *telem = *iter;
            if ( telem->fb == boxel->fr->f->fb )
            {
                canclose = false;
            }
        }
        if ( canclose )
        {

            boxel->fr->f->fb->quit ();
            delete boxel->fr->hf;
            delete boxel->fr->f->toolbar;
            delete boxel->fr->f->toolbar2;
            delete boxel->fr->frame;
            sender = NULL;

        }
        else
        {
            FXMessageBox about ( this, "error", "There are some processes connected with this filelist,\n finish them all and then you can close this filelist\n(this behavior will change in the next release of program)", NULL, MBOX_OK | DECOR_TITLE | DECOR_BORDER );
            about.execute ();

        }

        return 1;

    }

    else
    {
        if ( id == ID_TORIGHT )
        {
            pos = 1;
        }

        if ( left_frame->f->getWidth() == 0 )
            pos = 1;

        if ( right_frame->f->getWidth() == 0 )
            pos = -1;


    }
    if ( pos == -1 )
    {
        boxel->fr->frame->reparent ( leftframe );
        boxel->fr->hf->reparent ( leftcontrolframe );
        left_frame->hf->reparent ( controlframe );
        left_frame->f->toolbar->hide();
        left_frame->f->toolbar2->hide();
        left_frame->toleft->show ();
        left_frame->toright->show ();
        left_frame->toclose->show ();
        left_frame->frame->hide ();
        left_frame = boxel->fr;
        left_frame->frame->show ();

    }

    else
    {
        boxel->fr->frame->reparent ( rightframe );
        boxel->fr->hf->reparent ( rightcontrolframe );
        right_frame->hf->reparent ( controlframe );
        right_frame->f->toolbar->hide();
        right_frame->f->toolbar2->hide();
        right_frame->toleft->show ();
        right_frame->toright->show ();
        right_frame->toclose->show ();
        right_frame->frame->hide ();
        right_frame = boxel->fr;
        right_frame->frame->show ();
    }
    left_frame->f->filelist_opposite = right_frame->f;
    right_frame->f->filelist_opposite = left_frame->f;
    boxel->fr->toleft->hide ();
    boxel->fr->toright->hide ();
    boxel->fr->toclose->hide ();
    boxel->fr->f->handle ( this, FXSEL ( SEL_FOCUSIN, filelist::ID_ICO ), NULL );

    rightframe->recalc ();
    leftframe->recalc ();
}


//notify from filelist
long MainWindow::onNotify ( FXObject * sender, FXSelector sel, void *ptr )
{
    FXTRACE ( ( 5, "NOTIFY\n" ) );
    FXushort id = FXSELID ( sel );
    if ( id == 667 )
    {
        string * file_ptr = ( string * ) ptr;
        string file = *file_ptr;
        FXTRACE ( ( 5, "change to directory %s\n", file.c_str () ) );
        delete file_ptr;
        string dir;
        string type;
        string str_server = file;
        dir = "/";
        type = "archive";
        pathtype pt ( dir, type, str_server );
        Frame *fr;
        if ( left_frame->f->active )
            fr = new Frame ( leftcontrolframe, leftframe, pt, this, -1 );

        else
            fr = new Frame ( rightcontrolframe, rightframe, pt, this, -1 );
        if ( left_frame->f->active )
        {
            left_frame->hf->reparent ( controlframe );
            left_frame->toleft->show ();
            left_frame->toright->show ();
            left_frame->toclose->show ();
            left_frame->frame->hide ();
            left_frame->f->toolbar->hide();
            left_frame->f->toolbar2->hide();
            left_frame = fr;

        }

        else
        {
            right_frame->hf->reparent ( controlframe );
            right_frame->toleft->show ();
            right_frame->toright->show ();
            right_frame->toclose->show ();
            right_frame->frame->hide ();
            right_frame->f->toolbar->hide();
            right_frame->f->toolbar2->hide();
            right_frame = fr;


        }
        controlframe->recalc ();
        fr->frame->create ();
        fr->hf->create ();
        fr->frame->show ();
        fr->f->toolbar->dock( topdock );
        fr->f->toolbar2->dock( rightdock );

        left_frame->f->filelist_opposite = right_frame->f;
        right_frame->f->filelist_opposite = left_frame->f;

        if ( left_frame->f->active )
            left_frame->f->handle ( this, FXSEL ( SEL_FOCUSIN, filelist::ID_ICO ), NULL );
        else
            right_frame->f->handle ( this, FXSEL ( SEL_FOCUSIN, filelist::ID_ICO ), NULL );

        rightframe->recalc ();
        leftframe->recalc ();
    }

    else if ( id == 666 )       //directory change, for example user clicked double on direcotry, or clicked with 3rd button to go to parent dir
    {
        if ( left_frame->f->active )
            left_frame->generate_menu ( left_frame->f->path, this );

        else
            right_frame->generate_menu ( right_frame->f->path, this );
    }

    else if ( id == 668 )
    {
        if ( left->getWidth () == 0 || right->getWidth () == 0 )
            left->setWidth ( getWidth () / 2 );

        else
        {
            if ( left_frame->f == ( filelist * ) sender )
                left->setWidth ( right->getWidth () + left->getWidth () );

            else
                left->setWidth ( 0 );
        }
    }
}


//pressed button in path
long MainWindow::onOpenDir ( FXObject * sender, FXSelector, void *ptr )
{
    FXTRACE ( ( 5, "OPEN DIR\n" ) );
    FXButton *bt = ( FXButton * ) sender;
    box *boxel = ( box * ) bt->getUserData ();
    box *boxbackup = boxel;
    if ( boxel->fr != right_frame && boxel->fr != left_frame )
        return 0;
    string path = "";
    while ( boxel->prebutton != NULL )
    {
        boxel = ( box * ) boxel->prebutton->getUserData ();
    }
    while ( boxel != NULL )
    {
        if ( boxel == boxbackup )
        {
            break;
        }
        path.append ( boxel->bt->getText ().text () );
        boxel = ( box * ) boxel->nextbutton->getUserData ();
    }
    if ( path == "" )
        path = "/";
    boxbackup->fr->f->path = path;
    boxbackup->fr->f->opendir ( path );
    boxbackup->fr->generate_menu ( path, this );
}


//executed cyclically to update informations about progress of copying/moving/deleteing files
//using vector of special objects thread_elem to read from them information and to write request
//for example stoping copying.
long MainWindow::onTimer ( FXObject *, FXSelector, void * )
{
    vector < thread_elem * >::iterator iter;
    unsigned int indx;
    for ( iter = objmanager->thread_vec.begin (); iter != objmanager->thread_vec.end (); iter++ )
    {
        bool end = false;
        informationpanel *infp;
        thread_elem *telem = *iter;
        if ( telem->mutex.trylock () )
        {
            if ( telem->gui == NULL )
            {
                infp = new informationpanel ( infoframe, telem->command );
                telem->gui = ( void * ) infp;
                if ( telem->command == "copy" || telem->command == "move" || telem->command == "remove" )
                {
                    infp->bu->setUserData ( telem );
                    infp->bu->setSelector ( MainWindow::ID_CANCEL );
                    infp->bu->setTarget ( this );
                }

                else if ( telem->command == "execute" )
                {
                    string show_commands = conf->readonestring ( "/OpenspaceConfig/show_commands" );
                    if ( show_commands == "true" )
                    {
                        FXFont * font = new FXFont ( getApp (), "Arial", 8 );
                        if ( font )
                        {
                            font->create ();
                            infp->lab0->setFont ( font );
                        }
                        infp->lab0->setText ( telem->src[ 0 ].c_str () );
                        infp->frame->recalc ();
                    }

                    else
                        infp->frame->hide ();
                }

                else if ( telem->command == "init" )
                {
                    string text = "connecting ... ";
                    infp->lab0->setText ( text.c_str () );
                    infp->frame->recalc ();
                }
            }
            //popup with question about skipping or overwriting file
            if ( telem->question == true )
            {
                telem->question = false;
                telem->pane = new FXMenuPane ( this );
                FXButton *but = new FXButton ( telem->pane, "overwrite", NULL, this,
                                               ID_OVERWRITE );
                but->setUserData ( telem );
                but = new FXButton ( telem->pane, "skip", NULL, this, ID_SKIP );
                but->setUserData ( telem );
                but = new FXButton ( telem->pane, "skip all", NULL, this, ID_SKIP_ALL );
                but->setUserData ( telem );
                but = new FXButton ( telem->pane, "overwrite all", NULL, this, ID_OVERWRITE_ALL );
                but->setUserData ( telem );
                telem->pane->create ();
                telem->pane->popup ( NULL, this->getX () + 300, this->getY () + 300, but->getWidth (), 100 );
            }
            //when command is copy or move update information about progress
            if ( telem->command == "copy" || telem->command == "move" )
            {
                infp = ( informationpanel * ) telem->gui;
                string info = "total size: " + numtostring ( telem->total_size );
                infp->lab0->setText ( info.c_str () );
                info = "Current file: " + telem->act_file_name + " size: " + numtostring ( telem->file_size );
                infp->lab1->setText ( info.c_str () );
                double pct2 = 100 * ( double ) telem->act_total_size / ( double ) telem->total_size;
                infp->pbar2->setProgress ( ( int ) pct2 );
                double pct = 100 * ( double ) telem->act_file_size / ( double ) telem->file_size;
                infp->pbar->setProgress ( ( int ) pct );
                double diff = difftime ( time ( NULL ), *telem->tim );
                info = "time elapsed: " + ntos ( ( unsigned int ) diff ) + " copied size: ";
                info.append ( numtostring ( telem->act_file_size ) );
                if ( diff == 0 )
                    diff = 0.001;
                double speed = ( double ) telem->act_total_size / diff;
                info.append ( " Speed: " + numtostring ( ( unsigned int ) speed ) + "/s" );
                unsigned int eta = ( unsigned int ) ( ( diff * ( double ) telem->total_size ) / ( double ) telem->act_total_size );
                eta = eta - ( unsigned int ) diff;
                info.append ( " ETA: " + ntos ( eta ) );
                infp->lab2->setText ( info.c_str () );
            }

            else if ( telem->command == "remove" )
            {
                infp = ( informationpanel * ) telem->gui;
                string info = "Removing: " + telem->act_file_name;
                //FXTRACE ((5, "remove file %s\n", telem->act_file_name.c_str ()));
                infp->lab0->setText ( info.c_str () );
                infp->frame->recalc ();
            }

            else if ( telem->command == "execute" )
            {
                string::size_type pos = telem->options.find ( "capture" );
                if ( pos != string::npos )
                {
                    char readbuf[ 80 ];
                    int k = 0;
                    while ( read ( telem->p[ 0 ], readbuf, 1 ) > 0 && k++ < 2000 )
                    {
                        string s = "";
                        s = s + readbuf[ 0 ];
                        if ( s == "!" )
                            telem->end = true;

                        else
                            telem->dst = telem->dst + readbuf[ 0 ];
                    }
                }
            }
            end = telem->end;
            telem->mutex.unlock ();
            if ( end )      //when telem ended
            {

                if ( telem->error )
                {
                    FXMessageBox about ( this, "error", telem->msg.c_str(), NULL, MBOX_OK | DECOR_TITLE | DECOR_BORDER );
                    about.execute ();
                }
                filelist *fil = ( filelist* ) telem->filel;

                string::size_type pos = telem->options.find ( "download" );
                bool download = false;
                if ( pos != string::npos )
                {
                    download = true;

                }

                if ( telem->command == "init" )
                {
                    FXTRACE ( ( 5, "INIT \n" ) );
                    if ( fil->init () != false )
                    {
                        if ( left_frame->f->active )
                            left_frame->generate_menu ( fil->path, this );
                        else
                            right_frame->generate_menu ( fil->path, this );
                    }
                    else
                    {
                        FXMessageBox box( getApp (), "error", "Can't connect :|", NULL, MBOX_OK | DECOR_TITLE | DECOR_BORDER );
                        box.execute ();


                    }
                }
                if ( telem->command == "copy" )
                {
                    if ( ( left_frame->f->fb == telem->fb && download == true ) || ( download == false && right_frame->f->fb == telem->fb ) )
                        left_frame->f->refresh ();

                    else
                        right_frame->f->refresh ();
                }

                else if ( telem->command == "remove" )
                {
                    fil->refresh ();
                }

                else if ( telem->command == "move" )
                {
                    right_frame->f->refresh ();
                    left_frame->f->refresh ();
                }
                string options = telem->options;    // get special options
                pos = options.find ( "rescan" );    // when we need to rescan directory after command finish
                if ( pos != string::npos )
                {

                    fil->refresh ();

                }
                pos = telem->options.find ( "capture" );    // when we need to capture data from standard output from executed program
                if ( pos != string::npos )
                {
                    telem->pane = new FXMenuPane ( this );
                    FXLabel *lab = new FXLabel ( telem->pane, telem->dst.c_str () );
                    telem->pane->create ();
                    telem->pane->popup ( NULL, this->getX (), this->getY (), lab->getWidth (), lab->getHeight () );
                }
                //cleanup
                delete ( ( informationpanel * ) telem->gui );
                telem->gui = NULL;
                infoframe->recalc ();

                //erase from vector and delete telem;
                objmanager->thread_vec.erase ( iter );
                delete telem;
                break;
            }
        }
    }               //end for
    getApp () ->addTimeout ( this, ID_TIMER, 200 );
    return 1;
}


//when we choose in popup if we want overwrite or skip
//notify about this filelist plugin using telem
long MainWindow::onOverwrite ( FXObject * sender, FXSelector sel, void * )
{
    FXushort id = FXSELID ( sel );
    FXButton *but = ( FXButton * ) sender;
    thread_elem *te = ( thread_elem * ) but->getUserData ();
    te->mutex.lock ();
    if ( id == ID_OVERWRITE )
    {
        te->overwrite = true;
    }

    else if ( id == ID_SKIP )
    {
        te->overwrite = false;
    }

    else if ( ID_OVERWRITE_ALL )
    {
        te->overwrite = true;
        te->all = true;
    }

    else if ( id == ID_SKIP_ALL )
    {
        te->overwrite = false;
        te->all = true;
    }
    te->pane->hide ();
    delete te->pane;
    te->answer = true;
    te->mutex.unlock ();
}


//NEED TO CHANGE THIS
long MainWindow::onConfigure ( FXObject * sender, FXSelector sel, void *ptr )
{
    if ( !conf ) return 0;

    FXMainWindow::onConfigure ( sender, sel, ptr );
    float widthpanel = this->getWidth () * ratio;
    left->setWidth ( ( int ) widthpanel );

}
long MainWindow::onUpdate ( FXObject * sender, FXSelector sel, void *ptr )
{
    if ( !conf ) return 0;

    FXMainWindow::onUpdate ( sender, sel, ptr );
    float l = left->getWidth ();
    float r = right->getWidth ();
    float w = getWidth ();
    ratio = ( float ) l / ( float ) w;

}

//telem is canceled
long MainWindow::cancel ( FXObject * sender, FXSelector, void * )
{
    FXButton * but = ( FXButton * ) sender;
    thread_elem *te = ( thread_elem * ) but->getUserData ();
    te->mutex.lock ();
    te->cancel = true;
    te->mutex.unlock ();
}




//load icon from file and put in the array
void MainWindow::loadicons ( string icondir )
{

    objmanager = objectmanager::instance( getApp() );


    FXIconSource *source = new FXIconSource ( getApp () );
    FXString fil = icondir.c_str();
    FXString name = fil + conf->readonestring ( "/OpenspaceConfig/file_types/COMMON/types/dir/icon" ).c_str() + ".png";
    objmanager->specialicons[ 0 ] = source->loadIcon ( name );
    if ( objmanager->specialicons[ 0 ] )
        objmanager->specialicons[ 0 ] ->create();
    name = fil + "big_" + conf->readonestring ( "/OpenspaceConfig/file_types/COMMON/types/dir/icon" ).c_str() + ".png";
    objmanager->specialicons[ 1 ] = source->loadIcon ( name );
    if ( objmanager->specialicons[ 1 ] )
        objmanager->specialicons[ 1 ] ->create();
    name = fil + conf->readonestring ( "/OpenspaceConfig/file_types/COMMON/types/all/icon" ).c_str() + ".png";
    objmanager->specialicons[ 2 ] = source->loadIcon ( name );
    if ( objmanager->specialicons[ 2 ] )
        objmanager->specialicons[ 2 ] ->create();
    name = fil + "big_" + conf->readonestring ( "/OpenspaceConfig/file_types/COMMON/types/all/icon" ).c_str() + ".png";
    objmanager->specialicons[ 3 ] = source->loadIcon ( name );
    if ( objmanager->specialicons[ 3 ] )
        objmanager->specialicons[ 3 ] ->create();
    name = fil + conf->readonestring ( "/OpenspaceConfig/file_types/COMMON/types/executable/icon" ).c_str() + ".png";
    objmanager->specialicons[ 4 ] = source->loadIcon ( name );
    if ( objmanager->specialicons[ 4 ] )
        objmanager->specialicons[ 4 ] ->create();
    name = fil + conf->readonestring ( "/OpenspaceConfig/file_types/COMMON/types/symlink/icon" ).c_str() + ".png";
    objmanager->specialicons[ 5 ] = source->loadIcon ( name );
    if ( objmanager->specialicons[ 5 ] )
        objmanager->specialicons[ 5 ] ->create();


    struct stat status;
    struct dirent *dp;
    DIR *dirp;

    dirp = opendir ( icondir.c_str () );

    if ( dirp )
        while ( ( dp = readdir ( dirp ) ) != NULL )
        {

            if ( dp->d_name[ 0 ] != '.' || ( dp->d_name[ 1 ] != '\0' && ( dp->d_name[ 1 ] != '.' || dp->d_name[ 2 ] != '\0' ) ) )
            {
                string name = dp->d_name;

                //if (name.length () >= 3 && (name.substr (name.length () - 3, 3) == "gif" || name.substr (name.length () - 3, 3) == "png"))
                {

                    string file = icondir;
                    file.append ( name );
                    string shortname = name.substr ( 0, name.length () - 4 );

                    FXString fil_name = file.c_str();

                    objmanager->osicons[ shortname ] = source->loadIcon ( fil_name );

                    objmanager->osicons[ shortname ] ->create ();


                }

            }
        }

    closedir ( dirp );


}


// read color definied as FF FF FF and return FXColor
FXColor readcolor ( string col )
{
    if ( col == "" )
        return FXRGB ( 0, 0, 0 );

    return fxcolorfromname( col.c_str() );

}


//the same as above but defualt color is white
FXColor readcolor2 ( string col )
{
    if ( col == "" )
        return FXRGB ( 255, 255, 255 );
    return fxcolorfromname( col.c_str() );
}

string MainWindow::parseDir ( string dir )
{
    if ( dir == "{homedir}" )
        return FXFile::getHomeDirectory ().text ();

    else
        return dir;
}

bool MainWindow::loadMimeSettings ( string path, string type )
{

    string res2 = conf->readonestring ( path + "/icon" );
    string colorstr = conf->readonestring ( path + "/color" );
    FXColor color = readcolor ( colorstr );
    string backcolorstr = conf->readonestring ( path + "/backcolor" );
    FXColor backcolor = readcolor2 ( backcolorstr );
    if ( res2 != "" )
        objmanager->file_type_settings[ type ] = new file_type ( objmanager->osicons[ res2 ], objmanager->osicons[ "big_" + res2 ], color, backcolor );
    else
        return false;

}




//-----FRAME---------------------------------------------------------------------------------------------------------------------------
Frame::Frame ( FXComposite * cp, FXComposite * p, pathtype pt, FXObject * tgt, int position = 0 )
{
    this->pathdir = pt.dir;
    this->type = pt.type;
    string path = pathdir;
    hf = new FXHorizontalFrame ( cp, LAYOUT_FILL_X | FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    pathdir = path;

    //generate_menu(path,tgt);
    int z = 0;
    FXButton *prebutton = NULL;
    FXButton *nextbutton = NULL;

    objectmanager*objmanager = objectmanager::instance( cp->getApp() );


    toleft = new FXButton ( hf, "", objmanager->osicons[ "left" ], tgt, MainWindow::ID_TOLEFT, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );
    toleft->setUserData ( new box ( this, NULL, NULL, NULL ) );
    toclose = new FXButton ( hf, "", objmanager->osicons[ "close" ], tgt, MainWindow::ID_TOCLOSE, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );
    toclose->setUserData ( new box ( this, NULL, NULL, NULL ) );
    toright = new FXButton ( hf, "", objmanager->osicons[ "right" ], tgt, MainWindow::ID_TORIGHT, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );
    toright->setUserData ( new box ( this, NULL, NULL, NULL ) );
    if ( pt.server != "" )
    {
        string lab = pt.server + ": ";
        new FXLabel( hf, lab.c_str() );
    }

    //new FXLabel (hf," ");
    if ( position != 0 )
    {
        toleft->hide ();
        toclose->hide ();
        toright->hide ();
    }
    while ( z != -1 )
    {
        string path_element = "";
        z = path.find ( SEPARATOR, 1 );
        if ( z == -1 )
            path_element.append ( path );

        else
            path_element.append ( path.substr ( 0, z ) );
        path.erase ( 0, z );
        FXButton *bt = new FXButton ( hf, path_element.c_str (), NULL, tgt,
                                      MainWindow::ID_DIR, FRAME_THICK, 0, 0, 0, 0, 0, 0,
                                      0, 0 );
        bt->setBackColor ( objmanager->maincolor );
        if ( prebutton != NULL )
        {
            box * boxel = ( box * ) prebutton->getUserData ();
            boxel->nextbutton = bt;
        }

        else
            firstbutton = bt;
        bt->setUserData ( new box ( this, prebutton, bt, NULL ) );
        prebutton = bt;
    }

    frame = new FXVerticalFrame ( p, LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_SUNKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    f = new filelist ( frame, pt );
    if ( position == 0 )
    {
        frame->hide ();
    }
}


//generate buttons path for given path
void Frame::generate_menu ( string path, FXObject * tgt )
{

    objectmanager * objmanager = objectmanager::instance( hf->getApp() );

    FXTRACE ( ( 5, "GENERATE MENU path %s ", path.c_str () ) );
    if ( path[ 0 ] == '/' && path[ 1 ] == '/' )
        path.erase ( 0, 1 );
    box *boxel = ( box * ) firstbutton->getUserData ();
    box *boxbackup = boxel;
    while ( boxel )
    {
        box * boxel_del = boxel;
        if ( boxel->nextbutton != NULL )
            boxel = ( box * ) boxel->nextbutton->getUserData ();

        else
            boxel = NULL;
        delete boxel_del->bt;
        delete boxel_del;
        boxel_del->bt = NULL;
        boxel_del = NULL;
    }
    this->pathdir = path;
    int z = 0;
    FXButton *prebutton = NULL;
    FXButton *nextbutton = NULL;
    while ( z != -1 )
    {
        string path_element = "";
        z = path.find ( SEPARATOR, 1 );
        if ( z == -1 )
            path_element.append ( path );

        else
            path_element.append ( path.substr ( 0, z ) );
        path.erase ( 0, z );
        FXButton *bt = new FXButton ( hf, path_element.c_str (), NULL, tgt,
                                      MainWindow::ID_DIR, FRAME_THICK, 0, 0, 0, 0, 0, 0,
                                      0, 0 );
        bt->setBackColor ( objmanager->maincolor );
        bt->create ();
        if ( prebutton != NULL )
        {
            box * boxel = ( box * ) prebutton->getUserData ();
            boxel->nextbutton = bt;
        }

        else
            firstbutton = bt;
        bt->setUserData ( new box ( this, prebutton, bt, NULL ) );
        prebutton = bt;
    }
}
