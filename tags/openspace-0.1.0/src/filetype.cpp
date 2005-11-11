#include <fx.h>
#include "filetype.h"

file_type::file_type (FXIcon * ic,FXIcon*ic2, FXColor col, FXColor backcol)
{

    icon = ic;
   	 if(ic2!=NULL)
   	 bigicon=ic2;
 	 else
	 bigicon=ic;
    
    color = col;
    backcolor = backcol;

}
