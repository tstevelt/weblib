/*----------------------------------------------------------------------------
	Program : webJaavFuncs.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Java functions
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

void webStartJava ()
{
	printf ( "\n<script language='JavaScript1.2' type='text/javascript'>\n" );
	printf ( "<!-- hide code from non-js browsers\n\n" );
}

void webEndJava ()
{
	printf ( "\n// end hiding -->\n\n</script>\n\n" );
}

void webIncludeJava ( char *JavaScriptFile, char *Version )
{
	if ( nsStrlen ( Version ) > 0 )
	{
		printf ( "<script language='JavaScript1.2' src='%s?version=%s'> </script>\n", JavaScriptFile, Version );
	}
	else
	{
		printf ( "<script language='JavaScript1.2' src='%s'> </script>\n", JavaScriptFile );
	}
}
