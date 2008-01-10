## Process this file with automake to produce Makefile.in

bin_PROGRAMS = wxdfast

INCLUDES = \
	$(WX_CXXFLAGS)

if DEBUG
DEBUG_FLAGS = -g -ggdb
endif

if XRCMODE
XRC_FLAGS = -D USE_EXTERNAL_XRC
endif

if PORTABLE
PORTABLE_FLAGS = -D WXDFAST_PORTABLE
endif

CFLAGS =\
	 -Wall -O2 $(DEBUG_FLAGS) $(XRC_FLAGS) $(PORTABLE_FLAGS)

CXXFLAGS =\
	 -Wall -O2 $(DEBUG_FLAGS) $(XRC_FLAGS) $(PORTABLE_FLAGS)

wxdfast_SOURCES = \
	BoxFind.cpp\
	DownloadThread.cpp\
	InProgressList.cpp\
	UtilFunctions.cpp\
	BoxNew.cpp\
	FinishedList.cpp\
	MainFrame.cpp\
	Metalink.cpp\
	wxDFast.cpp\
	BoxOptions.cpp\
	FTP.cpp\
	ProgressBar.cpp\
	CheckNewRelease.cpp\
	Graph.cpp\
	TaskBarIcon.cpp\
	DownloadFile.cpp\
	HTTP.cpp\
	UrlName.cpp\
	wxMD5/md5.cpp\
	wxMD5/wxMD5.cpp\
	wxSHA1/sha1.cpp\
	wxSHA1/wxSHA1.cpp

## Include the Resources.cpp file
## which is created using the XRC files
## See the Makefile.am inside resources/
if NOXRCMODE
wxdfast_SOURCES += ../resources/Resources.cpp
endif

## if WIN32BUILD
## SUFFIXES = .rc
## EXTRA_wxdfast_SOURCES = \
## 	wxDFast.rc
## wxDFast.res: wxDFast.rc
## 	@WINDRES@ -i wxDFast.rc -J rc -o $@ -O coff --include-dir=
## endif

wxdfast_LDFLAGS = \
	$(WX_LIBS)


wxdfast_LDADD =  \
	$(WX_LIBS)

## Build wxDFast.rc
if WIN32BUILD
wxdfast_LDFLAGS += wxDFast.res
wxDFast.res: wxDFast.rc
	@WINDRES@ -i $< -J rc -O coff -o $@ --include-dir=`@WX_CONFIG_PATH@ --cflags | @AWK@ -F-I '{ print $$3 }'`
BUILT_SOURCES = wxDFast.res
endif
