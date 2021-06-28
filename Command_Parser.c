/* ========================================
 * Vuong Do
 * Command_Parser.C
 * ========================================
*/
#include "Command_Parser.h"
#include "project.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "GUI.h"
#include "stdlib.h"
#include "math.h"


//Argument Parsing Function
void argumentParse(char* str, int* OSCILLO_STATE, int* RUNNING_STATE, int* TRIGGER_SLOPE_STATE, int* TRIGGER_CHAN_STATE, int* TRIGGER_LEVEL_STATE, int* XSCALE_STATE, int* YSCALE_STATE){
    char* token;   
    char* token2;
    char* error = "Invalid Command. Available options:\r\n   -set mode [free/trigger]\r\n   -set trigger_level [multiples of 100 up to 3300]\r\n   -set trigger_slope [positive/negative]\r\n   -set trigger_channel [1/2]\r\n   -set yscale [500/1000/1500/2000]\r\n -set xscale [100/200/500/1000/2000/5000/10000]\r\n";
    char* state_error = "Command cannot be called. System first needs to be stopped.\r\n";

    //Check if the string is only "start", otherwise, print error
    token = strtok(str, " \r\n");
    if(strcmp(token, "start")==0){
        token = strtok(NULL, " \r\n");
        if(token == NULL){
            printf("Hi. Starting oscilloscope.\r\n");
            *OSCILLO_STATE = RUN_STATE;
        }else{
            printf("%s", error);
        }
    //Check if the string is only "stop", otherwise, print error    
    }else if(strcmp(token, "stop")==0){
        token = strtok(NULL, " \r\n");
        if(token == NULL){
            printf("Stopping oscilloscope.\r\n");
            *OSCILLO_STATE = STOP_STATE;
        }else{
            printf("%s", error);
        }        
    }else{    
        if(strcmp(token, "set")!=0){
            printf("%s", error);
        }else{
            if(token != NULL){
                token = strtok(NULL, " \r\n");
                if(token != NULL){
                    if(strcmp(token, "mode")==0){
                        token = strtok(NULL, " \r\n");
                        if(token != NULL){
                            if(strcmp(token, "free")==0){
                                token = strtok(NULL, " \r\n");
                                if(token == NULL){
                                    //Check if the string is only "set mode free" and the oscilloscope is stopped, otherwise, print error
                                    if(*OSCILLO_STATE == STOP_STATE){
                                        printf("Setting mode to free.\r\n");
                                        //Change the mode of the oscilloscope to FREE RUNNING
                                        *RUNNING_STATE = FREE_RUNNING;
                                    }else{
                                        printf("%s", state_error);
                                    }
                                }else{
                                    printf("%s", error);
                                }
                            }else if(strcmp(token, "trigger")==0){
                                token = strtok(NULL, " \r\n");
                                if(token == NULL){
                                    //Check if the string is only "set mode trigger" and the oscilloscope is stopped, otherwise, print error
                                    if(*OSCILLO_STATE == STOP_STATE){
                                        printf("Setting mode to trigger.\r\n");
                                        //Change the mode of the oscilloscope to TRIGGER
                                        *RUNNING_STATE = TRIGGER;
                                    }else{
                                        printf("%s", state_error);
                                    }
                                }else{
                                    printf("%s", error);
                                }
                            }else{
                                printf("%s", error);
                            }
                        }else{
                            printf("%s", error);
                        }
                    }else if(strcmp(token, "trigger_level")==0){
                        token = strtok(NULL, " \r\n");
                        if(token != NULL){
                            token2 = strtok(NULL, " \r\n");
                            if(token2 == NULL){
                                //Check if the string is only "set trigger_level X", otherwise, print error
                                if(*OSCILLO_STATE == STOP_STATE){
                                    int trigger_level = atoi(token);
                                    //Check if the trigger level is a valid number, otherwise, print error
                                    if(trigger_level%100==0 && trigger_level<=3300){
                                        printf("Setting trigger level to %s\r\n", token);
                                        //Change the trigger level value to the string's value
                                        *TRIGGER_LEVEL_STATE = atoi(token);
                                    }else{
                                        printf("%s", error);
                                    }
                                }else{
                                    printf("%s", state_error);
                                }
                            }else{
                                printf("%s", error);
                            }
                        }else{
                            printf("%s", error);
                        }                      
                    }else if(strcmp(token, "xscale")==0){
                        token = strtok(NULL, " \r\n");
                        if(token != NULL){
                            //Check if the string is only "set xscale X" and X is a valid number, otherwise, print error
                            token2 = strtok(NULL, " \r\n");
                            if(token2 == NULL && (strcmp(token, "100")==0 || strcmp(token, "200")==0 || strcmp(token, "500")==0 || strcmp(token, "1000")==0 || strcmp(token, "2000")==0 || strcmp(token, "5000")==0 || strcmp(token, "10000")==0)){
                                printf("Setting Xscale to %s\r\n", token);
                                //Change xscale setting of the scope to X
                                *XSCALE_STATE = atoi(token);
                            }else{
                                printf("%s", error);
                            }
                        }else{
                            printf("%s", error);
                        }
                    }else if(strcmp(token, "yscale")==0){
                        token = strtok(NULL, " \r\n");
                        if(token != NULL){
                            //Check if the string is only "set yscale Y" and Y is a valid number, otherwise, print error
                            token2 = strtok(NULL, " \r\n");
                            if(token2 == NULL && (strcmp(token, "500")==0 || strcmp(token, "1000")==0 || strcmp(token, "1500")==0 || strcmp(token, "2000")==0)){
                                printf("Setting Yscale to %s\r\n", token);
                                //Change yscale setting of the scope to Y
                                *YSCALE_STATE = atoi(token);
                            }else{
                                printf("%s", error);
                            }
                        }else{
                            printf("%s", error);
                        }                        
                    }else if(strcmp(token, "trigger_slope")==0){
                        token = strtok(NULL, " \r\n");
                        if(token != NULL){
                            if(strcmp(token, "positive")==0){
                                token = strtok(NULL, " \r\n");
                                if(token == NULL){
                                    //Check if the string is only "set trigger_slope positive" and the oscilloscope is stopped, otherwise, print error
                                    if(*OSCILLO_STATE == STOP_STATE){
                                        printf("Setting trigger slope to positive.\r\n");
                                        //Change the slope setting to POSITIVE SLOPE
                                        *TRIGGER_SLOPE_STATE = POS_SLOPE;
                                    }else{
                                        printf("%s", state_error);
                                    }    
                                }else{
                                    printf("%s", error);
                                }
                            }else if(strcmp(token, "negative")==0){
                                token = strtok(NULL, " \r\n");
                                if(token == NULL){
                                    //Check if the string is only "set trigger_slope negative" and the oscilloscope is stopped, otherwise, print error
                                    if(*OSCILLO_STATE == STOP_STATE){
                                        printf("Setting trigger slope to negative.\r\n");
                                        //Change the slope setting to NEGATIVE SLOPE
                                        *TRIGGER_SLOPE_STATE = NEG_SLOPE;
                                    }else{
                                        printf("%s", state_error);
                                    }
                                }else{
                                    printf("%s", error);
                                }
                            }else{
                                printf("%s", error);
                            }
                        }else{
                            printf("%s", error);
                        }
                    }else if(strcmp(token, "trigger_channel")==0){
                        token = strtok(NULL, " \r\n");
                        if(token != NULL){
                            if(strcmp(token, "1")==0){
                                token = strtok(NULL, " \r\n");
                                if(token == NULL){
                                    //Check if the string is only "set trigger_channel 1" and the oscilloscope is stopped, otherwise, print error
                                    if(*OSCILLO_STATE == STOP_STATE){
                                        printf("Setting trigger channel to 1.\r\n");
                                        *TRIGGER_CHAN_STATE = TRIGGER_CHAN_1;
                                    }else{
                                        printf("%s", state_error);
                                    }
                                }else{
                                    printf("%s", error);
                                }
                            }else if(strcmp(token, "2")==0){
                                token = strtok(NULL, " \r\n");
                                if(token == NULL){
                                    //Check if the string is only "set trigger_channel 1" and the oscilloscope is stopped, otherwise, print error
                                    if(*OSCILLO_STATE == STOP_STATE){
                                        printf("Setting trigger channel to 2.\r\n");
                                        *TRIGGER_CHAN_STATE = TRIGGER_CHAN_2;
                                    }else{
                                        printf("%s", state_error);
                                    }
                                }else{
                                    printf("%s", error);
                                }
                            }else{
                                printf("%s", error);
                            }
                        }else{
                            printf("%s", error);
                        }
                    }else{
                        printf("%s", error);
                    }
                }else{
                    printf("%s", error);
                }
            }else{
                printf("%s", error);
            }
        }
    } 
}