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

RCSID("$Id: insstr.c,v 1.30 2006/10/08 20:54:30 wmcbrine Exp $");

/*man-start**************************************************************

  Name:                                                        insstr

  Synopsis:
	int insstr(const char *str);
	int insnstr(const char *str, int n);
	int winsstr(WINDOW *win, const char *str);
	int winsnstr(WINDOW *win, const char *str, int n);
	int mvinsstr(int y, int x, const char *str);
	int mvinsnstr(int y, int x, const char *str, int n);
	int mvwinsstr(WINDOW *win, int y, int x, const char *str);
	int mvwinsnstr(WINDOW *win, int y, int x, const char *str, int n);

	int ins_wstr(const wchar_t *wstr);
	int ins_nwstr(const wchar_t *wstr, int n);
	int wins_wstr(WINDOW *win, const wchar_t *wstr);
	int wins_nwstr(WINDOW *win, const wchar_t *wstr, int n);
	int mvins_wstr(int y, int x, const wchar_t *wstr);
	int mvins_nwstr(int y, int x, const wchar_t *wstr, int n);
	int mvwins_wstr(WINDOW *win, int y, int x, const wchar_t *wstr);
	int mvwins_nwstr(WINDOW *win, int y, int x, const wchar_t *wstr, int n);

  System V Curses Description:
	With these routines, a character string (as many characters as 
	will fit on the line) is inserted before the character under 
	the cursor.  All characters to the right of the cursor are moved 
	to the right, with the possibility of the rightmost characters 
	on the line being lost.  The cursor position does not change 
	(after moving to y,x if specified).  The four routines with n as 
	the last argument insert at most n characters.  If n<=0, then 
	the entire string is inserted.

	NOTE: insstr(), insnstr(), mvinsstr(), mvinsnstr(), mvwinsstr() 
	and mvwinsnstr() are implemented as macros.

  X/Open Return Value:
	All functions return OK on success and ERR on error.

  Portability				     X/Open    BSD    SYS V
					     Dec '88
	insstr					-	-      4.0
	winsstr					-	-      4.0
	mvinsstr				-	-      4.0
	mvwinsstr				-	-      4.0
	insnstr					-	-      4.0
	winsnstr				-	-      4.0
	mvinsnstr				-	-      4.0
	mvwinsnstr				-	-      4.0

**man-end****************************************************************/

int insstr(const char *str)
{
	PDC_LOG(("insstr() - called: string=\"%s\"\n", str));

	return winsnstr(stdscr, str, -1);
}

int insnstr(const char *str, int n)
{
	PDC_LOG(("insnstr() - called: string=\"%s\" n %d \n", str, n));

	return winsnstr(stdscr, str, n);
}

int winsstr(WINDOW *win, const char *str)
{
	PDC_LOG(("winsstr() - called: string=\"%s\"\n", str));

	return winsnstr(win, str, -1);
}

int winsnstr(WINDOW *win, const char *str, int n)
{
	int ic;

	PDC_LOG(("winsnstr() - called: string=\"%s\" n %d \n", str, n));

	if (win == (WINDOW *)NULL || str == (const char *)NULL)
		return ERR;

	ic = strlen(str);

	if (n > 0)
		ic = ((ic < n) ? ic : n) - 1;
	else
		--ic;

	for (; ic >= 0; ic--)
		if (winsch(win, str[ic]) == ERR)
			return ERR;

	return OK;
}

int mvinsstr(int y, int x, const char *str)
{
	PDC_LOG(("mvinsstr() - called: y %d x %d string=\"%s\"\n",
		y, x, str));

	if (move(y, x) == ERR)
		return ERR;

	return winsnstr(stdscr, str, -1);
}

int mvinsnstr(int y, int x, const char *str, int n)
{
	PDC_LOG(("mvinsnstr() - called: y %d x %d string=\"%s\" n %d \n",
		y, x, str, n));

	if (move(y, x) == ERR)
		return ERR;

	return winsnstr(stdscr, str, n);
}

int mvwinsstr(WINDOW *win, int y, int x, const char *str)
{
	PDC_LOG(("mvwinsstr() - called: string=\"%s\"\n", str));

	if (wmove(win, y, x) == ERR)
		return ERR;

	return winsnstr(win, str, -1);
}

int mvwinsnstr(WINDOW *win, int y, int x, const char *str, int n)
{
	PDC_LOG(("mvwinsnstr() - called: y %d x %d string=\"%s\" n %d \n",
		y, x, str, n));

	if (wmove(win, y, x) == ERR)
		return ERR;

	return winsnstr(win, str, n);
}

#ifdef PDC_WIDE
int ins_wstr(const wchar_t *wstr)
{
	PDC_LOG(("ins_wstr() - called\n"));

	return wins_nwstr(stdscr, wstr, -1);
}

int ins_nwstr(const wchar_t *wstr, int n)
{
	PDC_LOG(("ins_nwstr() - called\n"));

	return wins_nwstr(stdscr, wstr, n);
}

int wins_wstr(WINDOW *win, const wchar_t *wstr)
{
	PDC_LOG(("wins_wstr() - called\n"));

	return wins_nwstr(win, wstr, -1);
}

int wins_nwstr(WINDOW *win, const wchar_t *wstr, int n)
{
	const wchar_t *p;
	int ic;

	PDC_LOG(("wins_nwstr() - called\n"));

	if (win == (WINDOW *)NULL || wstr == (const wchar_t *)NULL)
		return ERR;

	for (ic = 0, p = wstr; *p != L'\0'; p++)
		ic++;

	if (n > 0)
		ic = ((ic < n) ? ic : n) - 1;
	else
		--ic;

	for (; ic >= 0; ic--)
		if (winsch(win, wstr[ic]) == ERR)
			return ERR;

	return OK;
}

int mvins_wstr(int y, int x, const wchar_t *wstr)
{
	PDC_LOG(("mvins_wstr() - called\n"));

	if (move(y, x) == ERR)
		return ERR;

	return wins_nwstr(stdscr, wstr, -1);
}

int mvins_nwstr(int y, int x, const wchar_t *wstr, int n)
{
	PDC_LOG(("mvinsnstr() - called\n"));

	if (move(y, x) == ERR)
		return ERR;

	return wins_nwstr(stdscr, wstr, n);
}

int mvwins_wstr(WINDOW *win, int y, int x, const wchar_t *wstr)
{
	PDC_LOG(("mvwinsstr() - called\n"));

	if (wmove(win, y, x) == ERR)
		return ERR;

	return wins_nwstr(win, wstr, -1);
}

int mvwins_nwstr(WINDOW *win, int y, int x, const wchar_t *wstr, int n)
{
	PDC_LOG(("mvwinsnstr() - called\n"));

	if (wmove(win, y, x) == ERR)
		return ERR;

	return wins_nwstr(win, wstr, n);
}
#endif
