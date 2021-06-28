/******************************************************************************
* Vuong Do
* CSE 121 FINAL PROJECT
* MAIN .C 
 *******************************************************************************/

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
#include "Graphics_Display.h"
#include "Command_Parser.h"
#include "Calculations.h"

/***************************************
* Macros
****************************************/

#define NEWLINE 13
#define DMA_BUFFER_SIZE 256
#define PRINT_BUFFER_SIZE 1024
#define FREQUENCY_BUFFER_SIZE PRINT_BUFFER_SIZE*5

//Macros used for ISR handler indexing
#define IS_ALSO_A_10000_INDEX 85                        //When xscale= 10000, we need to store every 85 samples
#define IS_ALSO_A_5000_INDEX 42                         //When xscale= 5000, we need to store every 42 samples
#define IS_ALSO_A_2000_INDEX 17                         //When xscale= 2000, we need to store every 17 samples
#define IS_ALSO_A_1000_INDEX 8                          //When xscale= 1000, we need to store every 8 samples
#define IS_ALSO_A_500_INDEX 4                           //When xscale= 500, we need to store every 4 samples    
#define IS_ALSO_A_200_INDEX 2                           //When xscale= 200, we need to store every 2 samples


/***************************************
* Global variables
****************************************/

bool readSecond = false;                                //Indicate which array to read from

//DMA buffers
uint32_t arrayA[DMA_BUFFER_SIZE];                       //First DMA buffer
uint32_t arrayB[DMA_BUFFER_SIZE];                       //Second DMA buffer

uint32_t array_freq[FREQUENCY_BUFFER_SIZE];             //Array used to find the frequency

//Drawing buffers
uint32_t array10000[PRINT_BUFFER_SIZE];                 //xscale=10k array buffer used for drawing
uint32_t array5000[PRINT_BUFFER_SIZE];                  //xscale=5k    
uint32_t array2000[PRINT_BUFFER_SIZE];                  //xscale=2k
uint32_t array1000[PRINT_BUFFER_SIZE];                  //xscale=1k
uint32_t array500[PRINT_BUFFER_SIZE];                   //xscale=500
uint32_t array200[PRINT_BUFFER_SIZE];                   //xscale=200
uint32_t array100[PRINT_BUFFER_SIZE];                   //xscale=100

//Index counters for the arrays
int freq_index = 0;
int index10000 = 0;
int index5000 = 0;
int index2000 = 0;
int index1000 = 0;
int index500 = 0;
int index200 = 0;
int index100 = 0;

//Input line buffer
char input[100];

//Oscilloscope settings trackers
int OSCILLO_STATE = STOP_STATE;
int RUNNING_STATE = FREE_RUNNING;
int TRIGGER_SLOPE_STATE = POS_SLOPE;
int TRIGGER_CHAN_STATE = TRIGGER_CHAN_1;
int TRIGGER_LEVEL_STATE = 1000;
int XSCALE_STATE = 1000;
int YSCALE_STATE = 1000;


/*******************************************************************************
 * User-Callable Functions                                                           *
 ******************************************************************************/  

//ISR Handler Function
void ISR_Handler()
{   
    //Determine which buffer to read values from
    if (readSecond == true){                                            //Buffer 2 is ready to be used
        readSecond = false;                  
        for(int i=0;i<DMA_BUFFER_SIZE;i++){                             //Traverse through the second buffer
            array_freq[freq_index] = (uint16_t)arrayB[i];               //Add value to the frequency array        
            array100[index100] = (uint16_t)arrayB[i];                   //Add every value to the (xscale=100) buffer
            
            if(index100%IS_ALSO_A_10000_INDEX==0){                      //Add every 85th value to the xscale=10000 buffer
                array10000[index10000] = (uint16_t)arrayB[i];
                if(index10000<PRINT_BUFFER_SIZE){                       //Reset the index for the xscale=10000 buffer if it has reached its limit
                    index10000++;
                }else{
                    index10000 = 0;                                     //Otherwise increment it
                } 
            }
            if(index100%IS_ALSO_A_5000_INDEX==0){                       //Add every 42th value to the xscale=5000 buffer
                array5000[index5000] = (uint16_t)arrayB[i];
                if(index5000<PRINT_BUFFER_SIZE){                        //Reset the index for the xscale=5000 buffer if it has reached its limit
                    index5000++;
                }else{
                    index5000 = 0;                                      //Otherwise increment it
                } 
            }
            if(index100%IS_ALSO_A_2000_INDEX==0){                       //Add every 17th value to the xscale=2000 buffer
                array2000[index2000] = (uint16_t)arrayB[i];
                if(index2000<PRINT_BUFFER_SIZE){                        //Reset the index for the xscale=2000 buffer if it has reached its limit
                    index2000++;
                }else{
                    index2000 = 0;                                      //Otherwise increment it
                } 
            }
            if(index100%IS_ALSO_A_1000_INDEX==0){                       //Add every 8th value to the xscale=1000 buffer
                array1000[index1000] = (uint16_t)arrayB[i];
                if(index1000<PRINT_BUFFER_SIZE){                        //Reset the index for the xscale=1000 buffer if it has reached its limit
                    index1000++;
                }else{
                    index1000 = 0;                                      //Otherwise increment it
                } 
            }
            if(index100%IS_ALSO_A_500_INDEX==0){                        //Add every 4th value to the xscale=500 buffer
                array500[index500] = (uint16_t)arrayB[i];
                if(index500<PRINT_BUFFER_SIZE){                         //Reset the index for the xscale=500 buffer if it has reached its limit
                    index500++;
                }else{
                    index500 = 0;                                       //Otherwise increment it
                } 
            }
            if(index100%IS_ALSO_A_200_INDEX==0){                        //Add every 2nd value to the xscale=200 buffer
                array200[index200] = (uint16_t)arrayB[i];
                if(index200<PRINT_BUFFER_SIZE){                         //Reset the index for the xscale=200 buffer if it has reached its limit
                    index200++;
                }else{
                    index200 = 0;                                       //Otherwise increment it
                } 
            }
            if(index100<PRINT_BUFFER_SIZE){                             //Reset the index for the xscale=100 buffer if it has reached its limit
                index100++;
            }else{
                index100 = 0;                                           //Otherwise increment it
            }
            if(freq_index<FREQUENCY_BUFFER_SIZE){                       //Reset the index for the frequency buffer if it has reached its limit
                freq_index++;
            }else{
                freq_index = 0;                                         //Otherwise increment it
            }           
        } 
    }else{                                                              //Array A is ready to be used
        readSecond = true;                    
        for(int i=0;i<DMA_BUFFER_SIZE;i++){                             //Traverse through the second buffer
            array_freq[freq_index] = (uint16_t)arrayA[i];               //Add value to the frequency array        
            array100[index100] = (uint16_t)arrayA[i];                   //Add every value to the (xscale=100) buffer
            
            if(index100%IS_ALSO_A_10000_INDEX==0){                      //Add every 85th value to the xscale=10000 buffer
                array10000[index10000] = (uint16_t)arrayA[i];
                if(index10000<PRINT_BUFFER_SIZE){                       //Reset the index for the xscale=10000 buffer if it has reached its limit
                    index10000++;
                }else{
                    index10000 = 0;                                     //Otherwise increment it
                } 
            }
            if(index100%IS_ALSO_A_5000_INDEX==0){                       //Add every 42th value to the xscale=5000 buffer
                array5000[index5000] = (uint16_t)arrayA[i];
                if(index5000<PRINT_BUFFER_SIZE){                        //Reset the index for the xscale=5000 buffer if it has reached its limit
                    index5000++;
                }else{
                    index5000 = 0;                                      //Otherwise increment it
                } 
            }
            if(index100%IS_ALSO_A_2000_INDEX==0){                       //Add every 17th value to the xscale=2000 buffer
                array2000[index2000] = (uint16_t)arrayA[i];
                if(index2000<PRINT_BUFFER_SIZE){                        //Reset the index for the xscale=2000 buffer if it has reached its limit
                    index2000++;
                }else{
                    index2000 = 0;                                      //Otherwise increment it
                } 
            }
            if(index100%IS_ALSO_A_1000_INDEX==0){                       //Add every 8th value to the xscale=1000 buffer
                array1000[index1000] = (uint16_t)arrayA[i];
                if(index1000<PRINT_BUFFER_SIZE){                        //Reset the index for the xscale=1000 buffer if it has reached its limit
                    index1000++;
                }else{
                    index1000 = 0;                                      //Otherwise increment it
                } 
            }
            if(index100%IS_ALSO_A_500_INDEX==0){                        //Add every 4th value to the xscale=500 buffer
                array500[index500] = (uint16_t)arrayA[i];
                if(index500<PRINT_BUFFER_SIZE){                         //Reset the index for the xscale=500 buffer if it has reached its limit
                    index500++;
                }else{
                    index500 = 0;                                       //Otherwise increment it
                } 
            }
            if(index100%IS_ALSO_A_200_INDEX==0){                        //Add every 2nd value to the xscale=200 buffer
                array200[index200] = (uint16_t)arrayA[i];
                if(index200<PRINT_BUFFER_SIZE){                         //Reset the index for the xscale=200 buffer if it has reached its limit
                    index200++;
                }else{
                    index200 = 0;                                       //Otherwise increment it
                } 
            }
            if(index100<PRINT_BUFFER_SIZE){                             //Reset the index for the xscale=100 buffer if it has reached its limit
                index100++;
            }else{
                index100 = 0;                                           //Otherwise increment it
            }
            if(freq_index<FREQUENCY_BUFFER_SIZE){                       //Reset the index for the frequency buffer if it has reached its limit
                freq_index++;
            }else{
                freq_index = 0;                                         //Otherwise increment it
            }           
        }
    }
    //Clear the interrupt
    Cy_DMA_Channel_ClearInterrupt(DMA_1_HW,0);     
    NVIC_ClearPendingIRQ(SysInt_1_cfg.intrSrc);
}

//Function that initializes the DMA
void beginDMA(void){
    //Set the source and destination addresses for each descriptor
    Cy_DMA_Descriptor_Init(&DMA_1_Descriptor_1, &DMA_1_Descriptor_1_config);
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_1_Descriptor_1, (uint32_t *) &(SAR->CHAN_RESULT[0]));
    Cy_DMA_Descriptor_SetDstAddress(&DMA_1_Descriptor_1, arrayA);    
    
    Cy_DMA_Descriptor_Init(&DMA_1_Descriptor_2, &DMA_1_Descriptor_2_config);    
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_1_Descriptor_2, (uint32_t *) &(SAR->CHAN_RESULT[0]));
    Cy_DMA_Descriptor_SetDstAddress(&DMA_1_Descriptor_2, arrayB);

    cy_stc_dma_channel_config_t channelConfig;
    channelConfig.descriptor  = &DMA_1_Descriptor_1;
    channelConfig.preemptable = DMA_1_PREEMPTABLE;
    channelConfig.priority    = DMA_1_PRIORITY;
    channelConfig.enable      = false;
    channelConfig.bufferable  = DMA_1_BUFFERABLE;   
    
    //Initialize and start the DMA Channel
    Cy_DMA_Channel_Init(DMA_1_HW, DMA_1_DW_CHANNEL, &channelConfig);  
    Cy_DMA_Channel_Enable(DMA_1_HW, DMA_1_DW_CHANNEL);  
    Cy_DMA_Enable(DMA_1_HW); 
    
    //Set the interrupt mask for the DMA
    Cy_DMA_Channel_SetInterruptMask (DMA_1_HW,DMA_1_DW_CHANNEL, CY_DMA_INTR_MASK);       
}

//Function that initializes the arrays
void arrayInitialize(void){
    //Fill the DMA buffers with zeroes
    for(int i=0;i<DMA_BUFFER_SIZE;i++){
        arrayA[i] = (uint32_t) 0;
        arrayB[i] = (uint32_t) 0;      
    }
    //Fill the graphics buffers with zeroes
    for(int i=0;i<PRINT_BUFFER_SIZE;i++){ 
        array100[i] = (uint32_t) 0;
        array200[i] = (uint32_t) 0;
        array500[i] = (uint32_t) 0;
        array1000[i] = (uint32_t) 0;
        array2000[i] = (uint32_t) 0;
        array5000[i] = (uint32_t) 0;
        array10000[i] = (uint32_t) 0;
    }
    //Fill the frequency buffer with zeroes
    for(int i=0;i<FREQUENCY_BUFFER_SIZE;i++){
        array_freq[i] = (uint32_t) 0;
    }
}


int ind = 0;
//Main function
int main(void)
{
     __enable_irq(); /* Enable global interrupts. */

    //Initialize the arrays
    arrayInitialize();
    
    //Initialize the UART  
    PRINT_UART_Start();
  
    //Initialize the interrupt
    Cy_SysInt_Init(&SysInt_1_cfg, ISR_Handler);
    NVIC_EnableIRQ(SysInt_1_cfg.intrSrc);

    //Initialize EMWIN
    GUI_Init();
   
    //Display the startup screen
    ShowStartupScreen();   
    Cy_SysLib_Delay(1000);
    
    //Display background
    drawBackground(XSIZE, YSIZE, XMARGIN, YMARGIN);
    drawGrid(XSIZE, YSIZE, XDIVISIONS, YDIVISIONS, XMARGIN, YMARGIN);
    printScaleSettings(200, 15, XSCALE_STATE, YSCALE_STATE);
    printFrequency(10, 15, array_freq);
    
    //Initialize the DMA  
    beginDMA();
    
    //Initialize the ADC
    ADC_1_Start();                 
    ADC_1_StartConvert();    
        
    
    while(1)
    {
        //Check the status of the RX FIFO to see if it's empty
        while (Cy_SCB_UART_GetRxFifoStatus(PRINT_UART_HW) & CY_SCB_UART_RX_NOT_EMPTY){
            //Add the character to the line buffer
            input[ind] = Cy_SCB_UART_Get(PRINT_UART_HW);
            //Call the argument parse function if sees a newline
            if(input[ind]==NEWLINE){
                ind = 0;
                argumentParse(input, &OSCILLO_STATE, &RUNNING_STATE, &TRIGGER_SLOPE_STATE, &TRIGGER_CHAN_STATE, &TRIGGER_LEVEL_STATE, &XSCALE_STATE, &YSCALE_STATE);
                //Reset the line buffer
                memset(input, 0, sizeof(input));
            }else{
                ind++;
            }
            //Clear the RX FIFO status
            Cy_SCB_UART_ClearRxFifoStatus(PRINT_UART_HW, CY_SCB_UART_RX_NOT_EMPTY);         
        }

        if(OSCILLO_STATE==RUN_STATE && RUNNING_STATE == FREE_RUNNING){
            drawFreeRunning(XSCALE_STATE, YSCALE_STATE, array10000, array5000, array2000, array1000, array500, array200, array100);
            //Redraw the grid, scale settings, and frequencies
            drawGrid(XSIZE, YSIZE, XDIVISIONS, YDIVISIONS, XMARGIN, YMARGIN);       
            printScaleSettings(200, 15, XSCALE_STATE, YSCALE_STATE);
            printFrequency(10, 15, array_freq);
        }else if(OSCILLO_STATE==RUN_STATE && RUNNING_STATE == TRIGGER){
            drawTriggerRunning(TRIGGER_LEVEL_STATE, XSCALE_STATE, YSCALE_STATE, array10000, array5000, array2000, array1000, array500, array200, array100, TRIGGER_SLOPE_STATE);
            //Redraw the grid, scale settings, and frequencies
            drawGrid(XSIZE, YSIZE, XDIVISIONS, YDIVISIONS, XMARGIN, YMARGIN);       
            printScaleSettings(200, 15, XSCALE_STATE, YSCALE_STATE);
            printFrequency(10, 15, array_freq);        
        }
    }    
}

