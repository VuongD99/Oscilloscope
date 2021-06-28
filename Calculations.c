/* ========================================
 * Vuong Do
 * Calculations.c
 * ========================================
*/
#include "Calculations.h"
#include "project.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "GUI.h"
#include "stdlib.h"
#include "math.h"


int triggerStartIndex = 0;

//Function that finds the trigger index when trigger slope = positive
int getTriggerPosStartIndex(uint array[PRINT_BUFFER_SIZE], int TRIGGER_LEVEL_STATE){
    //Find the trigger level
    uint trigger_In_ADC = TRIGGER_LEVEL_STATE*MAXIMUM_ADC_READING/MAXIMUM_VOLTAGE_READING;
    
    //If trigger level is 0, find a point where the index is close to 0 and the slope is positive
    if(TRIGGER_LEVEL_STATE == 0){
        for(int i = 0;i<PRINT_BUFFER_SIZE-1;i++){
            if (array[i]<=100 && array[i+1]>array[i]){
                return triggerStartIndex = i;
            }
        }
    //If trigger level is not 0...    
    }else{
        //Traverse through the array and find the first point where the value is above the trigger level and the slope is positive
        for(int i = 5;i<PRINT_BUFFER_SIZE;i++){
            //Get the average slope of the current index
            int BN = array[i-4]+2*array[i-3]+3*array[i-2]+2*array[i-1]+array[i];
            //Get the average slope of the previous index
            int BNPrevious = array[i-5]+2*array[i-4]+3*array[i-3]+2*array[i-2]+array[i-1];
            if (array[i]>=trigger_In_ADC && array[i-1]<trigger_In_ADC && BNPrevious<BN){    
                return triggerStartIndex = i;
            }
        }
    }    
    return TRIGGER_INDEX_NOT_FOUND;
}


//Function that finds the trigger index when trigger slope = negative
int getTriggerNegStartIndex(uint array[PRINT_BUFFER_SIZE], int TRIGGER_LEVEL_STATE){
    //Find the trigger level
    uint trigger_In_ADC = TRIGGER_LEVEL_STATE*MAXIMUM_ADC_READING/MAXIMUM_VOLTAGE_READING;

    //If trigger level is 0, find a point where the index is close to 0 and the slope is negative
    if(TRIGGER_LEVEL_STATE == 0){
        for(int i = 1;i<PRINT_BUFFER_SIZE;i++){
            if (array[i]<=100 && array[i-1]>array[i]){
                return triggerStartIndex = i;
            }
        }
    //If trigger level is not 0...      
    }else{
        //Traverse through the array and find the first point where the value is below the trigger level and the slope is negative
        for(int i = 5;i<PRINT_BUFFER_SIZE;i++){
            //Get the average slope of the current index
            int BN = array[i-4]+2*array[i-3]+3*array[i-2]+2*array[i-1]+array[i];
            //Get the average slope of the previous index
            int BNPrevious = array[i-5]+2*array[i-4]+3*array[i-3]+2*array[i-2]+array[i-1];
            if (array[i]<=trigger_In_ADC && array[i-1]>trigger_In_ADC && BNPrevious>BN){    
                return triggerStartIndex = i;
            }        
        }
    }
    return TRIGGER_INDEX_NOT_FOUND;
}

