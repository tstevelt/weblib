/*----------------------------------------------------------------------------
	Program : webGetBrowser.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Jan 6, 2003
	Synopsis: Get browser info and set capabilities.
	Return  : 

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		01/02/2005	revised and added to cgilib.

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

#include        "webprivate.h"
#include        "weblib.h"

static	BROWSER_INFO	BrowserInfo;

char *BrowserName ( int BrowserCode )
{
	static	char	*rv;

	switch ( BrowserCode )
	{
		case BROWSER_FIREFOX: rv = "Firefox"; break;
		case BROWSER_CHROME: rv = "Chrome"; break;
		case BROWSER_IE: rv = "Internet Explorer"; break;
		case BROWSER_NS: rv = "Netscape"; break;
		case BROWSER_LINKS: rv = "Links"; break;
		case BROWSER_LYNX: rv = "Lynx"; break;
		case BROWSER_OPERA: rv = "Opera"; break;
		case BROWSER_KONQUEROR: rv = "Konqueror"; break;
		case BROWSER_MOZILLA: rv = "Mozilla"; break;
		case BROWSER_WAP: rv = "WAP"; break;
		case BROWSER_UNK:
		default:
			rv = "Unknown"; break;
	}

	return ( rv );
}

BROWSER_INFO *webGetBrowser (  int Debug )
{
	char    *cp, *cp2;
	int		xo, Part;
	char	TempStr[256];

	/*----------------------------------------------------------
		initialize to minimums ...
	----------------------------------------------------------*/
	BrowserInfo.BrowserName = BROWSER_UNK;
	BrowserInfo.BrowserVersionMajor = 0;
	BrowserInfo.BrowserVersionMinor = 0;
	BrowserInfo.BrowserVersionRevision = 0;
	BrowserInfo.AccessKey = 0;
	BrowserInfo.ClickCell = 0;
	BrowserInfo.DisableButton = 0;
	BrowserInfo.CardSuits = 0;

	/*----------------------------------------------------------
		get HTTP_ACCEPT string
	----------------------------------------------------------*/
	if (( cp = getenv ( "HTTP_ACCEPT" )) != (char *)0 )
	{
		if ( nsStrstr ( cp, "wap" ) != (char *)0 )
		{
			BrowserInfo.BrowserName = BROWSER_WAP;
		}
		else if (( cp = getenv ( "HTTP_X_WAP_PROFILE" )) != (char *)0 )
		{
			BrowserInfo.BrowserName = BROWSER_WAP;
		}
	}

	if ( BrowserInfo.BrowserName == BROWSER_WAP )
	{
		return ( &BrowserInfo );
	}

	/*----------------------------------------------------------
		get user agent string
	----------------------------------------------------------*/
	if (( cp = getenv ( "HTTP_USER_AGENT" )) == (char *)0 )
	{
		return ( &BrowserInfo );
	}

	/*----------------------------------------------------------
		scan for browser name.  save pointer to name.
	----------------------------------------------------------*/
	cp2 = cp;
	if (( cp2 = nsStrstr ( cp, "MSIE" )) != (char *)0 )
	{
		BrowserInfo.BrowserName = BROWSER_IE;
	}
	else if (( cp2 = nsStrstr ( cp, "Firefox" )) != (char *)0 )
	{
		BrowserInfo.BrowserName = BROWSER_FIREFOX;
	}
	else if (( cp2 = nsStrstr ( cp, "Chrome" )) != (char *)0 )
	{
		BrowserInfo.BrowserName = BROWSER_CHROME;
	}
	else if (( cp2 = nsStrstr ( cp, "Links" )) != (char *)0 )
	{
		BrowserInfo.BrowserName = BROWSER_LINKS;
	}
	else if (( cp2 = nsStrstr ( cp, "Lynx" )) != (char *)0 )
	{
		BrowserInfo.BrowserName = BROWSER_LYNX;
	}
	else if (( cp2 = nsStrstr ( cp, "Opera" )) != (char *)0 )
	{
		BrowserInfo.BrowserName = BROWSER_OPERA;
	}
	else if (( cp2 = nsStrstr ( cp, "Netscape" )) != (char *)0 )
	{
		BrowserInfo.BrowserName = BROWSER_NS;
	}
	else if (( cp2 = nsStrstr ( cp, "Konqueror" )) != (char *)0 )
	{
		BrowserInfo.BrowserName = BROWSER_KONQUEROR;
	}
	else if (( cp2 = nsStrstr ( cp, "Mozilla" )) != (char *)0 )
	{
		BrowserInfo.BrowserName = BROWSER_MOZILLA;
	}
	else if (( cp2 = nsStrstr ( cp, "PalmOS" )) != (char *)0 )
	{
		BrowserInfo.BrowserName = BROWSER_WAP;
	}
	else if (( cp2 = nsStrstr ( cp, "Geos" )) != (char *)0 )
	{
		BrowserInfo.BrowserName = BROWSER_WAP;
	}
	else
	{
		cp2 = cp;
	}

	/*----------------------------------------------------------
		extract version.
	----------------------------------------------------------*/
	BrowserInfo.BrowserVersionMajor = 0;
	BrowserInfo.BrowserVersionMinor = 0;
	BrowserInfo.BrowserVersionRevision = 0;
	xo = 0;
	Part = 0;
	while ( xo != -1 && *cp2 != '\0' )
	{
		if ( *cp2 >= '0' && *cp2 <= '9' )
		{
			if ( xo == 0 )
			{
				Part++;
			}

			TempStr[xo] = *cp2;
			xo++;
		}
		else
		{
			TempStr[xo] = '\0';
			switch ( Part )
			{
				case 1: BrowserInfo.BrowserVersionMajor    = nsAtoi(TempStr); xo =  0; break;
				case 2: BrowserInfo.BrowserVersionMinor    = nsAtoi(TempStr); xo =  0; break;
				case 3: BrowserInfo.BrowserVersionRevision = nsAtoi(TempStr); xo = -1; break;
			}
		}

		cp2++;
	}

	if ( xo != -1 )
	{
		TempStr[xo] = '\0';
		switch ( Part )
		{
			case 1: BrowserInfo.BrowserVersionMajor    = nsAtoi(TempStr); break;
			case 2: BrowserInfo.BrowserVersionMinor    = nsAtoi(TempStr); break;
			case 3: BrowserInfo.BrowserVersionRevision = nsAtoi(TempStr); break;
		}
	}

	/*----------------------------------------------------------
		set capabilities.
	----------------------------------------------------------*/
	if ( BrowserInfo.BrowserName == BROWSER_IE )
	{
		if ( BrowserInfo.BrowserVersionMajor >= 4  )
		{
			BrowserInfo.AccessKey = 1;
			BrowserInfo.ClickCell = 1;
			BrowserInfo.DisableButton = 1;
		}
		if ( BrowserInfo.BrowserVersionMajor >= 6  )
		{
			BrowserInfo.CardSuits = 1;
		}
	}
	else if ( BrowserInfo.BrowserName == BROWSER_NS )
	{
		if ( BrowserInfo.BrowserVersionMajor >= 5 )
		{
			BrowserInfo.AccessKey = 1;
			BrowserInfo.ClickCell = 1;
			BrowserInfo.DisableButton = 1;
			BrowserInfo.CardSuits = 1;
		}
	}
	else if (( BrowserInfo.BrowserName == BROWSER_FIREFOX ) ||
			 ( BrowserInfo.BrowserName == BROWSER_MOZILLA ))
	{
		BrowserInfo.AccessKey = 1;
		BrowserInfo.ClickCell = 1;
		BrowserInfo.DisableButton = 1;
		BrowserInfo.CardSuits = 1;
	}
	else if ( BrowserInfo.BrowserName == BROWSER_CHROME )
	{
		BrowserInfo.AccessKey = 1;
		BrowserInfo.ClickCell = 1;
		BrowserInfo.DisableButton = 1;
		BrowserInfo.CardSuits = 1;
	}

	/*----------------------------------------------------------
		if in debug mode, print info
	----------------------------------------------------------*/
	if ( Debug )
	{
		printf ( "BrowserName    %d %s<br>\n", BrowserInfo.BrowserName, BrowserName(BrowserInfo.BrowserName) );
		printf ( "BrowserVersion %d.%d.%d<br>\n", 
				BrowserInfo.BrowserVersionMajor, BrowserInfo.BrowserVersionMinor, BrowserInfo.BrowserVersionRevision );
		printf ( "AccessKey      %d<br>\n", BrowserInfo.AccessKey );
		printf ( "ClickCell      %d<br>\n", BrowserInfo.ClickCell );
		printf ( "DisableButton  %d<br>\n", BrowserInfo.DisableButton );
		printf ( "CardSuits      %d<br>\n", BrowserInfo.CardSuits );
	}

	return ( &BrowserInfo );
}
