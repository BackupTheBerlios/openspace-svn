pkglib_LTLIBRARIES= libfilelistlocal.la  libfilelistsearch.la  libfilelistarchive.la libfilelistftp.la libfilelistsftp.la
pkglibdir=$(libdir)/openspace/plugins/filelist
INCLUDES = `WANT_FOX="1.4" fox-config --cflags` `xml2-config  --cflags` 

libfilelistlocal_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs`  
libfilelistlocal_la_SOURCES =filelist_local.cpp filelist_local.h OSFXFile.cpp OSFXFile.h ../../thread_elem.cpp ../../thread_elem.h  ../../configure.h ../../sharedFunctions.cpp ../../sharedFunctions.h 
libfilelistsearch_la_LDFLAGS = `WANT_FOX=1.4 fox-config --libs` `xml2-config  --libs`
libfilelistsearch_la_SOURCES =filelist_search.cpp filelist_search.h OSFXFile.cpp OSFXFile.h ../../thread_elem.cpp ../../thread_elem.h  ../../configure.h 
libfilelistarchive_la_LDFLAGS = `WANT_FOX=1.4 fox-config --libs` `xml2-config  --libs`
libfilelistarchive_la_SOURCES =filelist_archive.cpp filelist_archive.h ../../thread_elem.cpp ../../thread_elem.h  ../../configure.h 
libfilelistsftp_la_LDFLAGS = `WANT_FOX=1.4 fox-config --libs` `xml2-config  --libs`
libfilelistsftp_la_SOURCES =filelist_sftp.cpp filelist_sftp.h  ../../thread_elem.cpp ../../thread_elem.h  ../../configure.h 
libfilelistftp_la_LDFLAGS = `WANT_FOX=1.4 fox-config --libs` `xml2-config  --libs`
libfilelistftp_la_SOURCES =filelist_ftp.cpp filelist_ftp.h  pftp.cpp pftp.h  ../../thread_elem.cpp ../../thread_elem.h ../../configure.h 
