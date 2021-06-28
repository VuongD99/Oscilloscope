/* ========================================
 * Vuong Do
 * Graphics_Display.h
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
#include "Command_Parser.h"
#include "Calculations.h"

/***************************************
* Constants related to the LCD display
****************************************/

#define DMA_BUFFER_SIZE 256
#define PRINT_BUFFER_SIZE 1024
#define FREQ_BUFFER_SIZE 1024*5
#define SET_FREQUENCY_THRESHOLD_VALUE 1000
#define SAMPLE_RATE 231000

#define SCREEN_EDGE_OFFSET 5
#define XSIZE 320                                       // Width of LCD screen in pixels
#define YSIZE 240                                       // Height of LCF screen in pixels
#define XDIVISIONS 10                                   // Number of x-axis divisions for grid
#define YDIVISIONS 8                                    // Number of y-axis divisions for grid
#define XMARGIN 5                                       // Margin around screen on x-axis
#define YMARGIN 4                                       // Margin around screen on y-axis
#define MINPOINTS 2*XMARGIN                             // Minimum x drawing coordinate
#define MAXPOINTS XSIZE-2*XMARGIN                       // Maximum x drawing coordinate

#define MAXIMUM_ADC_READING 2047
#define MAXIMUM_VOLTAGE_READING 3300


/*******************************************************************************
 * User-Callable Functions                                                           *
 ******************************************************************************/ 

/**
 * Function: ShowStartupScreen(void)
 * @param None
 * @return None
 * @brief Draw the initial screen when the oscilloscope wakes up */  
void ShowStartupScreen(void);


/**
 * Function: drawBackground(int w, int h, int xmargin, int ymargin)
 * @param w       : The width of the background
 * @param h       : The height of the background
 * @param xmargin : Offset from the left and right edges
 * @param ymargin : Offset from the top and bottom edges
 * @return None   
 * @brief Draw the background for the oscilloscope*/
void drawBackground(int w, int h, int xmargin, int ymargin);


/**
 * Function: drawGrid(int w, int h, int xdiv, int ydiv, int xmargin, int ymargin)
 * @param w       : The width of the background
 * @param h       : The height of the background
 * @param xdiv    : Number of x pixels per division
 * @param ydiv    : Number of y pixels per division
 * @param xmargin : Offset from the left and right edges
 * @param ymargin : Offset from the top and bottom edges
 * @return None   
 * @brief Draw the grid for the oscilloscope*/
void drawGrid(int w, int h, int xdiv, int ydiv, int xmargin, int ymargin);


/**
 * Function: printScaleSettings(int xposition, int yposition, int XSCALE_STATE, int YSCALE_STATE)
 * @param xposition    : Starting x coordinate for drawing the scales
 * @param yposition    : Starting y coordinate for drawing the scales
 * @param XSCALE_STATE : Current X scale setting value of the oscilloscope
 * @param YSCALE_STATE : Current Y scale setting value of the oscilloscope
 * @return None   
 * @brief Draw the x and y scalings for the oscilloscope*/
void printScaleSettings(int xposition, int yposition, int XSCALE_STATE, int YSCALE_STATE);


/**
 * Function: printFrequency(int xposition, int yposition, uint32_t array_freq[FREQ_BUFFER_SIZE])
 * @param xposition    : Starting x coordinate for drawing the frequency
 * @param yposition    : Starting y coordinate for drawing the frequency
 * @param array_freq   : Frequency array used to find the frequency
 * @return None   
 * @brief Find and display the frequency for the oscilloscope*/
void printFrequency(int xposition, int yposition, uint32_t array_freq[FREQ_BUFFER_SIZE]);


/**
 * Function: get_Y_Pixel_Range(int YSCALE_STATE)
 * @YSCALE_STATE      : Current Y scale setting value of the oscilloscope
 * @return            : Number of y pixels   
 * @brief Return the number of y pixels needed to draw the waveform on the oscilloscope screen*/
int get_Y_Pixel_Range(int YSCALE_STATE);


/**
 * Function: drawFreeRunning(int XSCALE_STATE, int YSCALE_STATE, uint32_t array10000[PRINT_BUFFER_SIZE], uint32_t array5000[PRINT_BUFFER_SIZE], uint32_t array2000[PRINT_BUFFER_SIZE], uint32_t array1000[PRINT_BUFFER_SIZE], uint32_t array500[PRINT_BUFFER_SIZE], uint32_t array200[PRINT_BUFFER_SIZE], uint32_t array100[PRINT_BUFFER_SIZE])
 * @param XSCALE_STATE    : Current X scale setting value of the oscilloscope
 * @param YSCALE_STATE    : Current Y scale setting value of the oscilloscope
 * @param array10000      : Draw buffer when xscale=10k
 * @param array5000       : Draw buffer when xscale=5k
 * @param array2000       : Draw buffer when xscale=2k
 * @param array1000       : Draw buffer when xscale=1k
 * @param array500        : Draw buffer when xscale=500
 * @param array200        : Draw buffer when xscale=200
 * @param array100        : Draw buffer when xscale=100
 * @return None   
 * @brief Draw the waveform for the oscilloscope in free run mode*/
void drawFreeRunning(int XSCALE_STATE, int YSCALE_STATE, uint32_t array10000[PRINT_BUFFER_SIZE], uint32_t array5000[PRINT_BUFFER_SIZE], uint32_t array2000[PRINT_BUFFER_SIZE], uint32_t array1000[PRINT_BUFFER_SIZE], uint32_t array500[PRINT_BUFFER_SIZE], uint32_t array200[PRINT_BUFFER_SIZE], uint32_t array100[PRINT_BUFFER_SIZE]);


/**
 * Function: drawTriggerRunning(int TRIGGER_LEVEL_STATE, int XSCALE_STATE, int YSCALE_STATE, uint32_t array10000[PRINT_BUFFER_SIZE], uint32_t array5000[PRINT_BUFFER_SIZE], uint32_t array2000[PRINT_BUFFER_SIZE], uint32_t array1000[PRINT_BUFFER_SIZE], uint32_t array500[PRINT_BUFFER_SIZE], uint32_t array200[PRINT_BUFFER_SIZE], uint32_t array100[PRINT_BUFFER_SIZE], uint32_t TRIGGER_SLOPE_STATE)
 * @param TRIGGER_LEVEL_STATE  : Current trigger setting value of the oscilloscope
 * @param XSCALE_STATE         : Current X scale setting value of the oscilloscope
 * @param YSCALE_STATE         : Current Y scale setting value of the oscilloscope
 * @param array10000           : Draw buffer when xscale=10k
 * @param array5000            : Draw buffer when xscale=5k
 * @param array2000            : Draw buffer when xscale=2k
 * @param array1000            : Draw buffer when xscale=1k
 * @param array500             : Draw buffer when xscale=500
 * @param array200             : Draw buffer when xscale=200
 * @param array100             : Draw buffer when xscale=100
 * @param TRIGGER_SLOPE_STATE  : Current trigger slope setting value of the oscilloscope
 * @return None   
 * @brief Draw the waveform for the oscilloscope in trigger mode*/
void drawTriggerRunning(int TRIGGER_LEVEL_STATE, int XSCALE_STATE, int YSCALE_STATE, uint32_t array10000[PRINT_BUFFER_SIZE], uint32_t array5000[PRINT_BUFFER_SIZE], uint32_t array2000[PRINT_BUFFER_SIZE], uint32_t array1000[PRINT_BUFFER_SIZE], uint32_t array500[PRINT_BUFFER_SIZE], uint32_t array200[PRINT_BUFFER_SIZE], uint32_t array100[PRINT_BUFFER_SIZE], uint32_t TRIGGER_SLOPE_STATE);

