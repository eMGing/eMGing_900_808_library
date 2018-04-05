/***************************************************
  # Proyecto de ejemplo para chequeo del Shield eMGing 900/808

--(Esta librería requiere Arduino v1.0.6 o superior)--

Esta librería se codeo especialmente para manejar los dos Shields GSM-GPRS que desarrolló eMGing
Nuestra filosofía en eMGing, al igual que Arduino es el Hardware y Software abierto, por lo 
que no reinventamos la rueda, si existe ya algo bien hecho, lo tomamos y en todo caso lo mejoramos
Esta librería se armo en base a muchas librerías que estan dando vuelta por internet, tomando
lo mejor de cada una, para sacar mejor provecho de ésto, y mejorandola para el provecho y uso de 
nuestros clientes.

Desarrollada especialmente para trabajar con los módulos Shield eMGing GSM-GPRS
  * http://www.emging.com.ar/product/shield-emging-808-gsm-gprs-2g-gps-usd-arduino-iot/
  * http://www.emging.com.ar/product/shield-emging-900-gsm-gprs-2g-usd-arduino-iot/


Por Gustavo Marek para eMGing Electrónica Argentina.  
Licencia BSD, Todo el texto aquí descripto debe estar incluído en cualquier tipo de distribución de esta u otra librería

Ubicar la carpeta de la librería ( que debe incluír los archivos: eMGing_900_808.cpp e eMGing_900_808.h ) y colocarla en la
siguiente ubicación: " documentos/Arduino/librerías/ " si no existe crear la subcarpeta y luego de copiar alli la librería
reiniciar el IDE Arduino para que tome la nueva librería.


Entren a la página oficial donde encontrarán los tutoriales y los esquemas de conexión para ambos módulos,
o pueden ver los links siguientes para entrar directamente:

  * http://www.emging.com.ar/2016/07/06/shield-gsm-gprs-sim900/
  * http://www.emging.com.ar/2016/07/20/shield-gsm-gprs-gps-sim808/

****************************************************/

/*
Abrir la consola Serial en 9600 baud para ver el menu de opciones.

Si necesitas setar los datos GPRS como APN, username, password podes hacerlo al final del setup
y descomentar esas lineas para que quede seteado correctamente.
( ATENCION!, estos datos te los puede suministrar tu compañia de celular )

Nota:

 Para SoftwareSerial usamos los pines 7 y 8 ( TX,RX del shield respectivamente ),
 por lo que para otras placas compatibles con el Shield se debe cambiar el pin RX,
 
 No todos los pines del MEGA y MEGA2560 pueden ser usados para cambiar interrupciones,
 por lo que solo los siguientes pines pueden ser usados para RX:
 10, 11, 12, 13, 53, 62, 63, 64, 65, 66, 67, 68, 69

 No todos los pines del LEONARDO y MICRO pueden ser usados para cambiar interrupciones,
 por lo que solo los siguientes pines pueden ser usados para RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

*/

/* Ejemplo de llamada entrante para el modulo eMGing
 Escucha a una llamada y luego muestra el numero de telefono del que llama, si es posible.
 Use este ejemplo para detectar su numero de telefono en tu propio proyecto !.
*/
 
#include "eMGing_900_808.h"

#define EMGING_RX 8    // corresponde al pin TX del Arduino UNO/MEGA
//PARA ARDUINO UNO
//#define EMGING_TX 7    // PARA SHIELD eMGING 808/900 v1.0 corresponde al pin RX del Arduino UNO
#define EMGING_TX 10    // PARA SHIELD eMGING 808/900 v1.1 corresponde al pin RX del Arduino UNO
//PARA ARDUINO MEGA
//#define EMGING_TX 62    // PARA SHIELD eMGING 808/900 v1.0 corresponde al pin RX del Arduino MEGA 
//#define EMGING_TX 10    // PARA SHIELD eMGING 808/900 v1.1 corresponde al pin RX del Arduino UNO
#define EMGING_RST 4

// en esta aplicacion, puedes conectar el pin de RI del shield
// al pin 2 del Arduino, para esto debes conectar fisicamente 
// el RI con el pin 2 del arduino.
// Luego hay que mapear el numero de interrupcion con el numero 
// de pin de tu placa. en el Arduino UNO & MEGA, la interrupcion
// numero 0 esta en el pin 2 del Arduino, pero para la Leonardo,
// esta en el pin 3.
// Miren en esta página para ver la tabla completa :
//   http://arduino.cc/en/Reference/attachInterrupt
// Acegurate de que el pin de interrupcion este conectado 
// fisicamente en el pin de RI del Shield eMGing 900/808

#define EMGING_RI_INTERRUPT  0

/// Estos Shield usan por defecto software serial. Si quieren usar hardware serial
// deben soldar los pines correctamente en la parte de abajo del Shield para que 
// los puertos 0 y 1 esten conectados a la UART del GPRS, para esto deben comentar
// las 3 líneas a continuación y descomentar las líneas de HARDWARE SERIAL 
#include <SoftwareSerial.h>
SoftwareSerial eMGingSS = SoftwareSerial(EMGING_TX, EMGING_RX);
SoftwareSerial *eMGingSerial = &eMGingSS;

// Hardware serial is also possible!
//  HardwareSerial *eMGingSerial = &Serial1;

eMGing_900_808 eMGing = eMGing_900_808(4);

void setup() {
  Serial.begin(9600);
  Serial.println(F("Ejemplo de llamada entrante"));
  Serial.println(F("Inicializando"));

  eMGingSerial->begin(9600);
  if (! eMGing.begin(*eMGingSerial)) {
    Serial.println(F("No se encontro el shield"));
    while (1);
  }
  Serial.println(F("El Shield eMGing esta OK"));

  // Habilitando notificacion de llamada entrante.
  if(eMGing.callerIdNotification(true, EMGING_RI_INTERRUPT)) {
    Serial.println(F("Notificacion del Caller id habilitada."));
  }
  else {
    Serial.println(F("Notificacion del Caller id deshabilitada."));
  }
}

void loop(){
  // Creacion del un buffer de string para guardar el numero de la llamada entrante.
  char phone[32] = {0};
  // Chequeo de una llamada entrante. Devolvera true si hay una llamada entrante.
  if(eMGing.incomingCallNumber(phone)){
    Serial.println(F("RING!"));
    Serial.print(F("Phone Number: "));
    Serial.println(phone);
  }
}
