//Error_handler.c
#include <Dialogs.h>
#include <Processes.h>
#include <TextUtils.h>
#include <memory.h>

#include <strings.h>

#include <InputSprocket.h>

#include	"error_handler.ext"
#define kALRTFatal 128

void report_error(char *error_string)
{
unsigned char	errorString[256];
char i;
	i=0;
	while ((errorString[i+1]=error_string[i]) !='\0')	i++;
	errorString[0]=i;	//make pascal
	
//	ISpSuspend();

	ParamText((unsigned char*)errorString, "\p", "\p", "\p");
	if (Alert(kALRTFatal, nil) == cancel)
		Debugger();
	
	ExitToShell();

}
