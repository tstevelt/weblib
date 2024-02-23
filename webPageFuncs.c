/*----------------------------------------------------------------------------
	Program : webPageFuncs.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Start and end an HTML page.
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

void webStartHead ( WEBPARMS *ptrWebParms )
{
	char	*CharacterEncoding;

	printf ( "<!DOCTYPE HTML>\n" );
	CharacterEncoding = "<meta charset='UTF-8'>";

	printf ( "<html lang='en-US'>\n" );
	printf ( "<head>\n" );
	if ( ptrWebParms->WP_Title != (char *)0 )
		printf ( "<title>%s</title>\n", ptrWebParms->WP_Title );
	else
		printf ( "<title>web page</title>\n" );

	printf ( "%s\n", CharacterEncoding );
}

void webHeadToBody ( WEBPARMS *ptrWebParms )
{
	printf ( "</head>\n" );

	printf ( "<body" );

	if ( ptrWebParms->WP_Class != (char *)0 )
	{
		if ( nsStrcmp ( ptrWebParms->WP_Class, "inherit" ) != 0 )
		{
			printf ( " class='%s'", ptrWebParms->WP_Class );
		}
	}
	else if ( ptrWebParms->WP_Color != (char *)0 )
	{
		printf ( " bgcolor='%s'", ptrWebParms->WP_Color );
	}
	else
	{
		printf ( " bgcolor='#ffffff'" );
	}

	if ( ptrWebParms->WP_OnLoad != (char *)0 )
	{
		printf ( " onload='%s'", ptrWebParms->WP_OnLoad );
	}
	else
	{
		printf ( " onload='javascript:window.focus();'" );
	}

	printf ( ">\n" );

}

void webEndPage ()
{
	printf ( "</body>\n" );
	printf ( "</html>\n" );
}
