//
// C++ Implementation: TaskBarIcon
//
// Description: Implements the class taskbaricon, with include a icon on the tray.
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"

////////////////////////XPM IMAGES////////////////////////////////
/*#ifndef __WXMSW__
#include "../resources/xpm/wxdfast.xpm"
#endif*/


BEGIN_EVENT_TABLE(mTaskBarIcon, wxTaskBarIcon)
    EVT_MENU(HIDE,  mTaskBarIcon::OnHide)
    EVT_MENU(NEW,  mTaskBarIcon::OnNew)
    EVT_MENU(CLOSE,  mTaskBarIcon::OnClose)
    EVT_MENU(OFF,  mTaskBarIcon::OnBandUnlimited)
    EVT_MENU(ON,  mTaskBarIcon::OnBandControlOn)
    EVT_MENU(PERDOWNLOAD,  mTaskBarIcon::OnBandControlPerDownload)
    EVT_TASKBAR_LEFT_DOWN(mTaskBarIcon::OnLButtonClick)
    EVT_TASKBAR_MOVE(mTaskBarIcon::OnMouseMove)
END_EVENT_TABLE()

mTaskBarIcon::mTaskBarIcon(mMainFrame *frame)
{
    mainframe = frame;
};

void mTaskBarIcon::OnLButtonClick(wxTaskBarIconEvent&)
{
    wxCommandEvent event;
    OnHide(event);
}

void mTaskBarIcon::OnMouseMove(wxTaskBarIconEvent&)
{
    wxString taskTip = PROGRAM_NAME;
    int totalpercentual = 0,count = 0;
    float totalspeed = 0.0;
    if(!wxGetApp().downloadlist.IsEmpty())
    {
        for ( mDownloadList::Node *node = wxGetApp().downloadlist.GetFirst(); node; node = node->GetNext() )
        {
            mDownloadFile *current = node->GetData();
            if  (current->GetStatus() == STATUS_ACTIVE)
            {
                totalpercentual += current->GetProgress();
                totalspeed += current->totalspeed;
                count++;
            }
        }
        if (count > 0)
        {
            wxString msg;
            msg = _("Total Speed:");
            msg += wxT(" %.2fkB/s\n");
            msg += _("Total Complete:");
            msg += wxT(" %d%%\n");
            msg += _("Downloads in progress:");
            msg += wxT(" %d");
            taskTip.Clear();
            taskTip.Printf(msg, totalspeed/1024.0, totalpercentual/count,count);
        }
    }
    SetIcon(wxGetApp().appicon,taskTip);

}

wxMenu *mTaskBarIcon::CreatePopupMenu()
{
    wxMenu *traymenu = new wxMenu;
    wxMenuItem *mnuhide;
    if (mainframe)
    {
        if (mainframe->IsShown())
            mnuhide = new wxMenuItem(traymenu,HIDE, _("Hide the main window"));
        else
            mnuhide = new wxMenuItem(traymenu,HIDE, _("Show the main window"));
        wxMenuItem *mnunew = new wxMenuItem(traymenu,NEW, _("New download"));
        wxMenuItem *mnuclose = new wxMenuItem(traymenu,CLOSE, _("Close the program"));

        wxMenu *mnusubband = new wxMenu;
        wxMenuItem *mnuband = new wxMenuItem(traymenu,BAND,_("Band control"));

        wxMenuItem *mnuoff = new wxMenuItem(mnusubband,OFF, _("Unlimited"),_("Just for this session. To change definitely go to \"Options\""),wxITEM_RADIO);
        wxString temp; temp = _("Active"); temp += wxT(" (") + MyUtilFunctions::int2wxstr(mainframe->programoptions.bandwidth) + wxT(" kB/s)");
        wxMenuItem *mnuon = new wxMenuItem(mnusubband,ON, temp,_("Just for this session. To change definitely go to \"Options\""),wxITEM_RADIO);
        wxMenuItem *mnuperdownload = new wxMenuItem(mnusubband,PERDOWNLOAD, _("Per Download"),_("Just for this session. To change definitely go to \"Options\""),wxITEM_RADIO);

        mnusubband->Append(mnuoff);
        mnusubband->Append(mnuon);
        mnusubband->Append(mnuperdownload);
        mnuband->SetSubMenu(mnusubband);

        mnunew->SetBitmap(NewDownload);
        mnuclose->SetBitmap(Quit);
        traymenu->Append(mnuhide);
        traymenu->AppendSeparator();
        traymenu->Append(mnunew);
        traymenu->Append(mnuband);
        traymenu->AppendSeparator();
        traymenu->Append(mnuclose);

        if (mainframe->programoptions.bandwidthoption == 1)
            mnuperdownload->Check();
        else if (mainframe->programoptions.bandwidthoption == 2)
            mnuon->Check();
        else
            mnuoff->Check();
    }
    return traymenu;
}

void mTaskBarIcon::OnClose(wxCommandEvent& event)
{
    if (mainframe)
        mainframe->Close();
}

void mTaskBarIcon::OnNew(wxCommandEvent& event)
{
    if (mainframe)
        mainframe->OnNew(event);
}

void mTaskBarIcon::OnHide(wxCommandEvent& event)
{
    if (mainframe)
    {
        if (mainframe->IsShown())
        {
            if (mainframe->active)
            {
                restoring = TRUE;
                mainframe->Hide();
            }
        }
        else
        {
            restoring = TRUE;
            if (mainframe->IsIconized())
                mainframe->Iconize(FALSE);
            mainframe->Show();
            restoring = FALSE;
            //mainframe->RequestUserAttention();
        }
    }
}

void mTaskBarIcon::OnBandUnlimited(wxCommandEvent& event)
{
    if (mainframe)
    {
        #ifndef DISABLE_MUTEX
        mainframe->mutex_programoptions->Lock();
        #endif
        mainframe->programoptions.bandwidthoption = 0;
        #ifndef DISABLE_MUTEX
        mainframe->mutex_programoptions->Unlock();
        #endif
        if (mainframe->statusbar)
        {
            wxString temp = _("Band control");
            temp += wxT(": ");
            temp += _("Unlimited");
            mainframe->statusbar->SetStatusText(temp,2);
        }
    }
}

void mTaskBarIcon::OnBandControlOn(wxCommandEvent& event)
{
    if (mainframe)
    {
        #ifndef DISABLE_MUTEX
        mainframe->mutex_programoptions->Lock();
        #endif
        mainframe->programoptions.bandwidthoption = 2;
        #ifndef DISABLE_MUTEX
        mainframe->mutex_programoptions->Unlock();
        #endif
        if (mainframe->statusbar)
        {
            wxString temp = _("Band control");
            temp += wxT(": ");
            temp += _("Active");
            temp += wxT(" (") + MyUtilFunctions::int2wxstr(mainframe->programoptions.bandwidth) + wxT(" kB/s)");
            mainframe->statusbar->SetStatusText(temp,2);
        }
    }
}

void mTaskBarIcon::OnBandControlPerDownload(wxCommandEvent& event)
{
    if (mainframe)
    {
        #ifndef DISABLE_MUTEX
        mainframe->mutex_programoptions->Lock();
        #endif
        mainframe->programoptions.bandwidthoption = 1;
        #ifndef DISABLE_MUTEX
        mainframe->mutex_programoptions->Unlock();
        #endif
        if (mainframe->statusbar)
        {
            wxString temp = _("Band control");
            temp += wxT(": ");
            temp += _("Per Download");
            mainframe->statusbar->SetStatusText(temp,2);
        }
    }
}
