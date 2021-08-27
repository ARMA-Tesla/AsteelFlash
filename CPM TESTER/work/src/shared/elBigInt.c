/*
 *	"elBigInt.c" - Big Integer routines.
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
 *	2007-10-06	Radomir Tomis (Radomir.Tomis@dvi.elcom.cz)
 *		'elBigIntVal2StrTSep()': resulting string returned using
 *		'elStrTmpFmt()'.
 *	2007-08-05	Radomir Tomis (Radomir.Tomis@dvi.elcom.cz)
 *		Created from "lib/LGIntStr.c".
 *
 * Functions:
 * ----------
 *	elBigIntVal2StrTSep		Converts value to string with thousand separators.
 *	elBigIntVal2StrMMult	Converts value to string using one or more
 *							(multiple) integer multiples.
 *	elBigIntVal2StrMMultOpt	Optional 'elBigIntVal2StrMMult()'.
 *	elBigIntStr2ValMMult	Converts string with one or more (multiple)
 *							multiples to value.
 *	elBigIntStr2ValSMult	Converts string with single multiple to value.
 *	elBigIntVal2RValSMult	Converts value to real value using single multiple.
 *	elBigIntVal2RStrSMult	Converts value to real value string using single
 *							multiple.
 *	elBigIntMult			Returns attributes for multiple given by its index.
 */

#if 1
static char _ident[] = "@(#) elBigInt.c v1.01 2007-10-06";
#else
static char _ident[] = "@(#) elBigInt.c v1.0 2007-08-05";
#endif

#include <elBase.h>
#include <elLog.h>
#include <elStr.h>
#include <elBigInt.h>

/*
 * Multiples:
 *
 * The binary multiples are defined as standard in:
 *
 *	Amendment 2 to IEC International Standard IEC 60027-2:
 *		Letter symbols to be used in electrical technology
 *		- Part 2: Telecommunications and electronics
 *
 * See <http://physics.nist.gov/cuu/Units/binary.html> for details.
 */
static struct {
	char *sym, *name;
	bool binary;
	elBigUInt_t val;
} gs_mult[] = {
	"b",	"block",	(bool)-1,	EL_KiB/2,	/* not a standard multiple */
	"k",	"kilo",		FALSE,		1000,
	"M",	"mega",		FALSE,		1000*1000,
	"G",	"giga",		FALSE,		1000*1000*1000,
#if EL_BIG_INT_SIZEOF > 4			
	"T",	"tera",		FALSE,		1000i64*1000*1000*1000,
	"P",	"peta",		FALSE,		1000i64*1000*1000*1000*1000,
	"E",	"exa",		FALSE,		1000i64*1000*1000*1000*1000*1000,
#endif								
#if EL_BIG_INT_SIZEOF > 8			
	"Z",	"zetta",	FALSE,		1000i64*1000*1000*1000*1000*1000*1000,
	"Y",	"yotta",	FALSE,		1000i64*1000*1000*1000*1000*1000*1000*1000,
#endif								
	"Ki",	"kibi",		TRUE,		EL_KiB,
	"Mi",	"mebi",		TRUE,		EL_MiB,
	"Gi",	"gibi",		TRUE,		EL_GiB,
#if EL_BIG_INT_SIZEOF > 4			
	"Ti",	"tebi",		TRUE,		1024i64*EL_GiB,
	"Pi",	"pebi",		TRUE,		1024i64*1024*EL_GiB,
	"Ei",	"exbi",		TRUE,		1024i64*1024*1024*EL_GiB,
#endif
#if EL_BIG_INT_SIZEOF > 8
	"Zi",	"zebi",		TRUE,		1024i64*1024*1024*1024*EL_GiB,
	"Yi",	"yobi",		TRUE,		1024i64*1024*1024*1024*1024*EL_GiB,
#endif
};
#define MULTs	EL_ARRAY_CNT(gs_mult)

/******************************************************************************/
/*
 * 'mult_by_sym()': Multiplies given value by given multiple symbol.
 */
#undef	__FUNC__
#define	__FUNC__	"mult_by_sym"
#if OLD_PROTOTYPES
static void mult_by_sym(sym,val,p_sym_end)
register char *sym;
elBigUInt_t *val;
char **p_sym_end;
#else
static void mult_by_sym(char *sym,elBigUInt_t *val,char **p_sym_end)
#endif
{
	register int i, ifound, len, maxlen;

	if ( isdigit(*sym) )
	{
		char *sym_end;

		*val *= strtol(sym,&sym_end,0);
		sym = sym_end;
	}
	else
	{
		/* find the longest ("Mi" vs. "M") matching multiple prefix: */
		for(i = maxlen = 0, ifound = -1; i < MULTs; i++)
		{
			len = (int) strlen(gs_mult[i].sym);
			if ( !strncmp(gs_mult[i].sym,sym,len) && len > maxlen )
				ifound = i, maxlen = len;
		}
		if ( ifound == -1 )
		{
#if 0
			elLog("'%s': unknown multiple prefix",mult_sym);
#endif
		}
		else
		{
			sym += maxlen;
			*val *= gs_mult[ifound].val;
		}
	}

	if ( p_sym_end )
		*p_sym_end = sym;
}

/****************************************************************************/
/*
 * 'elBigIntVal2StrTSep()': Converts value to string with thousand separators.
 *
 * For example: 2,000,000,000 = "2,000,000,000"
 */
#undef	__FUNC__
#define	__FUNC__	"elBigIntVal2StrTSep"
EL_DLL_API
#if OLD_PROTOTYPES
char *elBigIntVal2StrTSep(val)
elBigUInt_t val;
#else
char *elBigIntVal2StrTSep(elBigUInt_t val)
#endif
{
	static char thousep_str[4+EL_ST];
#if 1
	char *str = elStrTmpFmt("%*s",EL_BIG_UINT_MAX_DECIMAL_STRLEN_WSEP,"");
#else
	static char str[EL_BIG_UINT_MAX_DECIMAL_STRLEN_WSEP+EL_ST];
#endif
	char val_str[EL_BIG_UINT_MAX_DECIMAL_STRLEN+EL_ST];	
	char *p;
	register int i, j, l;

	if ( !str )
		return __FUNC__ "(): cannot get temporary string !";

	if ( !*thousep_str )
	{
#if WIN && !MQX
		char buf[10+EL_ST];

		if ( !GetLocaleInfo(LOCALE_USER_DEFAULT,
#if 1
			LOCALE_STHOUSAND,
#elif 1
			LOCALE_SMONTHOUSANDSEP 
#else
			LOCALE_SGROUPING 
#endif
			buf,
			EL_ARRAY_CNT(buf)
		) )
			p = buf;
		else
			p = NULL;
#elif UNIX
		p = nl_langinfo(THOUSEP);
#else
		p = ",";
#endif
		/* default thousand separator is ",": */
		strcpy(thousep_str,p && *p ? p : ",");
	}
	sprintf(val_str,"%" EL_BIG_INT_FMTSZPFX "u",val);
	l = (int) strlen(val_str);
	for(i = j = 0; i < l; i++)
	{
		str[j++] = val_str[i];
		if ( l-i>1 && (l-(i+1)) % 3 == 0 )
			for(p = thousep_str; *p; p++)
				str[j++] = *p;
	}
	str[j] = '\0';
	return str;
}

/******************************************************************************/
/*
 * 'elBigIntVal2StrMMult()':
 *
 * Converts value to string using one or more (multiple) integer multiples.
 *
 * For example: 2,000,000,000 = "1Gi+883Mi+357Ki"
 */
#undef	__FUNC__
#define	__FUNC__	"elBigIntVal2StrMMult"
EL_DLL_API
#if OLD_PROTOTYPES
char *elBigIntVal2StrMMult(val,binary)
register elBigUInt_t val;
bool binary;
#else
char *elBigIntVal2StrMMult(register elBigUInt_t val,bool binary)
#endif
{
	static char str[EL_BIG_UINT_MAX_MMULT_STRLEN+EL_ST];
	register char *p;
	register i;

	*str = '\0';
	for(i = MULTs-1, p = str; i >= 0 && val; i--)
	{
		register elBigUInt_t v;

		if ( gs_mult[i].binary != binary || gs_mult[i].val > val )
			continue;
		if ( p != str )
			*p++ = '+';
		v = val / gs_mult[i].val;
		p += sprintf(p,"%" EL_BIG_INT_FMTSZPFX "u%s",v,gs_mult[i].sym);
		val -= v * gs_mult[i].val;
	}
#if 0
	;							/* 1023 / 0 = "1023" / "0" */
#else
	if ( p != str && val )		/* 1023 / 0 = NULL */
#endif
	{
		if ( p != str )
			*p++ = '+';
		sprintf(p,"%d",val);
	}
	return *str ? str : NULL;
}

/******************************************************************************/
/*
 * 'elBigIntVal2StrMMultOpt()': Optional 'elBigIntVal2StrMMult()'.
 *
 * For example:
 *	 2,000,000,000 = "1Gi+883Mi+357Ki"
 *	         1,023 = ""
 */
#undef	__FUNC__
#define	__FUNC__	"elBigIntVal2StrMMultOpt"
EL_DLL_API
#if OLD_PROTOTYPES
char *elBigIntVal2StrMMultOpt(fmt,val,binary)
register char *fmt;
register elBigUInt_t val;
bool binary;
#else
char *elBigIntVal2StrMMultOpt(register char *fmt,register elBigUInt_t val,bool binary)
#endif
{
#if 0
	TLS_VAR static char str[128+EL_ST];
#endif
	register char *p;

	p = elBigIntVal2StrMMult(val,binary);
	if ( p )
	{
#if 1
		return elStrTmpFmt(fmt,p);
#else
		sprintf(str,fmt,p);
		return str;
#endif
	}
	else
		return "";
}

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4701)	/* "unreferenced formal parameter" */
#endif
/******************************************************************************/
/*
 * 'elBigIntStr2ValMMult()':
 *
 * Converts string with one or more (multiple) multiples to value.
 *
 * For example: "4Gi-1Mi+100Ki-64" = 4,294,021,056
 */
#undef	__FUNC__
#define	__FUNC__	"elBigIntStr2ValMMult"
EL_DLL_API
#if OLD_PROTOTYPES
bool elBigIntStr2ValMMult(str,pval,p_str_end)
char *str;
elBigUInt_t *pval;
char **p_str_end;
#else
bool elBigIntStr2ValMMult(char *str,elBigUInt_t *pval,char **p_str_end)
#endif
{
	char *p_end;
	elBigUInt_t tmp;
	bool ret;
	register char *p1, *p2;
	register char lop, op;

	*pval = 0;
	lop = '+';
	p1 = str;
	do
	{
#if EL_BIG_INT_STR_HEXPFX_DOLLAR_SIGN
		if ( (p2=strpbrk(p1,"+-")) != NULL )
#else
		if ( (p2=strpbrk(p1,"+-!@#$%^&()_=|\\[]{};:'\",.<>/?`~")) != NULL )
#endif
		{
			op = *p2;
			/*
			 * '*p2' = '\0':
			 *
			 * Terminate operand (+/-) after value w/o unit (e.g. "1+1"), so
			 * that the operand is not interpreted as unit.
			 */
			*p2 = '\0';
		}
		ret = elBigIntStr2ValSMult(p1,&tmp,&p_end);
		if ( p2 )
			*p2++ = op;		/* restore operand in original string */
		if ( !ret )
			return FALSE;	/* 'elBigIntStr2ValSMult()' failed */
#if 0
		LogD("tmp = %" EL_BIG_INT_FMTSZPFX "u",tmp);
#endif
		switch(lop)
		{
		case '+': *pval += tmp; break;
		case '-': *pval -= tmp; break;
		default:
#if 1
			elLog("%s: '%c': invalid operand",str,lop);
#endif
			return FALSE;
			break;
		}
#if 0
		LogD("pval = %" EL_BIG_INT_FMTSZPFX "u",*pval);
#endif
		lop = op;
	} while( (p1 = p2) != NULL );

	if ( p_str_end )
		*p_str_end = p_end;
	
	return TRUE;
}
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

/******************************************************************************/
/*
 * 'elBigIntStr2ValSMult()': Converts string with single multiple to value.
 *
 * For example:
 *	"10Ki"  = 10 * 1024
 *	"0xb*b" = 11 * 512
 *	"$b*b"  = 11 * 512		(if 'EL_BIG_INT_STR_HEXPFX_DOLLAR_SIGN' is enabled)
 */
#undef	__FUNC__
#define	__FUNC__	"elBigIntStr2ValSMult"
EL_DLL_API
#if OLD_PROTOTYPES
bool elBigIntStr2ValSMult(str,pval,p_str_end)
char *str;
register elBigUInt_t *pval;
char **p_str_end;
#else
bool elBigIntStr2ValSMult(char *str,register elBigUInt_t *pval,char **p_str_end)
#endif
{
#if EL_BIG_INT_STR_HEXPFX_DOLLAR_SIGN
	if ( *str == '$' )
	{
		str++;
#if EL_BIG_INT_SIZEOF > 4
		{	/* inner block */
		/*
		 * Because there is no std. routine for hex. string => int64 conversion
		 * (other than 'sscanf("0x...","%I64x",...)'), we will do the conversion
		 * ourselves:
		 */
		char c;
		
		for(*pval = 0; (c = (char)toupper(*str), isxdigit(c)); str++)
		{
			*pval *= 16;
			*pval |= c >= 'A' ? 0xA + c - 'A' : c - '0';
		}
		}	/* inner block */
#else
		*pval = strtoul(str,&str,16);
#endif
	}
	else
	{
#endif	/* EL_BIG_INT_STR_HEXPFX_DOLLAR_SIGN */

#if EL_BIG_INT_STR_BINPFX
	if ( *str == '&' )
	{
		char c;

		str++;
		for(*pval = 0; (c = (*str)) != '\0' && (c == '0' || c == '1'); str++)
		{
			*pval <<= 1;
			if ( c == '1' )
				*pval |= 1;
		}
	}
	else
	{
#endif	/* EL_BIG_INT_STR_BINPFX */

#if WIN && !MQX
	int n;

	/* "%li" - oct/dec/hex */
	switch( sscanf(str,"%" EL_BIG_INT_FMTSZPFX "i%n",pval,&n) )
	{
	case -1:	/* str = "" */
		*pval = 0;
		return TRUE;
	case 0:
#if 0
		elLog("'%s': invalid oct/dec/hex value",str);
#endif
		return FALSE;
	}
	str += n;
#else
	/*
	 * MQX provided 'sscanf()' does not correctly process "%n" (it quits
	 * processing as soon as the input string is exhausted, even before the
	 * "%n" conversion is reached). That's why we must use 'strtoul()' instead:
	 */
	*pval = strtoul(str,&str,0);
#endif

#if EL_BIG_INT_STR_BINPFX
	}	/* if ( *str != '&' ) */
#endif

#if EL_BIG_INT_STR_HEXPFX_DOLLAR_SIGN
	}	/* if ( *str != '$' ) */
#endif

	if ( *str == '*' )	/* value/multiple separator */
		str++;
	if ( *str )
		mult_by_sym(str,pval,&str);
	if ( p_str_end )
		*p_str_end = str;

	return TRUE;
}

/******************************************************************************/
/*
 * 'elBigIntVal2RValSMult()': Converts value to real value using single multiple.
 *
 * For example:
 *	1,048,576 = 1 "Mi"
 *	    1,536 = 1.5 "Ki"
 */
#undef	__FUNC__
#define	__FUNC__	"elBigIntVal2RValSMult"
EL_DLL_API
double elBigIntVal2RValSMult(elBigUInt_t val,bool binary,register char **mult_sym)
{
	register i;
	register double dval;

	for(i = MULTs-1; i >= 0; i--)
		if ( gs_mult[i].binary == binary && val >= gs_mult[i].val )
			break;
	if ( i >= 0 )
	{
		dval = ((double)(elBigInt_t)val / (elBigInt_t)gs_mult[i].val);
		*mult_sym = gs_mult[i].sym;
	}
	else
	{
		dval = (double) (elBigInt_t)val;
		*mult_sym = NULL;	/* one multiple (bytes) */
	}
	return dval;
}

/******************************************************************************/
/*
 * 'elBigIntVal2RStrSMult()':
 *
 * Converts value to real value string using single multiple.
 *
 * For example:
 *	1,048,576 = "1 Mi"
 *	    1,536 = "1.5 Ki"
 */
#undef	__FUNC__
#define	__FUNC__	"elBigIntVal2RStrSMult"
EL_DLL_API
#if OLD_PROTOTYPES
char *elBigIntVal2RStrSMult(val,binary,prec)
elBigUInt_t val;
bool binary;
register prec;
#else
char *elBigIntVal2RStrSMult(elBigUInt_t val,bool binary,register int prec)
#endif
{
	static char str[4+1+6+3];	/* "1023.123456 Ki" */
	char *mult_sym;
	register double dval;

	dval = elBigIntVal2RValSMult(val,binary,&mult_sym);
	sprintf(str,"%.*lf ",prec,dval);
	if ( mult_sym )
		strcat(str,mult_sym);
	return str;
}

/****************************************************************************/
/*
 * 'elBigIntMult()': Returns attributes for multiple given by its index.
 */
#undef	__FUNC__
#define	__FUNC__	"elBigIntMult"
EL_DLL_API
#if OLD_PROTOTYPES
bool elBigIntMult(mult_idx,mult_sym,mult_name,pval)
int mult_idx;
char *mult_sym, *mult_name;
elBigInt_t *pval;
#else
bool elBigIntMult(int mult_idx,char *mult_sym,char *mult_name,elBigInt_t *pval)
#endif
{
	if ( (uint)mult_idx >= EL_ARRAY_CNT(gs_mult) )
		return FALSE;		/* no such multiple */
	
	strcpy(mult_sym, gs_mult[mult_idx].sym);
	strcpy(mult_name,gs_mult[mult_idx].name);
	*pval = gs_mult[mult_idx].val;
	
	return TRUE;
}

/******************************************************************************/
