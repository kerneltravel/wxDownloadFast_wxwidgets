//
// C++ Implementation: UrlName
//
// Description: Implements a class with handle url's and permits to return
//              filenames, paths, host and verify if this is complete or not.
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"

mUrlName::mUrlName() : wxURI()
{
}

mUrlName::mUrlName(wxString uri)
{
    wxString m_uri = uri;
    //COMPLETE THE ADDRESS
    if ((m_uri.Mid(0,4).Lower()) == wxT("www."))
        m_uri = wxT("http://") + m_uri;
    else if ((m_uri.Mid(0,4).Lower()) == wxT("ftp."))
        m_uri = wxT("ftp://") + m_uri;
    else if ((m_uri.Mid(0,1).Lower()) == wxT("/"))
        m_uri = wxT("file://") + m_uri;
    else if ((m_uri.Mid(1,1).Lower()) == wxT(":"))
        m_uri = wxT("file://") + m_uri;

    wxURI::Create(m_uri);
    if (wxURI::HasScheme())
    {
        wxString scheme = wxURI::GetScheme().Lower();
        if ((scheme != wxT("http")) && (scheme != wxT("ftp")) && (scheme != wxT("file")))
            wxURI::Create(wxT("http://") + m_uri);
    }

    if (wxURI::HasFragment())
    {
        if (wxURI::GetFragment().Mid(0,11).Lower() == wxT("!metalink3!"))
            wxURI::Create(wxURI::GetFragment().Mid(11));
        else
            wxURI::Create(GetFullPath().BeforeLast('#'));
    }
}

bool mUrlName::IsComplete()
{
    if (Type() == LOCAL_FILE)
        return (HasScheme() && HasPath());
    else
        return (HasServer() && HasScheme() && HasPath());
}

wxString mUrlName::GetHost()
{
    return wxURI::GetServer();
}

wxString mUrlName::GetPort()
{
    wxString port = wxURI::GetPort();
    int type = Type();
    if (!HasPort())
    {
        if (type == HTTP)
            port = wxT("80");
        else if (type == FTP)
            port = wxT("21");
        else
            port = wxT("0");
    }
    return port;
}

int mUrlName::Type()
{
    wxString scheme = GetScheme();
    if (scheme == wxT("http"))
        return HTTP;
    else if (scheme == wxT("ftp"))
        return FTP;
    else if (scheme == wxT("file"))
        return LOCAL_FILE;
    else
        return -1;
}

wxString mUrlName::GetDir()
{
    return wxURI::GetPath().BeforeLast('/') + wxT("/");
}

wxString mUrlName::GetFullName()
{
    wxString fullrealname = wxURI::GetPath().AfterLast('/');
    return fullrealname;
}

wxString mUrlName::GetFullRealName()
{
    wxString fullrealname = wxURI::GetPath().AfterLast('/');
    if (wxURI::HasQuery())
        return fullrealname + wxT("?") + wxURI::GetQuery();
    else
        return fullrealname;
}

wxString mUrlName::GetFullPath()
{
    return wxURI::BuildURI();
}

wxString mUrlName::GetUrlUser()
{
    return wxURI::GetUser();
}

wxString mUrlName::GetUrlPassword()
{
    return wxURI::GetPassword();
}
