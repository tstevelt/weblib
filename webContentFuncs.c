/*----------------------------------------------------------------------------
	Program : webContentFuncs.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Output various Content-type lines to start CGI page
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

void webContentHtml ()
{
	static	int		Count = 0;

	Count++;

	if ( Count == 1 )
	{
		printf ( "Content-type: text/html\n\n" );
	}
}

void webContentMSExcel ()
{
	printf ( "Content-type: application/x-msexcel\n\n" );
}

void webContentMSWord ()
{
	printf ( "Content-type: application/msword\n\n" );
}

void webContentPDF ( WEBPARMS *ptrWebParms )
{
	if ( ptrWebParms->WP_FileName == (char *)0 )
	{
		fprintf ( stderr, "webContentPDF requires a file name\n" );
	}
	else
	{
		printf ( "Content-type: application/pdf\n" );
		printf ( "Content-Disposition:inline; filename=%s\n", ptrWebParms->WP_FileName );
		printf ( "\n" );
	}
}

void webContentPlainText ()
{
	printf ( "Content-type: text/plain\n\n" );
}

void webContentXML ()
{
	printf ( "Content-type: text/xml\n\n" );
}
