bin_PROGRAMS = openspace

openspace_SOURCES = cmddialog.cpp \
	filetype.cpp \
	openspace.cpp \
	preferences.cpp \
	configure.cpp \
	informationpanel.cpp \
	oslistitem.cpp \
	shutter.cpp \
	filelist.cpp \
	mainwindow.cpp \
	pathtype.cpp \
	thread_elem.cpp \
	cmddialog.h \
	filetype.h \
	oslistitem.h \
	shutter.h \
	configure.h \
	informationpanel.h \
	pathtype.h \
	thread_elem.h \
	filelist_base.h \
	mainwindow.h \
	preferences.h \
	filelist.h \
	osfile.h \
	sharedobjects.h \
	MimeType.h \
	MimeType.cpp

SUBDIRS = plugins

INCLUDES = -I$(top_srcdir)/src `WANT_FOX="1.4" fox-config --cflags`  `xml2-config  --cflags`  \
	-DPATH_CFG=\"$(sysconfdir)\" \
  	-DPATH_GRC=\"$$HOME/\" \
  	-DPATH_ICN=\"$(datadir)/gentoo/icons\"\
  	-DLOCALEDIR=\"$(localedir)\"
openspace_LDFLAGS = `WANT_FOX="1.4" fox-config --libs`  `xml2-config  --libs`  

