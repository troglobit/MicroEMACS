/*
 * The routines in this file
 * provide support for VT52 style terminals
 * over a serial line. The serial I/O services are
 * provided by routines in "termio.c". It compiles
 * into nothing if not a VT52 style device. The
 * bell on the VT52 is terrible, so the "beep"
 * routine is conditionalized on defining BEL.
 */
#define	termdef	1			/* don't define "term" external */

#include        <stdio.h>
#include        "estruct.h"
#include	"edef.h"

#if     VT52

#define NROW    24                      /* Screen size.                 */
#define NCOL    80                      /* Edit if you want to.         */
#define	MARGIN	8			/* size of minimim margin and	*/
#define	SCRSIZ	64			/* scroll size for extended lines */
#define BIAS    0x20                    /* Origin 0 coordinate bias.    */
#define ESC     0x1B                    /* ESC character.               */
#define BEL     0x07                    /* ascii bell character         */

extern  int     ttopen();               /* Forward references.          */
extern  int     ttgetc();
extern  int     ttputc();
extern  int     ttflush();
extern  int     ttclose();
extern  int     vt52move();
extern  int     vt52eeol();
extern  int     vt52eeop();
extern  int     vt52beep();
extern  int     vt52open();
extern	int	vt52rev();

/*
 * Dispatch table. All the
 * hard fields just point into the
 * terminal I/O code.
 */
TERM    term    = {
        NROW-1,
        NCOL,
	MARGIN,
	SCRSIZ,
        &vt52open,
        &ttclose,
        &ttgetc,
        &ttputc,
        &ttflush,
        &vt52move,
        &vt52eeol,
        &vt52eeop,
        &vt52beep,
        &vt52rev
};

vt52move(row, col)
{
        ttputc(ESC);
        ttputc('Y');
        ttputc(row+BIAS);
        ttputc(col+BIAS);
}

vt52eeol()
{
        ttputc(ESC);
        ttputc('K');
}

vt52eeop()
{
        ttputc(ESC);
        ttputc('J');
}

vt52rev(status)	/* set the reverse video state */

int status;	/* TRUE = reverse video, FALSE = normal video */

{
	/* can't do this here, so we won't */
}

vt52beep()
{
#ifdef  BEL
        ttputc(BEL);
        ttflush();
#endif
}

#endif

vt52open()
{
#if     V7
        register char *cp;
        char *getenv();

        if ((cp = getenv("TERM")) == NULL) {
                puts("Shell variable TERM not defined!");
                exit(1);
        }
        if (strcmp(cp, "vt52") != 0 && strcmp(cp, "z19") != 0) {
                puts("Terminal type not 'vt52'or 'z19' !");
                exit(1);
        }
#endif
        ttopen();
}
