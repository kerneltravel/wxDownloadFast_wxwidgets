//////////////////////////////////////////////////////////////////////
//    Name:               wxSHA1.h
//    Purpose:            wxSHA1 Class
//    Author:             Max Velasques
//    Created:            03/03/2007
//    Last modified:      03/03/2007
//    Licence:            wxWindows license
//////////////////////////////////////////////////////////////////////

// wxSHA1.h: interface for the wxSHA1 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXSHA1_H__
#define _WXSHA1_H__

#ifdef __GNUG__
    #pragma interface "wxSHA1.h"
#endif

#include "wx/filename.h"
#include "wx/string.h"
#include "wx/file.h"

class wxSHA1
{
public:
    wxSHA1();
    wxSHA1(const wxString& szText);
    wxSHA1(const wxFileName& szfile);
    virtual ~wxSHA1();

    // Other Methods
    void SetText(const wxString& szText);
    void SetFile(const wxFileName& szfile);

    const wxString GetDigest(bool mainthread = FALSE);

    // Static Methods
    static const wxString GetDigest(const wxString& szText);
    static const wxString GetDigest(const wxFileName& szfile);

protected:
    bool        m_bCalculatedDigest;
    wxUint8     m_arrDigest[20];
    wxChar      m_pszDigestString[41];
    wxString    m_szText;
    bool        m_isfile;
    wxFileName  m_file;

private:
};

#endif // _WXSHA1_H__

