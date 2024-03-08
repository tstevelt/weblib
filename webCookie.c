/*----------------------------------------------------------------------------
	Program : webCookie.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Nov 2020
	Synopsis: Cookie functions
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


#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<time.h>
#include	<sys/time.h>

#include	"shslib.h"
#include	"webCookie.h"

#undef DO_CODING
#define DO_CODING

#define		DEBUG
#undef		DEBUG

int webMakeCookie ( char *CookieString, int MaxLength, COOKIE_RECORD *CookieRecord )
{
	long			tvec;
	int				xi, xl;
	char			Salt;
	char			DateString[40];
	long			xtime;
	struct tm		*tm;
	char			*Dow[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	char			*Mon[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov",  "Dec" };

#ifdef DEBUG
	fprintf ( stderr, "webMakeCookie: CR->CookieName = %s, CR->UserName = %s\n", 
					CookieRecord->CookieName, CookieRecord->UserName );
#endif

	if ( nsStrlen ( CookieRecord->CookieName ) == 0 )
	{
		return ( -1 );
	}

	if ( nsStrlen ( CookieRecord->UserName ) == 0 )
	{
		return ( -2 );
	}

	time ( &tvec );
	srand ( (int) tvec );
	xi = rand();
	xi = xi % 26;
	Salt = 'a' + xi;
	if ( Salt < 'a' || Salt > 'z' )
	{
		Salt = 'j';
	}

#ifdef DO_CODING
	sprintf ( CookieRecord->Encrypted, "%s", tmsEncode ( Salt, CookieRecord->UserName ) );
#else
	sprintf ( CookieRecord->Encrypted, "%s", CookieRecord->UserName );
#endif

#ifdef DEBUG
	fprintf ( stderr, "CR->Encrypted = %s\n", CookieRecord->Encrypted );
#endif

	xl = nsStrlen ( CookieRecord->Encrypted );
#ifdef DEBUG
	fprintf ( stderr, "xl = %d\n", xl );
#endif

	CookieRecord->CheckSum = 0;
	for ( xi = 0; xi < xl; xi++ )
	{
		CookieRecord->CheckSum = CookieRecord->CheckSum + CookieRecord->Encrypted[xi];
	}
	CookieRecord->CheckSum = CookieRecord->CheckSum + CookieRecord->LoggedIn;
#ifdef DEBUG
	fprintf ( stderr, "CR->CheckSum = %u length %d\n", CookieRecord->CheckSum, xl );
#endif

	snprintf ( CookieString, MaxLength, "%s=%s:%u:%u:%u;", 
			CookieRecord->CookieName,
			CookieRecord->Encrypted,
			CookieRecord->Counter,
			CookieRecord->LoggedIn,
			CookieRecord->CheckSum );

	strcat ( CookieString, " Version=1;" );

	if ( CookieRecord->LifeSeconds > 0 )
	{
		time ( &xtime );
		tm = localtime ( &xtime );
		tm->tm_gmtoff = 0 - tm->tm_gmtoff;
		xtime = xtime + CookieRecord->LifeSeconds + (tm->tm_gmtoff);
		tm = localtime ( &xtime );

		snprintf ( DateString, sizeof(DateString), " expires=%s, %d-%s-%d %02d:%02d:%02d GMT;",
					Dow[tm->tm_wday],
					tm->tm_mday,
					Mon[tm->tm_mon],
					tm->tm_year + 1900,
					tm->tm_hour,
					tm->tm_min,
					tm->tm_sec );

#ifdef DEBUG
		fprintf ( stderr, "DateString = %s\n", DateString );
#endif

		strcat ( CookieString, DateString );
	}

#ifdef DEBUG
		fprintf ( stderr, "CookieString = %s\n", CookieString );
#endif

	return ( 0 );
}

void webPrintCookieString ( char *CookieString )
{
	printf ( "Set-Cookie: %s;\n", CookieString );
}

void webPrintCookieRecord ( COOKIE_RECORD *Cookie )
{
	char	CookieString[MAX_COOKIE_LENGTH];

	if ( webMakeCookie ( CookieString, MAX_COOKIE_LENGTH, Cookie ) == 0 )
	{
		webPrintCookieString ( CookieString );
	}
}

int webParseCookie ( char *CookieName, COOKIE_RECORD *CookieRecord )
{
#define		MAXTOKS		20
	int		rv = 0;
	char	*tokens[MAXTOKS];
	int		tokcnt;
	char 	*CookieString;
	char	*cp;
	char	WorkBuffer[MAX_COOKIE_LENGTH];
	int		MyCheckSum;
	int		xi, xl;

	if (( CookieString = getenv ( "HTTP_COOKIE" )) == NULL )
	{
#ifdef DEBUG
		fprintf ( stderr, "MY COOKIE NOT FOUND\n" );
#endif
		return ( -1 );
	}

#ifdef DEBUG
	fprintf ( stderr, "HTTP_COOKIE = %s\n", CookieString );
#endif

	if (( CookieString = nsStrstr ( CookieString, CookieName )) == (char *)0 )
	{
#ifdef DEBUG
		fprintf ( stderr, "COOKIE NAMED %s NOT FOUND!\n", CookieName );
#endif
		return ( -1 );
	}

	snprintf ( WorkBuffer, MAX_COOKIE_LENGTH, "%s", CookieString );

	if (( cp = nsStrstr ( WorkBuffer, ";" )) != ( char *)0 )
	{
		*cp = '\0';
	}

	if (( tokcnt = GetTokensStd ( WorkBuffer, "=:", tokens, MAXTOKS, 0 )) < COOKIE_FIELD_COUNT )
	{
		fprintf ( stderr, "Invalid format for %s cookie!\n", CookieName );
		return ( -1 );
	}

	snprintf ( CookieRecord->Encrypted, MAX_USERNAME_LENGTH, "%s", tokens[1] );

#ifdef DO_CODING
	snprintf ( CookieRecord->UserName,  MAX_USERNAME_LENGTH, "%s", tmsDecode ( CookieRecord->Encrypted ) );
#else
	snprintf ( CookieRecord->UserName,  MAX_USERNAME_LENGTH, "%s", CookieRecord->Encrypted );
#endif

#ifdef DEBUG
	fprintf ( stderr, "UserName: %s\n", CookieRecord->UserName );
#endif

	CookieRecord->Counter = nsAtoi ( tokens[2] );

	CookieRecord->LoggedIn = nsAtoi ( tokens[3] );

	CookieRecord->CheckSum = nsAtoi ( tokens[4] );

/*----------------------------------------------------------
	snprintf ( CookieString, MaxLength, "%s=%s:%u:%u:%u;", 
			CookieRecord->CookieName,
			CookieRecord->Encrypted,
			CookieRecord->Counter,
			CookieRecord->LoggedIn,
			CookieRecord->CheckSum );

----------------------------------------------------------*/
	MyCheckSum = 0;
	xl = nsStrlen ( CookieRecord->Encrypted );
	for ( xi = 0; xi < xl; xi++ )
	{
		MyCheckSum += CookieRecord->Encrypted[xi];
	}
	MyCheckSum += CookieRecord->LoggedIn;
	if ( MyCheckSum != CookieRecord->CheckSum )
	{
		fprintf ( stderr, 
			"Invalid %s cookie! checksum mine %d cookie %d, length %d\n", 
				CookieName, MyCheckSum, CookieRecord->CheckSum, xl );

		memset ( CookieRecord, '\0', sizeof(COOKIE_RECORD) );
		return ( -1 );
	}

	return ( rv );
}

static	char	EnvVar[256];

char *webGetCookieUserName ( char *CookieName, char *ProgramName, char *OnFail, int WriteCookie, int LifeSeconds )
{
	int		rv;
	static	COOKIE_RECORD	Cookie;
	char	*Argv[2];

	rv = webParseCookie ( CookieName, &Cookie );

#ifdef DEBUG
	fprintf ( stderr, "Logged in %d cookie name %20.20s user name %8.8s, rv %d\n", 
				Cookie.LoggedIn, Cookie.CookieName, Cookie.UserName, rv );
#endif

	if ( rv == 0 )
	{
		if (( Cookie.LoggedIn == 0                       ) || 
			( nsStrcmp ( Cookie.UserName, "unknown" ) == 0 ) ||
			( lastchr ( Cookie.UserName, 8 ) == 0        ))
		{
			rv = -1;
		}
	}

	if ( rv != 0 )
	{
		if ( OnFail == (char *)0 )
		{
			return ( (char *)0 );
		}
		else if ( nsStrlen ( OnFail ) == 0 )
		{
			return ( (char *)0 );
		}
		else
		{
/*--------------------------------------------------------------
			execlp ( OnFail, (char *) 0 );
			execlp ( OnFail, "", (char *) 0 );
--------------------------------------------------------------*/
			Argv[0] = OnFail;
			Argv[1] = (char *)0;
			execvp ( OnFail, Argv );
		}
	}

	if ( WriteCookie )
	{
		FILE	*logfp;
		char	*cp;
		char	AsciiDate[9], AsciiTime[9];
		extern	int		include_pid;

		Cookie.Counter++;
		if ( LifeSeconds > 0 )
		{
			Cookie.LifeSeconds = LifeSeconds;
		}
		else
		{
			Cookie.LifeSeconds = 0;
		}
		webPrintCookieRecord ( &Cookie );

		include_pid = 0;

		/*---------------------------------------------------------------------------
			replace with in-line code, debug statements (printf) in logmsg system
			cause web pages to crash.
		xxx	logDTmsg ( "/var/local/access.log", Cookie.UserName, ProgramName );
		---------------------------------------------------------------------------*/
		/*---------------------------------------------------------------------------
			move log to /var/local/webstate for ipmon on IAT
			only log iatweb, others are logged already in other ways.
		---------------------------------------------------------------------------*/
		if ( nsStrcmp ( CookieName, "iatweb" ) == 0 )
		{
			Xgetdate ( AsciiDate, AsciiTime );

			cp = getenv ( "REMOTE_ADDR" );
			if ( cp == (char *)0 )
			{
				cp = "-";
			}

			if (( logfp = fopen ( "/var/local/webstate/iatweb/access.log", "a+" )) != (FILE *)0 )
			{
				fprintf ( logfp, "%s %-9.9s %-9.9s %-8.8s %s\n", 
						cp, AsciiDate, AsciiTime, Cookie.UserName, ProgramName );
				fflush ( logfp );
				nsFclose ( logfp );
			}
		}

		if (( cp = getenv ( "REMOTE_USER" )) == (char *)0 )
		{
			sprintf ( EnvVar, "REMOTE_USER=%s", Cookie.UserName );
			putenv ( EnvVar );
		}
	}

	return ( Cookie.UserName );
}
