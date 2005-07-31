pkglib_LTLIBRARIES= libfilelistlocal.la
pkglibdir=$(datadir)/openspace/plugins/filelist
INCLUDES = `WANT_FOX="1.4" fox-config --cflags` `xml2-config  --cflags` 

libfilelistlocal_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs`  
libfilelistlocal_la_SOURCES =filelist_local.cpp filelist_local.h OSFXFile.cpp ../../thread_elem.cpp ../../thread_elem.h ../../configure.cpp ../../configure.h
