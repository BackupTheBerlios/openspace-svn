pkglib_LTLIBRARIES= libOSVfsLocal.la libOSVfsSearch.la libOSVfsArchive.la libOSVfsFtp.la    libOSVfsSftp.la
pkglibdir=$(libdir)/nao/plugins/filelist
INCLUDES = `WANT_FOX="1.4" fox-config --cflags` `xml2-config  --cflags` 

libOSVfsLocal_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs`  
libOSVfsLocal_la_SOURCES =OSVfsLocal.cpp OSVfsLocal.h OSFXFile.cpp OSFXFile.h ../../OSThreadExec.cpp ../../OSThreadExec.h  ../../OSConfigure.h ../../OSUtils.cpp ../../OSUtils.h 

libOSVfsSearch_la_LDFLAGS = `WANT_FOX=1.4 fox-config --libs` `xml2-config  --libs`
libOSVfsSearch_la_SOURCES =OSVfsSearch.cpp OSVfsSearch.h OSFXFile.cpp OSFXFile.h ../../OSThreadExec.cpp ../../OSThreadExec.h  ../../OSConfigure.h 
libOSVfsArchive_la_LDFLAGS = `WANT_FOX=1.4 fox-config --libs` `xml2-config  --libs`
libOSVfsArchive_la_SOURCES =OSVfsArchive.cpp OSVfsArchive.h ../../OSThreadExec.cpp ../../OSThreadExec.h  ../../OSConfigure.h 
libOSVfsFtp_la_LDFLAGS = `WANT_FOX=1.4 fox-config --libs` `xml2-config  --libs`
libOSVfsFtp_la_SOURCES =OSVfsFtp.cpp OSVfsFtp.h  pftp.cpp pftp.h  ../../OSThreadExec.cpp ../../OSThreadExec.h ../../OSConfigure.h 

libOSVfsSftp_la_LDFLAGS = `WANT_FOX=1.4 fox-config --libs` `xml2-config  --libs` -lssh
libOSVfsSftp_la_SOURCES =OSVfsSftp.cpp OSVfsSftp.h OSFXFile.cpp OSFXFile.h ../../OSThreadExec.cpp ../../OSThreadExec.h  ../../OSConfigure.h 


#libfilelistsftp_la_LDFLAGS = `WANT_FOX=1.4 fox-config --libs` `xml2-config  --libs`
#libfilelistsftp_la_SOURCES =filelist_sftp.cpp filelist_sftp.h  ../../OSThreadExec.cpp ../../OSThreadExec.h  ../../OSConfigure.h 

