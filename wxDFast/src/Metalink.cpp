//
// C++ Implementation: Metalink
//
// Description: Implements the Metalink Class
//
//
// Author: Max Magalhães Velasques <maxvelasques@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "wxDFast.h"

/*RETURN THE NUMBER OS FILE ENTRY ON THE METALINK FILE*/

int mMetalinkDocument::GetMetalinkData(mMetalinkData *data,int index)
{
    int result = 0;
    wxXmlNode *rootnode;
    if (IsOk())
    {
        rootnode = this->GetRoot();
        wxLogDebug(wxT("metalink tag"));
        if (rootnode->GetName().Lower() == wxT("metalink"))
        {
            wxXmlNode *node = rootnode->GetChildren();
            while (node)
            {
                if (node->GetName().Lower() == wxT("publisher"))
                {
                    wxLogDebug(wxT("publisher tag..."));
                    wxXmlNode *subnode = node->GetChildren();
                    while (subnode)
                    {
                        if (subnode->GetName().Lower() == wxT("name"))
                            data->publishername = this->GetContent(subnode->GetChildren());
                        else if (subnode->GetName().Lower() == wxT("url"))
                            data->publisherurl = this->GetContent(subnode->GetChildren());
                        subnode = subnode->GetNext();
                    }
                }
                else if (node->GetName().Lower() == wxT("description"))
                {
                    wxLogDebug(wxT("description tag..."));
                    data->description = this->GetContent(node->GetChildren());
                }
                else if (node->GetName().Lower() == wxT("files"))
                {
                    wxLogDebug(wxT("files tag..."));
                    wxXmlNode *subnode = node->GetChildren();
                    bool ok = false;
                    while (subnode)
                    {
                        if (subnode->GetName().Lower() == wxT("file"))
                        {
                            if (result==index)
                            {
                                wxLogDebug(wxT("Getting file tag..."));
                                if (this->GetFileData(data,subnode))
                                {
                                    ok = true;
                                    wxLogDebug(wxT("OK"));
                                }
                                else
                                    break;
                            }
                            result++;
                        }
                        subnode = subnode->GetNext();
                    }
                    if (!ok)
                    {
                        result = 0;
                        wxLogDebug(wxT("ERRO"));
                    }
                }

                node = node->GetNext();
            }

        }
    }
    return result;
}

int mMetalinkDocument::GetFileData(mMetalinkData *data,wxXmlNode *subnode)
{
    bool result = false;
	data->filename = subnode->GetPropVal(wxT("name"),wxT("")).Trim(TRUE).Trim(FALSE);
	wxXmlNode *subsubnode = subnode->GetChildren();
	while (subsubnode)
	{
        if (subsubnode->GetName().Lower() == wxT("version"))
            data->version = this->GetContent(subsubnode->GetChildren());
        else if (subsubnode->GetName().Lower() == wxT("size"))
            data->size = MyUtilFunctions::wxstrtolonglong(this->GetContent(subsubnode->GetChildren()));
        else if (subsubnode->GetName().Lower() == wxT("language"))
            data->language = this->GetContent(subsubnode->GetChildren());
        else if (subsubnode->GetName().Lower() == wxT("os"))
            data->os = this->GetContent(subsubnode->GetChildren());
        else if (subsubnode->GetName().Lower() == wxT("verification"))
        {
            wxXmlNode *subsubsubnode = subsubnode->GetChildren();
            while (subsubsubnode)
            {
                if (subsubsubnode->GetName().Lower() == wxT("hash"))
                {
                    if (subsubsubnode->GetPropVal(wxT("type"),wxEmptyString).Lower() == wxT("md5"))
                        data->md5 = this->GetContent(subsubsubnode->GetChildren());
                    else if (subsubsubnode->GetPropVal(wxT("type"),wxEmptyString).Lower() == wxT("sha1"))
                        data->sha1 = this->GetContent(subsubsubnode->GetChildren());
                }
                subsubsubnode = subsubsubnode->GetNext();
            }
        }
        else if (subsubnode->GetName().Lower() == wxT("resources"))
        {
            wxXmlNode *subsubsubnode = subsubnode->GetChildren();
            while (subsubsubnode)
            {
                if (subsubsubnode->GetName().Lower() == wxT("url"))
                {
                    mUrlName *urltmp = new mUrlName(this->GetContent(subsubsubnode->GetChildren()));
                    if ((urltmp->IsComplete()) && (!urltmp->GetFullRealName().Contains(wxT(".torrent"))) && (urltmp->Type() != -1))
                    {
                        data->urllist.Append(urltmp);
                        result = true;
                    }
                }
                subsubsubnode = subsubsubnode->GetNext();
            }
        }
        subsubnode = subsubnode->GetNext();
	}
	return result;
}

wxString mMetalinkDocument::GetContent(wxXmlNode *children)
{
    if (children)
        return children->GetContent().Trim(TRUE).Trim(FALSE);
    else
        return wxEmptyString;
}


void mMetalinkData::Clear()
{
    publishername.Clear();
    publisherurl.Clear();
    description.Clear();
    filename.Clear();
    version.Clear();
    size = 0;
    language.Clear();
    os.Clear();
    md5.Clear();
    sha1.Clear();
    urllist.Clear();
}

