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
// C++ Interface: OSConfigure
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSCONFIGURE_H__
#define __OSCONFIGURE_H__

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <string>

//read and save to XML config file
/**
 * nao file manager configuration manager class.
 * Read and save XML based configuration file.
 * 
 * @todo Shouldn't it be implemented using Singleton design pattern?
 * @todo Should we change names of functions to something more readable?
 */
class OSConfigure
{
public:

    /**
     * Default constructor.
     */
    OSConfigure( void );

    /**
     * Copy constructor.
     * @param config - existing instance of OSConfigure class.
     */
    OSConfigure( OSConfigure & config );
    
    /**
     * Destructor.
     */
    ~OSConfigure( void );

    bool loadconfig( bool restore = false );
    std::string readonestring ( std::string path );
    bool readonestring ( std::string path, std::string &ret );
    bool saveonestring ( std::string path, std::string value );
    bool addstring ( std::string path, std::string node, std::string value );
    int openxpath ( std::string path );
    bool getnextstring ( std::string &ret );
    bool  getnextnode ( std::string &ret );
    int  countxpath ( std::string path );
    bool removestring ( std::string path );

private:
    
    /**
     * Pointer to XML document instance.
     */
    xmlDocPtr doc;
    
    xmlXPathObjectPtr result;
    xmlNodeSetPtr nodeset;
    xmlNodePtr cur;
    
    /**
     * Configuration file path.
     */
    std::string file;
    
    int pos;
    
    bool copy;

};

#endif // #ifndef __OSCONFIGURE_H__
