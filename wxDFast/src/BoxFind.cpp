//
// C++ Implementation: BoxFind
//
// Description: Implements the Find file box.
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"

BEGIN_EVENT_TABLE(mBoxFind, wxFindReplaceDialog)
    EVT_FIND(-1, mBoxFind::OnFind)
    EVT_FIND_NEXT(-1, mBoxFind::OnFind)
    EVT_FIND_CLOSE(-1, mBoxFind::OnClose)
END_EVENT_TABLE()

void mBoxFind::OnFind(wxFindDialogEvent& event)
{
    int i = -1, index = -1, flags, countitems;
    int column;
    bool found = FALSE;
    wxListCtrl *list;
    mInProgressList *list01 = XRCCTRL(*(wxGetApp().mainframe), "inprogresslist",mInProgressList );
    mFinishedList *list02 = XRCCTRL(*(wxGetApp().mainframe), "finishedlist",mFinishedList);
    wxNotebook *notebook = XRCCTRL(*(wxGetApp().mainframe), "notebook01",wxNotebook );
    if (notebook->GetSelection() == 0) //DOWNLOADS IN PROGRESS

    {
        index = list01->GetCurrentLastSelection();
        column = INPROGRESS_NAME;
        list = (wxListCtrl*)list01;
    }
    else if (notebook->GetSelection() == 1) //FINISHED DOWNLOADS
    {
        index = list02->GetCurrentLastSelection();
        column = FINISHED_NAME;
        list = (wxListCtrl*)list02;
    }
    else
    {
        wxMessageBox(_("Unable to search in this page.\nChange to another page and try again"),_("Error..."),wxOK | wxICON_ERROR, this);
        return ;
    }

    flags = event.GetFlags();
    if (flags & wxFR_DOWN)
    {
        i = index+1;
        countitems = list->GetItemCount();
    }
    else
    {
        if (index > -1)
            i = index-1;
        countitems = -1;
    }
    while (i != countitems)
    {
        wxListItem item;
        item.SetId(i);
        item.SetMask(wxLIST_MASK_STATE|wxLIST_MASK_TEXT);

        item.SetColumn(column);

        list->GetItem(item);
        if (flags & wxFR_WHOLEWORD)
        {
            if (flags & wxFR_MATCHCASE)
            {
                if (item.GetText() == event.GetFindString())
                    found = TRUE;
            }
            else
            {
                if (item.GetText().Lower() == event.GetFindString().Lower())
                    found = TRUE;
            }
        }
        else
        {
            if (flags & wxFR_MATCHCASE)
            {
                if (item.GetText().Find(event.GetFindString()) >= 0)
                    found = TRUE;
            }
            else
            {
                if (item.GetText().Lower().Find(event.GetFindString().Lower()) >= 0)
                    found = TRUE;
            }
        }
        if (found)
        {
            list->SetItemState(i,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
            break;
        }
        if (flags & wxFR_DOWN)
            i++;
        else
            i--;
    }
    if (!found)
        wxMessageBox(_("File not found."), _("Information..."),wxOK | wxICON_INFORMATION, this);

    return ;
}

void mBoxFind::OnClose(wxFindDialogEvent& event)
{
    this->Destroy();
    return ;
}
