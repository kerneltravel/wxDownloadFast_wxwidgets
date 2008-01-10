//
// C++ Implementation: Graph
//
// Description: Implements the Graph painting routine.
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"

IMPLEMENT_DYNAMIC_CLASS(mGraph, wxPanel)

BEGIN_EVENT_TABLE(mGraph, wxPanel)
    EVT_PAINT  (mGraph::OnPaint)
END_EVENT_TABLE()


mGraph::mGraph()
{
    mainframe = NULL;
    programoptions = NULL;
    graphpoints = NULL;
}

void mGraph::SetMainFrame(mMainFrame *mainframe)
{
    this->mainframe = mainframe;
    if (mainframe)
        this->programoptions = &(mainframe->programoptions);
    else
        this->programoptions = NULL;

}

void mGraph::OnPaint(wxPaintEvent &event)
{
    wxBufferedPaintDC dc(this);
    if (!mainframe)
        return;
    #ifndef DISABLE_MUTEX
    if (mainframe->mutex_programoptions->TryLock() != wxMUTEX_NO_ERROR)
        return;
    #endif
    if (!programoptions)
        return;
    if (!programoptions->graphshow)
        return;
    int x, y, lastx, lasty,count;
    float dx, dy;

    //DEFINE DE PARAMETERS
    //int newscale = 0;
    int scale = programoptions->graphscale;
    int textarea = programoptions->graphtextarea;
    wxFont smallfont,bigfont;
    bigfont.SetPointSize(programoptions->graphspeedfontsize);
    smallfont.SetPointSize(10);

    //SET THE BACKGROUND COLOR
    wxBrush b(programoptions->graphbackcolor, wxSOLID);
    dc.SetBackground(b);
    dc.Clear();

    //GET THE LIMITS OF THE WINDOW
    this->GetSize(&x,&y);
    dx = 3.0;
    dy = ((float) y)/((float) scale);

    //DRAW A GRID FOR THE GRAPH
    wxPen gridpen(programoptions->graphgridcolor);
    dc.SetPen(gridpen);
    int grid = y/5;
    count = 1;
    while (y >= (grid*count))
    {
        dc.DrawLine(textarea,y-grid*count ,x-((int)dx),y-grid*count);
        count++;
    }
    dc.DrawLine(textarea,0 ,textarea,y);


    //DRAW THE GRAPH
    wxPen linepen(programoptions->graphlinecolor);
    linepen.SetWidth(programoptions->graphlinewidth);
    dc.SetPen(linepen);
    mGraphPoints::Node *node = graphpoints->GetFirst();
    if (node)
    {
        float *current = node->GetData();
        float last = 0.0;
        count = 0;

        //CALCULATE THE STARTPOINT
        int xstart = x - textarea-5;
        int nitens = graphpoints->GetCount();
        int startitem = 0;
        if (xstart > (int)(nitens*dx))
            xstart = x-(int)(nitens*dx);
        else
        {
            startitem = (int)(((nitens*dx)-((float)xstart))/dx);
            xstart = textarea+5;
        }

        lastx = xstart+((int)dx*count);
        lasty = y-((int)(dy*(*current)));
        for (node = node->GetNext() ; node; node = node->GetNext() )
        {
            current = node->GetData();
            if (*current > ((double)scale))
            {
                /*if (((*current)*1.4) > ((double)scale))
                    newscale = (int)((*current)*1.4);*/
                *current = last;
            }
            else
            {
                /*if (((*current) < (((double)scale)*0.1)) && ((*current) > 10))
                    newscale = (int)((*current)*1.4);*/
                last = *current;
            }
            count++;
            if (startitem <= count)
                //            X1    Y1        X2                 Y2
                dc.DrawLine(lastx,lasty ,xstart+((int)dx)*(count-startitem),y-((int)(dy*(*current))));

            lastx = xstart+((int)dx*(count-startitem));
            lasty = y-((int)(dy*(*current)));

            //DON'T DRAW THE GRAPH AFTER THE END OF THE WINDOW
            if ((lastx >= x) || (programoptions->graphhowmanyvalues <= count+1))
                break;
        }
        //WRITE THE CURRENT SPEED
        wxString temp;
        temp.Printf(wxT("%0.1f"), *current);
        temp.Replace(wxT(","),wxT("."));
        dc.SetTextForeground(programoptions->graphfontcolor);
        dc.SetFont(bigfont);
        dc.DrawText(temp,5,5);
        dc.SetFont(smallfont);
        dc.DrawText(wxT("kB/s"),5,programoptions->graphspeedfontsize + 10);
    }
    else
    {
        //WRITE THE DEFAULT SPEED
        dc.SetTextForeground(*wxBLUE);
        dc.SetFont(bigfont);
        dc.DrawText(wxT("0.0"),5,5);
        dc.SetFont(smallfont);
        dc.DrawText(wxT("kB/s"),5,programoptions->graphspeedfontsize + 10);
    }
    /*if (newscale > 0)
    {
        programoptions->graphscale = newscale;
        mApplication::Configurations(WRITE,OPT_GRAPH_SCALE_REG, programoptions->graphscale);
    }*/
    #ifndef DISABLE_MUTEX
    wxGetApp().mainframe->mutex_programoptions->Unlock();
    #endif
}

bool mGraph::Hide()
{
    return Show(false);
}

bool mGraph::Show(bool show)
{
    wxSplitterWindow *splitter = XRCCTRL(*mainframe, "splitter01",wxSplitterWindow);
    int x,y,width,height;
    bool value = FALSE;
    if (show)
    {
        #if wxCHECK_VERSION(2, 8, 0)
        this->SetInitialSize(wxSize(200,programoptions->graphheight));
        #else
        this->SetBestFittingSize(wxSize(200,programoptions->graphheight));
        #endif
        splitter->GetPosition(&x,&y);
        splitter->GetSize(&width,&height);
        this->SetSize(wxSize(width,programoptions->graphheight));
        if (!IsShown())
        {
            splitter->SetSize(x,y+programoptions->graphheight+5,width,height-programoptions->graphheight-5);
            value = wxPanel::Show(TRUE);
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
        splitter->GetPosition(&x,&y);
        splitter->GetSize(&width,&height);
        splitter->SetSize(x,y-programoptions->graphheight-5,width,height+programoptions->graphheight+5);
    }
    return value;
}
