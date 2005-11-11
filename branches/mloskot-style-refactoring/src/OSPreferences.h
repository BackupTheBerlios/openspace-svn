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
// C++ Interface: OSPreferences
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSPREFERENCES_H__
#define __OSPREFERENCES_H__

#include "OSObjectManager.h"
#include "OSUtils.h"
#include "OSMimeApp.h"

#include "OSFileTypeInfo.h"
#include "OSCommandTypeInfo.h"

#include <vector>
#include <string>
#include <map>

class OSCommandTypeInfo;

class shutter_container
{
public:
    std::string shutter;
    std::string command;
    shutter_container( std::string s, std::string c ) : shutter( s ), command( c ) {}}
;

class toolbar_container
{
public:
    std::string toolbar;
    std::string command;
    toolbar_container( std::string s, std::string c ) : toolbar( s ), command( c ) {}}
;

//OSPreferences window, this is one of the most important things to be extended
/**
 * Preferences editiing window class.
 * This is one of the most important thing to be extended.
 */
class OSPreferences : public FXDialogBox
{

FXDECLARE ( OSPreferences ) protected:

private:

    /**
     * Default constructor.
     */
    OSPreferences( void );
   

    std::string actualvfs;

    bool saveconfiguration;
    FXSpinner* mainwindow_width;
    FXSpinner* mainwindow_height;
    FXTextField *leftdir, *rightdir, *defaultdir;
    FXTextField *font1, *font2, *font3;
    FXTextField *newCommandEdit;
    FXTextField *newShutterEdit;
    FXSpinner *thumbsize;
    FXListBox *commandsCombo;
    FXListBox *shortcutList;
    FXListBox *shortcutMaskList;
    FXListBox *iconsList, *iconsList2;
    FXVerticalFrame *commandsPane;
    FXTextField *commandsTextfield;
    FXTextField *commandsTextfieldText;
    FXCheckButton *commandsRescan;
    FXCheckButton *commandsCapture;
    FXLabel *commandsType;
    FXLabel *commandsIcon;
    FXListBox *iconsTheme;
    FXButton *removeCommandButton;
    FXVerticalFrame *vfsPane;
    FXListBox *vfsList;
    FXList*headersList, *availableHeadersList;
    FXListBox *placement;

    FXList *additionalCommands, *additionalCommandsAvailable, *additionalCommandsAvailableForButtons;
    FXList *shutterCommands;

    std::string currentCommandName;
    std::string currentFileType;
    OSObjectManager* objmanager;

    FXVerticalFrame *frame;

    FXComboBox *allMime;
    FXButton *colorbutton;
    FXButton *backcolorbutton;

    FXListBox *shutterList;
    FXListBox *toolbarList;
    FXListBox *fileTypeList;
    FXListBox *fileTypeDefaultBox;
    std::string filetypestring;

    OSMimeApp *mimeapp;
    FXColorDialog *colordlg;

    FXListBox *commandPluginsList, *availableCommandPluginsList;
    FXListBox * availableVfsPluginsList;
    FXList*buttonsList;

    std::map <std::string, OSCommandTypeInfo> commandsMap;
    std::map <std::string, OSFileTypeInfo> filetypesMap;

    std::vector <shutter_container> shutterVector;
    std::vector <toolbar_container> toolbarVector;


    FXVerticalFrame *filetypePane;

    bool validateName( std::string name );
    void setAllColor( FXButton* button, FXColor color );

    FXColor base;
    FXColor border;
    FXColor back;
    FXColor fore;
    FXColor selback;
    FXColor selfore;
    FXColor tipback;
    FXColor tipfore;
    FXColor menuback;
    FXColor menufore;
    FXColor hilite;
    FXColor shadow;
    FXColor main;

    FXToolTip *tooltip;
    FXTabBook *tabbook;
    FXTabItem *tabitem;
    FXVerticalFrame *tabframe;
    FXVerticalFrame *mainframe;
    FXVerticalFrame *menuframe;
    FXHorizontalFrame *labeltextframe1;
    FXHorizontalFrame *labeltextframe2;
    FXHorizontalFrame *textframe1;
    FXHorizontalFrame *textframe2;
    FXHorizontalFrame *tabsubframe;
    FXGroupBox *grpbox1;
    FXGroupBox *grpbox2;
    FXLabel *label1;
    FXLabel *label2;
    FXLabel *label3;
    FXLabel *label4;
    FXLabel *label5;
    FXLabel *menulabels[ 6 ];
    FXTextField *textfield1;
    FXButton *button1;
    FXButton *fontbutton[ 4 ];
    FXSeparator *sep1;
    FXSeparator *sep2;
    FXSeparator *sep3;

private:

    FXFont *font[ 4 ];

    FXuint typingSpeed;
    FXuint clickSpeed;
    FXuint scrollSpeed;
    FXuint scrollDelay;
    FXuint blinkSpeed;
    FXuint animSpeed;
    FXuint menuPause;
    FXuint tooltipPause;
    FXuint tooltipTime;
    FXint dragDelta;
    FXint wheelLines;


private:

    FXDataTarget target_base;
    FXDataTarget target_back;
    FXDataTarget target_border;
    FXDataTarget target_fore;
    FXDataTarget target_hilite;
    FXDataTarget target_shadow;
    FXDataTarget target_selfore;
    FXDataTarget target_selback;
    FXDataTarget target_tipfore;
    FXDataTarget target_tipback;
    FXDataTarget target_menufore;
    FXDataTarget target_menuback;

    FXDataTarget target_main;

    FXDataTarget target_typingspeed;
    FXDataTarget target_clickspeed;
    FXDataTarget target_scrollspeed;
    FXDataTarget target_scrolldelay;
    FXDataTarget target_blinkspeed;
    FXDataTarget target_animspeed;
    FXDataTarget target_menupause;
    FXDataTarget target_tooltippause;
    FXDataTarget target_tooltiptime;
    FXDataTarget target_dragdelta;
    FXDataTarget target_wheellines;

public:

    enum
    {
        ID_SAVE = FXDialogBox::ID_LAST,
        ID_COMMAND_CHANGE,
        ID_FILETYPE_CHANGE,
        ID_VFS_CHANGE,
        ID_NEW_COMMAND,
        ID_NEW_FILETYPE,
        ID_NEW_SHUTTER,
        ID_REMOVE_SHUTTER,
        ID_REMOVE_COMMAND,
        ID_MIME_APP,
        ID_MIME_APP_AUTO,
        ID_ADD_COMMAND_ADDITIONAL,
        ID_DEL_COMMAND_ADDITIONAL,
        ID_ADD_FILETYPE,
        ID_CHOOSE_COLOR,
        ID_CHOOSE_BACKCOLOR,
        ID_COMMANDPLUGIN_CHANGE,
        ID_ADD_BUTTON_COMMAND,
        ID_DEL_BUTTON_COMMAND,
        ID_ADD_SHUTTER_COMMAND,
        ID_DEL_SHUTTER_COMMAND,
        ID_ADD_HEADER,
        ID_DEL_HEADER,
        ID_SHUTTER_CHANGE,
        ID_TOOLBAR_CHANGE,
        ID_DOWNLOAD_INSTALL_CMD_PLUGIN,
        ID_DOWNLOAD_INSTALL_VFS_PLUGIN,
        ID_UPDATE_CMD_PLUGIN_LIST,
        ID_UPDATE_VFS_PLUGIN_LIST,
        ID_COLORS,
        ID_CHOOSE_FONT,
        ID_CHOOSE_CAPTIONFONT1,
        ID_CHOOSE_CAPTIONFONT2,
        ID_CHOOSE_CAPTIONFONT3,
        ID_UPDATE_WINDOW_SIZE,
        ID_UP_BUTTON_COMMAND,
        ID_DOWN_BUTTON_COMMAND,
        ID_UP_SHUTTER_COMMAND,
        ID_DOWN_SHUTTER_COMMAND,
        ID_UP_VFS_HEADER,
        ID_DOWN_VFS_HEADER,
        ID_UP_COMMAND,
        ID_DOWN_COMMAND,
        ID_LAST,
    };

    /**
     * Constructor.
     * @param owner - pointer to parent window.
     */
    OSPreferences ( FXWindow * owner );
    
    /**
     * Destructor.
     */
    virtual ~OSPreferences( void );

    long save ( void );
    long onCommandChange ( FXObject * sender, FXSelector sel, void * );
    long onFileTypeChange ( FXObject * sender, FXSelector sel, void * );
    long onShutterChange ( FXObject * sender, FXSelector sel, void * );
    long onToolbarChange ( FXObject * sender, FXSelector sel, void * );
    long onNewCommand ( FXObject * sender, FXSelector sel, void * );
    long onRemoveCommand ( FXObject * sender, FXSelector sel, void * );
    long onNewShutter ( FXObject * sender, FXSelector sel, void * );
    long onRemoveShutter ( FXObject * sender, FXSelector sel, void * );
    long onOpenMimeApp ( FXObject * sender, FXSelector sel, void * );
    long onAdditionalCommandChange( FXObject * sender, FXSelector sel, void * );
    long onAddFiletype ( FXObject * sender, FXSelector sel, void * );
    long onChooseColor ( FXObject * sender, FXSelector sel, void * );
    long onAddButtonCommand ( FXObject * sender, FXSelector sel, void * );
    long onAddShutterCommand ( FXObject * sender, FXSelector sel, void * );
    long close ( FXObject * sender, FXSelector sel, void * );
    long downloadInstallPlugin ( FXObject * sender, FXSelector sel, void * );
    long updatePluginList ( FXObject * sender, FXSelector sel, void * );
    long onVfsChange ( FXObject * sender, FXSelector sel, void * );
    long onAddHeader ( FXObject * sender, FXSelector sel, void * );
    long onColorChanged( FXObject*, FXSelector, void* );
    long onChooseFont( FXObject*, FXSelector, void* );
    long updateWindowSize( FXObject * sender, FXSelector sel, void * );
    long onUpDown( FXObject * sender, FXSelector sel, void * );
    long onUpDownVfsHeader( FXObject * sender, FXSelector sel, void * );
    long onUpDownCommand( FXObject * sender, FXSelector sel, void * );

    void setupColors( void );
    void setupFont( int nr );
};

#endif // #ifndef __OSPREFERENCES_H__
