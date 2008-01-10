//
// C++ Interface: wxDFast
//
// Description: This file include the external files, and declare the
//              classes used by the program.
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __WXPROG_H__
    #define __WXPROG_H__
    #include "wx/wxprec.h"
    #include "wx/wx.h"
    #include "wx/app.h"
    #include "wx/utils.h"
    #include "wx/listctrl.h"
    #include "wx/treectrl.h"
    #include "wx/html/htmlwin.h"
    #include "wx/frame.h"
    #include "wx/fileconf.h"
    #include "wx/splitter.h"
    #include "wx/notebook.h"
    #include "wx/imaglist.h"
    #include "wx/protocol/ftp.h"
    #include "wx/protocol/http.h"
    #include "wx/file.h"
    #include "wx/thread.h"
    #include "wx/toolbar.h"
    #include "wx/bitmap.h"
    #include "wx/filename.h"
    #include "wx/menu.h"
    #include "wx/clipbrd.h"
    #include "wx/sckstrm.h"
    #include "wx/fdrepdlg.h"
    #include "wx/cmdline.h"
    #include "wx/progdlg.h"
    #include "wx/ipc.h"
    #include "wx/process.h"
    #include "wx/socket.h"
    #include "wx/spinctrl.h"
    #include "wx/taskbar.h"
    #include "wx/xrc/xmlres.h"
    #include "wx/image.h"
    #include "wx/snglinst.h"
    #include "wx/pen.h"
    #include "wx/calctrl.h"
    #include "wx/tokenzr.h"
    #include "wx/wfstream.h"
    #include "wx/longlong.h"
    #include "wx/uri.h"
    #include "wx/datectrl.h"
    #include "wx/xml/xml.h"
    #include "wx/zipstrm.h"
    #include "wx/dcbuffer.h"
	#include "wx/stdpaths.h"
    #include "wxMD5/wxMD5.h"
    #include "wxSHA1/wxSHA1.h"
    #ifndef USE_EXTERNAL_XRC
    extern void InitXmlResource();
    #endif


    //CUSTOM EVENTS
    extern const wxEventType wxEVT_OPEN_URL;
    extern const wxEventType wxEVT_SHUTDOWN;
    extern const wxEventType wxEVT_DISCONNECT;
    extern const wxEventType wxEVT_NEW_RELEASE;
    extern const wxEventType wxEVT_NEW_DOWNLOAD;
    extern const wxEventType wxEVT_EXECUTE_COMMAND;

    const wxCmdLineEntryDesc cmdlinedesc[] =
    {
        { wxCMD_LINE_SWITCH, wxT("hide"), wxT("hide"), wxT("Start with the the main frame hide.")},
        { wxCMD_LINE_SWITCH, wxT("notray"), wxT("notray"),  wxT("Don't show the icon on system tray")},
        { wxCMD_LINE_OPTION, wxT("list"), wxT("list"),  wxT("Parse a text file with the list of files to download")},
        { wxCMD_LINE_OPTION, wxT("destination"), wxT("destination"),  wxT("Destination directory")},
        { wxCMD_LINE_OPTION, wxT("comments"), wxT("comments"),  wxT("Add comments to download")},
        { wxCMD_LINE_OPTION, wxT("reference"), wxT("reference"),  wxT("Set a reference URL")},
        { wxCMD_LINE_PARAM, NULL, NULL,  wxT("URL of the file(s) to be downloaded"),wxCMD_LINE_VAL_STRING,wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE},
        { wxCMD_LINE_NONE }
    };

    #define ID_MY_WINDOW    1324
    #ifdef __WXMSW__
        #define wxXPM(x)  (wxBitmap((const char **)x))
    #else
        #define wxXPM(x)  (wxBitmap(x))
    #endif

    const wxString PROGRAM_NAME = wxT("wxDownload Fast");
    const wxString VERSION = wxT("0.6.0");
    const wxString SEPARATOR_URL = wxT("/");
    #ifdef __WXMSW__
       const wxString SEPARATOR_DIR = wxT("\\");
    #else
       const wxString SEPARATOR_DIR = wxT("/");
    #endif

    #define MANUAL                     0
    #define NOW                        1
    #define SCHEDULE                   2

    #define MAX_NUM_PARTS              32
    #define DEFAULT_NUM_PARTS          5
    #define DEFAULT_START_OPTION       NOW
    #define DEFAULT_ONTOP_OPTION       false
                                        //START THE DOWNLOAD IMMEDIATLY
    #define MIN_SIZE_TO_SPLIT          500000l
    #define USE_HTML_MESSAGES          1
                                        //CHANGE THIS AND THE FILE MAIN_WINDOW.XRC
    #define MAX_SCHEDULE_EXCEPTIONS    9

    #define IPC_SERVICE                wxT("24242")
    #define IPC_TOPIC                  wxT("IPC_WXDOWNLAD_FAST_")
    #define IPC_END_CONNECTION         wxT("IPC_WXDOWNLAD_FAST_END_CONECTION")

    #ifdef __WXMAC__
        #define DISABLE_MUTEX
    #endif

    const wxString STOPWITHOUTSAVINGSTOPSTATUS = wxT("STOPWITHOUTSAVINGSTOPSTATUS");

    const wxString DFAST_REG = wxT("wxDownloadFast");
    const wxString FILES_REG = wxT("files");
    const wxString INPROGRESS_REG = wxT("files/inprogress");
    const wxString FINISHED_REG = wxT("files/finished");
    const wxString BACK_DIR_REG = wxT("..");
    const wxString NAME_REG = wxT("name");
    const wxString INDEX_REG = wxT("index");
    const wxString STATUS_REG = wxT("status");
    const wxString SCHEDULED_REG = wxT("scheduled");
    const wxString RESTART_REG = wxT("restart");
    const wxString PARTS_REG = wxT("parts");
    const wxString DESTINATION_REG = wxT("destination");
    const wxString TEMPDESTINATION_REG = wxT("tempdestination");
    const wxString SIZE_REG = wxT("size");
    const wxString SIZEPART_REG = wxT("sizepart");
    const wxString SIZECOMPLETED_REG = wxT("sizecompleted");
    const wxString SIZEPARTCOMPLETED_REG = wxT("sizepartcompleted");
    const wxString TIMEPASSED_REG = wxT("timepassed");
    const wxString PERCENTUAL_REG = wxT("percentual");
    const wxString SPEED_REG = wxT("speed");
    const wxString URL_REG = wxT("url");
    const wxString MD5_REG = wxT("md5");
    const wxString COMMENTS_REG = wxT("comments");
    const wxString REFERENCE_REG = wxT("reference");
    const wxString COMMAND_REG = wxT("command");
    const wxString CONTENTTYPE_REG = wxT("contenttype");
    const wxString BANDWIDTH_REG = wxT("bandwidth");
    const wxString START_REG = wxT("date_start");
    const wxString END_REG = wxT("date_end");
    const wxString USER_REG = wxT("user");
    const wxString PASSWORD_REG = wxT("password");
    const wxString NEED_TO_REGET_METALINK_REG = wxT("needtoregetmetalink");
    const wxString METALINK_INDEX_REG = wxT("metalinkindex");

    const wxString CONFIG_REG = wxT("config");
    const wxString LANGUAGE_REG = wxT("language");
    const wxString DETAILS_REG = wxT("details");
    const wxString SIZE_X_REG = wxT("width");
    const wxString SIZE_Y_REG = wxT("height");
    const wxString POS_X_REG = wxT("posx");
    const wxString POS_Y_REG = wxT("posy");
    const wxString MAXIMIZED_REG = wxT("maximized");
    const wxString SEPARATOR01_REG = wxT("separator01position");
    const wxString SEPARATOR02_REG = wxT("separator02position");

    const wxString OPT_LAST_NEW_RELEASE_CHECK = wxT("lastnewreleasecheck");
    const wxString OPT_CURRENT_RELEASE = wxT("currentrelease");
    const wxString OPT_DIALOG_CLOSE_REG = wxT("closewindow");
    const wxString OPT_ATTEMPTS_REG = wxT("attempts");
    const wxString OPT_ATTEMPTS_TIME_REG = wxT("attemptstime");
    const wxString OPT_SIMULTANEOUS_REG = wxT("simultaneous");
    const wxString OPT_REMEMBER_BOXNEW_OPTIONS_REG = wxT("rememberboxnewoptions");
    const wxString OPT_DESTINATION_REG = wxT("dirdestination");
    const wxString OPT_DOWNLOAD_PARTS_DEFAULT_DIR_REG = wxT("downloadpartsdefaultdir");
    const wxString OPT_FILE_MANAGER_PATH_REG = wxT("filemanagerpath");
    const wxString OPT_BROWSER_PATH_REG = wxT("browserpath");
    const wxString OPT_SHUTDOWN_REG = wxT("shutdown");
    const wxString OPT_SHUTDOWN_CMD_REG = wxT("shutdowncmd");
    const wxString OPT_DISCONNECT_REG = wxT("disconnect");
    const wxString OPT_DISCONNECT_CMD_REG = wxT("disconnectcmd");
    const wxString OPT_TIMERINTERVAL_REG = wxT("timerinterval");
    const wxString OPT_READBUFFERSIZE_REG = wxT("readbuffersize");
    const wxString OPT_RESTORE_MAINFRAME_REG = wxT("restoremainframe");
    const wxString OPT_HIDE_MAINFRAME_REG = wxT("hidemainframe");
    const wxString OPT_CHECK_FOR_UPDATES_REG = wxT("checkforupdates");
    const wxString OPT_PROGRESS_BAR_SHOW_REG = wxT("progressbarshow");
    const wxString OPT_GRAPH_SHOW_REG = wxT("graphshow");
    const wxString OPT_GRAPH_HOWMANYVALUES_REG = wxT("graphhowmanyvalues");
    const wxString OPT_GRAPH_REFRESHTIME_REG = wxT("graphrefreshtime");
    const wxString OPT_GRAPH_SCALE_REG = wxT("graphscale");
    const wxString OPT_GRAPH_TEXTAREA_REG = wxT("graphtextarea");
    const wxString OPT_GRAPH_HEIGHT_REG = wxT("graphheight");
    const wxString OPT_GRAPH_SPEEDFONTSIZE_REG = wxT("graphspeedfontsize");
    const wxString OPT_GRAPH_LINEWIDTH_REG = wxT("graphlinewidth");
    const wxString OPT_GRAPH_COLORBACK_REG = wxT("graphcolorbackground");
    const wxString OPT_GRAPH_COLORGRID_REG = wxT("graphcolorgrid");
    const wxString OPT_GRAPH_COLORLINE_REG = wxT("graphcolorline");
    const wxString OPT_GRAPH_COLORFONT_REG = wxT("graphcolorfont");
    const wxString OPT_SCHED_ACTIVATESCHEDULING_REG = wxT("activatescheduling");
    const wxString OPT_SCHED_STARTDATETIME_REG = wxT("schedstartdatetime");
    const wxString OPT_SCHED_FINISHDATETIME_REG = wxT("schedfinishdatetime");
    const wxString OPT_SCHED_SCHEDULEEXCEPTION_START_REG = wxT("scheduleexceptionstart");
    const wxString OPT_SCHED_SCHEDULEEXCEPTION_FINISH_REG = wxT("scheduleexceptionfinish");
    const wxString OPT_SCHED_SCHEDULEEXCEPTION_DAY_REG = wxT("scheduleexceptionday");
    const wxString OPT_SCHED_SCHEDULEEXCEPTION_ISACTIVE_REG = wxT("scheduleexceptionisactive");
    const wxString OPT_LAST_DESTINATION_REG = wxT("lastdestination");
    const wxString OPT_LAST_COMMAND_REG = wxT("lastcommand");
    const wxString OPT_LAST_NUMBER_OF_PARTS_REG = wxT("lastnumberofparts");
    const wxString OPT_LAST_START_OPTION_REG = wxT("laststartoption");
    const wxString OPT_LAST_ONTOP_OPTION_REG = wxT("lastontopoption");
    const wxString OPT_LAST_BOXNEW_X_REG = wxT("lastboxnew_x");
    const wxString OPT_LAST_BOXNEW_Y_REG = wxT("lastboxnew_y");
    const wxString OPT_BAND_WIDTH_OPTION_REG = wxT("bandwidthoption");
    const wxString OPT_BAND_WIDTH_GENERAL_REG = wxT("bandwidthgeneral");
    const wxString OPT_TASKBAR_ICON_SIZE_REG = wxT("taskbariconsize");
    const wxString OPT_PROXY_REG = wxT("proxyuse");
    const wxString OPT_PROXY_SERVER_REG = wxT("proxyserver");
    const wxString OPT_PROXY_PORT_REG = wxT("proxyport");
    const wxString OPT_PROXY_USERNAME_REG = wxT("proxyusername");
    const wxString OPT_PROXY_AUTHSTRING_REG = wxT("proxyauthstring");


    const wxString EXT = wxT(".dfast");
    #ifdef __WXMSW__
        const wxString PREFIX = wxT("");
    #else
        #ifdef WXDFAST_PORTABLE
        const wxString PREFIX = wxT("");
        #else
        const wxString PREFIX = wxT(".");
        #endif
    #endif
    const wxString LINE = wxT("-");
    const wxString CRLF = wxT("\r\n");
    const wxString ANONYMOUS_USER = wxT("anonymous");
    const wxString ANONYMOUS_PASS = wxT("anonymous@anonymous.com");

    const wxColor RED = wxColor(255,0,30);
    const wxColor BLUE = wxColor(0,47,94);
    const wxColor LIGHT_BLUE = wxColor(0,0,200);
    const wxColor GREEN = wxColor(0,98,65);
    const wxColor LIGHT_GREEN = wxColor(0,181,0);
    const wxColor YELLOW = wxColor(250,230,24);

    const wxString HTMLERROR = wxT("#FF0030");         //RED
    const wxString HTMLSERVER = wxT("#009865");        //GREEN
    const wxString HTMLNORMAL = wxT("#002f5e");        //BLUE
    const wxString HTMLBLACK = wxT("#000000");        //BLACK

    const wxString days[7] = {_("Sundays"),_("Mondays"),_("Tuesdays"),
                              _("Wednesdays"),_("Thursdays"),_("Fridays"),
                              _("Saturdays")};

    const int INPROGRESS_ICON01 = 0;
    const int INPROGRESS_ICON02 = 1;
    const int INPROGRESS_NAME = 2;
    const int INPROGRESS_SIZE = 3;
    const int INPROGRESS_COMPLETED = 4;
    const int INPROGRESS_PERCENTUAL = 5;
    const int INPROGRESS_TIMEPASSED = 6;
    const int INPROGRESS_TIMEREMAINING = 7;
    const int INPROGRESS_SPEED = 8;
    const int INPROGRESS_ATTEMPTS = 9;
    const int INPROGRESS_URL = 10;

    const int FINISHED_ICON01 = 0;
    const int FINISHED_NAME = 1;
    const int FINISHED_SIZE = 2;
    const int FINISHED_END = 3;

    #define HTTP                1
    #define FTP                 2
    #define LOCAL_FILE          3
    #define READ                4
    #define WRITE               5

    #define HIDE                1000
    #define NEW                 1001
    #define CLOSE               1002
    #define TIMER_ID            1003
    #define BAND                1004
    #define OFF                 1005
    #define ON                  1006
    #define PERDOWNLOAD         1007

    //mDownloadFile STATUS
    #define STATUS_STOPED               0
    #define STATUS_ACTIVE               1
    #define STATUS_FINISHED             2
    #define STATUS_ERROR                3
    #define STATUS_QUEUE                4
    #define STATUS_SCHEDULE_QUEUE       5


    //mDownloadFile RESTART
    #define YES                 0
    #define NO                  1

    class mMainFrame;
    class mUrlName;
    class mBoxNew;
    class mBoxOptions;
    class mInProgressList;
    class mFinishedList;
    class mTaskBarIcon;
    class mConnection;
    class mClient;
    class mServer;
    class mDownloadThread;
    class mGraph;
    class mProgressBar;
    class mMetalinkData;

    WX_DECLARE_OBJARRAY(int,mListSelection);
    WX_DEFINE_ARRAY(mDownloadThread *, mDownloadThreadArray);
    WX_DECLARE_LIST(mUrlName, mUrlList);

    class mDownloadFile
    {
    public:
        ~mDownloadFile()
        {
            if (this->urllist)
                delete this->urllist;
            //if (this->metalinkdata)
                //delete this->metalinkdata;
        };
        //FUNCTIONS
        void RegisterListItemOnDisk();
        void RemoveListItemFromDisk();
        int GetStatus();
        void SetAsActive();
        void SetAsStoped(bool stopschedule = TRUE);
        void SetAsFinished();
        void ErrorOccurred();
        void PutOnQueue();
        bool IsScheduled();
        void PutOnScheduleQueue();
        //int GetIndex();
        wxString GetName();
        wxString GetExposedName();
        void SetExposedName(wxString name);
        void UnSetExposedName();
        wxString GetDestination();
        wxString GetTemporaryDestination();
        wxString GetReferenceURL();
        wxString GetComments();
        wxString GetUser();
        wxString GetPassword();
        int RestartSupport();
        void SetRestartSupport(bool support = TRUE);
        wxString GetContentType();
        void SetContentType(wxString contenttype);
        bool IsMetalink();
        bool IsHtml();
        bool IsZip();
        int GetNumberofParts();
        int GetCurrentAttempt();
        void ResetAttempts();
        void IncrementAttempt();
        mUrlName GetFirstUrl();
        mUrlName GetNextUrl();
        bool AppendUrl(mUrlName *url);
        bool FindUrl(mUrlName url);
        int GetUrlCount();
        wxArrayString GetUrlArray();
        void SetFinishedDateTime(wxDateTime time);
        wxDateTime GetFinishedDateTime();
        void SetMD5(wxString md5);
        int GetProgress();
        void SetProgress(int percentual);
        bool IsSplitted();
        void Split(bool split);
        bool WaitingForSplit();
        void WaitSplit();
        bool WriteIsPending();
        void MarkWriteAsPending(bool pending);
        bool RemoveIsPending();
        void MarkRemoveAsPending(bool pending);
        void SetFree(bool free = TRUE);
        bool IsFree();
        void SetBandWidth(int band);
        int GetBandWidth();
        bool NeedToReGetMetalink();
        void SetToReGetMetalinkWhenNeeded(bool reget);
        void SetMetalinkFileIndex(int index);
        int GetMetalinkFileIndex();
        wxString GetCommand();
        void SetChangedSinceLastSave();

        //PUBLIC VARIABLES
        friend class mDownloadList;
        wxLongLong timepassed;
        wxLongLong timeremaining;
        wxLongLong totalsize;
        wxLongLong totalsizecompleted;
        long totalspeed;

        bool criticalerror;
        bool speedpoint;
        int speedpointowner;
        #ifndef DISABLE_MUTEX
        wxMutex *mutex_speedcalc;
        #endif

        bool finished[MAX_NUM_PARTS];
        wxLongLong startpoint[MAX_NUM_PARTS];
        wxLongLong size[MAX_NUM_PARTS];
        wxLongLong sizecompleted[MAX_NUM_PARTS];
        long delta_size[MAX_NUM_PARTS];
        int percentualparts[MAX_NUM_PARTS];
        wxString messages[MAX_NUM_PARTS];
        mMetalinkData *metalinkdata;
    private:
        int index;
        int status;
        bool split;
        bool waitbeforesplit;
        bool free;
        bool needtoregetmetalink;
        wxString name;
        wxString exposedname;
        wxString destination;
        wxString tempdestination;
        wxString reference;
        wxString comments;
        wxString user;
        wxString password;
        wxString MD5;
        int scheduled;
        int restart;
        wxString contenttype;
        mUrlList *urllist;
        unsigned int currenturl;
        int parts;
        int currentattempt;
        wxDateTime start;
        wxDateTime end;
        int percentual;
        bool writependig;
        bool removepending;
        int bandwidth;
        int metalinkindex;
        wxString command;
        bool changedsincelastsave;
    };

    WX_DECLARE_LIST(mDownloadFile, mDownloadListType);

    class mDownloadList: public mDownloadListType
    {
    public:
        void ChangePosition(mDownloadFile *file01, mDownloadFile *file02);
        mDownloadFile *NewDownloadRegister(mUrlList *urllist,wxFileName destination,wxFileName tempdestination, int metalinkindex, int parts, wxString user, wxString password,wxString reference, wxString comments,wxString command,int scheduled,int bandwidth,int ontop);
        void RemoveDownloadRegister(mDownloadFile *currentfile);
        void ChangeDownload(mDownloadFile *file, mUrlList *urllist,wxFileName destination, wxString user, wxString password, wxString reference, wxString comments,wxString command,int bandwidth);
        void ChangeName(mDownloadFile *file, wxString name, int value = 0);
        mDownloadFile *FindDownloadFile(wxString str);
        void LoadDownloadListFromDisk();
        void RecreateIndex();
        static int ListCompareByIndex(const mDownloadFile** arg1, const mDownloadFile** arg2);
        int GetNumberofActiveDownloads();
        void SetNumberofActiveDownloads(int number);
    private:
        int numberofactivedownloads;
    };

    WX_DECLARE_LIST(float, mGraphPoints);

    class mScheduleException
    {
    public:
        wxString start,finish;
        wxString newstart,newfinish;
        int day;    //0 - Sunday
        int newday; //1 - Monday ...
        int isactive;
    };

    class mOptions
    {
    public:
        wxString currentrelease;
        wxDateTime lastnewreleasecheck;
        int attempts;             //number of attempts
        bool closedialog;         //show the close dialog
        int simultaneous;         //number of simultaneous downloads
        int attemptstime;         //time between the attempts in seconds
        bool shutdown;
        bool disconnect;
        bool alwaysshutdown;
        bool alwaysdisconnect;
        int timerupdateinterval; //time between the timer refreshs in milliseconds
        long readbuffersize;
        bool restoremainframe;   //Restore the mainframe when all downloads are finished
        bool hidemainframe;      //Hide the mainframe when the user start a download
        bool checkforupdates;
        bool progressbarshow;
        bool graphshow;
        int graphhowmanyvalues;
        int graphrefreshtime;    //time between the graph refreshs in milliseconds
        int graphscale;          //max value showed in the graph
        int graphtextarea;       //size the area reserved for the speed value
        int graphheight;
        int graphspeedfontsize;
        wxColour graphbackcolor;
        wxColour graphgridcolor;
        wxColour graphlinecolor;
        int graphlinewidth;
        wxColour graphfontcolor;
        wxString shutdowncmd;
        wxString disconnectcmd;
        wxString destination;
        wxString browserpath;
        wxString filemanagerpath;
        wxString downloadpartsdefaultdir;
        bool activatescheduling;
        wxDateTime startdatetime;
        wxDateTime finishdatetime;
        mScheduleException scheduleexceptions[MAX_SCHEDULE_EXCEPTIONS];
        bool scheduleexceptionschanged;
        wxString lastcommand;
        wxString lastdestination;
        int lastnumberofparts;
        int laststartoption;
        int lastontopoption;
        bool rememberboxnewoptions;
        int bandwidthoption;
        long bandwidth;
        int taskbariconsize;
        int boxnew_x, boxnew_y;
        bool proxy;
        wxString proxy_server;
        wxString proxy_port;
        wxString proxy_username;
        wxString proxy_authstring;
    };

    class mApplication : public wxApp
    {
    public:
        mApplication();
        virtual ~mApplication();
        virtual bool OnInit();
        virtual int OnExit();
        bool NewInstance();
        mDownloadList downloadlist;
        mDownloadThreadArray *downloadthreads;
        static wxString Configurations(int operation, wxString option, wxString value);
        static int Configurations(int operation, wxString option, int value);
        static long Configurations(int operation, wxString option,long value);
        mMainFrame *mainframe;
        wxCmdLineParser *parameters;
        mServer *m_server;
        wxSocketBase *dummy;
        mConnection *connection;
        wxSingleInstanceChecker *m_checker;
        wxCriticalSection m_critsect;
        wxMutex m_mutexAllDone;
        wxCondition m_condAllDone;
        bool m_waitingUntilAllDone;
        void SetLanguage(int language);
        wxString themepath;
        wxIcon appicon;
        #ifdef __WXMSW__
        wxString programvolume;
        #endif
    private:
        wxLocale *m_locale;
    };

    DECLARE_APP(mApplication)

    class mMainFrame : public wxFrame
    {
    public:
        mMainFrame();
        ~mMainFrame();
        void OnTimer(wxTimerEvent& event);
        bool NewDownload(wxArrayString url, wxString destination,int metalinkindex, int parts,wxString user,wxString password,wxString reference,wxString comments,wxString command,int startoption, bool ontop, bool show,bool permitdifferentnames);
        bool StartDownload(mDownloadFile *downloadfile);
        void StopDownload(mDownloadFile *downloadfile,bool stopschedule = TRUE);
        void OnNew(wxCommandEvent& event);
        void OnRemove(wxCommandEvent& event);
        void OnSchedule(wxCommandEvent& event);
        void OnStart(wxCommandEvent& event);
        void OnStop(wxCommandEvent& event);
        void OnStartAll(wxCommandEvent& event);
        void OnStopAll(wxCommandEvent& event);
        void OnPasteURL(wxCommandEvent& event);
        void OnCopyURL(wxCommandEvent& event);
        void OnSelectAll(wxCommandEvent& event);
        void OnInvertSelection(wxCommandEvent& event);
        void OnFind(wxCommandEvent& event);
        void OnShowProgressBar(wxCommandEvent& event);
        void OnShowGraph(wxCommandEvent& event);
        void ShowHideResizeGraph(int oldgraphheight);
        void OnDetails(wxCommandEvent& event);
        //Languages
        void MarkCurrentLanguageMenu(int language);
        void SetLanguage(int language);
        void OnDefaultLanguage(wxCommandEvent& event);
        void OnEnglish(wxCommandEvent& event);
        void OnPortuguese(wxCommandEvent& event);
        void OnPortugueseBrazil(wxCommandEvent& event);
        void OnGerman(wxCommandEvent& event);
        void OnSpanish(wxCommandEvent& event);
        void OnCzech(wxCommandEvent& event);
        void OnHungarian(wxCommandEvent& event);
        void OnRussian(wxCommandEvent& event);
        void OnIndonesian(wxCommandEvent& event);
        void OnArmenian(wxCommandEvent& event);
        void OnPolish(wxCommandEvent& event);
        void OnTurkish(wxCommandEvent& event);
        void OnFrench(wxCommandEvent& event);
        void OnDutch(wxCommandEvent& event);

        void OnProperties(wxCommandEvent& event);
        void OnMove(wxCommandEvent& event);
        void OnDownloadAgain(wxCommandEvent& event);
        void OnCheckMD5(wxCommandEvent& event);
        void OnOpenDestination(wxCommandEvent& event);
        void OnCopyDownloadData(wxCommandEvent& event);
        void OnExportConf(wxCommandEvent& event);
        void OnImportConf(wxCommandEvent& event);
        bool ImportConf(wxString path);
        bool ExportConf(wxString dir);
        void OnShutdown(wxCommandEvent& event);
        void OnDisconnect(wxCommandEvent& event);
        void OnOptions(wxCommandEvent& event);
        void OnIconize(wxIconizeEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnSite(wxCommandEvent& event);
        void OnBug(wxCommandEvent& event);
        void OnDonate(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& WXUNUSED(event));
        void BrowserFile();
        void OnToolLeftClick(wxCommandEvent& event);
        void OnToolMouseMove(wxCommandEvent& event);
        void OnUpDown(bool up);
        void OnOpenURL(wxCommandEvent& event);
        void OnShutdownEvent(wxCommandEvent& event);
        void OnDisconnectEvent(wxCommandEvent& event);
        void OnExecuteEvent(wxCommandEvent& event);
        bool UpdateListItemField(mDownloadFile *current);
        void CheckNewRelease();
        void OnNewRelease(wxCommandEvent& event);
        void OnFilePreview(wxCommandEvent& event);
        void OnNewDownloadEvent(wxCommandEvent& event);
        mTaskBarIcon *taskbaricon;
        mProgressBar *progressbar;
        mGraph *graph;
        wxMenuBar *menubar;
        wxToolBar *toolbar;
        wxStatusBar *statusbar;
        wxMenu *menupopup;
        mOptions programoptions;
        #ifndef DISABLE_MUTEX
        wxMutex *mutex_programoptions;
        #endif
        mGraphPoints graphpoints;
        int timerinterval;
        int writetimerinterval;
        bool active;
        wxString defaultstatusbarmessage;
    private:
        int completed[MAX_NUM_PARTS];
        wxTimer *mtimer;
        wxImageList *imageslist;
        DECLARE_EVENT_TABLE()
    };

    class mGraph : public wxPanel
    {
    public:
        mGraph();
        bool Hide();
        bool Show(bool show = TRUE);
        void OnPaint(wxPaintEvent &event);
        void SetMainFrame(mMainFrame *mainframe);
        mGraphPoints *graphpoints;
        DECLARE_DYNAMIC_CLASS(mGraph)
    private:
        mMainFrame *mainframe;
        mOptions *programoptions;
        DECLARE_EVENT_TABLE()
    };

    class mProgressBar : public wxPanel
    {
    public:
        mProgressBar();
        void OnPaint(wxPaintEvent &event);
        bool Hide();
        bool Show(bool show = TRUE);
        void SetMainFrame(mMainFrame *mainframe);
        void SetParams(int parts,int *completed);
        DECLARE_DYNAMIC_CLASS(mProgressBar)
    private:
        mMainFrame *mainframe;
        int parts;
        int completed[MAX_NUM_PARTS];
        DECLARE_EVENT_TABLE()
    };


    class mTaskBarIcon: public wxTaskBarIcon
    {
    public:
        mTaskBarIcon(mMainFrame *frame);
        void OnLButtonClick(wxTaskBarIconEvent&);
        void OnMouseMove(wxTaskBarIconEvent&);
        void OnClose(wxCommandEvent& event);
        void OnHide(wxCommandEvent& event);
        void OnNew(wxCommandEvent& event);
        void OnBandUnlimited(wxCommandEvent& event);
        void OnBandControlOn(wxCommandEvent& event);
        void OnBandControlPerDownload(wxCommandEvent& event);
        virtual wxMenu *CreatePopupMenu();
        bool restoring;
        wxBitmap NewDownload;
        wxBitmap Quit;
    private:
        mMainFrame *mainframe;
        DECLARE_EVENT_TABLE()
    };

    class mBoxNew: public wxDialog
    {
    public:
        void OnOk(wxCommandEvent& event);
        void OnCancel(wxCommandEvent& event);
        void OnButtonDir(wxCommandEvent& event);
        void OnButtonAdd(wxCommandEvent& event);
        void OnButtonEdit(wxCommandEvent& event);
        int CheckURL(mUrlName url);
        bool PermitDifferentNames();
        void SetDifferentNamesPermition(bool permit);
    private:
        bool permitdifferentnames;
        DECLARE_EVENT_TABLE()
    };

    class mBoxOptions: public wxDialog
    {
    public:
        void OnOk(wxCommandEvent& event);
        void OnCancel(wxCommandEvent& event);
        void OnButtonDir(wxCommandEvent& event);
        void OnBrowserPath(wxCommandEvent& event);
        void OnFileManagerPath(wxCommandEvent& event);
        void OnTempPath(wxCommandEvent& event);
        void OnGraphBackgroundColour(wxCommandEvent& event);
        void OnGraphGridColour(wxCommandEvent& event);
        void OnGraphLineColour(wxCommandEvent& event);
        void OnGraphFontColour(wxCommandEvent& event);
        void OnButtonStartDate(wxCommandEvent& event);
        void OnButtonFinishDate(wxCommandEvent& event);
        void OnAdd(wxCommandEvent& event);
        void OnRemove(wxCommandEvent& event);
    private:
        DECLARE_EVENT_TABLE()
    };

    class mDatePicker: public wxDialog
    {
    public:
        mDatePicker(wxWindow* parent, wxWindowID id, const wxString& title, wxString date):
                wxDialog(parent, id, title)
        {
            //int wdate,hdate,wbtn,hbtn;
            wxDateTime tmpdate;
            tmpdate.ParseDate(date);
            m_datepicker = new wxCalendarCtrl(this,0,tmpdate,wxPoint(0,0),wxDefaultSize);
            m_btnok = new wxButton(this,wxID_OK,_("OK"));

            grid_sizer = new wxFlexGridSizer(2, 1, 0, 0);
            grid_sizer->Add(m_datepicker, 1, wxEXPAND, 0);
            grid_sizer->Add(m_btnok, 0, wxALIGN_RIGHT|wxADJUST_MINSIZE, 0);
            SetAutoLayout(true);
            SetSizer(grid_sizer);
            grid_sizer->Fit(this);
            grid_sizer->SetSizeHints(this);
            Layout();
            this->CentreOnParent();
        };
        ~mDatePicker()
        {
            delete m_datepicker;
            delete m_btnok;
        };
        void OnOk(wxCommandEvent& event)
        {
            m_selecteddate = m_datepicker->GetDate().Format(wxT("%Y/%m/%d"));
            EndModal(wxID_OK);
        };
        wxString GetSelectedDate()
        {
            return m_selecteddate;
        };
    private:
        wxFlexGridSizer* grid_sizer;
        wxCalendarCtrl *m_datepicker;
        wxButton *m_btnok;
        wxString m_selecteddate;
        DECLARE_EVENT_TABLE()
    };

    class mBoxOptionsColorPanel: public wxPanel
    {
    public:
        void OnPaint(wxPaintEvent &event);
        wxColor colour;
        DECLARE_DYNAMIC_CLASS(mBoxOptionsColorPanel)
    private:
        DECLARE_EVENT_TABLE()
    };

    class mBoxFind: public wxFindReplaceDialog {
    public:
        mBoxFind(wxWindow * parent, wxFindReplaceData* data, const wxString& title, int style = 0):
        wxFindReplaceDialog(parent, data, title, style){}

        void OnFind(wxFindDialogEvent& event);
        void OnClose(wxFindDialogEvent& event);
    private:
        DECLARE_EVENT_TABLE()
    };

    class mInProgressList : public wxListCtrl
    {
    public:
        mInProgressList();
        void OnRClick(wxListEvent& event);
        void OnDoubleClick(wxListEvent& event);
        void OnSelected(wxListEvent& event);
        void OnDeselected(wxListEvent& event);
        void OnLeaveWindow(wxMouseEvent& event);
        void OnEnterWindow(wxMouseEvent& event);
        void SelectUnselect(bool selected,int selection);
        int Insert(mDownloadFile *current, int item,bool ontop = false);
        mListSelection GetCurrentSelection();
        int GetCurrentLastSelection();
        void SetCurrentSelection(int selection);
        void RemoveItemListandFile(int item);
        void HandleSelectDeselectEvents(bool value);
        void GenerateList(wxImageList *imageslist);
        mMainFrame *mainframe;
         DECLARE_DYNAMIC_CLASS(mInProgressList)
    private:
        int lastselection;
        bool handleselectdeselectevents;
        DECLARE_EVENT_TABLE()
    };

    class mFinishedList : public wxListCtrl
    {
    public:
        void OnRClick(wxListEvent& event);
        void OnDoubleClick(wxListEvent& event);
        void OnSelected(wxListEvent& event);
        void OnDeselected(wxListEvent& event);
        void OnLeaveWindow(wxMouseEvent& event);
        void OnEnterWindow(wxMouseEvent& event);
        void SelectUnselect(bool selected,int selection);
        mListSelection GetCurrentSelection();
        int GetCurrentLastSelection();
        void SetCurrentSelection(int selection);
        void GenerateList(wxImageList *imageslist);
        static int wxCALLBACK CompareDates(long item1, long item2, long WXUNUSED(sortData));
        mMainFrame *mainframe;
        DECLARE_DYNAMIC_CLASS(mFinishedList)
    private:
        int lastselection;
        DECLARE_EVENT_TABLE()
    };

    class mNotebook : public wxNotebook
    {
    public:
        void ReSetPagesLabel();
        void OnChangePage(wxNotebookEvent& event);
        DECLARE_DYNAMIC_CLASS(mNotebook)
    private:
        wxString labelpage01;
        wxString labelpage02;
        DECLARE_EVENT_TABLE()
    };

    class mUrlName : public wxURI
    {
    public:
        mUrlName();
        mUrlName(wxString uri);
        //~mUrlName();
        wxString GetHost();
        wxString GetPort();
        wxString GetDir();
        wxString GetFullName();
        wxString GetFullRealName();
        wxString GetFullPath();
        wxString GetUrlUser();
        wxString GetUrlPassword();
        int Type();
        bool IsComplete();
    };

    class mFTP: public wxFTP
    {
    public:
        wxLongLong GetFileSize(const wxString& fileName);
        wxInputStream *GetInputStream(const wxString& path);
    };

    class mHTTP: public wxHTTP
    {
    public:
        mHTTP();
        wxString BuildGetRequest(mUrlName url,wxLongLong start);
        void SendGetRequest();
        wxString GetResponseMessage();
        bool Connect(wxSockAddress& addr, bool wait);
        int GetResponse() { return m_http_response; }
        int GetCompleteResponse() { return m_http_complete_response; }
        void UseProxy(wxString proxy_authstring);
    private:
        bool ParseHeaders();
        char *wxstr2str(wxString wxstr);
        wxString m_headersmsg;
        wxString m_getcommand;
        wxString m_messagereceived;
        wxString path;
        int m_http_complete_response;
        bool m_use_proxy;
        wxString m_proxy_authstring;
    };

    class mConnection: public wxConnection
    {
    public:
        bool OnExecute(const wxString& topic, wxChar* data, int size, wxIPCFormat format);
    };

    class mClient: public wxClient
    {
    public:
        wxConnectionBase *OnMakeConnection(void) { return new mConnection; }
    };

    class mServer: public wxServer
    {
    public:
        wxConnectionBase *OnAcceptConnection(const wxString& topic);
    };

    class mDownloadThread : public wxThread
    {
    public:
        mDownloadThread(mDownloadFile *file, int index);
        // thread execution starts here
        virtual void *Entry();

        // called when the thread exits - whether it terminates normally or is
        // stopped with Delete() (but not when it is Kill()ed!)
        virtual void OnExit();
        mUrlName CheckHtmlFile(bool downloaded = FALSE);
        wxLongLong CurrentSize(wxString filepath,wxString filename);
        int FinishDownload(wxFileName *destination,wxFileName tempdestination);
        wxSocketClient *ConnectHTTP(wxLongLong *start);
        wxSocketClient *ConnectFTP(wxLongLong *start);
        wxInputStream *ConnectLOCAL_FILE(wxLongLong start);
        int  DownloadPart(wxSocketClient *connection, wxInputStream *filestream, wxLongLong start, wxLongLong end);
                        //the parameter wxInputStream *filestream is used just for LOCAL_FILE
        void PrintMessage(wxString str,wxString color=HTMLNORMAL);
        void WaitUntilAllFinished(bool canstop = TRUE);
        bool JoinFiles(wxFileName *destination,wxFileName tempdestination);
        void SpeedCalculation(long delta_t);
        mDownloadFile *downloadfile;
        mOptions *programoptions;
        int downloadpartindex;
        mUrlName currenturl;
        bool redirecting;
        mDownloadList *downloadlist;
        wxLongLong realtotalsize_copy;
        mUrlName *proxy_address;
        bool proxy;
        wxString proxy_authstring;
    };

    class mCheckNewReleaseThread : public wxThread
    {
    public:
        mCheckNewReleaseThread();
        // thread execution starts here
        virtual void *Entry();

        // called when the thread exits - whether it terminates normally or is
        // stopped with Delete() (but not when it is Kill()ed!)
        virtual void OnExit();
    };

    class mMetalinkData
    {
    public:
        wxString publishername;
        wxString publisherurl;
        wxString description;
        wxString filename;
        wxString version;
        wxLongLong size;
        wxString language;
        wxString os;
        wxString md5;
        wxString sha1;
        mUrlList urllist;
        void Clear();
    };

    class mMetalinkDocument : public wxXmlDocument
    {
    public:
        int GetMetalinkData(mMetalinkData *data,int index);
    private:
        int GetFileData(mMetalinkData *data,wxXmlNode *subnode);
        wxString GetContent(wxXmlNode *children);
    };

    class MyUtilFunctions
    {
    public:
        static wxString int2wxstr(long value,int format = 0);
        static wxString TimeString(long value);
        static wxString TimeString(wxLongLong value);
        static wxString GetLine(wxString text, int line);
        static wxString str2wxstr(char *str);
        static wxString str2wxstr(char str);
        static wxString ByteString(long size);
        static wxString ByteString(wxLongLong size);
        static wxLongLong wxstrtolonglong(wxString string);
        static double wxlonglongtodouble(wxLongLong value);
        static wxString GenerateAuthString(wxString user, wxString pass);
        #ifdef __WXMSW__
        static wxString GetProgramFilesDir();
        static wxString GetMyDocumentsDir();
        static wxString GetDefaultBrowser();
        #endif
    };
#endif
