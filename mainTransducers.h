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
volatile bool statusDoor = false;
volatile bool statusEnergy = false;

int crudeStatusVars = 0; // Bandera para enviar notificacion de puerta y energia
int humBuffer = 0;
int tempBuffer = 0;
float humVolts = 0;
float tempVolts = 0;
uint8_t crudeTempData = 0; // Datos de Temperatura a 1byte
uint16_t crudeHumData = 0; // Datos de Humedad a 2 bytes
uint8_t crudeThermopairData = 0; // Datos de Temperatura de Termopar a 2bytes

byte statusDoor = 0;
byte statusAC = 1;

volatile float tempThermopair = 0; //  Datos de Temperatura de termopar
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
void performUplink (void); // Funcion rutina para leer y enviar los valores obtenidos de los sensores a la nube
float readTempData (void); // Lee el sensor de temperatura, devuelve un valor flotante
float readHumData (void);  // Lee el sensor de humedad, devuelve un valor flotante
float readThermocoupleData(void); // Lee el termopar
void readVariables(void);

wisolDev devModule; //declaramos el modulo como un objeto
/****************************** SETUP PORTS ************************************************/
void initTransducers (void)
{
    Serial.begin(9600);
    setupNextion();
    devModule.wisolDevInit(); //inicializamos el modulo
}
/****************************** MAIN READING SENSORS VARIABLES ******************************/
void performUplink(void) 
{ 
//----------------- Lee Variables de entorno ---------------
  readVariables();
//----------------------------------------------------------------------
  Serial.println("Wait a moment please...");
  devModule.clearBuffer(); // Limpiamos buffer de Payload
  devModule.initPayload(); // Iniciamos la trama de Payload

  devModule.addTwoBytes(crudeHumData); // Añadimos el valor leído al buffer
  devModule.addOneByte(crudeTempData); // Añadimos el valor leído al buffer 
  devModule.addOneByte(crudeThermopairData);
  devModule.addOneByte(crudeStatusVars);
  
  Serial.print("Payload Status: ");
  Serial.println(devModule.bufer);

  devModule.sendPayload(); // Envíamos la trama de Payload al servidor Sifgox
  Serial1.print("\n");
  devModule.clearBuffer(); // Limpiamos buffer de Payload
  Serial.println("Payload OK!");
  Serial.println(" ");
}


/************************* FUNCION PARA LEER SENSOR DE HUMEDAD *********************************************/
float readHumData (void)
{
  humBuffer = analogRead(humPort);
  delay(40);
  
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
  delay(50);
   
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
  delay(15);
  
  if(tempThermopair > 0)
  {
    return tempThermopair;
  }
  else{
    return 0.00001;
  }
}
/***********************************************************************************************/
void readVariables(void){

  //----------------- Lectura de Humedad Relativa de Site ---------------
  crudeHumData = (readHumData()*10); // Leemos humedad relativa 
//----------------- Lectura de Temperatura Ambiente de Site -----------
  crudeTempData = readTempData();  // Leemos temperatura
//----------------- Lectura de Temperatura de Transmisor -------------- 
  crudeThermopairData = readThermocoupleData();
//----------------------------------------------------------------------
  delay(50);

  crudeStatusVars = statusDoor + statusEnergy;
  /*Serial.print(crudeHumData);
  Serial.print("%RH ");
  Serial.print(crudeTempData);
  Serial.print("ºC TempSite  ");
  Serial.print(crudeThermopairData);
  Serial.println("ºC Tx  ");
  Serial.println(devModule.bufer);*/
  delay(100);
 //---------------------------------------------------------------------- 
  sendDataNextion(crudeThermopairData, crudeTempData, crudeHumData); // Envia variables a pantalla
}
#endif
