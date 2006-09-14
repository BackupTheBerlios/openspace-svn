#include <fx.h>
#include "../../OSCMDDialogBox.h"
#include "../../OSThreadExec.h"

#include <string>
using namespace std;

#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif


#ifdef WIN32
#define SEPARATOR "\\"
#else
#include <dlfcn.h>
#define SEPARATOR "/"
#endif

class OSClone: public OSCMDDialogBox
{
FXDECLARE ( OSClone ) protected:

    OSClone ( const OSClone & )
    {}

private:
    vector < FXTextField * >vec;
    string dir;

public:

    enum
    {
        ID_ENTER = OSCMDDialogBox::ID_LAST,
    };


    OSClone ()
    {}
    OSClone ( FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src );


    int exec ( void )
    {
        int error = 0;
    
        
        vector < string >::iterator iter;
        int i = 0;
        for ( iter = src.begin (); iter != src.end(); iter++ )
        {
            string newname = dir + SEPARATOR + vec[ i ] ->getText ().text ();
            string newonlyname = vec[ i ] ->getText ().text ();

            string destdir = "/tmp/nao";
            string filename = FXFile::name( iter->c_str() ).text();
            string dirname = FXFile::directory( iter->c_str() ).text();

            OSThreadExec *el2 = new OSThreadExec ( fb, "copy", "upload", *iter, destdir );
            fb->copy ( el2 );
            delete el2;



            if ( fb->rename ( destdir + "/" + filename, destdir + "/" + newonlyname ) == false )
            {
                error = -1;
                string err = "can't clone " + *iter + " to " + newname;

                FXLabel *lab = new FXLabel ( contents, err.c_str () );
                lab->create ();
                this->layout ();
                this->resize ( this->getWidth (), this->getHeight () + lab->getHeight () );

            }
            else
            {

                el2 = new OSThreadExec ( fb, "move", "upload", destdir + "/" + newonlyname, dirname );
                fb->copy ( el2 );
                delete el2;

            }


            i++;
        }

    
        return error;

    }


    long press ( FXObject * sender, FXSelector, void * );

};

FXDEFMAP ( OSClone ) OSCloneMap[] =
    {
        FXMAPFUNC ( SEL_COMMAND, OSClone::ID_ENTER, OSClone::press ), };
FXIMPLEMENT ( OSClone, OSCMDDialogBox, OSCloneMap, ARRAYNUMBER ( OSCloneMap ) )

OSClone::OSClone ( FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src ) :
        OSCMDDialogBox ( w, fb, src )
{


    vector < string >::iterator iter;

    for ( iter = src.begin (); iter != src.end(); iter++ )
    {


        dir = FXFile::directory ( iter->c_str () ).text ();

        FXString name = FXFile::name ( iter->c_str () );
        new FXLabel ( contents, name );
        FXTextField *text = new FXTextField ( contents, 25, this, ID_ENTER );
        text->setFocus ();
        text->setText ( name );
        vec.push_back ( text );

    }



}

long OSClone::press ( FXObject * sender, FXSelector, void * )
{

    FXObject * target = ( FXObject * ) ok->getTarget ();
    target->handle ( ok, FXSEL ( SEL_COMMAND, ID_COMMAND ), NULL );

}





EXPORTFUNCTION OSCMDDialogBox *get_cmddialog ( FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src )
{
    return new OSClone ( w, fb, src );
}
