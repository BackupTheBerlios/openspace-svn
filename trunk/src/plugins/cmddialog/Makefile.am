pkglib_LTLIBRARIES= libaccess.la libmkdir.la librename.la
pkglibdir=$(datadir)/openspace/plugins/cmddialog
INCLUDES = `WANT_FOX="1.4" fox-config --cflags` `xml2-config  --cflags` 

libaccess_la_SOURCES =access.cpp access.h  ../../thread_elem.cpp ../../thread_elem.h ../../configure.cpp ../../configure.h ../../cmddialog.cpp ../../cmddialog.h
libaccess_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs`  

libmkdir_la_SOURCES =mkdir.cpp mkdir.h  ../../thread_elem.cpp ../../thread_elem.h ../../configure.cpp ../../configure.h ../../cmddialog.cpp ../../cmddialog.h
libmkdir_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 

librename_la_SOURCES =rename.cpp rename.h  ../../thread_elem.cpp ../../thread_elem.h ../../configure.cpp ../../configure.h ../../cmddialog.cpp ../../cmddialog.h
librename_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 
