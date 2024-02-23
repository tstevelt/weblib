/*----------------------------------------------------------------------------
	Program : webGetInput.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Get all input from various metthods and store in webValues 
				and webNames arrays.
	Return  : 

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
    Utility functions for writing CGI output

    Copyright (C)  2020 Tom Stevelt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
---------------------------------------------------------------------------*/

#include	"webprivate.h"
#include	"weblib.h"

int webGetInput ()
{
	int		xi;

	/*----------------------------------------------------------
		get standard in
	----------------------------------------------------------*/
	if ( webGetStdin () != 0 )
	{
		return ( 1 );
	}

	/*----------------------------------------------------------
		get query_string
	----------------------------------------------------------*/
	if ( webGetQuery () != 0 )
	{
		return ( 2 );
	}

	/*----------------------------------------------------------
		combine results
	----------------------------------------------------------*/
	webCount = 0;

	for ( xi = 0; xi < webStdinCount; xi++ )
	{
		if ( webCount >= MAXINTEGRATED )
		{
			return ( 1 );
		}
		if ( nsStrcmp ( webStdinNames [xi], "fbclid" ) == 0 )
		{
			continue;
		}
		webNames [webCount] = webStdinNames [xi];
		webValues[webCount] = webStdinValues[xi];
		webCount++;
	}

	for ( xi = 0; xi < webQryCount; xi++ )
	{
		if ( webCount >= MAXINTEGRATED )
		{
			return ( 3 );
		}
		if ( nsStrcmp ( webQryNames [xi], "fbclid" ) == 0 )
		{
			continue;
		}
		webNames [webCount] = webQryNames [xi];
		webValues[webCount] = webQryValues[xi];
		webCount++;
	}

	return ( 0 );
}
