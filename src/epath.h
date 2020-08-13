/*	PATH:	This file contains certain info needed to locate the
		MicroEMACS files on a system dependant basis.

									*/

/*	possible names and paths of help files under different OSs	*/

char *pathname[] = {

#ifdef	AMIGA
	".emacsrc",
	"emacs.hlp",
	"",
	":c/",
	":t/"
#endif

#ifdef	MSDOS
	"emacs.rc",
	"emacs.hlp",
	"\\sys\\public\\",
	"\\usr\\bin\\",
	"\\bin\\",
	"\\",
	""
#endif

#ifdef	V7
	".emacsrc",
	"emacs.hlp",
	"/usr/local/",
	"/usr/lib/",
	""
#endif

#ifdef	VMS
	"emacs.rc",
	"emacs.hlp",
	"",
	"sys$sysdevice:[vmstools]"
#endif

#if defined(BSD) || defined(linux)
	".emacsrc",
	"emacs.hlp",
	SYS_PATH "/",
	"/usr/local/",
	"/usr/lib/",
	""
#endif
};

#define	NPNAMES	(sizeof(pathname)/sizeof(char *))
