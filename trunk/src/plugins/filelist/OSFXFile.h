#ifndef OSFXFILE_H
#define OSFXFILE_H

#include <fx.h>

class OSThreadExec;

/// Declared as "C" so as to not clash tag-names
extern "C"
{
    struct stat;
}

namespace OSFXFile
{


/// Remove file or directory, recursively.
    FXbool remove (const FXString & file, OSThreadExec* te);

/// Copy file or directory, recursively
    FXbool copy (const FXString & srcfile, const FXString & dstfile, OSThreadExec * te, FXbool overwrite = FALSE);

/// Rename or move file or directory
    FXbool move (const FXString & srcfile, const FXString & dstfile, OSThreadExec* te, FXbool overwrite = FALSE);

}

#endif

