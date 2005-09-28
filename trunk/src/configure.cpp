#include "configure.h"

#include "fx.h"


#ifdef WIN32
#define SEPARATOR "\\"
#else
#include <dlfcn.h>
#define SEPARATOR "/"
#endif

#ifndef PATH_CFG
#define PATH_CFG        "/usr/local/etc/"
#endif

configure *conf;

configure::configure ()
{

    doc=NULL;
}

configure::configure (configure & conf)
{

    doc = conf.doc;
    copy = true;
}


configure::~configure ()
{
    if (!copy)
    {
	FXTRACE ((5, "CONFIGURATION SAVING\n"));

	
	if(doc)
	{
	xmlSaveFormatFileEnc(file.c_str(), doc, "UTF-8", 1);
	string cmd="xmllint --format " +file+ " -o "+file;
	system(cmd.c_str());
	xmlFreeDoc (doc);
	xmlCleanupParser ();
	}

    }

}

bool configure::loadconfig(bool restore)
{

string dir = FXFile::getUserDirectory ("").text ();
    file = dir +  "/.openspace/openspacerc";
    
    if (!FXFile::exists (file.c_str ()))
    {
	file = "";
	file = file + PATH_CFG + SEPARATOR + "openspacerc";
    }
    FXTRACE ((5, "OPENING CONFIGRATION FILE %s\n", file.c_str ()));

    doc = xmlParseFile (file.c_str ());
    copy = false;
    
    

	if(!doc)
	{	
		if(restore)
		{
		fxmessage("restore");
		doc = xmlParseFile (string(file+".backup").c_str());
			if(doc)
			return true;
		}
	return false;
	}
	else
	{
	FXFile::copy(file.c_str(),string(file+".backup").c_str(),true);
	return true;
	}

}



bool configure::readonestring (string path,string &ret)
{


    xmlChar *xpath = (xmlChar *) path.c_str ();
    xmlChar *keyword;
    string retstring;


    xmlNodeSetPtr nodeset;
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;

    context = xmlXPathNewContext (doc);
    if (context == NULL)
	return false;

    result = xmlXPathEvalExpression (xpath, context);
    xmlXPathFreeContext (context);
    if (result == NULL)
	return false;


    if (xmlXPathNodeSetIsEmpty (result->nodesetval))
    {
	return false;
    }


    nodeset = result->nodesetval;
    keyword = xmlNodeListGetString (doc, nodeset->nodeTab[0]->xmlChildrenNode, 1);
    if(keyword!=NULL)
    retstring = string ((char *) keyword);
    xmlFree (keyword);




    xmlXPathFreeObject (result);
    ret=retstring;
    return true;

}

string configure::readonestring (string path)
{

    xmlChar *xpath = (xmlChar *) path.c_str ();
    xmlChar *keyword;
    string retstring;


    xmlNodeSetPtr nodeset;
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;

    context = xmlXPathNewContext (doc);
    if (context == NULL)
	return "";

    result = xmlXPathEvalExpression (xpath, context);
    xmlXPathFreeContext (context);
    if (result == NULL)
	return "";


    if (xmlXPathNodeSetIsEmpty (result->nodesetval))
    {
	return "";
    }


    nodeset = result->nodesetval;
    keyword = xmlNodeListGetString (doc, nodeset->nodeTab[0]->xmlChildrenNode, 1);
    if(keyword!=NULL)
    retstring = string ((char *) keyword);
    xmlFree (keyword);




    xmlXPathFreeObject (result);

    return retstring;

}


bool configure::removestring (string path)
{

    xmlChar *xpath = (xmlChar *) path.c_str ();
    xmlChar *keyword;
    string retstring;


    xmlNodeSetPtr nodeset;
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;

    context = xmlXPathNewContext (doc);
    if (context == NULL)
	return false;

    result = xmlXPathEvalExpression (xpath, context);
    xmlXPathFreeContext (context);
    if (result == NULL)
	return false;


    if (xmlXPathNodeSetIsEmpty (result->nodesetval))
    {
	return false;
    }


    nodeset = result->nodesetval;
    /*keyword = xmlNodeListGetString(doc, nodeset->nodeTab[0]->xmlChildrenNode, 1);
       retstring=string((char*)keyword);
       xmlFree(keyword);
     */
    //delete nodeset->nodeTab[0]; 
    //nodeset->nodeTab[0] = NULL;

    //xmlXPathFreeNodeSet(nodeset->nodeTab[0]);

    xmlUnlinkNode (nodeset->nodeTab[0]);
    xmlFreeNode (nodeset->nodeTab[0]);





    xmlXPathFreeObject (result);

    return true;

}


bool configure::saveonestring (string path, string value)
{

    xmlChar *xpath = (xmlChar *) path.c_str ();
    xmlChar *keyword;
    string retstring;


    xmlNodeSetPtr nodeset;
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;

    context = xmlXPathNewContext (doc);
    if (context == NULL)
	return "";

    result = xmlXPathEvalExpression (xpath, context);
    xmlXPathFreeContext (context);
    if (result == NULL)
	return false;


    if (xmlXPathNodeSetIsEmpty (result->nodesetval))
    {
	return false;
    }


    nodeset = result->nodesetval;




    int size;
    int i;
    size = (nodeset) ? nodeset->nodeNr : 0;


    for (i = size - 1; i >= 0; i--)
    {

	xmlNodeSetContent (nodeset->nodeTab[i], (xmlChar *) value.c_str ());



	if (nodeset->nodeTab[i]->type != XML_NAMESPACE_DECL)
	    nodeset->nodeTab[i] = NULL;
    }





    xmlXPathFreeObject (result);

    return true;

}


bool configure::addstring (string path, string node, string value)
{

    xmlChar *xpath = (xmlChar *) path.c_str ();
    xmlChar *keyword;
    string retstring;


    xmlNodeSetPtr nodeset;
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;

    context = xmlXPathNewContext (doc);
    if (context == NULL)
	return false;

    result = xmlXPathEvalExpression (xpath, context);
    xmlXPathFreeContext (context);
    if (result == NULL)
	return false;


    if (xmlXPathNodeSetIsEmpty (result->nodesetval))
    {
	return false;
    }


    nodeset = result->nodesetval;

   xmlNewChild (nodeset->nodeTab[0], NULL, (xmlChar *) node.c_str (), (xmlChar *) value.c_str ());




    xmlXPathFreeObject (result);

    return true;

}



int configure::openxpath (string path)
{

    xmlChar *xpath = (xmlChar *) path.c_str ();
    xmlChar *keyword;
    string retstring;


    xmlXPathContextPtr context;

    context = xmlXPathNewContext (doc);
    if (context == NULL)
	return -1;

    result = xmlXPathEvalExpression (xpath, context);
    xmlXPathFreeContext (context);
    if (result == NULL)
	return -1;


    if (xmlXPathNodeSetIsEmpty (result->nodesetval))
    {
	return -1;
    }
    nodeset = result->nodesetval;

    cur = nodeset->nodeTab[0]->xmlChildrenNode;
    pos = 0;
    return 0;

}

bool configure::getnextnode (string &ret)
{

    string retstring;
    if (cur)
    {
	if (cur->next)
	{
	    retstring = string ((char *) cur->next->name);
	    
	}
	else
	    return false;
    }
    else
	return false;

    if(cur->next!=NULL)
    cur = cur->next->next;
    else
    return false;

    ret=retstring;	
    return true;



}


bool configure::getnextstring (string &ret)
{


    xmlChar *keyword;

    if (nodeset->nodeNr == pos)
    {
	xmlXPathFreeObject (result);
	return false;
    }



    keyword = xmlNodeListGetString (doc, nodeset->nodeTab[pos]->xmlChildrenNode, 1);
    string retstring = string ((char *) keyword);

    xmlFree (keyword);
    pos++;
    ret=retstring;
    return true;



}

int configure::countxpath (string path)
{
    int counter = 0;
    openxpath (path);
    string res;
    while (getnextstring (res))
    {
	counter++;
    }
    return counter;
}

