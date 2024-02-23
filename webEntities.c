/*----------------------------------------------------------------------------
	Program : webEntities.c
	Author  : Tom Stevelt
	Date    : 12/09/2020
	Synopsis: Clean up text for HTML and MySql.
	Return  : 0 .. positive is number of substitutions made.
			  negative on errirs
	Refers  : https://www.ascii-code.com/

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

#define TESTMAIN
#undef TESTMAIN

#ifdef TESTMAIN
#define		nsStrlen		strlen
#define		nsStrcat		strcat
#endif

static	unsigned char	*OutputText = NULL;
static	int		CurrentLength = 0;

typedef struct
{
	int		Decimal;
	char	*Entity;
} RECORD;

static	RECORD	Array[] =
{
	{ 34, "&quot;" },
	{ 38, "&amp;" },
	{ 39, "&apos;" },
	{ 60, "&lt;" },
	{ 62, "&gt;" },

	{ 128,"&euro;" },
	{ 130,"&sbquo;" },
	{ 131,"&fnof;" },
	{ 132,"&bdquo;" },
	{ 133,"&hellip;" },
	{ 134,"&dagger;" },
	{ 135,"&Dagger;" },
	{ 136,"&circ;" },
	{ 137,"&permil;" },
	{ 138,"&Scaron;" },
	{ 139,"&lsaquo;" },
	{ 140,"&OElig;" },
	{ 145,"&lsquo;" },
	{ 146,"&rsquo;" },
	{ 147,"&ldquo;" },
	{ 148,"&rdquo;" },
	{ 149,"&bull;" },
	{ 150,"&ndash;" },
	{ 151,"&mdash;" },
	{ 152,"&tilde;" },
	{ 153,"&trade;" },
	{ 154,"&scaron;" },
	{ 155,"&rsaquo; " },
	{ 156,"&oelig;" },
	{ 159,"&Yuml;" },
	{ 160,"&nbsp;" },
	{ 161,"&iexcl;" },
	{ 162,"&cent;" },
	{ 163,"&pound;" },
	{ 164,"&curren;" },
	{ 165,"&yen;" },
	{ 166,"&brvbar;" },
	{ 167,"&sect;" },
	{ 168,"&uml;" },
	{ 169,"&copy;" },
	{ 170,"&ordf;" },
	{ 171,"&laquo;" },
	{ 172,"&not;" },
	{ 173,"&shy;" },
	{ 174,"&reg;" },
	{ 175,"&macr;" },
	{ 176,"&deg;" },
	{ 177,"&plusmn;" },
	{ 178,"&sup2;" },
	{ 179,"&sup3;" },
	{ 180,"&acute;" },
	{ 181,"&micro;" },
	{ 182,"&para;" },
	{ 183,"&middot;" },
	{ 184,"&cedil;" },
	{ 185,"&sup1;" },
	{ 186,"&ordm;" },
	{ 187,"&raquo;" },
	{ 188,"&frac14;" },
	{ 189,"&frac12;" },
	{ 190,"&frac34;" },
	{ 191,"&iquest;" },
	{ 192,"&Agrave;" },
	{ 193,"&Aacute;" },
	{ 194,"&Acirc;" },
	{ 195,"&Atilde;" },
	{ 196,"&Auml;" },
	{ 197,"&Aring;" },
	{ 198,"&AElig;" },
	{ 199,"&Ccedil;" },
	{ 200,"&Egrave;" },
	{ 201,"&Eacute;" },
	{ 202,"&Ecirc;" },
	{ 203,"&Euml;" },
	{ 204,"&Igrave;" },
	{ 205,"&Iacute;" },
	{ 206,"&Icirc;" },
	{ 207,"&Iuml;" },
	{ 208,"&ETH;" },
	{ 209,"&Ntilde;" },
	{ 210,"&Ograve;" },
	{ 211,"&Oacute;" },
	{ 212,"&Ocirc;" },
	{ 213,"&Otilde;" },
	{ 214,"&Ouml;" },
	{ 215,"&times;" },
	{ 216,"&Oslash;" },
	{ 217,"&Ugrave;" },
	{ 218,"&Uacute;" },
	{ 219,"&Ucirc;" },
	{ 220,"&Uuml;" },
	{ 221,"&Yacute;" },
	{ 222,"&THORN;" },
	{ 223,"&szlig;" },
	{ 224,"&agrave;" },
	{ 225,"&aacute;" },
	{ 226,"&acirc;" },
	{ 227,"&atilde;" },
	{ 228,"&auml;" },
	{ 229,"&aring;" },
	{ 230,"&aelig;" },
	{ 231,"&ccedil;" },
	{ 232,"&egrave;" },
	{ 233,"&eacute;" },
	{ 234,"&ecirc;" },
	{ 235,"&euml;" },
	{ 236,"&igrave;" },
	{ 237,"&iacute;" },
	{ 238,"&icirc;" },
	{ 239,"&iuml;" },
	{ 240,"&eth;" },
	{ 241,"&ntilde;" },
	{ 242,"&ograve;" },
	{ 243,"&oacute;" },
	{ 244,"&ocirc;" },
	{ 245,"&otilde;" },
	{ 246,"&ouml;" },
	{ 247,"&divide;" },
	{ 248,"&oslash;" },
	{ 249,"&ugrave;" },
	{ 250,"&uacute;" },
	{ 251,"&ucirc;" },
	{ 252,"&uuml;" },
	{ 253,"&yacute;" },
	{ 254,"&thorn;" },
	{ 255,"&yuml;" },
};

static	int		Count = sizeof(Array) / sizeof(RECORD);

static int cmprec ( RECORD *a, RECORD *b )
{
	if ( a->Decimal < b->Decimal )
	{
		return ( -1 );
	}
	if ( a->Decimal > b->Decimal )
	{
		return ( 1 );
	}
	return ( 0 );
}

static char *LookupEntity ( int Decimal )
{
	RECORD	*ptr, key;

	key.Decimal = Decimal;
	ptr = bsearch ( &key, Array, Count, sizeof(RECORD), (int(*)()) cmprec );

	if ( ptr == NULL )
	{
		return ( NULL );
	}

	return ( ptr->Entity );
}

int webEntities ( unsigned char *InputText, int Options, unsigned char **NewText )
{
	int		SubstituteCounter = 0;
	int		NewLength;
	int		MaxEntityLength = 6;
	int		xl, xi, xo, xe;
	int		Debug = 0;
	char	*ptrEntity;
	int		ival;
	int		InTag = 0;
	int		IsEntity = 0;

	if ( Options == 0 )
	{
		Options = WE_ALL;
	}

	for ( xi = 0; xi < Count; xi++ )
	{
		xl = nsStrlen ( Array[xi].Entity );

		if ( MaxEntityLength < xl )
		{
			MaxEntityLength = xl;
		}
	}

	if ( Debug )
	{
		printf ( "MaxEntityLength %d\n", MaxEntityLength );
	}

	NewLength = nsStrlen ( (char *) InputText );
	if ( NewLength == 0 )
	{
		return ( -1 );
	}

	if ( NewLength > CurrentLength )
	{
		CurrentLength = NewLength;
		if ( Debug )
		{
			printf ( "realloc %d\n", CurrentLength );
		}
		if (( OutputText = realloc ( OutputText, CurrentLength )) == (unsigned char *)0 )
		{
			return ( -1 );
		}
	}

	memset ( OutputText, '\0', CurrentLength );

	for ( xi = 0, xo = 0; xi < NewLength; xi++ )
	{
		if (  xo + MaxEntityLength > CurrentLength )
		{
			CurrentLength = CurrentLength * 2.0;
			if ( Debug )
			{
				printf ( "realloc %d\n", CurrentLength );
			}
			if (( OutputText = realloc ( OutputText, CurrentLength )) == (unsigned char *)0 )
			{
				return ( -1 );
			}
		}

		if (( (Options&WE_ENTITIES) == WE_ENTITIES ) && 
			( InputText[xi] == '&'                 ))
		{
			/*---------------------------------------------------------------------------
				go cheap route, look for semi-colon with MaxEntityLength bytes.
				expensive route would search Array for matching entity.
			---------------------------------------------------------------------------*/
			IsEntity = 0;
			for ( xe = 1; xe < MaxEntityLength && xi + xe < NewLength; xe++ )
			{
				if ( InputText[xi+xe] == ';' )
				{
					IsEntity = 1;
					break;
				}
			}
			if ( IsEntity )
			{
				OutputText[xo++] = InputText[xi];
				continue;
			}
		}

		if ( (Options&WE_TAGS) == WE_TAGS )
		{
			if ( InputText[xi] == '<' )
			{
				InTag = 1;
			}
			else if ( InputText[xi] == '>' )
			{
				InTag = 2;
			}
		}
		
		if ( InTag )
		{
			OutputText[xo++] = InputText[xi];
			if ( InTag == 2 )
			{
				InTag = 0;
			}
			continue;
		}

		ival = (int) InputText[xi];
		if ( Debug == 39 )
		{
			printf ( "%c %d\n", InputText[xi], ival );
		}

		ptrEntity = NULL;
		if ( ival > 0 && ival <= 127 && (Options&WE_LOW) == WE_LOW )
		{
			ptrEntity = LookupEntity ( ival );
		}
		else if ( ival > 127 && ival < 255 && (Options&WE_HIGH) == WE_HIGH )
		{
			ptrEntity = LookupEntity ( ival );
		}
		if ( ptrEntity == NULL )
		{
			OutputText[xo++] = InputText[xi];
		}
		else
		{
			nsStrcat ( (char *) OutputText, ptrEntity );
			xl = nsStrlen ( ptrEntity );
			xo += xl;
			SubstituteCounter++;
		}

/*---------------------------------------------------------------------------
					if ( Debug )
					{
						printf ( "=== LOOKUP %d ===", (int) InputText[xi] );
					}

					if ( ptrEntity == NULL )
					{
						if ( Debug )
						{
							printf ( " not found\n" );
						}
					}
					else
					{
						if ( Debug )
						{
							printf ( " found add %d\n", xl );
						}
					}
---------------------------------------------------------------------------*/

	}

	*NewText = OutputText;

	return ( SubstituteCounter );
}

#ifdef TESTMAIN
int main ( int argc, char *argv[] )
{
	unsigned char	Buffer[100000];
	FILE	*fp;
	int		nr;
	int		rv;
	unsigned char	*NewText = NULL;

	if ( argc < 2 )
	{
//		rv = webEntities ( "this is a test of the emergency broadcast system.", 0, &NewText );
//		rv = webEntities ( "mrs. smith's apples are red & jenny's eyes are blue.", 0, &NewText );
//		rv = webEntities ( "This is the URL <a href='https://www.stevelt.com'>Click here fuck'r</a>", 0, &NewText );
		rv = webEntities ( "mrs. smith's apples are red &amp; jenny's eyes are blue.", 0, &NewText );

		if ( rv == -1 )
		{
			printf ( "An error occurred\n" );
		}
		else if ( rv == 0 )
		{
			printf ( "No substitutions\n" );
		}
		else
		{
			printf ( "%d substitutions\n", rv );
			printf ( "%s\n", NewText );
		}
	}
	else
	{
		if (( fp = fopen ( argv[1], "r" )) == (FILE *)0 )
		{
			printf ( "Cannot open %s fore reading\n", argv[1] );
			exit ( 1 );
		}

		nr = fread ( Buffer, 1, sizeof(Buffer), fp );
		Buffer[nr] = '\0';

		fclose ( fp );

		rv = webEntities ( Buffer, 0, &NewText );
		if ( rv == -1 )
		{
			printf ( "An error occurred\n" );
		}
		else if ( rv == 0 )
		{
			printf ( "No substitutions\n" );
		}
		else
		{
			printf ( "%d substitutions\n", rv );
			printf ( "%s\n", NewText );
		}
	}
	
	return ( 0 );
}
#endif
