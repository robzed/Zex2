//Error_handler.cpp
/*
 * $Log: Error_handler.cpp,v $
 * Revision 1.5  2003/10/25 18:14:30  stu_c
 * A complete of the routines weren't dumping to the system log.
 *
 * Revision 1.4  2003/09/28 17:30:09  robp
 * Changed files from .c to .cpp and removed spaces out of a couple of filenames.
 *
 * Revision 1.3  2003/09/27 08:58:31  robp
 * Removal of Invalid Conversion Warning for C++ (I hate unsigned/signed chars)
 *
 * Revision 1.2  2003/09/20 12:56:57  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:35:22  stu_c
 * First Imported.
 *
 * Revision 1.13  2002/09/08 02:02:46  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.12  2002/07/07 03:44:47  stu_c
 * Lasers now work but need moving to POCB's
 *
 * Revision 1.11  2002/03/09 15:16:45  stu
 * Fatal error dialog now copied to stdErr
 *
 * Revision 1.10  2001/12/29 01:10:10  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.9  2001/11/24 19:28:52  stu
 * *** empty log message ***
 *
 * Revision 1.8  2001/11/02 23:23:51  stu
 * Further OS X work. Enabled syscon logging.
 *
 * Revision 1.7  2001/11/02 23:14:15  rob
 * build name change
 *
 * Revision 1.6  2001/11/02 19:31:11  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.5  2001/10/27 18:57:22  rob
 * dual interface lib/carbon build
 *
 * Revision 1.4  2001/10/22 21:28:54  rob
 * Carbon warning changes
 *
 * Revision 1.3  2001/10/21 12:35:48  rob
 * const strings.
carbon warning reduction
 *
 * Revision 1.2  2001/10/21 00:57:25  stu
 * Initial port work
 *
 * Revision 1.1  2001/10/20 23:59:49  rob
 * Various carbonisation fixes.
 *
 * Revision 1.0.0.1  2001/10/17 20:46:04  rob
 * First Imported.
 *
 * Revision 1.3  2001/09/09 21:30:56  rob
 * Added ability to see console screen if something goes wrong during boot sequence by clicking on the Debugger button.
 *
 * Revision 1.2  2001/06/03 08:39:15  rob
 * Altered report_error (and variants) so would not close down drawsprokets if it wasn't active yet, thereby allowing us to see early dialog boxes. (e.g. R folder missing, coffee could not load, etc).
 *
 * Revision 1.1.1.1  2001/01/01 21:13:35  rob
 * First Imported.
 *
 * Revision 1.0.0.1  2000/08/21 22:01:29  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #include "options.h"
  #include	"error_handler.h"
  #include "zex_misc.h"
#endif //pch

#include "pictures.h"
#include "OGL.h"
#include "Splat_control.h"

#include "console.h"

#define kALRTFatal 128
#define kALRTRun 132


#define MYDBSTR_LEN 80
Str255 mydebug_cstring;
void MyDebugStr( short line, const char* file, const char* text)
// Internal Debugger function, NORMALLY it is never called, only when FATAL error
// Called from PlayerPro engine
{
int i;
char *ptr;

ptr=((char *)mydebug_cstring)+1;

for(i=0; i < (MYDBSTR_LEN-1) ; i++)
  {
  if(*file) 
    {
    *ptr=*file; ptr++; file++;
    }
  else
    {
    break;
    }
  }
  
mydebug_cstring[0]=i;

report_error(text, mydebug_cstring,line);
}


void report_error(const char *error_string, const Str255 error_string2, int error_number)
{
//extern GDHandle				origGDH;
//extern GrafPtr		       origPort;
extern DSpContextReference		mDisplayContext;
extern int vbl_block;

char	errorString[256];
unsigned char number_string[12];
int i;
	i=0;
        
	disable_watchdog();
    vbl_block=1;	//stop any vbl activity

    if(drawsprockets_is_running())
        {
        ogl_inactive();
        GraphicsPaused();
	DSpContext_Release(mDisplayContext);
	DSpShutdown();
        }
        
        ShowCursor();
    
	while ((errorString[i+1]=error_string[i]) !='\0')	i++;
	errorString[0]=(char)i;	//make pascal


NumToString(error_number,number_string);
//	ISpSuspend();
        if (error_number==10008)
        error_string2="\pAGL_BAD_VALUE. Invalid numerical value.";
//        DebugStr(number_string);

	ParamText((unsigned char*)errorString, (unsigned char*)error_string2, (unsigned char*) number_string, "\p");
   #if  ZEX_PROJECT_BUILDER==1
    fprintf (stderr, "Zex EXEC: Fatal Error Dialog. Detail follows...\n");
    fprintf (stderr, &errorString[1]);
    fprintf (stderr, (char *)error_string2);
    fprintf (stderr, " %d\n\n", error_number);
   #endif
   
	if (Alert(kALRTFatal, nil) == cancel)
	        {
	        show_console_window();
		Debugger();
	        }


    show_text_in_loading_screen("Exec: Zex: Abend. Quit.\n");	

	ExitToShell();

}




void report_error_id(const char *error_string, unsigned long id)
{
unsigned char	errorString[256];
unsigned char id_string[12];
int i;
extern int vbl_block;

	i=0;

	disable_watchdog();
    vbl_block=1;	//stop any vbl activity
        ogl_inactive();
   if(drawsprockets_is_running())
        {
        GraphicsPaused();
	DSpContext_Release(mDisplayContext);
	DSpShutdown();
	}
    ShowCursor();
 
	while ((errorString[i+1]=error_string[i]) !='\0')	i++;
	errorString[0]=(char)i;	//make pascal
//make id a pascal string
id_string[0]=4;
id_string[1]=id>>24;
id_string[2]=id>>16;
id_string[3]=id>>8;
id_string[4]=id;
//	ISpSuspend();

    fprintf (stderr, "Zex EXEC: Fatal Error Dialog. Detail follows...\n");
    fprintf (stderr, (const char*) &errorString[1]);
    fprintf (stderr, (char *)id_string);
    fprintf (stderr, " \n\n");

	ParamText((unsigned char*)errorString, (unsigned char*) id_string, "\p", "\p");
	if (Alert(kALRTFatal, nil) == cancel)
		Debugger();
	
	ExitToShell();

}

void report_error_id_non_fatal(const char *error_string, unsigned long id)
{
unsigned char	errorString[256];
unsigned char id_string[12];
int i;

	i=0;
    if(drawsprockets_is_running())
        {
        GraphicsPaused();
        }
    ShowCursor();
    
	while ((errorString[i+1]=error_string[i]) !='\0')	i++;
	errorString[0]=(char)i;	//make pascal
//make id a pascal string
id_string[0]=4;
id_string[1]=id>>24;
id_string[2]=id>>16;
id_string[3]=id>>8;
id_string[4]=id;
//	ISpSuspend();

	ParamText((unsigned char*)errorString, (unsigned char*) id_string, "\p", "\p");
	if (Alert(kALRTRun, nil) == cancel)
		Debugger();
	
//	ExitToShell();

}


void report_error_id_number(const char *error_string, unsigned long id, int number)
{
unsigned char	errorString[256];
unsigned char id_string[12];
unsigned char number_string[12];
extern int vbl_block;

int i;

	i=0;

	disable_watchdog();
    vbl_block=1;	//stop any vbl activity

    if(drawsprockets_is_running())
        {
        GraphicsPaused();
	DSpContext_Release(mDisplayContext);
	DSpShutdown();
	}
    ShowCursor();
NumToString(number,number_string);
    
	while ((errorString[i+1]=error_string[i]) !='\0')	i++;
	errorString[0]=(char)i;	//make pascal
//make id a pascal string
id_string[0]=4;
id_string[1]=id>>24;
id_string[2]=id>>16;
id_string[3]=id>>8;
id_string[4]=id;
//	ISpSuspend();

    fprintf (stderr, "Zex EXEC: Fatal Error Dialog. Detail follows...\n");
    fprintf (stderr, (const char*) &errorString[1]);
    fprintf (stderr, (char *)id_string);
    fprintf (stderr, " %d\n\n", number);
	ParamText((unsigned char*)errorString, (unsigned char*) id_string, (unsigned char*) number_string, "\p");
	if (Alert(kALRTFatal, nil) == cancel)
		Debugger();
	
	ExitToShell();

}
