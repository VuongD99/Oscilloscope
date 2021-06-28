/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/

#include "project.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "GUI.h"
#include "stdlib.h"
#include "math.h"

/************************************************
* Constants related to the oscilloscope settings
*************************************************/

#define FREE_RUNNING 0                                  //Free running mode macro
#define TRIGGER 1                                       //Trigger mode macro
#define POS_SLOPE 0                                     //Positive trigger slope macro
#define NEG_SLOPE 1                                     //Negative trigger slope macro    
#define TRIGGER_CHAN_1 0                                //Trigger channel 1 macro
#define TRIGGER_CHAN_2 1                                //Trigger channel 2 macro    
#define STOP_STATE false                                //Oscilloscope stop state macro
#define RUN_STATE true                                  //Oscilloscope run state macro


/*******************************************************************************
 * User-Callable Functions                                                           *
 ******************************************************************************/ 

/**
 * Function: argumentParse(char* str, int* OSCILLO_STATE, int* RUNNING_STATE, int* TRIGGER_SLOPE_STATE, int* TRIGGER_CHAN_STATE, int* TRIGGER_LEVEL_STATE, int* XSCALE_STATE, int* YSCALE_STATE)
 * @param str                   : Command input string
 * @param OSCILLO_STATE         : Current state of the oscilloscope
 * @param RUNNING_STATE         : Current run state of the oscilloscope
 * @param TRIGGER_SLOPE_STATE   : Current trigger slope state of the oscilloscope
 * @param TRIGGER_CHAN_STATE    : Current trigger channel of the oscilloscope
 * @param TRIGGER_LEVEL_STATE   : Current trigger level value of the oscilloscope
 * @param XSCALE_STATE          : Current x scale value of the oscilloscope
 * @param YSCALE_STATE          : Current y scale value of the oscilloscope
 * @return None   
 * @brief Parse the input string, return error if the string is invalid, otherwise change the oscilloscope settings accordingly*/
void argumentParse(char* str, int* OSCILLO_STATE, int* RUNNING_STATE, int* TRIGGER_SLOPE_STATE, int* TRIGGER_CHAN_STATE, int* TRIGGER_LEVEL_STATE, int* XSCALE_STATE, int* YSCALE_STATE);
