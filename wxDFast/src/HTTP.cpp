//
// C++ Implementation: http
//
// Description:This file contains the implementation of the mHTTP class.
//
//
// Author: Max Magalhães Velasques <max@debiancomp1>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"

mHTTP::mHTTP():wxHTTP()
{
    m_getcommand = wxEmptyString;
    m_headersmsg = wxEmptyString;
    m_messagereceived = wxEmptyString;
    m_use_proxy = false;
}

bool mHTTP::Connect(wxSockAddress& addr, bool wait)
{
    return (wxProtocol::Connect(addr,wait));
}

void mHTTP::UseProxy(wxString proxy_authstring)
{
    m_use_proxy = true;
    m_proxy_authstring = proxy_authstring;
}

wxString mHTTP::BuildGetRequest(mUrlName url,wxLongLong start)
{
    m_getcommand = wxEmptyString;
    if (m_use_proxy)
        m_getcommand << wxT("GET ") << url.GetFullPath() << wxT(" HTTP/1.1\r\n");
    else
        m_getcommand << wxT("GET ") << url.GetDir() << url.GetFullRealName() << wxT(" HTTP/1.1\r\n");

    m_headersmsg = wxEmptyString;


    // Send authentication information
    #if wxCHECK_VERSION(2, 8, 0)
    if (!m_username.IsEmpty() || !m_password.IsEmpty())
    {
        m_headersmsg << wxT("Authorization: ") << GenerateAuthString(m_username, m_password) << wxT("\r\n");
    }
    #endif

    if (!m_proxy_authstring.IsEmpty())
        m_headersmsg << wxT("Proxy-Authorization: ") << m_proxy_authstring << wxT("\r\n");

    m_headersmsg << wxT("HOST: ") << url.GetHost() << wxT("\r\n");
    m_headersmsg << wxT("ACCEPT: */*\r\n");
    m_headersmsg << wxT("User-Agent: wxDownload Fast\r\n");
    m_headersmsg << wxT("Range: bytes=") << start << wxT("-\r\n");
    m_headersmsg << wxT("Pragma: no-cache\r\n");
    m_headersmsg << wxT("Connection: close\r\n\r\n");

    return m_getcommand + m_headersmsg;
}

void mHTTP::SendGetRequest()
{
    if (m_getcommand.IsEmpty())
        return;
    char *dados;
    dados = wxstr2str(m_getcommand);
    wxHTTP::Write(dados, strlen(dados));
    delete dados;

    dados = wxstr2str(m_headersmsg);
    wxHTTP::Write(dados, strlen(dados));
    delete dados;
}

bool mHTTP::ParseHeaders()
{
    wxString line;
    char buf[8192];
    bool firstline = TRUE;
    unsigned int lastcount = 0,count = 0;
    unsigned int i;
    wxHTTP::Read(buf,8192);
    lastcount = wxHTTP::LastCount();
    do
    {
        line.Clear();
        while ((buf[count] != '\n') && (count <8192))
        {
            if ((buf[count] != '\n') && (buf[count] != '\r'))
                line.Append(buf[count],1);
            count++;
        }
        if (count < 8192)
        {
            count++;
            if (buf[count] == '\r')
                count++;
        }
        else
        {
            m_messagereceived += wxT("\nImcomplete message\n");
            return false;
        }
        m_messagereceived += line + wxT("\n");
        if (!firstline)
        {
            wxString left_str = line.BeforeFirst(':');
            m_headers[left_str] = line.AfterFirst(':').Strip(wxString::both);
        }
        else
        {
            m_http_response = wxAtoi(line.Mid(9,1));
            m_http_complete_response = wxAtoi(line.Mid(9,3));
            firstline = FALSE;
        }
    }while (line != wxEmptyString);

    {//PUT IN THE QUEUE THE DATA THAT ISN'T PART OF THE MESSAGE
        char unreadbuf[8192];
        for (i = 0 ; i < (lastcount-count);i++)
            unreadbuf[i] = buf[i+count];
        wxHTTP::Unread(unreadbuf,i);
    }
    return true;
}

wxString mHTTP::GetResponseMessage()
{
    if (m_messagereceived.IsEmpty())
    {
        ParseHeaders();
    }
    return m_messagereceived;
};

char *mHTTP::wxstr2str(wxString wxstr)
{
    int tamanho = wxstr.Length() + 1;
    char *data = new char[tamanho];
    int i;
    for (i = 0 ; i < (tamanho-1); i++)
        data[i] = wxstr.GetChar(i);
    data[i] = '\0';
    return data;
}
