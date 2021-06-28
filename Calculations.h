/* ========================================
 * Vuong Do
 * Calculations.h
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

/***************************************
* Constants needed for calculations
****************************************/

#define TRIGGER_INDEX_NOT_FOUND -1
#define PRINT_BUFFER_SIZE 1024
#define MAXIMUM_ADC_READING 2047
#define MAXIMUM_VOLTAGE_READING 3300

/*******************************************************************************
 * User-Callable Functions                                                           *
 ******************************************************************************/ 

/**
 * Function: getTriggerPosStartIndex(uint array[PRINT_BUFFER_SIZE], int TRIGGER_LEVEL_STATE)
 * @param array                : Array used to find the trigger point
 * @param TRIGGER_LEVEL_STATE  : The current trigger level setting value for the oscilloscope
 * @return int                 : Index of the first trigger   
 * @brief Given an array, find the first trigger index where the value is above the trigger threshold and the slope is positive*/
int getTriggerPosStartIndex(uint array[PRINT_BUFFER_SIZE], int TRIGGER_LEVEL_STATE);


/**
 * Function: getTriggerNegStartIndex(uint array[PRINT_BUFFER_SIZE], int TRIGGER_LEVEL_STATE)
 * @param array                : Array used to find the trigger point
 * @param TRIGGER_LEVEL_STATE  : The current trigger level setting value for the oscilloscope
 * @return int                 : Index of the first trigger   
 * @brief Given an array, find the first trigger index where the value is below the trigger threshold and the slope is negative*/
int getTriggerNegStartIndex(uint array[PRINT_BUFFER_SIZE], int TRIGGER_LEVEL_STATE);
