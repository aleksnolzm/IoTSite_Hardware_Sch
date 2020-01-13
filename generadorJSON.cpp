#include <TimerOne.h>
volatile bool flagUpdateDataCloud = false; 
volatile int minutes = 0;
volatile int seconds = 0;
volatile int minutesSet = 2;
void readAndSendData(void);
String jsonString = " ";

volatile int tempSiteRandom;
volatile float humSiteRandom;
volatile int tempTransmiterRandom;

String bufffer;
/*******************************************************************************************/
void setup() {
  Timer1.initialize(1000000);         // Dispara cada 1000ms = 1seg
  Timer1.attachInterrupt(triggerPayload); // Activa la interrupcion y la asocia a ISR_Blink TIMEON
  Serial.begin(9600);
}

void loop() {
   while(flagUpdateDataCloud)
      {
          readAndSendData(); // Leemos y enviamos los valores de los sensores a la nube Sigfox   
          noInterrupts();
          flagUpdateDataCloud=false;
          interrupts();
       }
}

/*********************** FUNCION PARA ENVIO DE INFORMACION PROGRAMADO POR TIMER ******************************/
   void triggerPayload() // Rutina de interrupcion interna TIMER1 cada 10 minutos.
   {
      noInterrupts();
    
      if(seconds==59){
        seconds=0;
        minutes++;
      }
      seconds++;
      flagUpdateDataCloud=true;
      
 //-----------------------------------------------------------------------------------------------------------
      while(minutes==minutesSet){
          seconds=0;
          minutes=0;
      } 
      interrupts();
   }

 //-------------------------------------------------------------------------------
void readAndSendData(void)
{ 
  tempSiteRandom = random(1,50);
  humSiteRandom = random(10,99);
  tempTransmiterRandom = random(1,50);
  
  bufffer = String(tempSiteRandom);

  //jsonString = "{ \"name\": \"642\" }";
  
  /*jsonString = "{ \"tempSite\" : \"";
  jsonString += tempSiteRandom;
  jsonString += "\" }"; */

  
  jsonString = "{ \"tempSite\": \"";
  jsonString += tempSiteRandom;
  jsonString += "\", \"humSite\": \"";
  jsonString += humSiteRandom;
  jsonString += "\", \"tempTransmiter\": \"";
  jsonString += tempTransmiterRandom;
  jsonString += "\" }";
  Serial.print(jsonString);
  Serial.println(" ");

 
}
 
