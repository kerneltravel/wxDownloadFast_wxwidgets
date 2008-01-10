//
// C++ Implementation: UtilFunctions
//
// Description: This file contain the implementation of some usefull conversion
//              functions.
//
//
// Author: Max Magalh�s Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"

wxString MyUtilFunctions::int2wxstr(long value,int format)
{
    wxString temp,string;
    string << wxT("%0") << format << wxT("ld");
    temp = temp.Format(string,value);
    return temp;
}

wxString MyUtilFunctions::TimeString(long value)
{
    wxString tmp;
    int hour,min,sec;
    long time;
    time = (long) (value / 1000);
    hour = (int) (time/3600);
    time = time - (hour * 3600);
    min = (int) (time/60);
    sec = (time%60);
    if (hour == 0)
       tmp.Printf(wxT("%01dm %01ds"), min, sec);
    else
       tmp.Printf(wxT("%01dh %01dm %01ds"),hour, min, sec);
    return(tmp);
}

wxString MyUtilFunctions::TimeString(wxLongLong value)
{
    wxString tmp;
    int hour,min,sec;
    long time;
    time = (value / 1000).ToLong();
    hour = (int) (time/3600);
    time = time - (hour * 3600);
    min = (int) (time/60);
    sec = (time%60);
    if (hour == 0)
       tmp.Printf(wxT("%01dm %01ds"), min, sec);
    else
       tmp.Printf(wxT("%01dh %01dm %01ds"),hour, min, sec);
    return(tmp);
}

wxString MyUtilFunctions::GetLine(wxString text, int line)
{
    wxString str = text;
    int pos = str.Find(wxT("\n"));
    int i;
    if (pos == 0)
    {
        str=str.Mid(1);
        pos = str.Find(wxT("\n"));
    }
    for (i=1; i<line;i++)
    {
        str = str.Mid(pos+1);
        pos = str.Find(wxT("\n"));
    }
    return str.Mid(0,pos-1);
}

wxString MyUtilFunctions::str2wxstr(char *str)
{
    wxString retorno;
    unsigned int i;
    for (i = 0 ; i < strlen(str); i++)
        retorno.Append(str[i],1);
    return retorno;
}

wxString MyUtilFunctions::str2wxstr(char str)
{
    wxString retorno;
    retorno.Append(str,1);
    return retorno;
}

wxString MyUtilFunctions::ByteString(long size)
{
    wxString str;
    if ( size < 1024)
//        str.Printf(wxT("%0.1f Bytes"),(double) size);
        str.Printf(wxT("0.0 kB"));
    if (( size >= 1024) && (size < 1048576))
        str.Printf(wxT("%0.1f kB"),((double) size) /((double)1024));
    if ( size >= 1048576)
        str.Printf(wxT("%0.1f mB"),((double) size)/((double)1048576));
    return(str);
}

wxString MyUtilFunctions::ByteString(wxLongLong size)
{
    wxString str;
    if ( size < 1024)
//        str.Printf(wxT("%0.1f Bytes"),(double) size);
        str.Printf(wxT("0.0 kB"));
    if (( size >= 1024) && (size < 1048576))
        str.Printf(wxT("%0.1f kB"),( wxlonglongtodouble(size)) /((double)1024));
    if ( size >= 1048576)
        str.Printf(wxT("%0.1f MB"),( wxlonglongtodouble(size))/((double)1048576));
    return(str);
}

wxLongLong MyUtilFunctions::wxstrtolonglong(wxString string)
{
    wxString tmp = string.Trim().Trim(FALSE);
    char carac;
    wxLongLong result = 0;
    int sign=1;
    if (string.Length() > 0)
    {
        if (tmp.GetChar(0) == '-')
            sign = -1;
        for (unsigned int i = 0; i < tmp.Length(); i++)
        {
            carac = tmp.GetChar(i);
            if ((carac >= '0') && (carac <= '9'))
                result = result * 10LL + carac-'0';
            else
                continue;
        }
    }
    return result*sign;
}

double MyUtilFunctions::wxlonglongtodouble(wxLongLong value)
{
    double d = value.GetHi();
    d *= 1.0 + (double)ULONG_MAX;
    d += value.GetLo();
    return d;
}

wxString MyUtilFunctions::GenerateAuthString(wxString user, wxString pass)
{
    static const char *base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    wxString buf;
    wxString toencode;

    buf.Printf(wxT("Basic "));

    toencode.Printf(wxT("%s:%s"),user.c_str(),pass.c_str());

    size_t len = toencode.length();
    const wxChar *from = toencode.c_str();
    while (len >= 3) { // encode full blocks first
        buf << wxString::Format(wxT("%c%c"), base64[(from[0] >> 2) & 0x3f], base64[((from[0] << 4) & 0x30) | ((from[1] >> 4) & 0xf)]);
        buf << wxString::Format(wxT("%c%c"), base64[((from[1] << 2) & 0x3c) | ((from[2] >> 6) & 0x3)], base64[from[2] & 0x3f]);
        from += 3;
        len -= 3;
    }
    if (len > 0) { // pad the remaining characters
        buf << wxString::Format(wxT("%c"), base64[(from[0] >> 2) & 0x3f]);
        if (len == 1) {
            buf << wxString::Format(wxT("%c="), base64[(from[0] << 4) & 0x30]);
        } else {
            buf << wxString::Format(wxT("%c%c"), base64[(from[0] << 4) & 0x30] + ((from[1] >> 4) & 0xf), base64[(from[1] << 2) & 0x3c]);
        }
        buf << wxString::Format(wxT("="));
    }

    return buf;
}

#ifdef __WXMSW__
wxString MyUtilFunctions::GetProgramFilesDir()
{
    wxString result = wxEmptyString;
    wxRegKey regKey;
    wxString idName(wxT("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion"));

    regKey.SetName(idName);
    regKey.QueryValue(wxT("ProgramFilesDir"),result);
    regKey.Close();

    return result;
}

wxString MyUtilFunctions::GetMyDocumentsDir()
{
    wxString result = wxEmptyString;
    wxRegKey regKey;
    wxString idName(wxT("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"));

    regKey.SetName(idName);
    regKey.QueryValue(wxT("Personal"),result);
    regKey.Close();

    return result;
}

wxString MyUtilFunctions::GetDefaultBrowser()
{
    wxString result = wxEmptyString;
    wxRegKey regKey;
    wxString idName(wxT("HKEY_CLASSES_ROOT\\http\\shell\\open\\command"));

    regKey.SetName(idName);
    regKey.QueryValue(wxEmptyString,result);
    regKey.Close();
    if (!result.IsEmpty())
    {
        result.Replace(wxT("\"%1\""),wxEmptyString);
        result.Replace(wxT("%1"),wxEmptyString);
    }

    return result;
}

#endif
