
#ifndef nextionLibRoutine_h
#define nextionLibRoutine_h
#include "Arduino.h";
#include <Nextion.h>
#include <stdio.h>
#include <stdlib.h>

/********************** J0 N0 DECLARACION OBJETO TEMPERATURA DE TRANSMISOR ****************************/
//pagina 1
NexText t0 = NexText(0, 3, "t0");

/********************** J1 N1 DECLARACION OBJETO TEMPERATURA AMBIENTE DE SITE *************************/
//pagina 2
NexText t1 = NexText(0, 4, "t1");

/********************** J2 N2 DECLARACION OBJETO HUMEDAD RELATIVA DE SITE **********************+******/
//pagina 3
NexText t2 = NexText(0, 5, "t2");

/****************************************************************************************************+*/
/********************** PROTOTPIPO DE FUNCIONES PARA DISPLAY NEXTION **********************************/
void setupNextion(void);
void sendDataNextion(uint8_t dataTempTxSiteDisp, uint8_t dataTempSiteDisp, uint16_t dataHumSiteDisp);


/********************** FUNCIONES DE DISPLAY NEXTION **********************+******/

void setupNextion(void) {
  nexInit(); //inicializar la pantalla
}

void sendDataNextion(uint8_t dataTempSiteDisp, uint8_t dataTempTxSiteDisp, uint16_t dataHumSiteDisp) {
  
  // Temperatura TX Site         => n0  
  // Temperatura Ambiente Site   => n1 
  // Humedad Relativa Site       => n2 

  //nexLoop(NULL); // Service the Nextion display

  String strData0 = String(dataTempTxSiteDisp);
  String strData1 = String(dataTempSiteDisp);

  float fData2 = dataHumSiteDisp*0.1;
  String strData2 = String(fData2);
   
  t0.setText(strData2); //Humedadº
  t1.setText(strData1); //TemperaturaSite
  t2.setText(strData0); //TemperaturaTx
    
  delay(500); 
}

#endif
