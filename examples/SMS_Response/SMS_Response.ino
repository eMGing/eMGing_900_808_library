/***************************************************
  # Proyecto de ejemplo SMS para chequeo del Shield eMGing 900/808

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

Si necesitas setear los datos GPRS como APN, username, password podes hacerlo al final del setup
y descomentar esas lineas para que quede seteado correctamente.
( ATENCION!, estos datos te los puede suministrar tu compañia de celular )

*/

#include "eMGing_900_808.h"

#define EMGING_RX 8
#define EMGING_TX 7
#define EMGING_RST 4

// Buffer para las respuestas a los comandos AT
char replybuffer[255];

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

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

void setup() {
  while (!Serial);

  Serial.begin(9600);
  Serial.println(F("Testeo de SMS Caller ID EMGING"));
  Serial.println(F("Inicialiando...."));

  eMGingSerial->begin(9600);
  if (! eMGing.begin(*eMGingSerial)) {
    Serial.println(F("No se encontro el shield"));
    while (1);
  }
  Serial.println(F("El Shield eMGing esta OK"));

  // Muestra el numero de IMEI.
  char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = eMGing.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }
  // Opcionalmente podes configurar los datos GPRS de tu compañia
  // como pueden ser APN, APN, nombre de usuario y password.
  // Debes configurar esto para poder acceder a la red GPRS
  // que te permite conectarte a internet y enviar datos.
  // Contacta a tu proveedor de red celular, para que te pase
  // la configuracion correcta de APN, usuario, contraseña.
  // Quizá en algunos el usuario y contraseña no es necesario pero
  // el APN.
  // Para configurar tenes que descomentar la siguientes linea y 
  // reemplazar por los valores correctos:
  // PARA MOVISTAR ARGENTINA:
  // apn: internet.gprs.unifon.com
  // usuario: internet
  // password: gprs
  //eMGing.setGPRSNetworkSettings(F("cambiar por tu APN"), F("cambiar por tu usuario"), F("cambiar por tu password"));

  // Opcionalmente tambien podes configurar el HTTP para que redireccione
  // por SSL.
  // Por default no hace redirecciones por SSL, pero si descomentan la siguiente
  // linea, se puede activar.
  //eMGing.setHTTPSRedirect(true); 
  Serial.println("Shield eMGing listo");
}

  
char eMGingInBuffer[64];          //Para notificaciones del Shield

void loop() {
  
  char* bufPtr = eMGingInBuffer;    //Puntero al buffer de notificaciones
  
  if (eMGing.available())      //chequea informacion proveniente del shield
  {
    int slot = 0;            //este es el numero de mensaje SMS 
    int charCount = 0;
    //Leer notificationes en el eMGingInBuffer
    do  {
      *bufPtr = eMGing.read();
      Serial.write(*bufPtr);
      delay(1);
    } while ((*bufPtr++ != '\n') && (eMGing.available()) && (++charCount < (sizeof(eMGingInBuffer)-1)));
    
    //Add a terminal NULL to the notification string
    *bufPtr = 0;
    
    //Scan the notification string for an SMS received notification.
    //  If it's an SMS message, we'll get the slot number in 'slot'
    if (1 == sscanf(eMGingInBuffer, "+CMTI: \"SM\",%d", &slot)) {
      Serial.print("slot: "); Serial.println(slot);
      
      char callerIDbuffer[32];  //almacenaremos aqui el numero de celular de quien envia el SMS
      
      // Buscaremos los datos y telefono de quien envio el SMS.
      if (! eMGing.getSMSSender(slot, callerIDbuffer, 31)) {
        Serial.println("No se encontro ningun mensaje SMS!");
      }
      Serial.print(F("De: ")); Serial.println(callerIDbuffer);
      
      //Send back an automatic response
      Serial.println("Enviando respuesta...");
      if (!eMGing.sendSMS(callerIDbuffer, "Hola, me llego tu mensaje!")) {
        Serial.println(F("Fallo"));
      } else {
        Serial.println(F("Enviado!"));
      }
      
      // Borrar el mensaje luego de haberlo recibido
      //   sino, puede llenarse el espacio para mensajes
      //   y no podremos recibir mas mensajes.
      if (eMGing.deleteSMS(slot)) {
        Serial.println(F("OK!"));
      } else {
        Serial.println(F("No se pudieron borrar los mensajes"));
      }
    }
  }
}
