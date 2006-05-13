pkglib_LTLIBRARIES=libOSMkdir.la libOSAccess.la libOSRename.la libOSClone.la libOSSymlink.la libOSEditSymlink.la libOSCompress.la libOSMd5sum.la
pkglibdir=$(libdir)/nao/plugins/cmddialog
INCLUDES = `WANT_FOX="1.4" fox-config --cflags` `xml2-config  --cflags` 

libOSMkdir_la_SOURCES =OSMkdir.cpp  ../../OSThreadExec.cpp ../../OSThreadExec.h ../../OSCMDDialogBox.cpp ../../OSCMDDialogBox.h 
libOSMkdir_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 

libOSAccess_la_SOURCES =OSAccess.cpp  ../../OSThreadExec.cpp ../../OSThreadExec.h ../../OSCMDDialogBox.cpp ../../OSCMDDialogBox.h 
libOSAccess_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs`  

libOSRename_la_SOURCES =OSRename.cpp  ../../OSThreadExec.cpp ../../OSThreadExec.h ../../OSCMDDialogBox.cpp ../../OSCMDDialogBox.h 
libOSRename_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 

libOSClone_la_SOURCES =OSClone.cpp  ../../OSThreadExec.cpp ../../OSThreadExec.h  ../../OSCMDDialogBox.cpp ../../OSCMDDialogBox.h 
libOSClone_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 

libOSSymlink_la_SOURCES =OSSymlink.cpp  ../../OSThreadExec.cpp ../../OSThreadExec.h  ../../OSCMDDialogBox.cpp ../../OSCMDDialogBox.h 
libOSSymlink_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 

libOSEditSymlink_la_SOURCES =OSEditSymlink.cpp  ../../OSThreadExec.cpp ../../OSThreadExec.h  ../../OSCMDDialogBox.cpp ../../OSCMDDialogBox.h 
libOSEditSymlink_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 

libOSCompress_la_SOURCES =OSCompress.cpp  ../../OSThreadExec.cpp ../../OSThreadExec.h  ../../OSCMDDialogBox.cpp ../../OSCMDDialogBox.h 
libOSCompress_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 

libOSMd5sum_la_SOURCES =OSMd5sum.cpp  ../../OSThreadExec.cpp ../../OSThreadExec.h  ../../OSCMDDialogBox.cpp ../../OSCMDDialogBox.h 
libOSMd5sum_la_LDFLAGS = `WANT_FOX="1.4" fox-config --libs` `xml2-config  --libs` 
