/************************************************************************
*  MAIN SOURCE CODE FOR MONITORING ENVIRONMENTAL VARIABLES INSIDE THE SITE
*  Codigo realizado por:
*  Esteban Alejandro Montellano Nolasco
*  Edgar Bernardo Olvera Salazar
*  Rev. 1.6.0 GIT  Fecha: 19 Diciembre 2019 
************************************************************************/
#include <TimerOne.h>
#include "serialWisollib.h";
#include "mainTransducers.h";
/********************** GLOBAL SCOPE VARIABLES *************************/
// {volatile variables for TIMER1}
volatile int minutes = 0; // contador de minutos
volatile int seconds = 0; // contador de segundos
volatile int minutesSet = 10;  // Variable para establecer envio de datos cada 10 minutos
volatile bool ledStatus = false;
/************************ FUNCTION SETUP  ************************************/
void setup(void)
   {  
       
       Timer1.initialize(1000000);         // Dispara cada 1000ms = 1seg
       Timer1.attachInterrupt(triggerPayload); // Activa la interrupcion y la asocia a ISR_Blink TIMEON

       attachInterrupt(digitalPinToInterrupt(2), sendForcedPayload, RISING);
       //attachInterrupt(digitalPinToInterrupt(2), sendForcedPayload, FALLING);
       initTransducers(); // Inicializamos a los sensores y el modulo Wisol
       Serial.begin(9600);

       pinMode(8, OUTPUT); //LED 
   }

/******************************* RUTINA PRINCIPAL **************************************/
   void loop(void)
   {
      while(flagUpdateDataCloud)
      {
         performUplink(); // Leemos y enviamos los valores de los sensores a la nube Sigfox   
         noInterrupts();
         flagUpdateDataCloud=false;
         interrupts();
      }
      while(flagUpdateDataCloud==false)
     {  digitalWrite(8,true);
         delay(20);
        digitalWrite(8,false);
        readVariables();
        delay(500);

     } 
      
   }

/*********************** FUNCION PARA ENVIO DE INFORMACION PROGRAMADO POR TIMER ******************************/
   void triggerPayload() // Rutina de interrupcion interna TIMER1 cada 10 minutos.
   {
      noInterrupts();
    
      if(seconds==60){
        seconds=0;
        minutes++;
      }
      seconds++;
//-----------------------------------------------------------------------------------------------------------
      while(minutes==minutesSet){
          flagUpdateDataCloud=true;
          seconds=0;
          minutes=0;
      } 
      interrupts();
   }
/**************** FUNCION PARA FORZAR ENVIO DE INFORMACION POR INTERRUPCION EXTERNA *************************/
    void sendForcedPayload(){
      noInterrupts();
      flagUpdateDataCloud=true;
      interrupts();
    }
