//
// C++ Implementation: CheckNewRelease
//
// Description: Implements a routine to access the wxdfast host, and to check if exists some
//              new release.
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "wxDFast.h"

mCheckNewReleaseThread::mCheckNewReleaseThread()
{

}

void *mCheckNewReleaseThread::Entry()
{
    wxURL url(wxT("http://dfast.sourceforge.net/lastrelease.html"));
    if (url.GetError() == wxURL_NOERR)
    {
        wxInputStream *in_stream;
        char *data = new char[2049];
        in_stream = url.GetInputStream();
        if (in_stream)
        {
            in_stream->Read(data, 2048);
            int read = in_stream->LastRead();
            if (read > 0)
            {
                long old01,old02,old03,new01,new02,new03;
                data[read] = '\0';
                wxString strnewrelease = MyUtilFunctions::str2wxstr(data);
                int pos1, pos2;
                pos1 = strnewrelease.Find(wxT("version=\""));
                if (pos1 >= 0)
                {
                    bool found = FALSE;
                    pos1+= 9;
                    pos2 = pos1;
                    while (pos2++ < read)
                        if (data[pos2] == '\"')
                        {
                            found = TRUE;
                            break;
                        }
                    strnewrelease = strnewrelease.Mid(pos1,pos2-pos1);
                    wxStringTokenizer newrelease(strnewrelease,wxT("."));
                    wxStringTokenizer currentrelease(VERSION,wxT("."));

                    newrelease.GetNextToken().ToLong(&new01);
                    currentrelease.GetNextToken().ToLong(&old01);
                    newrelease.GetNextToken().ToLong(&new02);
                    currentrelease.GetNextToken().ToLong(&old02);
                    newrelease.GetNextToken().ToLong(&new03);
                    currentrelease.GetNextToken().ToLong(&old03);

                    if ((new01*100)+(new02*10)+(new03) > (old01*100)+(old02*10)+(old03))
                    {
                        wxCommandEvent newversionevent(wxEVT_NEW_RELEASE);
                        newversionevent.SetString(strnewrelease);
                        wxGetApp().mainframe->GetEventHandler()->AddPendingEvent(newversionevent);
                    }
                }
            }
            delete [] data;
            delete in_stream;
        }
    }
    return NULL;
}

void mCheckNewReleaseThread::OnExit()
{

}
