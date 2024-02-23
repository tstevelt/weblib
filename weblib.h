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

#ifdef	MAIN
#define	TYPE	/* */
#else
#define	TYPE	extern
#endif

typedef struct
{
	char *WP_Title;
	char *WP_OnLoad;
	char *WP_Color;
	char *WP_Class;
	char *WP_FormAction;
	char *WP_FormEnctype;
	char *WP_FormMethod;
	char *WP_FormName;
	char *WP_FormOther;
	char *WP_FileName;
} WEBPARMS;


#define		CAPTCHADIR		"/var/local/webstate/char74k"

#define	MAXCHARS	100000

#define	MAXWEBPARMSS		10
TYPE	char	*webArgv[MAXWEBPARMSS];
TYPE	int		webArgc;

#define	MAXCOMMANDLINE	100
TYPE	char	*webCommands[MAXCOMMANDLINE];
TYPE	int		 webCommandCount;

TYPE	char	StdinBuffer[MAXCHARS];
#define	MAXSTDIN	3000
TYPE	char	*webStdinNames[MAXSTDIN];
TYPE	char	*webStdinValues[MAXSTDIN];
TYPE	int		 webStdinCount;

TYPE	char	envbuf[MAXCHARS];
#define	MAXENV	100
TYPE	char	*webEnvNames[MAXENV];
TYPE	char	*webEnvValues[MAXENV];
TYPE	int		 webEnvCount;

TYPE	char	qrybuf[MAXCHARS];
#define	MAXQRY	100
TYPE	char	*webQryNames[MAXENV];
TYPE	char	*webQryValues[MAXENV];
TYPE	int		 webQryCount;

#define		MAXINTEGRATED	(MAXSTDIN+MAXQRY)
TYPE	char	*webNames [MAXINTEGRATED];
TYPE	char	*webValues[MAXINTEGRATED];
TYPE	int		 webCount;

typedef struct
{
	char	*lookfor;
	int		looklen;
	char	*replacewith;
	int		sublen;
} SUBSTITUTE;

#define	MAXSUBS	100
TYPE SUBSTITUTE	subarr[MAXSUBS];
TYPE int subcnt;

typedef struct
{
	char	*program;
	char	*problem;
	char	*server;
	int		 errnum;
	char	*errmsg;
	char	*action;
} CGIERROR;


#define BROWSER_UNK			190
#define BROWSER_FIREFOX		191
#define BROWSER_IE			192
#define BROWSER_NS			193
#define	BROWSER_LINKS		194
#define	BROWSER_LYNX		195
#define	BROWSER_OPERA		196
#define	BROWSER_KONQUEROR	197
#define	BROWSER_MOZILLA		198
#define	BROWSER_WAP			199
#define	BROWSER_CHROME		200

/*----------------------------------------------------------
	LOW - replace low ascii
	HIGH - replace high ascii
	TAGS - preserve HTML tags
	ENTITIES - don't replace ampersand if text already has
				entities in it.
----------------------------------------------------------*/
#define		WE_LOW			2
#define		WE_HIGH			4
#define		WE_TAGS			8
#define		WE_ENTITIES		16
#define		WE_ALL		(WE_LOW|WE_HIGH|WE_TAGS|WE_ENTITIES)

typedef struct
{
	int		BrowserName;
	short	BrowserVersionMajor;
	short	BrowserVersionMinor;
	short	BrowserVersionRevision;
	int		AccessKey;
	int		ClickCell;
	int		DisableButton;
	int		CardSuits;
} BROWSER_INFO;


/*----------------------------------------------------------
:.,$d
:r ! mkproto -p *.c
?main
remove COOKIE prototypes, they are in webCookie.h
----------------------------------------------------------*/

/* webContentFuncs.c */
void webContentHtml ( void );
void webContentMSExcel ( void );
void webContentMSWord ( void );
void webContentPDF ( WEBPARMS *ptrWebParms );
void webContentPlainText ( void );
void webContentXML ( void );

/* webFormFuncs.c */
void webStartForm ( WEBPARMS *ptrWebParms );
void webEndForm ( void );

/* webJaavFuncs.c */
void webStartJava ( void );
void webEndJava ( void );
void webIncludeJava ( char *JavaScriptFile , char *Version );

/* webPageFuncs.c */
void webStartHead ( WEBPARMS *ptrWebParms );
void webEndPage ( void );

/* webEntities.c */
int webEntities ( unsigned char *InputText , int Options , unsigned char **NewText );

/* webFixHex.c */
void webFixHex ( char *SomeString );

/* webFreeParms.c */
void webFreeParms ( WEBPARMS *ptrWebParms );

/* webGetBrowser.c */
char *BrowserName ( int BrowserCode );
BROWSER_INFO *webGetBrowser ( int Debug );

/* webGetCommandLine.c */
int webGetCommandLine ( int argc , char *argv []);

/* webGetEnvironment.c */
int webGetEnvironment ( char *envp []);

/* webGetInput.c */
int webGetInput ( void );

/* webGetOneEnv.c */
char *webGetOneEnv ( char *EnvVarName );

/* webGetQuery.c */
int webGetQuery ( void );

/* webGetStdin.c */
int webGetStdin ( void );

/* webInitParms.c */
WEBPARMS *webInitParms ( void );

/* webTrim.c */
void webTrim ( char SomeString [], int OptionalLength );
