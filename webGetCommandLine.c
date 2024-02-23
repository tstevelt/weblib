/*----------------------------------------------------------------------------
	Program : webGetCommandLine.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Read command line into webCommands array.
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

int webGetCommandLine ( int argc, char *argv[] )
{
	int	xa;

	webCommandCount = 0;
	
	for ( xa = 1; xa < argc; xa++ )
	{
		if ( webCommandCount >= MAXCOMMANDLINE )
			return ( 1 );

		webCommands[webCommandCount] = argv[xa];
		webCommandCount++;
	}

	return ( 0 );
}
