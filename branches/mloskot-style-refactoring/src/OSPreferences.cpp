// $Id$
/////////////////////////////////////////////////////////////////////////////
// This file is part of OpenSpace project.
//
// OpenSpace is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// OpenSpace is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenSpace; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
/////////////////////////////////////////////////////////////////////////////
//
// C++ Implementation: OSPreferences
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////

#include "OSPreferences.h"
#include "MimeType.h"
#include <fx.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

#include "FXDLL.h"
#include "OSVirtualFileSystemBase.h"
#include <fstream>
#include <sstream>
#include "config.h"

FXDEFMAP ( OSPreferences ) preferencesMap[] =
    {
        //________Message_Type_____________________ID____________Message_Handler_______
        //FXMAPFUNC (SEL_COMMAND, OSPreferences::ID_SAVE, OSPreferences::onSave),
        FXMAPFUNC ( SEL_COMMAND, OSPreferences::ID_COMMAND_CHANGE, OSPreferences::onCommandChange ),
        FXMAPFUNC ( SEL_COMMAND, OSPreferences::ID_FILETYPE_CHANGE, OSPreferences::onFileTypeChange ),
        FXMAPFUNC ( SEL_COMMAND, OSPreferences::ID_SHUTTER_CHANGE, OSPreferences::onShutterChange ),
        FXMAPFUNC ( SEL_COMMAND, OSPreferences::ID_TOOLBAR_CHANGE, OSPreferences::onToolbarChange ),
        FXMAPFUNC ( SEL_COMMAND, OSPreferences::ID_VFS_CHANGE, OSPreferences::onVfsChange ),
        FXMAPFUNC ( SEL_COMMAND, OSPreferences::ID_NEW_COMMAND, OSPreferences::onNewCommand ),
        FXMAPFUNC ( SEL_COMMAND, OSPreferences::ID_REMOVE_COMMAND, OSPreferences::onRemoveCommand ),
        FXMAPFUNC ( SEL_COMMAND, OSPreferences::ID_NEW_SHUTTER, OSPreferences::onNewShutter ),
        FXMAPFUNC ( SEL_COMMAND, OSPreferences::ID_REMOVE_SHUTTER, OSPreferences::onRemoveShutter ),
        FXMAPFUNCS ( SEL_COMMAND, OSPreferences::ID_MIME_APP, OSPreferences::ID_MIME_APP_AUTO, OSPreferences::onOpenMimeApp ),
        FXMAPFUNC ( SEL_COMMAND, OSPreferences::ID_ADD_FILETYPE, OSPreferences::onAddFiletype ),
        FXMAPFUNCS ( SEL_COMMAND, OSPreferences::ID_CHOOSE_COLOR, OSPreferences::ID_CHOOSE_BACKCOLOR, OSPreferences::onChooseColor ),
        FXMAPFUNCS ( SEL_COMMAND, OSPreferences::ID_ADD_COMMAND_ADDITIONAL, OSPreferences::ID_DEL_COMMAND_ADDITIONAL, OSPreferences::onAdditionalCommandChange ),
        FXMAPFUNCS ( SEL_COMMAND, OSPreferences::ID_ADD_BUTTON_COMMAND, OSPreferences::ID_DEL_BUTTON_COMMAND, OSPreferences::onAddButtonCommand ),
        FXMAPFUNCS ( SEL_COMMAND, OSPreferences::ID_ADD_SHUTTER_COMMAND, OSPreferences::ID_DEL_SHUTTER_COMMAND, OSPreferences::onAddShutterCommand ),
        FXMAPFUNCS ( SEL_COMMAND, OSPreferences::ID_ADD_HEADER, OSPreferences::ID_DEL_HEADER, OSPreferences::onAddHeader ),
        FXMAPFUNC( SEL_CLOSE, 0, OSPreferences::close ),
        FXMAPFUNCS ( SEL_COMMAND, OSPreferences::ID_DOWNLOAD_INSTALL_CMD_PLUGIN, OSPreferences::ID_DOWNLOAD_INSTALL_VFS_PLUGIN, OSPreferences::downloadInstallPlugin ),
        FXMAPFUNCS ( SEL_COMMAND, OSPreferences::ID_UPDATE_CMD_PLUGIN_LIST, OSPreferences::ID_UPDATE_VFS_PLUGIN_LIST, OSPreferences::updatePluginList ),
        FXMAPFUNC( SEL_COMMAND, OSPreferences::ID_COLORS, OSPreferences::onColorChanged ),
        FXMAPFUNC( SEL_CHANGED, OSPreferences::ID_COLORS, OSPreferences::onColorChanged ),
        FXMAPFUNCS( SEL_COMMAND, OSPreferences::ID_CHOOSE_FONT, OSPreferences::ID_CHOOSE_CAPTIONFONT3, OSPreferences::onChooseFont ),
        FXMAPFUNC( SEL_COMMAND, OSPreferences::ID_UPDATE_WINDOW_SIZE, OSPreferences::updateWindowSize ),
        FXMAPFUNCS( SEL_COMMAND, OSPreferences::ID_UP_BUTTON_COMMAND, OSPreferences::ID_DOWN_SHUTTER_COMMAND, OSPreferences::onUpDown ),
        FXMAPFUNCS( SEL_COMMAND, OSPreferences::ID_UP_VFS_HEADER, OSPreferences::ID_DOWN_VFS_HEADER, OSPreferences::onUpDownVfsHeader ),
        FXMAPFUNCS( SEL_COMMAND, OSPreferences::ID_UP_COMMAND, OSPreferences::ID_DOWN_COMMAND, OSPreferences::onUpDownCommand ),

    };


FXIMPLEMENT ( OSPreferences, FXDialogBox, preferencesMap, ARRAYNUMBER ( preferencesMap ) )

long OSPreferences::onUpDownCommand( FXObject * sender, FXSelector sel, void * )
{
    if ( additionalCommands->getNumItems () == 0 )
        return 0;

    FXushort id = FXSELID( sel );
    if ( id == ID_UP_COMMAND )
    {
        if ( additionalCommands->getCurrentItem() != 0 )
            additionalCommands->moveItem( additionalCommands->getCurrentItem() - 1, additionalCommands->getCurrentItem() );
    }
    else
    {
        if ( additionalCommands->getCurrentItem() != additionalCommands->getNumItems () - 1 )
            additionalCommands->moveItem( additionalCommands->getCurrentItem() + 1, additionalCommands->getCurrentItem() );
    }

    filetype_container *ct_prev = &filetypesMap[ currentFileType ];
    ct_prev->commands.clear();

    for ( int i = 0; i < additionalCommands->getNumItems (); i++ )
    {
        ct_prev->commands.push_back( additionalCommands->getItemText( i ).text() );
    }



}

long OSPreferences::onUpDownVfsHeader( FXObject * sender, FXSelector sel, void * )
{

    if ( headersList->getNumItems () == 0 )
        return 0;

    FXushort id = FXSELID( sel );
    if ( id == ID_UP_VFS_HEADER )
    {
        if ( headersList->getCurrentItem() != 0 )
            headersList->moveItem( headersList->getCurrentItem() - 1, headersList->getCurrentItem() );
    }
    else
    {
        if ( headersList->getCurrentItem() != headersList->getNumItems () - 1 )
            headersList->moveItem( headersList->getCurrentItem() + 1, headersList->getCurrentItem() );
    }

    this->onVfsChange( NULL, 0, NULL );

}

long OSPreferences::onUpDown( FXObject * sender, FXSelector sel, void * )
{




    FXList * updownlist;
    FXListBox * box;
    bool up;

    FXushort id = FXSELID( sel );
    if ( id == ID_UP_BUTTON_COMMAND )
    {
        updownlist = buttonsList;
        box = toolbarList;
        up = true;
    }
    else if ( id == ID_DOWN_BUTTON_COMMAND )
    {
        updownlist = buttonsList;
        box = toolbarList;
        up = false;
    }
    else if ( id == ID_UP_SHUTTER_COMMAND )
    {
        updownlist = shutterCommands;
        box = shutterList;
        up = true;
    }
    else if ( id == ID_DOWN_SHUTTER_COMMAND )
    {
        updownlist = shutterCommands;
        box = shutterList;
        up = false;
    }

    if ( updownlist->getNumItems () == 0 )
        return 0;

    if ( up )
    {
        if ( updownlist->getCurrentItem() != 0 )
            updownlist->moveItem( updownlist->getCurrentItem() - 1, updownlist->getCurrentItem() );
    }
    else
    {
        if ( updownlist->getCurrentItem() != updownlist->getNumItems () - 1 )
            updownlist->moveItem( updownlist->getCurrentItem() + 1, updownlist->getCurrentItem() );
    }

    string actual_box = box->getItem( box->getCurrentItem() ).text();

    if ( id == ID_UP_BUTTON_COMMAND || id == ID_DOWN_BUTTON_COMMAND )
    {
        vector <toolbar_container>::iterator iter;
        for ( iter = toolbarVector.begin();iter != toolbarVector.end(); iter++ )
        {
            if ( iter->toolbar == actual_box )
            {
                toolbarVector.erase( iter );
                iter = toolbarVector.begin();
            }
        }

        for ( int i = 0; i < updownlist->getNumItems (); i++ )
        {
            string cmd = updownlist->getItem( i ) ->getText().text();
            toolbarVector.push_back( toolbar_container( actual_box, cmd ) );
        }
    }
    else if ( id == ID_DOWN_SHUTTER_COMMAND || id == ID_UP_SHUTTER_COMMAND )
    {
        vector <shutter_container>::iterator iter;
        for ( iter = shutterVector.begin();iter != shutterVector.end(); iter++ )
        {
            if ( iter->shutter == actual_box )
            {
                shutterVector.erase( iter );
                iter = shutterVector.begin();
            }
        }

        for ( int i = 0; i < updownlist->getNumItems (); i++ )
        {
            string cmd = updownlist->getItem( i ) ->getText().text();
            shutterVector.push_back( shutter_container( actual_box, cmd ) );
        }
    }

}


long OSPreferences::updateWindowSize( FXObject * sender, FXSelector sel, void * )
{
    mainwindow_width->setValue( getOwner () ->getWidth() );
    mainwindow_height->setValue( getOwner () ->getHeight() );
}

OSPreferences::OSPreferences ( FXWindow * owner ) : FXDialogBox ( owner, "Preferences", DECOR_ALL, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4 )
{

    objmanager = objectmanager::instance( getApp() );
    getApp() ->getNormalFont() ->create();
    FXFontDesc fontdescription;
    getApp() ->getNormalFont() ->getFontDesc( fontdescription );

    font[ 0 ] = new FXFont( getApp(), fontdescription );
    font[ 0 ] ->create();
    objmanager->captionfont1->getFontDesc( fontdescription );
    font[ 1 ] = new FXFont( getApp(), fontdescription );
    font[ 1 ] ->create();
    objmanager->captionfont2->getFontDesc( fontdescription );
    font[ 2 ] = new FXFont( getApp(), fontdescription );
    font[ 2 ] ->create();
    objmanager->captionfont3->getFontDesc( fontdescription );
    font[ 3 ] = new FXFont( getApp(), fontdescription );
    font[ 3 ] ->create();

    typingSpeed = getApp() ->getTypingSpeed();
    clickSpeed = getApp() ->getClickSpeed();
    scrollSpeed = getApp() ->getScrollSpeed();
    scrollDelay = getApp() ->getScrollDelay();
    blinkSpeed = getApp() ->getBlinkSpeed();
    animSpeed = getApp() ->getAnimSpeed();
    menuPause = getApp() ->getMenuPause();
    tooltipPause = getApp() ->getTooltipPause();
    tooltipTime = getApp() ->getTooltipTime();
    dragDelta = getApp() ->getDragDelta();
    wheelLines = getApp() ->getWheelLines();


    target_typingspeed.connect( typingSpeed );
    target_clickspeed.connect( clickSpeed );
    target_scrollspeed.connect( scrollSpeed );
    target_scrolldelay.connect( scrollDelay );
    target_blinkspeed.connect( blinkSpeed );
    target_animspeed.connect( animSpeed );
    target_menupause.connect( menuPause );
    target_tooltippause.connect( tooltipPause );
    target_tooltiptime.connect( tooltipTime );
    target_dragdelta.connect( dragDelta );
    target_wheellines.connect( wheelLines );

    tooltip = new FXToolTip( getApp() );

    /// Retrieve Current Color Settings
    hilite = getApp() ->getHiliteColor();
    shadow = getApp() ->getShadowColor();
    base = getApp() ->getBaseColor();
    back = getApp() ->getBackColor();
    border = getApp() ->getBorderColor();
    fore = getApp() ->getForeColor();
    selfore = getApp() ->getSelforeColor();
    selback = getApp() ->getSelbackColor();
    tipfore = getApp() ->getTipforeColor();
    tipback = getApp() ->getTipbackColor();
    menufore = getApp() ->getSelMenuTextColor();
    menuback = getApp() ->getSelMenuBackColor();
    main = fxcolorfromname( conf->readonestring ( "/OpenspaceConfig/colors/main" ).c_str() );

    target_base.connect( base );
    target_back.connect( back );
    target_border.connect( border );
    target_fore.connect( fore );
    target_hilite.connect( hilite );
    target_shadow.connect( shadow );
    target_selfore.connect( selfore );
    target_selback.connect( selback );
    target_tipfore.connect( tipfore );
    target_tipback.connect( tipback );
    target_menufore.connect( menufore );
    target_menuback.connect( menuback );
    target_main.connect( main );

    target_base.setTarget( this );
    target_back.setTarget( this );
    target_border.setTarget( this );
    target_fore.setTarget( this );
    target_hilite.setTarget( this );
    target_shadow.setTarget( this );
    target_selfore.setTarget( this );
    target_selback.setTarget( this );
    target_tipfore.setTarget( this );
    target_tipback.setTarget( this );
    target_menufore.setTarget( this );
    target_menuback.setTarget( this );
    target_main.setTarget( this );

    target_base.setSelector( ID_COLORS );
    target_back.setSelector( ID_COLORS );
    target_border.setSelector( ID_COLORS );
    target_fore.setSelector( ID_COLORS );
    target_hilite.setSelector( ID_COLORS );
    target_shadow.setSelector( ID_COLORS );
    target_selfore.setSelector( ID_COLORS );
    target_selback.setSelector( ID_COLORS );
    target_tipfore.setSelector( ID_COLORS );
    target_tipback.setSelector( ID_COLORS );
    target_menufore.setSelector( ID_COLORS );
    target_menuback.setSelector( ID_COLORS );
    target_main.setSelector( ID_COLORS );

    mimeapp = NULL;
    saveconfiguration = true;


    colordlg = new FXColorDialog( this, "Color Dialog" );

    FXVerticalFrame *vertical = new FXVerticalFrame ( this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y );
    FXHorizontalFrame *horizontal = new FXHorizontalFrame ( vertical, LAYOUT_FILL_X | LAYOUT_FILL_Y );
    FXVerticalFrame *buttons = new FXVerticalFrame ( horizontal, LAYOUT_LEFT | LAYOUT_FILL_Y | FRAME_SUNKEN | PACK_UNIFORM_WIDTH | LAYOUT_FIX_HEIGHT, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0 );
    FXSwitcher *switcher = new FXSwitcher ( horizontal, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0 );


    FXVerticalFrame *mainpane = new FXVerticalFrame ( switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y );

    new FXButton ( buttons, "Main Settings", NULL, switcher, FXSwitcher::ID_OPEN_FIRST, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y );
    new FXLabel ( mainpane, "Let program to autoconfigure file type associacion settings" );
    FXHorizontalFrame *hoz1 = new FXHorizontalFrame ( mainpane, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    new FXButton ( hoz1, "Semi-Auto configure", NULL, this, OSPreferences::ID_MIME_APP );
    new FXButton ( hoz1, "Full-Auto configure", NULL, this, OSPreferences::ID_MIME_APP_AUTO );
    new FXSeparator( mainpane );



    FXHorizontalFrame *hoz2 = new FXHorizontalFrame ( mainpane, LAYOUT_FILL_X );
    new FXLabel ( hoz2, "width:" );
    mainwindow_width = new FXSpinner( hoz2, 4 );
    mainwindow_width->setRange( 1, 2000 );
    mainwindow_width->setIncrement( 1 );
    mainwindow_width->setValue( atoi( conf->readonestring ( "/OpenspaceConfig/mainwindow/width" ).c_str () ) );
    new FXLabel ( hoz2, "height:" );
    mainwindow_height = new FXSpinner( hoz2, 4 );
    mainwindow_height->setRange( 1, 2000 );
    mainwindow_height->setIncrement( 1 );
    mainwindow_height->setValue( atoi( conf->readonestring ( "/OpenspaceConfig/mainwindow/height" ).c_str () ) );

    new FXButton ( hoz2, "get current openspace size", NULL, this, OSPreferences:: ID_UPDATE_WINDOW_SIZE );

    new FXSeparator( mainpane );

    new FXLabel ( mainpane, "left directory:" );
    leftdir = new FXTextField ( mainpane, 50 );
    leftdir->setText( conf->readonestring ( "/OpenspaceConfig/leftdir/dir" ).c_str () );

    new FXLabel ( mainpane, "right directory (in single directory window mode this is default window):" );
    rightdir = new FXTextField ( mainpane, 50 );
    rightdir->setText( conf->readonestring ( "/OpenspaceConfig/rightdir/dir" ).c_str () );

    new FXLabel ( mainpane, "default new opened directory:" );
    defaultdir = new FXTextField ( mainpane, 50 );
    defaultdir->setText( conf->readonestring ( "/OpenspaceConfig/defaultdir/dir" ).c_str () );

    new FXSeparator( mainpane );

    new FXLabel ( mainpane, "maximum size of image file for generating thumbnails (in Bytes)" );

    thumbsize = new FXSpinner( mainpane, 8 );
    thumbsize->setRange( 0, 10000000 );
    thumbsize->setIncrement( 1000 );
    thumbsize->setValue( atoi( conf->readonestring ( "/OpenspaceConfig/filelist/local/thumbs/size" ).c_str () ) );

    new FXLabel ( mainpane, "icons theme:" );
    iconsTheme = new FXListBox ( mainpane );
    iconsTheme->setNumVisible( 5 );

    new FXLabel ( mainpane, "openspace placement on startup" );
    placement = new FXListBox ( mainpane );
    placement->setNumVisible( 2 );
    placement->appendItem( "centered" );
    placement->appendItem( "cursor" );

    string iconsdir = conf->readonestring ( "/OpenspaceConfig/path" ) + "/icons";
    struct dirent *dp;
    DIR *dirp;
    dirp = opendir ( iconsdir.c_str () );
    if ( dirp )
        while ( ( dp = readdir ( dirp ) ) != NULL )
        {
            if ( dp->d_name[ 0 ] != '.' || ( dp->d_name[ 1 ] != '\0' && ( dp->d_name[ 1 ] != '.' || dp->d_name[ 2 ] != '\0' ) ) )
            {
                string name = dp->d_name;
                iconsTheme->appendItem( name.c_str() );
            }
        }
    closedir ( dirp );
    iconsdir = FXFile::getUserDirectory ( "" ).text () + string( "/.openspace/icons" );

    dirp = opendir ( iconsdir.c_str () );
    if ( dirp )
        while ( ( dp = readdir ( dirp ) ) != NULL )
        {
            if ( dp->d_name[ 0 ] != '.' || ( dp->d_name[ 1 ] != '\0' && ( dp->d_name[ 1 ] != '.' || dp->d_name[ 2 ] != '\0' ) ) )
            {
                string name = dp->d_name;
                iconsTheme->appendItem( name.c_str() );
            }
        }
    closedir ( dirp );

    iconsTheme->setCurrentItem( iconsTheme->findItem( conf->readonestring ( "/OpenspaceConfig/icons_theme" ).c_str() ) );




    //======================================================BUTTONS===========================================================

    FXVerticalFrame *buttonsPane = new FXVerticalFrame ( switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

    new FXButton ( buttons, "Menu and Buttons Settings", NULL, switcher, FXSwitcher::ID_OPEN_SECOND, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y );

    new FXLabel ( buttonsPane, "Buttons in configurable toolbox:", NULL, LAYOUT_LEFT );

    toolbarList = new FXListBox ( buttonsPane, this, ID_TOOLBAR_CHANGE );
    toolbarList->setNumVisible( 10 );

    FXHorizontalFrame *hfx = new FXHorizontalFrame ( buttonsPane, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

    buttonsList = new FXList ( hfx, NULL, 0, LIST_NORMAL | LAYOUT_FIX_WIDTH, 0, 0, 250 );
    buttonsList->setNumVisible( 5 );

    new FXLabel( hfx, " " );
    new FXArrowButton( hfx, this, ID_UP_BUTTON_COMMAND, FRAME_RAISED | FRAME_THICK | ARROW_UP );
    new FXArrowButton( hfx, this, ID_DOWN_BUTTON_COMMAND, FRAME_RAISED | FRAME_THICK | ARROW_DOWN );

    if ( conf->openxpath( "/OpenspaceConfig/toolbars" ) != -1 )
    {
        string commandstr;
        while ( conf->getnextnode( commandstr ) )
        {
            toolbarList->appendItem( commandstr.c_str() );
            buttonsList->clearItems();

            configure conflocal = *conf;

            if ( conflocal.openxpath( "/OpenspaceConfig/toolbars/" + commandstr + "/command" ) != -1 )
            {
                string commandstr2;
                while ( conflocal.getnextstring( commandstr2 ) )
                {
                    string icon = conflocal.readonestring ( "/OpenspaceConfig/commands/" + commandstr2 + "/icon" );
                    buttonsList->appendItem( commandstr2.c_str(), objmanager->osicons[ icon ] );
                    toolbarVector.push_back( toolbar_container( commandstr, commandstr2 ) );
                }
            }


        }
    }
    toolbarList->setCurrentItem ( toolbarList->getNumItems () - 1 );

    FXHorizontalFrame *buttonsHframe = new FXHorizontalFrame ( buttonsPane, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

    new FXLabel( buttonsHframe, " " );
    new FXLabel( buttonsHframe, " " );
    new FXArrowButton( buttonsHframe, this, ID_ADD_BUTTON_COMMAND, FRAME_RAISED | FRAME_THICK | ARROW_UP );
    new FXArrowButton( buttonsHframe, this, ID_DEL_BUTTON_COMMAND, FRAME_RAISED | FRAME_THICK | ARROW_DOWN );


    new FXLabel ( buttonsPane, "All available commands:", NULL, LAYOUT_LEFT );

    additionalCommandsAvailableForButtons = new FXList ( buttonsPane, NULL, 0, LIST_NORMAL | LAYOUT_FIX_WIDTH, 0, 0, 250 );
    additionalCommandsAvailableForButtons->setNumVisible( 8 );
    additionalCommandsAvailableForButtons->appendItem( "SEPARATOR" );
    FXHorizontalFrame *buttonsHframe2 = new FXHorizontalFrame ( buttonsPane, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

    new FXLabel( buttonsHframe2, " " );
    new FXLabel( buttonsHframe2, " " );
    new FXArrowButton( buttonsHframe2, this, ID_DEL_SHUTTER_COMMAND, FRAME_RAISED | FRAME_THICK | ARROW_UP );
    new FXArrowButton( buttonsHframe2, this, ID_ADD_SHUTTER_COMMAND, FRAME_RAISED | FRAME_THICK | ARROW_DOWN );
    new FXLabel ( buttonsPane, "Shutter:", NULL, LAYOUT_LEFT );


    shutterList = new FXListBox ( buttonsPane, this, ID_SHUTTER_CHANGE );
    shutterList->setNumVisible( 10 );

    hfx = new FXHorizontalFrame ( buttonsPane, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );


    shutterCommands = new FXList ( hfx, NULL, 0, LIST_NORMAL | LAYOUT_FIX_WIDTH, 0, 0, 250 );
    shutterCommands->setNumVisible( 5 );

    new FXLabel( hfx, " " );

    new FXArrowButton( hfx, this, ID_UP_SHUTTER_COMMAND, FRAME_RAISED | FRAME_THICK | ARROW_UP );
    new FXArrowButton( hfx, this, ID_DOWN_SHUTTER_COMMAND, FRAME_RAISED | FRAME_THICK | ARROW_DOWN );


    new FXSeparator( buttonsPane );
    FXHorizontalFrame *hfr = new FXHorizontalFrame ( buttonsPane, LAYOUT_FILL_X | LAYOUT_FILL_Y );
    new FXButton ( hfr, "Remove shutter", NULL, this, ID_REMOVE_SHUTTER, FRAME_RAISED | ICON_ABOVE_TEXT );
    new FXButton ( hfr, "New shutter", NULL, this, ID_NEW_SHUTTER, FRAME_RAISED | ICON_ABOVE_TEXT );
    newShutterEdit = new FXTextField ( hfr, 20 );


    if ( conf->openxpath( "/OpenspaceConfig/shutter" ) != -1 )
    {
        string commandstr;
        while ( conf->getnextnode( commandstr ) )
        {
            shutterList->appendItem( commandstr.c_str() );
            shutterCommands->clearItems();

            configure conflocal = *conf;

            if ( conflocal.openxpath( "/OpenspaceConfig/shutter/" + commandstr + "/command" ) != -1 )
            {
                string commandstr2;
                while ( conflocal.getnextstring( commandstr2 ) )
                {
                    shutterCommands->appendItem( commandstr2.c_str() );
                    shutterVector.push_back( shutter_container( commandstr, commandstr2 ) );
                }
            }


        }
    }

    shutterList->setCurrentItem ( shutterList->getNumItems () - 1 );


    //======================================================COMMANDS===========================================================


    FXVerticalFrame *commandPluginsPane = new FXVerticalFrame ( switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

    new FXButton ( buttons, "Command plugins Settings", NULL, switcher, FXSwitcher::ID_OPEN_THIRD, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y );

    new FXLabel ( commandPluginsPane, "installed plugins" );
    commandPluginsList = new FXListBox ( commandPluginsPane, this, ID_COMMANDPLUGIN_CHANGE );
    commandPluginsList->setNumVisible( 30 );


    FXPacker *vv = new FXGroupBox ( commandPluginsPane, "available plugins - download from internet", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0 );

    hfr = new FXHorizontalFrame ( vv, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    availableCommandPluginsList = new FXListBox ( hfr );
    availableCommandPluginsList->setNumVisible( 30 );
    new FXButton ( hfr, "Download and Install", NULL, this, ID_DOWNLOAD_INSTALL_CMD_PLUGIN, FRAME_RAISED | ICON_ABOVE_TEXT );
    new FXButton ( vv, "Update available plugins list", NULL, this, ID_UPDATE_CMD_PLUGIN_LIST, FRAME_RAISED | ICON_ABOVE_TEXT );



    string plugin_path = string( PATH_LIBDIR ) + "/openspace/plugins/cmddialog";




    dirp = opendir ( plugin_path.c_str () );
    if ( dirp )
        while ( ( dp = readdir ( dirp ) ) != NULL )
        {
            if ( dp->d_name[ 0 ] != '.' || ( dp->d_name[ 1 ] != '\0' && ( dp->d_name[ 1 ] != '.' || dp->d_name[ 2 ] != '\0' ) ) )
            {
                string name = dp->d_name;
                if ( name.substr( name.length() - 3, 3 ) == ".so" )
                {
                    name = name.substr( 0, name.length() - 3 );
                    name = name.substr( 3, name.length() - 3 );
                    commandPluginsList->appendItem( name.c_str() );
                }


            }
        }

    closedir ( dirp );

    plugin_path = FXFile::getUserDirectory ( "" ).text () + string( "/.openspace/plugins/cmddialog" );

    dirp = opendir ( plugin_path.c_str () );
    if ( dirp )
        while ( ( dp = readdir ( dirp ) ) != NULL )
        {
            if ( dp->d_name[ 0 ] != '.' || ( dp->d_name[ 1 ] != '\0' && ( dp->d_name[ 1 ] != '.' || dp->d_name[ 2 ] != '\0' ) ) )
            {
                string name = dp->d_name;
                if ( name.substr( name.length() - 3, 3 ) == ".so" )
                {
                    name = name.substr( 0, name.length() - 3 );
                    name = name.substr( 3, name.length() - 3 );
                    commandPluginsList->appendItem( name.c_str() );
                }


            }
        }

    closedir ( dirp );






    commandsPane = new FXVerticalFrame ( switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    new FXLabel ( commandsPane, "Commands settings", NULL, LAYOUT_LEFT );

    new FXButton ( buttons, "Commands Settings", NULL, switcher, FXSwitcher::ID_OPEN_FOURTH, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y );

    commandsCombo = new FXListBox ( commandsPane, this, ID_COMMAND_CHANGE );
    new FXLabel ( commandsPane, "icon: ", NULL, LAYOUT_LEFT );
    iconsList = new FXListBox ( commandsPane );
    commandsCombo->setNumVisible( 30 );
    iconsList->setNumVisible( 30 );
    new FXLabel ( commandsPane, "exec: ", NULL, LAYOUT_LEFT );
    commandsTextfield = new FXTextField ( commandsPane, 20 );



    new FXLabel ( commandsPane, "shortuct mask: ", NULL, LAYOUT_LEFT );
    shortcutMaskList = new FXListBox ( commandsPane );
    shortcutMaskList->setNumVisible( 3 );
    shortcutMaskList->appendItem( "" );
    shortcutMaskList->appendItem( "control" );
    shortcutMaskList->appendItem( "alt" );


    new FXLabel ( commandsPane, "shortuct: ", NULL, LAYOUT_LEFT );
    shortcutList = new FXListBox ( commandsPane );
    shortcutList->setNumVisible( 30 );


    map<string, unsigned int>::iterator s_iter;

    shortcutList->appendItem( "" );
    for ( s_iter = objmanager->key_map.begin();s_iter != objmanager->key_map.end();s_iter++ )
    {
        shortcutList->appendItem( s_iter->first.c_str() );
    }

    new FXLabel ( commandsPane, "text: ", NULL, LAYOUT_LEFT );
    commandsTextfieldText = new FXTextField ( commandsPane, 20 );
    commandsType = new FXLabel ( commandsPane, "", NULL, LAYOUT_LEFT );


    vv = new FXGroupBox ( commandsPane, "Options", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0 );
    commandsRescan = new FXCheckButton ( vv, "rescan", NULL, 0, JUSTIFY_LEFT | JUSTIFY_TOP | ICON_BEFORE_TEXT | LAYOUT_SIDE_TOP );
    commandsCapture = new FXCheckButton ( vv, "capture", NULL, 0, JUSTIFY_LEFT | JUSTIFY_TOP | ICON_BEFORE_TEXT | LAYOUT_SIDE_TOP );


    if ( conf->openxpath ( "/OpenspaceConfig/commands" ) != -1 )
    {



        command_container * ctlast;
        string res;
        while ( conf->getnextnode ( res ) )
        {

            command_container ct;
            ctlast = &ct;
            commandsCombo->appendItem( res.c_str (), objmanager->osicons[ "execute" ] );
            ct.name = res;
            configure conflocal = *conf;
            ct.exec = conflocal.readonestring ( "/OpenspaceConfig/commands/" + res + "/exec" );

            string options = conflocal.readonestring ( "/OpenspaceConfig/commands/" + res + "/options" );

            string::size_type pos = options.find ( "capture" );
            if ( pos != string::npos )
                ct.capture = true;
            else
                ct.capture = false;


            pos = options.find ( "rescan" );
            if ( pos != string::npos )
                ct.rescan = true;
            else
                ct.rescan = false;

            ct.text = conflocal.readonestring ( "/OpenspaceConfig/commands/" + res + "/text" );
            ct.icon = conflocal.readonestring ( "/OpenspaceConfig/commands/" + res + "/icon" );
            ct.type = conflocal.readonestring ( "/OpenspaceConfig/commands/" + res + "/type" );
            ct.key = conflocal.readonestring ( "/OpenspaceConfig/commands/" + res + "/key" );
            ct.key_mask = conflocal.readonestring ( "/OpenspaceConfig/commands/" + res + "/key_mask" );

            commandsMap[ ct.name ] = ct;

        }

        commandsRescan->setCheck( ctlast->rescan );
        commandsCapture->setCheck( ctlast->capture );
        commandsTextfield->setText ( ctlast->exec.c_str () );
        commandsCombo->setCurrentItem ( commandsCombo->getNumItems () - 1 );
        currentCommandName = ctlast->name;

        string str;
        if ( ctlast->type == "" )
            str = "command type: EXTERNAL";
        else
            str = "command type: " + ctlast->type;

        commandsType->setText( str.c_str() );
        string shorticonname;



        map < string, FXIcon * >::iterator iter;

        iconsList->appendItem( "", NULL );
        for ( iter = objmanager->osicons.begin (); iter != objmanager->osicons.end (); iter++ )
        {
            FXIcon *icon = iter->second;
            if ( icon != NULL )
                iconsList->appendItem( iter->first.c_str(), icon );
        }
        if ( ctlast->icon != "" )
            iconsList->setCurrentItem( iconsList->findItem( ctlast->icon.c_str() ) );

        if ( ctlast->key != "" )
            shortcutList->setCurrentItem( shortcutList->findItem( ctlast->key.c_str() ) );

        if ( ctlast->key_mask != "" )
            shortcutMaskList->setCurrentItem( shortcutMaskList->findItem( ctlast->key_mask.c_str() ) );



        new FXSeparator( commandsPane );
        FXHorizontalFrame *hfr = new FXHorizontalFrame ( commandsPane, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
        removeCommandButton = new FXButton ( hfr, "Remove", NULL, this, ID_REMOVE_COMMAND, FRAME_RAISED | ICON_ABOVE_TEXT );
        new FXButton ( hfr, "New Command", NULL, this, ID_NEW_COMMAND, FRAME_RAISED | ICON_ABOVE_TEXT );
        newCommandEdit = new FXTextField ( hfr, 20 );

        if ( ctlast->type == "INTERNAL" || ctlast->type == "PLUGIN" )
        {
            commandsTextfield->disable();
            commandsRescan->disable();
            commandsCapture->disable();
            removeCommandButton->disable();
        }

    }


    filetypePane = new FXVerticalFrame ( switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

    new FXButton ( buttons, "File types Settings", NULL, switcher, FXSwitcher::ID_OPEN_FIFTH, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y );



    if ( conf->openxpath ( "/OpenspaceConfig/file_types" ) != -1 )
    {
        filetype_container * ctlast;
        new FXLabel( filetypePane, "File Type:" );
        fileTypeList = new FXListBox ( filetypePane, this, ID_FILETYPE_CHANGE );
        fileTypeList->setNumVisible( 30 );


        FXGroupBox *filetypeGroup = new FXGroupBox( filetypePane, "commands", GROUPBOX_TITLE_CENTER | FRAME_RIDGE | LAYOUT_FILL_X );


        new FXLabel( filetypeGroup, "default command:" );
        fileTypeDefaultBox = new FXListBox ( filetypeGroup );
        fileTypeDefaultBox->setNumVisible( 30 );



        FXHorizontalFrame* hzframe = new FXHorizontalFrame ( filetypeGroup, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
        FXVerticalFrame* vframe0 = new FXVerticalFrame ( hzframe, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
        new FXLabel( vframe0, "additional commands:" );
        additionalCommands = new FXList ( vframe0, NULL, 0, LIST_NORMAL | LAYOUT_FIX_WIDTH, 0, 0, 250 );
        additionalCommands->setNumVisible( 5 );
        FXVerticalFrame* vframe = new FXVerticalFrame ( hzframe, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
        new FXLabel( vframe, "\n" );
        new FXArrowButton( vframe, this, ID_UP_COMMAND, FRAME_RAISED | FRAME_THICK | ARROW_UP );
        new FXArrowButton( vframe, this, ID_DOWN_COMMAND, FRAME_RAISED | FRAME_THICK | ARROW_DOWN );
        new FXArrowButton( vframe, this, ID_ADD_COMMAND_ADDITIONAL, FRAME_RAISED | FRAME_THICK | ARROW_LEFT );
        new FXArrowButton( vframe, this, ID_DEL_COMMAND_ADDITIONAL, FRAME_RAISED | FRAME_THICK | ARROW_RIGHT );
        FXVerticalFrame* vframe1 = new FXVerticalFrame ( hzframe, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
        new FXLabel( vframe1, "all available commands:" );
        additionalCommandsAvailable = new FXList ( vframe1, NULL, 0, LIST_NORMAL | LAYOUT_FIX_WIDTH, 0, 0, 250 );
        additionalCommandsAvailable->setNumVisible( 5 );

        filetypeGroup = new FXGroupBox( filetypePane, "colors and icon", GROUPBOX_TITLE_CENTER | FRAME_RIDGE | LAYOUT_FILL_X );
        FXHorizontalFrame* hf0 = new FXHorizontalFrame ( filetypeGroup, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 10, 10 );

        iconsList2 = new FXListBox ( hf0 );
        iconsList2->setNumVisible( 30 );
        //new FXLabel(hf0,"  ");
        vframe1 = new FXVerticalFrame ( hf0, 0 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
        new FXLabel( vframe1, "text color:" );
        colorbutton = new FXButton( vframe1, "Color", NULL, this, ID_CHOOSE_COLOR );
        vframe1 = new FXVerticalFrame ( hf0, 0 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
        new FXLabel( vframe1, "back color:" );
        backcolorbutton = new FXButton( vframe1, "Back Color", NULL, this, ID_CHOOSE_BACKCOLOR );



        filetypeGroup = new FXGroupBox( filetypePane, "add new file type", GROUPBOX_TITLE_CENTER | FRAME_RIDGE | LAYOUT_FILL_X );

        allMime = new FXComboBox ( filetypeGroup, 60 );
        allMime->setNumVisible( 30 );
        new FXButton ( filetypeGroup, "Add", NULL, this, OSPreferences::ID_ADD_FILETYPE );

        MimeType::__initialize();
        map<string, string>::iterator iter0;
        for ( iter0 = MimeType::mimeMap.begin();iter0 != MimeType::mimeMap.end();iter0++ )
        {
            if ( allMime->findItem( xml2mime( iter0->second ).c_str() ) == -1 )
            {
                allMime->appendItem( xml2mime( iter0->second ).c_str() );
            }
        }




        string res;
        while ( conf->getnextnode ( res ) )
        {


            configure conflocal = *conf;
            if ( conflocal.openxpath ( "/OpenspaceConfig/file_types/" + res + "/types" ) != -1 )
            {
                string res2;
                while ( conflocal.getnextnode ( res2 ) )
                {


                    filetype_container ct;
                    ct.load( res + "/" + res2 );
                    fileTypeList->appendItem( xml2mime( ct.name ).c_str (), objmanager->osicons[ "unknown" ] );


                    filetypesMap[ ct.name ] = ct;

                }

            }
            filetype_container ct;
            ct.load( res );
            ctlast = &ct;
            fileTypeList->appendItem( xml2mime( ct.name ) .c_str (), objmanager->osicons[ "unknown" ] );


            filetypesMap[ ct.name ] = ct;
        }

        filetypestring = ctlast->name;

        map < string, FXIcon * >::iterator iter2;
        iconsList2->appendItem( "", NULL );
        for ( iter2 = objmanager->osicons.begin (); iter2 != objmanager->osicons.end (); iter2++ )
        {
            FXIcon *icon = iter2->second;
            if ( icon != NULL && ( iter2->first.length() < 4 || iter2->first.substr( 0, 4 ) != "big_" ) )
                iconsList2->appendItem( iter2->first.c_str(), icon );
        }
        if ( ctlast->icon != "" )
            iconsList2->setCurrentItem( iconsList2->findItem( ctlast->icon.c_str() ) );



        fileTypeDefaultBox->appendItem ( "", objmanager->osicons[ "execute" ] );

        if ( conf->openxpath ( "/OpenspaceConfig/commands" ) != -1 )
        {
            string command;
            while ( conf->getnextnode ( command ) )
            {

                fileTypeDefaultBox->appendItem ( command.c_str (), objmanager->osicons[ "execute" ] );
                string icon = conf->readonestring ( "/OpenspaceConfig/commands/" + command + "/icon" );
                additionalCommandsAvailable->appendItem ( command.c_str () );
                additionalCommandsAvailableForButtons->appendItem ( command.c_str (), objmanager->osicons[ icon ] );
            }
        }

        vector<string>::iterator iter;
        if ( ctlast->commands.size() > 0 )
            for ( iter = ctlast->commands.begin();iter != ctlast->commands.end();iter++ )
            {
                additionalCommands->appendItem( iter->c_str() );

            }



        fileTypeList->setCurrentItem ( fileTypeList->getNumItems () - 1 );
        fileTypeDefaultBox->setCurrentItem( fileTypeDefaultBox->findItem( ctlast->command.c_str() ) );

        setAllColor( colorbutton, readcolor( ctlast->color ) );
        setAllColor( backcolorbutton, readcolor2( ctlast->backcolor ) );

    }

    //====================================================LOOK AND FEEL============================================================

    new FXButton ( buttons, "Look and feel", NULL, switcher, FXSwitcher::ID_OPEN_SIXTH, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y );

    FXVerticalFrame *vframe = new FXVerticalFrame( switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

    FXHorizontalFrame *hframe = new FXHorizontalFrame( vframe, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    new FXSeparator( vframe, SEPARATOR_GROOVE | LAYOUT_FILL_X );


    frame = new FXVerticalFrame( hframe, LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    new FXSeparator( hframe, SEPARATOR_GROOVE | LAYOUT_FILL_Y );



    FXMatrix* matrix = new FXMatrix( frame, 2, LAYOUT_FILL_Y | MATRIX_BY_COLUMNS, 0, 0, 0, 0, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING, 1, 1 );

    FXColorWell *colorwell = new FXColorWell( matrix, FXRGB( 0, 0, 255 ), &target_base, FXDataTarget::ID_VALUE );
    FXLabel *label = new FXLabel( matrix, "Base Color" );

    colorwell = new FXColorWell( matrix, FXRGB( 0, 0, 255 ), &target_border, FXDataTarget::ID_VALUE );
    label = new FXLabel( matrix, "Border Color" );

    colorwell = new FXColorWell( matrix, FXRGB( 0, 0, 255 ), &target_fore, FXDataTarget::ID_VALUE );
    label = new FXLabel( matrix, "Text Color" );

    colorwell = new FXColorWell( matrix, FXRGB( 0, 0, 255 ), &target_back, FXDataTarget::ID_VALUE );
    label = new FXLabel( matrix, "Background Color" );

    colorwell = new FXColorWell( matrix, FXRGB( 0, 0, 255 ), &target_selfore, FXDataTarget::ID_VALUE );
    label = new FXLabel( matrix, "Selected Text Color" );

    colorwell = new FXColorWell( matrix, FXRGB( 0, 0, 255 ), &target_selback, FXDataTarget::ID_VALUE );
    label = new FXLabel( matrix, "Selected Background Color" );

    colorwell = new FXColorWell( matrix, FXRGB( 0, 0, 255 ), &target_menufore, FXDataTarget::ID_VALUE );
    label = new FXLabel( matrix, "Selected Menu Text Color" );

    colorwell = new FXColorWell( matrix, FXRGB( 0, 0, 255 ), &target_menuback, FXDataTarget::ID_VALUE );
    label = new FXLabel( matrix, "Selected Menu Background Color" );

    colorwell = new FXColorWell( matrix, FXRGB( 0, 0, 255 ), &target_tipfore, FXDataTarget::ID_VALUE );
    label = new FXLabel( matrix, "Tip Text Color" );

    colorwell = new FXColorWell( matrix, FXRGB( 0, 0, 255 ), &target_tipback, FXDataTarget::ID_VALUE );
    label = new FXLabel( matrix, "Tip Background Color" );

    colorwell = new FXColorWell( matrix, FXRGB( 0, 0, 255 ), &target_shadow, FXDataTarget::ID_VALUE );
    label = new FXLabel( matrix, "Shadow Color" );

    colorwell = new FXColorWell( matrix, FXRGB( 0, 0, 255 ), &target_hilite, FXDataTarget::ID_VALUE );
    label = new FXLabel( matrix, "Hilite Color" );

    colorwell = new FXColorWell( matrix, FXRGB( 0, 0, 255 ), &target_main, FXDataTarget::ID_VALUE );
    label = new FXLabel( matrix, "Main Color" );

    frame = new FXVerticalFrame( hframe, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING, 0, 0 );

    tabbook = new FXTabBook( frame, NULL, 0, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0 );
    tabitem = new FXTabItem( tabbook, " Item 1 " );
    tabframe = new FXVerticalFrame( tabbook, LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_THICK | FRAME_RAISED );

    labeltextframe1 = new FXHorizontalFrame( tabframe, LAYOUT_FILL_X );
    label1 = new FXLabel( labeltextframe1, "Label with Text", NULL );
    textfield1 = new FXTextField( labeltextframe1, 30, NULL, 0, LAYOUT_FILL_X | FRAME_THICK | FRAME_SUNKEN );
    textfield1->setText( "Select this text, to see the selected colors" );

    labeltextframe2 = new FXHorizontalFrame( tabframe, LAYOUT_FILL_X );
    textframe1 = new FXHorizontalFrame( labeltextframe2, LAYOUT_FILL_X | FRAME_THICK | FRAME_SUNKEN, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0 );
    label3 = new FXLabel( textframe1, "Selected Text (with focus)", NULL, LAYOUT_FILL_X, 0, 0, 0, 0, 1, 1, 1, 1 );
    textframe2 = new FXHorizontalFrame( labeltextframe2, LAYOUT_FILL_X | FRAME_THICK | FRAME_SUNKEN, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0 );
    label4 = new FXLabel( textframe2, "Selected Text (no focus)", NULL, LAYOUT_FILL_X, 0, 0, 0, 0, 1, 1, 1, 1 );

    sep1 = new FXSeparator( tabframe, LAYOUT_FILL_X | SEPARATOR_LINE );

    tabsubframe = new FXHorizontalFrame( tabframe, LAYOUT_FILL_X | LAYOUT_FILL_Y );

    grpbox1 = new FXGroupBox( tabsubframe, "MenuPane", FRAME_GROOVE | LAYOUT_FILL_Y | LAYOUT_FILL_X );

    menuframe = new FXVerticalFrame( grpbox1, FRAME_RAISED | FRAME_THICK | LAYOUT_CENTER_X | LAYOUT_CENTER_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    menulabels[ 0 ] = new FXLabel( menuframe, "&Open", NULL, LABEL_NORMAL, 0, 0, 0, 0, 16, 4 );
    menulabels[ 1 ] = new FXLabel( menuframe, "S&ave", NULL, LABEL_NORMAL, 0, 0, 0, 0, 16, 4 );
    sep2 = new FXSeparator( menuframe, LAYOUT_FILL_X | SEPARATOR_GROOVE );
    menulabels[ 2 ] = new FXLabel( menuframe, "I&mport", NULL, LABEL_NORMAL, 0, 0, 0, 0, 16, 4 );
    menulabels[ 4 ] = new FXLabel( menuframe, "Selected Menu Entry", NULL, LABEL_NORMAL, 0, 0, 0, 0, 16, 4 );
    menulabels[ 3 ] = new FXLabel( menuframe, "Print", NULL, LABEL_NORMAL, 0, 0, 0, 0, 16, 4 );
    sep3 = new FXSeparator( menuframe, LAYOUT_FILL_X | SEPARATOR_GROOVE );
    menulabels[ 5 ] = new FXLabel( menuframe, "&Quit", NULL, LABEL_NORMAL, 0, 0, 0, 0, 16, 4 );

    grpbox2 = new FXGroupBox( tabsubframe, "Tooltips", FRAME_GROOVE | LAYOUT_FILL_Y | LAYOUT_FILL_X );

    label2 = new FXLabel( grpbox2, "Sample Tooltip", NULL, FRAME_LINE | LAYOUT_CENTER_X );
    label5 = new FXLabel( grpbox2, "Multiline Sample\n Tooltip", NULL, FRAME_LINE | LAYOUT_CENTER_X );

    hframe = new FXHorizontalFrame( vframe, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING );

    new FXLabel( hframe, "Normal Font: ", NULL, LAYOUT_CENTER_Y );
    fontbutton[ 0 ] = new FXButton( hframe, " ", NULL, this, ID_CHOOSE_FONT, LAYOUT_CENTER_Y | FRAME_RAISED | JUSTIFY_CENTER_X | JUSTIFY_CENTER_Y | LAYOUT_FILL_X );

    hframe = new FXHorizontalFrame( vframe, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING );
    new FXLabel( hframe, "Caption1 Font: ", NULL, LAYOUT_CENTER_Y );
    fontbutton[ 1 ] = new FXButton( hframe, " ", NULL, this, ID_CHOOSE_CAPTIONFONT1, LAYOUT_CENTER_Y | FRAME_RAISED | JUSTIFY_CENTER_X | JUSTIFY_CENTER_Y | LAYOUT_FILL_X );

    hframe = new FXHorizontalFrame( vframe, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING );
    new FXLabel( hframe, "Caption2 Font: ", NULL, LAYOUT_CENTER_Y );
    fontbutton[ 2 ] = new FXButton( hframe, " ", NULL, this, ID_CHOOSE_CAPTIONFONT2, LAYOUT_CENTER_Y | FRAME_RAISED | JUSTIFY_CENTER_X | JUSTIFY_CENTER_Y | LAYOUT_FILL_X );

    hframe = new FXHorizontalFrame( vframe, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING );
    new FXLabel( hframe, "Caption3 Font: ", NULL, LAYOUT_CENTER_Y );
    fontbutton[ 3 ] = new FXButton( hframe, " ", NULL, this, ID_CHOOSE_CAPTIONFONT3, LAYOUT_CENTER_Y | FRAME_RAISED | JUSTIFY_CENTER_X | JUSTIFY_CENTER_Y | LAYOUT_FILL_X );

    //====================================================SPEED AND DELAYS============================================================



    new FXButton ( buttons, "Speed and delays", NULL, switcher, FXSwitcher::ID_OPEN_SEVENTH, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y );


    hframe = new FXHorizontalFrame( switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

    matrix = new FXMatrix( hframe, 3, LAYOUT_FILL_Y | MATRIX_BY_COLUMNS, 0, 0, 0, 0, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING );

    const FXuint spinnerstyle = FRAME_SUNKEN | FRAME_THICK;

    new FXLabel( matrix, "Typing Speed\t\tTyping Speed", NULL, LAYOUT_RIGHT | LAYOUT_CENTER_Y );
    FXSpinner *spinner = new FXSpinner( matrix, 4, &target_typingspeed, FXDataTarget::ID_VALUE, spinnerstyle );
    spinner->setRange( 1, 9999 );
    spinner->setIncrement( 500 );
    new FXLabel( matrix, "ms", NULL, LAYOUT_CENTER_Y );


    new FXLabel( matrix, "Double Click Speed\t\tDouble Click Speed", NULL, LAYOUT_RIGHT | LAYOUT_CENTER_Y );
    spinner = new FXSpinner( matrix, 4, &target_clickspeed, FXDataTarget::ID_VALUE, spinnerstyle );
    spinner->setRange( 1, 9999 );
    spinner->setIncrement( 100 );
    new FXLabel( matrix, "ms", NULL, LAYOUT_CENTER_Y );

    new FXLabel( matrix, "Scroll Speed\t\tScroll Speed", NULL, LAYOUT_RIGHT | LAYOUT_CENTER_Y );
    spinner = new FXSpinner( matrix, 4, &target_scrollspeed, FXDataTarget::ID_VALUE, spinnerstyle );
    spinner->setRange( 1, 9999 );
    spinner->setIncrement( 10 );
    new FXLabel( matrix, "ms", NULL, LAYOUT_CENTER_Y );

    new FXLabel( matrix, "Scroll Delay\t\tScroll Delay", NULL, LAYOUT_RIGHT | LAYOUT_CENTER_Y );
    spinner = new FXSpinner( matrix, 4, &target_scrolldelay, FXDataTarget::ID_VALUE, spinnerstyle );
    spinner->setRange( 1, 9999 );
    spinner->setIncrement( 100 );
    new FXLabel( matrix, "ms", NULL, LAYOUT_CENTER_Y );

    new FXLabel( matrix, "Cursor Blink Speed\t\tCursor Blink Speed", NULL, LAYOUT_RIGHT | LAYOUT_CENTER_Y );
    spinner = new FXSpinner( matrix, 4, &target_blinkspeed, FXDataTarget::ID_VALUE, spinnerstyle );
    spinner->setRange( 1, 9999 );
    spinner->setIncrement( 100 );
    new FXLabel( matrix, "ms", NULL, LAYOUT_CENTER_Y );

    new FXLabel( matrix, "Animation Speed\t\tAnimation Speed", NULL, LAYOUT_RIGHT | LAYOUT_CENTER_Y );
    new FXSpinner( matrix, 4, &target_animspeed, FXDataTarget::ID_VALUE, spinnerstyle );
    spinner->setRange( 1, 9999 );
    spinner->setIncrement( 1 );
    new FXLabel( matrix, "ms", NULL, LAYOUT_CENTER_Y );

    new FXLabel( matrix, "Cascade Menu Popup Delay\t\tAmount of delay before cascading menu is shown", NULL, LAYOUT_RIGHT | LAYOUT_CENTER_Y );
    spinner = new FXSpinner( matrix, 4, &target_menupause, FXDataTarget::ID_VALUE, spinnerstyle );
    spinner->setRange( 1, 9999 );
    spinner->setIncrement( 100 );
    new FXLabel( matrix, "ms", NULL, LAYOUT_CENTER_Y );

    new FXLabel( matrix, "Tooltip Popup Delay\t\tAmount of Delay before tooltip is shown ", NULL, LAYOUT_RIGHT | LAYOUT_CENTER_Y );
    spinner = new FXSpinner( matrix, 4, &target_tooltippause, FXDataTarget::ID_VALUE, spinnerstyle );
    spinner->setRange( 1, 9999 );
    spinner->setIncrement( 100 );
    new FXLabel( matrix, "ms", NULL, LAYOUT_CENTER_Y );

    new FXLabel( matrix, "Tooltip Time\t\tTime that tooltips are shown", NULL, LAYOUT_RIGHT | LAYOUT_CENTER_Y );
    spinner = new FXSpinner( matrix, 4, &target_tooltiptime, FXDataTarget::ID_VALUE, spinnerstyle );
    spinner->setRange( 1, 9999 );
    spinner->setIncrement( 500 );
    new FXLabel( matrix, "ms", NULL, LAYOUT_CENTER_Y );

    new FXLabel( matrix, "Drag Delta\t\tMinimum distance considered a mouse move", NULL, LAYOUT_RIGHT | LAYOUT_CENTER_Y );
    spinner = new FXSpinner( matrix, 4, &target_dragdelta, FXDataTarget::ID_VALUE, spinnerstyle );
    spinner->setRange( 1, 9999 );
    spinner->setIncrement( 1 );
    new FXFrame( matrix, FRAME_NONE );

    new FXLabel( matrix, "Wheel Lines", NULL, LAYOUT_RIGHT | LAYOUT_CENTER_Y );
    spinner = new FXSpinner( matrix, 4, &target_wheellines, FXDataTarget::ID_VALUE, spinnerstyle );
    spinner->setRange( 1, 100 );


    //====================================================VFS============================================================


    vfsPane = new FXVerticalFrame ( switcher, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    new FXLabel ( vfsPane, "VFS settings", NULL, LAYOUT_LEFT );

    new FXButton ( buttons, "Virtual File System Settings", NULL, switcher, FXSwitcher::ID_OPEN_EIGHTH, FRAME_RAISED | ICON_ABOVE_TEXT | LAYOUT_FILL_Y );



    vfsList = new FXListBox ( vfsPane, this, ID_VFS_CHANGE );
    vfsList->setNumVisible( 10 );



    FXHorizontalFrame *hzframe = new FXHorizontalFrame ( vfsPane, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    FXVerticalFrame *vframe0 = new FXVerticalFrame ( hzframe, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    new FXLabel( vframe0, "visible headers:" );
    headersList = new FXList ( vframe0, NULL, 0, LIST_NORMAL | LAYOUT_FIX_WIDTH, 0, 0, 250 );
    headersList->setNumVisible( 5 );
    vframe = new FXVerticalFrame ( hzframe, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    new FXLabel( vframe, "\n" );
    new FXArrowButton( vframe, this, ID_UP_VFS_HEADER, FRAME_RAISED | FRAME_THICK | ARROW_UP );
    new FXArrowButton( vframe, this, ID_DOWN_VFS_HEADER, FRAME_RAISED | FRAME_THICK | ARROW_DOWN );
    new FXArrowButton( vframe, this, ID_ADD_HEADER, FRAME_RAISED | FRAME_THICK | ARROW_LEFT );
    new FXArrowButton( vframe, this, ID_DEL_HEADER, FRAME_RAISED | FRAME_THICK | ARROW_RIGHT );



    FXVerticalFrame *vframe1 = new FXVerticalFrame ( hzframe, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    new FXLabel( vframe1, "all available headers:" );
    availableHeadersList = new FXList ( vframe1, NULL, 0, LIST_NORMAL | LAYOUT_FIX_WIDTH, 0, 0, 250 );
    availableHeadersList->setNumVisible( 5 );

    new FXLabel( vfsPane, "" );
    new FXSeparator( vfsPane );
    new FXLabel( vfsPane, "" );

    vv = new FXGroupBox ( vfsPane, "available plugins - download from internet", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0 );

    hfr = new FXHorizontalFrame ( vv, LAYOUT_FILL_X , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    availableVfsPluginsList = new FXListBox ( hfr );
    availableVfsPluginsList->setNumVisible( 30 );
    new FXButton ( hfr, "Download and Install", NULL, this, ID_DOWNLOAD_INSTALL_VFS_PLUGIN, FRAME_RAISED | ICON_ABOVE_TEXT );
    new FXButton ( vv, "Update available plugins list", NULL, this, ID_UPDATE_VFS_PLUGIN_LIST, FRAME_RAISED | ICON_ABOVE_TEXT );



    if ( conf->openxpath ( "/OpenspaceConfig/filelist" ) != -1 )
    {
        string command;
        while ( conf->getnextnode ( command ) )
        {
            vfsList->appendItem( command.c_str() );
        }
    }

    actualvfs = "";

    this->onVfsChange( NULL, 0, NULL );

    setupColors();
    setupFont( 0 );
    setupFont( 1 );
    setupFont( 2 );
    setupFont( 3 );
}

long OSPreferences::onVfsChange ( FXObject * sender, FXSelector sel, void * )
{

    if ( actualvfs != "" )   //not first run, save
    {
        conf->removestring ( "/OpenspaceConfig/filelist/" + actualvfs + "/headers" );
        conf->addstring( "/OpenspaceConfig/filelist/" + actualvfs, "headers", "" );

        for ( int c = 0; c < headersList->getNumItems (); c++ )
        {

            string cmd = headersList->getItemText ( c ).text ();
            conf->addstring( "/OpenspaceConfig/filelist/" + actualvfs + "/headers", "header", cmd );
        }


    }

    actualvfs = vfsList->getItem( vfsList->getCurrentItem() ).text();
    availableHeadersList->clearItems();
    if ( conf->openxpath ( "/OpenspaceConfig/filelist/" + actualvfs + "/properties" ) != -1 )
    {
        string command;
        while ( conf->getnextnode ( command ) )
        {
            if ( command != "name" )
                availableHeadersList->appendItem ( command.c_str () );
        }
    }
    headersList->clearItems();
    if ( conf->openxpath ( "/OpenspaceConfig/filelist/" + actualvfs + "/headers/header" ) != -1 )
    {
        string command;
        while ( conf->getnextstring ( command ) )
        {
            headersList->appendItem ( command.c_str () );
        }
    }



}

long OSPreferences::onAddButtonCommand ( FXObject * sender, FXSelector sel, void * )
{

    FXushort id = FXSELID( sel );
    string actual_toolbar = toolbarList->getItem( toolbarList->getCurrentItem() ).text();

    if ( id == ID_ADD_BUTTON_COMMAND )
    {


        string cmd = additionalCommandsAvailableForButtons->getItemText( additionalCommandsAvailableForButtons->getCurrentItem() ).text();
        toolbarVector.push_back( toolbar_container( actual_toolbar, cmd ) );
        buttonsList->appendItem( cmd.c_str() );
    }
    else
    {

        string cmd = buttonsList->getItemText( buttonsList->getCurrentItem() ).text();
        buttonsList->removeItem( buttonsList->getCurrentItem() );

        vector <toolbar_container>::iterator iter;

        for ( iter = toolbarVector.begin();iter != toolbarVector.end(); iter++ )
        {
            if ( iter->command == cmd && iter->toolbar == actual_toolbar )
            {
                toolbarVector.erase( iter );
                break;
            }
        }

    }


}

long OSPreferences::onAddHeader ( FXObject * sender, FXSelector sel, void * )
{

    FXushort id = FXSELID( sel );

    if ( id == ID_ADD_HEADER )
    {
        headersList->appendItem( availableHeadersList->getItemText( availableHeadersList->getCurrentItem() ) );
    }
    else
    {
        headersList->removeItem( headersList->getCurrentItem() );
    }



}

long OSPreferences::onAddShutterCommand ( FXObject * sender, FXSelector sel, void * )
{

    FXushort id = FXSELID( sel );
    string actual_shutter = shutterList->getItem( shutterList->getCurrentItem() ).text();

    if ( id == ID_ADD_SHUTTER_COMMAND )
    {


        string cmd = additionalCommandsAvailableForButtons->getItemText( additionalCommandsAvailableForButtons->getCurrentItem() ).text();
        if ( cmd != "SEPARATOR" )
        {
            shutterVector.push_back( shutter_container( actual_shutter, cmd ) );
            shutterCommands->appendItem( cmd.c_str() );
        }
    }
    else
    {

        string cmd = shutterCommands->getItemText( shutterCommands->getCurrentItem() ).text();
        shutterCommands->removeItem( shutterCommands->getCurrentItem() );

        vector <shutter_container>::iterator iter;

        for ( iter = shutterVector.begin();iter != shutterVector.end(); iter++ )
        {
            if ( iter->command == cmd && iter->shutter == actual_shutter )
            {
                shutterVector.erase( iter );
                break;
            }
        }

    }



}


void OSPreferences::setAllColor( FXButton* button, FXColor color )
{
    button->setBaseColor( color );
    button->setBorderColor( color );
    button->setShadowColor( color );
    button->setHiliteColor( color );
    button->setTextColor( color );
    button->setBackColor( color );
}

long OSPreferences::onChooseColor ( FXObject * sender, FXSelector sel, void * )
{

    if ( !colordlg->execute() )
        return 0;


    FXColor color = colordlg->getRGBA();
    FXushort id = FXSELID( sel );
    if ( id == ID_CHOOSE_COLOR )
        setAllColor( colorbutton, color );
    else
        setAllColor( backcolorbutton, color );


}

long OSPreferences::onAddFiletype ( FXObject * sender, FXSelector sel, void * )
{
    string mime = allMime->getText().text();

    filetype_container ct = filetypesMap[ mime ];
    if ( ct.name != "" )    // already exists
        return 0;

    ct.name = mime;
    filetypesMap[ mime ] = ct;

    fileTypeList->appendItem( ct.name.c_str (), objmanager->osicons[ "unknown" ] );
    this->onFileTypeChange( NULL, 0, NULL );
}

OSPreferences::~OSPreferences ()
{}

long OSPreferences::save ( void )
{
    FXTRACE ( ( 5, "save\n" ) );

    this->onCommandChange( NULL, 0, NULL );
    this->onFileTypeChange( NULL, 0, NULL );
    this->onVfsChange( NULL, 0, NULL );

    map < string, command_container >::iterator iter;

    for ( iter = commandsMap.begin (); iter != commandsMap.end (); iter++ )
    {
        command_container ct = iter->second;
        ct.save();
    }

    map < string, filetype_container >::iterator iter2;

    for ( iter2 = filetypesMap.begin (); iter2 != filetypesMap.end (); iter2++ )
    {
        filetype_container ct = iter2->second;
        ct.save();
    }



    conf->removestring ( "/OpenspaceConfig/button_commands" );
    conf->addstring( "/OpenspaceConfig", "button_commands", "" );

    for ( int c = 0; c < buttonsList->getNumItems (); c++ )
    {
        string cmd = buttonsList->getItemText ( c ).text ();
        conf->addstring( "/OpenspaceConfig/button_commands", "command", cmd );
    }


    conf->removestring ( "/OpenspaceConfig/shutter" );
    conf->addstring( "/OpenspaceConfig", "shutter", "" );

    vector <shutter_container>::iterator shutter_iter;

    for ( shutter_iter = shutterVector.begin();shutter_iter != shutterVector.end(); shutter_iter++ )
    {
        string res;
        if ( !conf->readonestring( "/OpenspaceConfig/shutter/" + shutter_iter->shutter, res ) )
            conf->addstring( "/OpenspaceConfig/shutter", shutter_iter->shutter, "" );

        conf->addstring( "/OpenspaceConfig/shutter/" + shutter_iter->shutter, "command", shutter_iter->command );

    }

    conf->removestring ( "/OpenspaceConfig/toolbars" );
    conf->addstring( "/OpenspaceConfig", "toolbars", "" );

    vector <toolbar_container>::iterator toolbar_iter;

    for ( toolbar_iter = toolbarVector.begin();toolbar_iter != toolbarVector.end(); toolbar_iter++ )
    {
        string res;
        if ( !conf->readonestring( "/OpenspaceConfig/toolbars/" + toolbar_iter->toolbar, res ) )
            conf->addstring( "/OpenspaceConfig/toolbars", toolbar_iter->toolbar, "" );

        conf->addstring( "/OpenspaceConfig/toolbars/" + toolbar_iter->toolbar, "command", toolbar_iter->command );

    }

    conf->saveonestring ( "/OpenspaceConfig/mainwindow/width", ntos( mainwindow_width->getValue() ) );
    conf->saveonestring ( "/OpenspaceConfig/mainwindow/height", ntos( mainwindow_height->getValue() ) );

    conf->saveonestring ( "/OpenspaceConfig/leftdir/dir", leftdir->getText().text() );
    conf->saveonestring ( "/OpenspaceConfig/rightdir/dir", rightdir->getText().text() );
    conf->saveonestring ( "/OpenspaceConfig/defaultdir/dir", defaultdir->getText().text() );
    conf->saveonestring ( "/OpenspaceConfig/filelist/local/thumbs/size", ntos( thumbsize->getValue() ) );
    conf->saveonestring ( "/OpenspaceConfig/icons_theme", iconsTheme->getItem( iconsTheme->getCurrentItem() ).text() );


    conf->saveonestring ( "/OpenspaceConfig/speed_delay/typing_speed", ntos( typingSpeed ) );
    conf->saveonestring ( "/OpenspaceConfig/speed_delay/click_speed", ntos( clickSpeed ) );
    conf->saveonestring ( "/OpenspaceConfig/speed_delay/scroll_speed", ntos( scrollSpeed ) );
    conf->saveonestring ( "/OpenspaceConfig/speed_delay/scroll_delay", ntos( scrollDelay ) );
    conf->saveonestring ( "/OpenspaceConfig/speed_delay/blink_speed", ntos( blinkSpeed ) );
    conf->saveonestring ( "/OpenspaceConfig/speed_delay/anim_speed", ntos( animSpeed ) );
    conf->saveonestring ( "/OpenspaceConfig/speed_delay/menu_pause", ntos( menuPause ) );
    conf->saveonestring ( "/OpenspaceConfig/speed_delay/tooltip_pause", ntos( tooltipPause ) );
    conf->saveonestring ( "/OpenspaceConfig/speed_delay/tooltip_time", ntos( tooltipTime ) );
    conf->saveonestring ( "/OpenspaceConfig/speed_delay/drag_delta", ntos( dragDelta ) );
    conf->saveonestring ( "/OpenspaceConfig/speed_delay/wheel_lines", ntos( wheelLines ) );

    FXchar buffer[ 60 ];
    conf->saveonestring ( "/OpenspaceConfig/colors/base", fxnamefromcolor( buffer, base ) );
    conf->saveonestring ( "/OpenspaceConfig/colors/fore", fxnamefromcolor( buffer, fore ) );
    conf->saveonestring ( "/OpenspaceConfig/colors/hilite", fxnamefromcolor( buffer, hilite ) );
    conf->saveonestring ( "/OpenspaceConfig/colors/shadow", fxnamefromcolor( buffer, shadow ) );
    conf->saveonestring ( "/OpenspaceConfig/colors/selfore", fxnamefromcolor( buffer, selfore ) );
    conf->saveonestring ( "/OpenspaceConfig/colors/selback", fxnamefromcolor( buffer, selback ) );
    conf->saveonestring ( "/OpenspaceConfig/colors/tipfore", fxnamefromcolor( buffer, tipfore ) );
    conf->saveonestring ( "/OpenspaceConfig/colors/tipback", fxnamefromcolor( buffer, tipback ) );
    conf->saveonestring ( "/OpenspaceConfig/colors/selmenutext", fxnamefromcolor( buffer, menufore ) );
    conf->saveonestring ( "/OpenspaceConfig/colors/selmenuback", fxnamefromcolor( buffer, menuback ) );
    conf->saveonestring ( "/OpenspaceConfig/colors/main", fxnamefromcolor( buffer, main ) );

    FXString fontspec = font[ 0 ] ->getFont();
    conf->saveonestring ( "/OpenspaceConfig/fonts/normalfont", fontspec.text() );

    fontspec = font[ 1 ] ->getFont();
    conf->saveonestring ( "/OpenspaceConfig/fonts/captionfont1", fontspec.text() );

    fontspec = font[ 2 ] ->getFont();
    conf->saveonestring ( "/OpenspaceConfig/fonts/captionfont2", fontspec.text() );

    fontspec = font[ 3 ] ->getFont();
    conf->saveonestring ( "/OpenspaceConfig/fonts/captionfont3", fontspec.text() );


    conf->saveonestring ( "/OpenspaceConfig/position", placement->getItem ( placement->getCurrentItem() ).text() );

}

long OSPreferences::onCommandChange ( FXObject * sender, FXSelector sel, void * )
{

    command_container * ct_prev = &commandsMap[ currentCommandName ];
    ct_prev->exec = commandsTextfield->getText().text();
    ct_prev->capture = commandsCapture->getCheck();
    ct_prev->rescan = commandsRescan->getCheck();
    ct_prev->text = commandsTextfieldText->getText().text();
    ct_prev->icon = iconsList->getItem( iconsList->getCurrentItem() ).text();
    ct_prev->key = shortcutList->getItem( shortcutList->getCurrentItem() ).text();
    ct_prev->key_mask = shortcutMaskList->getItem( shortcutMaskList->getCurrentItem() ).text();

    command_container ct = commandsMap[ commandsCombo->getItem ( commandsCombo->getCurrentItem () ).text () ];

    commandsRescan->setCheck( ct.rescan );
    commandsCapture->setCheck( ct.capture );
    commandsTextfield->setText ( ct.exec.c_str () );
    commandsTextfieldText->setText ( ct.text.c_str () );

    string str;
    if ( ct.type == "" )
        str = "command type: EXTERNAL";
    else
        str = "command type: " + ct.type;

    commandsType->setText( str.c_str() );


    if ( ct.type == "INTERNAL" || ct.type == "PLUGIN" )
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
    iconsList->setCurrentItem( iconsList->findItem( ct.icon.c_str() ) );
    shortcutList->setCurrentItem( shortcutList->findItem( ct.key.c_str() ) );
    shortcutMaskList->setCurrentItem( shortcutMaskList->findItem( ct.key_mask.c_str() ) );


    currentCommandName = ct.name;

}


long OSPreferences::onFileTypeChange ( FXObject * sender, FXSelector sel, void * )
{

    filetype_container * ct_prev = &filetypesMap[ currentFileType ];
    ct_prev->command = fileTypeDefaultBox->getItem( fileTypeDefaultBox->getCurrentItem() ).text();

    ct_prev->commands.clear();
    for ( int i = 0;i < additionalCommands->getNumItems ();i++ )
    {
        ct_prev->commands.push_back( additionalCommands->getItemText( i ).text() );
    }

    FXchar buffer[ 60 ];
    fxnamefromcolor( buffer, colorbutton->getBackColor() );

    ct_prev->color = buffer;

    fxnamefromcolor( buffer, backcolorbutton->getBackColor() );

    ct_prev->backcolor = buffer;
    ct_prev->icon = iconsList2->getItem( iconsList2->getCurrentItem() ).text();

    filetype_container ct = filetypesMap[ mime2xml( fileTypeList->getItem ( fileTypeList->getCurrentItem () ).text () ) ];
    fileTypeDefaultBox->setCurrentItem( fileTypeDefaultBox->findItem( ct.command.c_str() ) );
    additionalCommands->clearItems();

    iconsList2->setCurrentItem( iconsList2->findItem( ct.icon.c_str() ) );

    vector<string>::iterator iter;
    for ( iter = ct.commands.begin();iter != ct.commands.end();iter++ )
    {
        additionalCommands->appendItem( iter->c_str() );
    }

    setAllColor( colorbutton, readcolor( ct.color ) );
    setAllColor( backcolorbutton, readcolor2( ct.backcolor ) );


    currentFileType = ct.name;

}

long OSPreferences::onShutterChange ( FXObject * sender, FXSelector sel, void * )
{
    string actual_shutter = shutterList->getItem( shutterList->getCurrentItem() ).text();
    shutterCommands->clearItems();
    vector <shutter_container>::iterator iter;
    for ( iter = shutterVector.begin();iter != shutterVector.end();iter++ )
    {
        if ( iter->shutter == actual_shutter )
            shutterCommands->appendItem( iter->command.c_str() );
    }

}

long OSPreferences::onToolbarChange ( FXObject * sender, FXSelector sel, void * )
{
    string actual_toolbar = toolbarList->getItem( toolbarList->getCurrentItem() ).text();
    buttonsList->clearItems();
    vector <toolbar_container>::iterator iter;
    for ( iter = toolbarVector.begin();iter != toolbarVector.end();iter++ )
    {
        if ( iter->toolbar == actual_toolbar )
        {
            string icon = conf->readonestring ( "/OpenspaceConfig/commands/" + iter->command + "/icon" );
            buttonsList->appendItem( iter->command.c_str(), objmanager->osicons[ icon ] );
        }
    }

}


bool OSPreferences::validateName( string name )
{
    if ( name == "" ) return false;

    int p, k;
    FXRex identifier( "[0-9a-zA-Z]*" );
    identifier.match( name.c_str(), name.size(), &p, &k );


    if ( k != name.size() )
    {
        FXMessageBox error ( this, "error", "Incorrect characters in name", NULL, MBOX_OK | DECOR_TITLE | DECOR_BORDER );
        error.execute();
        return false;
    }
    else
    {
        return true;
    }


}

long OSPreferences::onNewShutter ( FXObject * sender, FXSelector sel, void * )
{
    string shutter_name = newShutterEdit->getText ().text ();
    if ( !validateName( shutter_name ) )
        return 0;

    shutterList->appendItem( shutter_name.c_str() );

}

long OSPreferences::onRemoveShutter ( FXObject * sender, FXSelector sel, void * )
{

    string shutter = shutterList->getItem ( shutterList->getCurrentItem () ).text ();
    int cur = shutterList->getCurrentItem ();
    shutterList->setCurrentItem ( 0 );
    shutterList->removeItem ( cur );
    this->onShutterChange( NULL, 0, NULL );


    string res = "/OpenspaceConfig/shutter/" + shutter;
    conf->removestring ( res );


    vector <shutter_container>::iterator shutter_iter;

    for ( shutter_iter = shutterVector.begin();shutter_iter != shutterVector.end(); shutter_iter++ )
    {
        if ( shutter_iter->shutter == shutter )
        {
            shutterVector.erase( shutter_iter );
            break;
        }
    }


}


long OSPreferences::onNewCommand ( FXObject * sender, FXSelector sel, void * )
{

    string command_name = newCommandEdit->getText ().text ();
    if ( !validateName( command_name ) )
        return 0;

    command_container ct = commandsMap[ command_name ];
    if ( ct.name != "" )    // already exists
        return 0;

    ct.name = command_name;
    ct.rescan = false;
    ct.capture = false;
    commandsCombo->appendItem( ct.name.c_str() );
    commandsMap[ command_name ] = ct;
    commandsCombo->setCurrentItem ( commandsCombo->getNumItems () - 1 );
    this->onCommandChange( NULL, 0, NULL );

    fileTypeDefaultBox->appendItem( ct.name.c_str() );
    additionalCommandsAvailable->appendItem( ct.name.c_str() );
    additionalCommandsAvailableForButtons->appendItem( ct.name.c_str() );


}

long OSPreferences::onRemoveCommand ( FXObject * sender, FXSelector sel, void * )
{

    string command = commandsCombo->getItem ( commandsCombo->getCurrentItem () ).text ();
    int cur = commandsCombo->getCurrentItem ();
    commandsCombo->setCurrentItem ( 0 );
    commandsCombo->removeItem ( cur );
    this->onCommandChange( NULL, 0, NULL );


    string res = "/OpenspaceConfig/commands/" + command;
    conf->removestring ( res );


}

long OSPreferences:: onAdditionalCommandChange( FXObject * sender, FXSelector sel, void * )
{
    FXushort id = FXSELID( sel );
    filetype_container *ct_prev = &filetypesMap[ currentFileType ];
    if ( id == ID_ADD_COMMAND_ADDITIONAL )
    {
        ct_prev->commands.push_back( additionalCommandsAvailable->getItemText( additionalCommandsAvailable->getCurrentItem() ).text() );
        additionalCommands->appendItem( additionalCommandsAvailable->getItemText( additionalCommandsAvailable->getCurrentItem() ) );
    }
    else
    {
        vector <string>::iterator iter = find( ct_prev->commands.begin(), ct_prev->commands.end(), additionalCommands->getItemText( additionalCommands->getCurrentItem() ).text() );
        if ( iter != ct_prev->commands.end() )
        {
            ct_prev->commands.erase( iter );
        }
        additionalCommands->removeItem( additionalCommands->getCurrentItem() );





    }

}

long OSPreferences::onOpenMimeApp ( FXObject * sender, FXSelector sel, void * )
{
    if ( mimeapp == NULL )
    {
        mimeapp = new MimeApp( this );


        FXushort id = FXSELID( sel );

        if ( id == ID_MIME_APP_AUTO )
        {
            mimeapp->doAutomaticConfiguration();
        }
        else
        {
            mimeapp->create();
            mimeapp->execute ( PLACEMENT_OWNER );
        }


        saveconfiguration = false;
        this->handle ( this, FXSEL ( SEL_CLOSE, 0 ), NULL );
    }



}

long OSPreferences::onColorChanged( FXObject*, FXSelector, void* )
{
    setupColors();
    return 1;
}


void OSPreferences::setupColors()
{
    //shadow = makeShadowColor(base);
    //hilite = makeHiliteColor(base);

    tabitem->setBorderColor( border );
    tabitem->setBaseColor( base );
    tabitem->setBackColor( base );
    tabitem->setTextColor( fore );
    tabitem->setShadowColor( shadow );
    tabitem->setHiliteColor( hilite );

    tabframe->setBorderColor( border );
    tabframe->setBaseColor( base );
    tabframe->setBackColor( base );
    tabframe->setShadowColor( shadow );
    tabframe->setHiliteColor( hilite );


    tabsubframe->setBorderColor( border );
    tabsubframe->setBaseColor( base );
    tabsubframe->setBackColor( base );
    tabsubframe->setShadowColor( shadow );
    tabsubframe->setHiliteColor( hilite );

    menuframe->setBorderColor( border );
    menuframe->setBaseColor( base );
    menuframe->setBackColor( base );
    menuframe->setShadowColor( shadow );
    menuframe->setHiliteColor( hilite );

    grpbox1->setBorderColor( border );
    grpbox1->setBaseColor( base );
    grpbox1->setBackColor( base );
    grpbox1->setShadowColor( shadow );
    grpbox1->setHiliteColor( hilite );
    grpbox1->setTextColor( fore );

    grpbox2->setBorderColor( border );
    grpbox2->setBaseColor( base );
    grpbox2->setBackColor( base );
    grpbox2->setShadowColor( shadow );
    grpbox2->setHiliteColor( hilite );
    grpbox2->setTextColor( fore );

    sep1->setBorderColor( border );
    sep1->setBaseColor( base );
    sep1->setBackColor( base );
    sep1->setShadowColor( shadow );
    sep1->setHiliteColor( hilite );

    sep2->setBorderColor( border );
    sep2->setBaseColor( base );
    sep2->setBackColor( base );
    sep2->setShadowColor( shadow );
    sep2->setHiliteColor( hilite );

    sep3->setBorderColor( border );
    sep3->setBaseColor( base );
    sep3->setBackColor( base );
    sep3->setShadowColor( shadow );
    sep3->setHiliteColor( hilite );

    labeltextframe1->setBorderColor( border );
    labeltextframe1->setBaseColor( base );
    labeltextframe1->setBackColor( base );
    labeltextframe1->setShadowColor( shadow );
    labeltextframe1->setHiliteColor( hilite );


    labeltextframe2->setBorderColor( border );
    labeltextframe2->setBaseColor( base );
    labeltextframe2->setBackColor( base );
    labeltextframe2->setShadowColor( shadow );
    labeltextframe2->setHiliteColor( hilite );

    label1->setBorderColor( border );
    label1->setBaseColor( base );
    label1->setBackColor( base );
    label1->setTextColor( fore );
    label1->setShadowColor( shadow );
    label1->setHiliteColor( hilite );

    label2->setBorderColor( tipfore );
    label2->setBaseColor( tipback );
    label2->setBackColor( tipback );
    label2->setTextColor( tipfore );
    label2->setShadowColor( shadow );
    label2->setHiliteColor( hilite );

    label3->setBorderColor( border );
    label3->setBaseColor( base );
    label3->setBackColor( selback );
    label3->setTextColor( selfore );
    label3->setShadowColor( shadow );
    label3->setHiliteColor( hilite );

    label4->setBorderColor( border );
    label4->setBaseColor( base );
    label4->setBackColor( base );
    label4->setTextColor( fore );
    label4->setShadowColor( shadow );
    label4->setHiliteColor( hilite );

    label5->setBorderColor( tipfore );
    label5->setBaseColor( tipback );
    label5->setBackColor( tipback );
    label5->setTextColor( tipfore );
    label5->setShadowColor( shadow );
    label5->setHiliteColor( hilite );

    for ( int i = 0;i < 6;i++ )
    {
        menulabels[ i ] ->setBorderColor( border );
        menulabels[ i ] ->setBaseColor( base );
        menulabels[ i ] ->setBackColor( base );
        menulabels[ i ] ->setTextColor( fore );
        menulabels[ i ] ->setShadowColor( shadow );
        menulabels[ i ] ->setHiliteColor( hilite );
    }

    menulabels[ 4 ] ->setBorderColor( border );
    menulabels[ 4 ] ->setBaseColor( menuback );
    menulabels[ 4 ] ->setBackColor( menuback );
    menulabels[ 4 ] ->setTextColor( menufore );
    menulabels[ 4 ] ->setShadowColor( shadow );
    menulabels[ 4 ] ->setHiliteColor( hilite );

    textframe1->setBorderColor( border );
    textframe1->setBaseColor( base );
    textframe1->setBackColor( back );
    textframe1->setShadowColor( shadow );
    textframe1->setHiliteColor( hilite );

    textframe2->setBorderColor( border );
    textframe2->setBaseColor( base );
    textframe2->setBackColor( back );
    textframe2->setShadowColor( shadow );
    textframe2->setHiliteColor( hilite );

    textfield1->setBorderColor( border );
    textfield1->setBackColor( back );
    textfield1->setBaseColor( base );
    textfield1->setTextColor( fore );
    textfield1->setSelTextColor( selfore );
    textfield1->setSelBackColor( selback );
    textfield1->setCursorColor( fore );
    textfield1->setShadowColor( shadow );
    textfield1->setHiliteColor( hilite );

    tooltip->setTextColor( tipfore );
    tooltip->setBackColor( tipback );
}

static FXString weightToString( FXuint weight )
{
    switch ( weight )
    {
            case FONTWEIGHT_THIN : return "thin"; break;
            case FONTWEIGHT_EXTRALIGHT: return "extralight"; break;
            case FONTWEIGHT_LIGHT : return "light"; break;
            case FONTWEIGHT_NORMAL : return "normal"; break;
            case FONTWEIGHT_MEDIUM : return "medium"; break;
            case FONTWEIGHT_DEMIBOLD : return "demibold"; break;
            case FONTWEIGHT_BOLD : return "bold"; break;
            case FONTWEIGHT_EXTRABOLD : return "extrabold"; break;
            case FONTWEIGHT_HEAVY : return "heavy"; break;
            default: return ""; break;
    }
    return "";
}

static FXString slantToString( FXuint slant )
{
    switch ( slant )
    {
            case FONTSLANT_REGULAR : return "regular"; break;
            case FONTSLANT_ITALIC : return "italic"; break;
            case FONTSLANT_OBLIQUE : return "oblique"; break;
            case FONTSLANT_REVERSE_ITALIC : return "reverse italic"; break;
            case FONTSLANT_REVERSE_OBLIQUE : return "reverse oblique"; break;
            default : return ""; break;
    }
    return "";
}

void OSPreferences::setupFont( int nr )
{



    FXString fontname = font[ nr ] ->getActualName() + ", " + FXStringVal( font[ nr ] ->getSize() / 10 );
    if ( font[ nr ] ->getWeight() != 0 && font[ nr ] ->getWeight() != FONTWEIGHT_NORMAL )
    {
        fontname += ", " + weightToString( font[ nr ] ->getWeight() );
    }
    if ( font[ nr ] ->getSlant() != 0 && font[ nr ] ->getSlant() != FONTSLANT_REGULAR )
    {
        fontname += ", " + slantToString( font[ nr ] ->getSlant() );
    }

    if ( nr == 0 )
    {

        tabitem->setFont( font[ nr ] );
        label1->setFont( font[ nr ] );
        label2->setFont( font[ nr ] );
        label3->setFont( font[ nr ] );
        label4->setFont( font[ nr ] );
        label5->setFont( font[ nr ] );

        menulabels[ 0 ] ->setFont( font[ nr ] );
        menulabels[ 1 ] ->setFont( font[ nr ] );
        menulabels[ 2 ] ->setFont( font[ nr ] );
        menulabels[ 3 ] ->setFont( font[ nr ] );
        menulabels[ 4 ] ->setFont( font[ nr ] );
        menulabels[ 5 ] ->setFont( font[ nr ] );

        textfield1->setFont( font[ nr ] );

    }


    fontbutton[ nr ] ->setText( fontname );


}

long OSPreferences::onChooseFont( FXObject*, FXSelector sel, void* )
{

    FXushort id = FXSELID( sel );
    int nr = id - ID_CHOOSE_FONT;


    FXFontDialog dialog( this, "Select Font" );
    FXFontDesc fontdescription;
    font[ nr ] ->getFontDesc( fontdescription );
    strncpy( fontdescription.face, font[ nr ] ->getActualName().text(), sizeof( fontdescription.face ) );
    dialog.setFontSelection( fontdescription );
    if ( dialog.execute( PLACEMENT_SCREEN ) )
    {
        FXFont * oldfont = font[ nr ];
        dialog.getFontSelection( fontdescription );


        font[ nr ] = new FXFont( getApp(), fontdescription );
        font[ nr ] ->create();

        setupFont( nr );
        delete oldfont;

    }
    return 1;
}

long OSPreferences::close ( FXObject * sender, FXSelector sel, void *ptr )
{

    if ( saveconfiguration )
        this->save();

    FXMessageBox about ( this, "restart", "restart openspace to apply changes", NULL, MBOX_OK | DECOR_ALL );
    about.execute ();

    FXDialogBox::onCmdAccept( sender, sel, ptr );

}


long OSPreferences::downloadInstallPlugin ( FXObject * sender, FXSelector sel, void * )
{
    FXushort id = FXSELID( sel );
    string file;

    if ( id == ID_DOWNLOAD_INSTALL_CMD_PLUGIN )
        file = string( FXFile::getUserDirectory ( "" ).text () ) + "/.openspace/plugins/cmddialog/commandPluginsList.txt";
    else
        file = string( FXFile::getUserDirectory ( "" ).text () ) + "/.openspace/plugins/filelist/vfsPluginsList.txt";

    if ( FXFile::exists( file.c_str() ) )
    {

        std::string line;
        std::ifstream infile ( file.c_str() );

        while ( std::getline ( infile, line ) )
        {
            string name;
            string download;
            std::stringstream parser ( line );
            parser >> name;
            parser >> download;

            if ( id == ID_DOWNLOAD_INSTALL_CMD_PLUGIN )
                if ( name == availableCommandPluginsList->getItem( availableCommandPluginsList->getCurrentItem() ).text() )
                {
                    if ( name == "" )
                        return 0;

                    string cmd = "cd " + string( FXFile::getUserDirectory ( "" ).text () ) + "/.openspace/plugins/cmddialog/ && wget -N --connect-timeout=5  " + download;
                    system( cmd.c_str() );


                    command_container ct = commandsMap[ name ];
                    if ( ct.name != "" )    // already exists
                        return 0;

                    ct.name = name;
                    ct.rescan = false;
                    ct.capture = false;
                    ct.type = "PLUGIN";
                    commandsCombo->appendItem( ct.name.c_str() );
                    commandsMap[ name ] = ct;
                    commandsCombo->setCurrentItem ( commandsCombo->getNumItems () - 1 );
                    this->onCommandChange( NULL, 0, NULL );

                    fileTypeDefaultBox->appendItem( ct.name.c_str() );
                    additionalCommands->appendItem( ct.name.c_str() );

                    break;
                }
                else
                    if ( name == availableVfsPluginsList->getItem( availableVfsPluginsList->getCurrentItem() ).text() )
                    {
                        if ( name == "" )
                            return 0;

                        string cmd = "cd " + string( FXFile::getUserDirectory ( "" ).text () ) + "/.openspace/plugins/filelist/ && wget -N --connect-timeout=5  " + download;
                        system( cmd.c_str() );

                        string plugin_path = FXFile::getUserDirectory ( "" ).text () + string( "/.openspace/plugins/filelist/libfilelist" ) + name + ".so";


                        void *dllhandle = fxdllOpen ( plugin_path.c_str () );
                        if ( dllhandle )
                        {

                            filelist_base * ( *gg ) ( void );
                            gg = ( filelist_base * ( * ) ( void ) ) fxdllSymbol ( dllhandle, "get_filelist" );
                            filelist_base *fb = gg ();

                            vfs v = fb->setup();

                            conf->removestring ( "/OpenspaceConfig/filelist/" + name );
                            conf->addstring( "/OpenspaceConfig/filelist", name, "" );
                            conf->addstring( "/OpenspaceConfig/filelist/" + name, "type", v.type );
                            conf->addstring( "/OpenspaceConfig/filelist/" + name, "properties", "" );

                            vector <vfsheader_container>::iterator iter;

                            for ( iter = v.vfsheaders.begin();iter != v.vfsheaders.end();iter++ )
                            {
                                conf->addstring( "/OpenspaceConfig/filelist/" + name + "/properties", iter->name, "" );
                                conf->addstring( "/OpenspaceConfig/filelist/" + name + "/properties/" + iter->name, "width", iter->width );
                                conf->addstring( "/OpenspaceConfig/filelist/" + name + "/properties/" + iter->name, "type", iter->type );
                            }
                            vfsList->appendItem( name.c_str() );
                        }
                    }
        }
    }

}

long OSPreferences::updatePluginList ( FXObject * sender, FXSelector sel, void * )
{
    string cmd;

    FXushort id = FXSELID( sel );

    if ( id == ID_UPDATE_CMD_PLUGIN_LIST )
        cmd = "cd " + string( FXFile::getUserDirectory ( "" ).text () ) + "/.openspace/plugins/cmddialog/ && wget -N http://openspace.linux.pl/files/" + string( PACKAGE_VERSION ) + "/x86/commandPluginsList.txt";
    else
        cmd = "cd " + string( FXFile::getUserDirectory ( "" ).text () ) + "/.openspace/plugins/filelist/ && wget -N http://openspace.linux.pl/files/" + string( PACKAGE_VERSION ) + "/x86/vfsPluginsList.txt";


    system( cmd.c_str() );
    string file;
    if ( id == ID_UPDATE_CMD_PLUGIN_LIST )
        file = string( FXFile::getUserDirectory ( "" ).text () ) + "/.openspace/plugins/cmddialog/commandPluginsList.txt";
    else
        file = string( FXFile::getUserDirectory ( "" ).text () ) + "/.openspace/plugins/filelist/vfsPluginsList.txt";


    if ( FXFile::exists( file.c_str() ) )
    {
        std::string line;
        std::ifstream infile ( file.c_str() );

        while ( std::getline ( infile, line ) )
        {
            string name;
            string download;
            std::stringstream parser ( line );
            parser >> name;
            parser >> download;
            if ( id == ID_UPDATE_CMD_PLUGIN_LIST )
            {
                availableCommandPluginsList->clearItems();
                availableCommandPluginsList->appendItem( name.c_str() );
            }
            else
            {
                availableVfsPluginsList->clearItems();
                availableVfsPluginsList->appendItem( name.c_str() );
            }
        }
    }

}