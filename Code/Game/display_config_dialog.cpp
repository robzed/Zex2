// *************************************MODULE HEADER*********************************
// * FILENAME           : display_config_dialog.cpp
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 14th January 2001
// * FIRST AUTHOR       : Rob
// *
// * COPYRIGHT (c)        Lightsoft 2002
// *
// ***********************************************************************************
// * HISTORY:
// * Date      Initial         Descrip.
// * 30Apr2002   RP            Did the dirty, and started the file
// * 
// *
// ***********************************************************************************


// ***********************************************************************************
// * INCLUDED FILES
// *

#ifndef __ZEX_PCH__
  #include "zselector.h"	      	      // this selects which build we will be doing
  #include "zplatform.h"


  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
    #include <AGL/agl.h>
    #include <DrawSprocket/DrawSprocket.h>
  #else
    #include <Dialogs.h>
    #include <Processes.h>
    #include <TextUtils.h>
    #include <memory.h>
    #include <agl.h>
    #include <DrawSprocket.h>
  #endif

  #include "Error_handler.h"
#include "zex_misc.h"

#endif //pch

#include "display_config_dialog.h"
#include "coffee.h"
#include "splat_control.h"

extern int window_w, window_h;

// ***********************************************************************************
// * CONSTANTS 
// *

#define HIDE_FREQUENCY_SELECTION 1		// if 1, doesn't select frequency

#define module_private static


// ***********************************************************************************
// * TYPE DEFINITIONS
// *


// ***********************************************************************************
// * DATA STRUCTURE DECLARATIONS 
// *

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

bool shift_key_pressed(void);
short isPressed_cached(unsigned short k );
void isPressed_cached_read_keys_now(void);
bool configuration_file_present(void);
void write_screen_prefs(void);

void construct_screen_layout_map(void);
void draw_screen_boxes(void);

int get_screen_cursor_is_in(int v, int h);

void highlight_screen_from_key(Boolean forward);
void highlight_screen_from_position(int v, int h);
static pascal OSStatus ScreenSelectionWindowHandler (EventHandlerCallRef myHandler, EventRef theEvent, void* userData);


void draw_screen_select_graphics(void);
void dispose_screen_select_graphics(void);
void draw_screen_config_graphics(void);
void dispose_screen_config_graphics(void);

void select_no_screens(void);
int get_number_of_screens(void);
int get_highlighted_screen(void);

int return_user_selected_screen(void);
void install_window_button_handler(void);
int user_select_res_screen(int screen);

void construct_screen_resolution_map(int screen);

void Enable_Screen_Select_OK_Button(void);
void Disable_Screen_Select_OK_Button(void);

OSStatus res_data_callback(
  ControlRef browser, 
  DataBrowserItemID itemID, 
  DataBrowserPropertyID property,
  DataBrowserItemDataRef itemData,
  Boolean changeValue);
  
void fill_list_boxes(void);
int return_actual_number_of_contexts(void);
void get_a_context(int context_num, int *width, int *height, Fixed *freq, unsigned int *depthmask);
bool check_for_duplicates(int width, int height, Fixed freq, unsigned int depthmask);
void insert_context(int width, int height, Fixed freq, unsigned int depthmask);
int get_match_preloaded_resolution(void);
OSStatus resolution_selection_callback(ControlRef browser, DataBrowserItemID item, DataBrowserItemNotification message);
int get_match_preloaded_window(void);
OSStatus window_selection_callback(ControlRef browser, DataBrowserItemID item, DataBrowserItemNotification message);
int get_specific_depth(int context_num);
void get_control_values(void);
void alter_game_variables(void);
int get_existing_depth(void);
bool is_selected_resolution_is_possible();

// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *


int hide_screen_preferences=0;			// whether to hide the screen selection

int force_screen_select_dialog=0;		// if 1, even single screened systems get a display setup dialog

int number_of_previous_screens=0;		// can't do this if number of screens=1

int selected_screen=0;				// this is the previous screen

// ***********************************************************************************
// * ACTUAL CODE
// *


// +-------------------------------------+-------------------------+-----------------------
// | TITLE: display_configuration_dialog | AUTHOR(s): Rob Probin   | DATE STARTED: 30 Apr 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----


#define upArrow 0x7e
#define downArrow 0x7d
#define leftArrow 0x7b
#define rightArrow 0x7c

void display_configuration_dialog(void)
{
int screen_index;
int run_game;
// this is probably not needed - we hard link against OpenGL. Added here for defensive coding
 if ((Ptr) kUnresolvedCFragSymbolAddress == (Ptr) aglChoosePixelFormat) // check for existance of OpenGL
 {
  MyDebugStr(__LINE__,"Display Configuration","OpenGL not installed - Zex cannot run");
 }


// 
// Collect some basic information before getting into the dialog side
//
construct_screen_layout_map();

//
// only do the configuration screens if we need to...
//
if(configuration_file_present())
  {
  if(!shift_key_pressed()) 
    {
    if(number_of_previous_screens == get_number_of_screens())	// check number of screens hasn't changed
      {
        
        // check resolution is actually valid for current monitor
        construct_screen_resolution_map(selected_screen);
        if(is_selected_resolution_is_possible())
          {
          return;
          }
      }
    }
  }

number_of_previous_screens=get_number_of_screens();		// set up number of previous screens

do  {
    //
    // SCREEN SELECTION
    //
    if(get_number_of_screens() > 1 || force_screen_select_dialog)	// if there is more than one screen we need a dialog for it
        {
        screen_index = return_user_selected_screen();
        }
    else
        {
        screen_index=0;
        }

    // 
    // RESOLUTION SELECTION
    //
    construct_screen_resolution_map(screen_index);
    
    run_game = user_select_res_screen(screen_index);

    } while (run_game==0);

selected_screen=screen_index;		// set up selected screen

alter_game_variables();
write_screen_prefs();
}


// **************************************
//        T O   D O   L I S T
// **************************************
//
// allow key selection
// get game to use selected screen
//
//
// DONE 6. save to disk (w/ #screens & actual screen)
// DONE 9. check for previous screen number change (& display dialog if changed)
// DONE save and restore selected screen
// DONE check that resolution of that pre-selected screen exists


//   http://developer.apple.com/technotes/tn/tn2009.html



//
// Variables common to the two Dialogs
//

WindowRef theWindow;		// a reference to the window
enum { NO_BUTTON, OK_BUTTON, BACK_BUTTON };
int ButtonClick=NO_BUTTON;


// +---------------------------------------+-------------------------+------------------------
// | TITLE: return_user_selected_screen    | AUTHOR(s): Rob Probin   | DATE STARTED: 2 June 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int return_user_selected_screen(void)
{
EventRecord theEvent;
int screen;
int oldmouse_v, oldmouse_h;
Boolean mouse_hide;
int theKey;

  draw_screen_select_graphics();
  install_window_button_handler();
  
  select_no_screens();  screen=-1;
  
  // wait until a screen is selected
  do {
        ButtonClick=NO_BUTTON;		// no button click at beginning
        // The event loop for the screen selection dialog
        WaitNextEvent(everyEvent, &theEvent, 60/4, NULL);		// we use both WNE + Carbon events, and we poll 4 times a second to
                                                                        // update the screen control, and check for termination. 
                                                                        // However, the processing is minimal, the dialog is only here for a
                                                                        // short amount of time, and the system load looks like nothing to me.
        // handle mouse events
        if(theEvent.where.v != oldmouse_v || theEvent.where.h != oldmouse_h)
          {
          oldmouse_v = theEvent.where.v; 
          oldmouse_h = theEvent.where.h;
          if(mouse_hide) { mouse_hide=FALSE; ShowCursor(); }
          }

        if(mouse_hide==FALSE) highlight_screen_from_position(theEvent.where.v,theEvent.where.h);
        
        // handle key presses
        if(theEvent.what == keyDown)
          {
          theKey = (theEvent.message & keyCodeMask) >> 8;
          if(theKey==upArrow || theKey==downArrow || theKey==leftArrow || theKey==rightArrow)
            {
            if(mouse_hide==FALSE) { HideCursor(); mouse_hide=TRUE; }
            highlight_screen_from_key(theKey==downArrow || theKey==rightArrow);
            }
          }
          
         // check to see if we have a valid end condition
         if(((mouse_hide==FALSE) && (theEvent.what==mouseDown)) || (ButtonClick==OK_BUTTON))
            {
            screen = get_highlighted_screen();
            }

         // if there is a screen, enable the OK button, else don't
         if(get_highlighted_screen() != -1)
           { Enable_Screen_Select_OK_Button(); }
         else
           { Disable_Screen_Select_OK_Button(); }
           
      } while(screen==-1);
      
      
  if(mouse_hide) ShowCursor();	// bring back the mouse! (if hidden)

  dispose_screen_select_graphics();
  
  return screen;
}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: user_select_res_screen         | AUTHOR(s): Rob Probin   | DATE STARTED: 2 June 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----
ControlRef resolution_browser,window_browser;

int user_select_res_screen(int screen)
{
EventRecord theEvent;
//int oldmouse_v, oldmouse_h;
//Boolean mouse_hide;
//int theKey;
int run_game;

  draw_screen_config_graphics();
  install_window_button_handler();

  // wait until a screen is selected
  do {
        ButtonClick=NO_BUTTON;		// no button click at beginning
        // The event loop for the screen selection dialog
        WaitNextEvent(everyEvent, &theEvent, 60/4, NULL);		// we use both WNE + Carbon events, and we poll 4 times a second to
                                                                        // update the screen control, and check for termination. 
                                                                        // However, the processing is minimal, the dialog is only here for a
                                                                        // short amount of time, and the system load looks like nothing to me.
#if 0
      // hack for testing
    if(ButtonClick==OK_BUTTON)
      {
        int temp;
//        Boolean active;
        ButtonClick=NO_BUTTON;
        
        
        if(GetDataBrowserItemCount(resolution_browser, kDataBrowserNoItem, FALSE, kDataBrowserItemIsSelected, &temp))
          { MyDebugStr(0,"ek","ek"); }
        if(temp==1)
          {
          Handle selectedItems;
          OSStatus err;
          selectedItems=NewHandle(0);
          if(selectedItems==NULL) {MyDebugStr(0,"Panic","Panic"); }
          err=GetDataBrowserItems(resolution_browser,kDataBrowserNoItem,FALSE,kDataBrowserItemIsSelected,selectedItems);
          if(err!=noErr) { MyDebugStr(0,"Panic2","Panic2"); }
          temp=GetHandleSize(selectedItems)/sizeof(DataBrowserItemID);
          if(temp!=1)
            { MyDebugStr(0,"Oh no!","Oh no!"); }
          DisposeHandle(selectedItems);
          
//          GetDataBrowserActiveItems(resolution_browser,&active);
        
//          try installing callback....


          }
        
        }
#endif


      } while(ButtonClick==NO_BUTTON);
        
  get_control_values();

  dispose_screen_config_graphics();
  
  if(ButtonClick==BACK_BUTTON)
    {
    run_game = 0;
    }
  else
    {
    run_game = 1;
    }
    
return run_game;
}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: install_window_button_handler  | AUTHOR(s): Rob Probin   | DATE STARTED: 4 June 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void install_window_button_handler(void)
{
OSStatus error;
EventTypeSpec eventList[] = {{ kEventClassCommand,kEventProcessCommand}}; // replaced with command, allocated to window

  error=InstallWindowEventHandler(theWindow,NewEventHandlerUPP(ScreenSelectionWindowHandler),1,eventList,NULL,NULL);
  if(error) MyDebugStr(error,"Display Configuration","Couldn't install my event handler - sorry!");
}

// +---------------------------------------+-------------------------+------------------------
// | TITLE: ScreenSelectionWindowHandler   | AUTHOR(s): Rob Probin   | DATE STARTED: 3 June 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----
static pascal OSStatus ScreenSelectionWindowHandler (EventHandlerCallRef myHandler, EventRef theEvent, void* userData)
{
    #pragma unused (myHandler)		// we don't use the handler call reference
    #pragma unused (userData)		// we don't use the user pointer

    OSStatus result = eventNotHandledErr;
    HICommand commandStruct;
    UInt32 theCommandID;

    GetEventParameter(theEvent, kEventParamDirectObject,typeHICommand, NULL, sizeof(HICommand), NULL, &commandStruct);
    theCommandID = commandStruct.commandID;	// get the command ID of the event that was generated by a button press
      
    if(theCommandID==kHICommandOK)	// 'ok  '
        {
        ButtonClick=OK_BUTTON;
        result = noErr;
        }
    else if(theCommandID=='Back')
        {
        ButtonClick=BACK_BUTTON;
        result = noErr;
        }
    else if(theCommandID==kHICommandQuit || theCommandID==kHICommandCancel)	// 'not!' or 'quit' -- don't remember what I used in interface builder
        {
        ExitToShell();
        result = noErr;
        }
                
    return (result);
}



// +---------------------------------------+-------------------------+------------------------
// | TITLE: center_window                  | AUTHOR(s): Rob Probin   | DATE STARTED: 2 June 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----
/// side effect of center_window - also generates the contents for these variables
int window_content_width=400;   // these are the current window content sizes
int window_content_height=300;	// these are dummy values just in case someone breaks the code

void center_window(WindowPtr theWindow)
{
Rect  wBounds;
int  top, left;
Rect availableBoundsRect;

// Centering the window is a good thing....
GetAvailableWindowPositioningBounds(GetMainDevice(),&availableBoundsRect);	// Carbon 1.3 & later
//GetQDGlobalsScreenBits(&bitMap);	// Carbon 1.0 & later

GetWindowBounds(theWindow, kWindowContentRgn, &wBounds);

window_content_width = wBounds.right - wBounds.left; 
window_content_height = wBounds.bottom - wBounds.top;

left = (((availableBoundsRect.right - availableBoundsRect.left) - window_content_width) / 2);
top = (((availableBoundsRect.bottom - availableBoundsRect.top) - window_content_height) / 2);

MoveWindow(theWindow, left, top, TRUE);
}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: draw_screen_config_graphics    | AUTHOR(s): Rob Probin   | DATE STARTED: 13 May 02
// +
// | DESCRIPTION:  Draws the screen graphics
// +----------------------------------------------------------------ROUTINE HEADER----
ControlRef screen_select_ok_button_control;

void draw_screen_select_graphics(void)
{
OSStatus err;
IBNibRef theNib;
ControlID  ctrlID;

err = CreateNibReference (CFSTR("ScreenConfig"), &theNib);
if (err) MyDebugStr(__LINE__,"Display configuration","Couldn't open screen configuation resource");
err = CreateWindowFromNib (theNib, CFSTR("ScrnSelect"), &theWindow);
if (err) MyDebugStr(__LINE__,"Display configuration","Couldn't open screen select dialog");

SetPortWindowPort(theWindow);
center_window(theWindow);

// lets find the OK button control reference
ctrlID.id = 101;
ctrlID.signature = 'LsZx';
err=GetControlByID(theWindow, &ctrlID, &screen_select_ok_button_control);
if(err) MyDebugStr(err,"Display configuration","Couldn't find OK control");

// disable ok button
Disable_Screen_Select_OK_Button();

ShowWindow(theWindow);

// these are the screen selection boxes...
draw_screen_boxes();
}

// +---------------------------------------+-------------------------+------------------------
// | TITLE: dispose_screen_config_graphics    | AUTHOR(s): Rob Probin   | DATE STARTED: 4 June 02
// +
// | DESCRIPTION:  Draws the screen graphics
// +----------------------------------------------------------------ROUTINE HEADER----

void dispose_screen_select_graphics(void)
{
  DisposeWindow(theWindow);
}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: draw_screen_config_graphics    | AUTHOR(s): Rob Probin   | DATE STARTED: 13 May 02
// +
// | DESCRIPTION:  Draws the screen graphics
// +----------------------------------------------------------------ROUTINE HEADER----
ControlRef depth_control;
ControlRef next_time_control;

void draw_screen_config_graphics(void)
{
OSStatus err;
IBNibRef theNib;
ControlID  ctrlID;

err = CreateNibReference (CFSTR("ScreenConfig"), &theNib);
if (err) MyDebugStr(err,"Display configuration","Couldn't open screen configuation resource");
err = CreateWindowFromNib (theNib, CFSTR("ScrnPrefs"), &theWindow);
if (err) MyDebugStr(err,"Display configuration","Couldn't open screen select dialog");

SetPortWindowPort(theWindow);
center_window(theWindow);

if(get_number_of_screens() > 1 || force_screen_select_dialog) 
    { 
    ControlRef BackButtonControl;

    ctrlID.id = 202;
    ctrlID.signature = 'LsZx';
    err=GetControlByID(theWindow, &ctrlID, &BackButtonControl);
    if(err) MyDebugStr(err,"Display configuration","Couldn't find back control");

    err=SetControlVisibility ( BackButtonControl, TRUE, TRUE); // (ControlRef inControl, Is Visible, Do Draw);
    if(err) MyDebugStr(err,"Display configuration","Couldn't enable back control");
    }

// the 16/32 depth radio button
ctrlID.id = 206;
ctrlID.signature = 'LsZx';
err=GetControlByID(theWindow, &ctrlID, &depth_control);
if(err) MyDebugStr(err,"Display configuration","Couldn't find depth control");

err=DisableControl ( depth_control); // (ControlRef inControl);
if(err) MyDebugStr(err,"Display configuration","Couldn't access depth control");

// the next time tick box
ctrlID.id = 203;
ctrlID.signature = 'LsZx';
err=GetControlByID(theWindow, &ctrlID, &next_time_control);
if(err) MyDebugStr(err,"Display configuration","Couldn't find resolution control");

SetControlValue(next_time_control,hide_screen_preferences);	// set it up to reflect how we came in

fill_list_boxes();

ShowWindow(theWindow);
}

// +---------------------------------------+-------------------------+------------------------
// | TITLE: dispose_screen_config_graphics    | AUTHOR(s): Rob Probin   | DATE STARTED: 4 June 02
// +
// | DESCRIPTION:  Draws the screen graphics
// +----------------------------------------------------------------ROUTINE HEADER----

void dispose_screen_config_graphics(void)
{
  DisposeWindow(theWindow);
}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: fill_list_boxes                | AUTHOR(s): Rob Probin   | DATE STARTED: 30 June 02
// +
// | DESCRIPTION: Fills the resolution boxes with id values
// +----------------------------------------------------------------ROUTINE HEADER----
int selected_resolution;		// 0 based
int selected_window;			// 0 based
int selected_depth;			// 0 based, 0=16, 1=32

void fill_list_boxes(void)
{
OSStatus err;

ControlID  ctrlID;
DataBrowserCallbacks callback_struct;
DataBrowserItemID my_ids[1];

//
// first do resolution select box
//
ctrlID.id = 204;
ctrlID.signature = 'LsZx';
err=GetControlByID(theWindow, &ctrlID, &resolution_browser);
if(err) MyDebugStr(err,"Display configuration","Couldn't find res list control");

callback_struct.version = kDataBrowserLatestCallbacks;
err=InitDataBrowserCallbacks(&callback_struct);

if(err) MyDebugStr(err,"Display configuration","Couldn't init callbacks!!");
callback_struct.u.v1.itemDataCallback = NewDataBrowserItemDataUPP((DataBrowserItemDataProcPtr)res_data_callback);

callback_struct.u.v1.itemNotificationCallback = NewDataBrowserItemNotificationUPP((DataBrowserItemNotificationProcPtr)resolution_selection_callback);

err=SetDataBrowserCallbacks(resolution_browser,&callback_struct);
if(err) MyDebugStr(err,"Display configuration","Couldn't set callbacks");


//
// second do window select box
//
ctrlID.id = 205;
ctrlID.signature = 'LsZx';
err=GetControlByID(theWindow, &ctrlID, &window_browser);
if(err) MyDebugStr(err,"Display configuration","Couldn't find window list control");

callback_struct.version = kDataBrowserLatestCallbacks;
err=InitDataBrowserCallbacks(&callback_struct);

if(err) MyDebugStr(err,"Display configuration","Couldn't init window callbacks!!");
callback_struct.u.v1.itemDataCallback = NewDataBrowserItemDataUPP((DataBrowserItemDataProcPtr)res_data_callback);

callback_struct.u.v1.itemNotificationCallback = NewDataBrowserItemNotificationUPP((DataBrowserItemNotificationProcPtr)window_selection_callback);

err=SetDataBrowserCallbacks(window_browser,&callback_struct);
if(err) MyDebugStr(err,"Display configuration","Couldn't set callbacks");

selected_window=get_match_preloaded_window();		// preload the selection

//
// now sort out the contents of the first selection window (since this may cause a selection event, we need the
// second window sorted out...
//
if(return_actual_number_of_contexts()!=0)
  {
  
  err=AddDataBrowserItems(resolution_browser, kDataBrowserNoItem, return_actual_number_of_contexts(), NULL, kDataBrowserItemNoProperty );
  if(err) MyDebugStr(err,"Display configuration","Couldn't fill resolution list");

  // now highlight that item
  my_ids[0]=get_match_preloaded_resolution()+1;	// the currently selected array, or 0 if non of them
  if(my_ids[0]==0) my_ids[0]=1; // default to the first item
  err=SetDataBrowserSelectedItems(resolution_browser,1,my_ids,kDataBrowserItemsAssign);
  if(err) MyDebugStr(err,"Display configuration","Couldn't select resolution");
  
  err=RevealDataBrowserItem(resolution_browser,my_ids[0],'sres',TRUE);
  if(err) MyDebugStr(err,"Display configuration","Couldn't move list!");
  }
  


}

// +---------------------------------------+-------------------------+------------------------
// | TITLE: resolution_selection_callback  | AUTHOR(s): Rob Probin   | DATE STARTED: 30 June 02
// +
// | DESCRIPTION: When the resolution screen is altered we alter the game window, if the game window
// | is 
// +----------------------------------------------------------------ROUTINE HEADER----

OSStatus resolution_selection_callback(ControlRef browser, DataBrowserItemID item, DataBrowserItemNotification message)
{
OSStatus err;
DataBrowserItemID my_ids[1];
int depth;

if(browser!=resolution_browser) MyDebugStr(0,"Display Configuration","Wrong selection callback called");

if(message==kDataBrowserItemSelected)
  {
  if(item!=0)
    {
    // record the selected item
    selected_resolution = item-1;

    //
    // update the 16/32 bit selection buttons
    //
    depth = get_specific_depth(selected_resolution);
    if((depth& (16+32)) == (16+32))
      {	// both available
      
      if(get_existing_depth()==32) SetControlValue(depth_control,2); else SetControlValue(depth_control,1);
      // enable buttons
      err=EnableControl ( depth_control); // (ControlRef inControl);
      if(err) MyDebugStr(err,"Display configuration","Couldn't access resolution control");
      }
    else // no both
      {
      if(depth & 16)
        {
        // select 16 button
        SetControlValue( depth_control,1);
        }
      else // must be (depth & 32)
        {
        SetControlValue( depth_control,2);
        // select 32 button
        }
        
      // disable buttons
      err=DisableControl ( depth_control); // (ControlRef inControl);
      if(err) MyDebugStr(err,"Display configuration","Couldn't access resolution control");
      }
    
     
    //
    // update the game window
    //
    err=RemoveDataBrowserItems(window_browser, kDataBrowserNoItem, 0, NULL, kDataBrowserItemNoProperty );
    if(err) MyDebugStr( err, "Display configuration","Couldn't remove window list");
    err=AddDataBrowserItems(window_browser, kDataBrowserNoItem, item, NULL, kDataBrowserItemNoProperty );
    if(err) MyDebugStr(err,"Display configuration","Couldn't fill window list");

    //check selected window is not greater than selected resolution
    if(selected_window>selected_resolution)
      {
      selected_window=selected_resolution;
      }
    
    // now highlight that item
    if(selected_window!=-1)
      {
      my_ids[0]=selected_window+1;	// the currently selected array, or 0 if non of them
      if(my_ids[0]==0) my_ids[0]=1; // default to the first item
      
      
      err=SetDataBrowserSelectedItems(window_browser,1,my_ids,kDataBrowserItemsAssign);
      if(err) MyDebugStr(err,"Display configuration","Couldn't select window");
  
      err=RevealDataBrowserItem(window_browser,my_ids[0],'wres',TRUE);
      if(err) MyDebugStr(err,"Display configuration","Couldn't move window list!");
      }
    }
  }

return noErr;

}

// +---------------------------------------+-------------------------+------------------------
// | TITLE: window_selection_callback      | AUTHOR(s): Rob Probin   | DATE STARTED: 30 June 02
// +
// | DESCRIPTION: When the resolution screen is altered we alter the game window, if the game window
// | is 
// +----------------------------------------------------------------ROUTINE HEADER----

OSStatus window_selection_callback(ControlRef browser, DataBrowserItemID item, DataBrowserItemNotification message)
{

if(browser!=window_browser) MyDebugStr(0,"Display Configuration","Wrong selection callback called");

if(message==kDataBrowserItemSelected)
  {
  if(item!=0)
    {
    // record the selected item
    selected_window = item-1;
    }
   }
   
return noErr;
}

// +---------------------------------------+-------------------------+------------------------
// | TITLE: res_data_callback              | AUTHOR(s): Rob Probin   | DATE STARTED: 30 June 02
// +
// | DESCRIPTION: Fills the resolution boxes with drawing values
// +----------------------------------------------------------------ROUTINE HEADER----

OSStatus res_data_callback(
  ControlRef browser, 
  DataBrowserItemID itemID, 
  DataBrowserPropertyID property,
  DataBrowserItemDataRef itemData,
  Boolean changeValue)
{
OSStatus err;
CFMutableStringRef theString;
int width, height;
#if !HIDE_FREQUENCY_SELECTION
int freq;
#endif
Fixed fixed_freq;
unsigned int depthmask;

theString=CFStringCreateMutable(NULL,100);
if(theString==NULL) MyDebugStr(0,"Res Data Callback","Problem making a new string?!");

get_a_context(itemID-1, &width, &height, &fixed_freq, &depthmask);

#if HIDE_FREQUENCY_SELECTION
  CFStringAppendFormat(theString,NULL,CFSTR("%d x %d"),width,height);
#else
  freq=(fixed_freq+0x8000)/0x10000;
  CFStringAppendFormat(theString,NULL,CFSTR("%d x %d @ %dHz"),width,height,freq);
#endif

err=SetDataBrowserItemDataText(itemData, theString);

CFRelease(theString);

return err;
}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: get_control_values             | AUTHOR(s): Rob Probin   | DATE STARTED: 7 July 02
// +
// | DESCRIPTION: fetches the control values before deletion 
// +----------------------------------------------------------------ROUTINE HEADER----

void get_control_values(void)
{
// fetch the depth radio button before it disappears
if(GetControlValue(depth_control)==2)
  {
  selected_depth=1;	// 32 bit
  }
else
  {
  selected_depth=0;	// 16 bit
  }

// fetch the hide this window checkbox
hide_screen_preferences=GetControlValue(next_time_control);
}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: Enable_Screen_Select_OK_Button | AUTHOR(s): Rob Probin   | DATE STARTED: 19 June 02
// +
// | DESCRIPTION:  Draws the screen graphics
// +----------------------------------------------------------------ROUTINE HEADER----

void Enable_Screen_Select_OK_Button(void)
{
OSStatus err;

err = EnableControl(screen_select_ok_button_control); // (ControlRef inControl);
if(err) MyDebugStr(err,"Display configuration","Couldn't enable OK control");
}


// +----------------------------------------+-------------------------+------------------------
// | TITLE: Disable_Screen_Select_OK_Button | AUTHOR(s): Rob Probin   | DATE STARTED: 19 June 02
// +
// | DESCRIPTION:  Draws the screen graphics
// +----------------------------------------------------------------ROUTINE HEADER----

void Disable_Screen_Select_OK_Button(void)
{
OSStatus err;

err = DisableControl(screen_select_ok_button_control); // (ControlRef inControl);
if(err) MyDebugStr(err,"Display configuration","Couldn't disable OK control");
}



// +---------------------------------------+-------------------------+------------------------
// | TITLE: aglReportError                 | AUTHOR(s): Geoff Stahl   | DATE STARTED: 11/11/99-19/1/01
// +
// | DESCRIPTION: if error dump agl errors to debugger string, return error
// +----------------------------------------------------------------ROUTINE HEADER----
//
// Borrowed from Apple for Support of CheckRender
// http://developer.apple.com/samplecode/Sample_Code/Graphics_3D/Carbon_SetupGL/Carbon_Error_Handler.c.htm

OSStatus aglReportError (void)
{
 GLenum err = aglGetError();
 if (AGL_NO_ERROR != err)
  MyDebugStr (__LINE__,"Display Configuration", (char *)aglErrorString(err));

 // ensure we are returning an OSStatus noErr if no error condition
 if (err == AGL_NO_ERROR)
  return noErr;
 else
  return (OSStatus) err;
}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: CheckRenderer                  | AUTHOR(s): Geoff Stahl   | DATE STARTED: 11/11/99-23/8/01
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
// Borrowed from Apple. Thanks Geoff Stahl
// http://developer.apple.com/samplecode/Sample_Code/Graphics_3D/Carbon_SetupGL/Carbon_SetupGL.c.htm

// CheckRenderer

// looks at renderer attributes it has at least the VRAM is accelerated

// Inputs:  hGD: GDHandle to device to look at
//   pVRAM: pointer to VRAM in bytes required; out is actual VRAM if a renderer was found, otherwise it is the input parameter
//   pTextureRAM:  pointer to texture RAM in bytes required; out is same (implementation assume VRAM returned by card is total so we add texture and VRAM)
//   fAccelMust: do we check for acceleration

// Returns: true if renderer for the requested device complies, false otherwise

static Boolean CheckRenderer (GDHandle hGD, long* pVRAM, long* pTextureRAM, GLint* pDepthSizeSupport, Boolean fAccelMust)
{
 AGLRendererInfo info, head_info;
 GLint inum;
 GLint dAccel = 0;
 GLint dVRAM = 0, dMaxVRAM = 0;
 Boolean canAccel = false, found = false;
 head_info = aglQueryRendererInfo(&hGD, 1);
 aglReportError ();
 if(!head_info)
 {
  MyDebugStr(__LINE__,"Display Configuration","aglQueryRendererInfo error -- Sorry!");
  return false;
 }
 else
 {
  info = head_info;
  inum = 0;
  // see if we have an accelerated renderer, if so ignore non-accelerated ones
  // this prevents returning info on software renderer when actually we'll get the hardware one
  while (info)
  { 
   aglDescribeRenderer(info, AGL_ACCELERATED, &dAccel);
   aglReportError ();
   if (dAccel)
    canAccel = true;
   info = aglNextRendererInfo(info);
   aglReportError ();
   inum++;
  }
   
  info = head_info;
  inum = 0;
  while (info)
  {
   aglDescribeRenderer (info, AGL_ACCELERATED, &dAccel);
   aglReportError ();
   // if we can accel then we will choose the accelerated renderer 
   // how about compliant renderers???
   if ((canAccel && dAccel) || (!canAccel && (!fAccelMust || dAccel)))
   {
    aglDescribeRenderer (info, AGL_VIDEO_MEMORY, &dVRAM); // we assume that VRAM returned is total thus add texture and VRAM required
    aglReportError ();
    if (dVRAM >= (*pVRAM + *pTextureRAM))
    {
     if (dVRAM >= dMaxVRAM) // find card with max VRAM
     {
      aglDescribeRenderer (info, AGL_DEPTH_MODES, pDepthSizeSupport); // which depth buffer modes are supported
      aglReportError ();
      dMaxVRAM = dVRAM; // store max
      found = true;
     }
    }
   }
   info = aglNextRendererInfo(info);
   aglReportError ();
   inum++;
  }
 }
 aglDestroyRendererInfo(head_info);
 if (found) // if we found a card that has enough VRAM and meets the accel criteria
 {
  *pVRAM = dMaxVRAM; // return VRAM
  return true;
 }
 // VRAM will remain to same as it did when sent in
 return false;
}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: get_number_of_screens         | AUTHOR(s): Rob Probin   | DATE STARTED: 28 May 02
// +
// | DESCRIPTION: get number of active screens on the system
// +----------------------------------------------------------------ROUTINE HEADER----

int number_of_screens;		// number of active screens on the system

int get_number_of_screens(void)
{
return number_of_screens;
}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: construct_screen_layout_map    | AUTHOR(s): Rob Probin   | DATE STARTED: 28 May 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
// See http://developer.apple.com/techpubs/macosx/Carbon/graphics/DisplayManager/Optimizing_DisplayManager/Conceptual/DisplayManager-11.html#elementId-49938


#define MAX_NUMBER_OF_SCREENS 10
GDHandle active_screen_gdevs[MAX_NUMBER_OF_SCREENS];
Rect active_screen_coords[MAX_NUMBER_OF_SCREENS];
Rect active_draw_coords[MAX_NUMBER_OF_SCREENS];
Boolean active_accel_screen[MAX_NUMBER_OF_SCREENS];

int main_device_index;


// These constants represent minimum limits where we will consider a accelerated render "accelerated" - and will cause 
// a X to be draw in the screen box.
//
// I have no clue what these constants should be in reality, so I made up some numbers. These *should* mean the original
// iBook will be accelerated under OS 8 or 9, but (currently) it won't under Mac OS X, 'cause there isn't a driver for it.
//
// That said, since there is only one screen (and can only be one screen) on an original iBook, you will never see the
// screen selection screen. But you get the idea.
//
#define MIN_TEXTURE_RAM_REQUIRED 2000000
#define MIN_VRAM_REQUIRED 1000000

// we use DMGetFirstScreenDevice rather than GetDeviceList because we can specify active bit
// rather than interigating the device attributes every time to find out whether it is active
// and also whether it is a screen.


#define REAL_SCREEN_DATA 1		// put this to zero to test the selection screen with some test data

#if REAL_SCREEN_DATA

void construct_screen_layout_map(void)
{
GDHandle current_device;
long VRAM, TextureRAM;		// input and out parameters for CheckRender
GLint DepthSizeSupport;		// returns what Depths are accelerated

current_device=DMGetFirstScreenDevice(dmOnlyActiveDisplays); // only show me active displays
number_of_screens=0;
main_device_index=-1;

while (current_device!=NULL && number_of_screens!=MAX_NUMBER_OF_SCREENS)
   {
   active_screen_gdevs[number_of_screens]=current_device;
   active_screen_coords[number_of_screens]=(*current_device)->gdRect;
   VRAM = MIN_VRAM_REQUIRED;
   TextureRAM = MIN_TEXTURE_RAM_REQUIRED;
   active_accel_screen[number_of_screens]=CheckRenderer (current_device, &VRAM, &TextureRAM, &DepthSizeSupport, TRUE);

   // record which one is the main screen
   if(TestDeviceAttribute(current_device,mainScreen)) main_device_index=number_of_screens;
   
   number_of_screens++;
   
   current_device=DMGetNextScreenDevice(current_device,dmOnlyActiveDisplays); // get next active display
   }
   
if(number_of_screens==0) MyDebugStr(__LINE__,"display_configuration_dialog.c","No screens to play Zex on!!");

}

#else	// allows testing of different multiple screens for THIS MODULE ONLY!!

void construct_screen_layout_map(void)
{
number_of_screens=4;
main_device_index=1;

// center monitor
active_screen_gdevs[0]=0;
active_screen_coords[0].top=0;
active_screen_coords[0].left=0;
active_screen_coords[0].bottom=767;
active_screen_coords[0].right=1023;
active_accel_screen[0]=1;

// monitor below (narrow, high)
active_screen_gdevs[1]=0;
active_screen_coords[1].top=768;
active_screen_coords[1].left=0;
active_screen_coords[1].bottom=1700;
active_screen_coords[1].right=600;
active_accel_screen[1]=0;

// monitor right (small 640x480)
active_screen_gdevs[2]=0;
active_screen_coords[2].top=0;
active_screen_coords[2].left=1024;
active_screen_coords[2].bottom=479;
active_screen_coords[2].right=1664;
active_accel_screen[2]=1;

// monitor left (small right 800 x 600)
active_screen_gdevs[3]=0;
active_screen_coords[3].top=100;
active_screen_coords[3].left=-800;
active_screen_coords[3].bottom=700;
active_screen_coords[3].right=-1;
active_accel_screen[3]=0;

}

#endif
// +---------------------------------------+-------------------------+------------------------
// | TITLE: fetch_screen_draw_coords              | AUTHOR(s): Rob Probin   | DATE STARTED: 2 June 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void fetch_screen_draw_coords(int screen, int *top, int *left, int *bottom, int *right)
{
  // fetch this screens boundaries
  *top=active_draw_coords[screen].top;
  *left=active_draw_coords[screen].left;
  *bottom=active_draw_coords[screen].bottom;
  *right=active_draw_coords[screen].right;
}



// +---------------------------------------+-------------------------+------------------------
// | TITLE: draw_box                       | AUTHOR(s): Rob Probin   | DATE STARTED: 2 June 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void draw_box(int top, int left, int bottom, int right)
{
RGBColor blackC = { 0x0000, 0x0000, 0x0000 };

RGBForeColor( &blackC );

MoveTo(left,top);
LineTo(right,top);
LineTo(right,bottom);
LineTo(left,bottom);
LineTo(left,top);  
}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: draw_cross                     | AUTHOR(s): Rob Probin   | DATE STARTED: 2 June 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void draw_cross(int top, int left, int bottom, int right)
{
int atop, aleft, abottom, aright;	// a standards for attenuated, by the way. It was late, OK.

// reduce the cross size
atop    = top    + ((bottom-top)/3);
abottom = bottom - ((bottom-top)/3);
aleft   = left   + ((right-left)/3);
aright  = right  - ((right-left)/3);

// draw the actual cross then
MoveTo(aleft,atop);
LineTo(aright,abottom);

MoveTo(aright,atop);
LineTo(aleft,abottom);
}



// +---------------------------------------+-------------------------+------------------------
// | TITLE: draw_box                       | AUTHOR(s): Rob Probin   | DATE STARTED: 28 May 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


void draw_screen(int screen)
{
int top, left, bottom, right;

  fetch_screen_draw_coords(screen, &top, &left, &bottom, &right);
  draw_box(top,left,bottom,right);
  
  // draw menu bar in main screen
  if(main_device_index==screen) draw_box(top,left,top+((bottom-top)/16),right);

  // draw an X in accelerated contexts
  if(active_accel_screen[screen]) draw_cross(top,left,bottom,right);
}

// +---------------------------------------+-------------------------+------------------------
// | TITLE: highlight_a_screen             | AUTHOR(s): Rob Probin   | DATE STARTED: 2 June 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void highlight_a_screen(int screen)
{
Rect r;
RGBColor yellowC = { 0xFFFF, 0xFFFF, 0x0000 };
int top, left, bottom, right;

fetch_screen_draw_coords(screen, &top, &left, &bottom, &right);

SetRect(&r, left,top,right,bottom);
PenNormal();
RGBForeColor( &yellowC );
PaintRect(&r);

draw_screen(screen);
}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: unhighlight_a_screen           | AUTHOR(s): Rob Probin   | DATE STARTED: 2 June 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void unhighlight_a_screen(int screen)
{
Rect r;
RGBColor whiteC = { 0xFFFF, 0xFFFF, 0xFFFF };
int top, left, bottom, right;

fetch_screen_draw_coords(screen, &top, &left, &bottom, &right);

SetRect(&r, left,top,right,bottom);
PenNormal();
RGBForeColor( &whiteC );
PaintRect(&r);

draw_screen(screen);
}


// +----------------------------------------+-------------------------+------------------------
// | TITLE: find_all_screens_extents        | AUTHOR(s): Rob Probin   | DATE STARTED: 2 June 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void find_all_screens_extents(int *width, int *height, int *center_x, int *center_y)
{
int screen;
int min_x, max_x, min_y, max_y;

min_x = active_screen_coords[0].left;
max_x = active_screen_coords[0].right;
min_y = active_screen_coords[0].top;
max_y = active_screen_coords[0].bottom;

if(number_of_screens>1)
  {
  for(screen=1; screen<number_of_screens; screen++)
    {
    if(min_x > active_screen_coords[screen].left)   min_x = active_screen_coords[screen].left;
    if(max_x < active_screen_coords[screen].right)  max_x = active_screen_coords[screen].right;
    if(min_y > active_screen_coords[screen].top)    min_y = active_screen_coords[screen].top;
    if(max_y < active_screen_coords[screen].bottom) max_y = active_screen_coords[screen].bottom;
    }
  }
  
*width = max_x - min_x;
*height = max_y - min_y;
*center_x = min_x + (*width)/2;
*center_y = min_y + (*height)/2;
}



// +---------------------------------------+-------------------------+------------------------
// | TITLE: draw_screen_boxes              | AUTHOR(s): Rob Probin   | DATE STARTED: 28 May 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

#define SCREEN_WINDOW_LEFT_BORDER 10
#define SCREEN_WINDOW_TOP_BORDER 70
#define SCREEN_WINDOW_BOTTOM_BORDER 95 
#define SCREEN_WINDOW_RIGHT_BORDER 10

#define SCREEN_WINDOW_WIDTH (window_content_width-(SCREEN_WINDOW_LEFT_BORDER+SCREEN_WINDOW_RIGHT_BORDER))
#define SCREEN_WINDOW_HEIGHT (window_content_height-(SCREEN_WINDOW_TOP_BORDER+SCREEN_WINDOW_BOTTOM_BORDER))

#define SCREEN_WINDOW_X_CENTER ((SCREEN_WINDOW_WIDTH/2)+SCREEN_WINDOW_LEFT_BORDER)
#define SCREEN_WINDOW_Y_CENTER ((SCREEN_WINDOW_HEIGHT/2)+SCREEN_WINDOW_TOP_BORDER)

void draw_screen_boxes(void)
{
int width, height, center_x, center_y;
float wscale, hscale, scale;
int screen;

find_all_screens_extents(&width, &height,&center_x,&center_y);

wscale=((float)SCREEN_WINDOW_WIDTH)/width;
hscale=((float)SCREEN_WINDOW_HEIGHT)/height;
// I want to take the smallest scale, because this represents the biggest difference in screen to window mapping
if(hscale<wscale) scale=hscale; else scale=wscale;

for(screen=0; screen<number_of_screens; screen++)
  {
  int top, left, bottom, right;
  
  // transform these into window sized coordinates
  top =    STATIC_CAST_TO_INT(((active_screen_coords[screen].top    - center_y) * scale) + SCREEN_WINDOW_Y_CENTER);
  left =   STATIC_CAST_TO_INT(((active_screen_coords[screen].left   - center_x) * scale) + SCREEN_WINDOW_X_CENTER);
  bottom = STATIC_CAST_TO_INT(((active_screen_coords[screen].bottom - center_y) * scale) + SCREEN_WINDOW_Y_CENTER);
  right =  STATIC_CAST_TO_INT(((active_screen_coords[screen].right  - center_x) * scale) + SCREEN_WINDOW_X_CENTER);
  active_draw_coords[screen].top=top;
  active_draw_coords[screen].left=left;
  active_draw_coords[screen].bottom=bottom;
  active_draw_coords[screen].right=right;
  
  draw_screen(screen);
  unhighlight_a_screen(screen);
    
  }


}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: get_screen_cursor_is_in        | AUTHOR(s): Rob Probin   | DATE STARTED: 2 June 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

int get_screen_cursor_is_in(int v, int h)
{
Point thePoint;
int screen;

// convert to local coordinates
thePoint.v=v; thePoint.h=h; GlobalToLocal(&thePoint); v=thePoint.v; h=thePoint.h;

for(screen=0; screen<number_of_screens; screen++)
  {
  int top, left, bottom, right;
  
  // fetch this screens boundaries
  fetch_screen_draw_coords(screen, &top, &left, &bottom, &right);

  // compare against 
  if(v>=top && v<=bottom && h<=right && h>=left)
    {
    return screen;
    }
    
  } // for(each screen)

return -1;	// no screen
}



// +---------------------------------------+-------------------------+------------------------
// | TITLE: select_no_screens              | AUTHOR(s): Rob Probin   | DATE STARTED: 4 June 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

int last_highlighted_screen=-1;	// which screen was last highlighted

void select_no_screens(void)
{
last_highlighted_screen=-1;
}

// +---------------------------------------+-------------------------+------------------------
// | TITLE: get_highlighted_screen         | AUTHOR(s): Rob Probin   | DATE STARTED: 4 June 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

int get_highlighted_screen(void)
{
return last_highlighted_screen;
}




// +---------------------------------------+-------------------------+------------------------
// | TITLE: highlight_screen_from_position | AUTHOR(s): Rob Probin   | DATE STARTED: 2 June 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void highlight_screen_from_position(int v, int h)
{
int screen;

screen = get_screen_cursor_is_in(v,h);

// if there has been a change
if(last_highlighted_screen != screen)
  {
  // was there one hightlight already?
  if(last_highlighted_screen!=-1) unhighlight_a_screen(last_highlighted_screen);

  // store new screen
  last_highlighted_screen=screen;

  // if not no-screen, highlight the screen
  if(screen!=-1) highlight_a_screen(screen);
  } // end of a different screen
  
}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: highlight_screen_from_key      | AUTHOR(s): Rob Probin   | DATE STARTED: 2 June 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void highlight_screen_from_key(Boolean forward)
{
int screen;

if(forward)
  {
  screen = last_highlighted_screen+1;
  if(screen>=number_of_screens) screen=0;
  }
else // backward
  {
  screen = last_highlighted_screen-1;
  if(screen<0) screen=number_of_screens-1;
  }

// if there has been a change
if(last_highlighted_screen != screen)
  {
  // was there one hightlight already?
  if(last_highlighted_screen!=-1) unhighlight_a_screen(last_highlighted_screen);

  // store new screen
  last_highlighted_screen=screen;

  // if not no-screen, highlight the screen
  if(screen!=-1) highlight_a_screen(screen);
  } // end of a different screen
  
}



// +-------------------------------------------+-------------------------+------------------------
// | TITLE: get_displayID_for_selected_context | AUTHOR(s): Rob Probin   | DATE STARTED: 11 July 02
// +
// | DESCRIPTION: Notice: I could fail to main here. I'm not sure that is good, especially during testing?
// +----------------------------------------------------------------ROUTINE HEADER----
DisplayIDType get_displayID_for_selected_context(void)
{
OSStatus theError;
DisplayIDType theDisplayID;

// convert G_ID to display_id
theError=DMGetDisplayIDByGDevice(active_screen_gdevs[selected_screen],&theDisplayID,FALSE); // boolean is failToMain?
if(theError) MyDebugStr(theError,"Get Selected Display ID","Couldn't find that display!");      /* process the error */

return theDisplayID;
}

// +----------------------------------------+-------------------------+------------------------
// | TITLE: construct_screen_resolution_map | AUTHOR(s): Rob Probin   | DATE STARTED: 11 June 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
#define MAX_NUMBER_OF_CONTEXTS 150

Fixed freqs[MAX_NUMBER_OF_CONTEXTS];
SInt32 widths[MAX_NUMBER_OF_CONTEXTS];
SInt32 heights[MAX_NUMBER_OF_CONTEXTS];
OptionBits depthmasks[MAX_NUMBER_OF_CONTEXTS];

int number_of_contexts;

void construct_screen_resolution_map(int screen)
{
DSpContextReference theContext;
OSStatus theError;
DSpContextAttributes theAttributes;
DisplayIDType theDisplayID;

number_of_contexts=0;

theError=DSpStartup();		// start up DSp temporarily
if(theError) MyDebugStr(theError,"DrawSprocket Startup","Couldn't start up Drawsprockets!");      /* process the error */


// convert G_ID to display_id
theError=DMGetDisplayIDByGDevice(active_screen_gdevs[screen],&theDisplayID,FALSE); // boolean is failToMain?
if(theError) MyDebugStr(theError,"Get Display ID","Couldn't find that display!");      /* process the error */

// OR
// convert screen position to display_id
//theError=DSpFindContextFromPoint(thePoint,&theContext);
//if(theError) MyDebugStr(theError,"","");

theError = DSpGetFirstContext(theDisplayID, &theContext);
if(theError) MyDebugStr(theError,"Get Context","Couldn't find any information on that display!");      /* process the error */

while (theContext && number_of_contexts!=MAX_NUMBER_OF_CONTEXTS)
  {
	  /* process the context */
	  theError = DSpContext_GetAttributes(theContext, &theAttributes);
	  if(theError) MyDebugStr(theError,"Get Attributes","Couldn't find any information on that display!");      /* process the error */
	  
	  if(theAttributes.displayDepthMask & (16+32))		// only accept displays that support 16 or 32 bit
		{
		if(!check_for_duplicates(theAttributes.displayWidth, theAttributes.displayHeight, 
								theAttributes.frequency, theAttributes.displayDepthMask))
		  {
		  // read DSpContextAttributes structure for resolution, frequency and depths
		  insert_context(theAttributes.displayWidth,theAttributes.displayHeight,theAttributes.frequency,theAttributes.displayDepthMask);
		  number_of_contexts++;
		  }
		}
	  
	  /* get the next context */
	  theError = DSpGetNextContext(theContext, &theContext);
	  if(theError) 
	  {	
		if (theError==kDSpContextNotFoundErr) theContext=0;
		else
		MyDebugStr(theError,"Next Context","Problem Getting Context information");     /* process the error */
	  }
  }


theError=DSpShutdown();
if(theError) MyDebugStr(theError,"DrawSprocket Shutdown","Couldn't shutdown Drawsprockets!");      /* process the error */

}

// +-----------------------------------------+-------------------------+------------------------
// | TITLE: check_for_duplicates             | AUTHOR(s): Rob Probin   | DATE STARTED: 3rd July 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

bool check_for_duplicates(int width, int height, Fixed freq, unsigned int depthmask)
{
int count;
if(number_of_contexts==0) return FALSE;

for(count=0; count < number_of_contexts; count++)
  {
  if(width!=widths[count]) continue;
  if(height!=heights[count]) continue;
#if !HIDE_FREQUENCY_SELECTION
  if(freq!=freqs[count]) continue;
#endif
  if(depthmask!=depthmasks[count]) continue;
  
  return TRUE;		// we have a match!
  }
  
return FALSE;
}

// +-----------------------------------------+-------------------------+------------------------
// | TITLE: check_for_duplicates             | AUTHOR(s): Rob Probin   | DATE STARTED: 3rd July 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void insert_context(int width, int height, Fixed freq, unsigned int depthmask)
{
int count;
int move;

count=0;
if(number_of_contexts!=0)
  {
  // find out where this fits into the context
  for(count=0; count < number_of_contexts; count++)
    {
    if(width < widths[count]) break;	// if smaller, we need to insert here
    if(width > widths[count]) continue;	// but if the width is greater, don't compare the others
    if(height < heights[count]) break;
    if(height > heights[count]) continue;
    if(freq < freqs[count]) break;
    if(freq > freqs[count]) continue;
    }


  if(count!=number_of_contexts)
    {
    // now make space in array, by moving all down
    for(move=number_of_contexts; move>count; move--)
      {
      widths[move]=widths[move-1];
      heights[move]=heights[move-1];
      freqs[move]=freqs[move-1];
      depthmasks[move]=depthmasks[move-1];
      }
    }
  }


// now copy the data into the array
widths[count]=width;
heights[count]=height;
freqs[count]=freq;
depthmasks[count]=depthmask;
}


// +-------------------------------------------+-------------------------+------------------------
// | TITLE: is_selected_resolution_is_possible | AUTHOR(s): Rob Probin   | DATE STARTED: 8th July 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
extern int ogl_depth;

bool is_selected_resolution_is_possible(void)
{
int resolution, window;
int depth;

resolution=get_match_preloaded_resolution();
if(resolution==-1) return FALSE;

window=get_match_preloaded_window();
if(window==-1) return FALSE;

depth=get_specific_depth(resolution);
if(ogl_depth==32 && !(depth&32)) return FALSE;

if(ogl_depth==16 && !(depth&16)) return FALSE;

return TRUE;	// selections appear possible for this screen
}


// +-----------------------------------------+-------------------------+------------------------
// | TITLE: get_match_preloaded_resolution | AUTHOR(s): Rob Probin   | DATE STARTED: 7th July 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
int get_match_preloaded_resolution(void)
{
int count;

for(count=0; count<number_of_contexts; count++)
  {
  if(widths[count]==monitor_w && heights[count]==monitor_h) break;
  }

if(count==number_of_contexts) return -1;

return count;
}


// +-----------------------------------------+-------------------------+------------------------
// | TITLE: get_match_preloaded_window | AUTHOR(s): Rob Probin   | DATE STARTED: 7th July 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
int get_match_preloaded_window(void)
{
int count;

for(count=0; count<number_of_contexts; count++)
  {
  if(widths[count]==window_w && heights[count]==window_h) break;
  }

if(count==number_of_contexts) return -1;

return count;
}

// +-----------------------------------------+-------------------------+------------------------
// | TITLE: get_existing_depth               | AUTHOR(s): Rob Probin   | DATE STARTED: 8th July 02
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


int get_existing_depth(void)
{
return ogl_depth;
}


// +-----------------------------------------+-------------------------+------------------------
// | TITLE: return_actual_number_of_contexts | AUTHOR(s): Rob Probin   | DATE STARTED: 1st July 02
// +
// | DESCRIPTION: Returns number of contexts found by construct_screen_resolution_map() 
// +----------------------------------------------------------------ROUTINE HEADER----

int return_actual_number_of_contexts(void)
{
return number_of_contexts;
}

// +-----------------------------------------+-------------------------+------------------------
// | TITLE: get_a_context                    | AUTHOR(s): Rob Probin   | DATE STARTED: 1st July 02
// +
// | DESCRIPTION: Returns a specified context - between 0 and num_contexts-1
// +----------------------------------------------------------------ROUTINE HEADER----

void get_a_context(int context_num, int *width, int *height, Fixed *freq, unsigned int *depthmask)
{

*width=widths[context_num];
*height=heights[context_num];
*freq=freqs[context_num];
*depthmask=depthmasks[context_num];
}

int get_specific_depth(int context_num)
{
return depthmasks[context_num];
}


// +---------------------------------------+-------------------------+------------------------
// | TITLE: alter_game_variables           | AUTHOR(s): Rob Probin   | DATE STARTED: 7 July 02
// +
// | DESCRIPTION:  writes the screen preferences to the game variables
// +----------------------------------------------------------------ROUTINE HEADER----
void alter_game_variables(void)
{
// monitor width / height
monitor_w=widths[selected_resolution];
monitor_h=heights[selected_resolution];

// window width / height
window_w=widths[selected_window];
window_h=heights[selected_window];

// screen depth
if(selected_depth==1) ogl_depth=32; else ogl_depth=16;

}

// +---------------------------------------+-------------------------+------------------------
// | TITLE: write_screen_prefs             | AUTHOR(s): Rob Probin   | DATE STARTED: 10 May 02
// +
// | DESCRIPTION:  writes the screen preferences to a file.
// +----------------------------------------------------------------ROUTINE HEADER----

void kwrite(char **output, char *string); // borrow routine from key_setting.c

void write_screen_prefs(void)
{
int i;
char **output_ptr;
char *output;
char *output_copy;
int *length;
unsigned char number_str[20];

 output_ptr=(char **)interpret("screen_prefs_buffer_ptr set-param1");
 output=*output_ptr;
 
 output_copy=output;	// start position for generating the length

 kwrite(&output,"\\ Zex's Current Screen Prefs - Autogenerated File\x0d\x0d\x0d");

 ZexNumToStringPascal(monitor_w,number_str); number_str[number_str[0]+1]='\0';
 kwrite(&output,(char *)&number_str[1]);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
 kwrite(&output," ");
 ZexNumToStringPascal(monitor_h,number_str); number_str[number_str[0]+1]='\0';
 kwrite(&output,(char *)&number_str[1]);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
 kwrite(&output," SRES2 \x0d");

 ZexNumToStringPascal(window_w,number_str); number_str[number_str[0]+1]='\0';
 kwrite(&output,(char *)&number_str[1]);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
 kwrite(&output," ");
 ZexNumToStringPascal(window_h,number_str); number_str[number_str[0]+1]='\0';
 kwrite(&output,(char *)&number_str[1]);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
 kwrite(&output," WRES2 \x0d");

 if(selected_depth==1)
   {
   kwrite(&output,"32 SDEPTH \x0d");
   }
 else
   {
   kwrite(&output,"16 SDEPTH \x0d");
   }

// kwrite(&output,"\\ No frequency setting yet \x0d"

 ZexNumToStringPascal(selected_screen,number_str); number_str[number_str[0]+1]='\0';
 kwrite(&output,(char *)&number_str[1]);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
 kwrite(&output," SELECTED_SCREEN \x0d");
 
 ZexNumToStringPascal(number_of_previous_screens,number_str); number_str[number_str[0]+1]='\0';
 kwrite(&output,(char *)&number_str[1]);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
 kwrite(&output," PREVIOUS_SCREENS \x0d");

 if(hide_screen_preferences)
   {
   kwrite(&output,"HIDE_SCRN_PREFS \x0d\x0d");
   }
  
 length=(int *)interpret("screen_prefs_length set-param1");
 *length=output-output_copy;	// write the length to fouth variable
 
 i=interpret("write_screen_prefs_to_disk set-param1");
 if(i) MyDebugStr(i,"display_configuration_dialog.c","Screen Prefs File Write Problem"); 
 
}



// +---------------------------------------+-------------------------+------------------------
// | TITLE: configuration_file_present     | AUTHOR(s): Rob Probin   | DATE STARTED: 10 May 02
// +
// | DESCRIPTION:  Returns TRUE if configuration file was correctly read
// +----------------------------------------------------------------ROUTINE HEADER----



bool configuration_file_present(void)
{
if(hide_screen_preferences)
  {
  return TRUE;
  }
else
  {
  return FALSE;
  }
}


// +---------------------------------------+-------------------------+-----------------------
// | TITLE: isPressed_cached_read_keys_now | AUTHOR(s): Rob Probin   | DATE STARTED: 5 May 02
// +
// | DESCRIPTION:  Stores internally keys pressed. 
// +----------------------------------------------------------------ROUTINE HEADER----
unsigned char km[16];
        
void isPressed_cached_read_keys_now(void)
{
	GetKeys( (long *) km);
	return;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: isPressed_cached        | AUTHOR(s): Rob Probin   | DATE STARTED: 5 May 02
// +
// | DESCRIPTION:  Returns if a specific key is pressed. You must call 
// | isPressed_cached_read_keys_no first to read the key map. 
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

short isPressed_cached(unsigned short k )
// k =  any keyboard scan code, 0-127
{
	return ( ( km[k>>3] >> (k & 7) ) & 1);
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: shift_key_pressed       | AUTHOR(s): Rob Probin   | DATE STARTED: 30 Apr 02
// +
// | DESCRIPTION:  Returns true if Apple, Option, Shift or Control is being pressed.
// |
// +----------------------------------------------------------------ROUTINE HEADER----

#define SHIFT_KEYCODE 0x38
#define APPLE_KEYCODE 0x37
#define OPTION_KEYCODE 0x3a
#define CTRL_KEYCODE 0x3b

bool shift_key_pressed(void)
{
isPressed_cached_read_keys_now();

return (isPressed_cached(SHIFT_KEYCODE) || isPressed_cached(APPLE_KEYCODE) || 
            isPressed_cached(OPTION_KEYCODE) || isPressed_cached(CTRL_KEYCODE));
}

// ***********************************************************************************
// * DESIGN NOTES
// *


/* 
DSp errors
==========
noErr  0 No error
kDSpNotInitializedErr -30440        DSpStartup has not yet been called.
kDSpSystemSWTooOldErr -30441         System software too old.
kDSpInvalidContextErr -30442         Invalid context reference.
kDSpInvalidAttributesErr -30443      Some field in an attributes structure has an invalid value.
kDSpContextAlreadyReservedErr -30444 The context is already reserved.
kDSpContextNotReservedErr -30445     The context is not reserved.
kDSpContextNotFoundErr -30446        DrawSprocket couldn’t find the context.
kDSpFrameRateNotReadyErr -30447      Not enough time has passed for DrawSprocket to calculate a frame rate.
kDSpConfirmSwitchWarning -30448      The gameMustConfirmSwitch flag is set.
kDSpInternalErr -30449               Corrupted DrawSprocket or other error.
kDSpStereoContextErr -30450          DrawSprocket attempted to process a stereo context. (DrawSprocket no longer supports GoggleSprocket.)

Display Manager Errors
======================
paramErr  -50                        Invalid value passed in a parameter. Your application passed an invalid parameter for dialog options.
kDMGenErr -6220                      Indeterminate error
kDMMirroringOnAlready -6221          Video mirroring is already enabled.
kDMWrongNumberOfDisplays -6222       Wrong number of displays
kDMMirroringBlocked -6223            Video blocked
kDMCantBlock -6224                   Video mirroring already enabled and can’t be blocked; use DMUnMirrorDevice, then call DMBlockMirroring again.
kDMMirroringNotOn -6225              Video mirroring is not currently enabled.
kSysSWTooOld -6226                   Some piece of system software is too old for the Display Manager to operate.
kDMSWNotInitializedErr -6227         Required pieces of system software are not initialized.
kDMDriverNotDisplayMgrAwareErr -6228 The video driver for the display does not support the Display Manager.
kDMNotFoundErr -6229 
kDMDisplayNotFoundErr -6229          There are no GDevice structures for displays in the device list.
kDMDisplayAlreadyInstalledErr -6230  The display is already in the device list and can’t be added.
kDMNoDeviceTableclothErr -6231 
kDMMainDisplayCannotMoveErr -6231 
kDMFoundErr -6232                     Item found

*/
