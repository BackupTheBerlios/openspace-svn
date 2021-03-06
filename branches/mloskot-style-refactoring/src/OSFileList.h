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
// C++ Interface: OSFileList
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __FILELIST_H__
#define __FILELIST_H__

#include "OSVirtualFileSystemBase.h"
#include "OSPathType.h"
#include "OSThreadExec.h"
#include "OSFileTypeSymbol.h"
#include "OSCMDDialogBox.h"
#include <string>
#include <map>
#include "OSPreferences.h"

//OSFileList
class OSFileList: public FXIconList
{

    FXDECLARE ( OSFileList )

private:


    string previous_path;
    string type;
    OSPathType *pt;

    FXWindow *notifyparent;
    FXLabel *label;
    FXLabel *info;
    FXMenuPane *popupmenu;
    FXMenuPane *popupwindow;
    FXTextField *textfield;
    FXOptionMenu *sortmenu;
    FXPopup *sortpop;
    FXHorizontalFrame *bottomframe;

    vector < string > commands_tab;
    vector < string > key_commands_tab;
    vector < string > button_commands_tab;
    vector < FXIcon * >icon_vec;
    vector < string > header_vec;
    vector < string > vector_name;
    vector < int >vector_type;
    vector < int >vector_width;

    bool show_hidden_files;
    string *fields_name;
    unsigned int *fields_type;
    unsigned int *fields_width;
    int sort_nr;
    unsigned long thumb_size;

    static bool ascend;
    static bool strcase;

    bool processing;
    bool maximize;
    int popup_x;
    int popup_y;

    int icons_size;

    OSCMDDialogBox *dial;

    FXDragAction dropaction;    // Drop action
    FXString dragfiles;     // Dragged files

    void dropData ( bool clipboard );
    void OSFileList::selectitem ( void );
    void OSFileList::start_thread ( OSThreadExec * te );

    void OSFileList::copymoveremove ( string com_name );
    static void *OSFileList::thread_func ( void *data );
    static FXint cmp ( const FXIconItem * a, const FXIconItem * b );


    string returnpath( string dirname );
    string OSFileList::getdefaultcommand ( string name );
    string OSFileList::resolvecommand( string command, string name );
    int OSFileList::runCommand( string name );
    static string OSFileList::getfiletype ( string name );
    long OSFileList::setKeys ( void );

public:

    OSFileList(void) {}
    virtual ~OSFileList( void );

    //==============================================
    OSFileList ( FXComposite * p, OSPathType pt );
    //==============================================


    OSObjectManager *objmanager;
    FXIcon **specialicons;
    FXToolBar *toolbar, *toolbar2;

    string path;
    bool active;
    OSVirtualFileSystemBase * fb;
    OSFileList *filelist_opposite;

    enum
    {
        ID_ICO = FXIconList::ID_LAST,
        ID_SORT_CHANGE,
        ID_TEXTFIELD_RUN,
        ID_LAST,
    };


    virtual void create ();

    void OSFileList::refresh ( void );
    bool OSFileList::init ( void );
    bool OSFileList::opendir ( string dir );

    long OSFileList::setFocus ( FXObject * obj, FXSelector sel, void *ptr );
    long OSFileList::lostFocus ( FXObject * obj, FXSelector sel, void *ptr );
    long OSFileList::openfile ( FXObject *, FXSelector, void * );
    long OSFileList::gotoparentdir ( FXObject *, FXSelector, void * );
    long OSFileList::onPopup ( FXObject *, FXSelector, void * );
    long OSFileList::file_operation ( FXObject *, FXSelector, void * );
    long OSFileList::onCmdHeader ( FXObject *, FXSelector, void *ptr );
    long OSFileList::onCommand ( FXObject *, FXSelector, void *ptr );
    long OSFileList::onCommandCancel ( FXObject *, FXSelector, void *ptr );
    long OSFileList::click ( FXObject *, FXSelector, void *ptr );
    long OSFileList::onCmdResize( FXObject * sender, FXSelector, void * );
    long OSFileList::key_shortcut ( FXObject * obj, FXSelector sel, void *ptr );
    long OSFileList::texfield_run ( FXObject * obj, FXSelector sel, void *ptr );

    long onDNDEnter ( FXObject *, FXSelector, void * );
    long onDNDLeave ( FXObject *, FXSelector, void * );
    long onDNDMotion ( FXObject *, FXSelector, void * );
    long onDNDDrop ( FXObject *, FXSelector, void * );
    long onDNDRequest ( FXObject *, FXSelector, void * );
    long onBeginDrag ( FXObject *, FXSelector, void * );
    long onEndDrag ( FXObject *, FXSelector, void * );
    long onDragged ( FXObject *, FXSelector, void * );

    long onClipboardLost ( FXObject *, FXSelector, void * );
    long onClipboardGained ( FXObject *, FXSelector, void * );
    long onClipboardRequest ( FXObject *, FXSelector, void * );

};

#endif // #ifndef __FILELIST_H__
