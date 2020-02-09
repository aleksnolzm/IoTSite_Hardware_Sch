
#ifndef nextionLibRoutine_h
#define nextionLibRoutine_h
#include "Arduino.h";
#include "Nextion.h"

/******************************* VARIABLES GLOBALES EN SCOPE  **********************************+******/

int int_Progress0 = 0; // Barra de estado de Temperatura del Transmisor

int int_Progress1 = 0; // Barra de estado de Temperatura ambiente del Site

int int_Progress2 = 0; // Barra de estado de Humedad del Site

/********************** J0 N0 DECLARACION OBJETO TEMPERATURA DE TRANSMISOR ****************************/
//pagina 1
NexNumber n0 = NexNumber(1, 6, "n0");
NexProgressBar j0 = NexProgressBar(1, 3, "j0");

/********************** J1 N1 DECLARACION OBJETO TEMPERATURA AMBIENTE DE SITE *************************/
//pagina 2
NexNumber n1 = NexNumber(2, 5, "n1");
NexProgressBar j1 = NexProgressBar(2, 3, "j1");

/********************** J2 N2 DECLARACION OBJETO HUMEDAD RELATIVA DE SITE **********************+******/
//pagina 3
NexNumber n2 = NexNumber(3, 3, "n2");
NexProgressBar j2 = NexProgressBar(3, 2, "j2");

/****************************************************************************************************+*/
/********************** PROTOTPIPO DE FUNCIONES PARA DISPLAY NEXTION **********************************/
void setupNextion(void);
void sendDataNextion(uint8_t dataTempTxSiteDisp, uint8_t dataTempSiteDisp, uint16_t dataHumSiteDisp);


/********************** FUNCIONES DE DISPLAY NEXTION **********************+******/

void setupNextion(void) {
  nexInit(); //inicializar la pantalla
}

void sendDataNextion(uint8_t dataTempTxSiteDisp, uint8_t dataTempSiteDisp, uint16_t dataHumSiteDisp) {
  
  // Temperatura TX Site         => n0  => j0
  // Temperatura Ambiente Site   => n1  => j1
  // Humedad Relativa Site       => n2  => j2

  nexLoop(NULL); // Service the Nextion display
/////////////////////////////////////////////////////////////////////////////////////////////////////

  n0.setValue(dataTempTxSiteDisp);
  int_Progress0 = 20 + dataTempTxSiteDisp;
  j0.setValue(int_Progress0);

/////////////////////////////////////////////////////////////////////////////////////////////////////
  n1.setValue(dataTempSiteDisp);
  int_Progress1 = dataTempSiteDisp;
  j1.setValue(int_Progress1);

/////////////////////////////////////////////////////////////////////////////////////////////////////
  n2.setValue((dataHumSiteDisp /10));
  int_Progress2 = ((dataHumSiteDisp /10 ));
  j2.setValue(int_Progress2);
    
  delay(500); 

}

#endif
