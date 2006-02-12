bin_PROGRAMS = openspace

openspace_SOURCES = main.cpp OSCMDDialogBox.cpp OSCMDDialogBox.h \
	OSCommandTypeInfo.cpp OSCommandTypeInfo.h OSConfigure.cpp OSConfigure.h OSFile.h \
	OSFileList.cpp OSFileList.h OSFileListItem.cpp OSFileListItem.h OSFileTypeInfo.cpp \
	OSFileTypeInfo.h OSFileTypeSymbol.cpp OSFileTypeSymbol.h OSFirstRunDialogBox.cpp \
	OSFirstRunDialogBox.h OSInfoPanel.cpp OSInfoPanel.h OSMainWindow.cpp OSMainWindow.h \
	OSMimeApp.cpp OSMimeApp.h OSMimeType.cpp OSMimeType.h OSObjectManager.cpp \
	OSObjectManager.h OSPathType.cpp OSPathType.h OSPreferences.cpp OSPreferences.h \
	OSShutter.cpp OSShutter.h OSThreadExec.cpp OSThreadExec.h OSUtils.cpp OSUtils.h \
	OSVirtualFileSystemBase.h OSVirtualFileSystemHeader.h OSVirtualFileSystemInfo.h \
	OSFrame.h OSFrame.cpp

SUBDIRS = plugins

INCLUDES = -I$(top_srcdir)/src `WANT_FOX="1.4" fox-config --cflags`  `xml2-config  --cflags`  \
	-DPATH_CFG=\"$(sysconfdir)\" \
	-DPATH_LIBDIR=\"$(libdir)\"

openspace_LDFLAGS = `WANT_FOX="1.4" fox-config --libs`  `xml2-config  --libs`  

noinst_HEADERS = OSVirtualFileSystemHeader.h OSVirtualFileSystemInfo.h \
	OSFileTypeInfo.h
