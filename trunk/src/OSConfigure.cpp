// $Id$
/////////////////////////////////////////////////////////////////////////////
// This file is part of nao file manager project.
//
// nao file manager is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// nao file manager is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with nao file manager; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
/////////////////////////////////////////////////////////////////////////////
//
// C++ Implementation: OSConfigure
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////

#include "OSConfigure.h"
#include <fx.h>
#include <string>

#ifdef WIN32
#define SEPARATOR "\\"
#else
#include <dlfcn.h>
#define SEPARATOR "/"
#endif

#ifndef PATH_CFG
#define PATH_CFG        "PREFIX/etc"
#endif

OSConfigure* conf;

OSConfigure::OSConfigure()
{

    doc = NULL;
}

OSConfigure::OSConfigure( OSConfigure & conf )
{

    doc = conf.doc;
    copy = true;
}

OSConfigure::~OSConfigure()
{
    if ( !copy )
    {
        FXTRACE( ( 5, "CONFIGURATION SAVING\n" ) );


        if ( doc )
        {
            xmlSaveFormatFileEnc( file.c_str(), doc, "UTF-8", 1 );
            std::string cmd = "xmllint --format " + file + " -o " + file;
            system( cmd.c_str() );
            xmlFreeDoc ( doc );
            xmlCleanupParser ();
        }
    }
}

bool OSConfigure::loadconfig( bool restore )
{

    std::string dir = FXFile::getUserDirectory ( "" ).text ();
    file = dir + "/.nao/nao.conf";

    if ( !FXFile::exists ( file.c_str () ) )
    {
        file = "";
        file = file + PATH_CFG + SEPARATOR + "nao.conf";
    }
    FXTRACE ( ( 5, "OPENING CONFIGRATION FILE %s\n", file.c_str () ) );

    doc = xmlParseFile ( file.c_str () );
    copy = false;



    if ( !doc )
    {
        if ( restore )
        {
            fxmessage( "restore" );
            doc = xmlParseFile ( std::string( file + ".backup" ).c_str() );
            if ( doc )
                return true;
        }
        return false;
    }
    else
    {
        FXFile::copy( file.c_str(), std::string( file + ".backup" ).c_str(), true );
        return true;
    }

}

bool OSConfigure::readonestring ( std::string path, std::string &ret )
{
    xmlChar * xpath = ( xmlChar * ) path.c_str ();
    xmlChar *keyword;
    std::string retstring;


    xmlNodeSetPtr nodeset;
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;

    context = xmlXPathNewContext ( doc );
    if ( context == NULL )
        return false;

    result = xmlXPathEvalExpression ( xpath, context );
    xmlXPathFreeContext ( context );
    if ( result == NULL )
        return false;


    if ( xmlXPathNodeSetIsEmpty ( result->nodesetval ) )
    {
        return false;
    }


    nodeset = result->nodesetval;
    keyword = xmlNodeListGetString ( doc, nodeset->nodeTab[ 0 ] ->xmlChildrenNode, 1 );
    if ( keyword != NULL )
        retstring = std::string ( ( char * ) keyword );
    xmlFree ( keyword );

    xmlXPathFreeObject ( result );
    ret = retstring;
    return true;

}

std::string OSConfigure::readonestring ( std::string path )
{
    xmlChar * xpath = ( xmlChar * ) path.c_str ();
    xmlChar *keyword;
    std::string retstring;


    xmlNodeSetPtr nodeset;
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;

    context = xmlXPathNewContext ( doc );
    if ( context == NULL )
        return "";

    result = xmlXPathEvalExpression ( xpath, context );
    xmlXPathFreeContext ( context );
    if ( result == NULL )
        return "";


    if ( xmlXPathNodeSetIsEmpty ( result->nodesetval ) )
    {
        return "";
    }

    nodeset = result->nodesetval;
    keyword = xmlNodeListGetString ( doc, nodeset->nodeTab[ 0 ] ->xmlChildrenNode, 1 );
    if ( keyword != NULL )
        retstring = std::string ( ( char * ) keyword );
    xmlFree ( keyword );

    xmlXPathFreeObject ( result );

    return retstring;

}


bool OSConfigure::removestring( std::string path )
{
    xmlChar * xpath = ( xmlChar * ) path.c_str ();
    xmlChar *keyword;
    std::string retstring;

    xmlNodeSetPtr nodeset;
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;

    context = xmlXPathNewContext ( doc );
    if ( context == NULL )
        return false;

    result = xmlXPathEvalExpression ( xpath, context );
    xmlXPathFreeContext ( context );
    if ( result == NULL )
        return false;

    if ( xmlXPathNodeSetIsEmpty ( result->nodesetval ) )
    {
        return false;
    }

    nodeset = result->nodesetval;
    /*keyword = xmlNodeListGetString(doc, nodeset->nodeTab[0]->xmlChildrenNode, 1);
       retstring=std::string((char*)keyword);
       xmlFree(keyword);
     */ 
    //delete nodeset->nodeTab[0];
    //nodeset->nodeTab[0] = NULL;

    //xmlXPathFreeNodeSet(nodeset->nodeTab[0]);

    xmlUnlinkNode ( nodeset->nodeTab[ 0 ] );
    xmlFreeNode ( nodeset->nodeTab[ 0 ] );

    xmlXPathFreeObject ( result );

    return true;
}

bool OSConfigure::saveonestring( std::string path, std::string value )
{
    xmlChar * xpath = ( xmlChar * ) path.c_str ();
    xmlChar *keyword;
    std::string retstring;


    xmlNodeSetPtr nodeset;
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;

    context = xmlXPathNewContext ( doc );
    if ( context == NULL )
        return "";

    result = xmlXPathEvalExpression ( xpath, context );
    xmlXPathFreeContext ( context );
    if ( result == NULL )
        return false;

    if ( xmlXPathNodeSetIsEmpty ( result->nodesetval ) )
    {
        return false;
    }

    nodeset = result->nodesetval;

    int size;
    int i;
    size = ( nodeset ) ? nodeset->nodeNr : 0;

    for ( i = size - 1; i >= 0; i-- )
    {
        xmlNodeSetContent ( nodeset->nodeTab[ i ], ( xmlChar * ) value.c_str () );

        if ( nodeset->nodeTab[ i ] ->type != XML_NAMESPACE_DECL )
        {
            nodeset->nodeTab[ i ] = NULL;
        }
    }

    xmlXPathFreeObject ( result );

    return true;
}

bool OSConfigure::addstring ( std::string path, std::string node, std::string value )
{
    xmlChar * xpath = ( xmlChar * ) path.c_str ();
    xmlChar *keyword;
    std::string retstring;


    xmlNodeSetPtr nodeset;
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;

    context = xmlXPathNewContext ( doc );
    if ( context == NULL )
        return false;

    result = xmlXPathEvalExpression ( xpath, context );
    xmlXPathFreeContext ( context );
    if ( result == NULL )
        return false;


    if ( xmlXPathNodeSetIsEmpty ( result->nodesetval ) )
    {
        return false;
    }

    nodeset = result->nodesetval;

    xmlNewChild ( nodeset->nodeTab[ 0 ], NULL, ( xmlChar * ) node.c_str (), ( xmlChar * ) value.c_str () );

    xmlXPathFreeObject ( result );

    return true;

}

int OSConfigure::openxpath ( std::string path )
{

    xmlChar * xpath = ( xmlChar * ) path.c_str ();
    xmlChar *keyword;
    std::string retstring;

    xmlXPathContextPtr context;

    context = xmlXPathNewContext ( doc );
    if ( context == NULL )
        return -1;

    result = xmlXPathEvalExpression ( xpath, context );
    xmlXPathFreeContext ( context );
    if ( result == NULL )
        return -1;


    if ( xmlXPathNodeSetIsEmpty ( result->nodesetval ) )
    {
        return -1;
    }
    nodeset = result->nodesetval;

    cur = nodeset->nodeTab[ 0 ] ->xmlChildrenNode;
    pos = 0;
    return 0;

}

bool OSConfigure::getnextnode ( std::string &ret )
{

    std::string retstring;
    if ( cur )
    {
        if ( cur->next )
        {
            retstring = std::string ( ( char * ) cur->next->name );

        }
        else
            return false;
    }
    else
        return false;

    if ( cur->next != NULL )
        cur = cur->next->next;
    else
        return false;

    ret = retstring;
    return true;
}

bool OSConfigure::getnextstring ( std::string &ret )
{
    xmlChar* keyword = NULL;

if(pos < nodeset->nodeNr)
{

    if ( nodeset->nodeNr == pos )
    {
        xmlXPathFreeObject ( result );
        return false;
    }

    keyword = xmlNodeListGetString ( doc, nodeset->nodeTab[ pos ] ->xmlChildrenNode, 1 );
    std::string retstring = std::string ( ( char * ) keyword );

    xmlFree ( keyword );
    pos++;
    ret = retstring;
    return true;
}
else 
{
xmlXPathFreeObject ( result );
return false;
}
}

int OSConfigure::countxpath ( std::string path )
{
    int counter = 0;
    openxpath ( path );
    std::string res;
    while ( getnextstring ( res ) )
    {
        counter++;
    }
    return counter;
}
