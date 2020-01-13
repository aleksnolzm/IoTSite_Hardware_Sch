/************************************************************************
*  LIBRARY TRANSDUCERS SOURCE CODE FOR MONITORING ENVIRONMENTAL VARIABLES INSIDE THE SITE
*  Codigo realizado por:
*  Esteban Alejandro Montellano Nolasco
*  Edgar Bernardo Olvera Salazar
*  Rev. 1.1.2 GIT  Fecha: 18 Diciembre 2019
************************************************************************/
#ifndef mainTransducers_h
#define mainTransducers_h
#include "Arduino.h";
#include "serialWisollib.h";
#include <TimerOne.h>
/********************** GLOBAL SCOPE VARIABLES *************************/
int humBuffer = 0;
int tempBuffer = 0;
float humVolts = 0;
float tempVolts = 0;
uint8_t crudeTempData = 0; // Datos de Temperatura a 1byte
uint16_t crudeHumData = 0; // Datos de Humedad a 2 bytes
byte statusDoor = 0;
byte statusAC = 1;
/********************* PUERTOS ANALOGICOS *******************************/
byte humPort = A5;  //Numero de puerto analogico para sensor de humedad
byte tempPort = A0; //Numero de puerto analogico para sensor de temperatura
/************************ FUNCTION PROTOTYPES **************************/
void initTransducers(void); // Inicializa el puerto serial del modulo Wisol, entradas analogicas de sensores etc...
void readAndSendData (void); // Funcion rutina para leer y enviar los valores obtenidos de los sensores a la nube
float readTempData (void); // Lee el sensor de temperatura, devuelve un valor flotante
float readHumData (void);  // Lee el sensor de humedad, devuelve un valor flotante

wisolDev devModule; //declaramos el modulo como un objeto

/****************************** SETUP PORTS ************************************************/
void initTransducers (void)
{
    devModule.wisolDevInit(); //inicializamos el modulo
}
/****************************** MAIN READING SENSORS VARIABLES ******************************/
void readAndSendData(void) 
{
 devModule.clearBuffer(); // Limpiamos buffer de Payload
 devModule.initPayload(); // Iniciamos la trama de Payload

 crudeHumData = (readHumData()*10); // Leemos humedad relativa
 //Serial.print("Crude Value Hum -> ");
 //Serial.print(crudeHumData);
 devModule.addTwoBytes(crudeHumData); // Añadimos el valor leído al buffer

 
//-------------------------------------------------------------------
 crudeTempData = readTempData();  // Leemos temperatura
 //Serial.print("    Crude Value Temp -> ");
 //Serial.print(crudeTempData);

 devModule.addOneByte(crudeTempData); // Añadimos el valor leído al buffer
 //Serial.print("   Payload buffer -> ");
 //Serial.println(devModule.bufer);
 
//------------------------------------------------------------------- 
 devModule.sendPayload(); // Envíamos la trama de Payload al servidor Sifgox
 Serial.print("\n");
 devModule.clearBuffer(); // Limpiamos buffer de Payload

}


/************************* FUNCION PARA LEER SENSOR DE HUMEDAD *********************************************/
float readHumData (void)
{
  humBuffer = analogRead(humPort);
  
  humVolts = humBuffer*0.00488759; //85532747;
    
  if(humVolts>0.826)
  {
     return ((humVolts - 0.826)/0.0315); //pasamos de un resultado flotante a uno entero para mandarlo en hex crudo
  } 
  else{
   return 0.01;
  }
}
/************************** FUNCION PARA LEER SENSOR DE TEMPERATURA ****************************************/
float readTempData (void)
{
  tempBuffer = analogRead(tempPort);
   
  tempVolts = tempBuffer*0.00488759; //85532747;
    
  if(tempVolts>0)
  {
     return tempVolts*100; //pasamos de un resultado flotante a uno entero para mandarlo en hex crudo
  } 
  else{
   return 0.00001;
  } 
}
/***********************************************************************/
#endif
