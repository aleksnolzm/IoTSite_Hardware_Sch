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
#include <max6675.h>
#include "serialWisollib.h";
#include <TimerOne.h>
#include "nextionLibRoutine.h";
/********************** GLOBAL SCOPE VARIABLES *************************/
volatile bool flagUpdateDataCloud = false;  // Bandera para ejecutar la funcion de lectura de variables de sensores

int humBuffer = 0;
int tempBuffer = 0;
float humVolts = 0;
float tempVolts = 0;
uint8_t crudeTempData = 0; // Datos de Temperatura a 1byte
uint16_t crudeHumData = 0; // Datos de Humedad a 2 bytes
uint8_t crudeThermopairData = 0; // Datos de Temperatura de Termopar a 2bytes
byte statusDoor = 0;
byte statusAC = 1;

float tempThermopair = 0; //  Datos de Temperatura de termopar
/********************* PUERTOS DIGITALES SPI **************************/
unsigned int ktcS0=11;
unsigned int ktcCS=12;
unsigned int ktcCLK=13;
/********************* PUERTOS ANALOGICOS *******************************/
byte humPort = A5;  //Numero de puerto analogico para sensor de humedad
byte tempPort = A0; //Numero de puerto analogico para sensor de temperatura
/************************ FUNCTIONES PREDEFINIDAS **************************/
MAX6675 ktc(ktcCLK, ktcCS, ktcS0);  // Determinando puerto SPI
/************************ FUNCTIONES PROTOTIPO **************************/
void initTransducers(void); // Inicializa el puerto serial del modulo Wisol, entradas analogicas de sensores etc...
void readAndSendData (void); // Funcion rutina para leer y enviar los valores obtenidos de los sensores a la nube
float readTempData (void); // Lee el sensor de temperatura, devuelve un valor flotante
float readHumData (void);  // Lee el sensor de humedad, devuelve un valor flotante
float readThermocoupleData(void);

wisolDev devModule; //declaramos el modulo como un objeto
/****************************** SETUP PORTS ************************************************/
void initTransducers (void)
{
    Serial.begin(9600);
    setupNextion();
    devModule.wisolDevInit(); //inicializamos el modulo
}
/****************************** MAIN READING SENSORS VARIABLES ******************************/
void readAndSendData(void) 
{
  devModule.clearBuffer(); // Limpiamos buffer de Payload
  devModule.initPayload(); // Iniciamos la trama de Payload
//----------------- Lectura de Humedad Relativa de Site ---------------
  crudeHumData = (readHumData()*10); // Leemos humedad relativa
  devModule.addTwoBytes(crudeHumData); // Añadimos el valor leído al buffer
//----------------- Lectura de Temperatura Ambiente de Site -----------
  crudeTempData = readTempData();  // Leemos temperatura
  devModule.addOneByte(crudeTempData); // Añadimos el valor leído al buffer 
//----------------- Lectura de Temperatura de Transmisor -------------- 
  crudeThermopairData = readThermocoupleData();
  devModule.addOneBytes(crudeThermopairData);
//----------------------------------------------------------------------

  
  serial.print(crudeHumData);
  serial.print(" ");
  serial.print(crudeTempData);
  serial.print(" ");
  serial.println(crudeThermopairData);

  sendDataNextion(crudeThermopairData, crudeTempData, crudeHumData);

//----------------------------------------------------------------------
  devModule.sendPayload(); // Envíamos la trama de Payload al servidor Sifgox
  Serial1.print("\n");
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
/************************** FUNCION PARA LEER SENSOR DE TEMPERATURA DE TRANSMISOR ****************************************/
float readThermocoupleData(void)
{
  tempThermopair = ktc.readCelsius();
  
  if(tempThermopair > 0)
  {
    return tempThermopair;
  }
  else{
    return 0.00001;
  }
}
/***********************************************************************************************/
#endif
