// *************************************MODULE HEADER*********************************
// * FILENAME           : main.c
// * MODULE TITLE       : 
// * PROJECT            : Zex Registration
// * DATE STARTED       : 23:32:05 on 2nd July 1999
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************


// ***********************************************************************************
// * INCLUDED FILES
// *
//#include <Desk.h>
#include <DiskInit.h>
#include <Dialogs.h>
#include <Files.h>
#include <Fonts.h>
#include <Lists.h>
#include <Menus.h>
#include <Resources.h>
#include <Memory.h>
#include <OSUtils.h>
#include <Events.h>
#include <Packages.h>
#include <Scrap.h>
#include <Script.h>
#include <SegLoad.h>
//#include <StdArg.h>
//#include <stdio.h>
#include <ToolUtils.h>
#include <NumberFormatting.h>

#define thisDialogID 128

/* Symbolic Dialog Item Numbers */

static enum {
	BUT1_OK = 1,
	STXT2_Name,
	EDIT3,
	STXT4_Code,
	EDIT5,
	BUT6_Calculate,
	LASTITEM
	} DialogItemNumber;

#define OK_ITEM 	BUT1_OK
#define CANCEL_ITEM 	?? /* Define this to be item that cancels dialog */

/* Useful constants */

#ifndef ENTERkey
#define ENTERkey	0x3
#endif
#ifndef DELETEkey
#define DELETEkey	0x8
#endif
#ifndef NIL
#define NIL ((void *)0)
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef FRONT_WINDOW
#define FRONT_WINDOW  ((WindowPtr) (-1L))
#endif

/* Prototypes */

int       DoDialog(void);
DialogPtr OpenThisDialog(void);
void      CloseThisDialog(DialogPtr dlog);
void      DoDialogUpdate(DialogPtr dlog);
void      DoDialogActivate(DialogPtr dlog, int activ);
void      DoDialogContent(DialogPtr dlog, EventRecord *evt);
int       DoDialogItem(DialogPtr dlog, short itemHit);

pascal  Boolean MyFilter(DialogPtr dlog, EventRecord *evt, short *itemHit);
Boolean CheckUserItems(Point where, short *itemHit);
int     AnyBadValues(DialogPtr dlog);

void    CenterWindow(WindowPtr w, short top);
long    strlen(char *);
char   *strcpy(char *dst, char *src);
char   *PascalToC(unsigned char *pstr);
unsigned char   *CToPascal(char *cstr);
void    PutDlgString(DialogPtr dlog, int item, unsigned char *str, int sel);
void    PutDlgWord(DialogPtr dlog, int item, int val, int sel);
void    PutDlgLong(DialogPtr dlog, int item, long val, int sel);
void    PutDlgChkRadio(DialogPtr dlog, int item, int val);
int     GetDlgString(DialogPtr dlog, int item, unsigned char *str);
int     GetDlgWord(DialogPtr dlog, int item, short *val);
int     GetDlgLong(DialogPtr dlog, int item, long *val);
int     GetDlgChkRadio(DialogPtr dlog, int item);
int     TextSelected(DialogPtr dlog);
OSType  CanPaste(int n, ...);
void    FrameDefault(DialogPtr dlog, int item, int frame);
void    GetDlgPanel(DialogPtr dlog, int item, Rect *panel);

static Point where;
static int modifiers;




// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *
QDGlobals qd;


// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *
void InitToolbox(void);
main();


void InitToolbox (void)
{
	InitGraf(&qd.thePort);		// Initialize QuickDraw variables for our program.
	InitFonts();				// Initialize fonts.
	FlushEvents(everyEvent, 0);	// Clear event queue of any pending events.
	InitWindows();				// Initialize the Window Manager.
	InitMenus();				// Ditto for the Menu Manager.
	TEInit();					// blah, blah Text Edit.
	InitDialogs(0L);			// blah, blah Dialog Manager.
	InitCursor();				// Set the cursor to the arrow cursor and init.
	
	MaxApplZone();				// Maximum heap size
}





main()
{
  InitToolbox();
  DoDialog();
}




int DoDialog()
	{
		short itemHit,okay=FALSE,keepGoing=TRUE;
		
		DialogPtr dlog=NIL; GrafPtr oldPort;
		ModalFilterUPP MyFilterUPP;

		GetPort(&oldPort);

		/* On PowerPC, need a RoutineDescriptor from heap; on 68K, no allocation */
		
		MyFilterUPP = NewModalFilterProc(MyFilter);
		if (MyFilterUPP == NIL) goto cleanUp;

		/* Build dialog window and install its item values */
		
		dlog = OpenThisDialog();
		if (dlog == NIL) goto cleanUp;

		/* Entertain filtered user events until dialog is dismissed */
next_loop:
		while (keepGoing) {
			ModalDialog(MyFilterUPP,&itemHit);

			keepGoing = DoDialogItem(dlog,itemHit);
			}
		
		/*
		 *	Do final processing of item values, such as exporting them to caller.
		 *	DoDialogItem() has already called AnyBadValues().
		 */
		
		okay = itemHit;
		
		
        
        
		if (okay) {		/* Or whatever is equivalent */
			}

		/* That's all, folks! */

cleanUp:		
		if (dlog) CloseThisDialog(dlog);
		if (MyFilterUPP) DisposeRoutineDescriptor(MyFilterUPP);
		SetPort(oldPort);
		
		return(okay);
	}

/*
 *	We have to have a filter function, at the very least so that we can outline
 *	any default button, entertain keyboard editing commands, cmd-period canceling, etc.
 *	Note that you do not need to have a special user item covering the default button
 *	in your dialog item list.
 */

static pascal Boolean MyFilter(DialogPtr dlog, EventRecord *evt, short *itemHit)
	{
		Boolean ans=FALSE,doHilite=FALSE; WindowPtr w;
		short type,ch; Handle hndl; Rect box;
		static long then; static Point clickPt;
		
		w = (WindowPtr)(evt->message);
		switch(evt->what) {
			case updateEvt:
				if (w == dlog) {
					/* Update our dialog contents */
					DoDialogUpdate(dlog);
					ans = TRUE; *itemHit = 0;
					}
				 else {
					/*
					 *	Call your main event loop DoUpdate(w) routine here if you
					 *	don't want unsightly holes in background windows caused
					 *	by nested alerts, balloon help, or screen savers (see
					 *	Tech Note #304).
					 */
					}
				break;
			case activateEvt:
				if (w == dlog) {
					DoDialogActivate(dlog,(evt->modifiers & activeFlag)!=0);
					*itemHit = 0;
					}
				 else {
					/*
					 *	Call your main event loop DoActivate(w) routine here if
					 *	you want to deactivate the former frontmost window, in order
					 *	to unhighlight any selection, scroll bars, etc.
					 */
					}
				break;
			case mouseDown:
			case mouseUp:
				where = evt->where;		/* Make info available to DoDialog() */
				GlobalToLocal(&where);
				modifiers = evt->modifiers;
				ans = CheckUserItems(where,itemHit);
				break;
			case keyDown:
				if ((ch=(unsigned char)evt->message)=='\r' || ch==ENTERkey) {
					*itemHit = OK_ITEM /* Default Item Number here */;
					doHilite = ans = TRUE;
					}
				 else if (evt->modifiers & cmdKey) {
					ch = (unsigned char)evt->message;
					switch(ch) {
						case 'x':
						case 'X':
							if (TextSelected(dlog))
								{ SystemEdit(3); ZeroScrap(); DialogCut(dlog); TEToScrap(); }
							 else {
								/* Cut from anything else cuttable, like a list */
								}
							break;
						case 'c':
						case 'C':
							if (TextSelected(dlog))
								{ SystemEdit(3); ZeroScrap(); DialogCopy(dlog); TEToScrap(); }
							 else {
								/* Copy from anything else copyable, like a list */
								}
							break;
						case 'v':
						case 'V':
							if (CanPaste(1,'TEXT'))
								{ TEFromScrap(); DialogPaste(dlog); }
							 else {
							 	/* Deal with any other pasteable scraps here */
								}
							break;
						case 'a':
						case 'A':
							if (((DialogPeek)dlog)->editField >= 0) {
								/* Dialog has text edit item: select all */
								SelectDialogItemText(dlog,((DialogPeek)dlog)->editField+1,0,32767);
								}
							 else {
								}
							*itemHit = 0;
							break;
						}
					ans = TRUE;		/* Other cmd-chars ignored */
					}
				break;
			}
		if (doHilite) {
			GetDialogItem(dlog,*itemHit,&type,&hndl,&box);
			/* Reality check */
			if (type == (btnCtrl+ctrlItem)) {
				long soon = TickCount() + 7;		/* Or whatever feels right */
				HiliteControl((ControlHandle)hndl,1);
				while (TickCount() < soon) ;		/* Leave hilited for a bit */
				}
			}
		return(ans);
	}

/*
 * Mouse down event:
 * Check if it's in some user item, and convert to itemHit if appropriate.
 */

static Boolean CheckUserItems(Point where, short *itemHit)
	{
		return(FALSE);
	}

/*
 * Redraw the contents of this dialog due to update event.
 * If you have not installed UserItem draw routines, you should redraw
 * them explicitly here; otherwise, UpdtDialog() will call your routines.
 */

static void DoDialogUpdate(DialogPtr dlog)
	{
		GrafPtr oldPort;

		GetPort(&oldPort); SetPort(dlog);
		BeginUpdate(dlog);

		UpdateDialog(dlog,dlog->visRgn);
		FrameDefault(dlog,BUT1_OK,TRUE);

		EndUpdate(dlog);
		SetPort(oldPort);
	}

/*
 * Activate event: Activate or deactivate this dialog and any items in it
 */

static void DoDialogActivate(DialogPtr dlog, int activ)
	{
		SetPort(dlog);
	}

/*
 * Build this dialog's window on desktop, and install initial item values.
 * Return the dlog opened, or NIL if error (no resource, no memory).
 */

static DialogPtr OpenThisDialog()
	{
		short type; Handle hndl; Rect box; GrafPtr oldPort;
		DialogPtr dlog; unsigned char *p,str[256];

		GetPort(&oldPort);
		dlog = GetNewDialog(thisDialogID,NIL,FRONT_WINDOW);
		if (dlog == NIL) { return(NIL); }	/* Poor man's error message */

		CenterWindow(dlog,0);
		SetPort(dlog);

		/* Fill in dialog's values here */


		ShowWindow(dlog);
		return(dlog);

	}

/*
 * Clean up any allocated stuff, and return dialog to primordial mists
 */

static void CloseThisDialog(DialogPtr dlog)
	{
		if (dlog) {
			DisposeDialog(dlog);	/* Call CloseDialog if you provide storage to GetNewDialog */
		}
	}

/*
 * Deal with user clicking on an item in this dialog, either modal or non-modal.
 * The local point is in where; modifiers in modifiers.
 * Returns whether or not the dialog should be closed (keepGoing).
 */

static int DoDialogItem(DialogPtr dlog, short itemHit)
	{
		short type,okay=FALSE,keepGoing=TRUE,val;
		Handle hndl; Rect box; Point pt;
		unsigned char *p,str[256];

		if (itemHit<1 || itemHit>=LASTITEM)
			return(keepGoing);				/* Only legal items, please */

		GetDialogItem(dlog,itemHit,&type,&hndl,&box);
		switch(type) {
			case ctrlItem+btnCtrl:
				switch(itemHit) {
					case BUT1_OK:
						keepGoing = FALSE; okay = TRUE;
						break;
					case BUT6_Calculate:
					{
unsigned int calc_code;
unsigned char name_str [256];
int str_len,i;

								//calculate code and stuff into item 5
			GetDlgString(dlog ,3,name_str);
//process string
            str_len=name_str[0];
            calc_code=0;
           if (str_len!=0)
           {
            for (i=1;i<str_len; i++)
              {
               calc_code+=name_str[i];
               calc_code*=3;
              }
           }
           while (calc_code<10000000) calc_code=calc_code*3+11;
           //stuff into 5 as a string
            //static void PutDlgLong(DialogPtr dlog, int item, long val, int sel)
            PutDlgLong(dlog,5,calc_code,0);
						break;
             }


					
					}
				break;
			case ctrlItem+chkCtrl:
				break;
			case ctrlItem+radCtrl:
				break;
			case ctrlItem+resCtrl:
				break;
			case statText:
				switch(itemHit) {
					case STXT2_Name:		/* NOT Enabled */
						break;
					case STXT4_Code:		/* NOT Enabled */
						break;
					}
				break;
			case editText:
				switch(itemHit) {
					case EDIT3:
						break;
					case EDIT5:
						break;
					}
				break;
			case iconItem:
				break;
			case picItem:
				break;
			case userItem:
				break;
			}

		if (okay) keepGoing = AnyBadValues(dlog);
		return(keepGoing);
	}

/*
 * Pull values out of dialog items and deliver TRUE if any of them are
 * illegal or inconsistent; otherwise deliver FALSE.  If any values are bad,
 * you should inform your user about the problem here before delivering TRUE.
 * If any items are missing values, this is the place to assign any defaults.
 */

static int AnyBadValues(DialogPtr dlog)
	{
		unsigned char str[256]; short val,len;

		if (GetDlgString(dlog,EDIT3,str)) {
			/* Got a string (can also call GetDlgWord(), etc. here) */
			}
		else {
			/* Field was empty */
			}
		if (GetDlgString(dlog,EDIT5,str)) {
			}
		else {
			}
		return(FALSE);
	}

/*��������������������������������������������������������������������������������������*/

/*
 *  The following are various utility routines for general dialog management.
 *  Typically, you'll want to keep them in a library that is available to all
 *  your dialog modules; however, they are included here (and declared static)
 *  as a private library so that you can quickly compile this file for testing.
 */

#define _PrivateLibraries_
#ifdef  _PrivateLibraries_

/*
 *	Center a given window, w, horizontally in the main screen, top pixels from
 *	the top, or centered vertically if top is 0.  The window should be invisible.
 */

static void CenterWindow(WindowPtr w, short top)
	{
		Rect scr; Point p;
		short rsize,size,margin,xoff,yoff;

		scr = qd.screenBits.bounds;
		SetPort(w);
		p.h = w->portRect.left; p.v = w->portRect.top;
		LocalToGlobal(&p);

		size = scr.right - scr.left;
		rsize = w->portRect.right - w->portRect.left;
		margin = size - rsize;
		if (margin > 0) {
			margin >>= 1;
			p.h = scr.left + margin;
			}
		size = scr.bottom - scr.top;
		rsize = w->portRect.bottom - w->portRect.top;
		margin = size - rsize;
		if (margin > 0) {
			margin >>= 1;
			p.v = scr.top + margin;
			}
		MoveWindow(w,p.h,top?top:p.v,FALSE);
	}

/* Local C string length routine */

static long strlen(register char *str)
	{
		register char *p;

		p = str;
		while (*p++) ;
		return((long)(--p - str));
	}

/* Convert in place a Pascal string to C string, and deliver its address */

static char *PascalToC(unsigned char *str)
	{
		register unsigned char *p,*q,*end;

		end = str + *str;
		q = (p=str) + 1;
		while (p < end) *p++ = *q++;
		*p = '\0';
		return((char *)str);
	}

/*
 *	Convert in place a C string to Pascal string, and deliver its address.
 *	The C string should not be greater than 255 chars in length, or the
 *	resulting Pascal string will be truncated to 255 chars.
 */

static unsigned char *CToPascal(char *str)
	{
		register char *p,*q;
		register long len;

		len = strlen(str);
		if (len > 255) len = 255;
		p = str + len;
		q = p-1;
		while (p != str) *p-- = *q--;
		*str = len;
		return((unsigned char *)str);
	}

/* Dialog Item Stuffers */

/*
 *	Install a given Pascal string, str, into the given static or edit text item
 *	in the dialog, dlog.  If the item is an edit text item, leave the installed
 *	text selected or not according to the value of sel (TRUE or FALSE).
 */

static void PutDlgString(DialogPtr dlog, int item, unsigned char *str, int sel)
	{
		short type; Handle hndl; Rect box;

		GetDialogItem(dlog,item,&type,&hndl,&box);
		SetDialogItemText(hndl,str);
		if (type == editText)
			SelectDialogItemText(dlog,item,sel?0:32767,32767);
		InvalRect(&box);
	}

/*
 *	Install a given decimal long value into the static or edit text item of the
 *	given dialog, dlog.  If the item is an edit text item, leave the installed
 *	text for the number selected or not according to sel (TRUE or FALSE).
 */

static void PutDlgLong(DialogPtr dlog, int item, long val, int sel)
	{
		unsigned char str[32];

		NumToString(val,str);
		PutDlgString(dlog,item,str,sel);
	}

/*
 *	Same as above, only for an int (word) decimal number.
 */

static void PutDlgWord(DialogPtr dlog, int item, int val, int sel)
	{
		PutDlgLong(dlog,item,(long)val,sel);
	}

/*
 *	Set the given check box or radio button item of the given dialog, dlog, to
 *	on or off, according to val.
 */

static void PutDlgChkRadio(DialogPtr dlog, int item, int val)
	{
		short type; Handle hndl; Rect box;

		GetDialogItem(dlog,item,&type,&hndl,&box);
		SetControlValue((ControlHandle)hndl,val!=0);
	}

/*
 *	Deliver the value of the checkbox or radio button item of the given dialog.
 */

static int GetDlgChkRadio(DialogPtr dlog, int item)
	{
		short type; Handle hndl; Rect box;
		
		GetDialogItem(dlog,item,&type,&hndl,&box);
		return(GetControlValue((ControlHandle)hndl) != 0);
	}

/* Dialog Item Unstuffers */

/*
 *	Retrieve the value of an edit text item in a given dialog, placing the
 *	resulting Pascal string in the buffer, str, which is assumed large enough
 *	to hold the text (256 bytes max).  If item is the number of a static text
 *	item, the empty string is delivered.  Delivers TRUE or FALSE according to
 *	whether or not the text so delivered was empty.  
 */

static int GetDlgString(DialogPtr dlog, int item, unsigned char *str)
	{
		short type; Handle hndl; Rect box;

		GetDialogItem(dlog,item,&type,&hndl,&box);
		if (type == editText) GetDialogItemText(hndl,str);
		 else                 *str = 0;
		return(*str != 0);
	}

/*
 *	Retrieve the value of an edit text item in a given dialog, converting the
 *	Pascal string to a long and setting *val to it.  Delivers TRUE or FALSE
 *	according to whether or not the text so delivered was empty.  If FALSE,
 *	*val is set to 0; if TRUE, *val is set to whatever StringToNum() says the
 *	value is, even if the text contains non-numerical characters.
 */

static int GetDlgLong(DialogPtr dlog, int item, long *val)
	{
		int ans; unsigned char str[256];

		*val = 0;
		ans = GetDlgString(dlog,item,str);
		if (ans)
			StringToNum(str,val);
		return(ans);
		}

/* Same as above, only delivers the value of a word */

static int GetDlgWord(DialogPtr dlog, int item, short *val)
	{
		int ans; long num;

		*val = 0;
		ans = GetDlgLong(dlog,item,&num);
		if (ans)
			*val = num;
		return(ans);
	}

/*
 *	Deliver the number of the current editText item in given dialog if any text
 *	is selected in it, or 0 if none selected.
 */

int TextSelected(DialogPtr dlog)
	{
		register TEHandle textH; int item = 0;
		
		textH = ((DialogPeek)dlog)->textH;
		if (*textH)
			if ( (*textH)->selStart != (*textH)->selEnd )
				item = ((DialogPeek)dlog)->editField+1;
		return(item);
	}

/*
 *  If any of the variable argument scrap types are available for pasting from
 *  the scrap, deliver the first one.  Otherwise, deliver 0.  For example,
 *	
 *      if (whichType = CanPaste(3,'TEXT','PICT','STUF')) ...
 *
 *  There can be any number of types in the list, as long as the preceding count, n,
 *  is correct.
 */

static OSType CanPaste(int n, ...)
	{
//	  	  register OSType nextType,ans = 0L;
//	  	  long err,offset;
//	  	  va_list nextArg;
//	  	  
//	  	  va_start(nextArg,n);
//	  	  nextType = va_arg(nextArg, OSType);
//	  	  
//	  	  while (n-- > 0) {
//	  	  	  err = GetScrap(NIL, nextType, &offset);
//	  	  	  if (err >= -1) {
//	  	  	  	  ans = nextType;
//	  	  	  	  break;
//	  	  	  	  }
//	  	  	  nextType = va_arg(nextArg, OSType);
//	  	  	  }
//	  	  
//	  	  va_end(nextArg);
//	  	  return(ans);
return 0;
	}

/*
 *	Frame or unframe a default dialog item (presumed a button) in given dialog.
 *	Note that you don't need to use an extra user item to do this unless you
 *	are doing some sort of non-standard default highlighting (not recommended).
 */

static void FrameDefault(DialogPtr dlog, int item, int frame)
	{
		short type; Handle hndl; Rect box;
		GrafPtr oldPort; PenState oldPen;
		
		GetPort(&oldPort); SetPort(dlog);
		GetPenState(&oldPen);
		
		GetDialogItem(dlog,item,&type,&hndl,&box);
		InsetRect(&box,-4,-4);
		
		PenSize(3,3);
		if (frame) PenPat(&qd.black);		/* Paint frame */
		 else      PenPat(&qd.white);		/* Erase frame */
		FrameRoundRect(&box,16,16);
		
		SetPenState(&oldPen);
		SetPort(oldPort);
	}

/*
 *	Get rectangle, *panel, for a given item (usually a user or picture item)
 *	and then hide the item so that it doesn't interfere with mouse clicking.
 *	This lets you stop worrying about the item order any user or pict items that
 *	obscure other items in the item list, which can affect how the DialogMgr
 *	returns itemHits.
 */

static void GetDlgPanel(DialogPtr dlog, int item, Rect *panel)
	{
		short type; Handle hndl;
		
		GetDialogItem(dlog,item,&type,&hndl,panel);
		HideDialogItem(dlog,item);
	}

#endif
