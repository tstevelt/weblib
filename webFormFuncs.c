/*----------------------------------------------------------------------------
	Program : webFormFuncs.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Start and end form functions.
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

void webStartForm ( WEBPARMS *ptrWebParms )
{
	printf ( "<form" );

	if ( ptrWebParms->WP_FormName != (char *)0 )
	{
		printf ( " name='%s'", ptrWebParms->WP_FormName );
		printf ( " id='%s'", ptrWebParms->WP_FormName );
	}

	/*----------------------------------------------------------
		action may have embedded single quotes, so use
		double quotes here ...
	----------------------------------------------------------*/
	if ( ptrWebParms->WP_FormAction != (char *)0 )
		printf ( " action=\"%s\"", ptrWebParms->WP_FormAction );

	if ( ptrWebParms->WP_FormMethod != (char *)0 )
		printf ( " method='%s'", ptrWebParms->WP_FormMethod );

	if ( ptrWebParms->WP_FormOther != (char *)0 )
		printf ( " %s", ptrWebParms->WP_FormOther );

	if ( ptrWebParms->WP_FormEnctype != (char *)0 )
	{
		printf ( " enctype='%s'>\n", ptrWebParms->WP_FormEnctype );
	}
	else
	{
		printf ( " enctype='application/x-www-form-urlencoded'>\n" );
	}

}

void webEndForm ()
{
	printf ( "</form>\n" );
}
