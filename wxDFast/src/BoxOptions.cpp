//
// C++ Implementation: BoxOptions
//
// Description: Implements the Options Box.
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"
#include "wx/colordlg.h"

BEGIN_EVENT_TABLE(mBoxOptions, wxDialog)
    EVT_BUTTON(XRCID("btnoptionsave"), mBoxOptions::OnOk)
    EVT_BUTTON(XRCID("btnoptioncancel"), mBoxOptions::OnCancel)
    EVT_BUTTON(XRCID("btnoptiondestination"), mBoxOptions::OnButtonDir)
    EVT_BUTTON(XRCID("btnfilemanagerpath"), mBoxOptions::OnFileManagerPath)
    EVT_BUTTON(XRCID("btnbrowserpath"), mBoxOptions::OnBrowserPath)
    EVT_BUTTON(XRCID("btntemppath"), mBoxOptions::OnTempPath)

    EVT_BUTTON(XRCID("graphbtnback"), mBoxOptions::OnGraphBackgroundColour)
    EVT_BUTTON(XRCID("graphbtngrid"), mBoxOptions::OnGraphGridColour)
    EVT_BUTTON(XRCID("graphbtnline"), mBoxOptions::OnGraphLineColour)
    EVT_BUTTON(XRCID("graphbtnfont"), mBoxOptions::OnGraphFontColour)
    EVT_BUTTON(XRCID("btnstartdate"), mBoxOptions::OnButtonStartDate)
    EVT_BUTTON(XRCID("btnfinishdate"), mBoxOptions::OnButtonFinishDate)
    EVT_BUTTON(XRCID("btnexceptionadd"), mBoxOptions::OnAdd)
    EVT_BUTTON(XRCID("btnexceptionremove"), mBoxOptions::OnRemove)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(mBoxOptionsColorPanel, wxPanel)
    EVT_PAINT(mBoxOptionsColorPanel::OnPaint)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(mDatePicker, wxDialog)
    EVT_BUTTON(wxID_OK,mDatePicker::OnOk)
END_EVENT_TABLE()

void mBoxOptions::OnOk(wxCommandEvent& event)
{
   EndModal(XRCID("btnoptionsave"));
}

void mBoxOptions::OnCancel(wxCommandEvent& event)
{
     EndModal(XRCID("btnoptioncancel"));
}

void mBoxOptions::OnButtonDir(wxCommandEvent& event)
{
     wxString dir;
     dir = wxDirSelector(_("Select the directory:"));
     if (dir != wxEmptyString)
         XRCCTRL(*this, "edtdestination",wxTextCtrl)->SetValue(dir);
}

void mBoxOptions::OnBrowserPath(wxCommandEvent& event)
{
    wxFileDialog dialog(this, _("Choose a file..."),wxEmptyString,wxEmptyString,wxT("*"),wxOPEN | wxFILE_MUST_EXIST);
    if (dialog.ShowModal()  == wxID_OK)
        XRCCTRL(*this, "edtbrowserpath",wxTextCtrl)->SetValue(dialog.GetPath());
}

void mBoxOptions::OnFileManagerPath(wxCommandEvent& event)
{
    wxFileDialog dialog(this, _("Choose a file..."),wxEmptyString,wxEmptyString,wxT("*"),wxOPEN | wxFILE_MUST_EXIST);
    if (dialog.ShowModal()  == wxID_OK)
        XRCCTRL(*this, "edtfilemanagerpath",wxTextCtrl)->SetValue(dialog.GetPath());
}

void mBoxOptions::OnTempPath(wxCommandEvent& event)
{
     wxString dir;
     dir = wxDirSelector(_("Select the directory:"));
     if (dir != wxEmptyString)
         XRCCTRL(*this, "edttemppath",wxTextCtrl)->SetValue(dir);
}

void mBoxOptions::OnGraphBackgroundColour(wxCommandEvent& event)
{
    wxColour colour = wxGetColourFromUser(this);
    if (colour.Ok())
    {
        XRCCTRL(*(this), "graphpanelback",mBoxOptionsColorPanel)->colour = colour;
        XRCCTRL(*(this), "graphpanelback",mBoxOptionsColorPanel)->Refresh();
    }
}

void mBoxOptions::OnGraphGridColour(wxCommandEvent& event)
{
    wxColour colour = wxGetColourFromUser(this);
    if (colour.Ok())
    {
        XRCCTRL(*(this), "graphpanelgrid",mBoxOptionsColorPanel)->colour = colour;
        XRCCTRL(*(this), "graphpanelgrid",mBoxOptionsColorPanel)->Refresh();
    }
}

void mBoxOptions::OnGraphLineColour(wxCommandEvent& event)
{
    wxColour colour = wxGetColourFromUser(this);
    if (colour.Ok())
    {
        XRCCTRL(*(this), "graphpanelline",mBoxOptionsColorPanel)->colour = colour;
        XRCCTRL(*(this), "graphpanelline",mBoxOptionsColorPanel)->Refresh();
    }
}

void mBoxOptions::OnGraphFontColour(wxCommandEvent& event)
{
    wxColour colour = wxGetColourFromUser(this);
    if (colour.Ok())
    {
        XRCCTRL(*(this), "graphpanelfont",mBoxOptionsColorPanel)->colour = colour;
        XRCCTRL(*(this), "graphpanelfont",mBoxOptionsColorPanel)->Refresh();
    }
}

void mBoxOptionsColorPanel::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
    if (colour.Ok())
    {
        wxBrush b(colour, wxSOLID);
        dc.SetBackground(b);
        dc.Clear();
    }
}

void mBoxOptions::OnButtonStartDate(wxCommandEvent& event)
{
    mDatePicker datepicker(this,0,_("Select the date..."),XRCCTRL(*this, "edtstartdate",wxTextCtrl)->GetValue());
    if (datepicker.ShowModal() == wxID_OK)
        XRCCTRL(*this, "edtstartdate",wxTextCtrl)->SetValue(datepicker.GetSelectedDate());
}

void mBoxOptions::OnButtonFinishDate(wxCommandEvent& event)
{
    mDatePicker datepicker(this,0,_("Select the date..."),XRCCTRL(*this, "edtfinishdate",wxTextCtrl)->GetValue());
    if (datepicker.ShowModal() == wxID_OK)
        XRCCTRL(*this, "edtfinishdate",wxTextCtrl)->SetValue(datepicker.GetSelectedDate());
}

void mBoxOptions::OnAdd(wxCommandEvent& event)
{
    if (XRCCTRL(*this, "lstexceptionlist",wxListBox)->GetCount() < MAX_SCHEDULE_EXCEPTIONS)
    {
        wxString start,finish,day,string;
        start = MyUtilFunctions::int2wxstr(XRCCTRL(*this, "spinexceptionstarthour",wxSpinCtrl)->GetValue(),2) + wxT(":") + MyUtilFunctions::int2wxstr(XRCCTRL(*this, "spinexceptionstartminute",wxSpinCtrl)->GetValue(),2);
        finish = MyUtilFunctions::int2wxstr(XRCCTRL(*this, "spinexceptionfinishhour",wxSpinCtrl)->GetValue(),2) + wxT(":") + MyUtilFunctions::int2wxstr(XRCCTRL(*this, "spinexceptionfinishminute",wxSpinCtrl)->GetValue(),2);
        string = start + wxT(" | ") + finish + wxT(" | ") + XRCCTRL(*this, "comboweekdays",wxComboBox)->GetValue();
        XRCCTRL(*this, "lstexceptionlist",wxListBox)->InsertItems(1,&string,0);
    }
    else
        wxMessageBox(_("Remove some items before adding a new one."));
}

void mBoxOptions::OnRemove(wxCommandEvent& event)
{
    int i;
    wxListBox *listbox = XRCCTRL(*this, "lstexceptionlist",wxListBox);
    for (i = 0;i < MAX_SCHEDULE_EXCEPTIONS;i++)
    {
        if (listbox->IsSelected(i))
        {
            listbox->Delete(i);
            break;
        }
    }
}
