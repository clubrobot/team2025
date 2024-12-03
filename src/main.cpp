#include <Arduino.h>
#include <STM32FreeRTOS.h>

#include "wheeledbase/wb_thread.h"
#include "screen/screen.h"


#define TEST_NO_FREERTOS true //Ignore le FreeRTOS et se comporte comme un arduino classique

screen::Screen ecran(0, 1, 2, 3, 4, 5, 6, 7, true, 9, 10);

void setup(){
    //Setup de base
    Serial.begin(9600);

    wb_setup();//Setup de la base roulante dans wb_thread

    if(TEST_NO_FREERTOS) {
        return;
    }
    //Setup FreeRTOS

    vTaskStartScheduler();//On commence FreeRTOS
    //On devrait pas être là; Uh oh
    Error_Handler(); //TODO: logger l'error handler
}
long a,b,c,d;

void loop() {
    //loop seuleuement accesssible quand TEST_NO_FREERTOS est à true
    ecran.writeTextAndSend("Hello World");
}
