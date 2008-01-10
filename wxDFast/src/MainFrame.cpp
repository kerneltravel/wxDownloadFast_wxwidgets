//
// C++ Implementation: MainFrame
//
// Description: Implements the mainframe events, including menus and the toolbar.
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"

IMPLEMENT_DYNAMIC_CLASS(mNotebook, wxNotebook)
IMPLEMENT_DYNAMIC_CLASS(mBoxOptionsColorPanel, wxPanel)

////////////////////////XPM IMAGES////////////////////////////////
/*#ifndef __WXMSW__
#include "../resources/xpm/wxdfast.xpm"
#endif
#include "../resources/xpm/small/stop.xpm"
#include "../resources/xpm/small/start.xpm"
#include "../resources/xpm/small/ok.xpm"
#include "../resources/xpm/small/error.xpm"
#include "../resources/xpm/small/queue.xpm"
#include "../resources/xpm/small/schedule.xpm"
#include "../resources/xpm/small/new.xpm"
#include "../resources/xpm/small/exit.xpm"*/

const wxEventType wxEVT_OPEN_URL = wxNewEventType();
#define wxEVT_OPEN_URL(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_OPEN_URL, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxCommandEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

const wxEventType wxEVT_SHUTDOWN = wxNewEventType();
#define wxEVT_SHUTDOWN(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_SHUTDOWN, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxCommandEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

const wxEventType wxEVT_DISCONNECT = wxNewEventType();
#define wxEVT_DISCONNECT(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_DISCONNECT, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxCommandEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

const wxEventType wxEVT_NEW_RELEASE = wxNewEventType();
#define wxEVT_NEW_RELEASE(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_NEW_RELEASE, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxCommandEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

const wxEventType wxEVT_NEW_DOWNLOAD = wxNewEventType();
#define wxEVT_NEW_DOWNLOAD(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_NEW_DOWNLOAD, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxCommandEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

const wxEventType wxEVT_EXECUTE_COMMAND = wxNewEventType();
#define wxEVT_EXECUTE_COMMAND(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_EXECUTE_COMMAND, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxCommandEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

BEGIN_EVENT_TABLE(mMainFrame,wxFrame)
    EVT_MENU(XRCID("menunew"),  mMainFrame::OnNew)
    EVT_MENU(XRCID("menuremove"),  mMainFrame::OnRemove)
    EVT_MENU(XRCID("menuschedule"),  mMainFrame::OnSchedule)
    EVT_MENU(XRCID("menustart"),  mMainFrame::OnStart)
    EVT_MENU(XRCID("menustop"),  mMainFrame::OnStop)
    EVT_MENU(XRCID("menustartall"),  mMainFrame::OnStartAll)
    EVT_MENU(XRCID("menustopall"),  mMainFrame::OnStopAll)
    EVT_MENU(XRCID("menuexit"),  mMainFrame::OnExit)
    EVT_MENU(XRCID("menusite"),  mMainFrame::OnSite)
    EVT_MENU(XRCID("menubug"),  mMainFrame::OnBug)
    EVT_MENU(XRCID("menudonate"),  mMainFrame::OnDonate)
    EVT_MENU(XRCID("menuabout"), mMainFrame::OnAbout)
    EVT_MENU(XRCID("menulang_default"), mMainFrame::OnDefaultLanguage)
    EVT_MENU(XRCID("menulang_ent"), mMainFrame::OnEnglish)
    EVT_MENU(XRCID("menulang_pt"), mMainFrame::OnPortuguese)
    EVT_MENU(XRCID("menulang_ptbr"), mMainFrame::OnPortugueseBrazil)
    EVT_MENU(XRCID("menulang_de"), mMainFrame::OnGerman)
    EVT_MENU(XRCID("menulang_es"), mMainFrame::OnSpanish)
    EVT_MENU(XRCID("menulang_cs"), mMainFrame::OnCzech)
    EVT_MENU(XRCID("menulang_hu"), mMainFrame::OnHungarian)
    EVT_MENU(XRCID("menulang_ru"), mMainFrame::OnRussian)
    EVT_MENU(XRCID("menulang_id"), mMainFrame::OnIndonesian)
    EVT_MENU(XRCID("menulang_hy"), mMainFrame::OnArmenian)
    EVT_MENU(XRCID("menulang_pl"), mMainFrame::OnPolish)
    EVT_MENU(XRCID("menulang_tr"), mMainFrame::OnTurkish)
    EVT_MENU(XRCID("menulang_fr"), mMainFrame::OnFrench)
    EVT_MENU(XRCID("menulang_nl"), mMainFrame::OnDutch)
    EVT_MENU(XRCID("menushowgraph"), mMainFrame::OnShowGraph)
    EVT_MENU(XRCID("menushowprogressbar"), mMainFrame::OnShowProgressBar)
    EVT_MENU(XRCID("menudetails"), mMainFrame::OnDetails)
    EVT_MENU(XRCID("menupaste"), mMainFrame::OnPasteURL)
    EVT_MENU(XRCID("menuoptions"), mMainFrame::OnOptions)
    EVT_MENU(XRCID("menuproperties"), mMainFrame::OnProperties)
    EVT_MENU(XRCID("menuselectall"), mMainFrame::OnSelectAll)
    EVT_MENU(XRCID("menuinvertselection"), mMainFrame::OnInvertSelection)
    EVT_MENU(XRCID("menufind"), mMainFrame::OnFind)
    EVT_MENU(XRCID("menuagain"), mMainFrame::OnDownloadAgain)
    EVT_MENU(XRCID("menumove"), mMainFrame::OnMove)
    EVT_MENU(XRCID("menumd5"), mMainFrame::OnCheckMD5)
    EVT_MENU(XRCID("menuopendestination"), mMainFrame::OnOpenDestination)
    EVT_MENU(XRCID("menucopyurl"), mMainFrame::OnCopyURL)
    EVT_MENU(XRCID("menucopydownloaddata"), mMainFrame::OnCopyDownloadData)
    EVT_MENU(XRCID("menuexport"), mMainFrame::OnExportConf)
    EVT_MENU(XRCID("menuimport"), mMainFrame::OnImportConf)
    EVT_MENU(XRCID("menushutdown"), mMainFrame::OnShutdown)
    EVT_MENU(XRCID("menudisconnect"), mMainFrame::OnDisconnect)
    EVT_BUTTON(XRCID("btnpreview"), mMainFrame::OnFilePreview)
    wxEVT_OPEN_URL(wxID_ANY,mMainFrame::OnOpenURL)
    wxEVT_SHUTDOWN(wxID_ANY, mMainFrame::OnShutdownEvent)
    wxEVT_DISCONNECT(wxID_ANY, mMainFrame::OnDisconnectEvent)
    wxEVT_EXECUTE_COMMAND(wxID_ANY, mMainFrame::OnExecuteEvent)
    wxEVT_NEW_RELEASE(wxID_ANY, mMainFrame::OnNewRelease)
    wxEVT_NEW_DOWNLOAD(wxID_ANY, mMainFrame::OnNewDownloadEvent)
    EVT_TOOL(-1, mMainFrame::OnToolLeftClick)
    EVT_TOOL_ENTER(-1, mMainFrame::OnToolMouseMove)
    EVT_ICONIZE(mMainFrame::OnIconize)
    EVT_TIMER(TIMER_ID, mMainFrame::OnTimer)
    EVT_CLOSE(mMainFrame::OnClose)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(mNotebook,wxNotebook)
    EVT_NOTEBOOK_PAGE_CHANGED(XRCID("notebook01"),  mNotebook::OnChangePage)
END_EVENT_TABLE()

void mNotebook::ReSetPagesLabel()
{
    SetPageText(0,wxGetTranslation(_("Downloads in Progress")));
    SetPageText(1,wxGetTranslation(_("Finished Downloads")));
}

void mNotebook::OnChangePage(wxNotebookEvent& event)
{
    int oldselection = event.GetOldSelection();
    if (oldselection == 0)
    {
        XRCCTRL(*(wxGetApp().mainframe), "inprogresslist",mInProgressList )->SelectUnselect(FALSE,-1);
        XRCCTRL(*(wxGetApp().mainframe), "finishedlist",mFinishedList )->SelectUnselect(FALSE,-1);
    }
    else if (oldselection == 1)
    {
        XRCCTRL(*(wxGetApp().mainframe), "finishedlist",mFinishedList )->SelectUnselect(FALSE,-1);
        XRCCTRL(*(wxGetApp().mainframe), "inprogresslist",mInProgressList )->SelectUnselect(FALSE,-1);
    }
    if (event.GetSelection() == 1)
    {
        XRCCTRL(*(wxGetApp().mainframe), "finishedlist",mFinishedList )->SortItems(mFinishedList::CompareDates, 0l);
    }
    event.Skip();
};

mMainFrame::mMainFrame()
{
    imageslist = new wxImageList(16, 16, TRUE);
    wxBitmap image[6];
    int i,timerupdateinterval;

    wxString iconpath = wxGetApp().themepath;
    image[0] = wxXmlResource::Get()->LoadBitmap(wxT("stop_png"));
    image[1] = wxXmlResource::Get()->LoadBitmap(wxT("start_png"));
    image[2] = wxXmlResource::Get()->LoadBitmap(wxT("completed_png"));
    image[3] = wxXmlResource::Get()->LoadBitmap(wxT("error_png"));
    image[4] = wxXmlResource::Get()->LoadBitmap(wxT("scheduled_png"));
    image[5] = wxXmlResource::Get()->LoadBitmap(wxT("schedule_png"));

    for (i=0;i<=5;i++)
       imageslist->Add(image[i]);
    wxXmlResource::Get()->LoadFrame(this,NULL, wxT("mainframe"));
    SetSize(-1,-1,600,400);
    SetTitle(PROGRAM_NAME);

    //LOAD USER OPTIONS
    programoptions.currentrelease = mApplication::Configurations(READ,OPT_CURRENT_RELEASE,VERSION);
    programoptions.lastnewreleasecheck.Set((time_t)mApplication::Configurations(READ,OPT_LAST_NEW_RELEASE_CHECK,0));
    programoptions.closedialog = mApplication::Configurations(READ,OPT_DIALOG_CLOSE_REG,1);
    programoptions.rememberboxnewoptions = mApplication::Configurations(READ,OPT_REMEMBER_BOXNEW_OPTIONS_REG,1);
    #ifdef __WXMSW__
        #ifdef WXDFAST_PORTABLE
            programoptions.destination = MyUtilFunctions::GetMyDocumentsDir();
            programoptions.filemanagerpath = wxGetOSDirectory() + wxT("\\explorer.exe");
            programoptions.browserpath = MyUtilFunctions::GetDefaultBrowser();
            programoptions.shutdowncmd = wxGetOSDirectory() + wxT("\\system32\\shutdown.exe -s -t 0");
            programoptions.disconnectcmd = wxGetOSDirectory() + wxT("\\system32\\rasdial.exe /disconnect");
        #else
            programoptions.destination = mApplication::Configurations(READ,OPT_DESTINATION_REG,MyUtilFunctions::GetMyDocumentsDir());
            programoptions.filemanagerpath = mApplication::Configurations(READ,OPT_FILE_MANAGER_PATH_REG,wxGetOSDirectory() + wxT("\\explorer.exe"));
            programoptions.browserpath = mApplication::Configurations(READ,OPT_BROWSER_PATH_REG,wxEmptyString);
            if (programoptions.browserpath.IsEmpty())
                programoptions.browserpath = MyUtilFunctions::GetDefaultBrowser();
            //defaultbrowserpath = MyUtilFunctions::GetProgramFilesDir() + wxT("\\Mozilla Firefox\\firefox.exe");
            programoptions.shutdowncmd = mApplication::Configurations(READ,OPT_SHUTDOWN_CMD_REG,wxGetOSDirectory() + wxT("\\system32\\shutdown.exe -s -t 0"));
            programoptions.disconnectcmd = mApplication::Configurations(READ,OPT_DISCONNECT_CMD_REG,wxGetOSDirectory() + wxT("\\system32\\rasdial.exe /disconnect"));
        #endif
    #else
        #ifdef WXDFAST_PORTABLE
            programoptions.destination = wxGetHomeDir();
        #else
            programoptions.destination = mApplication::Configurations(READ,OPT_DESTINATION_REG,wxGetHomeDir());
        #endif
        programoptions.filemanagerpath = mApplication::Configurations(READ,OPT_FILE_MANAGER_PATH_REG,wxT("/usr/bin/nautilus"));
        programoptions.browserpath = mApplication::Configurations(READ,OPT_BROWSER_PATH_REG,wxT("/usr/bin/firefox"));
        programoptions.shutdowncmd = mApplication::Configurations(READ,OPT_SHUTDOWN_CMD_REG,wxT("sudo /sbin/shutdown -h now"));
        programoptions.disconnectcmd = mApplication::Configurations(READ,OPT_DISCONNECT_CMD_REG,wxT("/usr/bin/poff"));
    #endif
    #ifdef WXDFAST_PORTABLE
        programoptions.downloadpartsdefaultdir = wxT("partial");
        if (!wxFileName::DirExists(programoptions.downloadpartsdefaultdir))
            wxFileName::Mkdir(programoptions.downloadpartsdefaultdir); //CREATE THE PARTIAL DIRECTORY
    #else
        programoptions.downloadpartsdefaultdir = mApplication::Configurations(READ,OPT_DOWNLOAD_PARTS_DEFAULT_DIR_REG,wxGetHomeDir());
    #endif
    programoptions.attempts = mApplication::Configurations(READ,OPT_ATTEMPTS_REG,999);
    programoptions.attemptstime = mApplication::Configurations(READ,OPT_ATTEMPTS_TIME_REG,5);
    programoptions.simultaneous = mApplication::Configurations(READ,OPT_SIMULTANEOUS_REG,5);
    programoptions.alwaysshutdown = mApplication::Configurations(READ,OPT_SHUTDOWN_REG,0);
    programoptions.shutdown = programoptions.alwaysshutdown;
    programoptions.alwaysdisconnect = mApplication::Configurations(READ,OPT_DISCONNECT_REG,0);
    programoptions.timerupdateinterval = mApplication::Configurations(READ,OPT_TIMERINTERVAL_REG,500);
    programoptions.readbuffersize = mApplication::Configurations(READ,OPT_READBUFFERSIZE_REG,1024);
    programoptions.disconnect = programoptions.alwaysdisconnect;
    programoptions.restoremainframe = mApplication::Configurations(READ,OPT_RESTORE_MAINFRAME_REG, 1);
    programoptions.hidemainframe = mApplication::Configurations(READ,OPT_HIDE_MAINFRAME_REG, 0);
    programoptions.checkforupdates = mApplication::Configurations(READ,OPT_CHECK_FOR_UPDATES_REG, 1);
    programoptions.progressbarshow = mApplication::Configurations(READ,OPT_PROGRESS_BAR_SHOW_REG, 1);
    programoptions.graphshow = mApplication::Configurations(READ,OPT_GRAPH_SHOW_REG, 1);
    programoptions.graphhowmanyvalues = mApplication::Configurations(READ,OPT_GRAPH_HOWMANYVALUES_REG, 300);
    programoptions.graphrefreshtime = mApplication::Configurations(READ,OPT_GRAPH_REFRESHTIME_REG, 1000);
    programoptions.graphscale = mApplication::Configurations(READ,OPT_GRAPH_SCALE_REG, 200);
    programoptions.graphtextarea = mApplication::Configurations(READ,OPT_GRAPH_TEXTAREA_REG, 80);
    programoptions.graphspeedfontsize = mApplication::Configurations(READ,OPT_GRAPH_SPEEDFONTSIZE_REG, 18);
    programoptions.graphheight = mApplication::Configurations(READ,OPT_GRAPH_HEIGHT_REG, 48);
    programoptions.graphlinewidth = mApplication::Configurations(READ,OPT_GRAPH_LINEWIDTH_REG, 3);
    {
        wxString colour;
        long red,green,blue;
        colour = mApplication::Configurations(READ,OPT_GRAPH_COLORBACK_REG, wxT(""));
        if (!colour.Mid(0,3).ToLong(&red))            red = wxBLACK->Red();
        if (!colour.Mid(4,3).ToLong(&green))        green = wxBLACK->Green();
        if (!colour.Mid(8,3).ToLong(&blue))            blue = wxBLACK->Blue();
        programoptions.graphbackcolor.Set(red,green,blue);
    }
    {
        wxString colour;
        long red,green,blue;
        colour = mApplication::Configurations(READ,OPT_GRAPH_COLORGRID_REG, wxT(""));
        if (!colour.Mid(0,3).ToLong(&red))            red = wxGREEN->Red();
        if (!colour.Mid(4,3).ToLong(&green))        green = wxGREEN->Green();
        if (!colour.Mid(8,3).ToLong(&blue))            blue = wxGREEN->Blue();
        programoptions.graphgridcolor.Set(red,green,blue);
    }
    {
        wxString colour;
        long red,green,blue;
        colour = mApplication::Configurations(READ,OPT_GRAPH_COLORLINE_REG, wxT(""));
        if (!colour.Mid(0,3).ToLong(&red))            red = wxRED->Red();
        if (!colour.Mid(4,3).ToLong(&green))        green = wxRED->Green();
        if (!colour.Mid(8,3).ToLong(&blue))            blue = wxRED->Blue();
        programoptions.graphlinecolor.Set(red,green,blue);
    }
    {
        wxString colour;
        long red,green,blue;
        colour = mApplication::Configurations(READ,OPT_GRAPH_COLORFONT_REG, wxT(""));
        if (!colour.Mid(0,3).ToLong(&red))            red = wxBLUE->Red();
        if (!colour.Mid(4,3).ToLong(&green))        green = wxBLUE->Green();
        if (!colour.Mid(8,3).ToLong(&blue))            blue = wxBLUE->Blue();
        programoptions.graphfontcolor.Set(red,green,blue);
    }
    programoptions.activatescheduling = mApplication::Configurations(READ,OPT_SCHED_ACTIVATESCHEDULING_REG,0);
    programoptions.startdatetime.Set((time_t)mApplication::Configurations(READ,OPT_SCHED_STARTDATETIME_REG,(long)wxDateTime::Now().GetTicks()));
    programoptions.finishdatetime.Set((time_t)mApplication::Configurations(READ,OPT_SCHED_FINISHDATETIME_REG,(long)wxDateTime::Now().GetTicks()));
    programoptions.scheduleexceptionschanged = FALSE;
    for (i=0;i<MAX_SCHEDULE_EXCEPTIONS;i++)
    {
        programoptions.scheduleexceptions[i].isactive = mApplication::Configurations(READ,OPT_SCHED_SCHEDULEEXCEPTION_ISACTIVE_REG+MyUtilFunctions::int2wxstr(i),0);
        programoptions.scheduleexceptions[i].start = mApplication::Configurations(READ,OPT_SCHED_SCHEDULEEXCEPTION_START_REG+MyUtilFunctions::int2wxstr(i),wxT("00:00"));
        programoptions.scheduleexceptions[i].finish = mApplication::Configurations(READ,OPT_SCHED_SCHEDULEEXCEPTION_FINISH_REG+MyUtilFunctions::int2wxstr(i),wxT("00:00"));
        programoptions.scheduleexceptions[i].day = mApplication::Configurations(READ,OPT_SCHED_SCHEDULEEXCEPTION_DAY_REG+MyUtilFunctions::int2wxstr(i),0);
        programoptions.scheduleexceptions[i].newday = 0;
        programoptions.scheduleexceptions[i].newstart = wxEmptyString;
        programoptions.scheduleexceptions[i].newfinish = wxEmptyString;
    }
    programoptions.boxnew_x = mApplication::Configurations(READ,OPT_LAST_BOXNEW_X_REG,200);
    programoptions.boxnew_y = mApplication::Configurations(READ,OPT_LAST_BOXNEW_Y_REG,200);
    programoptions.lastcommand = mApplication::Configurations(READ,OPT_LAST_COMMAND_REG,wxEmptyString);
    programoptions.lastdestination = mApplication::Configurations(READ,OPT_LAST_DESTINATION_REG,programoptions.destination);
    programoptions.lastnumberofparts = mApplication::Configurations(READ,OPT_LAST_NUMBER_OF_PARTS_REG,DEFAULT_NUM_PARTS);
    programoptions.laststartoption = mApplication::Configurations(READ,OPT_LAST_START_OPTION_REG,DEFAULT_START_OPTION);
    programoptions.lastontopoption = mApplication::Configurations(READ,OPT_LAST_ONTOP_OPTION_REG,DEFAULT_ONTOP_OPTION);
    programoptions.bandwidthoption = mApplication::Configurations(READ,OPT_BAND_WIDTH_OPTION_REG,0);
    programoptions.bandwidth = mApplication::Configurations(READ,OPT_BAND_WIDTH_GENERAL_REG,20l);
    programoptions.taskbariconsize = mApplication::Configurations(READ,OPT_TASKBAR_ICON_SIZE_REG,32);
    programoptions.proxy = mApplication::Configurations(READ,OPT_PROXY_REG,0);
    programoptions.proxy_server = mApplication::Configurations(READ,OPT_PROXY_SERVER_REG,wxEmptyString);
    programoptions.proxy_port = mApplication::Configurations(READ,OPT_PROXY_PORT_REG,wxEmptyString);
    programoptions.proxy_username = mApplication::Configurations(READ,OPT_PROXY_USERNAME_REG,wxEmptyString);
    programoptions.proxy_authstring = mApplication::Configurations(READ,OPT_PROXY_AUTHSTRING_REG,wxEmptyString);

   //LOAD THE PROGRAM ICON
    wxBitmap tmpicon = wxXmlResource::Get()->LoadBitmap(wxT("wxdfast_png"));
    wxGetApp().appicon.CopyFromBitmap(tmpicon);

    SetIcon(wxGetApp().appicon);

    //SET THE PROGRESS BAR VARS
    progressbar = XRCCTRL(*(this), "progressbar",mProgressBar );
    progressbar->SetMainFrame(this);
    progressbar->SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    //SET THE GRAPH VARS
    graphpoints.DeleteContents(TRUE);
    graph = XRCCTRL(*(this), "graphpanel",mGraph );
    graph->graphpoints = &graphpoints;
    graph->SetMainFrame(this);
    graph->SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    //LOAD THE MENU BAR
    if ((menubar = wxXmlResource::Get()->LoadMenuBar(wxT("menubar"))))
        this->SetMenuBar(menubar);
    else
        menubar = NULL;

    //LOAD THE TOOL BAR
    if ((toolbar = wxXmlResource::Get()->LoadToolBar(this,wxT("toolbar"))))
        this->SetToolBar(toolbar);
    else
        toolbar = NULL;

    statusbar = this->GetStatusBar();

    //CHECK THE RIGHT LANGUAGE MENU
    MarkCurrentLanguageMenu(mApplication::Configurations(READ,LANGUAGE_REG,0));

    if (menubar)
    {
        menubar->GetMenu(3)->Check(XRCID("menushutdown"),programoptions.shutdown);
        menubar->GetMenu(3)->Check(XRCID("menudisconnect"),programoptions.disconnect);
    }
    timerupdateinterval = programoptions.timerupdateinterval;


    //SET NOTEBOOK TAB LABELS
    XRCCTRL(*(this), "notebook01",mNotebook )->ReSetPagesLabel();

    //GENERATE THE LISTS
    XRCCTRL(*this, "inprogresslist",mInProgressList )->mainframe = this;
    XRCCTRL(*this, "finishedlist",mFinishedList )->mainframe = this;
    XRCCTRL(*this, "inprogresslist",mInProgressList )->GenerateList(imageslist);
    XRCCTRL(*this, "finishedlist",mFinishedList )->GenerateList(imageslist);

    //CREATE THE POPUPMENU FOR THE LISTS
    menupopup = new wxMenu;
    wxMenuItem *schedule = new wxMenuItem(menupopup,XRCID("menuschedule"), _("Schedule Download"));
    wxMenuItem *start = new wxMenuItem(menupopup,XRCID("menustart"), _("Start Download"));
    wxMenuItem *stop = new wxMenuItem(menupopup,XRCID("menustop"), _("Stop Download"));
    wxMenuItem *remove = new wxMenuItem(menupopup,XRCID("menuremove"), _("Remove Download"));
    wxMenuItem *properties = new wxMenuItem(menupopup,XRCID("menuproperties"), _("Properties of Download"));
    wxMenuItem *downloadagain = new wxMenuItem(menupopup,XRCID("menuagain"), _("Download File Again"));
    wxMenuItem *move = new wxMenuItem(menupopup,XRCID("menumove"), _("Move File"));
    wxMenuItem *checkmd5 = new wxMenuItem(menupopup,XRCID("menumd5"), _("Check Integrity"));
    wxMenuItem *copyurl = new wxMenuItem(menupopup,XRCID("menucopyurl"), _("Copy URL"));
    wxMenuItem *copydownloaddata = new wxMenuItem(menupopup,XRCID("menucopydownloaddata"), _("Copy Download Data"));
    wxMenuItem *opendestination = new wxMenuItem(menupopup,XRCID("menuopendestination"), _("Open Destination Directory"));

    start->SetBitmap(image[1]);
    stop->SetBitmap(image[0]);
    schedule->SetBitmap(image[5]);
    remove->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("remove_png")));
    properties->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("properties_png")));
    copyurl->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("copyurl_png")));
    copydownloaddata->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("copydata_png")));
    menupopup->Append(schedule);
    menupopup->Append(start);
    menupopup->Append(stop);
    menupopup->Append(remove);
    menupopup->Append(properties);
    menupopup->AppendSeparator();
    menupopup->Append(copyurl);
    menupopup->Append(copydownloaddata);
    menupopup->Append(checkmd5);
    menupopup->Append(opendestination);
    menupopup->AppendSeparator();
    menupopup->Append(move);
    menupopup->Append(downloadagain);

    #ifndef DISABLE_MUTEX
    mutex_programoptions = new wxMutex();
    #endif

    //CREATE TASKBARICON
    this->active = TRUE;
    taskbaricon = new mTaskBarIcon(this);
    if (!(wxGetApp().parameters->Found(wxT("notray"))))
        taskbaricon->SetIcon(wxGetApp().appicon,PROGRAM_NAME);
    taskbaricon->restoring = FALSE;
    taskbaricon->NewDownload = wxXmlResource::Get()->LoadBitmap(wxT("new_png"));
    taskbaricon->Quit = wxXmlResource::Get()->LoadBitmap(wxT("quit_png"));

    //HIDE OR SHOW THE SPEED GRAPH
    if (!programoptions.graphshow)
        graph->Hide();
    else
        graph->Show();

    //HIDE OR SHOW THE PROGRESS BAR
    if (!programoptions.progressbarshow)
        progressbar->Hide();
    else
        progressbar->Show();

    //DEFINE THE STATUSBAR DEFAULT TEXT
    if (statusbar)
    {
        this->defaultstatusbarmessage = wxEmptyString;
        wxStringTokenizer newrelease(programoptions.currentrelease,wxT("."));
        wxStringTokenizer currentrelease(VERSION,wxT("."));
        long old01,old02,old03,new01,new02,new03;
        newrelease.GetNextToken().ToLong(&new01);
        currentrelease.GetNextToken().ToLong(&old01);
        newrelease.GetNextToken().ToLong(&new02);
        currentrelease.GetNextToken().ToLong(&old02);
        newrelease.GetNextToken().ToLong(&new03);
        currentrelease.GetNextToken().ToLong(&old03);
        if ((new01*100)+(new02*10)+(new03) > (old01*100)+(old02*10)+(old03))
        {
            this->defaultstatusbarmessage = _("NEW RELEASE: ");
            this->defaultstatusbarmessage += wxT("wxDownload Fast ");
            this->defaultstatusbarmessage += programoptions.currentrelease + _(" is available.");
        }
        statusbar->SetStatusText(this->defaultstatusbarmessage);
        if (programoptions.bandwidthoption == 1)
        {
            wxString temp = _("Band control");
            temp += wxT(": ");
            temp += _("Per Download");
            statusbar->SetStatusText(temp,2);
        }
        else if (programoptions.bandwidthoption == 2)
        {
            wxString temp = _("Band control");
            temp += wxT(": ");
            temp += _("Active");
            temp += wxT(" (") + MyUtilFunctions::int2wxstr(programoptions.bandwidth) + wxT(" kB/s)");
            statusbar->SetStatusText(temp,2);
        }
        else
        {
            wxString temp = _("Band control");
            temp += wxT(": ");
            temp += _("Unlimited");
            statusbar->SetStatusText(temp,2);
        }
    }

    //DEFINE THE PREVIEW PANEL TEXT
    {
        wxString contentstring =  _("File type");
        contentstring += wxT(":\n");
        XRCCTRL(*this, "lblpreview", wxStaticText )->SetLabel(contentstring);
        XRCCTRL(*this, "btnpreview", wxButton )->SetLabel(_("Preview"));
    }

    mtimer = new wxTimer(this, TIMER_ID);
    timerinterval = 0;
    writetimerinterval = 0;

    mtimer->Start(timerupdateinterval);
}

void mMainFrame::OnIconize(wxIconizeEvent& event)
{
    if ((wxGetApp().parameters->Found(wxT("notray"))))
           event.Skip();
    else
    {
        if ((event.Iconized()) && (taskbaricon->restoring == FALSE))
        {
            wxCommandEvent eventtmp;
            taskbaricon->OnHide(eventtmp);
        }
    }
}

mMainFrame::~mMainFrame()
{
    wxSplitterWindow *splitter01 = XRCCTRL(*this, "splitter01",wxSplitterWindow);
    wxSplitterWindow *splitter02 = XRCCTRL(*this, "splitter02",wxSplitterWindow);
    if (splitter01->IsSplit())
    {
        mApplication::Configurations(WRITE,SEPARATOR01_REG,splitter01->GetSashPosition());
        if (splitter01->IsSplit())
            mApplication::Configurations(WRITE,SEPARATOR02_REG,splitter02->GetSashPosition());
        mApplication::Configurations(WRITE,DETAILS_REG,TRUE);
    }
    else
        mApplication::Configurations(WRITE,DETAILS_REG,FALSE);
    if (!IsIconized())
    {
        int x,y,width,height,maximized;
        GetSize(&width,&height);
        GetPosition(&x,&y);
        maximized = IsMaximized();
        if (!maximized)
        {
            mApplication::Configurations(WRITE,SIZE_X_REG,width);
            mApplication::Configurations(WRITE,SIZE_Y_REG,height);
            mApplication::Configurations(WRITE,POS_X_REG,x);
            mApplication::Configurations(WRITE,POS_Y_REG,y);
        }
        mApplication::Configurations(WRITE,MAXIMIZED_REG,maximized);
    }

    delete taskbaricon;
    delete mtimer;
    delete imageslist;
    delete menupopup;
    //mutex_programoptions->Unlock();
    //delete mutex_programoptions;
}

void mMainFrame::OnTimer(wxTimerEvent& event)
{
    #ifndef DISABLE_MUTEX
    if (mutex_programoptions->TryLock() != wxMUTEX_NO_ERROR)
        return;
    #endif
    mInProgressList* list01 = XRCCTRL(*this, "inprogresslist",mInProgressList );
    mFinishedList* list02 = XRCCTRL(*this, "finishedlist",mFinishedList );
    int selection = list01->GetCurrentLastSelection();
    int simultaneous = programoptions.simultaneous;
    bool somedownloadfinishednow = FALSE,writeondisk = FALSE;
    long currentspeed = 0;
    int numberofactivedownloads = 0;
    mDownloadFile *current;
    #ifdef USE_HTML_MESSAGES
    if ((selection < 0) && (*(XRCCTRL(*(this), "messages",wxHtmlWindow)->GetParser()->GetSource()) != wxEmptyString))
        XRCCTRL(*(this), "messages",wxHtmlWindow)->SetPage(wxEmptyString);
    #else
    if ((selection < 0) && (XRCCTRL(*(this), "messages",wxTextCtrl)->GetValue() != wxEmptyString))
        XRCCTRL(*(this), "messages",wxTextCtrl)->Clear();
    #endif
    if ((selection < 0) &&  (((int)XRCCTRL(*(this), "treemessages",wxTreeCtrl)->GetCount()) > 0))
        XRCCTRL(*(this), "treemessages",wxTreeCtrl)->DeleteAllItems();
    if ((selection < 0) && (programoptions.progressbarshow))
        progressbar->SetParams(0,NULL);
    writetimerinterval += mtimer->GetInterval();
    if (writetimerinterval > 10000) //SAVE ALL DOWNLOAD STATUS ON DISK EVERY 10 SECONDS
    {
        writeondisk = TRUE;
        writetimerinterval = 0;
    }

    for ( mDownloadList::Node *node = wxGetApp().downloadlist.GetFirst(); node; node = node->GetNext() )
    {
        current = node->GetData();
        if ((current->GetStatus() == STATUS_FINISHED) && (current->IsFree()))
        {
            if (current->GetCommand() != wxEmptyString)
            {
                wxCommandEvent command(wxEVT_EXECUTE_COMMAND);
                command.SetString(current->GetCommand());
                GetEventHandler()->AddPendingEvent(command);
            }
            int i = list02->GetItemCount();
            int index = wxGetApp().downloadlist.IndexOf(current);
            mUrlName url(current->GetFirstUrl());
            list02->InsertItem(i, wxEmptyString);
            list02->SetItem(i, FINISHED_ICON01, wxEmptyString,current->GetStatus());
            list02->SetItem(i, FINISHED_NAME, current->GetName());
            list02->SetItem(i, FINISHED_SIZE, MyUtilFunctions::ByteString(current->totalsize));
            list02->SetItem(i, FINISHED_END, current->GetFinishedDateTime().Format());
            list02->SetItemData(i,current->GetFinishedDateTime().GetTicks());
            current->RegisterListItemOnDisk();
            if (index > 0)
                node = node->GetPrevious();  //GO TO THE PREVIOUS NODE BEFORE DELETE THE CURRENT ONE
            wxGetApp().downloadlist.RemoveDownloadRegister(current);
            list01->DeleteItem(index);
            list01->SelectUnselect(FALSE,-1);
            somedownloadfinishednow = TRUE;
            if (index > 0)  //IF THIS IS THE FIRST NODE, WE CANN'T GO TO THE PREVIOUS ONE
                continue;
            else
                break;
        }
        else
        {
            if (current->RemoveIsPending())
            {
                int index = wxGetApp().downloadlist.IndexOf(current);
                node = node->GetPrevious();
                current->RemoveListItemFromDisk();
                wxGetApp().downloadlist.RemoveDownloadRegister(current);
                list01->DeleteItem(index);
                list01->SelectUnselect(FALSE,-1);
                if (index > 0)
                    continue;
                else
                    break;
            }
            int parts = 1;
            if (current->IsSplitted())
                parts = current->GetNumberofParts();

            if ((current->WriteIsPending()) || (writeondisk))
                current->RegisterListItemOnDisk();

            //REFRESH THE LISTCTRL
            if (UpdateListItemField(current)) //TRUE IF CHANGE SPEED, TIME, ETC
            {
                //CALCULATE THE TOTAL SPEED
                currentspeed += current->totalspeed;
            }

            if (current->GetStatus() == STATUS_ACTIVE)
            {
                simultaneous--;
                numberofactivedownloads++;
            }

            if (selection == wxGetApp().downloadlist.IndexOf(current))
            {
                wxString contentstring = _("File type");
                contentstring += wxT(":\n") + current->GetContentType();
                XRCCTRL(*this, "lblpreview", wxStaticText )->SetLabel(contentstring);
                if (current->IsZip())
                    XRCCTRL(*this, "btnpreview", wxButton )->Enable(TRUE);

                long treeindex = 0;
                wxString piecestring = _("Piece ");
                if  (((int)XRCCTRL(*(this), "treemessages",wxTreeCtrl)->GetCount()) != parts)
                {
                    wxTreeCtrl *tree = XRCCTRL(*(this), "treemessages",wxTreeCtrl);
                    wxTreeItemId root;
                    tree->DeleteAllItems();
                    root = tree->AddRoot(wxEmptyString);
                    for (int i=0;i<parts;i++)
                        tree->AppendItem(root,piecestring + MyUtilFunctions::int2wxstr(i+1,2));
                    treeindex = 0;
                    tree->Refresh();
                }
                else
                {
                    wxTreeCtrl *tree = XRCCTRL(*(this), "treemessages",wxTreeCtrl);
                    wxTreeItemId treeitemid = tree->GetSelection();
                    if (treeitemid.IsOk())
                    {
                        wxString text = tree->GetItemText(treeitemid);
                        text.Mid(piecestring.Length()).ToLong(&treeindex);
                        treeindex--;
                        if ((treeindex < 0) || (treeindex >= parts))
                            treeindex = 0;
                    }
                    else
                        treeindex = 0;
                }

                //SET PROGRESS BAR NEW PARAMETERS
                if (programoptions.progressbarshow)
                {
                    if ((!current->IsSplitted()) && (current->GetStatus() == STATUS_ACTIVE))
                        progressbar->SetParams(1,current->percentualparts);
                    else
                        progressbar->SetParams(current->GetNumberofParts(),current->percentualparts);
                }

                #ifdef USE_HTML_MESSAGES
                if  (*(XRCCTRL(*(this), "messages",wxHtmlWindow)->GetParser()->GetSource()) != current->messages[treeindex])
                {
                    int x,y,xx,yy;
                    XRCCTRL(*(this), "messages",wxHtmlWindow)->SetPage(wxEmptyString);
                    XRCCTRL(*(this), "messages",wxHtmlWindow)->AppendToPage(current->messages[treeindex]);
                    XRCCTRL(*(this), "messages",wxHtmlWindow)->GetScrollPixelsPerUnit(&x,&y);
                    XRCCTRL(*(this), "messages",wxHtmlWindow)->GetVirtualSize(&xx,&yy);
                    xx /= x;
                    yy /= y;
                    XRCCTRL(*(this), "messages",wxHtmlWindow)->Scroll(-1,yy);
                }

                #else
                if  (XRCCTRL(*(this), "messages",wxTextCtrl)->GetValue() != current->messages[treeindex])
                {
                    XRCCTRL(*(this), "messages",wxTextCtrl)->Clear();
                    XRCCTRL(*(this), "messages",wxTextCtrl)->SetValue(current->messages[treeindex]);
                    XRCCTRL(*(this), "messages",wxTextCtrl)->ShowPosition(current->messages[treeindex].Length()-200);

                }
                   #endif
            }
        }
    }
    wxGetApp().downloadlist.SetNumberofActiveDownloads(numberofactivedownloads);

    if (programoptions.scheduleexceptionschanged)
    {
        programoptions.scheduleexceptionschanged = FALSE;
        for (int i=0;i<MAX_SCHEDULE_EXCEPTIONS;i++)
        {
            if (programoptions.scheduleexceptions[i].newstart != wxEmptyString)
            {
                programoptions.scheduleexceptions[i].start = programoptions.scheduleexceptions[i].newstart;
                programoptions.scheduleexceptions[i].finish = programoptions.scheduleexceptions[i].newfinish;
                programoptions.scheduleexceptions[i].day = programoptions.scheduleexceptions[i].newday;
                programoptions.scheduleexceptions[i].isactive = TRUE;
            }
            else
                programoptions.scheduleexceptions[i].isactive = FALSE;
        }
    }

    if (programoptions.activatescheduling)
    {
        wxDateTime now = wxDateTime::Now();
        wxDateTime exceptionstart,exceptionfinish;
        long hour,minute;
        bool stopscheduled = FALSE;
        bool exceptionhappened = FALSE;

        if ((now > programoptions.startdatetime) && (now < programoptions.finishdatetime))
        {
            for (int i = 0;i < MAX_SCHEDULE_EXCEPTIONS;i++)
            {
                if ((programoptions.scheduleexceptions[i].day == now.GetWeekDay()) && (programoptions.scheduleexceptions[i].isactive))
                {
                    programoptions.scheduleexceptions[i].start.Mid(0,2).ToLong(&hour);
                    programoptions.scheduleexceptions[i].start.Mid(3,2).ToLong(&minute);
                    exceptionstart.Set(hour,minute);
                    programoptions.scheduleexceptions[i].finish.Mid(0,2).ToLong(&hour);
                    programoptions.scheduleexceptions[i].finish.Mid(3,2).ToLong(&minute);
                    exceptionfinish.Set(hour,minute);
                    if ((now > exceptionstart) && (now < exceptionfinish))
                    {
                        stopscheduled = TRUE;
                        exceptionhappened = TRUE;
                        break;
                    }
                }
            }

            if ((simultaneous > 0) && (!stopscheduled))
            {
                for ( mDownloadList::Node *node = wxGetApp().downloadlist.GetFirst(); node; node = node->GetNext() )
                {
                    current = node->GetData();
                    if (current->GetStatus() == STATUS_SCHEDULE_QUEUE)
                    {
                        if (StartDownload(current))
                        {
                            simultaneous--;
                            if (simultaneous <= 0)
                                break;
                        }
                    }
                }
            }
        }
        else
            stopscheduled = TRUE;
        if (stopscheduled)
        {
            mInProgressList *list = XRCCTRL(*this, "inprogresslist",mInProgressList );
            if (now > programoptions.finishdatetime)
                programoptions.activatescheduling = FALSE;
            for ( mDownloadList::Node *node = wxGetApp().downloadlist.GetFirst(); node; node = node->GetNext() )
            {
                current = node->GetData();
                if ((current->GetStatus() == STATUS_ACTIVE) && (current->IsScheduled()))
                {
                    list->SetCurrentSelection(wxGetApp().downloadlist.IndexOf(current));
                    StopDownload(current,FALSE); //STOP BUT DON'T CHANGE THE SCHEDULE
                    if (!exceptionhappened)
                        somedownloadfinishednow = TRUE;
                }
            }
        }
    }
    if (simultaneous > 0)
    {
        for ( mDownloadList::Node *node = wxGetApp().downloadlist.GetFirst(); node; node = node->GetNext() )
        {
            current = node->GetData();
            if ((current->GetStatus() == STATUS_QUEUE) && (!current->IsScheduled()))
            {
                if (StartDownload(current))
                {
                    simultaneous--;
                    if (simultaneous <= 0)
                        break;
                }
            }
        }
    }
    if (somedownloadfinishednow)
    {
        bool somedownloadactive = FALSE;
        for ( mDownloadList::Node *node = wxGetApp().downloadlist.GetFirst(); node; node = node->GetNext() )
        {
            current = node->GetData();
            if ((current->GetStatus() == STATUS_ACTIVE) || ((current->GetStatus() == STATUS_SCHEDULE_QUEUE) && (programoptions.activatescheduling)))
                somedownloadactive = TRUE;
        }
        if (!somedownloadactive)
        {
            if (programoptions.restoremainframe)
            {
                if (!this->IsShown())
                {
                    wxCommandEvent iconizeevent;
                    taskbaricon->OnHide(iconizeevent);
                }
            }
            if (programoptions.shutdown)
            {
                wxCommandEvent shutdown(wxEVT_SHUTDOWN);
                GetEventHandler()->AddPendingEvent(shutdown);
            }
            else if (programoptions.disconnect)
            {
                wxCommandEvent disconnect(wxEVT_DISCONNECT);
                GetEventHandler()->AddPendingEvent(disconnect);
            }
        }
    }
    if (timerinterval >= (programoptions.graphrefreshtime))
    {
        float *value = new float();
        wxString msg,temp;
        *value = ((float)currentspeed) / 1024.0;
        graphpoints.Append(value);

        msg = _("Total Speed:");
        msg += wxT(" %0.1f kB/s");
        temp.Printf(msg, *value);
        temp.Replace(wxT(","),wxT("."));
        if (statusbar)
            statusbar->SetStatusText(temp,1);

        if (graphpoints.GetCount() > (unsigned int)programoptions.graphhowmanyvalues)
            graphpoints.DeleteNode(graphpoints.GetFirst());

        timerinterval = 0;
        if (this->IsShown())
        {
            #ifndef DISABLE_MUTEX
            mutex_programoptions->Unlock();
            #endif

            //REFRESH THE GRAPH
            if (programoptions.graphshow)
                graph->Refresh();

            //REFRESH THE PROGRESS BAR
            if (programoptions.progressbarshow)
                progressbar->Refresh();
        }
    }
    timerinterval += mtimer->GetInterval();
    #ifndef DISABLE_MUTEX
    mutex_programoptions->Unlock();
    #endif
}

bool mMainFrame::NewDownload(wxArrayString url, wxString destination,int metalinkindex,int parts,wxString user,wxString password,wxString reference,wxString comments,wxString command,int startoption, bool ontop, bool show,bool permitdifferentnames)
{
    mBoxNew dlg;
    wxTextCtrl *edturl, *edtdestination, *edtuser ,*edtpassword, *edtreferenceurl, *edtcomments, *edtcommand;
    wxCheckListBox *lstaddresslist;
    wxSpinCtrl *spinsplit, *spinbandwidth;
    wxRadioButton *optnow,*optschedule;
    wxCheckBox *chkaddontop;
    int result;
    int i;
    wxXmlResource::Get()->LoadDialog(&dlg, this, wxT("boxnew"));

    edturl = XRCCTRL(dlg, "edturl",wxTextCtrl);
    edtdestination = XRCCTRL(dlg, "edtdestination",wxTextCtrl);
    edtuser = XRCCTRL(dlg, "edtuser",wxTextCtrl);
    edtpassword = XRCCTRL(dlg, "edtpassword",wxTextCtrl);
    edtreferenceurl = XRCCTRL(dlg, "edtreferenceurl",wxTextCtrl);
    edtcommand = XRCCTRL(dlg, "edtcommand",wxTextCtrl);
    edtcomments = XRCCTRL(dlg, "edtcomments",wxTextCtrl);
    lstaddresslist = XRCCTRL(dlg, "lstaddresslist",wxCheckListBox);
    optnow = XRCCTRL(dlg, "optnow",wxRadioButton);
    optschedule = XRCCTRL(dlg, "optschedule",wxRadioButton);
    spinsplit = XRCCTRL(dlg, "spinsplit",wxSpinCtrl);
    spinbandwidth = XRCCTRL(dlg, "spinbandwidth",wxSpinCtrl);
    chkaddontop = XRCCTRL(dlg, "chkaddontop",wxCheckBox);

    lstaddresslist->Clear();
    if ((url.GetCount() == 1) && (show))
        edturl->SetValue(url.Item(0));
    else
    {
        lstaddresslist->InsertItems(url,0);
        for (unsigned int i = 0; i < lstaddresslist->GetCount() ;i++)
            lstaddresslist->Check(i);
    }
    #if wxCHECK_VERSION(2, 8, 0)
    dlg.SetInitialSize();
    #else
    dlg.SetBestFittingSize();
    #endif
    dlg.SetDifferentNamesPermition(permitdifferentnames);
    edtdestination->SetValue(destination);
    /*if (user == ANONYMOUS_USER)
    {
        edtuser->SetValue(wxEmptyString);
        edtpassword->SetValue(wxEmptyString);
    }
    else
    {*/
    edtuser->SetValue(user);
    edtpassword->SetValue(password);
    edtreferenceurl->SetValue(reference);
    edtcommand->SetValue(command);
    edtcomments->SetValue(comments);
    if (startoption == NOW)
        optnow->SetValue(TRUE);
    else
        optnow->SetValue(FALSE);
    chkaddontop->SetValue(ontop);
    spinsplit->SetValue(parts);
    if (show)
    {
        this->active = FALSE;
        dlg.SetSize(programoptions.boxnew_x,programoptions.boxnew_y,-1,-1);
        result = dlg.ShowModal();
        dlg.GetPosition(&programoptions.boxnew_x,&programoptions.boxnew_y);
        this->active = TRUE;
    }
    else
        result = XRCID("btnok");

    if (result == XRCID("btnok"))
    {
        wxFileName destinationvalue,tempdestinationvalue;
        mDownloadFile *currentfile = NULL;
        int scheduled, now,forindex;
        int nparams = lstaddresslist->GetCount();
        int bandwidth = spinbandwidth->GetValue();
        wxString name = wxEmptyString;
        scheduled = optschedule->GetValue();
        now = optnow->GetValue();
        ontop = chkaddontop->GetValue();

        destinationvalue.Assign(edtdestination->GetValue());
        tempdestinationvalue.Assign(programoptions.downloadpartsdefaultdir);
        if (nparams == 0)
            return FALSE;

        for (i = 0; i < nparams; i++)
        {
            if (ontop)
                forindex = nparams-i-1;
            else
                forindex = i;
            if (!lstaddresslist->IsChecked(forindex))
                continue;

            mUrlName *urltmp = new mUrlName(lstaddresslist->GetString(forindex));

            if (name.IsEmpty())
                name = urltmp->GetFullName();
            if ((!permitdifferentnames) && (name != urltmp->GetFullName()))
                continue;

            currentfile = wxGetApp().downloadlist.FindDownloadFile(urltmp->GetFullName());

            if (!currentfile)
            {
                mUrlList *urllist = new mUrlList();
                urllist->Append(urltmp);
                currentfile = wxGetApp().downloadlist.NewDownloadRegister(urllist,destinationvalue, tempdestinationvalue,metalinkindex,spinsplit->GetValue(),
                        edtuser->GetValue(), edtpassword->GetValue(), edtreferenceurl->GetValue(), edtcomments->GetValue(),edtcommand->GetValue(),scheduled,bandwidth,ontop);
                XRCCTRL(*this, "inprogresslist",mInProgressList )->Insert(currentfile,-1,ontop);
            }
            else
            {
                if (!currentfile->FindUrl(*urltmp))
                    currentfile->AppendUrl(urltmp);
                continue;
            }

            if (now)
            {
                currentfile->PutOnQueue();
                if (programoptions.hidemainframe) //HIDE THE MAINFRAME
                {
                    wxIconizeEvent iconizeevent;
                    this->OnIconize(iconizeevent);
                }
            }
            else if (scheduled)
                currentfile->PutOnScheduleQueue();
        }
        //SET THE LAST BOXNEW OPTIONS LIKE THE DEFAULT ONE
        programoptions.lastnumberofparts = spinsplit->GetValue();
        if (scheduled)
            programoptions.laststartoption = SCHEDULE;
        else if (now)
            programoptions.laststartoption = NOW;
        else
            programoptions.laststartoption = MANUAL;
        programoptions.lastdestination = edtdestination->GetValue();
        programoptions.lastcommand = edtcommand->GetValue();
        programoptions.lastontopoption = chkaddontop->GetValue();

        //WRITE THE LAST BOXNEW OPTIONS ON DISK
        mApplication::Configurations(WRITE,OPT_LAST_BOXNEW_X_REG, programoptions.boxnew_x);
        mApplication::Configurations(WRITE,OPT_LAST_BOXNEW_Y_REG, programoptions.boxnew_y);
        mApplication::Configurations(WRITE,OPT_LAST_COMMAND_REG, programoptions.lastcommand);
        mApplication::Configurations(WRITE,OPT_LAST_DESTINATION_REG, programoptions.lastdestination);
        mApplication::Configurations(WRITE,OPT_LAST_NUMBER_OF_PARTS_REG, programoptions.lastnumberofparts);
        mApplication::Configurations(WRITE,OPT_LAST_START_OPTION_REG, programoptions.laststartoption);
        mApplication::Configurations(WRITE,OPT_LAST_ONTOP_OPTION_REG, programoptions.lastontopoption);
    }
    return TRUE;
}

void mMainFrame::OnNew(wxCommandEvent& event)
{
    OnPasteURL(event);
}

void mMainFrame::OnRemove(wxCommandEvent& event)
{
    mInProgressList *inprogresslist = XRCCTRL(*this, "inprogresslist",mInProgressList );
    if (inprogresslist->GetCurrentSelection().GetCount() > 0)
    {
        mListSelection currentselectionlist = inprogresslist->GetCurrentSelection();
        int nselection = currentselectionlist.GetCount();
        mDownloadFile *currentfile;

        wxProgressDialog *waitbox = new wxProgressDialog(_("Stopping the download..."),_("Stopping the current download before removing..."));
        for (int i = 0; i < nselection;i++)
        {
            currentfile = wxGetApp().downloadlist.Item(currentselectionlist.Item(i))->GetData();
            StopDownload(currentfile); //STOP AND SET SCHEDULE = FALSE
            while ((currentfile->IsFree() == FALSE) || (currentfile->GetStatus() == STATUS_ACTIVE))
                wxYield();
            waitbox->Update(100*(i+1)/nselection);
        }
        waitbox->Update(100);
        delete waitbox;

        wxMessageDialog *dlg = new wxMessageDialog(this, _("Do you also want to remove the partial downloaded file from the disk?"),_("Remove..."),
                    wxYES_NO | wxCANCEL | wxICON_QUESTION);

        if (dlg)
        {
            int resp;
            dlg->Centre(wxBOTH);
            this->active = FALSE;
            resp = dlg->ShowModal();
            this->active = TRUE;
            for (int i = nselection-1 ; i >= 0 ;i--) //REMOVE THE ITEM BACKWARD
            {
                currentfile = wxGetApp().downloadlist.Item(currentselectionlist.Item(i))->GetData();
                if (((currentfile->GetStatus() == STATUS_STOPED) || (currentfile->GetStatus() == STATUS_ERROR)) && (currentfile->IsFree()))
                {
                    if (resp == wxID_YES)
                    {
                        wxString destination = currentfile->GetDestination();
                        wxString name = currentfile->GetName();
                        currentfile->MarkRemoveAsPending(TRUE);
                        if (destination.Mid(destination.Length()-1,1) != SEPARATOR_DIR)
                            destination = destination + SEPARATOR_DIR;
                        for (int i = 0; i< MAX_NUM_PARTS;i++)
                            ::wxRemoveFile(destination + PREFIX + name + EXT + MyUtilFunctions::int2wxstr(i));
                    }
                    else if (resp == wxID_NO)
                    {
                        currentfile->MarkRemoveAsPending(TRUE);
                    }
                }
            }
            dlg->Destroy();
        }
        return ;
    }
    mFinishedList *finishedlist = XRCCTRL(*this, "finishedlist",mFinishedList );
    if (finishedlist->GetCurrentSelection().GetCount() > 0)
    {
        wxMessageDialog *dlg = new wxMessageDialog(this, _("Do you also want to remove downloaded file(s) from the disk?"),_("Remove..."),
                    wxYES_NO | wxCANCEL | wxICON_QUESTION);
        if (dlg)
        {
            int resp;
            int currentselection;
            dlg->Centre(wxBOTH);
            this->active = FALSE;
            resp = dlg->ShowModal();
            this->active = TRUE;
            mListSelection currentselectionlist = finishedlist->GetCurrentSelection();
            #ifdef WXDFAST_PORTABLE
            wxFileConfig *config = new wxFileConfig(DFAST_REG, wxEmptyString, DFAST_REG + wxT(".ini"), wxEmptyString,
                                            wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
            #else
            wxFileConfig *config = new wxFileConfig(DFAST_REG);
            #endif
            config->SetPath(FINISHED_REG);
            int nselection = currentselectionlist.GetCount();
            for (int i = nselection-1 ; i >= 0 ;i--) //REMOVE THE ITEM BACKWARD
            {
                currentselection = currentselectionlist.Item(i);
                if (resp != wxID_CANCEL)
                {
                    wxString destination,name;
                    if (destination.Mid(destination.Length()-1,1) != SEPARATOR_DIR)
                        destination = destination + SEPARATOR_DIR;

                    wxListItem item;
                    item.SetId(currentselection);
                    item.SetColumn(FINISHED_NAME);
                    item.SetMask(wxLIST_MASK_DATA|wxLIST_MASK_STATE|wxLIST_MASK_TEXT|wxLIST_MASK_IMAGE);
                    finishedlist->GetItem(item);

                    config->SetPath(item.GetText());

                    destination = wxEmptyString;
                    config->Read(DESTINATION_REG,&destination);
                    #ifdef WXDFAST_PORTABLE
                    {
                        #ifdef __WXMSW__
                        wxFileName destinationtmp(destination);
                        if (destinationtmp.GetVolume().Upper() == wxT("PORTABLE"))
                        {
                            destinationtmp.SetVolume(wxGetApp().programvolume);
                            destination = destinationtmp.GetFullPath();
                        }
                        #endif
                    }
                    #endif
                    if (destination.Mid(destination.Length()-1,1) != SEPARATOR_DIR)
                        destination = destination + SEPARATOR_DIR;

                    name = wxEmptyString;
                    config->Read(NAME_REG,&name);

                    config->SetPath(BACK_DIR_REG);
                    config->DeleteGroup(item.GetText());
                    finishedlist->DeleteItem(currentselection);

                    if (resp == wxID_YES)
                        ::wxRemoveFile(destination + name);
                }
            }
            delete config;
        }
    }
}

void mMainFrame::OnSchedule(wxCommandEvent& event)
{
    mInProgressList *list = XRCCTRL(*this, "inprogresslist",mInProgressList );
    if (list->GetCurrentSelection().GetCount() > 0)
    {
        mListSelection selectionlist = list->GetCurrentSelection();
        mDownloadFile *file;
        int status;
        for (unsigned int i = 0; i < selectionlist.GetCount();i++)
        {
            file = wxGetApp().downloadlist.Item(selectionlist.Item(i))->GetData();
            status = file->GetStatus();
            if (((status == STATUS_STOPED) || (status == STATUS_QUEUE) || (status == STATUS_ERROR)) && (file->IsFree()))
            {
                file->PutOnScheduleQueue();
                file->MarkWriteAsPending(TRUE);
            }
        }
    }
}

void mMainFrame::OnStart(wxCommandEvent& event)
{
    mInProgressList *list = XRCCTRL(*this, "inprogresslist",mInProgressList );
    if (list->GetCurrentSelection().GetCount() > 0)
    {
        mListSelection selectionlist = list->GetCurrentSelection();
        mDownloadFile *file;
        int status;
        for (unsigned int i = 0; i < selectionlist.GetCount();i++)
        {
            file = wxGetApp().downloadlist.Item(selectionlist.Item(i))->GetData();
            status = file->GetStatus();
            if (((status == STATUS_STOPED) || (status == STATUS_ERROR)) && (file->IsFree()))
            {
                file->PutOnQueue();
                file->MarkWriteAsPending(TRUE);
                if (programoptions.hidemainframe) //HIDE THE MAINFRAME
                {
                    wxIconizeEvent iconizeevent;
                    this->OnIconize(iconizeevent);
                }
            }
        }
    }
}

bool mMainFrame::StartDownload(mDownloadFile *downloadfile)
{
    if (downloadfile->IsFree())
    {
        if ((programoptions.checkforupdates) && ((wxDateTime::Now()-programoptions.lastnewreleasecheck).GetDays() >= 10))
        {
            programoptions.lastnewreleasecheck = wxDateTime::Now();
            mApplication::Configurations(WRITE,OPT_LAST_NEW_RELEASE_CHECK,(long)programoptions.lastnewreleasecheck.GetTicks());
            CheckNewRelease();
        }
        downloadfile->SetFree(FALSE);
        downloadfile->WaitSplit();
        downloadfile->speedpointowner = -1;
        for (int i=0; i < downloadfile->GetNumberofParts();i++)
        {
            mDownloadThread *thread = new mDownloadThread(downloadfile,i);
            if ( thread->Create() != wxTHREAD_NO_ERROR )
            {
                wxMessageBox(_("Error creating thread!"),_("Error...") ,wxOK | wxICON_ERROR,this);
                return FALSE;
            }
            else
            {
                wxCriticalSectionLocker enter(wxGetApp().m_critsect);
                if ( thread->Run() != wxTHREAD_NO_ERROR )
                {
                    wxMessageBox(_("Error starting thread!"),_("Error...") ,wxOK | wxICON_ERROR,this);
                    return FALSE;
                }
            }
        }
        Refresh();
        return TRUE;
    }
    return FALSE;
}

void mMainFrame::OnStop(wxCommandEvent& event)
{
    mInProgressList *list = XRCCTRL(*this, "inprogresslist",mInProgressList );
    mListSelection currentselectionlist = list->GetCurrentSelection();
    if (currentselectionlist.GetCount() > 0)
    {
        int status;
        mDownloadFile *file;
        for (unsigned int i = 0; i < currentselectionlist.GetCount();i++)
        {
            file = wxGetApp().downloadlist.Item(currentselectionlist.Item(i))->GetData();
            status = file->GetStatus();
            StopDownload(file); //STOP AND SET SCHEDULE = FALSE
            if ((status == STATUS_QUEUE) || (status == STATUS_SCHEDULE_QUEUE)) //WHEN THE DOWNLOAD IS ALREADY ACTIVE
                file->MarkWriteAsPending(TRUE);                                //THE WRITE ON DISK WILL BE MADE IN THE TIMER
        }
    }
}

void mMainFrame::StopDownload(mDownloadFile *downloadfile,bool stopschedule)
{
    if ((downloadfile->GetStatus() != STATUS_FINISHED) && (downloadfile->GetStatus() != STATUS_ERROR))
        downloadfile->SetAsStoped(stopschedule);
}

void mMainFrame::OnStartAll(wxCommandEvent& event)
{
    mInProgressList *list = XRCCTRL(*this, "inprogresslist",mInProgressList );
    wxProgressDialog *waitbox = new wxProgressDialog(_("Starting the downloads..."),_("Starting the downloads..."),100,NULL,wxPD_AUTO_HIDE | wxPD_APP_MODAL|wxPD_CAN_ABORT|wxPD_ELAPSED_TIME);
    int total = list->GetItemCount();
    for (int i=0 ; i < total;i++)
    {
        list->SetCurrentSelection(i);
        OnStart(event);
        waitbox->Update((int)(i*100/total));
    }
    waitbox->Update(100);
    delete waitbox;
    wxMilliSleep(200);
}

void mMainFrame::OnStopAll(wxCommandEvent& event)
{
    mDownloadFile *currentfile;
    mInProgressList *list = XRCCTRL(*this, "inprogresslist",mInProgressList );
    wxProgressDialog *waitbox = new wxProgressDialog(_("Stopping the downloads..."),_("Stopping the downloads..."),100,NULL,wxPD_AUTO_HIDE | wxPD_APP_MODAL|wxPD_CAN_ABORT|wxPD_ELAPSED_TIME);
    int total = list->GetItemCount();
    for (int i=0 ; i < total;i++)
    {
        list->SetCurrentSelection(i);
        if (event.GetString() == STOPWITHOUTSAVINGSTOPSTATUS)
            StopDownload(wxGetApp().downloadlist.Item(i)->GetData()); //STOP AND DON'T CARE ABOUT SCHEDULE
        else
            OnStop(event);
    }
    for (int i=0 ; i < total;i++)
    {
        currentfile = wxGetApp().downloadlist.Item(i)->GetData();
        while ((!currentfile->IsFree()) && (waitbox->Update((int)(i*100/total))))
        {
            wxYield();
            wxMilliSleep(200);
        }
    }
    waitbox->Update(100);
    delete waitbox;
    wxMilliSleep(200);
}

void mMainFrame::OnPasteURL(wxCommandEvent& event)
{
    int numberofparts, startoption;
    bool ontop;
    wxString destinationtmp = wxEmptyString, command = wxEmptyString;
    wxArrayString urltmp;
    if (wxTheClipboard->Open())
    {
        if (wxTheClipboard->IsSupported( wxDF_TEXT ))
        {
            wxTextDataObject data;
            wxTheClipboard->GetData( data );
            mUrlName url(data.GetText().Strip(wxString::both));
            if (url.IsComplete())
                urltmp.Add(url.GetFullPath());
        }
        wxTheClipboard->Close();
    }

    if (programoptions.rememberboxnewoptions)
    {
        numberofparts = programoptions.lastnumberofparts;
        startoption = programoptions.laststartoption;
        destinationtmp = programoptions.lastdestination;
        command = programoptions.lastcommand;
        ontop = programoptions.lastontopoption;
    }
    else
    {
        numberofparts = DEFAULT_NUM_PARTS;
        startoption = DEFAULT_START_OPTION;
        ontop = DEFAULT_ONTOP_OPTION;
        destinationtmp = programoptions.destination;
    }
    NewDownload(urltmp,destinationtmp,-1,numberofparts,wxEmptyString,wxEmptyString,wxEmptyString,wxEmptyString,command,startoption,ontop,TRUE,FALSE);
}

void mMainFrame::OnCopyURL(wxCommandEvent& event)
{
    wxString urltmp,destinationtmp;
    if (wxTheClipboard->Open())
    {
        #ifdef WXDFAST_PORTABLE
        wxFileConfig *config = new wxFileConfig(DFAST_REG, wxEmptyString, DFAST_REG + wxT(".ini"), wxEmptyString,
                                                wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
        #else
        wxFileConfig *config = new wxFileConfig(DFAST_REG);
        #endif
        wxString str,name;
        wxTextDataObject data;
        int currentselection,column;
        wxListCtrl *list;
        mInProgressList *list01 = XRCCTRL(*(wxGetApp().mainframe), "inprogresslist",mInProgressList );
        mFinishedList *list02 = XRCCTRL(*(wxGetApp().mainframe), "finishedlist",mFinishedList);
        if ((currentselection = list01->GetCurrentLastSelection()) >= 0)
        {
            list01->SetCurrentSelection(currentselection);
            config->SetPath(INPROGRESS_REG);
            column = INPROGRESS_NAME;
            list = (wxListCtrl*)list01;
        }
        else if ((currentselection = list02->GetCurrentLastSelection()) >= 0)
        {
            list02->SetCurrentSelection(currentselection);
            config->SetPath(FINISHED_REG);
            column = FINISHED_NAME;
            list = (wxListCtrl*)list02;
        }
        else
        {
            wxTheClipboard->Close();
            return;
        }

        if (column == INPROGRESS_NAME)
            name = wxGetApp().downloadlist.Item(currentselection)->GetData()->GetName();
        else
        {
            wxListItem item;
            item.SetId(currentselection);
            item.SetColumn(column);
            item.SetMask(wxLIST_MASK_DATA|wxLIST_MASK_STATE|wxLIST_MASK_TEXT|wxLIST_MASK_IMAGE);
            list->GetItem(item);
            name = item.GetText();
        }
        config->SetPath(name);
        str = wxEmptyString;
        config->Read(URL_REG + wxT("1"),&str);
        wxTheClipboard->SetData( new wxTextDataObject(str));
        wxTheClipboard->Close();
        delete config;
    }
    else
        wxMessageBox(_("Unable to open the clipboard!"),_("Error...") ,wxOK | wxICON_ERROR,this);
}

void mMainFrame::OnCopyDownloadData(wxCommandEvent& event)
{
    wxString urltmp,destinationtmp;
    if (wxTheClipboard->Open())
    {
        #ifdef WXDFAST_PORTABLE
        wxFileConfig *config = new wxFileConfig(DFAST_REG, wxEmptyString, DFAST_REG + wxT(".ini"), wxEmptyString,
                                                wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
        #else
        wxFileConfig *config = new wxFileConfig(DFAST_REG);
        #endif
        wxString str = wxEmptyString, downloadinfo = wxEmptyString, name;
        wxTextDataObject data;
        int currentselection,column;
        wxListCtrl *list;
        mInProgressList *list01 = XRCCTRL(*(wxGetApp().mainframe), "inprogresslist",mInProgressList );
        mFinishedList *list02 = XRCCTRL(*(wxGetApp().mainframe), "finishedlist",mFinishedList);
        if ((currentselection = list01->GetCurrentLastSelection()) >= 0)
        {
            list01->SetCurrentSelection(currentselection);
            config->SetPath(INPROGRESS_REG);
            column = INPROGRESS_NAME;
            list = (wxListCtrl*)list01;
        }
        else if ((currentselection = list02->GetCurrentLastSelection()) >= 0)
        {
            list02->SetCurrentSelection(currentselection);
            config->SetPath(FINISHED_REG);
            column = FINISHED_NAME;
            list = (wxListCtrl*)list02;
        }
        else
        {
            wxTheClipboard->Close();
            return;
        }

        if (column == INPROGRESS_NAME)
            name = wxGetApp().downloadlist.Item(currentselection)->GetData()->GetName();
        else
        {
            wxListItem item;
            item.SetId(currentselection);
            item.SetColumn(column);
            item.SetMask(wxLIST_MASK_DATA|wxLIST_MASK_STATE|wxLIST_MASK_TEXT|wxLIST_MASK_IMAGE);
            list->GetItem(item);
            name = item.GetText();
        }
        config->SetPath(name);

        downloadinfo += _("Name");
        downloadinfo += wxT(": ") + name + wxT("\n");

        str = wxEmptyString;
        config->Read(CONTENTTYPE_REG,&str);
        downloadinfo += _("File type");
        downloadinfo += wxT(": ") + str + wxT("\n");

        str = wxEmptyString;
        config->Read(SIZE_REG,&str);
        downloadinfo += _("Size");
        downloadinfo += wxT(": ") + MyUtilFunctions::ByteString(MyUtilFunctions::wxstrtolonglong(str)) + wxT("\n");

        str = wxEmptyString;
        config->Read(TIMEPASSED_REG,&str);
        downloadinfo += _("Time");
        downloadinfo += wxT(": ") + MyUtilFunctions::TimeString(MyUtilFunctions::wxstrtolonglong(str)) + wxT("\n");

        str = wxEmptyString;
        config->Read(DESTINATION_REG,&str);
        #ifdef WXDFAST_PORTABLE
        {
            #ifdef __WXMSW__
            wxFileName destinationtmp(str);
            if (destinationtmp.GetVolume().Upper() == wxT("PORTABLE"))
            {
                destinationtmp.SetVolume(wxGetApp().programvolume);
                str = destinationtmp.GetFullPath();
            }
            #endif
        }
        #endif
        downloadinfo += _("Destination");
        downloadinfo += wxT(": ") + str + wxT("\n");

        if (column == FINISHED_NAME)
        {
            str = wxEmptyString;
            config->Read(MD5_REG,&str);
            downloadinfo += _("MD5");
            downloadinfo += wxT(": ") + str + wxT("\n");
        }

        str = wxEmptyString;
        config->Read(REFERENCE_REG,&str);
        downloadinfo += _("Reference URL");
        downloadinfo += wxT(": ") + str + wxT("\n");

        str = wxEmptyString;
        config->Read(COMMENTS_REG,&str);
        downloadinfo += _("Comments");
        downloadinfo += wxT(": ") + str + wxT("\n");

        str = wxEmptyString;
        config->Read(URL_REG + wxT("1"),&str);
        downloadinfo += _("URLs");
        downloadinfo += wxT(":\t") + str + wxT("\n");

        bool existurl = TRUE;
        int count = 2;
        while (existurl)
        {
            str = wxEmptyString;
            config->Read(URL_REG + MyUtilFunctions::int2wxstr(count),&(str));
            if (str.IsEmpty())
                break;
            else
                downloadinfo += wxT("\t") + str + wxT("\n");
            count++;
        }

        wxTheClipboard->SetData( new wxTextDataObject(downloadinfo));
        wxTheClipboard->Close();
        delete config;
    }
    else
        wxMessageBox(_("Unable to open the clipboard!"),_("Error...") ,wxOK | wxICON_ERROR,this);
}
void mMainFrame::OnSelectAll(wxCommandEvent& event)
{
    mNotebook *notebook = XRCCTRL(*(this), "notebook01",mNotebook );
    wxListCtrl *list;
    if (notebook->GetSelection() == 0)
    {
        list = XRCCTRL(*(this), "inprogresslist",mInProgressList);
    }else if (notebook->GetSelection() == 1)
    {
        list = XRCCTRL(*(this), "finishedlist",mFinishedList);
    }
    else
        return;
    for (int i=0;i<list->GetItemCount();i++)
        list->SetItemState(i,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
}

void mMainFrame::OnInvertSelection(wxCommandEvent& event)
{
    mNotebook *notebook = XRCCTRL(*(this), "notebook01",mNotebook );
    mListSelection selectionlist;
    if (notebook->GetSelection() == 0)
    {
        mInProgressList *list = XRCCTRL(*(this), "inprogresslist",mInProgressList);
        selectionlist = list->GetCurrentSelection();
        list->SelectUnselect(FALSE,-1);
        int nitems = list->GetItemCount();
        int nselections = selectionlist.GetCount();
        int count = 0;
        int next = -1;
        if (nselections > 0)
            next = selectionlist.Item(count);
        for (int i = 0; i < nitems ; i++)
        {
            if (i != next)
                list->SetItemState(i,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
            else
            {
                count++;
                if (count < nselections)
                {
                    next = selectionlist.Item(count);
                }
            }
        }
    }
    else if (notebook->GetSelection() == 1)
    {
        mFinishedList *list = XRCCTRL(*(this), "finishedlist",mFinishedList);
        selectionlist = list->GetCurrentSelection();
        list->SelectUnselect(FALSE,-1);
        int nitems = list->GetItemCount();
        int nselections = selectionlist.GetCount();
        int count = 0;
        int next = -1;
        if (nselections > 0)
            next = selectionlist.Item(count);
        for (int i = 0; i < nitems ; i++)
        {
            if (i != next)
                list->SetItemState(i,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
            else
            {
                count++;
                if (count < nselections)
                {
                    next = selectionlist.Item(count);
                }
            }
        }
    }
    else
        return;
}

void mMainFrame::OnFind(wxCommandEvent& event)
{
    wxFindReplaceData *data = new wxFindReplaceData();
    data->SetFlags(wxFR_DOWN);
    mBoxFind *dlg = new mBoxFind(this,data,_("Find file..."));
    if (dlg)
    {
        this->active = FALSE;
        dlg->Show();
        this->active = TRUE;
    }
}

void mMainFrame::OnShowProgressBar(wxCommandEvent& event)
{
    if (programoptions.progressbarshow)
    {
        programoptions.progressbarshow = FALSE;
        progressbar->Hide();
    }
    else
    {
        programoptions.progressbarshow = TRUE;
        progressbar->Show();
    }
    mApplication::Configurations(WRITE,OPT_PROGRESS_BAR_SHOW_REG,programoptions.progressbarshow);
}

void mMainFrame::OnShowGraph(wxCommandEvent& event)
{
    if (programoptions.graphshow)
        programoptions.graphshow = FALSE;
    else
        programoptions.graphshow = TRUE;
    mApplication::Configurations(WRITE,OPT_GRAPH_SHOW_REG,programoptions.graphshow);
    ShowHideResizeGraph(programoptions.graphheight);
}

void mMainFrame::ShowHideResizeGraph(int oldgraphheight)
{
    if (programoptions.graphheight != oldgraphheight) //SHOW/HIDE/CHANGE THE HEIGHT OF THE GRAPH
    {
        int currentgraphheight = programoptions.graphheight;
        programoptions.graphheight = oldgraphheight;
        graph->Hide();
        programoptions.graphheight = currentgraphheight;
        if (programoptions.graphshow)
            graph->Show();
    }
    else
    {
        if (!programoptions.graphshow)
            graph->Hide();
        else
            graph->Show();
    }
}

void mMainFrame::OnDetails(wxCommandEvent& event)
{
    wxSplitterWindow *splitter = XRCCTRL(*this, "splitter01",wxSplitterWindow);
    if (splitter->IsSplit())
        splitter->Unsplit();
    else
    {
        splitter->SplitHorizontally(XRCCTRL(*this, "inprogresslist",mInProgressList ),XRCCTRL(*this, "panelmessages01",wxPanel));
        XRCCTRL(*this, "panelmessages01",wxPanel)->Show();
    }

}

void mMainFrame::OnUpDown(bool up)
{
    mInProgressList *list = XRCCTRL(*this, "inprogresslist",mInProgressList );
    int newselection = -1,currentselection;
    currentselection = list->GetCurrentLastSelection();
    if ((currentselection > 0) && (up == TRUE))
        newselection = currentselection -1;
    if ((currentselection >= 0) && (up == FALSE) && (currentselection+1 < list->GetItemCount()))
        newselection = currentselection +1;
    if (newselection >= 0)
    {
        mDownloadFile *currentfile = wxGetApp().downloadlist.Item(currentselection)->GetData();
        mDownloadFile *newfile = wxGetApp().downloadlist.Item(newselection)->GetData();

        wxGetApp().downloadlist.ChangePosition(currentfile,newfile);

        currentfile->MarkWriteAsPending(TRUE);
        newfile->MarkWriteAsPending(TRUE);

        list->Insert(currentfile,newselection);
        list->Insert(newfile,currentselection);
        list->HandleSelectDeselectEvents(FALSE);
        list->SetCurrentSelection(newselection);
        list->HandleSelectDeselectEvents(TRUE);
    }
}

void mMainFrame::MarkCurrentLanguageMenu(int language)
{
    if (!menubar)
        return;
    if (language != wxLANGUAGE_DEFAULT)
        menubar->GetMenu(2)->Check(XRCID("menulang_default"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_default"),TRUE);

    if (language != wxLANGUAGE_ENGLISH)
        menubar->GetMenu(2)->Check(XRCID("menulang_ent"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_ent"),TRUE);

    if (language != wxLANGUAGE_PORTUGUESE)
        menubar->GetMenu(2)->Check(XRCID("menulang_pt"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_pt"),TRUE);

    if (language != wxLANGUAGE_PORTUGUESE_BRAZILIAN)
        menubar->GetMenu(2)->Check(XRCID("menulang_ptbr"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_ptbr"),TRUE);

    if (language != wxLANGUAGE_GERMAN)
        menubar->GetMenu(2)->Check(XRCID("menulang_de"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_de"),TRUE);

    if (language != wxLANGUAGE_SPANISH)
        menubar->GetMenu(2)->Check(XRCID("menulang_es"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_es"),TRUE);

    if (language != wxLANGUAGE_CZECH)
        menubar->GetMenu(2)->Check(XRCID("menulang_cs"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_cs"),TRUE);

    if (language != wxLANGUAGE_RUSSIAN)
        menubar->GetMenu(2)->Check(XRCID("menulang_ru"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_ru"),TRUE);

    if (language != wxLANGUAGE_HUNGARIAN)
        menubar->GetMenu(2)->Check(XRCID("menulang_hu"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_hu"),TRUE);

    if (language != wxLANGUAGE_INDONESIAN)
        menubar->GetMenu(2)->Check(XRCID("menulang_id"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_id"),TRUE);

    if (language != wxLANGUAGE_ARMENIAN)
        menubar->GetMenu(2)->Check(XRCID("menulang_hy"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_hy"),TRUE);

    if (language != wxLANGUAGE_POLISH)
        menubar->GetMenu(2)->Check(XRCID("menulang_pl"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_pl"),TRUE);

    if (language != wxLANGUAGE_TURKISH)
        menubar->GetMenu(2)->Check(XRCID("menulang_tr"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_tr"),TRUE);

    if (language != wxLANGUAGE_FRENCH)
        menubar->GetMenu(2)->Check(XRCID("menulang_fr"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_fr"),TRUE);

    if (language != wxLANGUAGE_DUTCH)
        menubar->GetMenu(2)->Check(XRCID("menulang_nl"),FALSE);
    else
        menubar->GetMenu(2)->Check(XRCID("menulang_nl"),TRUE);
}

void mMainFrame::SetLanguage(int language)
{
    wxGetApp().SetLanguage(language);

    //STOP THE TIMER BEFORE UPDATE THE INTERFACE
    mtimer->Stop();

    //RELOAD THE MENU BAR
    wxMenuBar *oldmenubar = menubar;
    if ((menubar = wxXmlResource::Get()->LoadMenuBar(wxT("menubar"))))
    {
        this->SetMenuBar(menubar);
        delete oldmenubar;
    }
    else
        menubar = oldmenubar;

    //RELOAD THE TOOL BAR
    wxToolBar *oldtoolbar = toolbar;
    if ((toolbar = wxXmlResource::Get()->LoadToolBar(this,wxT("toolbar"))))
    {
        this->SetToolBar(toolbar);
        delete oldtoolbar;
    }
    else
        toolbar = oldtoolbar;

    //MARK CURRENT LANGUAGE ON MENU
    MarkCurrentLanguageMenu(language);

    //REGENERATE THE LISTS
    XRCCTRL(*(this), "inprogresslist",mInProgressList )->GenerateList(imageslist);
    XRCCTRL(*(this), "finishedlist",mFinishedList )->GenerateList(imageslist);

    //RESET NOTEBOOK PAGES LABELS
    XRCCTRL(*(this), "notebook01",mNotebook )->ReSetPagesLabel();

    //RESET THE PREVIEW PANEL TEXT
    {
        wxString contentstring =  _("File type");
        contentstring += wxT(":\n");
        XRCCTRL(*this, "lblpreview", wxStaticText )->SetLabel(contentstring);
        XRCCTRL(*this, "btnpreview", wxButton )->SetLabel(_("Preview"));
    }

    //RESET THE STATUSBAR TEXT
    if (statusbar)
    {
        this->defaultstatusbarmessage = wxEmptyString;
        wxStringTokenizer newrelease(programoptions.currentrelease,wxT("."));
        wxStringTokenizer currentrelease(VERSION,wxT("."));
        long old01,old02,old03,new01,new02,new03;
        newrelease.GetNextToken().ToLong(&new01);
        currentrelease.GetNextToken().ToLong(&old01);
        newrelease.GetNextToken().ToLong(&new02);
        currentrelease.GetNextToken().ToLong(&old02);
        newrelease.GetNextToken().ToLong(&new03);
        currentrelease.GetNextToken().ToLong(&old03);
        if ((new01*100)+(new02*10)+(new03) > (old01*100)+(old02*10)+(old03))
        {
            this->defaultstatusbarmessage = _("NEW RELEASE: ");
            this->defaultstatusbarmessage += wxT("wxDownload Fast ");
            this->defaultstatusbarmessage += programoptions.currentrelease + _(" is available.");
        }
        statusbar->SetStatusText(this->defaultstatusbarmessage);
        if (programoptions.bandwidthoption == 1)
        {
            wxString temp = _("Band control");
            temp += wxT(": ");
            temp += _("Per Download");
            statusbar->SetStatusText(temp,2);
        }
        else if (programoptions.bandwidthoption == 2)
        {
            wxString temp = _("Band control");
            temp += wxT(": ");
            temp += _("Active");
            temp += wxT(" (") + MyUtilFunctions::int2wxstr(programoptions.bandwidth) + wxT(" kB/s)");
            statusbar->SetStatusText(temp,2);
        }
        else
        {
            wxString temp = _("Band control");
            temp += wxT(": ");
            temp += _("Unlimited");
            statusbar->SetStatusText(temp,2);
        }
    }

    //WRITE LANGUAGE SELECTION
    mApplication::Configurations(WRITE,LANGUAGE_REG,language);

    //RESTART THE TIMER
    mtimer->Start();
}

void mMainFrame::OnDefaultLanguage(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_DEFAULT);
}

void mMainFrame::OnEnglish(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_ENGLISH);
}

void mMainFrame::OnPortuguese(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_PORTUGUESE);
}

void mMainFrame::OnPortugueseBrazil(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_PORTUGUESE_BRAZILIAN);
}

void mMainFrame::OnGerman(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_GERMAN);
}

void mMainFrame::OnSpanish(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_SPANISH);
}

void mMainFrame::OnCzech(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_CZECH);
}

void mMainFrame::OnHungarian(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_HUNGARIAN);
}

void mMainFrame::OnRussian(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_RUSSIAN);
}

void mMainFrame::OnArmenian(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_ARMENIAN);
}

void mMainFrame::OnIndonesian(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_INDONESIAN);
}

void mMainFrame::OnPolish(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_POLISH);
}

void mMainFrame::OnTurkish(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_TURKISH);
}

void mMainFrame::OnFrench(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_FRENCH);
}

void mMainFrame::OnDutch(wxCommandEvent& event)
{
    SetLanguage(wxLANGUAGE_DUTCH);
}

void mMainFrame::OnProperties(wxCommandEvent& event)
{
    mInProgressList *list = XRCCTRL(*this, "inprogresslist",mInProgressList );
    int currentselection;
    if ((currentselection = list->GetCurrentLastSelection()) >= 0)
    {
        mDownloadFile *currentfile = wxGetApp().downloadlist.Item(currentselection)->GetData();
        list->SetCurrentSelection(currentselection);
        mBoxNew dlg;
        wxString newname, oldname;
        wxCheckListBox *lstaddresslist;
        wxXmlResource::Get()->LoadDialog(&dlg, this, wxT("boxnew"));
        dlg.Centre(wxBOTH);
        dlg.SetTitle(_("Download Properties"));
        lstaddresslist = XRCCTRL(dlg,"lstaddresslist",wxCheckListBox);

        XRCCTRL(dlg, "edtdestination",wxTextCtrl)->SetValue(currentfile->GetDestination());
        lstaddresslist->Clear();
        lstaddresslist->InsertItems(currentfile->GetUrlArray(),0);
        for (unsigned int i = 0; i < lstaddresslist->GetCount() ;i++)
            lstaddresslist->Check(i);

        /*if (currentfile->GetUser() == ANONYMOUS_USER)
        {
            XRCCTRL(dlg, "edtuser",wxTextCtrl)->SetValue(wxEmptyString);
            XRCCTRL(dlg, "edtpassword",wxTextCtrl)->SetValue(wxEmptyString);
        }
        else
        {*/
        XRCCTRL(dlg, "edtuser",wxTextCtrl)->SetValue(currentfile->GetUser());
        XRCCTRL(dlg, "edtpassword",wxTextCtrl)->SetValue(currentfile->GetPassword());
        XRCCTRL(dlg, "spinsplit",wxSpinCtrl)->SetValue(currentfile->GetNumberofParts());
        XRCCTRL(dlg, "edtcomments",wxTextCtrl)->SetValue(currentfile->GetComments());
        XRCCTRL(dlg, "edtreferenceurl",wxTextCtrl)->SetValue(currentfile->GetReferenceURL());
        XRCCTRL(dlg, "edtcommand",wxTextCtrl)->SetValue(currentfile->GetCommand());
        XRCCTRL(dlg, "spinbandwidth",wxSpinCtrl)->SetValue(currentfile->GetBandWidth());
        XRCCTRL(dlg, "optmanual",wxRadioButton)->Enable(FALSE);
        XRCCTRL(dlg, "optnow",wxRadioButton)->Enable(FALSE);
        XRCCTRL(dlg, "optschedule",wxRadioButton)->Enable(FALSE);

        oldname = currentfile->GetFirstUrl().GetFullName();
        if ((currentfile->GetStatus() != STATUS_STOPED) && (currentfile->GetStatus() != STATUS_FINISHED) && (currentfile->GetStatus() != STATUS_ERROR))
        {
            XRCCTRL(dlg, "edturl",wxTextCtrl)->SetEditable(FALSE);
            XRCCTRL(dlg, "edtdestination",wxTextCtrl)->SetEditable(FALSE);
            XRCCTRL(dlg, "edtuser",wxTextCtrl)->SetEditable(FALSE);
            XRCCTRL(dlg, "edtpassword",wxTextCtrl)->SetEditable(FALSE);
            XRCCTRL(dlg, "edtcomments",wxTextCtrl)->SetEditable(FALSE);
            XRCCTRL(dlg, "edtreferenceurl",wxTextCtrl)->SetEditable(FALSE);
            XRCCTRL(dlg, "edtcommand",wxTextCtrl)->SetEditable(FALSE);
            lstaddresslist->Enable(FALSE);
            XRCCTRL(dlg,"btndir",wxButton)->Enable(FALSE);
            XRCCTRL(dlg,"btnadd",wxButton)->Enable(FALSE);
            XRCCTRL(dlg, "spinbandwidth",wxSpinCtrl)->Enable(FALSE);
            XRCCTRL(dlg, "chkaddontop",wxCheckBox)->Enable(FALSE);
        }
        XRCCTRL(dlg, "spinsplit",wxSpinCtrl)->Enable(FALSE);

        #if wxCHECK_VERSION(2, 8, 0)
        dlg.SetInitialSize();
        #else
        dlg.SetBestFittingSize();
        #endif
        dlg.SetDifferentNamesPermition(FALSE);

        this->active = FALSE;
        if (dlg.ShowModal() == XRCID("btnok"))
        {
            wxFileName destination; destination.AssignDir(XRCCTRL(dlg, "edtdestination",wxTextCtrl)->GetValue());
            wxString user = XRCCTRL(dlg, "edtuser",wxTextCtrl)->GetValue();
            wxString password = XRCCTRL(dlg, "edtpassword",wxTextCtrl)->GetValue();
            wxString reference = XRCCTRL(dlg, "edtreferenceurl",wxTextCtrl)->GetValue();
            wxString command = XRCCTRL(dlg, "edtcommand",wxTextCtrl)->GetValue();
            wxString comments = XRCCTRL(dlg, "edtcomments",wxTextCtrl)->GetValue();
            int bandwidth = XRCCTRL(dlg, "spinbandwidth",wxSpinCtrl)->GetValue();

            mUrlList *urllist = new mUrlList();
            for (unsigned int i = 0; i < lstaddresslist->GetCount(); i++)
            {
                if (!lstaddresslist->IsChecked(i))
                    continue;
                mUrlName *urltmp = new mUrlName(lstaddresslist->GetString(i));
                urllist->Append(urltmp);
            }
            wxGetApp().downloadlist.ChangeDownload(currentfile,urllist,destination,user,password,reference,comments,command,bandwidth);

            //VERIFY IF THE USER CHANGED THE FILE NAME
            newname = urllist->GetFirst()->GetData()->GetFullName();
            if (newname != oldname)
            {
                wxGetApp().downloadlist.ChangeName(currentfile,newname);
                XRCCTRL(*this, "inprogresslist",mInProgressList )->SetItem(wxGetApp().downloadlist.IndexOf(currentfile),INPROGRESS_NAME,currentfile->GetName());
            }
            list->SetItem(wxGetApp().downloadlist.IndexOf(currentfile),INPROGRESS_URL,currentfile->GetFirstUrl().GetFullPath());

        }
        this->active = TRUE;
        dlg.Destroy();
    }
}

void mMainFrame::OnDownloadAgain(wxCommandEvent& event)
{
    int currentselection;
    mFinishedList *list = XRCCTRL(*this, "finishedlist",mFinishedList );
    if (list->GetCurrentSelection().GetCount() > 0)
    {
        mListSelection currentselectionlist = list->GetCurrentSelection();
        int nselection = currentselectionlist.GetCount()-1;
        for (int i = nselection ; i >= 0 ;i--)
        {
            currentselection = currentselectionlist.Item(i);
            #ifdef WXDFAST_PORTABLE
            wxFileConfig *config = new wxFileConfig(DFAST_REG, wxEmptyString, DFAST_REG + wxT(".ini"), wxEmptyString,
                                                    wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
            #else
            wxFileConfig *config = new wxFileConfig(DFAST_REG);
            #endif
            wxListItem item;
            int startoption,ontop;
            wxString url,destination,user,password,comments,reference,command;
            wxArrayString urlarray;
            int parts;
            item.SetId(currentselection);
            item.SetColumn(FINISHED_NAME);
            item.SetMask(wxLIST_MASK_DATA|wxLIST_MASK_STATE|wxLIST_MASK_TEXT|wxLIST_MASK_IMAGE);
            list->GetItem(item);

            config->SetPath(FINISHED_REG);
            config->SetPath(item.GetText());

            destination = wxEmptyString;
            config->Read(DESTINATION_REG,&destination);
            #ifdef WXDFAST_PORTABLE
            {
                #ifdef __WXMSW__
                wxFileName destinationtmp(destination);
                if (destinationtmp.GetVolume().Upper() == wxT("PORTABLE"))
                {
                    destinationtmp.SetVolume(wxGetApp().programvolume);
                    destination = destinationtmp.GetFullPath();
                }
                #endif
            }
            #endif

            parts = 1;
            config->Read(PARTS_REG,&parts);

            user = wxEmptyString;
            config->Read(USER_REG,&user);

            password = wxEmptyString;
            config->Read(PASSWORD_REG,&password);

            comments = wxEmptyString;
            config->Read(COMMENTS_REG,&comments);

            reference = wxEmptyString;
            config->Read(REFERENCE_REG,&reference);

            command = wxEmptyString;
            config->Read(COMMAND_REG,&command);

            bool existurl = TRUE;
            int count = 1;
            while (existurl)
            {
                url = wxEmptyString;
                config->Read(URL_REG + MyUtilFunctions::int2wxstr(count),&(url));
                if (url.IsEmpty())
                    break;
                else
                    urlarray.Add(url);
                count++;
            }
            config->SetPath(BACK_DIR_REG);

            if (programoptions.rememberboxnewoptions)
            {
                startoption = programoptions.laststartoption;
                ontop = programoptions.lastontopoption;
            }
            else
            {
                startoption = DEFAULT_START_OPTION;
                ontop = DEFAULT_ONTOP_OPTION;
            }

            if (NewDownload(urlarray, destination,-1, parts, user, password, reference, comments, command, startoption,ontop, FALSE,FALSE))
            {
                config->DeleteGroup(item.GetText());
                list->DeleteItem(currentselection);
                list->SelectUnselect(FALSE,-1);
            }
            delete config;
        }
    }
}

void mMainFrame::OnMove(wxCommandEvent& event)
{
    int currentselection = -1;
    mFinishedList *list = XRCCTRL(*this, "finishedlist",mFinishedList );
    if (list->GetCurrentSelection().GetCount() > 0)
    {
        wxString dir;
        dir = wxDirSelector(_("Select the directory:"));
        if (dir != wxEmptyString)
        {
            mListSelection currentselectionlist = list->GetCurrentSelection();
            if (dir.Mid(dir.Length()-1,1) != SEPARATOR_DIR)
                dir = dir + SEPARATOR_DIR;
            #ifdef WXDFAST_PORTABLE
            wxFileConfig *config = new wxFileConfig(DFAST_REG, wxEmptyString, DFAST_REG + wxT(".ini"), wxEmptyString,
                                                    wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
            #else
            wxFileConfig *config = new wxFileConfig(DFAST_REG);
            #endif
            wxListItem item;
            config->SetPath(FINISHED_REG);

            for (unsigned int i = 0; i < currentselectionlist.GetCount();i++)
            {
                currentselection = currentselectionlist.Item(i);

                wxString name,destination;
                item.SetId(currentselection);
                item.SetColumn(FINISHED_NAME);
                item.SetMask(wxLIST_MASK_DATA|wxLIST_MASK_STATE|wxLIST_MASK_TEXT|wxLIST_MASK_IMAGE);
                list->GetItem(item);

                config->SetPath(item.GetText());

                name = wxEmptyString;
                config->Read(NAME_REG,&name);

                destination = wxEmptyString;
                config->Read(DESTINATION_REG,&destination);
                #ifdef WXDFAST_PORTABLE
                {
                    #ifdef __WXMSW__
                    wxFileName destinationtmp(destination);
                    if (destinationtmp.GetVolume().Upper() == wxT("PORTABLE"))
                    {
                        destinationtmp.SetVolume(wxGetApp().programvolume);
                        destination = destinationtmp.GetFullPath();
                    }
                    #endif
                }
                #endif

                if (destination.Mid(destination.Length()-1,1) != SEPARATOR_DIR)
                    destination = destination + SEPARATOR_DIR;

                if (destination != dir)
                {
                    if (::wxFileExists(destination+name))
                    {
                        this->active = FALSE;
                        wxProgressDialog *dlg = new wxProgressDialog(_("Moving..."),_("Moving file..."));
                        wxLogNull logNo;
                        if (::wxCopyFile(destination+name,dir+name,TRUE))
                        {
                            dlg->Update(50);
                            #ifdef WXDFAST_PORTABLE
                            {
                                #ifdef __WXMSW__
                                wxFileName destinationtmp(dir);
                                if (destinationtmp.GetVolume().Lower() == wxGetApp().programvolume.Lower())
                                {
                                    destinationtmp.SetVolume(wxEmptyString);
                                    config->Write(DESTINATION_REG,wxT("PORTABLE:") + destinationtmp.GetFullPath());
                                }
                                else
                                    config->Write(DESTINATION_REG,dir);
                                #else
                                config->Write(DESTINATION_REG,dir);
                                #endif
                            }
                            #else
                            config->Write(DESTINATION_REG,dir);
                            #endif
                            ::wxRemoveFile(destination+name);
                        }
                        else
                            wxMessageBox(_("Error moving file."),_("Error..."),wxOK|wxICON_ERROR,this);
                        this->active = TRUE;
                        delete dlg;
                    }
                    else
                    {
                        wxString message = _("The file isn't in the path that it was saved initially.\nDo you want to change only the old path for the current one?");
                        message += wxT("\n\nFilename: ") + name;
                        if (wxMessageBox(message ,
                                _("Continue..."),wxYES | wxNO | wxICON_QUESTION, this) == wxYES)
                        {
                            #ifdef WXDFAST_PORTABLE
                            {
                                #ifdef __WXMSW__
                                wxFileName destinationtmp(dir);
                                if (destinationtmp.GetVolume().Lower() == wxGetApp().programvolume.Lower())
                                {
                                    destinationtmp.SetVolume(wxEmptyString);
                                    config->Write(DESTINATION_REG,wxT("PORTABLE:") + destinationtmp.GetFullPath());
                                }
                                else
                                    config->Write(DESTINATION_REG,dir);
                                #else
                                config->Write(DESTINATION_REG,dir);
                                #endif
                            }
                            #else
                            config->Write(DESTINATION_REG,dir);
                            #endif
                        }
                    }
                }
                config->SetPath(BACK_DIR_REG);
            }
            delete config;
            list->SelectUnselect(TRUE,currentselection);
        }
    }
}

void mMainFrame::OnCheckMD5(wxCommandEvent& event)
{
    int currentselection;
    mFinishedList *list = XRCCTRL(*this, "finishedlist",mFinishedList );
    if ((currentselection = list->GetCurrentLastSelection()) >= 0)
    {
        list->SetCurrentSelection(currentselection);
        #ifdef WXDFAST_PORTABLE
        wxFileConfig *config = new wxFileConfig(DFAST_REG, wxEmptyString, DFAST_REG + wxT(".ini"), wxEmptyString,
                                                wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
        #else
        wxFileConfig *config = new wxFileConfig(DFAST_REG);
        #endif
        wxListItem item;
        wxString name,destination,md5old, md5new;
        item.SetId(currentselection);
        item.SetColumn(FINISHED_NAME);
        item.SetMask(wxLIST_MASK_DATA|wxLIST_MASK_STATE|wxLIST_MASK_TEXT|wxLIST_MASK_IMAGE);
        list->GetItem(item);

        config->SetPath(FINISHED_REG);
        config->SetPath(item.GetText());

        name = wxEmptyString;
        config->Read(NAME_REG,&name);

        destination = wxEmptyString;
        config->Read(DESTINATION_REG,&destination);
        #ifdef WXDFAST_PORTABLE
        {
            #ifdef __WXMSW__
            wxFileName destinationtmp(destination);
            if (destinationtmp.GetVolume().Upper() == wxT("PORTABLE"))
            {
                destinationtmp.SetVolume(wxGetApp().programvolume);
                destination = destinationtmp.GetFullPath();
            }
            #endif
        }
        #endif
        if (destination.Mid(destination.Length()-1,1) != SEPARATOR_DIR)
            destination = destination + SEPARATOR_DIR;

        md5old = wxEmptyString;
        config->Read(MD5_REG,&md5old);

        if (::wxFileExists(destination + name))
        {
            this->active = FALSE;
            wxProgressDialog *waitbox = new wxProgressDialog(_("Checking file's MD5..."),_("Checking file's MD5..."));
            waitbox->Update(10);
            wxFileName filemd5 = wxFileName(destination + name);
            wxMD5 md5(filemd5);
            md5new = md5.GetDigest(TRUE); //TELL TO GETDIGETS THAT IS THE MAIN THREAD
            waitbox->Update(100);
            if (md5new == md5old)
                wxMessageBox(_("The file was verified successfully."), _("Success..."), wxOK | wxICON_INFORMATION,this);
            else
            {
                wxString msg;
                msg = _("The MD5 calculated previously is different from the current one.");
                msg = msg + _("\nOld MD5 =\t\t") + md5old;
                msg = msg + _("\nCurrent MD5 =\t") + md5new;
                wxMessageBox(msg, _("Error..."), wxOK | wxICON_ERROR,this);
            }
            delete waitbox;
            this->active = TRUE;
        }
        else
            wxMessageBox(_("File not found."), _("Error..."), wxOK | wxICON_ERROR,this);
        delete config;
    }
}

void mMainFrame::OnOpenDestination(wxCommandEvent& event)
{
    this->BrowserFile();
}

void mMainFrame::OnExportConf(wxCommandEvent& event)
{
     wxString dir;
    dir = wxDirSelector(_("Select the directory:"));
    if (dir != wxEmptyString)
    {

        if (ExportConf(dir))
            wxMessageBox(_("The configuration was exported successfully."), _("Success..."),wxOK|wxICON_INFORMATION,this);
        else
            wxMessageBox(_("Error exporting configuration."), _("Error..."),wxOK|wxICON_ERROR,this);
    }
}

bool mMainFrame::ExportConf(wxString dir)
{
    bool result;
    if (dir.Mid(dir.Length()-1,1) != SEPARATOR_DIR)
        dir = dir + SEPARATOR_DIR;
    wxString source, destination;
    #ifdef WXDFAST_PORTABLE
        source = wxEmptyString;
    #else
        source = wxGetHomeDir();
        if (source.Mid(source.Length()-1,1) != SEPARATOR_DIR)
            source = source + SEPARATOR_DIR;
    #endif

    #ifdef __WXMSW__
        source = source + DFAST_REG + wxT(".ini");
    #else
        #ifdef WXDFAST_PORTABLE
            source = source + DFAST_REG + wxT(".ini");
        #else
            source = source + wxT(".") + DFAST_REG;
        #endif
    #endif
    destination = dir + DFAST_REG + wxT(".conf");
    wxLogNull noLog;
    result = ::wxCopyFile(source,destination,TRUE);
    return result;
}

void mMainFrame::OnImportConf(wxCommandEvent& event)
{
     wxFileDialog *dlg;
    dlg = new wxFileDialog(this, _("Select the file..."), wxEmptyString, wxEmptyString, wxT("*.conf"));
    this->active = FALSE;
    if (dlg->ShowModal() == wxID_OK)
    {
        if (ImportConf(dlg->GetPath()))
        {
            wxMessageBox(_("The configuration was imported successfully.\nThe program will be restated now for the changes to take effect."), _("Success..."),wxOK|wxICON_INFORMATION,this);
            Iconize(TRUE);
            Close(TRUE);
        }
        else
            wxMessageBox(_("Error importing configuration."), _("Error..."),wxOK|wxICON_ERROR,this);
    }
    this->active = TRUE;
}

bool mMainFrame::ImportConf(wxString path)
{
    wxString source, destination;
    bool result;
    source = path;
    #ifdef WXDFAST_PORTABLE
        destination = wxEmptyString;
    #else
        destination = wxGetHomeDir();
        if (destination.Mid(destination.Length()-1,1) != SEPARATOR_DIR)
            destination = destination + SEPARATOR_DIR;
    #endif

    #ifdef __WXMSW__
        destination = destination + DFAST_REG + wxT(".ini");
    #else
        #ifdef WXDFAST_PORTABLE
        destination = destination + DFAST_REG + wxT(".ini");
        #else
        destination = destination + wxT(".") + DFAST_REG;
        #endif
    #endif
    wxLogNull noLog;
    if (::wxFileExists(destination))
        ::wxCopyFile(destination,destination + wxT(".bak"),TRUE);
    result = ::wxCopyFile(source,destination,TRUE);
    return result;
}

void mMainFrame::OnShutdown(wxCommandEvent& event)
{
    if (menubar->GetMenu(3)->IsChecked(XRCID("menushutdown")))
    {
        programoptions.shutdown = TRUE;
        wxMessageBox(_("Command :\t" ) + programoptions.shutdowncmd,_("Shutdown Command..."),wxOK|wxICON_INFORMATION,this);
    }
    else
        programoptions.shutdown = FALSE;
}

void mMainFrame::OnDisconnect(wxCommandEvent& event)
{
    if (menubar->GetMenu(3)->IsChecked(XRCID("menudisconnect")))
    {
        programoptions.disconnect = TRUE;
        wxMessageBox(_("Command :\t" ) + programoptions.disconnectcmd,_("Disconnect Command..."),wxOK|wxICON_INFORMATION,this);
    }
    else
        programoptions.disconnect = FALSE;
}

void mMainFrame::OnOptions(wxCommandEvent& event)
{

    mBoxOptions dlg;
    wxXmlResource::Get()->LoadDialog(&dlg, this, wxT("boxoptions"));
    unsigned int i,j;
    int oldgraphheight = programoptions.graphheight;

    XRCCTRL(dlg, "spinattempts", wxSpinCtrl)->SetValue(programoptions.attempts);
    XRCCTRL(dlg, "spinattemptstime", wxSpinCtrl)->SetValue(programoptions.attemptstime);
    XRCCTRL(dlg, "spinsimultaneous", wxSpinCtrl)->SetValue(programoptions.simultaneous);
    XRCCTRL(dlg, "edtshutdown",wxTextCtrl)->SetValue(programoptions.shutdowncmd);
    XRCCTRL(dlg, "edtdisconnect",wxTextCtrl)->SetValue(programoptions.disconnectcmd);
    XRCCTRL(dlg, "chkclosedialog",wxCheckBox)->SetValue(programoptions.closedialog);
    XRCCTRL(dlg, "chkshutdown",wxCheckBox)->SetValue(programoptions.alwaysshutdown);
    XRCCTRL(dlg, "chkdisconnect",wxCheckBox)->SetValue(programoptions.alwaysdisconnect);
    XRCCTRL(dlg, "chkrememberboxnewoptions",wxCheckBox)->SetValue(programoptions.rememberboxnewoptions);
    XRCCTRL(dlg, "edtdestination",wxTextCtrl)->SetValue(programoptions.destination);
    XRCCTRL(dlg, "edtbrowserpath",wxTextCtrl)->SetValue(programoptions.browserpath);
    XRCCTRL(dlg, "edtfilemanagerpath",wxTextCtrl)->SetValue(programoptions.filemanagerpath);

    #ifdef WXDFAST_PORTABLE
    XRCCTRL(dlg, "optdefaulttemp",wxRadioButton)->Enable(false);
    XRCCTRL(dlg, "optusertemp",wxRadioButton)->Enable(false);
    XRCCTRL(dlg, "edttemppath",wxTextCtrl)->Enable(false);
    #endif
    if (programoptions.downloadpartsdefaultdir.IsEmpty())
        XRCCTRL(dlg, "optdefaulttemp",wxRadioButton)->SetValue(TRUE);
    else
        XRCCTRL(dlg, "optusertemp",wxRadioButton)->SetValue(TRUE);
    XRCCTRL(dlg, "edttemppath",wxTextCtrl)->SetValue(programoptions.downloadpartsdefaultdir);

    XRCCTRL(dlg, "spintimerinterval",wxSpinCtrl)->SetValue(programoptions.timerupdateinterval);
    XRCCTRL(dlg, "spinreadbuffersize",wxSpinCtrl)->SetValue(programoptions.readbuffersize);
    XRCCTRL(dlg, "spingraphpoints",wxSpinCtrl)->SetValue(programoptions.graphhowmanyvalues);
    XRCCTRL(dlg, "spingraphrefreshrate",wxSpinCtrl)->SetValue(programoptions.graphrefreshtime);
    XRCCTRL(dlg, "spingraphmaxvalue",wxSpinCtrl)->SetValue(programoptions.graphscale);
    XRCCTRL(dlg, "spingraphtextarea",wxSpinCtrl)->SetValue(programoptions.graphtextarea);
    XRCCTRL(dlg, "spingraphheight",wxSpinCtrl)->SetValue(programoptions.graphheight);
    XRCCTRL(dlg, "spingraphfontsize",wxSpinCtrl)->SetValue(programoptions.graphspeedfontsize);
    XRCCTRL(dlg, "spingraphlinewidth",wxSpinCtrl)->SetValue(programoptions.graphlinewidth);
    XRCCTRL(dlg, "chkrestoremainframe",wxCheckBox)->SetValue(programoptions.restoremainframe);
    XRCCTRL(dlg, "chkhidemainframe",wxCheckBox)->SetValue(programoptions.hidemainframe);
    XRCCTRL(dlg, "chkcheckforupdates",wxCheckBox)->SetValue(programoptions.checkforupdates);
    XRCCTRL(dlg, "chkgraphshow",wxCheckBox)->SetValue(programoptions.graphshow);
    XRCCTRL(dlg, "graphpanelback", mBoxOptionsColorPanel)->colour = programoptions.graphbackcolor;
    XRCCTRL(dlg, "graphpanelgrid", mBoxOptionsColorPanel)->colour = programoptions.graphgridcolor;
    XRCCTRL(dlg, "graphpanelline", mBoxOptionsColorPanel)->colour = programoptions.graphlinecolor;
    XRCCTRL(dlg, "graphpanelfont", mBoxOptionsColorPanel)->colour = programoptions.graphfontcolor;
    XRCCTRL(dlg, "chkactivatescheduling",wxCheckBox)->SetValue(programoptions.activatescheduling);
    XRCCTRL(dlg, "edtstartdate",wxTextCtrl)->SetValue(programoptions.startdatetime.Format(wxT("%Y/%m/%d")));
    XRCCTRL(dlg, "edtfinishdate",wxTextCtrl)->SetValue(programoptions.finishdatetime.Format(wxT("%Y/%m/%d")));
    XRCCTRL(dlg, "spinstarthour",wxSpinCtrl)->SetValue(programoptions.startdatetime.GetHour());
    XRCCTRL(dlg, "spinstartminute",wxSpinCtrl)->SetValue(programoptions.startdatetime.GetMinute());
    XRCCTRL(dlg, "spinfinishhour",wxSpinCtrl)->SetValue(programoptions.finishdatetime.GetHour());
    XRCCTRL(dlg, "spinfinishminute",wxSpinCtrl)->SetValue(programoptions.finishdatetime.GetMinute());
    for (i=0; i<7;i++)
        XRCCTRL(dlg, "comboweekdays",wxComboBox)->Append(
                                        wxGetTranslation(days[i]));
    XRCCTRL(dlg, "comboweekdays",wxComboBox)->SetValue(wxGetTranslation(days[0]));
    for (i=0;i<MAX_SCHEDULE_EXCEPTIONS;i++)
    {
        if (programoptions.scheduleexceptions[i].isactive)
        {
            wxString temp = programoptions.scheduleexceptions[i].start + wxT(" | ") +
                            programoptions.scheduleexceptions[i].finish + wxT(" | ") +
                            XRCCTRL(dlg, "comboweekdays",wxComboBox)->GetString(programoptions.scheduleexceptions[i].day);
            XRCCTRL(dlg, "lstexceptionlist",wxListBox)->InsertItems(1,&temp,0);
        }
    }
    if (programoptions.bandwidthoption == 1)
        XRCCTRL(dlg, "optbandwidthindependently", wxRadioButton)->SetValue(TRUE);
    else if (programoptions.bandwidthoption == 2)
        XRCCTRL(dlg, "optbandwidthcustom", wxRadioButton)->SetValue(TRUE);
    else
        XRCCTRL(dlg, "optbandwidthunlimited", wxRadioButton)->SetValue(TRUE);
    XRCCTRL(dlg, "spinbandwithcustom", wxSpinCtrl)->SetValue(programoptions.bandwidth);
    XRCCTRL(dlg, "chkproxy",wxCheckBox)->SetValue(programoptions.proxy);
    XRCCTRL(dlg, "edtproxyserver",wxTextCtrl)->SetValue(programoptions.proxy_server);
    XRCCTRL(dlg, "edtproxyport",wxTextCtrl)->SetValue(programoptions.proxy_port);
    if (!programoptions.proxy_username.IsEmpty())
    {
        XRCCTRL(dlg, "edtproxyusername",wxTextCtrl)->SetValue(programoptions.proxy_username);
        XRCCTRL(dlg, "edtproxypassword",wxTextCtrl)->SetValue(wxT("#NOPASSWORD#"));
    }


    this->active = FALSE;
    if (dlg.ShowModal() == XRCID("btnoptionsave"))
    {
        wxProgressDialog *waitbox = new wxProgressDialog(_("Updating the configuration..."),_("Updating and saving the configuration..."),100,NULL,wxPD_AUTO_HIDE | wxPD_APP_MODAL|wxPD_CAN_ABORT|wxPD_ELAPSED_TIME);
        waitbox->Update(0);
        #ifndef DISABLE_MUTEX
        mutex_programoptions->Lock();
        #endif

        programoptions.attempts = XRCCTRL(dlg, "spinattempts", wxSpinCtrl)->GetValue();
        programoptions.attemptstime = XRCCTRL(dlg, "spinattemptstime", wxSpinCtrl)->GetValue();
        programoptions.simultaneous = XRCCTRL(dlg, "spinsimultaneous", wxSpinCtrl)->GetValue();
        programoptions.shutdowncmd = XRCCTRL(dlg, "edtshutdown",wxTextCtrl)->GetValue();
        programoptions.disconnectcmd = XRCCTRL(dlg, "edtdisconnect",wxTextCtrl)->GetValue();
        programoptions.closedialog = XRCCTRL(dlg, "chkclosedialog",wxCheckBox)->GetValue();
        programoptions.alwaysshutdown = XRCCTRL(dlg, "chkshutdown",wxCheckBox)->GetValue();
        programoptions.alwaysdisconnect = XRCCTRL(dlg, "chkdisconnect",wxCheckBox)->GetValue();
        programoptions.rememberboxnewoptions = XRCCTRL(dlg, "chkrememberboxnewoptions",wxCheckBox)->GetValue();
        if (programoptions.destination != XRCCTRL(dlg, "edtdestination",wxTextCtrl)->GetValue())
        {
            programoptions.destination = XRCCTRL(dlg, "edtdestination",wxTextCtrl)->GetValue();
            programoptions.lastdestination = programoptions.destination;
        }
        programoptions.browserpath = XRCCTRL(dlg, "edtbrowserpath",wxTextCtrl)->GetValue();
        programoptions.filemanagerpath = XRCCTRL(dlg, "edtfilemanagerpath",wxTextCtrl)->GetValue();

        if (XRCCTRL(dlg, "optdefaulttemp",wxRadioButton)->GetValue())
            programoptions.downloadpartsdefaultdir = wxEmptyString;
        else
            programoptions.downloadpartsdefaultdir = XRCCTRL(dlg, "edttemppath",wxTextCtrl)->GetValue();

        programoptions.timerupdateinterval = XRCCTRL(dlg, "spintimerinterval",wxSpinCtrl)->GetValue();
        programoptions.readbuffersize = XRCCTRL(dlg, "spinreadbuffersize",wxSpinCtrl)->GetValue();
        programoptions.restoremainframe = XRCCTRL(dlg, "chkrestoremainframe",wxCheckBox)->GetValue();
        programoptions.hidemainframe = XRCCTRL(dlg, "chkhidemainframe",wxCheckBox)->GetValue();
        programoptions.checkforupdates = XRCCTRL(dlg, "chkcheckforupdates",wxCheckBox)->GetValue();
        programoptions.graphshow = XRCCTRL(dlg, "chkgraphshow",wxCheckBox)->GetValue();
        programoptions.graphhowmanyvalues = XRCCTRL(dlg, "spingraphpoints",wxSpinCtrl)->GetValue();
        programoptions.graphrefreshtime = XRCCTRL(dlg, "spingraphrefreshrate",wxSpinCtrl)->GetValue();
        programoptions.graphscale = XRCCTRL(dlg, "spingraphmaxvalue",wxSpinCtrl)->GetValue();
        programoptions.graphtextarea = XRCCTRL(dlg, "spingraphtextarea",wxSpinCtrl)->GetValue();
        programoptions.graphheight = XRCCTRL(dlg, "spingraphheight",wxSpinCtrl)->GetValue();
        programoptions.graphspeedfontsize = XRCCTRL(dlg, "spingraphfontsize",wxSpinCtrl)->GetValue();
        programoptions.graphlinewidth = XRCCTRL(dlg, "spingraphlinewidth",wxSpinCtrl)->GetValue();
        programoptions.graphbackcolor = XRCCTRL(dlg, "graphpanelback", mBoxOptionsColorPanel)->colour;
        programoptions.graphgridcolor = XRCCTRL(dlg, "graphpanelgrid", mBoxOptionsColorPanel)->colour;
        programoptions.graphlinecolor = XRCCTRL(dlg, "graphpanelline", mBoxOptionsColorPanel)->colour;
        programoptions.graphfontcolor = XRCCTRL(dlg, "graphpanelfont", mBoxOptionsColorPanel)->colour;
        programoptions.activatescheduling = FALSE;
        programoptions.startdatetime.ParseDate(XRCCTRL(dlg, "edtstartdate",wxTextCtrl)->GetValue());
        programoptions.startdatetime.SetHour(XRCCTRL(dlg, "spinstarthour",wxSpinCtrl)->GetValue());
        programoptions.startdatetime.SetMinute(XRCCTRL(dlg, "spinstartminute",wxSpinCtrl)->GetValue());
        programoptions.finishdatetime.ParseDate(XRCCTRL(dlg, "edtfinishdate",wxTextCtrl)->GetValue());
        programoptions.finishdatetime.SetHour(XRCCTRL(dlg, "spinfinishhour",wxSpinCtrl)->GetValue());
        programoptions.finishdatetime.SetMinute(XRCCTRL(dlg, "spinfinishminute",wxSpinCtrl)->GetValue());
        waitbox->Update(20);
        for (i=0;i<MAX_SCHEDULE_EXCEPTIONS;i++)
        {
            if (i < XRCCTRL(dlg, "lstexceptionlist",wxListBox)->GetCount())
            {
                wxString temp = XRCCTRL(dlg, "lstexceptionlist",wxListBox)->GetString(i);
                programoptions.scheduleexceptions[i].newstart = temp.Mid(0,5);
                programoptions.scheduleexceptions[i].newfinish = temp.Mid(8,5);
                for (j = 0;j <7;j++)
                    if (temp.Mid(16) == wxGetTranslation(days[j]))
                    {
                        programoptions.scheduleexceptions[i].newday = j;
                        break;
                    }
            }
            else
            {
                programoptions.scheduleexceptions[i].newstart = wxEmptyString;
                programoptions.scheduleexceptions[i].newfinish = wxEmptyString;
                programoptions.scheduleexceptions[i].newday = 0;
            }
            mApplication::Configurations(WRITE,OPT_SCHED_SCHEDULEEXCEPTION_ISACTIVE_REG+MyUtilFunctions::int2wxstr(i), programoptions.scheduleexceptions[i].isactive);
            mApplication::Configurations(WRITE,OPT_SCHED_SCHEDULEEXCEPTION_START_REG+MyUtilFunctions::int2wxstr(i), programoptions.scheduleexceptions[i].start);
            mApplication::Configurations(WRITE,OPT_SCHED_SCHEDULEEXCEPTION_FINISH_REG+MyUtilFunctions::int2wxstr(i), programoptions.scheduleexceptions[i].finish);
            mApplication::Configurations(WRITE,OPT_SCHED_SCHEDULEEXCEPTION_DAY_REG+MyUtilFunctions::int2wxstr(i), programoptions.scheduleexceptions[i].day);
        }
        programoptions.scheduleexceptionschanged = TRUE;
        if ((XRCCTRL(dlg, "chkactivatescheduling",wxCheckBox)->GetValue()) && (programoptions.startdatetime < programoptions.finishdatetime))
            programoptions.activatescheduling = TRUE;
        else
            programoptions.activatescheduling = FALSE;

        if (XRCCTRL(dlg, "optbandwidthindependently", wxRadioButton)->GetValue())
        {
            programoptions.bandwidthoption = 1;
            if (statusbar)
            {
                wxString temp = _("Band control");
                temp += wxT(": ");
                temp += _("Per Download");
                statusbar->SetStatusText(temp,2);
            }
        }
        else if (XRCCTRL(dlg, "optbandwidthcustom", wxRadioButton)->GetValue())
        {
            programoptions.bandwidthoption = 2;
            programoptions.bandwidth = XRCCTRL(dlg, "spinbandwithcustom", wxSpinCtrl)->GetValue();
            if (statusbar)
            {
                wxString temp = _("Band control");
                temp += wxT(": ");
                temp += _("Active");
                temp += wxT(" (") + MyUtilFunctions::int2wxstr(programoptions.bandwidth) + wxT(" kB/s)");
                statusbar->SetStatusText(temp,2);
            }
        }
        else
        {
            programoptions.bandwidthoption = 0;
            if (statusbar)
            {
                wxString temp = _("Band control");
                temp += wxT(": ");
                temp += _("Unlimited");
                statusbar->SetStatusText(temp,2);
            }
        }
        programoptions.proxy = XRCCTRL(dlg, "chkproxy",wxCheckBox)->GetValue();
        if (programoptions.proxy)
        {
            programoptions.proxy_server = XRCCTRL(dlg, "edtproxyserver",wxTextCtrl)->GetValue();
            programoptions.proxy_port = XRCCTRL(dlg, "edtproxyport",wxTextCtrl)->GetValue();
            programoptions.proxy_username = XRCCTRL(dlg, "edtproxyusername",wxTextCtrl)->GetValue();
            if (!programoptions.proxy_username.IsEmpty())
            {
                if (XRCCTRL(dlg, "edtproxypassword",wxTextCtrl)->GetValue() != wxT("#NOPASSWORD#"))
                    programoptions.proxy_authstring = MyUtilFunctions::GenerateAuthString(programoptions.proxy_username,
                                                                        XRCCTRL(dlg, "edtproxypassword",wxTextCtrl)->GetValue());
            }
            else
                programoptions.proxy_authstring = wxEmptyString;
        }
        else
        {
            programoptions.proxy_username = wxEmptyString;
            programoptions.proxy_authstring = wxEmptyString;
        }

        waitbox->Update(50);
        mApplication::Configurations(WRITE,OPT_DIALOG_CLOSE_REG,programoptions.closedialog);
        mApplication::Configurations(WRITE,OPT_REMEMBER_BOXNEW_OPTIONS_REG,programoptions.rememberboxnewoptions);
        mApplication::Configurations(WRITE,OPT_DESTINATION_REG,programoptions.destination);
        mApplication::Configurations(WRITE,OPT_BROWSER_PATH_REG,programoptions.browserpath);
        mApplication::Configurations(WRITE,OPT_FILE_MANAGER_PATH_REG,programoptions.filemanagerpath);
        mApplication::Configurations(WRITE,OPT_DOWNLOAD_PARTS_DEFAULT_DIR_REG,programoptions.downloadpartsdefaultdir);
        mApplication::Configurations(WRITE,OPT_ATTEMPTS_REG,programoptions.attempts);
        mApplication::Configurations(WRITE,OPT_ATTEMPTS_TIME_REG,programoptions.attemptstime);
        mApplication::Configurations(WRITE,OPT_SIMULTANEOUS_REG,programoptions.simultaneous);
        mApplication::Configurations(WRITE,OPT_SHUTDOWN_REG,programoptions.alwaysshutdown);
        mApplication::Configurations(WRITE,OPT_DISCONNECT_REG,programoptions.alwaysdisconnect);
        mApplication::Configurations(WRITE,OPT_SHUTDOWN_CMD_REG,programoptions.shutdowncmd);
        mApplication::Configurations(WRITE,OPT_DISCONNECT_CMD_REG,programoptions.disconnectcmd);
        mApplication::Configurations(WRITE,OPT_TIMERINTERVAL_REG,programoptions.timerupdateinterval);
        mApplication::Configurations(WRITE,OPT_READBUFFERSIZE_REG,programoptions.readbuffersize);
        mApplication::Configurations(WRITE,OPT_RESTORE_MAINFRAME_REG, programoptions.restoremainframe);
        mApplication::Configurations(WRITE,OPT_HIDE_MAINFRAME_REG, programoptions.hidemainframe);
        mApplication::Configurations(WRITE,OPT_CHECK_FOR_UPDATES_REG, programoptions.checkforupdates);
        mApplication::Configurations(WRITE,OPT_GRAPH_SHOW_REG,programoptions.graphshow);
        mApplication::Configurations(WRITE,OPT_GRAPH_HOWMANYVALUES_REG, programoptions.graphhowmanyvalues);
        mApplication::Configurations(WRITE,OPT_GRAPH_REFRESHTIME_REG, programoptions.graphrefreshtime);
        mApplication::Configurations(WRITE,OPT_GRAPH_SCALE_REG, programoptions.graphscale);
        mApplication::Configurations(WRITE,OPT_GRAPH_TEXTAREA_REG, programoptions.graphtextarea);
        mApplication::Configurations(WRITE,OPT_GRAPH_HEIGHT_REG, programoptions.graphheight);
        mApplication::Configurations(WRITE,OPT_GRAPH_SPEEDFONTSIZE_REG, programoptions.graphspeedfontsize);
        mApplication::Configurations(WRITE,OPT_GRAPH_LINEWIDTH_REG, programoptions.graphlinewidth);
        waitbox->Update(70);
        {
            wxString red,green,blue;
            red.Printf(wxT("%.3d"), programoptions.graphbackcolor.Red());
            green.Printf(wxT("%.3d"), programoptions.graphbackcolor.Green());
            blue.Printf(wxT("%.3d"), programoptions.graphbackcolor.Blue());
            mApplication::Configurations(WRITE,OPT_GRAPH_COLORBACK_REG, red + wxT("-") + green + wxT("-") + blue);
        }
        {
            wxString red,green,blue;
            red.Printf(wxT("%.3d"), programoptions.graphgridcolor.Red());
            green.Printf(wxT("%.3d"), programoptions.graphgridcolor.Green());
            blue.Printf(wxT("%.3d"), programoptions.graphgridcolor.Blue());
            mApplication::Configurations(WRITE,OPT_GRAPH_COLORGRID_REG, red + wxT("-") + green + wxT("-") + blue);
        }
        {
            wxString red,green,blue;
            red.Printf(wxT("%.3d"), programoptions.graphlinecolor.Red());
            green.Printf(wxT("%.3d"), programoptions.graphlinecolor.Green());
            blue.Printf(wxT("%.3d"), programoptions.graphlinecolor.Blue());
            mApplication::Configurations(WRITE,OPT_GRAPH_COLORLINE_REG, red + wxT("-") + green + wxT("-") + blue);
        }
        {
            wxString red,green,blue;
            red.Printf(wxT("%.3d"), programoptions.graphfontcolor.Red());
            green.Printf(wxT("%.3d"), programoptions.graphfontcolor.Green());
            blue.Printf(wxT("%.3d"), programoptions.graphfontcolor.Blue());
            mApplication::Configurations(WRITE,OPT_GRAPH_COLORFONT_REG, red + wxT("-") + green + wxT("-") + blue);
        }
        waitbox->Update(90);
        mApplication::Configurations(WRITE,OPT_SCHED_ACTIVATESCHEDULING_REG, programoptions.activatescheduling);
        mApplication::Configurations(WRITE,OPT_SCHED_STARTDATETIME_REG, (long)programoptions.startdatetime.GetTicks());
        mApplication::Configurations(WRITE,OPT_SCHED_FINISHDATETIME_REG, (long)programoptions.finishdatetime.GetTicks());
        mApplication::Configurations(WRITE,OPT_BAND_WIDTH_OPTION_REG, programoptions.bandwidthoption);
        mApplication::Configurations(WRITE,OPT_BAND_WIDTH_GENERAL_REG, programoptions.bandwidth);
        mApplication::Configurations(WRITE,OPT_PROXY_REG,programoptions.proxy);
        mApplication::Configurations(WRITE,OPT_PROXY_SERVER_REG,programoptions.proxy_server);
        mApplication::Configurations(WRITE,OPT_PROXY_PORT_REG,programoptions.proxy_port);
        mApplication::Configurations(WRITE,OPT_PROXY_USERNAME_REG,programoptions.proxy_username);
        mApplication::Configurations(WRITE,OPT_PROXY_AUTHSTRING_REG,programoptions.proxy_authstring);

        ShowHideResizeGraph(oldgraphheight); //VERIFY IF THE GRAPH SIZE AND STATUS CHANGED

        waitbox->Update(100);
        delete waitbox;
        #ifndef DISABLE_MUTEX
        mutex_programoptions->Unlock();
        #endif
    }
    this->active = TRUE;
}

void mMainFrame::OnSite(wxCommandEvent& event)
{
    if (::wxFileExists(programoptions.browserpath))
        ::wxExecute(programoptions.browserpath + wxT(" \"http://dfast.sourceforge.net\""));
    else
    {
        wxMessageBox(_("Unable to find the browser.\nGo to \"Options\" and specify a valid one."),
                _("Error..."),wxOK | wxICON_ERROR, this);
    }
}

void mMainFrame::OnBug(wxCommandEvent& event)
{
    if (::wxFileExists(programoptions.browserpath))
        ::wxExecute(programoptions.browserpath + wxT(" \"http://dfast.sourceforge.net/reportbugs.php\""));
    else
    {
        wxMessageBox(_("Unable to find the browser.\nGo to \"Options\" and specify a valid one."),
                _("Error..."),wxOK | wxICON_ERROR, this);
    }
}

void mMainFrame::OnDonate(wxCommandEvent& event)
{
    if (::wxFileExists(programoptions.browserpath))
        ::wxExecute(programoptions.browserpath + wxT(" \"http://dfast.sourceforge.net/donate.php\""));
    else
    {
        wxMessageBox(_("Unable to find the browser.\nGo to \"Options\" and specify a valid one."),
                _("Error..."),wxOK | wxICON_ERROR, this);
    }
}

void mMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxDialog dlg;
    wxXmlResource::Get()->LoadDialog(&dlg, this, wxT("boxabout"));
    wxString aboutstring;
    aboutstring  = _("Version: ") + VERSION + wxT("\n");
    aboutstring += _("Author: ");
    aboutstring += wxT("Max Magalhães Velasques\n\n");
    aboutstring += _("I'd like to thank the following contributors for their help:\n");
    aboutstring += wxT("Anthony Brian");
    aboutstring += _(" - several tips and donations\n");
    aboutstring += wxT("Erno Szabados");
    aboutstring += _(" - Ripstop theme\n\n");
    aboutstring += _("Visit our website:\n");
    aboutstring += wxT("http://dfast.sourceforge.net");
    aboutstring += wxT("\n\n");
    aboutstring += _("Help to improve the program!\n");
    aboutstring += _("Donate to this project!");

    XRCCTRL(dlg, "wxdfastabouttext",wxTextCtrl)->SetValue(aboutstring);

    dlg.ShowModal();
    if (programoptions.checkforupdates)
        CheckNewRelease();
}

void mMainFrame::OnExit(wxCommandEvent& event)
{
   Close();
}

void mMainFrame::OnClose(wxCloseEvent& event)
{
    int closedialog = programoptions.closedialog;
    if (event.CanVeto())
    {
        if ((closedialog) && (this->IsShown()))
        {
            wxMessageDialog *dlg = new wxMessageDialog(this, _("Do you want to close the program?"),_("Close..."),
                        wxYES_NO | wxICON_QUESTION);
            if (dlg)
            {
                dlg->Centre(wxBOTH);
                this->active = FALSE;
                if (dlg->ShowModal() == wxID_YES)
                {
                    this->active = TRUE;
                    event.Skip();
                    dlg->Destroy();
                }
                else
                {
                    this->active = TRUE;
                    event.Veto();
                    dlg->Destroy();
                    return ;
                }
            }
        }
        else
        {
            event.Skip();
        }
    }
    else
        event.Skip();
    mtimer->Stop();
    wxCommandEvent stopwithoutsavingstopstatus;
    stopwithoutsavingstopstatus.SetString(STOPWITHOUTSAVINGSTOPSTATUS);
    OnStopAll(stopwithoutsavingstopstatus);
}

void mMainFrame::OnToolLeftClick(wxCommandEvent& event)
{
    int id = event.GetId();
    if (id == XRCID("toolnew"))
    {
        OnNew(event);
    }
    else if (id == XRCID("toolremove"))
    {
        OnRemove(event);
    }
    else if (id == XRCID("toolschedule"))
    {
        OnSchedule(event);
    }
    else if (id == XRCID("toolstart"))
    {
        OnStart(event);
    }
    else if (id == XRCID("toolstop"))
    {
        OnStop(event);
    }
    else if (id == XRCID("toolstartall"))
    {
        OnStartAll(event);
    }
    else if (id == XRCID("toolstopall"))
    {
        OnStopAll(event);
    }
    else if (id == XRCID("toolup"))
    {
        OnUpDown(TRUE);
    }
    else if (id == XRCID("tooldown"))
    {
        OnUpDown(FALSE);
    }
    else if (id == XRCID("toolproperties"))
    {
        OnProperties(event);
    }
    else if (id == XRCID("toolclose"))
    {
        OnExit(event);
    }
}

void mMainFrame::OnToolMouseMove(wxCommandEvent& event)
{
    if (!statusbar)
        return;
    int selection = event.GetSelection();
    if (selection == XRCID("toolnew"))
    {
        statusbar->SetStatusText(_("Add new download"));
    }
    else if (selection == XRCID("toolremove"))
    {
        statusbar->SetStatusText(_("Remove the selected download"));
    }
    else if (selection == XRCID("toolschedule"))
    {
        statusbar->SetStatusText(_("Mark the selected download as scheduled"));
    }
    else if (selection == XRCID("toolstart"))
    {
        statusbar->SetStatusText(_("Start the selected download"));
    }
    else if (selection == XRCID("toolstop"))
    {
        statusbar->SetStatusText(_("Stop the selected download"));
    }
    else if (selection == XRCID("toolstartall"))
    {
        statusbar->SetStatusText(_("Start all downloads"));
    }
    else if (selection == XRCID("toolstopall"))
    {
        statusbar->SetStatusText(_("Stop all downloads"));
    }
    else if (selection == XRCID("toolup"))
    {
        statusbar->SetStatusText(_("Raise in one level the selected download in the queue"));
    }
    else if (selection == XRCID("tooldown"))
    {
        statusbar->SetStatusText(_("Lower in one level the selected download in the queue"));
    }
    else if (selection == XRCID("toolproperties"))
    {
        statusbar->SetStatusText(_("Show the properties of the selected download"));
    }
    else if (selection == XRCID("toolclose"))
    {
        statusbar->SetStatusText(_("Close the program"));
    }
    else
        statusbar->SetStatusText(this->defaultstatusbarmessage);
}

void mMainFrame::BrowserFile()
{
    int selection;
    mFinishedList *list = XRCCTRL(*this, "finishedlist",mFinishedList );
    if ((selection = list->GetCurrentLastSelection()) >= 0)
    {
        list->SetCurrentSelection(selection);
        #ifdef WXDFAST_PORTABLE
        wxFileConfig *config = new wxFileConfig(DFAST_REG, wxEmptyString, DFAST_REG + wxT(".ini"), wxEmptyString,
                                                wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
        #else
        wxFileConfig *config = new wxFileConfig(DFAST_REG);
        #endif
        wxString filepath;
        config->SetPath(FINISHED_REG);
        wxListItem item;
        item.SetId(selection);
        item.SetColumn(FINISHED_NAME);
        item.SetMask(wxLIST_MASK_DATA|wxLIST_MASK_STATE|wxLIST_MASK_TEXT|wxLIST_MASK_IMAGE);
        list->GetItem(item);
        config->SetPath(item.GetText());
        filepath = wxEmptyString;
        config->Read(DESTINATION_REG,&filepath);
        #ifdef WXDFAST_PORTABLE
        {
            #ifdef __WXMSW__
            wxFileName destinationtmp(filepath);
            if (destinationtmp.GetVolume().Upper() == wxT("PORTABLE"))
            {
                destinationtmp.SetVolume(wxGetApp().programvolume);
                filepath = destinationtmp.GetFullPath();
            }
            #endif
        }
        #endif
        delete config;
        ::wxExecute(programoptions.filemanagerpath + wxT(" \"") + filepath + wxT("\""));
    }
}

void mMainFrame::OnOpenURL(wxCommandEvent& event)
{
    if (wxMessageBox(_("You have downloaded a HTML file.\nOpen this file in your browser?"),
                    _("Continue..."),wxYES | wxNO | wxICON_QUESTION, this) == wxYES)
    {
        if (::wxFileExists(programoptions.browserpath))
            ::wxExecute(programoptions.browserpath + wxT(" \"") + event.GetString() + wxT("\""));
        else
        {
            wxMessageBox(_("Unable to find the browser.\nGo to \"Options\" and specify a valid one."),
                    _("Error..."),wxOK | wxICON_ERROR, this);
        }
    }
}

void mMainFrame::OnShutdownEvent(wxCommandEvent& event)
{
    wxStopWatch waittime;
    waittime.Start();
    wxProgressDialog *waitbox = new wxProgressDialog(_("Shutting down the computer..."),_("The computer will shut down in seconds..."),30000,NULL,wxPD_AUTO_HIDE | wxPD_APP_MODAL|wxPD_CAN_ABORT|wxPD_REMAINING_TIME);
    while ((waittime.Time() < 30000) && (waitbox->Update(waittime.Time())))
        wxMilliSleep(200);
    delete waitbox;
    if (waittime.Time() >= 30000)
    {
        if (this->IsShown())
        {
            wxCommandEvent iconizeevent;
            taskbaricon->OnHide(iconizeevent);
        }
        wxMilliSleep(200);
        ::wxExecute(programoptions.shutdowncmd);
    }
}

void mMainFrame::OnDisconnectEvent(wxCommandEvent& event)
{
    wxStopWatch waittime;
    waittime.Start();
    wxProgressDialog *waitbox = new wxProgressDialog(_("Disconnecting from the Internet..."),_("The Internet connection will be closed in seconds..."),30000,NULL,wxPD_AUTO_HIDE | wxPD_APP_MODAL|wxPD_CAN_ABORT|wxPD_REMAINING_TIME);
    while ((waitbox->Update(waittime.Time())) && (waittime.Time() < 30000))
        wxMilliSleep(200);
    delete waitbox;
    if (waittime.Time() >= 30000)
        ::wxExecute(programoptions.disconnectcmd);
}

void mMainFrame::OnExecuteEvent(wxCommandEvent& event)
{
    ::wxExecute(event.GetString());
}

bool mMainFrame::UpdateListItemField(mDownloadFile *current)
{
    mInProgressList* list01 = XRCCTRL(*this, "inprogresslist",mInProgressList );
    bool result = FALSE;
    if (current)
    {
        if (!current->WaitingForSplit())
        {
            int i,parts;
            current->totalsizecompleted = 0;

            for (i=0; i < current->GetNumberofParts(); i++)
                current->totalsizecompleted += current->sizecompleted[i];
            if (current->totalspeed > 0)
                current->timeremaining = wxLongLong(0l,1000l)*(current->totalsize - current->totalsizecompleted)/wxLongLong(0l,current->totalspeed);
            if (current->totalsize > 0)
                current->SetProgress((int)(100*( MyUtilFunctions::wxlonglongtodouble(current->totalsizecompleted)) / ( MyUtilFunctions::wxlonglongtodouble(current->totalsize))));

            if ((!current->IsSplitted()) && (current->GetStatus() == STATUS_ACTIVE))
                parts = 1;
            else
                parts = current->GetNumberofParts();

            int proportion = 100/parts;
            for (int i = 0; i < parts;i++)
            {
                if (current->totalsize > 0)
                {
                    if (i == (parts-1))
                    {
                        current->percentualparts[i] = (100 - ((parts-1)*proportion))-(int)(100*(MyUtilFunctions::wxlonglongtodouble(current->size[i] -
                                                      current->sizecompleted[i]) / MyUtilFunctions::wxlonglongtodouble(current->totalsize)));
                    }
                    else
                    {
                        current->percentualparts[i] = (proportion)-(int)(100*(MyUtilFunctions::wxlonglongtodouble(current->size[i] -
                                                      current->sizecompleted[i]) / MyUtilFunctions::wxlonglongtodouble(current->totalsize)));
                    }
                }
                else
                    current->percentualparts[i] = 0;
            }
            result = TRUE;
        }
        list01->Insert(current,wxGetApp().downloadlist.IndexOf(current));
    }
    return result;
}

void mMainFrame::CheckNewRelease()
{
    mCheckNewReleaseThread *thread = new mCheckNewReleaseThread();
    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        wxMessageBox(_("Error creating thread!"));
    }
    else
    {
        if ( thread->Run() != wxTHREAD_NO_ERROR )
        {
            wxMessageBox(_("Error starting thread!"));
        }
    }
}

void mMainFrame::OnNewRelease(wxCommandEvent& event)
{
    programoptions.currentrelease = event.GetString();
    mApplication::Configurations(WRITE,OPT_CURRENT_RELEASE,programoptions.currentrelease);

    this->defaultstatusbarmessage = _("NEW RELEASE: ");
    this->defaultstatusbarmessage += wxT("wxDownload Fast ");
    this->defaultstatusbarmessage += programoptions.currentrelease + _(" is available.");

    wxMessageBox(wxT("wxDownload Fast ") + programoptions.currentrelease + _(" is available.") + wxT("\n") + _("Visit http://dfast.sourceforge.net for more informations."));
}

void mMainFrame::OnFilePreview(wxCommandEvent& event)
{
    mInProgressList *list = XRCCTRL(*this, "inprogresslist",mInProgressList );
    int currentselection;
    if ((currentselection = list->GetCurrentLastSelection()) >= 0)
    {
        wxFileName tempdestination;
        int lastpiece;
        wxLogNull nolog;
        mDownloadFile *currentfile = wxGetApp().downloadlist.Item(currentselection)->GetData();
        list->SetCurrentSelection(currentselection);
        if ((!currentfile->IsSplitted()) && (currentfile->GetStatus() == STATUS_ACTIVE))
            lastpiece = 0;
        else
            lastpiece = currentfile->GetNumberofParts()-1;

        tempdestination.Assign(currentfile->GetTemporaryDestination() + wxT("/"));
        tempdestination.SetFullName(PREFIX + currentfile->GetName() + EXT + MyUtilFunctions::int2wxstr(lastpiece));
        if (tempdestination.FileExists())
        {
            wxZipInputStream *zip = new wxZipInputStream(*(new wxFileInputStream(tempdestination.GetFullPath())));
            if (zip)
            {
                wxZipEntry *currentzipentry;
                if ((currentzipentry = zip->GetNextEntry()))
                {
                    wxArrayString array;
                    do
                    {
                        if (currentzipentry->IsDir())
                            array.Add(currentzipentry->GetName() + wxT("/"));
                        else
                            array.Add(currentzipentry->GetName());
                    }while ((currentzipentry = zip->GetNextEntry()));
                    array.Sort();
                    wxSingleChoiceDialog dlg(this,_("List of files in Zip archive:"),_("Zip Preview"),array);
                    dlg.ShowModal();
                }
                else
                    wxMessageBox(_("Unable to extract zip file content!"),_("Error...") ,wxOK | wxICON_ERROR,this);
            }
            else
                wxMessageBox(_("Unable to extract zip file content!"),_("Error...") ,wxOK | wxICON_ERROR,this);
        }
        else
            wxMessageBox(_("File not found."),_("Error...") ,wxOK | wxICON_ERROR,this);
    }
}

void mMainFrame::OnNewDownloadEvent(wxCommandEvent& event)
{
    mDownloadFile *file = (mDownloadFile *)event.GetClientObject();
    NewDownload(file->GetUrlArray(),file->GetDestination(),event.GetInt(),file->GetNumberofParts(),file->GetUser(),
                file->GetPassword(),file->GetReferenceURL(),file->GetComments(),file->GetCommand(),programoptions.laststartoption,programoptions.lastontopoption, false,false);
}
