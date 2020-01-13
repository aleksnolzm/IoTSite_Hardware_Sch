- El archivo timerOne.zip adjunto a los archivos de cabecera debe ser instalado como una libreria mediante el gestor de librerias de Arduino para poder utilizar las funciones de interrupciones que son necesarias para este programa.

- El programa contiene 3 archivos, uno principal en formato Arduino "main.ino" y dos cabeceras que contienen funciones externas que la rutina principal del programa llamará.

- El programa hace uso de rutinas de espera para ejecutar las tareas cada cierto tiempo programado mediante desbordamiento de Temporizador "Timer1" y un temporizador externo configurado como interrupcion externa INT0

- El archivo "main.ino" contiene la funcion principal y las interrupciones prinicipales así como el tiempo pre-configurado cada 10 minutos para ejecutar la rutina por "Timer1" mientras la interrupcion externa utiliza un flanco de subida para ser llamada mediante "Timer2"

- La librería mainTransducers.h" contiene las funciones necesarias para realizar la lectura de los sensores utilizados asi como la conversion necesaria para añadir los valores en hexadecimal a la trama de Payload que serán enviados a la nube de Sigfox en el formato requerido por la plataforma en UTF-8 HEX Neutral

- La librería "serialWisollib.h" contiene las funciones necesarias para trabajar el modulo Wisol de Sigfox y podamos enviar, recibir y configurar la conexión de red en la plataforma, el modulo Wisol deberá ser llamado mediante POO para diferenciar el bufer de otras tareas que realicen las mismas funciones en la rutina, por tanto las funciones llamadas son invocadas mediante objetos.