/***************************************************
  # Proyecto de ejemplo para chequeo del Shield eMGing para Shield eMGing 900/808

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

#include "eMGing_900_808.h"

#define EMGING_RX 8    // corresponde al pin TX del Arduino
#define EMGING_TX 7    // corresponde al pin RX del Arduino
#define EMGING_RST 4

// Buffer para las respuestas a los comandos AT
char replybuffer[255];

// Estos Shield usan por defecto software serial. Si quieren usar hardware serial
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

uint8_t type;

void setup() {
  while (!Serial);

  Serial.begin(9600);
  Serial.println(F("test basico para Shields eMGing 900/808"));
  Serial.println(F("Inicializando....(puede tomar unos segundos)"));

  eMGingSerial->begin(9600);
  if (! eMGing.begin(*eMGingSerial)) {
    Serial.println(F("No se encontro el shield"));
    while (1);
  }
  type = eMGing.type();
  Serial.println(F("El Shield eMGing esta OK"));
  Serial.print(F("Found "));
  switch (type) {
    case EMGING_900:
      Serial.println(F("Shield eMGing 900")); break;
    case EMGING_808_V1:
      Serial.println(F("Shield eMGing 808 (v1)")); break;
    case EMGING_808_V2:
      Serial.println(F("Shield eMGing 808 (v2)")); break;
    case EMGING_3G_A:
      Serial.println(F("Shield eMGing 3G (American)")); break;
    case EMGING_3G_E:
      Serial.println(F("Shield eMGing 3G (European)")); break;
    default: 
      Serial.println(F("???")); break;
  }
  
  // Print module IMEI number.
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

  printMenu();
}

void printMenu(void) {
  Serial.println(F("-------------------------------------"));
  Serial.println(F("[?] Print this menu"));
  Serial.println(F("[a] read the ADC 2.8V max (Solo para Shield eMGing 900 & 808)"));
  Serial.println(F("[b] read the Battery V and % charged"));
  Serial.println(F("[C] read the SIM CCID"));
  Serial.println(F("[U] Unlock SIM with PIN code"));
  Serial.println(F("[i] read RSSI"));
  Serial.println(F("[n] get Network status"));
  Serial.println(F("[v] set audio Volume"));
  Serial.println(F("[V] get Volume"));
  Serial.println(F("[H] set Headphone audio (Solo para Shield eMGing 900 & 808)"));
  Serial.println(F("[e] set External audio (Solo para Shield eMGing 900 & 808)"));
  Serial.println(F("[T] play audio Tone"));
  Serial.println(F("[P] PWM/Buzzer out (Solo para 900 & 808)"));

  // Phone
  Serial.println(F("[c] make phone Call"));
  Serial.println(F("[A] get call status"));
  Serial.println(F("[h] Hang up phone"));
  Serial.println(F("[p] Pick up phone"));

  // SMS
  Serial.println(F("[N] Number of SMSs"));
  Serial.println(F("[r] Read SMS #"));
  Serial.println(F("[R] Read All SMS"));
  Serial.println(F("[d] Delete SMS #"));
  Serial.println(F("[s] Send SMS"));
  Serial.println(F("[u] Send USSD"));
  
  // Time
  Serial.println(F("[y] Enable network time sync (Solo para Shield eMGing 808)"));
  Serial.println(F("[Y] Enable NTP time sync (Solo para Shield eMGing 808)"));
  Serial.println(F("[t] Get network time"));

  // GPRS
  Serial.println(F("[G] Enable GPRS"));
  Serial.println(F("[g] Disable GPRS"));
  Serial.println(F("[l] Query GSMLOC (GPRS)"));
  Serial.println(F("[w] Read webpage (GPRS)"));
  Serial.println(F("[W] Post to website (GPRS)"));

  // GPS
  if ((type == EMGING_3G_A) || (type == EMGING_3G_E) || (type == EMGING_808_V1) || (type == EMGING_808_V2)) {
    Serial.println(F("[O] Turn GPS on (Solo para Shield eMGing 808)"));
    Serial.println(F("[o] Turn GPS off (Solo para Shield eMGing 808)"));
    Serial.println(F("[L] Query GPS location (Solo para Shield eMGing 808)"));
    if (type == EMGING_808_V1) {
      Serial.println(F("[x] GPS fix status (Solo para Shield eMGing 808 v1)"));
    }
    Serial.println(F("[E] Raw NMEA out (Solo para Shield eMGing 808)"));
  }
  
  Serial.println(F("[S] create Serial passthru tunnel"));
  Serial.println(F("-------------------------------------"));
  Serial.println(F(""));

}
void loop() {
  Serial.print(F("Shield eMGing > "));
  while (! Serial.available() ) {
    if (eMGing.available()) {
      Serial.write(eMGing.read());
    }
  }

  char command = Serial.read();
  Serial.println(command);


  switch (command) {
    case '?': {
        printMenu();
        break;
      }

    case 'a': {
        // read the ADC
        uint16_t adc;
        if (! eMGing.getADCVoltage(&adc)) {
          Serial.println(F("Failed to read ADC"));
        } else {
          Serial.print(F("ADC = ")); Serial.print(adc); Serial.println(F(" mV"));
        }
        break;
      }

    case 'b': {
        // read the battery voltage and percentage
        uint16_t vbat;
        if (! eMGing.getBattVoltage(&vbat)) {
          Serial.println(F("Failed to read Batt"));
        } else {
          Serial.print(F("VBat = ")); Serial.print(vbat); Serial.println(F(" mV"));
        }


        if (! eMGing.getBattPercent(&vbat)) {
          Serial.println(F("Failed to read Batt"));
        } else {
          Serial.print(F("VPct = ")); Serial.print(vbat); Serial.println(F("%"));
        }

        break;
      }

    case 'U': {
        // Unlock the SIM with a PIN code
        char PIN[5];
        flushSerial();
        Serial.println(F("Enter 4-digit PIN"));
        readline(PIN, 3);
        Serial.println(PIN);
        Serial.print(F("Unlocking SIM card: "));
        if (! eMGing.unlockSIM(PIN)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("OK!"));
        }
        break;
      }

    case 'C': {
        // read the CCID
        eMGing.getSIMCCID(replybuffer);  // make sure replybuffer is at least 21 bytes!
        Serial.print(F("SIM CCID = ")); Serial.println(replybuffer);
        break;
      }

    case 'i': {
        // read the RSSI
        uint8_t n = eMGing.getRSSI();
        int8_t r;

        Serial.print(F("RSSI = ")); Serial.print(n); Serial.print(": ");
        if (n == 0) r = -115;
        if (n == 1) r = -111;
        if (n == 31) r = -52;
        if ((n >= 2) && (n <= 30)) {
          r = map(n, 2, 30, -110, -54);
        }
        Serial.print(r); Serial.println(F(" dBm"));

        break;
      }

    case 'n': {
        // read the network/cellular status
        uint8_t n = eMGing.getNetworkStatus();
        Serial.print(F("Network status "));
        Serial.print(n);
        Serial.print(F(": "));
        if (n == 0) Serial.println(F("Not registered"));
        if (n == 1) Serial.println(F("Registered (home)"));
        if (n == 2) Serial.println(F("Not registered (searching)"));
        if (n == 3) Serial.println(F("Denied"));
        if (n == 4) Serial.println(F("Unknown"));
        if (n == 5) Serial.println(F("Registered roaming"));
        break;
      }

    /*** Audio ***/
    case 'v': {
        // set volume
        flushSerial();
        if ( (type == EMGING_3G_A) || (type == EMGING_3G_E) ) {
          Serial.print(F("Set Vol [0-8] "));
        } else {
          Serial.print(F("Set Vol % [0-100] "));
        }
        uint8_t vol = readnumber();
        Serial.println();
        if (! eMGing.setVolume(vol)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("OK!"));
        }
        break;
      }

    case 'V': {
        uint8_t v = eMGing.getVolume();
        Serial.print(v);
        if ( (type == EMGING_3G_A) || (type == EMGING_3G_E) ) {
          Serial.println(" / 8");
        } else {
          Serial.println("%");
        }
        break;
      }

    case 'H': {
        // Set Headphone output
        if (! eMGing.setAudio(HEADSETAUDIO)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("OK!"));
        }
        eMGing.setMicVolume(HEADSETAUDIO, 15);
        break;
      }
    case 'e': {
        // Set External output
        if (! eMGing.setAudio(EXTAUDIO)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("OK!"));
        }

        eMGing.setMicVolume(EXTAUDIO, 10);
        break;
      }

    case 'T': {
        // play tone
        flushSerial();
        Serial.print(F("Play tone #"));
        uint8_t kittone = readnumber();
        Serial.println();
        // play for 1 second (1000 ms)
        if (! eMGing.playToolkitTone(kittone, 1000)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("OK!"));
        }
        break;
      }

    /*** FM Radio ***/

    case 'f': {
        // get freq
        flushSerial();
        Serial.print(F("FM Freq (eg 1011 == 101.1 MHz): "));
        uint16_t station = readnumber();
        Serial.println();
        // FM radio ON using headset
        if (eMGing.FMradio(true, HEADSETAUDIO)) {
          Serial.println(F("Opened"));
        }
        if (! eMGing.tuneFMradio(station)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("Tuned"));
        }
        break;
      }
    case 'F': {
        // FM radio off
        if (! eMGing.FMradio(false)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("OK!"));
        }
        break;
      }
    case 'm': {
        // Set FM volume.
        flushSerial();
        Serial.print(F("Set FM Vol [0-6]:"));
        uint8_t vol = readnumber();
        Serial.println();
        if (!eMGing.setFMVolume(vol)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("OK!"));
        }
        break;
      }
    case 'M': {
        // Get FM volume.
        uint8_t fmvol = eMGing.getFMVolume();
        if (fmvol < 0) {
          Serial.println(F("Failed"));
        } else {
          Serial.print(F("FM volume: "));
          Serial.println(fmvol, DEC);
        }
        break;
      }
    case 'q': {
        // Get FM station signal level (in decibels).
        flushSerial();
        Serial.print(F("FM Freq (eg 1011 == 101.1 MHz): "));
        uint16_t station = readnumber();
        Serial.println();
        int8_t level = eMGing.getFMSignalLevel(station);
        if (level < 0) {
          Serial.println(F("Failed! Make sure FM radio is on (tuned to station)."));
        } else {
          Serial.print(F("Signal level (dB): "));
          Serial.println(level, DEC);
        }
        break;
      }

    /*** PWM ***/

    case 'P': {
        // PWM Buzzer output @ 2KHz max
        flushSerial();
        Serial.print(F("PWM Freq, 0 = Off, (1-2000): "));
        uint16_t freq = readnumber();
        Serial.println();
        if (! eMGing.setPWM(freq)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("OK!"));
        }
        break;
      }

    /*** Call ***/
    case 'c': {
        // call a phone!
        char number[30];
        flushSerial();
        Serial.print(F("Call #"));
        readline(number, 30);
        Serial.println();
        Serial.print(F("Calling ")); Serial.println(number);
        if (!eMGing.callPhone(number)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("Sent!"));
        }

        break;
      }
    case 'A': {
        // get call status
        int8_t callstat = eMGing.getCallStatus();
        switch (callstat) {
          case 0: Serial.println(F("Ready")); break;
          case 1: Serial.println(F("Could not get status")); break;
          case 3: Serial.println(F("Ringing (incoming)")); break;
          case 4: Serial.println(F("Ringing/in progress (outgoing)")); break;
          default: Serial.println(F("Unknown")); break;
        }
        break;
      }
      
    case 'h': {
        // hang up!
        if (! eMGing.hangUp()) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("OK!"));
        }
        break;
      }

    case 'p': {
        // pick up!
        if (! eMGing.pickUp()) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("OK!"));
        }
        break;
      }

    /*** SMS ***/

    case 'N': {
        // read the number of SMS's!
        int8_t smsnum = eMGing.getNumSMS();
        if (smsnum < 0) {
          Serial.println(F("Could not read # SMS"));
        } else {
          Serial.print(smsnum);
          Serial.println(F(" SMS's on SIM card!"));
        }
        break;
      }
    case 'r': {
        // read an SMS
        flushSerial();
        Serial.print(F("Read #"));
        uint8_t smsn = readnumber();
        Serial.print(F("\n\rReading SMS #")); Serial.println(smsn);

        // Retrieve SMS sender address/phone number.
        if (! eMGing.getSMSSender(smsn, replybuffer, 250)) {
          Serial.println("Failed!");
          break;
        }
        Serial.print(F("FROM: ")); Serial.println(replybuffer);

        // Retrieve SMS value.
        uint16_t smslen;
        if (! eMGing.readSMS(smsn, replybuffer, 250, &smslen)) { // pass in buffer and max len!
          Serial.println("Failed!");
          break;
        }
        Serial.print(F("***** SMS #")); Serial.print(smsn);
        Serial.print(" ("); Serial.print(smslen); Serial.println(F(") bytes *****"));
        Serial.println(replybuffer);
        Serial.println(F("*****"));

        break;
      }
    case 'R': {
        // read all SMS
        int8_t smsnum = eMGing.getNumSMS();
        uint16_t smslen;
        int8_t smsn;

        if ( (type == EMGING_3G_A) || (type == EMGING_3G_E) ) {
          smsn = 0; // zero indexed
          smsnum--;
        } else {
          smsn = 1;  // 1 indexed
        }

        for ( ; smsn <= smsnum; smsn++) {
          Serial.print(F("\n\rReading SMS #")); Serial.println(smsn);
          if (!eMGing.readSMS(smsn, replybuffer, 250, &smslen)) {  // pass in buffer and max len!
            Serial.println(F("Failed!"));
            break;
          }
          // if the length is zero, its a special case where the index number is higher
          // so increase the max we'll look at!
          if (smslen == 0) {
            Serial.println(F("[empty slot]"));
            smsnum++;
            continue;
          }

          Serial.print(F("***** SMS #")); Serial.print(smsn);
          Serial.print(" ("); Serial.print(smslen); Serial.println(F(") bytes *****"));
          Serial.println(replybuffer);
          Serial.println(F("*****"));
        }
        break;
      }

    case 'd': {
        // delete an SMS
        flushSerial();
        Serial.print(F("Delete #"));
        uint8_t smsn = readnumber();

        Serial.print(F("\n\rDeleting SMS #")); Serial.println(smsn);
        if (eMGing.deleteSMS(smsn)) {
          Serial.println(F("OK!"));
        } else {
          Serial.println(F("Couldn't delete"));
        }
        break;
      }

    case 's': {
        // send an SMS!
        char sendto[21], message[141];
        flushSerial();
        Serial.print(F("Send to #"));
        readline(sendto, 20);
        Serial.println(sendto);
        Serial.print(F("Type out one-line message (140 char): "));
        readline(message, 140);
        Serial.println(message);
        if (!eMGing.sendSMS(sendto, message)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("Sent!"));
        }

        break;
      }

    case 'u': {
      // send a USSD!
      char message[141];
      flushSerial();
      Serial.print(F("Type out one-line message (140 char): "));
      readline(message, 140);
      Serial.println(message);

      uint16_t ussdlen;
      if (!eMGing.sendUSSD(message, replybuffer, 250, &ussdlen)) { // pass in buffer and max len!
        Serial.println(F("Failed"));
      } else {
        Serial.println(F("Sent!"));
        Serial.print(F("***** USSD Reply"));
        Serial.print(" ("); Serial.print(ussdlen); Serial.println(F(") bytes *****"));
        Serial.println(replybuffer);
        Serial.println(F("*****"));
      }
    }

    /*** Time ***/

    case 'y': {
        // enable network time sync
        if (!eMGing.enableNetworkTimeSync(true))
          Serial.println(F("Failed to enable"));
        break;
      }

    case 'Y': {
        // enable NTP time sync
        if (!eMGing.enableNTPTimeSync(true, F("pool.ntp.org")))
          Serial.println(F("Failed to enable"));
        break;
      }

    case 't': {
        // read the time
        char buffer[23];

        eMGing.getTime(buffer, 23);  // make sure replybuffer is at least 23 bytes!
        Serial.print(F("Time = ")); Serial.println(buffer);
        break;
      }


    /*********************************** GPS (SIM808 only) */

    case 'o': {
        // turn GPS off
        if (!eMGing.enableGPS(false))
          Serial.println(F("Failed to turn off"));
        break;
      }
    case 'O': {
        // turn GPS on
        if (!eMGing.enableGPS(true))
          Serial.println(F("Failed to turn on"));
        break;
      }
    case 'x': {
        int8_t stat;
        // check GPS fix
        stat = eMGing.GPSstatus();
        if (stat < 0)
          Serial.println(F("Failed to query"));
        if (stat == 0) Serial.println(F("GPS off"));
        if (stat == 1) Serial.println(F("No fix"));
        if (stat == 2) Serial.println(F("2D fix"));
        if (stat == 3) Serial.println(F("3D fix"));
        break;
      }

    case 'L': {
        // check for GPS location
        char gpsdata[120];
        eMGing.getGPS(0, gpsdata, 120);
        if (type == EMGING_808_V1)
          Serial.println(F("Reply in format: mode,longitude,latitude,altitude,utctime(yyyymmddHHMMSS),ttff,satellites,speed,course"));
        else 
          Serial.println(F("Reply in format: mode,fixstatus,utctime(yyyymmddHHMMSS),latitude,longitude,altitude,speed,course,fixmode,reserved1,HDOP,PDOP,VDOP,reserved2,view_satellites,used_satellites,reserved3,C/N0max,HPA,VPA"));
        Serial.println(gpsdata);

        break;
      }

    case 'E': {
        flushSerial();
        if (type == EMGING_808_V1) {
          Serial.print(F("GPS NMEA output sentences (0 = off, 34 = RMC+GGA, 255 = all)"));
        } else {
          Serial.print(F("On (1) or Off (0)? "));
        }
        uint8_t nmeaout = readnumber();

        // turn on NMEA output
        eMGing.enableGPSNMEA(nmeaout);

        break;
      }

    /*********************************** GPRS */

    case 'g': {
        // turn GPRS off
        if (!eMGing.enableGPRS(false))
          Serial.println(F("Failed to turn off"));
        break;
      }
    case 'G': {
        // turn GPRS on
        if (!eMGing.enableGPRS(true))
          Serial.println(F("Failed to turn on"));
        break;
      }
    case 'l': {
        // check for GSMLOC (requires GPRS)
        uint16_t returncode;

        if (!eMGing.getGSMLoc(&returncode, replybuffer, 250))
          Serial.println(F("Failed!"));
        if (returncode == 0) {
          Serial.println(replybuffer);
        } else {
          Serial.print(F("Fail code #")); Serial.println(returncode);
        }

        break;
      }
    case 'w': {
        // read website URL
        uint16_t statuscode;
        int16_t length;
        char url[80];

        flushSerial();
        Serial.println(F("NOTE: in beta! Use small webpages to read!"));
        Serial.println(F("URL to read (e.g. www.eMGing.com.ar/test):"));
        Serial.print(F("http://")); readline(url, 79);
        Serial.println(url);

        Serial.println(F("****"));
        if (!eMGing.HTTP_GET_start(url, &statuscode, (uint16_t *)&length)) {
          Serial.println("Failed!");
          break;
        }
        while (length > 0) {
          while (eMGing.available()) {
            char c = eMGing.read();

            // Serial.write is too slow, we'll write directly to Serial register!
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
            loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
            UDR0 = c;
#else
            Serial.write(c);
#endif
            length--;
            if (! length) break;
          }
        }
        Serial.println(F("\n****"));
        eMGing.HTTP_GET_end();
        break;
      }

    case 'W': {
        // Post data to website
        uint16_t statuscode;
        int16_t length;
        char url[80];
        char data[80];

        flushSerial();
        Serial.println(F("NOTE: in beta! Use simple websites to post!"));
        Serial.println(F("URL to post (e.g. httpbin.org/post):"));
        Serial.print(F("http://")); readline(url, 79);
        Serial.println(url);
        Serial.println(F("Data to post (e.g. \"foo\" or \"{\"simple\":\"json\"}\"):"));
        readline(data, 79);
        Serial.println(data);

        Serial.println(F("****"));
        if (!eMGing.HTTP_POST_start(url, F("text/plain"), (uint8_t *) data, strlen(data), &statuscode, (uint16_t *)&length)) {
          Serial.println("Failed!");
          break;
        }
        while (length > 0) {
          while (eMGing.available()) {
            char c = eMGing.read();

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
            loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
            UDR0 = c;
#else
            Serial.write(c);
#endif

            length--;
            if (! length) break;
          }
        }
        Serial.println(F("\n****"));
        eMGing.HTTP_POST_end();
        break;
      }
    /*****************************************/

    case 'S': {
        Serial.println(F("Creating SERIAL TUBE"));
        while (1) {
          while (Serial.available()) {
            delay(1);
            eMGing.write(Serial.read());
          }
          if (eMGing.available()) {
            Serial.write(eMGing.read());
          }
        }
        break;
      }

    default: {
        Serial.println(F("Unknown command"));
        printMenu();
        break;
      }
  }
  // flush input
  flushSerial();
  while (eMGing.available()) {
    Serial.write(eMGing.read());
  }

}

void flushSerial() {
  while (Serial.available())
    Serial.read();
}

char readBlocking() {
  while (!Serial.available());
  return Serial.read();
}
uint16_t readnumber() {
  uint16_t x = 0;
  char c;
  while (! isdigit(c = readBlocking())) {
    //Serial.print(c);
  }
  Serial.print(c);
  x = c - '0';
  while (isdigit(c = readBlocking())) {
    Serial.print(c);
    x *= 10;
    x += c - '0';
  }
  return x;
}

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout) {
  uint16_t buffidx = 0;
  boolean timeoutvalid = true;
  if (timeout == 0) timeoutvalid = false;

  while (true) {
    if (buffidx > maxbuff) {
      //Serial.println(F("SPACE"));
      break;
    }

    while (Serial.available()) {
      char c =  Serial.read();

      //Serial.print(c, HEX); Serial.print("#"); Serial.println(c);

      if (c == '\r') continue;
      if (c == 0xA) {
        if (buffidx == 0)   // the first 0x0A is ignored
          continue;

        timeout = 0;         // the second 0x0A is the end of the line
        timeoutvalid = true;
        break;
      }
      buff[buffidx] = c;
      buffidx++;
    }

    if (timeoutvalid && timeout == 0) {
      //Serial.println(F("TIMEOUT"));
      break;
    }
    delay(1);
  }
  buff[buffidx] = 0;  // null term
  return buffidx;
}
