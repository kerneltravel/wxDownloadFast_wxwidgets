//
// C++ Implementation: DownloadThread
//
// Description: Implements the download routine by thread. In this file
//              is made all the communication with the FTP and HTTP serves.
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"

mUrlName mDownloadThread::CheckHtmlFile(bool downloaded)
{
    mUrlName returnurl;
    wxString result = wxEmptyString;
    if (downloaded)
    {
        wxFile *outputfile;
        wxFileName file;
        mUrlName *url = NULL;
        long data_size = 10240;
        long bytesread;
        char *data = new char[data_size];
        wxString wxdata;

        file.Assign(downloadfile->GetDestination() + SEPARATOR_DIR);
        file.SetFullName(downloadfile->GetName());
        if (file.FileExists())
        {
            outputfile = new wxFile(file.GetFullPath(),wxFile::read);
            if (outputfile)
            {
                bytesread = outputfile->Read(data,data_size);
                wxdata = MyUtilFunctions::str2wxstr(data);
                if (long pos1 = wxdata.Lower().Find(wxT("location=")))
                {
                    pos1 += 9;
                    if (wxdata.Mid(pos1,1) == wxT("\""))
                    {
                        long pos2 = pos1++;
                        while (++pos2 < bytesread)
                            if (wxdata.Mid(pos2,1) == wxT("\""))
                            {
                                url = new mUrlName(wxdata.Mid(pos1,pos2-pos1));
                                break;
                            }
                    }
                }
                outputfile->Close();
                delete outputfile;
                if (url)
                {
                    if (url->IsComplete())
                        result = url->GetFullPath();
                    returnurl = *(url);
                    delete url;
                }
            }
        }
    }
    if (!returnurl.IsComplete())
    {
        PrintMessage( _("You have downloaded a HTML file.\nOpening this file in your browser.\n"),HTMLERROR);
        wxCommandEvent openurl(wxEVT_OPEN_URL);
        openurl.SetString(currenturl.GetFullPath());
        wxGetApp().mainframe->GetEventHandler()->AddPendingEvent(openurl);
    }
    return returnurl;
}

wxLongLong mDownloadThread::CurrentSize(wxString filepath,wxString filename)
{
    wxFile *outputfile;
    wxFileName file;
    file.Assign(filepath + SEPARATOR_DIR);
    file.SetFullName(filename);
    wxLongLong size = 0;
    if (!file.FileExists())
       return size;
    outputfile = new wxFile(file.GetFullPath(),wxFile::read);
    if (!outputfile)
       return 0;
    size = outputfile->Length();
    outputfile->Close();
    delete outputfile;
    return size;
}

mDownloadThread::mDownloadThread(mDownloadFile *file, int index)
{
    downloadfile = file;
    downloadpartindex = index;
    programoptions = &(wxGetApp().mainframe->programoptions);
    downloadlist = &(wxGetApp().downloadlist);
    proxy_address = NULL;
}

void mDownloadThread::OnExit()
{
    wxGetApp().mainframe->SendSizeEvent();
    downloadfile->finished[downloadpartindex] = TRUE;
    if (downloadpartindex == 0)
    {
        #ifndef DISABLE_MUTEX
        delete downloadfile->mutex_speedcalc;
        downloadfile->mutex_speedcalc = NULL;
        #endif

        if (downloadfile->GetStatus() == STATUS_STOPED)
        {
            if (downloadfile->IsScheduled())
                downloadfile->PutOnScheduleQueue();
            downloadfile->MarkWriteAsPending(TRUE);
        }

        downloadfile->SetFree();
    }
    if (proxy_address)
        delete proxy_address;
    //delete programoptions;
}

void *mDownloadThread::Entry()
{
    wxLogNull nolog;
    wxSocketClient *connection = NULL;
    wxInputStream *filestream = NULL;
    int resp = -1;
    int maxattempts;
    int attemptstime;
    wxLongLong start,end;
    redirecting = FALSE;
    realtotalsize_copy = -1;
    downloadfile->ResetAttempts();
    downloadfile->messages[downloadpartindex].Clear();
    downloadfile->finished[downloadpartindex] = FALSE;
    if (downloadfile->metalinkdata)
    {
        delete downloadfile->metalinkdata;
        downloadfile->metalinkdata = NULL;
    }
    if (downloadpartindex == 0)
    {
        #ifndef DISABLE_MUTEX
        downloadfile->mutex_speedcalc = new wxMutex();
        #endif
        downloadfile->criticalerror = FALSE;
        for (int i=0;i<downloadfile->GetNumberofParts();i++)
            downloadfile->startpoint[i] = 0;
    }
    else
    {
        while (downloadfile->WaitingForSplit())
        {
            if (downloadfile->GetStatus() == STATUS_STOPED)
                return NULL;
            else if (downloadfile->criticalerror)
                return NULL;
            Sleep(10);
        }
        if (!downloadfile->IsSplitted())
            return NULL;
    }
    #ifndef DISABLE_MUTEX
    wxGetApp().mainframe->mutex_programoptions->Lock();
    #endif
    maxattempts = programoptions->attempts;
    attemptstime = programoptions->attemptstime;
    proxy  = programoptions->proxy;
    if (proxy)
    {
        proxy_address = new mUrlName(programoptions->proxy_server + wxT(":") + programoptions->proxy_port);
        if (!proxy_address->HasScheme())
        {
            delete proxy_address;
            proxy_address = new mUrlName(wxT("http://") + programoptions->proxy_server + wxT(":") + programoptions->proxy_port);
        }
        if (proxy_address->Type() != HTTP)
        {
            PrintMessage( _("\nProxy not found.\n"),HTMLERROR);
            return NULL;
        }
        proxy_authstring = programoptions->proxy_authstring;

    }
    else
        proxy_address = NULL;
    #ifndef DISABLE_MUTEX
    wxGetApp().mainframe->mutex_programoptions->Unlock();
    #endif
    do
    {
        connection = NULL;
        filestream = NULL;
        downloadfile->SetAsActive();
        if (!redirecting)
            currenturl = downloadfile->GetNextUrl();
        else
            redirecting = FALSE;
        if ((!downloadfile->metalinkdata) && (downloadfile->NeedToReGetMetalink()))
        {
            downloadlist->ChangeName(downloadfile,currenturl.GetFullPath().Mid(currenturl.GetFullPath().Find('/',true)+1));
            start = 0;
        }
        else
            start = CurrentSize(downloadfile->GetTemporaryDestination(),PREFIX + downloadfile->GetName() + EXT + MyUtilFunctions::int2wxstr(downloadpartindex));
        start += downloadfile->startpoint[downloadpartindex];
        do
        {
            int type = currenturl.Type();
            if (type == LOCAL_FILE)
                filestream = ConnectLOCAL_FILE(start);
            else if ((type == HTTP) || (proxy))
                connection = ConnectHTTP(&start);
            else if (type == FTP)
            {
                connection = ConnectFTP(&start);
            }
            else
            {
                PrintMessage( _("This protocol isn't supported.\n"),HTMLERROR);
                downloadfile->criticalerror = TRUE;
                break;
            }
            if ((!connection) && (!filestream) && (downloadpartindex == 0) && (downloadfile->IsHtml()))
            {
                this->CheckHtmlFile();
                resp = 0;
                downloadfile->SetAsFinished();
                break;
            }
        } while (redirecting);
        if ((connection) || (filestream))
        {

            if ((downloadpartindex == 0) && (downloadfile->WaitingForSplit()))
            {
                if ((downloadfile->RestartSupport() == YES) && (downloadfile->totalsize > MIN_SIZE_TO_SPLIT) && (!downloadfile->IsMetalink()) && (!downloadfile->IsHtml()))
                {
                    wxLongLong tempsize = (downloadfile->totalsize / downloadfile->GetNumberofParts());
                    int i;
                    int nparts = downloadfile->GetNumberofParts();
                    for (i=0; i < ((downloadfile->GetNumberofParts())-1); i++)
                    {
                        downloadfile->startpoint[i] = wxLongLong(0l,(long)i)*tempsize;
                        downloadfile->size[i] = tempsize;
                    }
                    downloadfile->startpoint[(nparts)-1] = wxLongLong(0l,(long)((nparts)-1))*tempsize;
                    downloadfile->size[(nparts)-1] = downloadfile->totalsize - wxLongLong(0l,(long)(((nparts)-1)))*tempsize;
                    downloadfile->Split(TRUE);
                }
                else
                {
                    wxString partialfile = downloadfile->GetTemporaryDestination();
                    if (partialfile.Mid(partialfile.Length()-1,1) != SEPARATOR_DIR)
                        partialfile += SEPARATOR_DIR;
                    partialfile += PREFIX + downloadfile->GetName() + EXT + MyUtilFunctions::int2wxstr(downloadpartindex);
                    ::wxRemoveFile(partialfile);
                    start = 0;
                    if ((!downloadfile->IsMetalink()) && (!downloadfile->IsHtml()))
                        downloadfile->Split(FALSE);
                    downloadfile->size[downloadpartindex] = downloadfile->totalsize;
                }
            }
            end = downloadfile->startpoint[downloadpartindex] + downloadfile->size[downloadpartindex];
            resp = DownloadPart(connection,filestream,start,end);

            if ((downloadpartindex == 0) && (downloadfile->GetStatus() == STATUS_FINISHED) && (downloadfile->IsHtml()))
            {
                mUrlName url = this->CheckHtmlFile(TRUE);
                if (url.IsComplete())
                {
                    currenturl = url;
                    redirecting = TRUE;
                    PrintMessage( _("Redirecting to ") + currenturl.GetFullPath() + wxT("\n\n"),HTMLSERVER);
                    //downloadfile->ErrorOccurred();
                    downloadlist->ChangeName(downloadfile,currenturl.GetFullPath().Mid(currenturl.GetFullPath().Find('/',true)+1));
                    resp = -1;
                    //downloadfile->criticalerror = TRUE;
                    continue;
                }
                else
                    downloadfile->Split(FALSE);
            }
            if ((downloadpartindex == 0) && (downloadfile->GetStatus() == STATUS_FINISHED) && (downloadfile->IsMetalink()))
            {
                mMetalinkDocument metalinkfile;
                wxFileName file(downloadfile->GetDestination() + wxT("/") + downloadfile->GetName());
                if (metalinkfile.Load(file.GetFullPath()))
                {
                    int index,count;
                    PrintMessage( wxT("\n"),HTMLSERVER);
                    PrintMessage( _("Metalink file detected\n"),HTMLSERVER);
                    PrintMessage( _("Getting Metalink data...\n"),HTMLSERVER);

                    if (!downloadfile->metalinkdata)
                        downloadfile->metalinkdata = new mMetalinkData();
                    if (downloadfile->GetMetalinkFileIndex() >= 0)
                        index = downloadfile->GetMetalinkFileIndex();
                    else
                        index = 0;
                    downloadfile->metalinkdata->Clear();
                    wxLogDebug(wxT("Metalinkindex = ") + MyUtilFunctions::int2wxstr(index));
                    count = metalinkfile.GetMetalinkData(downloadfile->metalinkdata,index);
                    wxLogDebug(wxT("NumberofMetalinkFiles = ") + MyUtilFunctions::int2wxstr(count));
                    if (count > 0 )
                    {
                        wxString str;
                        int currentmetalinkindex = downloadfile->GetMetalinkFileIndex();
                        str = wxT("PublisherName: ");
                        str += downloadfile->metalinkdata->publishername;
                        str += wxT("\nPublisherUrl: ");
                        str += downloadfile->metalinkdata->publisherurl;
                        str += wxT("\nDescription: ");
                        str += downloadfile->metalinkdata->description;
                        str += wxT("\nFilename: ");
                        str += downloadfile->metalinkdata->filename;
                        str += wxT("\nVersion: ");
                        str += downloadfile->metalinkdata->version;
                        str += wxT("\nSize: ");
                        if (downloadfile->metalinkdata->size > 0)
                            str += downloadfile->metalinkdata->size.ToString();
                        else
                            str += _("Not available");
                        str += wxT("\nLanguage: ");
                        str += downloadfile->metalinkdata->language;
                        str += wxT("\nOS: ");
                        str += downloadfile->metalinkdata->os;
                        str += wxT("\nMD5: ");
                        str += downloadfile->metalinkdata->md5;
                        //str += wxT("\nSHA1: ");
                        //str += downloadfile->metalinkdata->sha1;
                        str += wxT("\n\n");
                        PrintMessage( str,HTMLSERVER);
                        downloadfile->SetAsActive();
                        downloadfile->SetMetalinkFileIndex(index);
                        downloadfile->SetToReGetMetalinkWhenNeeded(TRUE);
                        downloadlist->ChangeName(downloadfile,downloadfile->metalinkdata->filename);
                        currenturl = downloadfile->GetFirstUrl();
                        redirecting = TRUE;
                        ::wxRemoveFile(file.GetFullPath());
                        resp = -1;
                        if (count > 1)
                        {
                            PrintMessage( _("This Metalink file contains multiples files.\n"),HTMLSERVER);
                            if (currentmetalinkindex < 0)
                            {
                                PrintMessage( _("Creating all downloads...\n"),HTMLSERVER);
                                for (int i = 1 ; i<count ; i++)
                                {
                                    wxCommandEvent newdownload(wxEVT_NEW_DOWNLOAD);
                                    newdownload.SetInt(i);
                                    newdownload.SetClientObject((wxClientData*)downloadfile);
                                    wxGetApp().mainframe->GetEventHandler()->AddPendingEvent(newdownload);
                                    Sleep(500);
                                }
                            }
                            PrintMessage( _("Getting file number ") + MyUtilFunctions::int2wxstr(index+1)  + wxT("\n"),HTMLSERVER);
                        }
                        continue;
                    }
                    else
                    {
                        PrintMessage( _("Error retrieving Metalink data\n"),HTMLERROR);
                        downloadfile->criticalerror = TRUE;
                        downloadfile->Split(FALSE);
                        break;
                    }
                }
                else
                {
                    PrintMessage( _("Error retrieving Metalink data\n"),HTMLERROR);
                    downloadfile->criticalerror = TRUE;
                    downloadfile->Split(FALSE);
                    break;
                }
            }
        }
        else
        {
            if (downloadfile->GetStatus() == STATUS_STOPED)
                resp = 0;
            else if (downloadfile->GetStatus() != STATUS_FINISHED)
                resp = -1;
        }
        if (downloadpartindex == 0)
            downloadfile->IncrementAttempt();
        if ((downloadfile->GetCurrentAttempt() <= maxattempts) && (resp == -1) && (!downloadfile->criticalerror))
        {
            PrintMessage( _("New attempt in ") + MyUtilFunctions::int2wxstr(attemptstime) + _(" seconds\n"));
            wxStopWatch waittime;
            waittime.Start();
            while (waittime.Time() < ((attemptstime)*1000))
            {
                if (downloadfile->GetStatus() == STATUS_STOPED)
                {
                    resp = 0;
                    break;
                }
                this->Sleep(10);
            }
            waittime.Pause();
            if ((downloadfile->GetStatus() == STATUS_ACTIVE) && (downloadpartindex == 0))
                PrintMessage( _("Attempt ") + MyUtilFunctions::int2wxstr(downloadfile->GetCurrentAttempt()) + _(" of ") + MyUtilFunctions::int2wxstr(maxattempts) + wxT(" ...\n"));
        }
    }
    while ((downloadfile->GetCurrentAttempt() <= maxattempts) && (resp == -1) && (!downloadfile->criticalerror));

    if ((resp == -1) || (downloadfile->criticalerror))
    //WILL BE TRUE IF A ERROR HAPPEN IN THIS THREAD(RESP == -1)
    //OR IN THE OTHERS(downloadfile->error == TRUE)
    {
        downloadfile->criticalerror = TRUE;
        downloadfile->SetAsStoped();
        if (downloadpartindex == 0)
        {
            WaitUntilAllFinished(FALSE);
            downloadfile->ErrorOccurred();
        }
    }
    if (downloadfile->GetStatus() == STATUS_STOPED)
    {
        if (downloadpartindex == 0)
            WaitUntilAllFinished(FALSE);
        PrintMessage( _("Canceled.\n"));
    }


    return NULL;
}

int mDownloadThread::DownloadPart(wxSocketClient *connection, wxInputStream *filestream, wxLongLong start,wxLongLong end)
{
    int resp = 0;
    wxInputStream *in = NULL;
    wxFileName destination;
    wxFileName tempdestination;
    int type = currenturl.Type();

    destination.Assign(downloadfile->GetDestination() + wxT("/"));
    destination.SetFullName(downloadfile->GetName());
    tempdestination.Assign(downloadfile->GetTemporaryDestination() + wxT("/"));
    tempdestination.SetFullName(PREFIX + destination.GetFullName() + EXT + MyUtilFunctions::int2wxstr(downloadpartindex));

    if (start > end)
    {
        downloadfile->criticalerror = TRUE;
        PrintMessage( _("Critical error!! The start point is bigger that the end point!!\n"),HTMLERROR);
        resp = -1;
    }
    else if (start == end)
    {
        resp = 0;
        downloadfile->sizecompleted[downloadpartindex] = start - downloadfile->startpoint[downloadpartindex];
    }
    else
    {
        if (type == LOCAL_FILE)
            in = filestream;
        else if ((type == HTTP) || (proxy))
            in = new wxSocketInputStream(*connection);
        else if (type == FTP)
        {
            mFTP *tempconnection = (mFTP*)connection;
            in = tempconnection->GetInputStream(currenturl.GetFullRealName());
            PrintMessage( tempconnection->GetLastResult() + wxT("\n"),HTMLSERVER);
        }

        if (!in)
        {
            PrintMessage( _("Error establishing input stream.\n"),HTMLERROR);
            if (connection) {connection->Close(); delete connection;}
            connection = NULL;
            in = NULL;
            return resp = -1;
        }

        if (downloadfile->GetStatus() == STATUS_STOPED)
        {
            if (in) {delete in;}
            in = NULL;
            if (connection) {connection->Close(); delete connection;}
            connection = NULL;
            return resp = 1;
        }
        PrintMessage( _("Copying file...\n"));

        wxFile *outputfile = new wxFile(tempdestination.GetFullPath(),wxFile::write_append);
        if (!outputfile)
        {
            PrintMessage( _("Error opening file ") + tempdestination.GetFullPath() + wxT(".\n"),HTMLERROR);
            if (connection) {connection->Close(); delete connection;}
            connection = NULL;
            delete in;
            in = NULL;
            return resp = -1;
        }

        long readbuffersize;
        #ifndef DISABLE_MUTEX
        wxGetApp().mainframe->mutex_programoptions->Lock();
        #endif
        readbuffersize = programoptions->readbuffersize;
        #ifndef DISABLE_MUTEX
        wxGetApp().mainframe->mutex_programoptions->Unlock();
        #endif

        char *data = new char[readbuffersize];
        long read_buffer;
        wxLongLong readbuffersizelonglong(0l,readbuffersize);
        wxLongLong lasttime;
        wxLongLong endminustart;
        int counttimes=0;
        wxStopWatch time;
        wxLongLong timepassed = downloadfile->timepassed;
        long time_checkifbandcontrolchanged;
        long starttime_bandwidth;
        long deltasize_bandwidth = 0l;
        long bandwidth = 0;
        int bandwidthoption = 0; //SET AS UNLIMITED

        downloadfile->delta_size[downloadpartindex] = 0;
        downloadfile->speedpoint = FALSE;
        time.Pause();
        time.Start();
        lasttime = timepassed + time.Time();
        starttime_bandwidth = time.Time();
        time_checkifbandcontrolchanged = time.Time() - 10000;

        while (start < end)
        {

            if ((time.Time() - time_checkifbandcontrolchanged) >= 10000)
            //CHECK EVERY 10 SECONDS IF THE USER CHANGED THE BANDWIDTH
            {
                int oldbandwidth = bandwidth;
                #ifndef DISABLE_MUTEX
                wxGetApp().mainframe->mutex_programoptions->Lock();
                #endif
                bandwidthoption = programoptions->bandwidthoption;
                if (bandwidthoption == 1)
                {
                    bandwidth = downloadfile->GetBandWidth()*1024;
                }
                else if (bandwidthoption == 2)
                {
                    bandwidth = programoptions->bandwidth*1024/downloadlist->GetNumberofActiveDownloads();
                }

                if (downloadfile->IsSplitted())
                    bandwidth = bandwidth/downloadfile->GetNumberofParts();
                #ifndef DISABLE_MUTEX
                wxGetApp().mainframe->mutex_programoptions->Unlock();
                #endif

                time_checkifbandcontrolchanged = time.Time();
                if (oldbandwidth != bandwidth)
                    deltasize_bandwidth = 0l;
            }

            if (((time.Time() - starttime_bandwidth) < 1000) && (bandwidthoption > 0) && (bandwidth > 0)) //IF HAS NOT UNLIMITED BANDWIDTH
            {
                if (deltasize_bandwidth >= bandwidth)
                {
                    this->Sleep(50);
                }
            }
            else
            {
                starttime_bandwidth = time.Time();
                deltasize_bandwidth = 0;
            }

            read_buffer = readbuffersize;
            endminustart = end - start;

            if (((deltasize_bandwidth + readbuffersize)>= bandwidth) && (readbuffersizelonglong <= endminustart) && (bandwidth > 0))
                read_buffer = bandwidth - deltasize_bandwidth;
            else if (readbuffersizelonglong > endminustart)
                read_buffer = endminustart.ToLong();

            if (downloadfile->GetStatus() == STATUS_STOPED){resp = 1; break;}
            this->Sleep(1);
            //this->Yield();
            in->Read(data, read_buffer);
            if (in->LastRead() == 0)
            {
                counttimes++;
                if (counttimes == 3000)
                {
                    PrintMessage( _("Error copying file.\n"),HTMLERROR);
                    this->Sleep(1);
                    resp = -1;
                    if (downloadpartindex == downloadfile->speedpointowner)
                    {
                        downloadfile->totalspeed = 0;
                        downloadfile->speedpointowner = -1;
                        downloadfile->speedpoint = FALSE;
                    }
                    downloadfile->delta_size[downloadpartindex] = 0;
                    break;
                }
            }
            else
            {
                counttimes = 0;
                if (downloadfile->GetStatus() == STATUS_STOPED){resp = 1; break;}

                if (outputfile->Write(data,in->LastRead()) != in->LastRead())
                {
                    PrintMessage( _("Error writing file.\n"),HTMLERROR);
                    resp = -1;
                    if (downloadpartindex == downloadfile->speedpointowner)
                    {
                        downloadfile->totalspeed = 0;
                        downloadfile->speedpointowner = -1;
                        downloadfile->speedpoint = FALSE;
                    }
                    downloadfile->delta_size[downloadpartindex] = 0;
                    break;
                }
                else
                   start += in->LastRead();
                if (downloadfile->GetStatus() == STATUS_STOPED){resp = 1; break;}
            }
            deltasize_bandwidth += in->LastRead();
            downloadfile->delta_size[downloadpartindex] += in->LastRead();
            downloadfile->sizecompleted[downloadpartindex] = start - downloadfile->startpoint[downloadpartindex];

            if (!downloadfile->speedpoint)
                downloadfile->delta_size[downloadpartindex] = 0;

            #ifndef DISABLE_MUTEX
            if (downloadfile->mutex_speedcalc->TryLock() == wxMUTEX_NO_ERROR)
            #endif
            {
                if (downloadfile->speedpointowner == -1)
                    downloadfile->speedpointowner = downloadpartindex;
                if (downloadfile->speedpointowner == downloadpartindex)
                {
                    if (!downloadfile->speedpoint)
                    {
                        //lasttime = time.Time();
                        lasttime = timepassed + time.Time();
                        downloadfile->speedpoint = TRUE;
                    }
                    downloadfile->timepassed = timepassed + time.Time();
                    if (downloadfile->delta_size[downloadpartindex] > 5*readbuffersize)
                    {
                        if ((timepassed + time.Time() - lasttime).ToLong() > 2000)
                        {
                            SpeedCalculation((timepassed + time.Time() - lasttime).ToLong());
                            downloadfile->speedpoint = FALSE;
                        }
                        if (time.Time() > 2000000000l) //I DID THIS BECAUSE Time() RETURN A LONG AND NOT LONGLONG;
                        {
                            timepassed += time.Time();
                            time.Pause();
                            time.Start();
                        }
                    }
                }
                #ifndef DISABLE_MUTEX
                downloadfile->mutex_speedcalc->Unlock();
                #endif
            }

            if (downloadfile->GetStatus() == STATUS_STOPED){resp = 1; break;}

        }
        if (downloadfile->speedpointowner == downloadpartindex)
        {
            downloadfile->totalspeed = 0;
            downloadfile->speedpointowner = -1;
            downloadfile->speedpoint = FALSE;
        }
        downloadfile->delta_size[downloadpartindex] = 0;
        outputfile->Close();
        delete outputfile;
        delete [] data;
        delete in;
        if (connection) {connection->Close(); delete connection;}
        connection = NULL;
        in = NULL;
    }
    if (resp == 0)
        PrintMessage( _("Finished this piece\n"));
    if (downloadpartindex == downloadfile->speedpointowner)
        downloadfile->speedpointowner = -1;
    if (downloadpartindex == 0)
    {
        if (resp == 0)
        {
            resp = FinishDownload(&destination,tempdestination);
        }
    }
    return resp;
}

int mDownloadThread::FinishDownload(wxFileName *destination,wxFileName tempdestination)
{
    int resp = 0;
    if (downloadfile->IsSplitted())
    {
        PrintMessage( _("Waiting for the other pieces...\n"));
        WaitUntilAllFinished(TRUE);
    }
    if (downloadfile->GetStatus() == STATUS_STOPED)
        resp = 1;
    else
    {
        if (downloadfile->IsSplitted())
            PrintMessage( _("Putting all the pieces together...\n"));
        downloadfile->SetFinishedDateTime(wxDateTime::Now());
        if (!JoinFiles(destination,tempdestination))
        {
            PrintMessage( _("Error joining the pieces.\nTry to restart the download.\n"),HTMLERROR);
            resp = -1;
            downloadfile->criticalerror = TRUE;
        }
        else
        {
            PrintMessage( _("Checking MD5...\n"));
            downloadfile->SetExposedName(_("Checking MD5 ... (") + downloadfile->GetName() + wxT(")"));
            wxFileName filemd5 = wxFileName(destination->GetFullPath());
            wxMD5 md5(filemd5);
            PrintMessage( wxT("MD5 = ") + md5.GetDigest() + wxT("\n"));
            downloadfile->SetMD5(md5.GetDigest());
            downloadfile->UnSetExposedName();
            if ((downloadfile->metalinkdata) && (!downloadfile->metalinkdata->md5.IsEmpty()))
            {
                PrintMessage( _("Comparing expected and calculated MD5...\n"));
                if (md5.GetDigest().Lower() == downloadfile->metalinkdata->md5.Lower())
                {
                    PrintMessage( _("File verified successfully\n"));
                    PrintMessage( _("Finished\n"));
                }
                else
                {
                    PrintMessage( _("Error comparing MD5.\n"),HTMLERROR);
                    resp = -1;
                    downloadfile->criticalerror = TRUE;
                }
            }
            else
                PrintMessage( _("Finished\n"));
        }
        downloadfile->SetAsFinished();
    }
    return resp;
}

wxSocketClient *mDownloadThread::ConnectHTTP(wxLongLong *start)
{
    mHTTP *client=NULL;
    wxFileName destination;
    wxString buffer = wxEmptyString;
    destination.Assign(downloadfile->GetDestination());
    destination.SetFullName(PREFIX + downloadfile->GetName() + EXT + MyUtilFunctions::int2wxstr(downloadpartindex));
    redirecting = FALSE;
    int restart = NO;
    if (downloadpartindex == 0)
        downloadfile->SetContentType(_("Not available"));

    if (downloadfile->GetStatus() == STATUS_STOPED){return NULL;}

    wxIPV4address address;
    client = new mHTTP();
    client->Notify(FALSE);
    client ->SetFlags(wxSOCKET_NOWAIT);
    #if wxCHECK_VERSION(2, 8, 0)
    if (currenturl.GetPassword().IsEmpty())
    {
        if (!downloadfile->GetPassword().IsEmpty())
        {
            client->SetUser(downloadfile->GetUser());
            client->SetPassword(downloadfile->GetPassword());
        }
    }
    else
    {
        client->SetUser(currenturl.GetUser());
        client->SetPassword(currenturl.GetPassword());
    }
    #endif

    if (proxy) //A PROXY SERVER WAS SET
    {
        client->UseProxy(proxy_authstring);
        PrintMessage( _("Resolving proxy host '") + proxy_address->GetHost() + wxT("' ..."));
        address.Service(proxy_address->GetPort());
        if (address.Hostname(proxy_address->GetHost())==FALSE)
        {
            PrintMessage( _("\nProxy not found.\n"),HTMLERROR);
            client->Close();
            delete client;
            return NULL;
        }
        else
            PrintMessage( _(" OK\n"),HTMLSERVER);
    }
    else
    {
        PrintMessage( _("Resolving host '") + currenturl.GetHost() + wxT("' ..."));
        address.Service(currenturl.GetPort());
        if (address.Hostname(currenturl.GetHost())==FALSE)
        {
            PrintMessage( _("\nHost not found.\n"),HTMLERROR);
            client->Close();
            delete client;
            return NULL;
        }
        else
            PrintMessage( _(" OK\n"),HTMLSERVER);
    }


    if (downloadfile->GetStatus() == STATUS_STOPED){client->Close(); delete client; return NULL;}

    if (proxy) //A PROXY SERVER WAS SET
        PrintMessage( _("Trying to connect in '") + proxy_address->GetHost() + wxT(" ") + proxy_address->GetPort() + wxT("' ...\n"));
    else
        PrintMessage( _("Trying to connect in '") + currenturl.GetHost() + wxT(" ") + currenturl.GetPort() + wxT("' ...\n"));

    this->Sleep(1);
    client->Connect(address,TRUE);
    if (client->IsConnected() == FALSE )
    {
        PrintMessage( _("Connection denied.\n"),HTMLERROR);
        client->Close();
        delete client;
        return NULL;
    }

    if (client)
    {
        if (downloadfile->GetStatus() == STATUS_STOPED){client->Close(); delete client; return NULL;}
        client->Notify(FALSE);

        PrintMessage( _("Accessing server...\n\n"),HTMLSERVER);

        //CREATE HEADER BEFORE SEND
        PrintMessage( client->BuildGetRequest(currenturl,*start),HTMLSERVER);

        //SEND GET REQUEST
        client->SendGetRequest();

        if (downloadfile->GetStatus() == STATUS_STOPED){client->Close(); delete client; return NULL;}

        if (client->WaitForRead(30,0))
        {
            int returnmessagecode;
            wxStringTokenizer tokenzr;
            wxLongLong sizetmp = 0;
            wxString newfilename = wxEmptyString;
            wxString headervalue;

            //GET SERVER RESPONSE
            PrintMessage( client->GetResponseMessage(),HTMLSERVER);

			if (!((headervalue = client->GetHeader( wxT("location"))).IsEmpty()))
			{
				mUrlName urltmp(headervalue);
				urltmp.Resolve(currenturl);
				currenturl = urltmp;
			}

            returnmessagecode = client->GetResponse();
            if ((returnmessagecode == 1) || (returnmessagecode == 2))
            {

				if (!((headervalue = client->GetHeader( wxT("content-length"))).IsEmpty()))
					sizetmp = MyUtilFunctions::wxstrtolonglong(headervalue);

				if (!((headervalue = client->GetHeader( wxT("content-type"))).IsEmpty()))
				{
					if (downloadpartindex == 0)
						downloadfile->SetContentType(headervalue);
				}
				if ((!((headervalue = client->GetHeader( wxT("accept-ranges"))).IsEmpty())) ||
				   (!((headervalue = client->GetHeader( wxT("content-range"))).IsEmpty())))
					restart = YES;

				if (!((headervalue = client->GetHeader( wxT("Content-Disposition"))).IsEmpty()))
				{
					wxStringTokenizer tkz(headervalue,wxT(" "));
					while ( tkz.HasMoreTokens() )
					{
						wxString token = tkz.GetNextToken();
						if (token.Mid(0,9).Lower() == wxT("filename="))
						{
							newfilename = token.Mid(9);
							if (newfilename.Mid(0,1) == wxT("\""))
								newfilename = newfilename.AfterFirst('\"').BeforeLast('\"');
							break;
						}
					}
				}

				if (newfilename.IsEmpty())
					newfilename = downloadfile->GetName();
				{
					wxString forbiddenchars;
					int pos;
					forbiddenchars = wxFileName::GetForbiddenChars();
					for (unsigned int i=0;i<forbiddenchars.Length();i++)
					{
						if ((pos = newfilename.Find(forbiddenchars.GetChar(i))) >= 0)
							newfilename.SetChar(pos,'_');
					}
				}
				if (newfilename != downloadfile->GetName())
				{
					if (downloadpartindex == 0)
						downloadlist->ChangeName(downloadfile,newfilename);
				}

                if (restart == YES)
                {
                    downloadfile->sizecompleted[downloadpartindex] = *start - downloadfile->startpoint[downloadpartindex];
                    if (downloadpartindex == 0)
                    {
                        //THIS IS DONE TO RECOVERY THE FILE SIZE OF THE REAL FILE
                        if (realtotalsize_copy >= 0)
                        {
                            downloadfile->totalsize = realtotalsize_copy;
                            realtotalsize_copy = -1;
                        }

                        //KEEP A COPY OF THE FILE SIZE OF THE REAL FILE TO BE DOWNLOADED
                        if (downloadfile->IsMetalink())
                            realtotalsize_copy = downloadfile->totalsize;

                        if ((downloadfile->totalsize > 0) && (downloadfile->totalsize != sizetmp + *start) && (downloadfile->GetCurrentAttempt() == 1) && (!downloadfile->IsMetalink()))
                        {
                            wxString partialfile = downloadfile->GetDestination();
                            if (partialfile.Mid(partialfile.Length()-1,1) != SEPARATOR_DIR)
                                partialfile += SEPARATOR_DIR;
                            partialfile += PREFIX + downloadfile->GetName() + EXT;
                            PrintMessage( _("The file size has changed since the last time.\nRemoving the partially downloaded files...\n"),HTMLERROR);
                            for (int i = 0;i < downloadfile->GetNumberofParts();i++)
                            {
                                ::wxRemoveFile(partialfile + MyUtilFunctions::int2wxstr(i));
                                downloadfile->sizecompleted[i] = 0;
                            }
                            downloadfile->totalsize =  sizetmp + *start;
                            downloadfile->MarkWriteAsPending(TRUE);
                            *start = 0;

                            redirecting = TRUE;
                            client->Close(); delete client;
                            return NULL;
                        }
                        else
                        {
                            downloadfile->totalsize =  sizetmp + *start;
                            downloadfile->MarkWriteAsPending(TRUE);
                        }

                        downloadfile->SetRestartSupport();
                    }
                }
                else
                {
                    if (downloadpartindex == 0)
                    {
                        restart = NO;
                        if (sizetmp > 0)
                        {
                            downloadfile->totalsize =  sizetmp;
                            downloadfile->sizecompleted[downloadpartindex] = 0;
                            PrintMessage( _("This server does not support restart.\n"),HTMLERROR);
                            if (downloadpartindex == 0)
                                downloadfile->SetRestartSupport(FALSE);
                            else
                            {
                                client->Close(); delete client;
                                return NULL;
                            }
                        }
                        else
                        {
                            downloadfile->totalsize =  0;
                            downloadfile->sizecompleted[downloadpartindex] = 0;
                            PrintMessage( _("Unable to return the file size.\n"),HTMLERROR);
                            client->Close(); delete client;
                            return NULL;
                        }
                    }
                    else
                    {
                        PrintMessage( _("Unable to return the file size.\n"),HTMLERROR);
                        //downloadfile->criticalerror = TRUE;
                        client->Close(); delete client;
                        return NULL;
                    }
                }
            }
            else if (returnmessagecode == 3)
            {
                redirecting = TRUE;
                PrintMessage( _("Redirecting to ") + currenturl.GetFullPath() + wxT("\n\n"),HTMLSERVER);
                if (downloadpartindex == 0)
                    downloadlist->ChangeName(downloadfile,currenturl.GetFullPath().Mid(currenturl.GetFullPath().Find('/',true)+1));
                client->Close(); delete client;
                return NULL;
            }
            else //returnmessagecode == 4
            {
                // HTTP/1.1 416 Requested Range Not Satisfiable
                //IF THIS HAPPEN, IS BECAUSE THE THE START POINT IS BIGGER THAT THE END POINT
                if (client->GetCompleteResponse() != 416)
                //    PrintMessage( _("The file already was downloaded.\n"));
                //else
                {
                    PrintMessage( _("Error requesting file.\n"),HTMLERROR);
                    client->Close(); delete client;
                    return NULL;
                }
            }
        }
        else
        {
            PrintMessage( _("The server timed out.\n"),HTMLERROR);
            client->Close(); delete client;
            return NULL;
        }
    }
    else
    {
        delete client;
        return NULL;
    }

    if (downloadfile->GetStatus() == STATUS_STOPED){client->Close(); delete client; return NULL;}

    return client;
}

wxSocketClient *mDownloadThread::ConnectFTP(wxLongLong *start)
{
    mFTP *client = new mFTP();
    wxIPV4address address;
    wxFileName destination;
    wxString buffer = wxEmptyString;
    wxLongLong sizetmp;
    client->Notify(FALSE);
    if (downloadpartindex == 0)
        downloadfile->SetContentType(_("Not available"));
    //client->SetFlags(wxSOCKET_NOWAIT);

    address.Service(currenturl.GetPort());
    destination.Assign(downloadfile->GetDestination());
    destination.SetFullName(PREFIX + downloadfile->GetName() + EXT + MyUtilFunctions::int2wxstr(downloadpartindex));
    redirecting = FALSE;

    if (downloadfile->GetStatus() == STATUS_STOPED){client->Close(); delete client; return NULL;}

    if (currenturl.GetPassword().IsEmpty())
    {
        if (downloadfile->GetPassword().IsEmpty())
        {
            client->SetUser(ANONYMOUS_USER);
            client->SetPassword(ANONYMOUS_PASS);
        }
        else
        {
            client->SetUser(downloadfile->GetUser());
            client->SetPassword(downloadfile->GetPassword());
        }
    }
    else
    {
        client->SetUser(currenturl.GetUser());
        client->SetPassword(currenturl.GetPassword());
    }

    PrintMessage( _("Resolving host '") + currenturl.GetHost() + wxT("' ..."));
    if (address.Hostname(currenturl.GetHost())==FALSE)
    {
        PrintMessage( _("\nHost not found.\n"),HTMLERROR);
        client->Close();
        delete client;
        return NULL;
    }
    else
        PrintMessage( _(" OK\n"),HTMLSERVER);

    if (downloadfile->GetStatus() == STATUS_STOPED){client->Close(); delete client; return NULL;}

    PrintMessage( _("Trying to connect in '") + currenturl.GetHost() + wxT("' ..."));
    this->Sleep(1);
    client->Connect(address,TRUE);
    if (client->IsConnected() == FALSE )
    {
        PrintMessage( _("\nConnection denied.\n"),HTMLERROR);
        client->Close();
        delete client;
        return NULL;
    }
    else
        PrintMessage( _(" OK\n"),HTMLSERVER);

    if (downloadfile->GetStatus() == STATUS_STOPED){client->Close(); delete client; return NULL;}

    PrintMessage( _("Waiting for welcome message.\n"));
    PrintMessage( client->GetLastResult() + wxT("\n"),HTMLSERVER);

    if (downloadfile->GetStatus() == STATUS_STOPED){client->Close(); delete client; return NULL;}

    PrintMessage( _("Changing to directory ") + currenturl.GetDir() + wxT(" ...\n"));
    if (!client->ChDir(currenturl.GetDir()))
    {
        PrintMessage( _("Invalid directory.\n"),HTMLERROR);
        client->Close();
        delete client;
        return NULL;
    }
    PrintMessage( client->GetLastResult() + wxT("\n"),HTMLSERVER);

    if (downloadfile->GetStatus() == STATUS_STOPED){client->Close(); delete client; return NULL;}

    PrintMessage( _("Changing to binary mode...\n"));
    if (client->SendCommand(wxT("TYPE I"))!='2')
    {
        PrintMessage( _("Unable to change to binary mode.\n"),HTMLERROR);
        client->Close();
        delete client;
        return NULL;
    }
    PrintMessage( client->GetLastResult() + wxT("\n"),HTMLSERVER);

    if (downloadfile->GetStatus() == STATUS_STOPED){client->Close(); delete client; return NULL;}

    PrintMessage( _("Verifying if the server supports restarting..."));
    if (client->SendCommand(wxT("REST ") + start->ToString())!= '3')
    {
        PrintMessage( _("\nThis server does not support restart.\n"),HTMLERROR);
        if (downloadpartindex == 0)
            downloadfile->SetRestartSupport(FALSE);
        else
        {
            client->Close();
            delete client;
            return NULL;
        }
    }
    else
    {
        PrintMessage( _(" OK\n"),HTMLSERVER);
        if (downloadpartindex == 0)
            downloadfile->SetRestartSupport();
    }

    if (downloadfile->GetStatus() == STATUS_STOPED){client->Close(); delete client; return NULL;}

    PrintMessage( _("Verifying the size of the ") + currenturl.GetFullName() + wxT("...\n"));
    sizetmp = client->GetFileSize(currenturl.GetFullName());
    if (sizetmp == -1)
    {
        wxArrayString fileList;
        client->GetList(fileList,wxEmptyString,false);
        for (unsigned int i=0;i<fileList.GetCount();i++)
        {
            wxLogDebug(fileList.Item(i));
            if (fileList.Item(i).Lower() == currenturl.GetFullName().Lower())
            {
                sizetmp = client->GetFileSize(fileList.Item(i));
                break;
            }
        }
    }
    if (sizetmp == -1)
    {
        PrintMessage( _("Unable to return the file size.\n"),HTMLERROR);
        downloadfile->criticalerror = TRUE;
        client->Close();
        delete client;
        return NULL;
    }
    else
    {
        PrintMessage( _("File size: ") + sizetmp.ToString() + wxT("\n"),HTMLSERVER);
        if (downloadfile->RestartSupport() == NO)
            downloadfile->sizecompleted[downloadpartindex] = 0;
        else
            downloadfile->sizecompleted[downloadpartindex] = *start - downloadfile->startpoint[downloadpartindex];
        if (downloadpartindex == 0)
        {
            //THIS IS DONE TO RECOVERY THE FILE SIZE OF THE REAL FILE
            if (realtotalsize_copy >= 0)
            {
                downloadfile->totalsize = realtotalsize_copy;
                realtotalsize_copy = -1;
            }

            //KEEP A COPY OF THE FILE SIZE OF THE REAL FILE TO BE DOWNLOADED
            if (downloadfile->IsMetalink())
                realtotalsize_copy = downloadfile->totalsize;

            if ((downloadfile->totalsize > 0) && (downloadfile->totalsize != sizetmp) && (!downloadfile->IsMetalink()))
            {
                wxString partialfile = downloadfile->GetDestination();
                if (partialfile.Mid(partialfile.Length()-1,1) != SEPARATOR_DIR)
                    partialfile += SEPARATOR_DIR;
                partialfile += PREFIX + downloadfile->GetName() + EXT;
                PrintMessage( _("The file size has changed since the last time.\nRemoving the partially downloaded files...\n"),HTMLERROR);
                for (int i = 0;i < downloadfile->GetNumberofParts();i++)
                {
                    ::wxRemoveFile(partialfile + MyUtilFunctions::int2wxstr(i));
                    downloadfile->sizecompleted[i] = 0;
                }
                *start = 0;
                downloadfile->totalsize =  sizetmp;
                downloadfile->MarkWriteAsPending(TRUE);
                redirecting = TRUE;
                client->Close(); delete client;
                return NULL;
            }
            else
            {
                downloadfile->totalsize =  sizetmp;
                downloadfile->MarkWriteAsPending(TRUE);
            }
        }
    }

    PrintMessage( _("Requesting file...\n"));

    return client;
}

wxInputStream *mDownloadThread::ConnectLOCAL_FILE(wxLongLong start)
{
    wxFileInputStream *filestream;
    wxFile *file;
    wxFileName source, destination;
    wxString buffer = wxEmptyString;
    wxLongLong sizetmp;
    if (downloadpartindex == 0)
        downloadfile->SetContentType(_("Not available"));

    source.Assign(currenturl.GetDir() + currenturl.GetFullRealName());
    destination.Assign(downloadfile->GetDestination());
    destination.SetFullName(PREFIX + downloadfile->GetName() + EXT + MyUtilFunctions::int2wxstr(downloadpartindex));
    redirecting = FALSE;
    if (downloadpartindex == 0)
        downloadfile->SetRestartSupport(); //YES

    if (downloadfile->GetStatus() == STATUS_STOPED){return NULL;}

    PrintMessage( _("Looking for file ") + currenturl.GetFullRealName() + wxT("...\n"));
    if (!source.FileExists())
    {
        PrintMessage( _("File not found.\n"),HTMLERROR);
        return NULL;
    }

    file = new wxFile(source.GetFullPath());
    PrintMessage( _("Verifying the size of the ") + currenturl.GetFullRealName() + wxT("...\n"));
    if (!file)
    {
        PrintMessage( _("Unable to return the file size.\n"),HTMLERROR);
        return NULL;
    }
    else
    {
        //THIS IS DONE TO RECOVERY THE FILE SIZE OF THE REAL FILE
        if (realtotalsize_copy >= 0)
        {
            downloadfile->totalsize = realtotalsize_copy;
            realtotalsize_copy = -1;
        }

        //KEEP A COPY OF THE FILE SIZE OF THE REAL FILE TO BE DOWNLOADED
        if (downloadfile->IsMetalink())
            realtotalsize_copy = downloadfile->totalsize;

        sizetmp = file->Length();
        PrintMessage( _("File size: ") + sizetmp.ToString() + wxT("\n"),HTMLSERVER);
        if (downloadpartindex == 0)
            downloadfile->totalsize =  sizetmp;
        downloadfile->sizecompleted[downloadpartindex] = start - downloadfile->startpoint[downloadpartindex];
        file->Close();
        delete file;
    }

    PrintMessage( _("Requesting file...\n"));
    filestream = new wxFileInputStream(source.GetFullPath());
    filestream->SeekI(start.GetValue());

    return filestream;
}

void mDownloadThread::PrintMessage(wxString str,wxString color)
{
    #ifdef USE_HTML_MESSAGES
    wxString newstr(str);
    newstr.Replace(wxT("\r"),wxT(""));
    newstr.Replace(wxT("\n"),wxT("<BR>"));
    downloadfile->messages[downloadpartindex].Append(wxT("<font size=1 color=") + color + wxT(">") + newstr + wxT("</font>"));
    #else
    wxString newstr(str);
    newstr.Replace(wxT("\r"),wxT(""));
    downloadfile->messages[downloadpartindex].Append(newstr);
    #endif
}

void mDownloadThread::SpeedCalculation(long delta_t)
{
    int parts;
    if (downloadfile->IsSplitted())
        parts = downloadfile->GetNumberofParts();
    else
        parts = 1;
    downloadfile->totalspeed = 0;
    for (int i = 0; i < parts; i++)
    {
        if (!downloadfile->finished[downloadpartindex])
            downloadfile->totalspeed += downloadfile->delta_size[i]*1000l / (delta_t);
        downloadfile->delta_size[i] = 0;
    }
}

void mDownloadThread::WaitUntilAllFinished(bool canstop)
{
    if (downloadfile->GetNumberofParts() > 1)
    {
        bool waitall = TRUE;
        while (waitall)
        {
            int i;
            waitall = FALSE;
            for (i=1; i < downloadfile->GetNumberofParts(); i++)
                if (downloadfile->finished[i] == FALSE)
                    waitall = TRUE;
            if ((downloadfile->GetStatus() == STATUS_STOPED) && (canstop))    {return ;}
            Sleep(500);
            if ((downloadfile->GetStatus() == STATUS_STOPED) && (canstop))    {return ;}
        }
    }
}

bool mDownloadThread::JoinFiles(wxFileName *destination,wxFileName tempdestination)
{
    wxFile *outputfile;
    long readbuffersize = 10240;
    bool result;
    long data_size = 60*readbuffersize;
    char *data = new char[data_size];
    long lastread = 0;
	wxLongLong freespace;
    wxString partfilename;

	//CHECK IF HAS ENOUGH DISK SPACE
	if (downloadfile->IsSplitted())
	{
		if (wxGetDiskSpace(destination->GetPath(),NULL,&freespace))
		{
            if (freespace < (downloadfile->totalsize))
            {
                PrintMessage(_("There isn't enough disk space to join the file parts.\n"),HTMLERROR);
                return FALSE;
            }
		}
		else
		{
            PrintMessage(_("Error retrieving disk space. Check if the destination directory exists.\n"),HTMLERROR);
            return FALSE;
		}
	}

    if (destination->FileExists())
    {
        downloadlist->ChangeName(downloadfile,MyUtilFunctions::int2wxstr(downloadfile->GetFinishedDateTime().GetTicks()) + wxT(" - ") + downloadfile->GetName());
        destination->SetFullName(downloadfile->GetName());
    }
    partfilename = tempdestination.GetFullPath();
    result = wxRenameFile(tempdestination.GetFullPath(),destination->GetFullPath());
    if ((downloadfile->IsSplitted()) && (result == TRUE))
    {
        outputfile = new wxFile(destination->GetFullPath(),wxFile::write_append);
        partfilename = tempdestination.GetFullPath().Mid(0,tempdestination.GetFullPath().Length()-1); //GetNumberofParts()
        int i;
        for (i=1;i < downloadfile->GetNumberofParts();i++)
        {
            if (::wxFileExists(partfilename + MyUtilFunctions::int2wxstr(i)))
            {
                wxFile *piece = new wxFile(partfilename + MyUtilFunctions::int2wxstr(i),wxFile::read);
                while (!piece->Eof())
                {
                    lastread = piece->Read(data,data_size);
                    outputfile->Write(data,lastread);
                    lastread = 0;
                }
                piece->Close();
                delete piece;
                wxRemoveFile(partfilename + MyUtilFunctions::int2wxstr(i));
            }
            else
            {
                result = FALSE;
                break;
            }
        }
        outputfile->Close();
        delete outputfile;
    }
    delete [] data;
    return result;
}
