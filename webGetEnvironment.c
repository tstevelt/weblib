/*----------------------------------------------------------------------------
	Program : webGetEnvironment.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Get environmental variables into webEnvValues array.
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

int webGetEnvironment ( char *envp[] )
{
	int		xi, xl;
	char	*cp;
	
	for ( webEnvCount = 0; ; webEnvCount++ )
	{
		if ( envp[webEnvCount]    == (char *)0 )
			break;
		if ( envp[webEnvCount][0] == '\0' )
			break;

		cp = envp[webEnvCount];
		xl = nsStrlen ( cp );

		webEnvNames[webEnvCount]  = cp;

		for ( xi = 0; xi < xl; xi++, cp++ )
		{
			if ( *cp == '=' )
			{
				*cp = '\0';
				cp++;
				webEnvValues[webEnvCount] = cp;
				break;
			}
		}
	}

	return ( 0 );
}
