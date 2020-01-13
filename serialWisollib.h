/************************************************************************
*  WISOL LIBRARY CODE FOR MONITORING ENVIRONMENTAL VARIABLES INSIDE THE SITE
*  Codigo realizado por:
*  Esteban Alejandro Montellano Nolasco
*  Edgar Bernardo Olvera Salazar
*  Rev. 1.0.9 GIT  Fecha: 18 Diciembre 2019 
************************************************************************/
#ifndef serialWisollib_h
#define serialWisollib_h
#include "Arduino.h";
#include <TimerOne.h>

const int enableModule = 7; // Enable modulo Wisol

class wisolDev
{   
	public: 
	wisolDevInit(); // Inicializa el puerto serial para el modulo Wisol RS232
  void clearBuffer(); // Limpia el buffer del Payload
	void initPayload(); // Inicializa la trama del Payload
	void sendPayload(); // Envía el Payload completado a la nube Sigfox
  void addFloat(float var1); // Agrega un valor flotante por conversión directa a HEX a la trama de Payload
	void addOneByte(int var2); // Agrega un valor de 1 Byte a la trama de Payload ideal para valores enteros
  void addTwoBytes(int var3) ; // Agrega un valor de 2 Bytes a la trama de Payload ideal para valores flotantes
	
	String bufer; //variable donde guardaremos nuestro payload
};

/*****************************************************************************************************************/
wisolDev::wisolDevInit()
{
  
  bufer = " ";
	Serial.begin(9600);
	pinMode(enableModule, OUTPUT);   //enable modulo wisol
	bufer="AT\n";
  Serial.print(bufer);
  delay(1000);
  bufer = " "; 
}
/************************************************************************/
void wisolDev::clearBuffer(){
  bufer =" ";
}

/*****************************************************************************************************************/
void wisolDev::initPayload()
{
	bufer="AT$SF=";
}
/*****************************************************************************************************************/
void wisolDev::sendPayload()
{
  //agregamos el salto de linea "\n"
  bufer+="\n";
  //*******************
  //Habilitamos el modulo Sigfox
  digitalWrite(enableModule, HIGH);
  delay(1000);
  //Reset del canal para asegurar que manda en la frecuencia correcta
  Serial.print("AT$RC\n"); 
  delay(1000);
  //************************
  //Enviamos la informacion por sigfox
  Serial.print(bufer);
  delay(3800);
  //deshabilitamos el modulo Sigfox
  digitalWrite(enableModule, LOW);
}
/*****************************************************************************************************************/
void wisolDev::addFloat(float var1) //funcion para agregar flotantes al payload
{
  byte* a1 = (byte*) &var1;    //convertimos el dato a bytes
  String str1;
  //agregamos al comando AT$SF= nuestra informacion a enviar
  for(int i=0;i<4;i++)
  {
    str1=String(a1[i], HEX);    //convertimos el valor hex a string 
    if(str1.length()<2)
    {
      bufer+=0+str1;    //si no, se agrega un cero
    }
    else
    {
      bufer+=str1;    //si esta completo, se copia tal cual
    }
  }
}
/*****************************************************************************************************************/
void wisolDev::addOneByte(int var2)    //funcion para agregar enteros al payload (hasta 255)
{
  uint8_t* a2 = (uint8_t*) &var2; //convertimos el dato a bytes
  String str2;
  str2=String(a2[0], HEX);  //convertimos el valor hex a string 
  //verificamos si nuestro byte esta completo
  if(str2.length()<2)
  {
    bufer+="0"+str2;    //si no, se agrega un cero
  }
  else
  {
    bufer+=str2;     //si esta completo, se copia tal cual
  }
}

/*****************************************************************************************************************/
void wisolDev::addTwoBytes(int var3)    //funcion para agregar enteros al payload (hasta 255)
{
  uint16_t* a3 = (uint16_t*) &var3; //convertimos el dato a bytes
  String str3;
  str3=String(a3[0], HEX);  //convertimos el valor hex a string 
  //verificamos si nuestro byte esta completo
  if(str3.length()==1)
  {
    bufer+= "000" +str3;    //si no, se agregan tres ceros
  }
  if(str3.length()==2)
  {
    bufer+= "00"+str3;    //si no, se agregan dos ceros
  }
  if(str3.length()==3)
  {
    bufer+="0"+str3;    //si no, se agrega un cero
  }
  if(str3.length()==4)
  {
    bufer+=str3;     //si esta completo, se copia tal cual
  }
}
/***********************************************************************/
#endif
