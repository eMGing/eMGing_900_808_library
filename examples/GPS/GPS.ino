/***************************************************
  # Proyecto de ejemplo GPS para chequeo del Shield eMGing 808

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
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 No todos los pines del LEONARDO y MICRO pueden ser usados para cambiar interrupciones,
 por lo que solo los siguientes pines pueden ser usados para RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

*/

//Abrir la consola Serial en 9600 baud para ver el menu de opciones.

#include "eMGing_900_808.h"


#define EMGING_RX 8
#define EMGING_TX 7
#define EMGING_RST 4

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

  while (! Serial);

  Serial.begin(9600);
  Serial.println(F("Ejemplo con Shield eMGing 808 - GPS"));
  Serial.println(F("Inicializando"));

  eMGingSerial->begin(9600);
  if (! eMGing.begin(*eMGingSerial)) {
    Serial.println(F("No se encontro el shield"));
    while (1);
  }
  Serial.println(F("El Shield eMGing esta OK"));
  
  Serial.println(F("Habilitando GPS..."));
  eMGing.enableGPS(true);
}

void loop() {
  delay(2000);

  float latitude, longitude, speed_kph, heading, speed_mph, altitude;

  // if you ask for an altitude reading, getGPS will return false if there isn't a 3D fix
  boolean gps_success = eMGing.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude);

  if (gps_success) {

    Serial.print("GPS lat:");
    Serial.println(latitude, 6);
    Serial.print("GPS long:");
    Serial.println(longitude, 6);
    Serial.print("GPS speed KPH:");
    Serial.println(speed_kph);
    Serial.print("GPS speed MPH:");
    speed_mph = speed_kph * 0.621371192;
    Serial.println(speed_mph);
    Serial.print("GPS heading:");
    Serial.println(heading);
    Serial.print("GPS altitude:");
    Serial.println(altitude);

  } else {
    Serial.println("Esperando a GPS 3D fix...");
  }

  if ((eMGing.type() == EMGING_3G_A) || (eMGing.type() == EMGING_3G_E))
    return;
  // Check for network, then GPRS 
  Serial.println(F("Chequando red celular..."));
  if (eMGing.getNetworkStatus() == 1) {
    // red y GPRS ? perfecto, muestra la localización GSM para comparar!
    boolean gsmloc_success = eMGing.getGSMLoc(&latitude, &longitude);

    if (gsmloc_success) {
      Serial.print("GSMLoc lat:");
      Serial.println(latitude, 6);
      Serial.print("GSMLoc long:");
      Serial.println(longitude, 6);
    } else {
      Serial.println("Fallo la localización GSM...");
      Serial.println(F("Desabilitando GPRS"));
      eMGing.enableGPRS(false);
      Serial.println(F("Habilitando GPRS"));
      if (!eMGing.enableGPRS(true)) {
        Serial.println(F("Fallo el encendido del GPRS"));  
      }
    }
  }
}

