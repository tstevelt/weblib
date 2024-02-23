/*----------------------------------------------------------------------------
	Program : webFixHex.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: progname
	Return  : 

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		unknown		replace %40 with @, %27 with ', etc.
	tms		unknown		also replace + with space

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

void webFixHex ( char *SomeString )
{
	int		xl, xi, xo;
	int		base10val;
	char	Hex1, Hex2;

	xl = nsStrlen ( SomeString );
	
	for ( xi = 0; xi < xl; xi++ )
	{
		if ( SomeString[xi] == '+' )
		{
			SomeString[xi] = ' ';
		}
		else if ( SomeString[xi] == '%' )
		{
			Hex1 = SomeString[xi+1];
			Hex2 = SomeString[xi+2];
			if ( nsStrchr ( "0123456789ABCDEF", Hex1 ) != (char *)0  &&
			     nsStrchr ( "0123456789ABCDEF", Hex2 ) != (char *)0 )
			{
				if ( nsStrchr ( "0123456789", Hex1 ) != (char *)0 )
					base10val = (Hex1 -  '0') * 16;
				else
					base10val = (10 + Hex1 -  'A') * 16;

				if ( nsStrchr ( "0123456789", Hex2 ) != (char *)0 )
					base10val += (Hex2 -  '0');
				else
					base10val += (10 + Hex2 -  'A');

				SomeString[xi] = (char) base10val;
				for ( xo = xi + 1; xo < xl - 2; xo++ )
				{
					SomeString[xo] = SomeString[xo+2];
				}
				xl -= 2;
				SomeString[xl] = '\0';
			}
		}
	}
}
