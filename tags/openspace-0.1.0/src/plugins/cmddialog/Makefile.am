pkglib_LTLIBRARIES= libaccess.la libmkdir.la librename.la libclone.la libsymlink.la libeditsymlink.la libcompress.la libmd5sum.la
pkglibdir=$(libdir)/openspace/plugins/cmddialog
INCLUDES = `WANT_FOX="1.4" fox-config --cflags` `xml2-config  --cflags` 

libaccess_la_SOURCES =access.cpp  ../../thread_elem.cpp ../../thread_elem.h ../../cmddialog.cpp ../../cmddialog.h
libaccess_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs`  

libmkdir_la_SOURCES =mkdir.cpp  ../../thread_elem.cpp ../../thread_elem.h ../../cmddialog.cpp ../../cmddialog.h
libmkdir_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 

librename_la_SOURCES =rename.cpp  ../../thread_elem.cpp ../../thread_elem.h ../../cmddialog.cpp ../../cmddialog.h
librename_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 

libclone_la_SOURCES =clone.cpp  ../../thread_elem.cpp ../../thread_elem.h  ../../cmddialog.cpp ../../cmddialog.h
libclone_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 

libsymlink_la_SOURCES =symlink.cpp  ../../thread_elem.cpp ../../thread_elem.h  ../../cmddialog.cpp ../../cmddialog.h
libsymlink_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 

libeditsymlink_la_SOURCES =editsymlink.cpp  ../../thread_elem.cpp ../../thread_elem.h  ../../cmddialog.cpp ../../cmddialog.h
libeditsymlink_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 

libcompress_la_SOURCES =compress.cpp  ../../thread_elem.cpp ../../thread_elem.h  ../../cmddialog.cpp ../../cmddialog.h
libcompress_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 

libmd5sum_la_SOURCES =md5sum.cpp  ../../thread_elem.cpp ../../thread_elem.h  ../../cmddialog.cpp ../../cmddialog.h
libmd5sum_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 
