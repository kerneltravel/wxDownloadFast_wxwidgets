//
// C++ Implementation: wxDFast
//
// Description: Implements the program initialization.
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"
#include "wx/listimpl.cpp"
#include "wx/arrimpl.cpp"

WX_DEFINE_LIST(mDownloadListType)
WX_DEFINE_LIST(mUrlList)
WX_DEFINE_LIST(mGraphPoints)
WX_DEFINE_OBJARRAY(mListSelection)

IMPLEMENT_APP(mApplication)

wxConnectionBase *mServer::OnAcceptConnection(const wxString& topic)
{
    if ( topic == IPC_TOPIC + wxGetUserId())
    {
        mConnection *connection = new mConnection();
        return connection;
    }
    return NULL;
}

bool mConnection::OnExecute(const wxString& topic, wxChar* data, int size, wxIPCFormat format)
{
    wxString urlsparameter = data;
    wxArrayString urls;
    wxStringTokenizer *tkz01, *tkz02;
    int numberofparts, startoption,ontop;
    wxString textfile,destination,comments,command,reference,strurls,strtemp;
    wxTextFile file;

    tkz01 = new wxStringTokenizer(urlsparameter, wxT("$"));
    textfile = tkz01->GetNextToken();
    destination = tkz01->GetNextToken();
    comments = tkz01->GetNextToken();
    reference = tkz01->GetNextToken();
    strurls = tkz01->GetNextToken();
    tkz02 = new wxStringTokenizer(strurls, wxT("#"));
    while (tkz02->HasMoreTokens())
    {
        strtemp = tkz02->GetNextToken();
        if (strtemp != wxEmptyString)
            urls.Add(strtemp);
    }
    delete tkz01;
    delete tkz02;
    wxLogNull nolog;
    if ((textfile != wxT("NONE")) && (file.Open(textfile)))
    {
        for ( strtemp = file.GetFirstLine(); !file.Eof(); strtemp = file.GetNextLine() )
            if (strtemp != wxEmptyString)
                urls.Add(strtemp);
        if (strtemp != wxEmptyString)
            urls.Add(strtemp);
    }
    if (wxGetApp().mainframe->programoptions.rememberboxnewoptions)
    {
        startoption = wxGetApp().mainframe->programoptions.laststartoption;
        ontop = wxGetApp().mainframe->programoptions.lastontopoption;
        numberofparts = wxGetApp().mainframe->programoptions.lastnumberofparts;
        if (destination == wxT("NONE"))
            destination = wxGetApp().mainframe->programoptions.lastdestination;
        command = wxGetApp().mainframe->programoptions.lastcommand;
    }
    else
    {
        command = wxEmptyString;
        startoption = DEFAULT_START_OPTION;
        ontop = DEFAULT_ONTOP_OPTION;
        numberofparts = DEFAULT_NUM_PARTS;
        if (destination == wxT("NONE"))
            destination = wxGetApp().mainframe->programoptions.destination;
    }
    if (comments == wxT("NONE"))
        comments = wxEmptyString;
    if (reference == wxT("NONE"))
        reference = wxEmptyString;

    if (urls.GetCount() > 0)
        wxGetApp().mainframe->NewDownload(urls,destination,-1,numberofparts,wxEmptyString,wxEmptyString,reference,comments,command,startoption,ontop,TRUE,TRUE);
    else if (!wxGetApp().mainframe->IsShown())
    {
        wxCommandEvent event;
        wxGetApp().mainframe->taskbaricon->OnHide(event);
    }
    return TRUE;
}

bool mApplication::NewInstance()
{
    wxString server = IPC_SERVICE;
    wxString hostName;
    wxString textfile, destination, comments, reference, urlparameter = wxEmptyString,stringtosend;
    hostName = wxGetHostName();
    bool returntmp;

    mClient *client = new mClient;
    wxLogNull nolog;
    connection = (mConnection *)client->MakeConnection(hostName, server, IPC_TOPIC + wxGetUserId());
    if (!connection)
        returntmp = FALSE;
    else
    {
        if (!wxGetApp().parameters->Found(wxT("list"),&textfile))
            textfile = wxT("NONE");
        if (!wxGetApp().parameters->Found(wxT("destination"),&destination))
            destination = wxT("NONE");
        if (!wxGetApp().parameters->Found(wxT("comments"),&comments))
            comments = wxT("NONE");
        if (!wxGetApp().parameters->Found(wxT("reference"),&reference))
            reference = wxT("NONE");

        for (unsigned int i = 0; i < parameters->GetParamCount(); i++)
            urlparameter += parameters->GetParam(i) + wxT("#");
        stringtosend = textfile + wxT("$") + destination + wxT("$") + comments + wxT("$") + reference + wxT("$") + urlparameter + wxT("$");
        connection->Execute((wxChar *)stringtosend.c_str(),(stringtosend.Length() + 1) * sizeof(wxChar));
        returntmp = TRUE;
    }

    delete connection;
    delete client;
    return returntmp;
}

mApplication::mApplication(): m_condAllDone(m_mutexAllDone)
{
    // the mutex associated with a condition must be initially locked, it will
    // only be unlocked when we call Wait()
    #ifndef DISABLE_MUTEX
    m_mutexAllDone.Lock();
    #endif
    m_waitingUntilAllDone = FALSE;
    m_locale = NULL;
}

mApplication::~mApplication()
{
    delete m_locale;
    delete dummy;
    delete m_server;
    delete parameters;
}

bool mApplication::OnInit()
{
    parameters = new wxCmdLineParser(cmdlinedesc, argc, argv);
    if (parameters->Parse() != 0)
        exit(TRUE);

    if (NewInstance())
    return FALSE;

    #ifdef __WXMSW__
    //GET THE PROGRAM VOLUME
    wxFileName tempprogramvolume(argv[0]);
    programvolume = tempprogramvolume.GetVolume();
    #endif

    wxString service = IPC_SERVICE; //REGISTER THIS INSTANCE
    m_server = new mServer;
    m_server->Create(service);

    dummy = new wxSocketBase(); //TO SOCKETS TO WORK INSIDE THREADS
    dummy->SetTimeout(0);

    wxImage::AddHandler(new wxXPMHandler);
    wxImage::AddHandler(new wxPNGHandler);
    wxXmlResource::Get()->InitAllHandlers();
    wxString resourcepath = wxT("resources/");
    if (!wxFileExists(resourcepath + wxT("mainwindow.xrc")))
    {
        //THIS IS NEVER GOING TO HAPPEN ON WINDOWS SYSTEM
        /*#ifdef __WXMSW__
        wxFileName programpath(parameters->GetParam(0));
        resourcepath = programpath.GetPath(wxPATH_GET_SEPARATOR | wxPATH_GET_VOLUME) + resourcepath;
        #else*/
        resourcepath = wxT("/usr/share/wxdfast/");
        //#endif
    }
    themepath = resourcepath + wxT("RipStop/");
    #ifdef USE_EXTERNAL_XRC
    wxXmlResource::Get()->Load(resourcepath + wxT("mainwindow.xrc"));
    wxXmlResource::Get()->Load(resourcepath + wxT("menubar.xrc"));
    wxXmlResource::Get()->Load(resourcepath + wxT("toolbar.xrc"));
    wxXmlResource::Get()->Load(resourcepath + wxT("boxnew.xrc"));
    wxXmlResource::Get()->Load(resourcepath + wxT("boxoptions.xrc"));
    wxXmlResource::Get()->Load(resourcepath + wxT("boxabout.xrc"));
    #else
    InitXmlResource();
    #endif

    SetLanguage(mApplication::Configurations(READ,LANGUAGE_REG,0)); //SET THE LANGUAGE

    downloadlist.DeleteContents(TRUE);
    downloadlist.LoadDownloadListFromDisk();
    mainframe = NULL;
    mainframe = new mMainFrame();
    {
        int x,y,width,height,maximized,separatorposition01,separatorposition02,details;
        wxSplitterWindow *splitter01 = XRCCTRL(*mainframe, "splitter01",wxSplitterWindow);
        wxSplitterWindow *splitter02 = XRCCTRL(*mainframe, "splitter02",wxSplitterWindow);
        x = mApplication::Configurations(READ,POS_X_REG,-1);
        y = mApplication::Configurations(READ,POS_Y_REG,-1);
        width = mApplication::Configurations(READ,SIZE_X_REG,-1);
        height = mApplication::Configurations(READ,SIZE_Y_REG,-1);
        maximized = mApplication::Configurations(READ,MAXIMIZED_REG,0);
        separatorposition01 = mApplication::Configurations(READ,SEPARATOR01_REG,-1);
        separatorposition02 = mApplication::Configurations(READ,SEPARATOR02_REG,-1);
        details = mApplication::Configurations(READ,DETAILS_REG,-1);
        mainframe->SetSize(x,y,width,height);
        if (maximized)               mainframe->Maximize(TRUE);
        if (!details)            splitter01->Unsplit();
        if (separatorposition01  < 20)    separatorposition01 = 100;
        if (separatorposition02  < 20)    separatorposition02 = 100;
        splitter01->SetSashPosition(separatorposition01);
        splitter02->SetSashPosition(separatorposition02);
    }

    //IF A URL OR A TEXT FILE WAS PASSED BY THE COMMAND LINE
    int startoption, numberofparts,ontop;
    wxString listtextfile,reference,comments,destination,command;
    wxArrayString url;
    if (parameters->GetParamCount() > 0)
    {
        int i, nparams = parameters->GetParamCount();
        for (i = 0;i<nparams; i++)
            if (parameters->GetParam(i) != wxEmptyString)
                url.Add(parameters->GetParam(i));
    }
    if (parameters->Found(wxT("list"),&listtextfile))
    {
        wxLogNull nolog;
        wxTextFile file(listtextfile);
        if (file.Open())
        {
            wxString str;
            for ( str = file.GetFirstLine(); !file.Eof(); str = file.GetNextLine() )
                if (str != wxEmptyString)
                    url.Add(str);
            if (str != wxEmptyString)
                url.Add(str);
        }
    }
    if (mainframe->programoptions.rememberboxnewoptions)
    {
        numberofparts = mainframe->programoptions.lastnumberofparts;
        startoption = mainframe->programoptions.laststartoption;
        ontop = mainframe->programoptions.lastontopoption;
        if (!parameters->Found(wxT("destination"),&destination))
            destination = mainframe->programoptions.lastdestination;
        command = mainframe->programoptions.lastcommand;
    }
    else
    {
        command = wxEmptyString;
        numberofparts = DEFAULT_NUM_PARTS;
        startoption = DEFAULT_START_OPTION;
        ontop = DEFAULT_ONTOP_OPTION;
        if (!parameters->Found(wxT("destination"),&destination))
            destination = mainframe->programoptions.destination;
    }
    if (!parameters->Found(wxT("comments"),&comments))
        comments = wxEmptyString;
    if (!parameters->Found(wxT("reference"),&reference))
        reference = wxEmptyString;
    if (url.GetCount() > 0)
        mainframe->NewDownload(url,destination,-1,numberofparts,wxEmptyString,wxEmptyString,reference,comments,command,startoption,ontop,TRUE,TRUE);


    if (!parameters->Found(wxT("hide")))
        mainframe->Show(true);
    return true;
}

int mApplication::OnExit()
{
     // the mutex must be unlocked before being destroyed
     #ifndef DISABLE_MUTEX
     m_mutexAllDone.Unlock();
     #endif
//    delete m_checker;
    return 0;
}

wxString mApplication::Configurations(int operation, wxString option,wxString value)
{
    wxString tmpvalue = value;

    #ifdef WXDFAST_PORTABLE
    wxFileConfig *config = new wxFileConfig(DFAST_REG, wxEmptyString, DFAST_REG + wxT(".ini"), wxEmptyString,
                                                wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
    #else
    wxFileConfig *config = new wxFileConfig(DFAST_REG);
    #endif

    config->SetPath(CONFIG_REG);
    if (operation == WRITE)
        config->Write(option,tmpvalue);
    else
        config->Read(option,&tmpvalue);
    delete config;
    return tmpvalue;
}

int mApplication::Configurations(int operation, wxString option,int value)
{
    int tmpvalue = value;
    #ifdef WXDFAST_PORTABLE
    wxFileConfig *config = new wxFileConfig(DFAST_REG, wxEmptyString, DFAST_REG + wxT(".ini"), wxEmptyString,
                                            wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
    #else
    wxFileConfig *config = new wxFileConfig(DFAST_REG);
    #endif

    config->SetPath(CONFIG_REG);
    if (operation == WRITE)
        config->Write(option,tmpvalue);
    else
        config->Read(option,&tmpvalue);
    delete config;
    return tmpvalue;
}

long mApplication::Configurations(int operation, wxString option,long value)
{
    wxString tmpvalue;
    long returnvalue;
    tmpvalue << value;
    #ifdef WXDFAST_PORTABLE
    wxFileConfig *config = new wxFileConfig(DFAST_REG, wxEmptyString, DFAST_REG + wxT(".ini"), wxEmptyString,
                                            wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
    #else
    wxFileConfig *config = new wxFileConfig(DFAST_REG);
    #endif

    config->SetPath(CONFIG_REG);
    if (operation == WRITE)
        config->Write(option,tmpvalue);
    else
        config->Read(option,&tmpvalue);
    delete config;
    tmpvalue.ToLong(&returnvalue);
    return returnvalue;
}

void mApplication::SetLanguage(int language)
{
    wxString msg;
    msg = _("Cannot set locale to");
    msg += wxT(" '") + m_locale->GetLanguageName(language) + wxT("'.\n");
    msg += _("This language is not supported by your system.");

    if (m_locale)
        delete m_locale;
    m_locale = new wxLocale();
    #if wxCHECK_VERSION(2, 8, 0)
    if (m_locale->IsAvailable(language))
        m_locale->Init(language);
    else
    {
        wxMessageBox(msg,_("Error...") ,wxOK | wxICON_ERROR,NULL);
        m_locale->Init(wxLANGUAGE_UNKNOWN);
    }
    #else
    m_locale->Init(language);
    #endif
    m_locale->AddCatalogLookupPathPrefix(wxT("languages"));
	m_locale->AddCatalogLookupPathPrefix(wxStandardPaths::Get().GetResourcesDir() + wxT("/locale"));
    m_locale->AddCatalog(wxT("wxDFast"));
}
