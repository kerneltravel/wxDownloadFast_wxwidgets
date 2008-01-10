//
// C++ Implementation: DownloadFile
//
// Description: Implements the classes DownloadList and DownloadFile.
//              It's this classes which keep all the downloads informations,
//              like URL, size name, etc
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"

void mDownloadList::RecreateIndex()
{
    int i=0;
    for ( mDownloadList::Node *node = this->GetFirst(); node; node = node->GetNext() )
    {
        mDownloadFile *current = node->GetData();
        if (current->index != i)
        {
            current->index = i;
            current->changedsincelastsave = true;
            current->RegisterListItemOnDisk();
        }
        i++;
    }
}

void mDownloadList::ChangePosition(mDownloadFile *file01, mDownloadFile *file02)
{
    int index01,index02;
    index01 = file01->index;
    index02 = file02->index;
    file01->index = index02;
    file02->index = index01;

    Sort(mDownloadList::ListCompareByIndex);
}

void mDownloadList::ChangeName(mDownloadFile *file, wxString name, int value)
{
    wxString strname = name;
    if (file->name == strname)
        return ;
    #ifdef WXDFAST_PORTABLE
    wxFileConfig *config = new wxFileConfig(DFAST_REG, wxEmptyString, DFAST_REG + wxT(".ini"), wxEmptyString,
                                            wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
    #else
    wxFileConfig *config = new wxFileConfig(DFAST_REG);
    #endif
    config->SetPath(INPROGRESS_REG);
    config->DeleteGroup(file->name);
    delete config;
    if (value > 0)
        strname = MyUtilFunctions::int2wxstr(value+1) + wxT("-") + strname;
    if (this->FindDownloadFile(strname))
    {
        this->ChangeName(file,name,value+1);
    }
    else
    {
        file->name = strname;
        file->MarkWriteAsPending(TRUE);
    }
}

void mDownloadList::ChangeDownload(mDownloadFile *file, mUrlList *urllist,wxFileName destination, wxString user, wxString password,wxString reference, wxString comments,wxString command,int bandwidth)
{
    if (file->urllist)
        delete file->urllist;
    file->urllist->DeleteContents(TRUE);
    file->urllist = urllist;
    file->destination = destination.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);
    file->user = user;
    file->password = password;
    file->reference = reference;
    file->command = command;
    file->comments = comments;
    file->bandwidth = bandwidth;
    file->MarkWriteAsPending(TRUE);
}

void mDownloadList::LoadDownloadListFromDisk()
{
    #ifdef WXDFAST_PORTABLE
    wxFileConfig *config = new wxFileConfig(DFAST_REG, wxEmptyString, DFAST_REG + wxT(".ini"), wxEmptyString,
                                            wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
    #else
    wxFileConfig *config = new wxFileConfig(DFAST_REG);
    #endif
    wxString name;
    wxString tmp;
    int status;
    long index;
    config->SetPath(INPROGRESS_REG);
    if (config->GetFirstGroup(name, index))
    {

        do
        {
            mDownloadFile *file = new mDownloadFile();
            file->name = name;
            this->Append(file);
        }
        while(config->GetNextGroup(name, index));
    }
    for ( mDownloadList::Node *node = this->GetFirst(); node; node = node->GetNext() )
    {
        mDownloadFile *file = node->GetData();
        tmp = wxEmptyString;
        config->SetPath(file->GetName());
        config->Read(INDEX_REG,&(file->index));

        config->Read(STATUS_REG,&(status));
        config->Read(SCHEDULED_REG,&(file->scheduled));
        if (status == STATUS_STOPED)
            file->SetAsStoped();
        else if ((status == STATUS_ACTIVE) || (status == STATUS_QUEUE))
            file->PutOnQueue();
        else if (status == STATUS_FINISHED)
            file->SetAsFinished();
        else if (status == STATUS_ERROR)
            file->ErrorOccurred();
        else if ((status == STATUS_SCHEDULE_QUEUE) || (file->scheduled))
            file->PutOnScheduleQueue();
        else
            file->SetAsStoped();

        config->Read(RESTART_REG,&(file->restart));
        config->Read(PARTS_REG,&(file->parts));
        if (file->parts > MAX_NUM_PARTS)
            file->parts = 1;

        config->Read(DESTINATION_REG,&(file->destination));
        #ifdef WXDFAST_PORTABLE
        {
            #ifdef __WXMSW__
            wxFileName destinationtmp(file->destination);
            if (destinationtmp.GetVolume().Upper() == wxT("PORTABLE"))
            {
                destinationtmp.SetVolume(wxGetApp().programvolume);
                file->destination = destinationtmp.GetFullPath();
            }
            #endif
        }
        #endif

        config->Read(TEMPDESTINATION_REG,&(file->tempdestination));

        //TOTAL SIZE
        config->Read(SIZE_REG,&tmp);
        file->totalsize = MyUtilFunctions::wxstrtolonglong(tmp);

        //TOTAL SIZE COMPLETED
        config->Read(SIZECOMPLETED_REG,&tmp);
        file->totalsizecompleted = MyUtilFunctions::wxstrtolonglong(tmp);

        //EACH PART SIZE AND SIZECOMPLETED
        for (int i = 0 ; i < file->parts ; i++)
        {
            tmp = wxT("0");
            config->Read(SIZEPART_REG + MyUtilFunctions::int2wxstr(i+1),&tmp);
            file->size[i] = MyUtilFunctions::wxstrtolonglong(tmp);

            tmp = wxT("0");
            config->Read(SIZEPARTCOMPLETED_REG + MyUtilFunctions::int2wxstr(i+1),&tmp);
            file->sizecompleted[i] = MyUtilFunctions::wxstrtolonglong(tmp);
        }

        config->Read(TIMEPASSED_REG,&tmp);
        file->timepassed = MyUtilFunctions::wxstrtolonglong(tmp);

        config->Read(SPEED_REG,&(file->totalspeed));
        config->Read(PERCENTUAL_REG,&(file->percentual));
        config->Read(MD5_REG,&(file->MD5));
        config->Read(COMMENTS_REG,&(file->comments));
        config->Read(REFERENCE_REG,&(file->reference));
        config->Read(COMMAND_REG,&(file->command));

        config->Read(CONTENTTYPE_REG,&(file->contenttype));
        {
            time_t value = 0;
            config->Read(START_REG,(long*)&(value));
            file->start.Set(value);
            value = 0;
            config->Read(END_REG,(long*)&(value));
            file->end.Set(value);
        }
        file->bandwidth = 0;
        config->Read(BANDWIDTH_REG,&(file->bandwidth));
        file->metalinkindex = -1;
        config->Read(METALINK_INDEX_REG,&(file->metalinkindex));

        file->urllist = new mUrlList();
        file->urllist->DeleteContents(TRUE);
        file->currenturl = 0;
        bool existurl = TRUE;
        int count = 1;
        wxString str;
        while (existurl)
        {
            str = wxEmptyString;
            config->Read(URL_REG + MyUtilFunctions::int2wxstr(count),&(str));
            if (str.IsEmpty())
                break;
            else
            {
                mUrlName *urltmp = new mUrlName(str);
                if (urltmp->IsComplete())
                    file->urllist->Append(urltmp);
            }
            count++;
        }

        if (file->percentual > 100)
            file->percentual = 0;
        file->timeremaining = 0;
        file->totalspeed = 0;
        file->currentattempt = 0;
        file->free = TRUE;
        file->criticalerror = FALSE;
        file->split = FALSE;
        file->waitbeforesplit = FALSE;
        file->changedsincelastsave = false;
        file->MarkWriteAsPending(FALSE);
        file->MarkRemoveAsPending(FALSE);
        if ((file->parts < 1) || (file->parts > MAX_NUM_PARTS))
            file->parts = 1;
        file->metalinkdata = NULL;
        config->Read(NEED_TO_REGET_METALINK_REG,&(file->needtoregetmetalink));

        for (int i =0;i<MAX_NUM_PARTS;i++)
        {

            file->messages[i].Clear();
            file->delta_size[i] = 0;
            //file->sizecompleted[i] = 0;
            file->percentualparts[i] = 0;
            file->startpoint[i] = 0;
            //file->size[i] = 0;
            file->finished[i] = FALSE;
        }

        config->Read(USER_REG,&(file->user));
        config->Read(PASSWORD_REG,&(file->password));

        config->SetPath(BACK_DIR_REG);
    }
    this->Sort(mDownloadList::ListCompareByIndex);
    delete config;
}

void mDownloadList::RemoveDownloadRegister(mDownloadFile *currentfile)
{
    /*unsigned int item = this->IndexOf(currentfile);
    if ((item + 1) < this->GetCount())
    {
        int count = item;
        for ( mDownloadList::Node *node = this->Item(item+1); node; node = node->GetNext() )
        {
            node->GetData()->index = count;
            node->GetData()->MarkWriteAsPending(TRUE);
            count++;
        }
    }*/
    mDownloadList::Node *node = this->Find(currentfile);
    this->DeleteNode(node);
}

mDownloadFile *mDownloadList::FindDownloadFile(wxString str)
{
    for ( mDownloadList::Node *node = this->GetFirst(); node; node = node->GetNext() )
        if (node->GetData()->GetName().Lower() == str.Lower())
            return (node->GetData());
    return (NULL);
}

int mDownloadList::ListCompareByIndex(const mDownloadFile** arg1, const mDownloadFile** arg2)
{
    if ((*arg1)->index > (*arg2)->index)
       return 1;
    else
       return -1;
}

mDownloadFile *mDownloadList::NewDownloadRegister(mUrlList *urllist,wxFileName destination,wxFileName tempdestination, int metalinkindex, int parts, wxString user, wxString password, wxString reference, wxString comments,wxString command,int scheduled,int bandwidth,int ontop)
{
    mDownloadFile *file = new mDownloadFile();
    file->metalinkdata = NULL;
    file->needtoregetmetalink = FALSE;
    if (this->GetCount() > 0)
    {
        if (ontop)
            file->index = this->GetFirst()->GetData()->index-1;
        else
            file->index = this->GetLast()->GetData()->index+1;
    }
    else
        file->index = 0;
    file->scheduled = scheduled;
    file->status = STATUS_STOPED;
    file->restart = -1;
    file->parts = parts;
    file->urllist = urllist;
    file->urllist->DeleteContents(TRUE);
    file->currenturl = 0;
    if (metalinkindex > 0)
        file->name = MyUtilFunctions::int2wxstr(metalinkindex) + file->GetFirstUrl().GetFullName();
    else
        file->name = file->GetFirstUrl().GetFullName();
    file->destination = destination.GetFullPath();
    file->tempdestination = tempdestination.GetFullPath();
    file->command = command;
    file->totalsize = 0;
    file->totalsizecompleted = 0;
    file->comments = comments;
    file->reference = reference;
    file->percentual = 0;
    file->timepassed = 0;
    file->timeremaining = 0;
    file->totalspeed = 0;
    file->contenttype = wxEmptyString;
    file->MD5 = wxEmptyString;
    file->start = wxDateTime::Now();
    file->end = wxDateTime::Now();
    file->currentattempt = 0;
    for (int i =0;i<MAX_NUM_PARTS;i++)
    {
        file->messages[i].Clear();
        file->delta_size[i] = 0;
        file->sizecompleted[i] = 0;
        file->percentualparts[i] = 0;
        file->startpoint[i] = 0;
        file->size[i] = 0;
        file->finished[i] = FALSE;
    }
    /*if (user.IsEmpty())
    {
        file->user = ANONYMOUS_USER;
        file->password = ANONYMOUS_PASS;
    }
    else
    {*/
    file->user = user;
    file->password = password;
    file->bandwidth = bandwidth;
    file->free = TRUE;
    file->criticalerror = FALSE;
    file->split = FALSE;
    file->waitbeforesplit = TRUE;
    file->metalinkindex = metalinkindex;

    file->MarkWriteAsPending(TRUE);
    file->MarkRemoveAsPending(FALSE);
    if (ontop)
        this->Insert(file);
    else
        this->Append(file);
    return file;
}

int mDownloadList::GetNumberofActiveDownloads()
{
    if (numberofactivedownloads < 1)
        return 1;
    return numberofactivedownloads;
}

void mDownloadList::SetNumberofActiveDownloads(int number)
{
    numberofactivedownloads = number;
}

void mDownloadFile::RemoveListItemFromDisk()
{
    #ifdef WXDFAST_PORTABLE
    wxFileConfig *config = new wxFileConfig(DFAST_REG, wxEmptyString, DFAST_REG + wxT(".ini"), wxEmptyString,
                                            wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
    #else
    wxFileConfig *config = new wxFileConfig(DFAST_REG);
    #endif
    config->SetPath(FILES_REG);
    if (this->status == STATUS_FINISHED)
    {
        config->SetPath(BACK_DIR_REG);
        config->SetPath(INPROGRESS_REG);
        config->DeleteGroup(this->name);
        config->SetPath(BACK_DIR_REG);
        config->SetPath(BACK_DIR_REG);
        config->SetPath(FINISHED_REG);

    }
    else
    {
        config->SetPath(BACK_DIR_REG);
        config->SetPath(INPROGRESS_REG);
    }
    config->DeleteGroup(this->name);
    delete config;
    this->MarkRemoveAsPending(FALSE);
}

void mDownloadFile::RegisterListItemOnDisk()
{
    if (changedsincelastsave)
    {
        #ifdef WXDFAST_PORTABLE
        wxFileConfig *config = new wxFileConfig(DFAST_REG, wxEmptyString, DFAST_REG + wxT(".ini"), wxEmptyString,
                                                    wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
        #else
        wxFileConfig *config = new wxFileConfig(DFAST_REG);
        #endif
        config->SetPath(FILES_REG);
        if (this->status == STATUS_FINISHED)
        {
            config->SetPath(BACK_DIR_REG);
            config->SetPath(INPROGRESS_REG);
            config->DeleteGroup(this->name);
            config->SetPath(BACK_DIR_REG);
            config->SetPath(BACK_DIR_REG);
            config->SetPath(FINISHED_REG);

        }
        else
        {
            config->SetPath(BACK_DIR_REG);
            config->SetPath(INPROGRESS_REG);
        }
        config->SetPath(this->name);
        config->Write(NAME_REG,this->name);
        config->Write(INDEX_REG,this->index);
        config->Write(STATUS_REG,this->status);
        config->Write(SCHEDULED_REG,this->scheduled);
        config->Write(RESTART_REG,this->restart);
        config->Write(PARTS_REG,this->parts);
        #ifdef WXDFAST_PORTABLE
        {
            #ifdef __WXMSW__
            wxFileName destinationtmp(this->destination);
            if (destinationtmp.GetVolume().Lower() == wxGetApp().programvolume.Lower())
            {
                destinationtmp.SetVolume(wxEmptyString);
                config->Write(DESTINATION_REG,wxT("PORTABLE:") + destinationtmp.GetFullPath());
            }
            else
            	config->Write(DESTINATION_REG,this->destination);
            #else
            config->Write(DESTINATION_REG,this->destination);
            #endif
        }
        #else
        config->Write(DESTINATION_REG,this->destination);
        #endif
        config->Write(TEMPDESTINATION_REG,this->tempdestination);
        config->Write(SIZE_REG,this->totalsize.ToString());
        config->Write(SIZECOMPLETED_REG,this->totalsizecompleted.ToString());

        //EACH PART SIZE AND SIZECOMPLETED
        for (int i = 0 ; i < this->parts ; i++)
        {
            config->Write(SIZEPART_REG + MyUtilFunctions::int2wxstr(i+1),this->size[i].ToString());
            config->Write(SIZEPARTCOMPLETED_REG + MyUtilFunctions::int2wxstr(i+1),this->sizecompleted[i].ToString());
        }

        config->Write(TIMEPASSED_REG,this->timepassed.ToString());
        config->Write(SPEED_REG,this->totalspeed);
        config->Write(PERCENTUAL_REG,this->percentual);
        config->Write(MD5_REG,this->MD5);
        config->Write(START_REG,(long)this->start.GetTicks());
        config->Write(END_REG,(long)this->end.GetTicks());
        config->Write(COMMENTS_REG,this->comments);
        config->Write(REFERENCE_REG,this->reference);
        config->Write(COMMAND_REG,this->command);
        config->Write(CONTENTTYPE_REG,this->contenttype);
        config->Write(BANDWIDTH_REG,this->bandwidth);
        config->Write(NEED_TO_REGET_METALINK_REG,this->needtoregetmetalink);
        config->Write(METALINK_INDEX_REG,this->metalinkindex);

        unsigned int count = 1;
        bool deleteoldurls = TRUE;
        wxString str;
        for ( mUrlList::Node *node = urllist->GetFirst(); node; node = node->GetNext() )
        {
            config->Write(URL_REG + MyUtilFunctions::int2wxstr(count),node->GetData()->GetFullPath());
            count++;
        }
        while (deleteoldurls) //THIS ERASE URLS THAT WAS WRITE BEFORE, BUT THAT DOESN'T WAS REWRITE NOW
        {
            str = wxEmptyString;
            config->Read(URL_REG + MyUtilFunctions::int2wxstr(count),&(str));
            if (str.IsEmpty())
                break;
            else
                config->DeleteEntry(URL_REG + MyUtilFunctions::int2wxstr(count));
        }

        config->Write(USER_REG,this->user);
        config->Write(PASSWORD_REG,this->password);

        delete config;
        changedsincelastsave = false;
    }
    this->MarkWriteAsPending(FALSE);
}

wxString mDownloadFile::GetName()
{
    return this->name;
}

wxString mDownloadFile::GetExposedName()
{
    if (this->exposedname != wxEmptyString)
        return this->exposedname;
    else
        return this->name;
}

void mDownloadFile::SetExposedName(wxString name)
{
    this->exposedname = name;
}

void mDownloadFile::UnSetExposedName()
{
    this->exposedname = wxEmptyString;
}

int mDownloadFile::GetStatus()
{
    return this->status;
}

void mDownloadFile::SetAsActive()
{
    changedsincelastsave = true;
    this->status = STATUS_ACTIVE;
}

void mDownloadFile::SetAsStoped(bool stopschedule)
{
    changedsincelastsave = true;
    this->status = STATUS_STOPED;
    if (stopschedule)
        this->scheduled = FALSE;
}

void mDownloadFile::SetAsFinished()
{
    changedsincelastsave = true;
    this->status = STATUS_FINISHED;
}

void mDownloadFile::ErrorOccurred()
{
    changedsincelastsave = true;
    this->status = STATUS_ERROR;
}

void mDownloadFile::PutOnScheduleQueue()
{
    changedsincelastsave = true;
    this->scheduled = TRUE;
    this->status = STATUS_SCHEDULE_QUEUE;
}

void mDownloadFile::PutOnQueue()
{
    changedsincelastsave = true;
    this->scheduled = FALSE;
    this->status = STATUS_QUEUE;
}

wxString mDownloadFile::GetDestination()
{
    return this->destination;
}

wxString mDownloadFile::GetTemporaryDestination()
{
    if (this->tempdestination.IsEmpty())
        return this->destination;
    else
        return this->tempdestination;
}

wxString mDownloadFile::GetReferenceURL()
{
    return this->reference;
}

wxString mDownloadFile::GetComments()
{
    return this->comments;
}

wxString mDownloadFile::GetUser()
{
    return this->user;
}

wxString mDownloadFile::GetPassword()
{
    return this->password;
}

int mDownloadFile::RestartSupport()
{
    return this->restart;
}

void mDownloadFile::SetRestartSupport(bool support)
{
    changedsincelastsave = true;
    if (support)
        this->restart = YES;
    else
        this->restart = NO;
}

bool mDownloadFile::IsScheduled()
{
    return this->scheduled;
}

wxString mDownloadFile::GetContentType()
{
    return this->contenttype;
}

void mDownloadFile::SetContentType(wxString contenttype)
{
    changedsincelastsave = true;
    this->contenttype = contenttype;
}

bool mDownloadFile::IsHtml()
{
    return this->GetContentType().Lower().Contains(wxT("html"));
}

bool mDownloadFile::IsMetalink()
{
    bool result = FALSE;
    if ((this->GetContentType().Lower().Contains(wxT("metalink"))) ||
        (this->name.Lower().Contains(wxT("metalink"))))
        result = TRUE;
    return result;
}

bool mDownloadFile::IsZip()
{
    return (this->GetContentType().Lower().Contains(wxT("zip"))
          & this->GetName().Lower().Contains(wxT(".zip"))) ;
}

int mDownloadFile::GetNumberofParts()
{
    return this->parts;
}

int mDownloadFile::GetCurrentAttempt()
{
    return this->currentattempt;
}

void mDownloadFile::ResetAttempts()
{
    changedsincelastsave = true;
    this->currentattempt = 1;
}

void mDownloadFile::IncrementAttempt()
{
    changedsincelastsave = true;
    this->currentattempt++;
}

mUrlName mDownloadFile::GetNextUrl()
{
    mUrlName urltmp;
    mUrlList::Node *node;
    if (metalinkdata)
    {
        if (metalinkdata->urllist.GetCount() > this->currenturl)
        {
            node = metalinkdata->urllist.Item(this->currenturl);
            if (node)
            {
                this->currenturl++;
                return *(node->GetData());
            }
            else
            {
                currenturl = 1;
                return this->GetFirstUrl();
            }
        }
        else
        {
            currenturl = 1;
            return this->GetFirstUrl();
        }
    }

    if (!this->urllist)
        return urltmp;
    if (this->urllist->GetCount() > this->currenturl)
    {
        node = this->urllist->Item(this->currenturl);
        if (node)
        {
            this->currenturl++;
            return *(node->GetData());
        }
        else
        {
            currenturl = 1;
            return this->GetFirstUrl();
        }
    }
    else
    {
        currenturl = 1;
        return this->GetFirstUrl();
    }
}

mUrlName mDownloadFile::GetFirstUrl()
{
    mUrlName urltmp;
    mUrlList::Node *node;
    if (metalinkdata)
    {
        node = metalinkdata->urllist.GetFirst();
        if (node)
        {
            return *(node->GetData());
        }
        else
        {
            return urltmp;
        }
    }

    if (!urllist)
        return urltmp;
    node = this->urllist->GetFirst();
    if (node)
    {
        return *(node->GetData());
    }
    else
    {
        return urltmp;
    }
}

bool mDownloadFile::AppendUrl(mUrlName *url)
{
    if (url->IsComplete())
    {
        this->urllist->Append(url);
        this->MarkWriteAsPending(TRUE);
        return TRUE;
    }
    else
        return FALSE;
}

bool mDownloadFile::FindUrl(mUrlName url)
{
    for ( mUrlList::Node *node = this->urllist->GetFirst(); node; node = node->GetNext() )
        if (node->GetData()->GetFullPath().Lower() == url.GetFullPath().Lower())
            return TRUE;
    return FALSE;
}

int mDownloadFile::GetUrlCount()
{
    return this->urllist->GetCount();
}

wxArrayString mDownloadFile::GetUrlArray()
{
    wxArrayString urlarray;
    for ( mUrlList::Node *node = this->urllist->GetFirst(); node; node = node->GetNext() )
        urlarray.Add(node->GetData()->GetFullPath());
    return urlarray;
}

void mDownloadFile::SetFinishedDateTime(wxDateTime time)
{
    changedsincelastsave = true;
    this->end = time;
}

wxDateTime mDownloadFile::GetFinishedDateTime()
{
    return this->end;
}

void mDownloadFile::SetMD5(wxString md5)
{
    changedsincelastsave = true;
    this->MD5 = md5;
}

int mDownloadFile::GetProgress()
{
    return this->percentual;
}

void mDownloadFile::SetProgress(int percentual)
{
    changedsincelastsave = true;
    this->percentual = percentual;
}

bool mDownloadFile::IsSplitted()
{
    return this->split;
}

void mDownloadFile::Split(bool split)
{
    changedsincelastsave = true;
    this->split = split;
    this->waitbeforesplit = FALSE;
}

bool mDownloadFile::WaitingForSplit()
{
    return this->waitbeforesplit;
}

void mDownloadFile::WaitSplit()
{
    this->split = FALSE;
    this->waitbeforesplit = TRUE;
}

bool mDownloadFile::WriteIsPending()
{
    return this->writependig;
}

void mDownloadFile::MarkWriteAsPending(bool pending)
{
    if (pending)
        changedsincelastsave = true;
    this->writependig = pending;
}

void mDownloadFile::SetFree(bool free)
{
    this->free = free;
}

bool mDownloadFile::IsFree()
{
    return this->free;
}

bool mDownloadFile::RemoveIsPending()
{
    return this->removepending;
}

void mDownloadFile::MarkRemoveAsPending(bool pending)
{
    this->removepending = pending;
}

void mDownloadFile::SetBandWidth(int band)
{
    changedsincelastsave = true;
    bandwidth = band;
}

int mDownloadFile::GetBandWidth()
{
    return bandwidth;
}

bool mDownloadFile::NeedToReGetMetalink()
{
    return needtoregetmetalink;
}

void mDownloadFile::SetToReGetMetalinkWhenNeeded(bool reget)
{
    changedsincelastsave = true;
    needtoregetmetalink = reget;
}

int mDownloadFile::GetMetalinkFileIndex()
{
    return metalinkindex;
}

void mDownloadFile::SetMetalinkFileIndex(int index)
{
    changedsincelastsave = true;
    metalinkindex = index;
}

wxString mDownloadFile::GetCommand()
{
    return command;
}

void mDownloadFile::SetChangedSinceLastSave()
{
    changedsincelastsave = true;
}
