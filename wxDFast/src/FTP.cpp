//
// C++ Implementation: ftp
//
// Description: This file contains the implementation of the mFTP class with
//              support to files bigger than 2GB. This is made using the wxLongLong.
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"

class wxInputFTPStream : public wxSocketInputStream
{
public:
    wxInputFTPStream(wxFTP *ftp, wxSocketBase *sock)
        : wxSocketInputStream(*sock)
    {
        m_ftp = ftp;
        // socket timeout automatically set in GetPort function
    }

    virtual ~wxInputFTPStream()
    {
        delete m_i_socket;   // keep at top

        // when checking the result, the stream will
        // almost always show an error, even if the file was
        // properly transfered, thus, lets just grab the result

        // we are looking for "226 transfer completed"
        char code = m_ftp->GetResult();
        if ('2' == code)
        {
            // it was a good transfer.
            // we're done!
             m_ftp->m_streaming = false;
            return;
        }
        // did we timeout?
        if (0 == code)
        {
            // the connection is probably toast. issue an abort, and
            // then a close. there won't be any more waiting
            // for this connection
            m_ftp->Abort();
            m_ftp->Close();
            return;
        }
        // There was a problem with the transfer and the server
        // has acknowledged it.  If we issue an "ABORT" now, the user
        // would get the "226" for the abort and think the xfer was
        // complete, thus, don't do anything here, just return
    }

    wxFTP *m_ftp;

    DECLARE_NO_COPY_CLASS(wxInputFTPStream)
};

wxLongLong mFTP::GetFileSize(const wxString& fileName)
{
    // return the filesize of the given file if possible
    // return -1 otherwise (predominantly if file doesn't exist
    // in current dir)
    wxLongLong filesize = -1;

    // Check for existance of file via wxFTP::FileExists(...)
    wxLogDebug(wxT("Checking FTP file exists"));
    if ( FileExists(fileName) )
    {
        wxLogDebug(wxT("Foundit"));
        wxString command;

        // First try "SIZE" command using BINARY(IMAGE) transfermode
        // Especially UNIX ftp-servers distinguish between the different
        // transfermodes and reports different filesizes accordingly.
        // The BINARY size is the interesting one: How much memory
        // will we need to hold this file?
        TransferMode oldTransfermode = m_currentTransfermode;
        SetTransferMode(BINARY);
        wxLogDebug(wxT("Sending SIZE command"));
        command << _T("SIZE ") << fileName;

        bool ok = CheckCommand(command, '2');

        if ( ok )
        {
            wxLogDebug(wxT("Received ok message"));
            // The answer should be one line: "213 <filesize>\n"
            // 213 is File Status (STD9)
            // "SIZE" is not described anywhere..? It works on most servers
            long statuscode;
            wxStringTokenizer lastresult(GetLastResult(),wxT(" "));
            if (lastresult.CountTokens() == 2)
            {
                ok = true;
                if (!lastresult.GetNextToken().ToLong(&statuscode))
                    ok = false;
                filesize = MyUtilFunctions::wxstrtolonglong(lastresult.GetNextToken());
                if (filesize < 0)
                    filesize = 0;
            }
            else
            {
                // Something bad happened.. A "2yz" reply with no size
                // Fallback
                ok = false;
            }
        }

        // Set transfermode back to the original. Only the "SIZE"-command
        // is dependant on transfermode
        if ( oldTransfermode != NONE )
        {
            SetTransferMode(oldTransfermode);
        }

        // this is not a direct else clause.. The size command might return an
        // invalid "2yz" reply
        if ( !ok )
        {
            wxLogDebug(wxT("Received error message"));
            // The server didn't understand the "SIZE"-command or it
            // returned an invalid reply.
            // We now try to get details for the file with a "LIST"-command
            // and then parse the output from there..
            wxLogDebug(wxT("Getting filelist"));
            wxArrayString fileList;
            if ( GetList(fileList, fileName, true) )
            {
                if ( !fileList.IsEmpty() )
                {
                    // We _should_ only get one line in return, but just to be
                    // safe we run through the line(s) returned and look for a
                    // substring containing the name we are looking for. We
                    // stop the iteration at the first occurrence of the
                    // filename. The search is not case-sensitive.
                    bool foundIt = false;

                    size_t i;
                    for ( i = 0; !foundIt && i < fileList.Count(); i++ )
                    {
                        foundIt = fileList[i].Upper().Contains(fileName.Upper());
                    }
                    wxLogDebug(wxT("Found the file"));

                    if ( foundIt )
                    {
                        // The index i points to the first occurrence of
                        // fileName in the array Now we have to find out what
                        // format the LIST has returned. There are two
                        // "schools": Unix-like
                        //
                        // '-rw-rw-rw- owner group size month day time filename'
                        //
                        // or Windows-like
                        //
                        // 'date size filename'

                        // check if the first character is '-'. This would
                        // indicate Unix-style (this also limits this function
                        // to searching for files, not directories)
                        wxLogDebug(wxT("Checking the file list"));
                        if ( fileList[i].Mid(0, 1) == _T("-") )
                        {
                            wxStringTokenizer lastresult(fileList[i],wxT(" "));
                            if (lastresult.CountTokens() == 9)
                            {
                                wxLogDebug(lastresult.GetNextToken());
                                wxLogDebug(lastresult.GetNextToken());
                                wxLogDebug(lastresult.GetNextToken());
                                wxLogDebug(lastresult.GetNextToken());
                                filesize = MyUtilFunctions::wxstrtolonglong(lastresult.GetNextToken());
                                if (filesize < 0)
                                    filesize = 0;
                            }
                            else
                            {
                                // Hmm... Invalid response
                                wxLogTrace(FTP_TRACE_MASK,
                                           _T("Invalid LIST response"));
                            }
                        }
                        else // Windows-style response (?)
                        {
                            wxStringTokenizer lastresult(fileList[i],wxT(" "));
                            if (lastresult.CountTokens() == 4)
                            {
                                lastresult.GetNextToken();
                                lastresult.GetNextToken();
                                filesize = MyUtilFunctions::wxstrtolonglong(lastresult.GetNextToken());
                                if (filesize < 0)
                                    filesize = 0;
                            }
                            else
                            {
                                // something bad happened..?
                                wxLogTrace(FTP_TRACE_MASK,
                                           _T("Invalid or unknown LIST response"));
                            }
                        }
                    }
                }
            }
        }
    }

    // filesize might still be -1 when exiting
    return filesize;
}

wxInputStream *mFTP::GetInputStream(const wxString& path)
{
    if ( ( m_currentTransfermode == NONE ) && !SetTransferMode(BINARY) )
        return NULL;

    wxSocketBase *sock = GetPort();

    if ( !sock )
    {
        m_lastError = wxPROTO_NETERR;
        return NULL;
    }

    wxString tmp_str = wxT("RETR ") + wxURI::Unescape(path);
    if ( !CheckCommand(tmp_str, '1') )
        return NULL;

    sock = AcceptIfActive(sock);
    if ( !sock )
        return NULL;

    sock->SetFlags(wxSOCKET_NOWAIT);

    m_streaming = true;

    wxInputFTPStream *in_stream;
    in_stream = new wxInputFTPStream(this, sock);

    return in_stream;
}

