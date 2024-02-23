/*----------------------------------------------------------------------------
	Program : webGetQuery.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Get QUERY_STRING values into webQryValues array.
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

int webGetQuery ()
{
	int	xa, xi, xo;
	int lookfornewline;
	char	*cp;
	
	webQryCount = 0;

	if (( cp = webGetOneEnv ( "QUERY_STRING" )) == (char *)0 )
		return ( 1 );

	xa = snprintf ( qrybuf, sizeof(qrybuf), "%s", cp );

	if ( xa > MAXCHARS )
	{
		fprintf ( stderr, "webGetQuery: exceeded MAXCHARS\n" );
		exit ( 1 );	
	}

	if ( xa )
	{
		xi = 0;
		xo = 0;
		lookfornewline = 0;

		for ( xi = 0; xi < xa; xi++ )
		{
			if ( lookfornewline == 0 && qrybuf[xi] == '=' )
			{
				lookfornewline = 1;
				webQryNames[webQryCount] = &qrybuf[xo];
				qrybuf[xi] = '\0';
				xo = xi + 1;
				if ( webQryCount >= MAXENV - 1 )
				{
					xi = xa;
					break;
				}
			}
			else if (( qrybuf[xi] == 10 ) || (  qrybuf[xi] == '&' ))
			{
				lookfornewline = 0;
				webQryValues[webQryCount] = &qrybuf[xo];
				qrybuf[xi] = '\0';
				xo = xi + 1;
				webQryCount++;
			}
		}

		webQryValues[webQryCount] = &qrybuf[xo];
		qrybuf[xi] = '\0';
		webQryCount++;

		if ( webQryCount >= MAXENV )
			return ( 2 );
	}

	return ( 0 );
}

