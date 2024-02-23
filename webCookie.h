/*---------------------------------------------------------------------------
	webCookie.h
	Web Cookie stuff is kept separate in webCookie.h and webCookie.c in
	order to allow applications to use these 'standard' cookies or to define
	their own cookie record structures (COOKIE_RECORD).  I.E., the defines
	and prototypes in this file are not included in weblib.h
---------------------------------------------------------------------------*/
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

#define		MAX_ERRORS		50
#define		MAX_ERR_STR_LEN	10240

#define	MAX_COOKIENAME_LENGTH	20
#define	MAX_USERNAME_LENGTH		20
#define	MAX_COOKIE_LENGTH		512
#define	COOKIE_FIELD_COUNT		5

typedef struct
{
	char	CookieName[MAX_COOKIENAME_LENGTH];
	char	UserName [MAX_USERNAME_LENGTH  ];
	char	Encrypted[MAX_USERNAME_LENGTH+1];
	int		LoggedIn;
	int		CheckSum;
	int		Counter;
	int		LifeSeconds;
} COOKIE_RECORD;


/* webCookie.c */
int webMakeCookie ( char *CookieString , int MaxLength , COOKIE_RECORD *CookieRecord );
void webPrintCookieString ( char *CookieString );
void webPrintCookieRecord ( COOKIE_RECORD *Cookie );
int webParseCookie ( char *CookieName , COOKIE_RECORD *CookieRecord );
char *webGetCookieUserName ( char *CookieName , char *ProgramName, char *OnFail, int WriteCookie, int LifeSeconds );
