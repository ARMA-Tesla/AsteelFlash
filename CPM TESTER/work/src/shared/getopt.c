/*
 *  "getopt.c" - Routines to process command line options and their arguments.
 *
 *	(c) 2007 ELCOM, a.s.
 *		All Rights Reserved.
 *
 *	The information  in this file  is provided for the exclusive use of
 *	the  licensees  of  ELCOM, a.s.  Such users have the right  to use,
 *	modify,  and incorporate this code into other products for purposes
 *	authorized  by the license  agreement  provided they  include  this
 *	notice  and the associated copyright notice  with any such product.
 *	The information in this file  is provided "AS IS" without warranty.
 *
 *
 * Revision History:
 * -----------------
 *	2007-03-05	Radomir Tomis (Radomir.Tomis@dvi.elcom.cz)
 *		Created from "ads/lib/misc/getopt.c".
 *
 *
 * Functions:
 * -------
 *  GetOpt		- Postupn� zpracov�n� voleb a jejich p��padn�ch parametr�
 *				  z dan�ho �et�zce dle dan�ho seznamu voleb.
 *	getwopt		- Obdoba standardn�ho 'getopt()' s podporou slovn�ch voleb.
 *				- GET Word OPTion.
 *	getopt		- Ekvivalent 'getopt()' pro OS, kter� ho nemaj�.
 */

#if 1
static char _ident[] = "@(#) getopt.c 1.63 2007-03-05";
#else
static char _ident[] = "@(#) getopt.c 1.62 2002-02-04";
static char _ident[] = "@(#) getopt.c 1.61 2000/12/17";
static char _ident[] = "@(#) getopt.c 1.6 1998/12/07";
static char _ident[] = "@(#) getopt.c 1.5 1998/09/28";
static char _ident[] = "@(#) getopt.c 1.0 1998/03/12";
#endif

#define EL_GETOPT_MODULE

#include <elBase.h>
#include <getopt.h>

#if WIN || DOS
EL_DLL_EXPORT int	optind = 1;
EL_DLL_EXPORT TCHAR	*optarg;
EL_DLL_EXPORT int	opterr = 1;
EL_DLL_EXPORT int	optopt;
#endif
#if SCO
extern int opterr;
int optopt;
#endif

/****************************************************************************/
/*
 * GetOpt() - Postupn� zpracov�n� voleb a jejich p��padn�ch parametr� z dan�ho
 *			  �et�zce dle dan�ho seznamu voleb.
 */
#undef  __FUNC__
#define __FUNC__	"GetOpt"
EL_DLL_API
#if OLD_PROTOTYPES
int GetOpt(optstring,opts,arg)
	TCHAR *optstring, *opts, **arg;
#else
int GetOpt(TCHAR *optstring,TCHAR *opts,TCHAR **arg)
#endif
{
	static TCHAR *ptr;
	register TCHAR *p, c;

	if ( opts )
		ptr = opts;
	while( (c = *ptr++) != 0 )
	{
		if ( _istspace(c) )
			continue;
		if ( c == _T('-') )
		{
			c = *ptr++;
			if ( !c || c == _T('-') )		/* "--" ? */
				break;
			if ( c == _T(':') || (p=_tcschr(optstring,c)) == NULL )
				break;
			if ( p[1] == _T(':') )
			{
				*arg = ptr;
#if 1
				while( *ptr && !_istspace(*ptr) )
					ptr++;
#else
				/*
				 * There is a bug in 'swscanf()' of MSVC 6.0, which causes GPF.
				 * It is allegedly fixed in 7.0.
				 */
				{	/* inner block */
				int n;
				_stscanf(ptr,_T("%*s%n"),&n);
				ptr += n;
				}	/* inner block */
#endif			
				return c;
			}
			;
		}
	}
	return 0;
}

/****************************************************************************/
/*
 * getwopt() - Obdoba standardn�ho 'getopt()' s podporou slovn�ch voleb.
 *			 - GET Word OPTion.
 */
#undef	__FUNC__
#define	__FUNC__	"getwopt"
EL_DLL_API
#if OLD_PROTOTYPES
TCHAR *getwopt(argc,argv,optstring,woptstring)
	int argc;
	TCHAR *argv[], *optstring, *woptstring;
#else
TCHAR *getwopt(int argc,TCHAR * const argv[],const TCHAR *optstring,const TCHAR *woptstring)
#endif
{
	/*
	 * 'woptstring': "wopt1[:][,wopt2[:][,...]]"
	 *		wopt1..woptN: slovn� volby ("install,log:,...")
	 */
	extern int optind;
	extern TCHAR *optarg;
	static TCHAR optname[20+EL_ST];	/* zde se vrac� n�zev nalezen� volby */
	static int charind = 1;		/* aktu�ln� znakov� index do 'argv[optind]' */
	TCHAR *opt;
	register const TCHAR *p, *p1, *p2;
	register int len;

	if ( charind == 1 )		/* za��n�me s dal��m 'argv[x]' ? */
	{
		/*
		 * Konec zpracov�n� voleb ? (��dn� dal�� argument / ��dn� dal�� volba /
		 * "-" / "--")
		 */
		if ( optind >= argc ||
			*argv[optind] != _T('-')
		
#if 0 && WIN
			&& *argv[optind] != _T('/')		/* "/opt" - pro Windows */
#endif
			|| !argv[optind][1] )
			return NULL;	/* konec */
		else if ( !_tcscmp(argv[optind],_T("--")) )
		{
			optind++;
			return NULL;	/* konec */
		}
	}

	/*
	 * N�vratov� hodnota:
	 */
	opt = NULL;		/* zat�m volba nenalezena */

	p1 = NULL;
	len = 0;

	/*
	 * Ukazatel na aktu�ln� zpracov�vanou volbu:
	 */
	p = &argv[optind][charind];

	/*
	 * Nejd��ve zkus�me naj�t volbu v r�mci slovn�ch ("dlouh�ch") voleb:
	 */
	if ( woptstring )
	{
		p1 = woptstring;
		do
		{
			p2 = _tcschr(p1,_T(','));
			if ( p2 )
				len = (int) (p2 - p1), p2++;
			else
				len = (int) _tcslen(p1);
			if ( p1[len-1] == _T(':') )
				len--;
#if 0
			_ftprintf(stderr,_T("%s(): p1 = '%.*s'\n"),_T(__FUNC__),len,p1);
#endif
#if 1
			if ( (int)_tcslen(p) != len )
#else	/* old incorrect code: */
			if ( (int)_tcslen(p) < len )
#endif
				continue;
#if UNIX
			if ( !_tcsncmp(p,p1,len) )
#endif
#if WIN
			if ( !_tcsnicmp(p,p1,len) )
#endif
			{
				opt = optname;
				break;
			}
		} while( (p1 = p2) != NULL );
	}

	/*
	 * Pak to zkus�me v r�mci jednoznakov�ch ("kr�tk�ch") voleb:
	 */
	if ( !opt )
	{
		if ( !optstring || (p1=_tcschr(optstring,*p)) == NULL )
		{
			optopt = *p;
			if ( opterr )
			{
				_ftprintf(stderr,
#if 1
					_T("%s: illegal option -- %c\n"),
#else
					_T("%s: illegal option '-%c'\n"),
#endif
					argv[0],*p
				);
			}
		}
		else
			opt = optname, len = 1;
	}
	if ( !opt )
	{
		opt = _T("?");
		len = 1;
		goto next_opt;
	}
	_tcsncpy(opt,p1,len);
	optname[len] = _T('\0');
#if 0
	optarg = NULL;
#endif
	if ( p1[len] == _T(':') )		/* volba vy�aduje parametr ? */
	{
		/*
		 * Po zpracov�n� parametru volby za�neme v�dy od za��tku n�sleduj�c�ho
		 * argumentu:
		 */
		charind = 1;

		/*
		 * Parametr volby bezprost�edn� n�sleduje volbu v r�mci
		 * stejn�ho argumentu p��kazov� ��dky ?
		 */	
		if ( p[len] )
			optarg = (TCHAR *)p + len, optind++;		/* ano */
		else
		{
			/*
			 * Parametr volby vememe z n�sleduj�c�ho argumentu (pokud
			 * existuje):
			 */
			if ( ++optind < argc )
				optarg = argv[optind++];
			else
			{
				if ( opterr )
				{
					_ftprintf(stderr,
#if 1
						_T("%s: option requires an argument -- %s\n"),
#else
						_T("%s: option '-%s' requires an argument\n"),
#endif
						argv[0],optname);
				}
				opt = _T("?");
				goto done;
			}
		}
	}
	else
	{
next_opt:
		/*
		 * Volba bez parametru:
		 *
		 * N�sleduj� bezprost�edn� za volbou je�t� n�jak� znaky v r�mci stejn�-
		 * ho argumentu ?
		 */
		if ( p[len] )
			charind += len;			/* ano, p��t� je zpracujeme */
		else
			optind++, charind = 1;	/* ne, dal�� argument na holen� */
	}
done:
	if ( opt && *opt == _T('?') )	/* error ? */
		charind = 1;	/* reset value to start with on next call */
	return opt;		/* vrac�me n�zev volby ("i" / "install") */
}

/****************************************************************************/
#if WIN || DOS	/* -- { -- */
/*
 * getopt() - Ekvivalent 'getopt()' pro OS, kter� ho nemaj�.
 */
#undef	__FUNC__
#define	__FUNC__	"getopt"
EL_DLL_API
#if OLD_PROTOTYPES
int getopt(argc,argv,optstring)
	int argc;
	TCHAR * const argv[];
	const TCHAR *optstring;
#else
int getopt(int argc,TCHAR * const argv[],const TCHAR *optstring)
#endif
{
	TCHAR *p;

#if 0
	fprintf(stderr,"%s(): argc: %d, argv: 0x%lX, optstring: %s, optind: %d\n",
		__FUNC__,argc,argv,optstring,optind);
#endif
	p = getwopt(argc,argv,optstring,(const TCHAR *)NULL);
	return p ? *p : -1;
}
#endif	/* -- } -- WIN -- */

/****************************************************************************/
