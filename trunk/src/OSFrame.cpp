#include "OSFrame.h" 
#include "OSMainWindow.h" 

OSFrame::OSFrame ( FXComposite * cp, FXComposite * p, OSPathType pt, FXObject * tgt,FXDockSite* dock1,FXDockSite* dock2,long id,OSFileListController* controller)
{
    this->pathdir = pt.dir;
    this->type = pt.type;
    this->id=id;
    string path = pathdir;
    hf = new FXHorizontalFrame ( cp, LAYOUT_FILL_X | FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    pathdir = path;
    firstbutton=NULL;
    
    this->dock1=dock1;
    this->dock2=dock2;
    
    FXButton *prebutton = NULL;
    FXButton *nextbutton = NULL;

    OSObjectManager*objmanager = OSObjectManager::instance( cp->getApp() );

    toleft = new FXButton ( hf, "", objmanager->osicons[ "left" ], tgt, OSMainWindow::ID_TOLEFT, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );
    toleft->setUserData ( new box ( this, NULL, NULL, NULL ) );
    toclose = new FXButton ( hf, "", objmanager->osicons[ "close" ], tgt, OSMainWindow::ID_TOCLOSE, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );
    toclose->setUserData ( new box ( this, NULL, NULL, NULL ) );
    toright = new FXButton ( hf, "", objmanager->osicons[ "right" ], tgt, OSMainWindow::ID_TORIGHT, FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0 );
    toright->setUserData ( new box ( this, NULL, NULL, NULL ) );
    if ( pt.server != "" )
    {
        string lab = pt.server + ": ";
        new FXLabel( hf, lab.c_str() );
    }

   
    generateMenu(pathdir,tgt);

    frame = new FXVerticalFrame ( p, LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_SUNKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
    f = new OSFileList ( frame, pt, id, controller );
  

}

void OSFrame::create(void)
{
    frame->create ();
    hf->create ();
}



//generate buttons path for given path
void OSFrame::generateMenu ( string path, FXObject * tgt )
{
    OSObjectManager * objmanager = OSObjectManager::instance( hf->getApp() );

    if (path.length()>1 && path[ 0 ] == '/' && path[ 1 ] == '/' )
        path.erase ( 0, 1 );
	
    	if(firstbutton!=NULL)
	{	
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
                                      OSMainWindow::ID_DIR, FRAME_THICK, 0, 0, 0, 0, 0, 0,
                                      0, 0 );
        bt->setBackColor ( objmanager->maincolor );
	
	if(firstbutton!=NULL)
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


OSFrame::~OSFrame ()
{
	    
    	    delete hf;
            delete f;
            delete frame;
}

bool OSFrame::openDir(string dir)
{
	return f->openDir(dir);
}


string OSFrame::getDir(void)
{
	return f->getDir();
}


void OSFrame::moveToFront(FXComposite * controlframeContainer,FXComposite * frameContainer,OSFrame * frameOpposite)  
{
	hf->reparent ( controlframeContainer );
	frame->reparent ( frameContainer );
	toleft->hide ();
        toright->hide ();
        toclose->hide ();
	frame->show ();	
	
    	frameOpposite->f->filelist_opposite = f;
	
    	f->filelist_opposite = frameOpposite->f;    
	f->toolbar->dock(dock1);
  	f->toolbar2->dock(dock2);
        f->toolbar->show();        
        f->toolbar2->show();		
	f->handle ( frameContainer, FXSEL ( SEL_FOCUSIN, OSFileList::ID_ICO ), NULL );	
	
	frameContainer->recalc();
        
}
void OSFrame::moveToBack(FXComposite * controlframeContainer)
{
	hf->reparent ( controlframeContainer );
	
        f->toolbar->hide();
        f->toolbar2->hide();
	
        toleft->show ();
        toright->show ();
        toclose->show ();
        frame->hide ();

	
	controlframeContainer->recalc ();
}

