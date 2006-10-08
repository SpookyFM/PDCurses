/************************************************************************ 
 * This file is part of PDCurses. PDCurses is public domain software;	*
 * you may use it for any purpose. This software is provided AS IS with	*
 * NO WARRANTY whatsoever.						*
 *									*
 * If you use PDCurses in an application, an acknowledgement would be	*
 * appreciated, but is not mandatory. If you make corrections or	*
 * enhancements to PDCurses, please forward them to the current		*
 * maintainer for the benefit of other users.				*
 *									*
 * No distribution of modified PDCurses code may be made under the name	*
 * "PDCurses", except by the current maintainer. (Although PDCurses is	*
 * public domain, the name is a trademark.)				*
 *									*
 * See the file maintain.er for details of the current maintainer.	*
 ************************************************************************/

#include <curspriv.h>
#include <string.h>
#include <limits.h>

RCSID("$Id: termattr.c,v 1.40 2006/10/08 20:54:30 wmcbrine Exp $");

/*man-start**************************************************************

  Name:                                                     termattr

  Synopsis:
	int baudrate(void);
	char erasechar(void);
	bool has_ic(void);
	bool has_il(void);
	char killchar(void);
	char *longname(void);
	chtype termattrs(void);
	attr_t term_attrs(void);
	char *termname(void);

	char wordchar(void);

	int erasewchar(wchar_t *ch);
	int killwchar(wchar_t *ch);

  X/Open Description:
	baudrate() returns the output speed of the terminal.  The number 
	returned is bits per second, for example 9600, and is an integer.

	The user's current ERASE character is returned from a call to
	erasechar();

	The has_ic function will return TRUE if the terminal has insert 
	character and delete character capabilities.

	The has_il() function will return TRUE if the terminal has insert
	line and delete line capabilities.

	The user's current KILL character is returned from a call to
	killchar();

	longname() returns a pointer to a static area containing a
	verbose description of the current terminal.  The maximum length
	of the string is 128 characters.  It is defined only after the
	call to initscr() or newterm().  The area is overwritten by each
	call to newterm() and is not restored by set_term().  The value
	should therefore be saved between calls to newterm(), if
	longname() is going to be used with multiple terminals.

	termname() returns a pointer to a static area containing a
	short description of the current terminal (14 characters).

	termattrs() returns a logical OR of all video attributes
	supported by the terminal.

  PDCurses Description:
	baudrate() returns the largest possible (portable) int value 
	(INT_MAX from limits.h).

	erasechar(), killchar() and wordchar() all return values that 
	are hardcoded at this time.  There may be future development to 
	allow applications an easy way to specify these value.

	has_ic() and has_il() always return TRUE.

	In addition to the above definition for longname(), the form of 
	this string includes the text resolution.

	The user's current WORD character is returned from a call to
	wordchar().

  X/Open Return Value:
	All functions return OK on success and ERR on error.

  Portability				     X/Open    BSD    SYS V
					     Dec '88
	baudrate				Y	Y	Y
	erasechar				Y	Y	Y
	has_ic					Y	Y	Y
	has_il					Y	Y	Y
	killchar				Y	Y	Y
	longname				Y	Y	Y
	termattrs				Y	Y	Y
	termname				Y	Y	Y
	wordchar				-	-	-

**man-end****************************************************************/

int baudrate(void)
{
	PDC_LOG(("baudrate() - called\n"));

	return INT_MAX;
}

char erasechar(void)
{
	PDC_LOG(("erasechar() - called\n"));

	return _ECHAR;		/* character delete char (^H) */
}

bool has_ic(void)
{
	PDC_LOG(("has_ic() - called\n"));

	return TRUE;
}

bool has_il(void)
{
	PDC_LOG(("has_il() - called\n"));

	return TRUE;
}

char killchar(void)
{
	PDC_LOG(("killchar() - called\n"));

	return _DLCHAR;		/* line delete char (^U) */
}

char *longname(void)
{
	static char _display[128];

	PDC_LOG(("longname() - called\n"));

	sprintf(_display, "PDCurses for %s %s-%dx%d", PDC_sysname(),
		SP->mono ? "MONO" : "COLOR", LINES, COLS);

	return _display;
}

chtype termattrs(void)
{
	chtype temp = A_BLINK | A_BOLD | A_INVIS | A_REVERSE | A_UNDERLINE;

	/* note: blink is bold background on some platforms */

	PDC_LOG(("termattrs() - called\n"));

	if (!SP->mono)
		temp |= A_COLOR;

	return temp;
}

attr_t term_attrs(void)
{
	PDC_LOG(("term_attrs() - called\n"));

	return WA_BLINK | WA_BOLD | WA_INVIS | WA_LEFT |
		WA_REVERSE | WA_RIGHT | WA_UNDERLINE;
}

char *termname(void)
{
	PDC_LOG(("termname() - called\n"));

	return "pdcurses";
}

char wordchar(void)
{
	PDC_LOG(("wordchar() - called\n"));

	return _DWCHAR;			/* word delete char */
}

#ifdef PDC_WIDE
int erasewchar(wchar_t *ch)
{
	PDC_LOG(("erasewchar() - called\n"));

	if (ch == (wchar_t *)NULL)
		return ERR;

	*ch = (wchar_t)_ECHAR;

	return OK;
}

int killwchar(wchar_t *ch)
{
	PDC_LOG(("killwchar() - called\n"));

	if (ch == (wchar_t *)NULL)
		return ERR;

	*ch = (wchar_t)_DLCHAR;

	return OK;
}
#endif
