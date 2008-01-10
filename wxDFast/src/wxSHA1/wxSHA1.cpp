//////////////////////////////////////////////////////////////////////
//    Name:               wxSHA1.cpp
//    Purpose:            wxSHA1 Class
//    Author:             Max Velasques
//    Created:            03/03/2007
//    Last modified:      03/03/2007
//    Licence:            wxWindows license
//////////////////////////////////////////////////////////////////////

// wxSHA1.cpp: implementation of the wxSHA1 class.
//
//////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "wxSHA1.h"
#endif

// for compilers that support precompilation, includes "wx.h"
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others
#ifndef WX_PRECOMP
    #include "wx/wx.h"
    #include "wx/file.h"
#endif
#include "sha1.h"
#include "wxSHA1.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxSHA1::wxSHA1()
{
    m_bCalculatedDigest = false;
    m_pszDigestString[40] = '\0';
    m_isfile = FALSE;
}

wxSHA1::wxSHA1(const wxString& szText)
{
    m_bCalculatedDigest = false;
    m_pszDigestString[40] = '\0';
    m_szText = szText;
    m_isfile = FALSE;
}

wxSHA1::wxSHA1(const wxFileName& szfile)
{
    m_bCalculatedDigest = false;
    m_pszDigestString[40] = '\0';
    m_file = szfile;
    m_isfile = TRUE;
}

wxSHA1::~wxSHA1()
{
}

//////////////////////////////////////////////////////////////////////
// Other Methods
//////////////////////////////////////////////////////////////////////

void wxSHA1::SetText(const wxString& szText)
{
    m_bCalculatedDigest = false;
    m_szText = szText;
    m_isfile = FALSE;
}

void wxSHA1::SetFile(const wxFileName& szfile)
{
    m_bCalculatedDigest = false;
    m_file = szfile;
    m_isfile = TRUE;
}

const wxString wxSHA1::GetDigest(bool mainthread)
{
    if (m_isfile)
    {
        if(m_bCalculatedDigest)
        {
            const wxString szRetVal = m_pszDigestString;
            return szRetVal;
        }
        else if(!m_file.FileExists())
        {
            return wxEmptyString;
        }
        else
        {
            sha1_context sha1Context;
            sha1_starts(&sha1Context);
            wxFile sha1file(m_file.GetFullPath(), wxFile::read);
            unsigned char buffer[16384];
            unsigned int i = 1;
            while (i >0)
            {
                i = sha1file.Read(buffer,16384);
                sha1_update(&sha1Context, buffer, (unsigned) i);
                if (mainthread)
                    wxYield();
            }
            sha1_finish(&sha1Context,m_arrDigest);

            wxString szRetVal;
            unsigned int j=0;
            for (i = 0; i < sizeof m_arrDigest; i++)
            {
                szRetVal << wxString::Format(wxT("%02X"),m_arrDigest[i]);
                m_pszDigestString[j] = szRetVal.GetChar(j);
                m_pszDigestString[j+1] = szRetVal.GetChar(j+1);
                j+=2;
            }
            return szRetVal;
        }
    }
    else
    {
        if(m_bCalculatedDigest)
        {
            const wxString szRetVal = m_pszDigestString;
            return szRetVal;
        }
        else if(m_szText.IsEmpty())
        {
            return wxEmptyString;
        }
        else
        {
            sha1_context sha1Context;
            sha1_starts(&sha1Context);
            char *text = new char[m_szText.Len()+1];
            unsigned int i;
            for (i=0; i < (m_szText.Len());i++)
                text[i] = m_szText.GetChar(i);
            text[i] = '\0';

            sha1_update(&sha1Context, (unsigned char*)(text), strlen(text));
            sha1_finish(&sha1Context,m_arrDigest);

            wxString szRetVal;
            unsigned int j=0;
            for (i = 0; i < sizeof m_arrDigest; i++)
            {
                szRetVal << wxString::Format(wxT("%02X"),m_arrDigest[i]);
                m_pszDigestString[j] = szRetVal.GetChar(j);
                m_pszDigestString[j+1] = szRetVal.GetChar(j+1);
                j+=2;
            }
            return szRetVal;
        }
    }
}

//////////////////////////////////////////////////////////////////////
// Static Methods
//////////////////////////////////////////////////////////////////////

const wxString wxSHA1::GetDigest(const wxString& szText)
{
    wxSHA1 sha1(szText);

    return sha1.GetDigest();
}

const wxString wxSHA1::GetDigest(const wxFileName& szfile)
{
    wxSHA1 sha1(szfile);

    return sha1.GetDigest();
}
