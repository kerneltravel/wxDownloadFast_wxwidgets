//
// C++ Implementation: BoxNew
//
// Description: Implements the New Download Box.
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"

BEGIN_EVENT_TABLE(mBoxNew, wxDialog)
    EVT_BUTTON(XRCID("btnok"), mBoxNew::OnOk)
    EVT_BUTTON(XRCID("btncancel"), mBoxNew::OnCancel)
    EVT_BUTTON(XRCID("btndir"), mBoxNew::OnButtonDir)
    EVT_BUTTON(XRCID("btnadd"), mBoxNew::OnButtonAdd)
    EVT_BUTTON(XRCID("btnedit"), mBoxNew::OnButtonEdit)
END_EVENT_TABLE()

void mBoxNew::OnOk(wxCommandEvent& event)
{
    unsigned int j;
    bool atleastoneitemischecked = FALSE;
    wxCheckListBox *list = XRCCTRL(*this, "lstaddresslist",wxCheckListBox);
    wxString name;
    wxString urlfromedturl = XRCCTRL(*this, "edturl",wxTextCtrl)->GetValue();
    if (!urlfromedturl.IsEmpty())
    {
        mUrlName textctrlurl(urlfromedturl);
        int result = this->CheckURL(textctrlurl);
        if (result == 1)
        {
            XRCCTRL(*this, "edturl",wxTextCtrl)->SetValue(wxEmptyString);
        }
        else if (result == 2)
        {
            XRCCTRL(*this, "lstaddresslist",wxCheckListBox)->Insert(textctrlurl.GetFullPath(),0);
            XRCCTRL(*this, "lstaddresslist",wxCheckListBox)->Check(0);
            XRCCTRL(*this, "edturl",wxTextCtrl)->SetValue(wxEmptyString);
        }
        else
        {
            wxMessageBox(_("The following URL is invalid:\n") + textctrlurl.GetFullPath(),_("Error..."),wxOK | wxICON_ERROR,this);
            return;
        }
    }

    for (j = 0; j < list->GetCount(); j++)
    {
        if (!list->IsChecked(j))
            continue;
        atleastoneitemischecked = TRUE;
        wxString url;
        url = list->GetString(j);
        mUrlName *urltmp;
        if (url.Length() < 3)
        {
            wxMessageBox(_("The URL is invalid!"),_("Error..."),wxOK | wxICON_ERROR,this);
            return;
        }
        int i = 0, count = 0;
        for (i=0; i < (int)(url.Length()-1);i++)
        {
            if (url.Mid(i,1) == SEPARATOR_URL)
                count++;
        }
        if ((count < 1) || (url.Mid(url.Length()-1,1) == SEPARATOR_URL))
        {
            if (url.Mid(url.Length()-1,1) != SEPARATOR_URL)
                url = url + SEPARATOR_URL;
            url = url + wxT("index.html");
        }
        urltmp = new mUrlName(url);
        if (!urltmp->IsComplete())
        {
            wxMessageBox(_("The following URL is invalid:\n") + url,_("Error..."),wxOK | wxICON_ERROR,this);
            delete urltmp;
            return;
        }
        list->SetString(j,urltmp->GetFullPath());
        list->Check(j);
        if (name.IsEmpty())
            name = urltmp->GetFullName();
        if (!this->PermitDifferentNames())
        {
            if (name != urltmp->GetFullName())
            {
                wxMessageBox(_("The following URL has a different filename:\n") + urltmp->GetFullPath(),_("Error..."),wxOK | wxICON_ERROR,this);
                return;
            }
        }
        delete urltmp;
    }
    if (!atleastoneitemischecked)
    {
        wxMessageBox(_("Select at least one item of the address list!\n"), _("Information..."),wxOK | wxICON_INFORMATION,this);
        return;
    }

    wxString destination;
    destination = XRCCTRL(*this, "edtdestination",wxTextCtrl)->GetValue();
    wxFileName *desttmp = new wxFileName();
    desttmp->AssignDir(destination);
    if (!desttmp->DirExists())
    {
        if (wxMessageBox(_("The destination directory does not exist!\nDo you want to create it?"),_("Continue..."),wxYES| wxNO | wxICON_QUESTION,this) == wxYES)
        {
            wxLogNull nolog;
            if (!desttmp->Mkdir(0777,wxPATH_MKDIR_FULL))
            {
                wxMessageBox(_("You don't have permission to create the directory!"),_("Error..."),wxOK | wxICON_ERROR,this);
                delete desttmp;
                return;
            }
        }
        else
        {
            delete desttmp;
            return;
        }
    }
    if (!desttmp->IsDirWritable())
    {
        wxMessageBox(_("You don't have write permissions for this directory!"),_("Error..."),wxOK | wxICON_ERROR,this);
        delete desttmp;
        return;
    }
    if (XRCCTRL(*this, "spinsplit",wxSpinCtrl)->GetValue() > MAX_NUM_PARTS)
    {
        wxMessageBox(_("I can't split the file into so many pieces!"),_("Error..."),wxOK | wxICON_ERROR,this);
        return;
    }
    delete desttmp;
    EndModal(XRCID("btnok"));
}

void mBoxNew::OnCancel(wxCommandEvent& event)
{
     EndModal(XRCID("btncancel"));
}

void mBoxNew::OnButtonDir(wxCommandEvent& event)
{
     wxString dir;
     dir = wxDirSelector(_("Select the directory:"),XRCCTRL(*this, "edtdestination",wxTextCtrl)->GetValue());
     if (dir != wxEmptyString)
         XRCCTRL(*this, "edtdestination",wxTextCtrl)->SetValue(dir);
}

void mBoxNew::OnButtonAdd(wxCommandEvent& event)
{
    mUrlName url(XRCCTRL(*this, "edturl",wxTextCtrl)->GetValue());
    int result = this->CheckURL(url);
    if (result == 1)
    {
        XRCCTRL(*this, "edturl",wxTextCtrl)->SetValue(wxEmptyString);
        XRCCTRL(*this, "edturl",wxTextCtrl)->SetFocus();
    }
    else if (result == 2)
    {
        XRCCTRL(*this, "lstaddresslist",wxCheckListBox)->Insert(url.GetFullPath(),0);
        XRCCTRL(*this, "lstaddresslist",wxCheckListBox)->Check(0);
        XRCCTRL(*this, "edturl",wxTextCtrl)->SetValue(wxEmptyString);
        XRCCTRL(*this, "edturl",wxTextCtrl)->SetFocus();
    }
    else
        wxMessageBox(_("The URL is invalid!"),_("Error..."),wxOK | wxICON_ERROR,this);
}

void mBoxNew::OnButtonEdit(wxCommandEvent& event)
{
    unsigned int i;
    wxCheckListBox *listbox = XRCCTRL(*this, "lstaddresslist",wxCheckListBox);
    for (i = 0;i < listbox->GetCount();i++)
    {
        if (listbox->IsSelected(i))
        {

            while (TRUE)
            {
                wxTextEntryDialog dialog(this,_("Edit the URL"),_("Edit the URL"),listbox->GetString(i));
                if (dialog.ShowModal() == wxID_OK)
                {
                    mUrlName url(dialog.GetValue());
                    int result = this->CheckURL(url);
                    if (result == 1)
                    {
                        if (listbox->GetString(i).Lower() != url.GetFullPath().Lower())
                            listbox->Delete(i);
                        break;
                    }
                    else if (result == 2)
                    {
                        listbox->Insert(url.GetFullPath(),0);
                        listbox->Check(0);
                        break;
                    }
                    else
                        wxMessageBox(_("The URL is invalid!"),_("Error..."),wxOK | wxICON_ERROR,this);
                }
                else
                    break;
            }
            break;
        }
    }
}

int mBoxNew::CheckURL(mUrlName url)
{
    if (url.IsComplete())
    {
        wxCheckListBox *list = XRCCTRL(*this, "lstaddresslist",wxCheckListBox);
        for (unsigned int i = 0; i < list->GetCount(); i++)
        {
            if (list->GetString(i) == url.GetFullPath())
                //alreadyexist
                return 1;
        }
        return 2;
    }
    else
        return -1;
}

bool mBoxNew::PermitDifferentNames()
{
    return this->permitdifferentnames;
}

void mBoxNew::SetDifferentNamesPermition(bool permit)
{
    this->permitdifferentnames = permit;
}
