// *************************************MODULE HEADER*********************************
// * FILENAME           : display_config_dialog.h
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 10th May 2002
// * FIRST AUTHOR       : Rob
// *
// * COPYRIGHT (c)        Lightsoft 2002
// *
// ***********************************************************************************

#define USE_DISLAY_CONFIGURATION_SETTINGS 1

void display_configuration_dialog(void);
DisplayIDType get_displayID_for_selected_context(void);

// exported for coffee
extern int hide_screen_preferences;
extern int force_screen_select_dialog;
extern int number_of_previous_screens;
extern int selected_screen;

