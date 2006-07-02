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

#include "pdcx11.h"

RCSID("$Id: pdcscrn.c,v 1.24 2006/07/02 19:03:59 wmcbrine Exp $");

bool GLOBAL_sb_on = FALSE;
bool GLOBAL_slk_on = FALSE;

/*man-start**************************************************************

  PDC_scr_close()	- Internal low-level binding to close the
			  physical screen

  PDCurses Description:
	This is a nop for the DOS platform.

  PDCurses Return Value:
	This function returns OK on success, otherwise an ERR is returned.

  PDCurses Errors:
	The DOS platform will never fail.

  Portability:
	PDCurses  int PDC_scr_close(void);

**man-end****************************************************************/

int PDC_scr_close(void)
{
	PDC_LOG(("PDC_scr_close() - called\n"));

	return OK;
}

/*man-start**************************************************************

  PDC_scr_open()	- Internal low-level binding to open the physical
			  screen

  PDCurses Description:
	This is a NOP in DOS.

  PDCurses Return Value:
	This function returns OK on success, otherwise an ERR is returned.

  PDCurses Errors:
	The DOS platform will never fail.

  Portability:
	PDCurses  int PDC_scr_open(SCREEN *internal, bool echo);

**man-end****************************************************************/

int PDC_scr_open(SCREEN *internal, bool echo)
{
	extern bool sb_started;

	PDC_LOG(("PDC_scr_open() - called\n"));

	internal->cursrow = internal->curscol = 0;
	internal->autocr	= TRUE;		/* cr -> lf by default        */
	internal->raw_out	= FALSE;	/* tty I/O modes	      */
	internal->raw_inp	= FALSE;	/* tty I/O modes	      */
	internal->cbreak	= TRUE;
	internal->save_key_modifiers	= FALSE;
	internal->return_key_modifiers	= FALSE;
	internal->echo		= echo;
	internal->visible_cursor= TRUE;		/* Assume that it is visible  */
	internal->cursor	= 0;
	internal->adapter	= 0;
	internal->audible	= TRUE;
	internal->visibility	= 1;
	internal->orig_cursor = internal->cursor;
	internal->orig_attr = FALSE;
	internal->orgcbr = 0;
	internal->blank = ' ';
	internal->resized = FALSE;
	internal->shell = FALSE;
	internal->_trap_mbe = 0L;
	internal->_map_mbe_to_key = 0L;
	internal->linesrippedoff = 0;
	internal->linesrippedoffontop = 0;
	internal->delaytenths = 0;
	internal->sb_on = sb_started;
	internal->sb_total_y = 0;
	internal->sb_viewport_y = 0;
	internal->sb_cur_y = 0;
	internal->sb_total_x = 0;
	internal->sb_viewport_x = 0;
	internal->sb_cur_x = 0;
	internal->line_color = COLOR_WHITE;

	return OK;
}

/*man-start**************************************************************

  PDC_resize_screen()	- Internal low-level function to resize screen

  PDCurses Description:
	This function provides a means for the application program to
	resize the overall dimensions of the screen.  Under DOS and OS/2
	the application can tell PDCurses what size to make the screen;
	under X11, resizing is done by the user and this function simply
	adjusts its internal structures to fit the new size.

  PDCurses Return Value:
	This function returns OK on success, otherwise an ERR is returned.

  PDCurses Errors:

  Portability:
	PDCurses  int PDC_resize_screen(int, int);

**man-end****************************************************************/

int PDC_resize_screen(int nlines, int ncols)
{
	PDC_LOG(("PDC_resize_screen() - called. Lines: %d Cols: %d\n",
		nlines, ncols));

	shmdt((char *)Xcurscr);
	XCursesInstructAndWait(CURSES_RESIZE);

	if ((shmid_Xcurscr = shmget(shmkey_Xcurscr,
		SP->XcurscrSize + XCURSESSHMMIN, 0700)) < 0)
	{
		perror("Cannot allocate shared memory for curscr");
		kill(otherpid, SIGKILL);
		return ERR;
	}

	XCursesLINES = SP->lines;
	XCursesCOLS = SP->cols;

	PDC_LOG(("%s:shmid_Xcurscr %d shmkey_Xcurscr %d SP->lines %d "
		"SP->cols %d\n", XCLOGMSG, shmid_Xcurscr, 
		shmkey_Xcurscr, SP->lines, SP->cols));

	Xcurscr = (unsigned char*)shmat(shmid_Xcurscr, 0, 0);
	atrtab = (unsigned char *)(Xcurscr + XCURSCR_ATRTAB_OFF);

	SP->resized = FALSE;

	return OK;
}
