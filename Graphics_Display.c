/* ========================================
 * Vuong Do
 * Graphics_Disply.c
 * ========================================
*/

#include "Graphics_Display.h"
#include "project.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "GUI.h"
#include "stdlib.h"
#include "math.h"


//Display the intro screen
void ShowStartupScreen(void)
{
    GUI_SetFont(GUI_FONT_32B_1);                            // Set font size
    GUI_SetBkColor(GUI_BLACK);                              // Set background color
    GUI_SetColor(GUI_YELLOW);                               // Set foreground color
    GUI_Clear();
  
    GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);      // Display text in center of the screen
    GUI_DispStringAt("TinyScope", 160, 120);
    
    GUI_SetFont(GUI_FONT_24B_1);                            // Set font size
    GUI_SetColor(GUI_LIGHTGREEN);                           // Set background color
    GUI_SetTextAlign(GUI_TA_HCENTER);                       // Set foreground color
    GUI_DispStringAt("CSE 121/L Final Project", 160, 200);  // Display text at the bottom of the screen
}


//Background draw function
void drawBackground(int w, int h, int xmargin, int ymargin){ 
    GUI_SetBkColor(GUI_BLACK);                              // Set background color
    GUI_SetColor(GUI_DARKCYAN);                             // Set foreground color
    GUI_FillRect(0, 0, w, h);                               // Fill the rectangle
    
    GUI_SetPenSize(1);                                      // Set pen size
    GUI_SetColor(GUI_LIGHTGRAY);                            // Set foreground color
    GUI_DrawRect(xmargin, ymargin, w-xmargin, h-ymargin);
    GUI_SetLineStyle(GUI_LS_DOT);
}


//Grid draw function
void drawGrid(int w, int h, int xdiv, int ydiv, int xmargin, int ymargin){ 
    int xstep = (w-xmargin*2)/xdiv;
    int ystep = (h-ymargin*2)/ydiv;
 
    GUI_SetPenSize(1);              // Set pen size
    GUI_SetColor(GUI_LIGHTGRAY);    // Set foreground color
    GUI_SetLineStyle(GUI_LS_DOT);   // Set line type

    //Draw vertical grid lines
    for (int i=1; i<ydiv; i++){
        GUI_DrawLine(xmargin, ymargin+i*ystep, w-xmargin, ymargin+i*ystep);
    }
    //Draw horizontal grid lines
    for (int i=1; i<xdiv; i++){
        GUI_DrawLine(xmargin+i*xstep, ymargin, xmargin+i*xstep, h-ymargin);
    }
}


//X and Y scale draw function
void printScaleSettings(int xposition, int yposition, int XSCALE_STATE, int YSCALE_STATE) { 
    char str[20];
    
    GUI_SetBkColor(GUI_DARKCYAN);       // Set background color 
    GUI_SetFont(GUI_FONT_16B_1);        // Set font size
    GUI_SetColor(GUI_LIGHTGRAY);        // Set foreground color
    
    //Draw the xscale 
    if (XSCALE_STATE >= 1000){
        sprintf(str, "Xscale: %0d ms/div", XSCALE_STATE/1000);
    }else{
        sprintf(str, "Xscale: %0d us/div", XSCALE_STATE);
    }
    GUI_DispStringAt(str, xposition, yposition);

    //Draw the yscale
    sprintf(str, "Yscale: %0d.%0d V/div", YSCALE_STATE/1000, (YSCALE_STATE%1000)/100);
    GUI_DispStringAt(str, xposition, yposition + 20);
}


//Frequency draw function
void printFrequency(int xposition, int yposition, uint32_t array_freq[FREQ_BUFFER_SIZE]) { 
    char str[20];
    
    //Index for the first and second point. 
    int index1 = FREQ_BUFFER_SIZE;
    int index2 = FREQ_BUFFER_SIZE;
    
    //Set some arbitrary voltage threshold for finding the frequency (=1000)
    uint voltage_threshold = SET_FREQUENCY_THRESHOLD_VALUE;
    
    //Traverse through the frequency array and find the point that is above the threshold (1000)
    //Where the slope is positive
    for(int i = 5;i<FREQ_BUFFER_SIZE;i++){
        int BN = array_freq[i-4]+2*array_freq[i-3]+3*array_freq[i-2]+2*array_freq[i-1]+array_freq[i];
        int BNPrevious = array_freq[i-5]+2*array_freq[i-4]+3*array_freq[i-3]+2*array_freq[i-2]+array_freq[i-1];
        if (array_freq[i]>=voltage_threshold && array_freq[i-1]<voltage_threshold && BNPrevious<BN){
            //Set the first index and break
            index1 = i;
            break;
        }
    }
    //Traverse through the frequency array and find the second point that is above the threshold (1000)
    //Where the slope is positive
    for(int i = index1+50;i<FREQ_BUFFER_SIZE;i++){
        int BN = array_freq[i-4]+2*array_freq[i-3]+3*array_freq[i-2]+2*array_freq[i-1]+array_freq[i];
        int BNPrevious = array_freq[i-5]+2*array_freq[i-4]+3*array_freq[i-3]+2*array_freq[i-2]+array_freq[i-1];
        if (array_freq[i]>=voltage_threshold && array_freq[i-1]<voltage_threshold && BNPrevious<BN){
            //Set the second index and break
            index2 = i;
            break;
        }        
    }
    
    GUI_SetBkColor(GUI_DARKCYAN);                           // Set background color
    GUI_SetFont(GUI_FONT_16B_1);                            // Set font
    GUI_SetColor(GUI_LIGHTGRAY);                            // Set foreground color

    int freq1 = SAMPLE_RATE/(index2-index1);                     // Find the frequency
    
    sprintf(str, "Ch 1 Freq: %0d Hz", freq1);               // Display first channel frequency
    GUI_DispStringAt(str, xposition, yposition);
    sprintf(str, "Ch 2 Freq: %0d Hz", 0);                   // Display second channel frequency
    GUI_DispStringAt(str, xposition, yposition + 20);
}

//Function that returns the number of y pixels needed to draw the wave based on the yxscale settings
int get_Y_Pixel_Range(int YSCALE_STATE){
    if(YSCALE_STATE == 500){
        return 99*2;
    }else if(YSCALE_STATE == 1000){
        return 99;
    }else if(YSCALE_STATE == 1500){
        return 99*2/3;
    }else if(YSCALE_STATE == 2000){
        return 50;
    }
    return 99;
}


//Draw function for the wave when the oscilloscope is in FREE mode
void drawFreeRunning(int XSCALE_STATE, int YSCALE_STATE, uint32_t array10000[PRINT_BUFFER_SIZE], uint32_t array5000[PRINT_BUFFER_SIZE], uint32_t array2000[PRINT_BUFFER_SIZE], uint32_t array1000[PRINT_BUFFER_SIZE], uint32_t array500[PRINT_BUFFER_SIZE], uint32_t array200[PRINT_BUFFER_SIZE], uint32_t array100[PRINT_BUFFER_SIZE]){
    int OFFSET_FROM_BOTTOM_EDGE = ADC_1_GetResult16(1)*YSIZE/MAXIMUM_ADC_READING/2+SCREEN_EDGE_OFFSET;
    
    int Y_PIXEL_RANGE = get_Y_Pixel_Range(YSCALE_STATE);
    
    //Set the pen size and color to draw
    GUI_SetPenSize(3);
    GUI_SetColor(GUI_YELLOW);                               
    
    //Create a temporary print buffer to copy and draw
    int temporary_draw_buffer[PRINT_BUFFER_SIZE];    
    
    //Copy the buffer depends on the xscale settings
    if(XSCALE_STATE == 10000){
        NVIC_DisableIRQ(SysInt_1_cfg.intrSrc);
        for(int i = 0;i<PRINT_BUFFER_SIZE;i++){                               
           temporary_draw_buffer[i] = array10000[i];
        }
        NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    }else if(XSCALE_STATE == 5000){
        NVIC_DisableIRQ(SysInt_1_cfg.intrSrc);
        for(int i = 0;i<PRINT_BUFFER_SIZE;i++){                               
            temporary_draw_buffer[i] = array5000[i];
        }
        NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    }else if(XSCALE_STATE == 2000){
        NVIC_DisableIRQ(SysInt_1_cfg.intrSrc);
        for(int i = 0;i<PRINT_BUFFER_SIZE;i++){                               
            temporary_draw_buffer[i] = array2000[i];
        }
        NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    }else if(XSCALE_STATE == 1000){
        NVIC_DisableIRQ(SysInt_1_cfg.intrSrc);
        for(int i = 0;i<PRINT_BUFFER_SIZE;i++){                               
           temporary_draw_buffer[i] = array1000[i];
        }
        NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    }else if(XSCALE_STATE == 500){
        NVIC_DisableIRQ(SysInt_1_cfg.intrSrc);
        for(int i = 0;i<PRINT_BUFFER_SIZE;i++){                               
            temporary_draw_buffer[i] = array500[i];
        }
        NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    }else if(XSCALE_STATE == 200){
        NVIC_DisableIRQ(SysInt_1_cfg.intrSrc);
        for(int i = 0;i<PRINT_BUFFER_SIZE;i++){                               
            temporary_draw_buffer[i] = array200[i];
        }
        NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    }else if(XSCALE_STATE == 100){
        NVIC_DisableIRQ(SysInt_1_cfg.intrSrc);
        for(int i = 0;i<PRINT_BUFFER_SIZE;i++){                               
            temporary_draw_buffer[i] = array100[i];
        }
        NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    }
    
        //Draw and erase the wave, starting from minimum x coordinate
    for(int i = MINPOINTS;i<MAXPOINTS;i++){                              
        GUI_DrawLine(i,YSIZE-OFFSET_FROM_BOTTOM_EDGE-(temporary_draw_buffer[i]*Y_PIXEL_RANGE/MAXIMUM_ADC_READING),i+1,YSIZE-OFFSET_FROM_BOTTOM_EDGE-(temporary_draw_buffer[i+1]*Y_PIXEL_RANGE/MAXIMUM_ADC_READING));
    }                                  
    GUI_SetColor(GUI_DARKCYAN);                           
    for(int i = MINPOINTS;i<MAXPOINTS;i++){                               
        GUI_DrawLine(i,YSIZE-OFFSET_FROM_BOTTOM_EDGE-(temporary_draw_buffer[i]*Y_PIXEL_RANGE/MAXIMUM_ADC_READING),i+1,YSIZE-OFFSET_FROM_BOTTOM_EDGE-(temporary_draw_buffer[i+1]*Y_PIXEL_RANGE/MAXIMUM_ADC_READING));
    }
}


int triggerIndex = -1;

//Draw function for the wave when the oscilloscope is in TRIGGER mode
void drawTriggerRunning(int TRIGGER_LEVEL_STATE, int XSCALE_STATE, int YSCALE_STATE, uint32_t array10000[PRINT_BUFFER_SIZE], uint32_t array5000[PRINT_BUFFER_SIZE], uint32_t array2000[PRINT_BUFFER_SIZE], uint32_t array1000[PRINT_BUFFER_SIZE], uint32_t array500[PRINT_BUFFER_SIZE], uint32_t array200[PRINT_BUFFER_SIZE], uint32_t array100[PRINT_BUFFER_SIZE], uint32_t TRIGGER_SLOPE_STATE){
    int OFFSET_FROM_BOTTOM_EDGE = ADC_1_GetResult16(1)*YSIZE/MAXIMUM_ADC_READING/2+SCREEN_EDGE_OFFSET;
    int Y_PIXEL_RANGE = get_Y_Pixel_Range(YSCALE_STATE);
    
    //set color to yellow to draw wave    
    GUI_SetPenSize(3);
    GUI_SetColor(GUI_YELLOW);                               

    //Create a temporary print buffer to copy and draw
    uint temporary_draw_buffer[PRINT_BUFFER_SIZE];    
    
    //Copy the buffer depends on the xscale settings
    if(XSCALE_STATE == 10000){
        NVIC_DisableIRQ(SysInt_1_cfg.intrSrc);
        for(int i = 0;i<PRINT_BUFFER_SIZE;i++){                               
           temporary_draw_buffer[i] = array10000[i];
        }
        NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    }else if(XSCALE_STATE == 5000){
        NVIC_DisableIRQ(SysInt_1_cfg.intrSrc);
        for(int i = 0;i<PRINT_BUFFER_SIZE;i++){                               
            temporary_draw_buffer[i] = array5000[i];
        }
        NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    }else if(XSCALE_STATE == 2000){
        NVIC_DisableIRQ(SysInt_1_cfg.intrSrc);
        for(int i = 0;i<PRINT_BUFFER_SIZE;i++){                               
            temporary_draw_buffer[i] = array2000[i];
        }
        NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    }else if(XSCALE_STATE == 1000){
        NVIC_DisableIRQ(SysInt_1_cfg.intrSrc);
        for(int i = 0;i<PRINT_BUFFER_SIZE;i++){                               
           temporary_draw_buffer[i] = array1000[i];
        }
        NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    }else if(XSCALE_STATE == 500){
        NVIC_DisableIRQ(SysInt_1_cfg.intrSrc);
        for(int i = 0;i<PRINT_BUFFER_SIZE;i++){                               
            temporary_draw_buffer[i] = array500[i];
        }
        NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    }else if(XSCALE_STATE == 200){
        NVIC_DisableIRQ(SysInt_1_cfg.intrSrc);
        for(int i = 0;i<PRINT_BUFFER_SIZE;i++){                               
            temporary_draw_buffer[i] = array200[i];
        }
        NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    }else if(XSCALE_STATE == 100){
        NVIC_DisableIRQ(SysInt_1_cfg.intrSrc);
        for(int i = 0;i<PRINT_BUFFER_SIZE;i++){                               
            temporary_draw_buffer[i] = array100[i];
        }
        NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);
    }
    
    //Find the trigger index based on the trigger slope settings
    if(TRIGGER_SLOPE_STATE == POS_SLOPE){
        triggerIndex = getTriggerPosStartIndex(temporary_draw_buffer, TRIGGER_LEVEL_STATE);
    }else{
        triggerIndex = getTriggerNegStartIndex(temporary_draw_buffer, TRIGGER_LEVEL_STATE);
    }
    
    //Draw and erase the wave
    if(triggerIndex!=-1 && triggerIndex<PRINT_BUFFER_SIZE-MAXPOINTS){
        int additional_index_offset = 0;
        for(int i = MINPOINTS;i<MAXPOINTS;i++){                              
            GUI_DrawLine(i,YSIZE-OFFSET_FROM_BOTTOM_EDGE-(temporary_draw_buffer[triggerIndex+additional_index_offset]*Y_PIXEL_RANGE/MAXIMUM_ADC_READING),i+1,YSIZE-OFFSET_FROM_BOTTOM_EDGE-(temporary_draw_buffer[triggerIndex+additional_index_offset+1]*Y_PIXEL_RANGE/MAXIMUM_ADC_READING));
            additional_index_offset++;
        }
        
        GUI_SetColor(GUI_DARKCYAN);
        additional_index_offset = 0;
        for(int i = MINPOINTS;i<MAXPOINTS;i++){                               
            GUI_DrawLine(i,YSIZE-OFFSET_FROM_BOTTOM_EDGE-(temporary_draw_buffer[triggerIndex+additional_index_offset]*Y_PIXEL_RANGE/MAXIMUM_ADC_READING),i+1,YSIZE-OFFSET_FROM_BOTTOM_EDGE-(temporary_draw_buffer[triggerIndex+additional_index_offset+1]*Y_PIXEL_RANGE/MAXIMUM_ADC_READING));
            additional_index_offset++;
        }
    }
}


