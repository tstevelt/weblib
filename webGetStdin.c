/*----------------------------------------------------------------------------
	Program : webGetStdin.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Get stdin variables into webStdinValues array.
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

#include	<limits.h>
#include	"webprivate.h"
#include	"weblib.h"

int webGetStdin ()
{
	int	xa, xi, xo;

	webStdinCount = 0;

	xa = fread ( StdinBuffer, 1, MAXCHARS, stdin );

	if ( xa >= MAXCHARS )
	{
		printf (          "webGetStdin: read MAXCHARS, probably missing some!<br>\n");
		fprintf ( stderr, "webGetStdin: read MAXCHARS, probably missing some!<br>\n");
	}

	if ( xa )
	{
		xi = 0;
		xo = 0;

		for ( xi = 0; xi < xa; xi++ )
		{
			if ( StdinBuffer[xi] == '=' )
			{
				webStdinNames[webStdinCount] = &StdinBuffer[xo];
				StdinBuffer[xi] = '\0';
				xo = xi + 1;
				if ( webStdinCount >= MAXSTDIN - 1 )
				{
					xi = xa;
					break;
				}
			}
			else if ( StdinBuffer[xi] == '&' )
			{
				webStdinValues[webStdinCount] = &StdinBuffer[xo];
				webTrim ( webStdinValues[webStdinCount], 0 );
				StdinBuffer[xi] = '\0';
				xo = xi + 1;
				webStdinCount++;
			}
			else if ( StdinBuffer[xi] == '%' )
			{
			}
		}

		webStdinValues[webStdinCount] = &StdinBuffer[xo];
		webTrim ( webStdinValues[webStdinCount], 0 );
		StdinBuffer[xi] = '\0';
		webStdinCount++;
		if ( webStdinCount >= MAXSTDIN )
			return ( 1 );
	}

	return ( 0 );
}
