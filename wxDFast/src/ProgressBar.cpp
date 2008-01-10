//
// C++ Implementation: ProgressBar
//
// Description: Implements the segmented progress bar
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"

IMPLEMENT_DYNAMIC_CLASS(mProgressBar, wxPanel)

BEGIN_EVENT_TABLE(mProgressBar, wxPanel)
    EVT_PAINT  (mProgressBar::OnPaint)
END_EVENT_TABLE()

mProgressBar::mProgressBar()
{
    SetParams(0,NULL);
}

void mProgressBar::OnPaint(wxPaintEvent &event)
{
    wxBufferedPaintDC dc(this);
    int realwidth, realheight;
    int width, height,x,y;

    //GET THE LIMITS OF THE WINDOW
    this->GetSize(&realwidth,&realheight);
    height = realheight-2;
    width = realwidth-2;
    x = 1;
    y = 1;

    //CLEAR THE PROGRESS BAR AREA
    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();
    dc.SetBrush(*wxWHITE_BRUSH);
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawRectangle(x,y,width,height);

    if (parts > 0)
    {
        int proportion = 100/parts;
        int lastproportion;
        int scale = width/100;
        int partssize = scale*proportion;
        int tmpwidth;

        for (int i = 0 ; i < parts ; i++)
        {
            if (i == (parts -1))
            {
                lastproportion = 100 - ((parts-1)*proportion);
                tmpwidth = width-((parts -1)*partssize);
                tmpwidth = completed[i]*tmpwidth/lastproportion;
            }
            else
                tmpwidth = completed[i]*scale;

            if (i % 3 == 0)
            {
                dc.SetBrush(wxBrush(LIGHT_BLUE));
                dc.SetPen(wxPen(LIGHT_BLUE));
            }
            else if (i % 3 == 1)
            {
                dc.SetBrush(wxBrush(RED));
                dc.SetPen(wxPen(RED));
            }
            else
            {
                dc.SetBrush(wxBrush(YELLOW));
                dc.SetPen(wxPen(YELLOW));
            }
            dc.DrawRectangle(x+partssize*i,y,tmpwidth,height);
        }
    }

    //DRAW THE PROGRESS BAR BORDER
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawRectangle(x,y,width,height);

}

bool mProgressBar::Hide()
{
    return Show(false);
}

bool mProgressBar::Show(bool show)
{
    wxSplitterWindow *splitter = XRCCTRL(*mainframe, "splitter01",wxSplitterWindow);
    int width,height;
    bool value = FALSE;
    if (show)
    {
        #if wxCHECK_VERSION(2, 8, 0)
        this->SetInitialSize(wxSize(200,30));
        #else
        this->SetBestFittingSize(wxSize(200,30));
        #endif
        splitter->GetSize(&width,&height);
        this->SetSize(wxSize(width,30));
        if (!IsShown())
        {
            splitter->SetSize(wxSize(width,height-30-5));
            value = wxPanel::Show(TRUE);
            //this->Refresh();
        }
    }
    else if (IsShown())
    {
        value = wxPanel::Show(FALSE);
        #if wxCHECK_VERSION(2, 8, 0)
        this->SetInitialSize(wxSize(200,0));
        #else
        this->SetBestFittingSize(wxSize(200,0));
        #endif
        splitter->GetSize(&width,&height);
        splitter->SetSize(wxSize(width,height+30+5));
    }
    return value;
}

void mProgressBar::SetParams(int parts,int *completed)
{
    this->parts = parts;
    for (int i=0; i < parts; i++)
        this->completed[i] = completed[i];
}

void mProgressBar::SetMainFrame(mMainFrame *mainframe)
{
    this->mainframe = mainframe;
}

