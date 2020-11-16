/*
 * This program is in public domain; written by Dave G. Conroy.
 * This file contains the main driving routine, and some keyboard processing
 * code, for the MicroEMACS screen editor.
 *
 * REVISION HISTORY:
 *
 * 1.0  Steve Wilhite, 30-Nov-85
 *      - Removed the old LK201 and VT100 logic. Added code to support the
 *        DEC Rainbow keyboard (which is a LK201 layout) using the the Level
 *        1 Console In ROM INT. See "rainbow.h" for the function key defs
 *      Steve Wilhite, 1-Dec-85
 *      - massive cleanup on code in display.c and search.c
 *
 * 2.0  George Jones, 12-Dec-85
 *      - Ported to Amiga.
 *
 * 3.0  Daniel Lawrence, 29-Dec-85
 *      - rebound keys/added new fast buffered I/O for AMIGA
 *	- added META- repeat commands
 *	- added reposition default to center screen (yeah!)
 *	- changed exit with modified buffers message
 *	- made filesave tell us what it is doing
 *	- changed search string entry to terminate with <ESC>
 *	  so we can use <NL> in search/replace strings
 *	- updated version number in mode line to 3.0
 *	12-Jan-86
 *	- Added code to reconize the Search/replace functions
 *	- Added code to perform search/replace & query functions
 *	14-Jan-86
 *	- moved search logic to separate function in search.c
 *	- added replace and query replace functions
 *	- separated out control key expansions to be used by others in search.c
 *	15-Jan-86
 *	- changed "visiting" to finding
 *	- changed yes/no responces to not need return
 *	- cleaned up various messages
 *	16-jan-86
 *	- fixed spurious spawn message in MSDOS
 *	- added ^X-S synonime to save command
 *	- moved escape to shell to ^X-C
 *	21-jan-86
 *	- added code to suspend shell under BSD
 *	22-jan-86
 *	- added function key support (SPEC) under MSDOS
 *	- Abort now prints [Aborted] on message line
 *	23-jan-86
 *	- Added modes and commends to set/unset them
 *	24-jan-86
 *	- Added Goto Line command
 *	- added Rename Buffer command
 *	28-jan-86
 *	- added goto begining and end of paragraph commands (META-P/META-N)
 *	- re-wrote kdelete to use realloc. gained MUCH speed here when
 *	  doing large wipes both on UNIX and MSDOS. Changed kill buffer
 *	  allocation block size from 256 bytes to 1 k
 *	29-jan-86
 *	- moved extern function declarations to efunc.h
 *	- made name[] name binding table
 *	30-jan-86
 *	- fixed Previous/Next paragraph command not to wrap around EOF
 *	- added Fill Paragraph command (META-Q)
 *	4-feb-86
 *	- added code to properly display long lines, scrolling them right
 *	  to left
 *	5-feb-85
 *	- rewrote code to right/left scroll...much better
 *	- added shifted arror keys on IBMPC
 *	6-feb-85
 *	- add option to allow forword-word to jump to begining of
 *	  next word instead of end of current one. This is different from
 *	  other emacs' but can be configured off in estruct.h
 *	- added VIEW mode to allow a buffer to be read only
 *	   (-v switch on command line will activate this)
 *	- changed quick exit to write out ALL changed buffers!!!
 *	  MAKE SURE YOU KNOW THIS WHEN META-Zing
 *	10-feb-86
 *	- added handling of lines longer than allowed on file read in
 *	  (they wrap on additional lines)
 *	- made having space clear the message line and NOT insert itself
 *	  a configuration option in ed.h
 *	11-feb-86
 *	- added Describe-command and Help commands.
 *	13-feb-86
 *	- added View file command (^X ^V) and finished HELP command
 *	14-feb-86
 *	- added option to let main loop skip update if type ahead commands
 *	   are queued up
 *	16-feb-86
 *	- added Insert File command
 *	17-feb-86
 *	- added scroll next window up/down commands
 *	18-feb-86
 *	- added CMODE indentation
 *	- re-arranged header files to standerdize extern and global
 *	  definitions
 *	- changed version number to 3.2
 *	- added numeric arguments to search, reverse search and
 *	  search and replace
 *	24-feb-86
 *	- added Bind To Key function (^C for now) to allow the user
 *	  to change his command keys
 *	- added Unbind key function (M-^C for now)
 *	- added execute named command to execute unbound commands (M-X)
 *	- added describe bindings command (not bound)
 *	- changed version number to 3.3
 *	25-feb-86
 *	- scrapped CERROR mode (too many compilers)
 *	- added EXACT mode for case sensitive searchers
 *	26-feb-86
 *	- added command completion on execute named command and
 *	  all routined grabbing a command name
 *	- adding execute-command-line command and its support functions
 *	  (in preporation for sourcing files)
 *	- added Execute Buffer command
 *	27-feb-86
 *	- added execute(source) file command and added code to automatically
 *	  execute emacs.rc (or .emacsrc on UNIX) before initial read in
 *	- changed version number to 3.4
 *	4-mar-86
 *	- changed word delete to be consistant with word move (it gets
 *	  rid of the inter word space now) This is configurable with the
 *	  NFWORD symbol in estruct.h
 *	- added B_ACTIVE entry to the buffer table. Let emacs read multiple
 *	  file names from the command line and only read them in as needed
 *	5-mar-85
 *	- rewrote command line parser to get rid of my patchy code
 *	- changed version number to 3.5
 *	1-apr-86
 *	- added support for Aztec C 3.20e under MSDOS
 *	- fixed bug in mlwrite on ADM3's and thier ilk under V7
 *	- added insertion of pounds in column one under CMODE
 *	- changed version number to 3.6
 *	3-apr-86
 *	- added next-buffer command (^X-X)
 *	5-apr-86
 *	- added kill paragraph command (M-^W)
 *	- changed fill-paragraph to leave 2 spaces after a period at the
 *	  end of a word.
 *	- added OVERWRITE mode
 *	7-apr-86
 *	- fixed overwrite mode to handle tabs
 *	8-apr-86
 *	- added add/delete global mode (<ESC>M & <ESC> ^M) commands
 *	9-apr-86
 *	- added insert space command
 *	- moved bindings around		^C	insert space
 *					M-K	bind-to-key
 *					INSERT	insert space
 *					DELETE	forwdel
 *	- added hunt forward and hunt reverse commands
 *	10-apr-86
 *	- fixed bug in DOBUF with non-terminated command string
 *	15-apr-86
 *	- fixed tab expansion bug in DISPLAY which hung the AMIGA
 *	  (send in by Dawn Banks)
 *	- fixed curcol problen if forwline/backline during keyboard
 *	  macro execution (sent in by Ernst Christen)
 *	- added AMIGA function/cursor key support
 *	- fixed nonterminating <NL> replacement bug
 *	- fixed word wrapping problems
 *	16-apr-86
 *	- updated documentation and froze development for 3.6 net release
 */

#include        <stdio.h>

/* make global definitions not external */
#define	maindef

#include        "estruct.h"	/* global structures and defines */
#include	"efunc.h"	/* function declarations and name table	*/
#include	"edef.h"	/* global definitions */
#include	"ebind.h"	/* default key bindings */

#if     VMS
#include        <ssdef.h>
#define GOOD    (SS$_NORMAL)
#endif

#ifndef GOOD
#define GOOD    0
#endif

main(argc, argv)
char    *argv[];
{
        register int    c;
        register int    f;
        register int    n;
        register int    mflag;
	register BUFFER *bp;
	register int	ffile;		/* first file flag */
	register int	carg;		/* current arg to scan */
	int basec;			/* c stripped of meta character */
	int viewflag;			/* are we starting in view mode? */
        char bname[NBUFN];		/* buffer name of file to read */

	/* initialize the editor and process the startup file */
        strcpy(bname, "main");	/* default buffer name */
        vtinit();		/* Displays.            */
        edinit(bname);		/* Buffers, windows.    */
	startup();		/* execute .emacsrc if there */
	viewflag = FALSE;
	ffile = TRUE;		/* no file to edit yet */
	update();		/* let the user know we are here */
	
	/* scan through the command line and get the files to edit */
	for (carg = 1; carg < argc; ++carg) {
		/* if its a switch, process it */
		if (argv[carg][0] == '-') {
			switch (argv[carg][1]) {
				case 'v':	/* -v for View File */
				case 'V':
					viewflag = TRUE;
					break;
				case 'e':	/* -e for Edit file */
				case 'E':
					viewflag = FALSE;
					break;
				default:	/* unknown switch */
					/* ignore this for now */
					break;
			}
		} else {	/* process a file name */
			/* set up a buffer for this file */
	                makename(bname, argv[carg]);

			/* if this is the first file, read it in */
			if (ffile) {
				bp = curbp;
				makename(bname, argv[carg]);
				strcpy(bp->b_bname, bname);
				strcpy(bp->b_fname, argv[carg]);
				if (readin(argv[carg], (viewflag==FALSE))
								== ABORT) {
					strcpy(bp->b_bname, "main");
					strcpy(bp->b_fname, "");
				}
				bp->b_dotp = bp->b_linep;
				bp->b_doto = 0;
				ffile = FALSE;
			} else {
				/* set this to inactive */
				bp = bfind(bname, TRUE, 0);
				strcpy(bp->b_fname, argv[carg]);
				bp->b_active = FALSE;
			}

			/* set the view mode appropriatly */
			if (viewflag)
				bp->b_mode |= MDVIEW;
		}
	}

	/* setup to process commands */
        lastflag = 0;                           /* Fake last flags.     */
	curbp->b_mode = curbp->b_mode | gmode;	/* and set default modes*/
	curwp->w_flag |= WFMODE;		/* and force an update	*/

loop:
        update();                               /* Fix up the screen    */
        c = getkey();
        if (mpresf != FALSE) {
                mlerase();
                update();
#if	CLRMSG
                if (c == ' ')                   /* ITS EMACS does this  */
                        goto loop;
#endif
        }
        f = FALSE;
        n = 1;

	/* do META-# processing if needed */

	basec = c & ~META;		/* strip meta char off if there */
	if ((c & META) && ((basec >= '0' && basec <= '9') || basec == '-')) {
		f = TRUE;		/* there is a # arg */
		n = 0;			/* start with a zero default */
		mflag = 1;		/* current minus flag */
		c = basec;		/* strip the META */
		while ((c >= '0' && c <= '9') || (c == '-')) {
			if (c == '-') {
				/* already hit a minus or digit? */
				if ((mflag == -1) || (n != 0))
					break;
				mflag = -1;
			} else {
				n = n * 10 + (c - '0');
			}
			if ((n == 0) && (mflag == -1))	/* lonely - */
				mlwrite("Arg:");
			else
				mlwrite("Arg: %d",n * mflag);

			c = getkey();	/* get the next key */
		}
		n = n * mflag;	/* figure in the sign */
	}

	/* do ^U repeat argument processing */

        if (c == (CTRL|'U')) {                  /* ^U, start argument   */
                f = TRUE;
                n = 4;                          /* with argument of 4 */
                mflag = 0;                      /* that can be discarded. */
                mlwrite("Arg: 4");
                while (((c = getkey()) >= '0' && c <= '9') || c == (CTRL|'U') || c == '-') {
                        if (c == (CTRL|'U'))
                                n = n*4;
                        /*
                         * If dash, and start of argument string, set arg.
                         * to -1.  Otherwise, insert it.
                         */
                        else if (c == '-') {
                                if (mflag)
                                        break;
                                n = 0;
                                mflag = -1;
                        }
                        /*
                         * If first digit entered, replace previous argument
                         * with digit and set sign.  Otherwise, append to arg.
                         */
                        else {
                                if (!mflag) {
                                        n = 0;
                                        mflag = 1;
                                }
                                n = 10*n + c - '0';
                        }
                        mlwrite("Arg: %d", (mflag >=0) ? n : (n ? -n : -1));
                }
                /*
                 * Make arguments preceded by a minus sign negative and change
                 * the special argument "^U -" to an effective "^U -1".
                 */
                if (mflag == -1) {
                        if (n == 0)
                                n++;
                        n = -n;
                }
        }
        if (c == (CTRL|'X'))                    /* ^X is a prefix       */
                c = CTLX | getctl();
        if (kbdmip != NULL) {                   /* Save macro strokes.  */
                if (c!=(CTLX|')') && kbdmip>&kbdm[NKBDM-6]) {
                        ctrlg(FALSE, 0);
                        goto loop;
                }
                if (f != FALSE) {
                        *kbdmip++ = (CTRL|'U');
                        *kbdmip++ = n;
                }
                *kbdmip++ = c;
        }
        execute(c, f, n);                       /* Do it.               */
        goto loop;
}

/*
 * Initialize all of the buffers and windows. The buffer name is passed down
 * as an argument, because the main routine may have been told to read in a
 * file by default, and we want the buffer name to be right.
 */
edinit(bname)
char    bname[];
{
        register BUFFER *bp;
        register WINDOW *wp;

        bp = bfind(bname, TRUE, 0);             /* First buffer         */
        blistp = bfind("[List]", TRUE, BFTEMP); /* Buffer list buffer   */
        wp = (WINDOW *) malloc(sizeof(WINDOW)); /* First window         */
        if (bp==NULL || wp==NULL || blistp==NULL)
                exit(1);
        curbp  = bp;                            /* Make this current    */
        wheadp = wp;
        curwp  = wp;
        wp->w_wndp  = NULL;                     /* Initialize window    */
        wp->w_bufp  = bp;
        bp->b_nwnd  = 1;                        /* Displayed.           */
        wp->w_linep = bp->b_linep;
        wp->w_dotp  = bp->b_linep;
        wp->w_doto  = 0;
        wp->w_markp = NULL;
        wp->w_marko = 0;
        wp->w_toprow = 0;
        wp->w_ntrows = term.t_nrow-1;           /* "-1" for mode line.  */
        wp->w_force = 0;
        wp->w_flag  = WFMODE|WFHARD;            /* Full.                */
}

/*
 * This is the general command execution routine. It handles the fake binding
 * of all the keys to "self-insert". It also clears out the "thisflag" word,
 * and arranges to move it to the "lastflag", so that the next command can
 * look at it. Return the status of command.
 */
execute(c, f, n)
{
        register KEYTAB *ktp;
        register int    status;

        ktp = &keytab[0];                       /* Look in key table.   */
        while (ktp->k_fp != NULL) {
                if (ktp->k_code == c) {
                        thisflag = 0;
                        status   = (*ktp->k_fp)(f, n);
                        lastflag = thisflag;
                        return (status);
                }
                ++ktp;
        }

        /*
         * If a space was typed, fill column is defined, the argument is non-
         * negative, wrap mode is enabled, and we are now past fill column,
	 * and we are not read-only, perform word wrap.
         */
        if (c == ' ' && (curwp->w_bufp->b_mode & MDWRAP) && fillcol > 0 &&
	    n >= 0 && getccol(FALSE) > fillcol &&
	    (curwp->w_bufp->b_mode & MDVIEW) == FALSE)
                wrapword();

        if ((c>=0x20 && c<=0x7E)                /* Self inserting.      */
        ||  (c>=0xA0 && c<=0xFE)) {
                if (n <= 0) {                   /* Fenceposts.          */
                        lastflag = 0;
                        return (n<0 ? FALSE : TRUE);
                }
                thisflag = 0;                   /* For the future.      */

		/* if we are in overwrite mode, not at eol,
		   and next char is not a tab or we are at a tab stop,
		   delete a char forword			*/
		if (curwp->w_bufp->b_mode & MDOVER &&
		    curwp->w_doto < curwp->w_dotp->l_used &&
			(lgetc(curwp->w_dotp, curwp->w_doto) != '\t' ||
			 (curwp->w_doto) % 8 == 7))
				ldelete(1, FALSE);

		/* do the appropriate insertion */
		if (c == '}' && (curbp->b_mode & MDCMOD) != 0)
	        	status = insbrace(n, c);
	        else if (c == '#' && (curbp->b_mode & MDCMOD) != 0)
	        	status = inspound();
	        else
	                status = linsert(n, c);

                lastflag = thisflag;
                return (status);
        }
	mlwrite("\007[Key not bound]");		/* complain		*/
        lastflag = 0;                           /* Fake last flags.     */
        return (FALSE);
}

/*
 * Read in a key.
 * Do the standard keyboard preprocessing. Convert the keys to the internal
 * character set.
 */
getkey()
{
	int    c;
#if	AMIGA
	int	d;
#endif

        c = (*term.t_getchar)();

#if RAINBOW

        if (c & Function_Key)
                {
                int i;

                for (i = 0; i < lk_map_size; i++)
                        if (c == lk_map[i][0])
                                return lk_map[i][1];
                }
        else if (c == Shift + 015) return CTRL | 'J';
        else if (c == Shift + 0x7F) return META | 0x7F;
#endif

#if	MSDOS
	if (c == 0) {				/* Apply SPEC prefix	*/
		c = getkey();
		return(SPEC | c);
	}
#endif

#if	AMIGA
	/* apply SPEC prefix */
	if ((unsigned)c == 155) {
		c = (*term.t_getchar)();

		/* first try to see if it is a cursor key */
		if ((c >= 'A' && c <= 'D') || c == 'S' || c == 'T')
			return(SPEC | c);

		/* next, a 2 char sequence */
		d = (*term.t_getchar)();
		if (d == '~')
			return(SPEC | c);

		/* decode a 3 char sequence */
		c = d + 32;
		/* if a shifted function key, eat the tilde */
		if (d >= '0' && d <= '9')
			d = (*term.t_getchar)();
		return(SPEC | c);
	}
#endif

        if (c == METACH) {                      /* Apply M- prefix      */
                c = getctl();
                return (META | c);
        }

        if (c>=0x00 && c<=0x1F)                 /* C0 control -> C-     */
                c = CTRL | (c+'@');
        return (c);
}

/*
 * Get a key.
 * Apply control modifications to the read key.
 */
getctl()
{
        register int    c;

        c = (*term.t_getchar)();
        if (c>='a' && c<='z')                   /* Force to upper       */
                c -= 0x20;
        if (c>=0x00 && c<=0x1F)                 /* C0 control -> C-     */
                c = CTRL | (c+'@');
        return (c);
}

/*
 * Fancy quit command, as implemented by Norm. If the any buffer has
 * changed do a write on that buffer and exit emacs, otherwise simply exit.
 */
quickexit(f, n)
{
	register BUFFER *bp;	/* scanning pointer to buffers */

	bp = bheadp;
	while (bp != NULL) {
	        if ((bp->b_flag&BFCHG) != 0	/* Changed.             */
        	&& (bp->b_flag&BFTEMP) == 0) {	/* Real.                */
			curbp = bp;		/* make that buffer cur	*/
			mlwrite("[Saving %s]",bp->b_fname);
                	filesave(f, n);
		}
	bp = bp->b_bufp;			/* on to the next buffer */
	}
        quit(f, n);                             /* conditionally quit   */
}

/*
 * Quit command. If an argument, always quit. Otherwise confirm if a buffer
 * has been changed and not written out. Normally bound to "C-X C-C".
 */
quit(f, n)
{
        register int    s;

        if (f != FALSE                          /* Argument forces it.  */
        || anycb() == FALSE                     /* All buffers clean.   */
						/* User says it's OK.   */
        || (s=mlyesno("Modified buffers exist. Leave anyway")) == TRUE) {
#if	FILOCK
		if (lockrel() != TRUE) {
			(*term.t_putchar)('\n');
			(*term.t_putchar)('\r');
			(*term.t_close)();
			exit(1);
		}
#endif
                vttidy();
                exit(GOOD);
        }
	mlwrite("");
        return (s);
}

/*
 * Begin a keyboard macro.
 * Error if not at the top level in keyboard processing. Set up variables and
 * return.
 */
ctlxlp(f, n)
{
        if (kbdmip!=NULL || kbdmop!=NULL) {
                mlwrite("Not now");
                return (FALSE);
        }
        mlwrite("[Start macro]");
        kbdmip = &kbdm[0];
        return (TRUE);
}

/*
 * End keyboard macro. Check for the same limit conditions as the above
 * routine. Set up the variables and return to the caller.
 */
ctlxrp(f, n)
{
        if (kbdmip == NULL) {
                mlwrite("Not now");
                return (FALSE);
        }
        mlwrite("[End macro]");
        kbdmip = NULL;
        return (TRUE);
}

/*
 * Execute a macro.
 * The command argument is the number of times to loop. Quit as soon as a
 * command gets an error. Return TRUE if all ok, else FALSE.
 */
ctlxe(f, n)
{
        register int    c;
        register int    af;
        register int    an;
        register int    s;

        if (kbdmip!=NULL || kbdmop!=NULL) {
                mlwrite("Not now");
                return (FALSE);
        }
        if (n <= 0)
                return (TRUE);
        do {
                kbdmop = &kbdm[0];
                do {
                        af = FALSE;
                        an = 1;
                        if ((c = *kbdmop++) == (CTRL|'U')) {
                                af = TRUE;
                                an = *kbdmop++;
                                c  = *kbdmop++;
                        }
                        s = TRUE;
                } while (c!=(CTLX|')') && (s=execute(c, af, an))==TRUE);
                kbdmop = NULL;
        } while (s==TRUE && --n);
        return (s);
}

/*
 * Abort.
 * Beep the beeper. Kill off any keyboard macro, etc., that is in progress.
 * Sometimes called as a routine, to do general aborting of stuff.
 */
ctrlg(f, n)
{
        (*term.t_beep)();
        if (kbdmip != NULL) {
                kbdm[0] = (CTLX|')');
                kbdmip  = NULL;
        }
	mlwrite("[Aborted]");
        return (ABORT);
}

/* tell the user that this command is illegal while we are in
   VIEW (read-only) mode				*/

rdonly()

{
	(*term.t_beep)();
	mlwrite("[Key illegal in VIEW mode]");
	return(FALSE);
}
