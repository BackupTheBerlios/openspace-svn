// $Id$
/////////////////////////////////////////////////////////////////////////////
// This file is part of OpenSpace project.
//
// OpenSpace is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// OpenSpace is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenSpace; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
/////////////////////////////////////////////////////////////////////////////
//
// C++ Interface: OSUtils
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSUTILS_H__
#define __OSUTILS_H__

/**
 * \file OSUtils.h
 * OpenSpace utilities functions
 * 
 * @todo Why not to implement them as C++ templates: numtostring<int|short|double> ?
 */

#include <string>

/**
 * Convert numeric type to string.
 * @param k - number to convert.
 * @return String value of number.
 */
std::string ntos ( unsigned long k );

/**
 * Convert numeric type to string.
 * @param k - number to convert.
 * @return String value of number.
 */
std::string numtostring ( unsigned long k );

/**
 * Mime string to XML string.
 * @param str - string with mime content.
 * @return XML encoded string.
 */
std::string mime2xml( std::string str );

/**
 * XML encoded string to mime type.
 * @param str - XML encoded string.
 * @return String with mime content.
 */
std::string xml2mime( std::string str );

#endif // #ifndef __OSUTILS_H__

