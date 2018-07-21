/***************************************************
  # Librería eMGing para Shield eMGing 900/808

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
#ifndef EMGING_900_808_H
#define EMGING_900_808_H

#include "includes/Config.h"
#include "includes/ExtIncludes.h"
#include "includes/platform/Platform.h"

#define EMGING_900 1

#define EMGING_808_V1 2
#define EMGING_808_V2 3

#define EMGING_3G_A 4
#define EMGING_3G_E 5

// Uncomment to changed the preferred SMS storage
//#define PREF_SMS_STORAGE "SM"

#define HEADSETAUDIO 0
#define EXTAUDIO 1

#define STTONE_DIALTONE 1
#define STTONE_BUSY 2
#define STTONE_CONGESTION 3
#define STTONE_PATHACK 4
#define STTONE_DROPPED 5
#define STTONE_ERROR 6
#define STTONE_CALLWAIT 7
#define STTONE_RINGING 8
#define STTONE_BEEP 16
#define STTONE_POSTONE 17
#define STTONE_ERRTONE 18
#define STTONE_INDIANDIALTONE 19
#define STTONE_USADIALTONE 20

#define DEFAULT_TIMEOUT_MS 500

#define HTTP_GET   0
#define HTTP_POST  1
#define HTTP_HEAD  2

#define CALL_READY 0
#define CALL_FAILED 1
#define CALL_UNKNOWN 2
#define CALL_RINGING 3
#define CALL_INPROGRESS 4

class eMGing_900_808 : public Stream {
 public:
  eMGing_900_808(int8_t r);
  boolean begin(Stream &port);
  uint8_t type();

  // Stream
  int available(void);
  size_t write(uint8_t x);
  int read(void);
  int peek(void);
  void flush();

  // FONA 3G requirements
  boolean setBaudrate(uint16_t baud);
  
  // RTC
  boolean enableRTC(uint8_t i);
  boolean readRTC(uint8_t *year, uint8_t *month, uint8_t *date, uint8_t *hr, uint8_t *min, uint8_t *sec);

  // Battery and ADC
  boolean getADCVoltage(uint16_t *v);
  boolean getBattPercent(uint16_t *p);
  boolean getBattVoltage(uint16_t *v);

  // SIM query
  uint8_t unlockSIM(char *pin);
  uint8_t getSIMCCID(char *ccid);
  uint8_t getNetworkStatus(void);
  uint8_t getRSSI(void);

  // IMEI
  uint8_t getIMEI(char *imei);

  // set Audio output
  boolean setAudio(uint8_t a);
  boolean setVolume(uint8_t i);
  uint8_t getVolume(void);
  boolean playToolkitTone(uint8_t t, uint16_t len);
  boolean setMicVolume(uint8_t a, uint8_t level);
  boolean playDTMF(char tone);

  // FM radio functions.
  boolean tuneFMradio(uint16_t station);
  boolean FMradio(boolean onoff, uint8_t a = HEADSETAUDIO);
  boolean setFMVolume(uint8_t i);
  int8_t getFMVolume();
  int8_t getFMSignalLevel(uint16_t station);

  // SMS handling
  boolean setSMSInterrupt(uint8_t i);
  uint8_t getSMSInterrupt(void);
  int8_t getNumSMS(void);
  boolean readSMS(uint8_t i, char *smsbuff, uint16_t max, uint16_t *readsize);
  boolean sendSMS(char *smsaddr, char *smsmsg);
  boolean deleteSMS(uint8_t i);
  boolean getSMSSender(uint8_t i, char *sender, int senderlen);
  boolean getSMSdate(uint8_t i, char *date, int datelen);
  boolean getSMStime(uint8_t i, char *time, int timelen);
  boolean sendUSSD(char *ussdmsg, char *ussdbuff, uint16_t maxlen, uint16_t *readlen);

  // Time
  boolean enableNetworkTimeSync(boolean onoff);
  boolean eMGing_900_808::enableNTPTimeSync(boolean onoff, FlashStringPtr ntpserver=0);
  boolean getTime(char *buff, uint16_t maxlen);

  // GPRS handling
  boolean enableGPRS(boolean onoff);
  uint8_t GPRSstate(void);
  boolean getGSMLoc(uint16_t *replycode, char *buff, uint16_t maxlen);
  boolean getGSMLoc(float *lat, float *lon);
  void setGPRSNetworkSettings(FlashStringPtr apn, FlashStringPtr username=0, FlashStringPtr password=0);
  

  // GPS handling
  boolean enableGPS(boolean onoff);
  int8_t GPSstatus(void);
  uint8_t getGPS(uint8_t arg, char *buffer, uint8_t maxbuff);
  boolean getGPS(float *lat, float *lon, float *speed_kph=0, float *heading=0, float *altitude=0);
  boolean enableGPSNMEA(uint8_t nmea);

  // TCP raw connections
  boolean TCPconnect(char *server, uint16_t port);
  boolean TCPclose(void);
  boolean TCPconnected(void);
  boolean TCPsend(char *packet, uint8_t len);
  uint16_t TCPavailable(void);
  uint16_t TCPread(uint8_t *buff, uint8_t len);

  // HTTP low level interface (maps directly to SIM800 commands).
  boolean HTTP_init();
  boolean HTTP_term();
  void HTTP_para_start(FlashStringPtr parameter, boolean quoted = true);
  boolean HTTP_para_end(boolean quoted = true);
  boolean HTTP_para(FlashStringPtr parameter, const char *value);
  boolean HTTP_para(FlashStringPtr parameter, FlashStringPtr value);
  boolean HTTP_para(FlashStringPtr parameter, int32_t value);
  boolean HTTP_data(uint32_t size, uint32_t maxTime=10000);
  boolean HTTP_action(uint8_t method, uint16_t *status, uint16_t *datalen, int32_t timeout = 10000);
  boolean HTTP_readall(uint16_t *datalen);
  boolean HTTP_ssl(boolean onoff);

  // HTTP high level interface (easier to use, less flexible).
  boolean HTTP_GET_start(char *url, uint16_t *status, uint16_t *datalen);
  void HTTP_GET_end(void);
  boolean HTTP_POST_start(char *url, FlashStringPtr contenttype, const uint8_t *postdata, uint16_t postdatalen,  uint16_t *status, uint16_t *datalen);
  void HTTP_POST_end(void);
  void setUserAgent(FlashStringPtr useragent);

  // HTTPS
  void setHTTPSRedirect(boolean onoff);

  // PWM (buzzer)
  boolean setPWM(uint16_t period, uint8_t duty = 50);

  // Phone calls
  boolean callPhone(char *phonenum);
  uint8_t getCallStatus(void);
  boolean hangUp(void);
  boolean pickUp(void);
  boolean callerIdNotification(boolean enable, uint8_t interrupt = 0);
  boolean incomingCallNumber(char* phonenum);

  // Helper functions to verify responses.
  boolean expectReply(FlashStringPtr reply, uint16_t timeout = 10000);
  boolean sendCheckReply(char *send, char *reply, uint16_t timeout = DEFAULT_TIMEOUT_MS);
  boolean sendCheckReply(FlashStringPtr send, FlashStringPtr reply, uint16_t timeout = DEFAULT_TIMEOUT_MS);
  boolean sendCheckReply(char* send, FlashStringPtr reply, uint16_t timeout = DEFAULT_TIMEOUT_MS);


 protected:
  int8_t _rstpin;
  uint8_t _type;

  char replybuffer[255];
  FlashStringPtr apn;
  FlashStringPtr apnusername;
  FlashStringPtr apnpassword;
  boolean httpsredirect;
  FlashStringPtr useragent;
  FlashStringPtr ok_reply;

  // HTTP helpers
  boolean HTTP_setup(char *url);

  void flushInput();
  uint16_t readRaw(uint16_t b);
  uint8_t readline(uint16_t timeout = DEFAULT_TIMEOUT_MS, boolean multiline = false);
  uint8_t getReply(char *send, uint16_t timeout = DEFAULT_TIMEOUT_MS);
  uint8_t getReply(FlashStringPtr send, uint16_t timeout = DEFAULT_TIMEOUT_MS);
  uint8_t getReply(FlashStringPtr prefix, char *suffix, uint16_t timeout = DEFAULT_TIMEOUT_MS);
  uint8_t getReply(FlashStringPtr prefix, int32_t suffix, uint16_t timeout = DEFAULT_TIMEOUT_MS);
  uint8_t getReply(FlashStringPtr prefix, int32_t suffix1, int32_t suffix2, uint16_t timeout); // Don't set default value or else function call is ambiguous.
  uint8_t getReplyQuoted(FlashStringPtr prefix, FlashStringPtr suffix, uint16_t timeout = DEFAULT_TIMEOUT_MS);

  boolean sendCheckReply(FlashStringPtr prefix, char *suffix, FlashStringPtr reply, uint16_t timeout = DEFAULT_TIMEOUT_MS);
  boolean sendCheckReply(FlashStringPtr prefix, int32_t suffix, FlashStringPtr reply, uint16_t timeout = DEFAULT_TIMEOUT_MS);
  boolean sendCheckReply(FlashStringPtr prefix, int32_t suffix, int32_t suffix2, FlashStringPtr reply, uint16_t timeout = DEFAULT_TIMEOUT_MS);
  boolean sendCheckReplyQuoted(FlashStringPtr prefix, FlashStringPtr suffix, FlashStringPtr reply, uint16_t timeout = DEFAULT_TIMEOUT_MS);


  boolean parseReply(FlashStringPtr toreply,
          uint16_t *v, char divider  = ',', uint8_t index=0);
  boolean parseReply(FlashStringPtr toreply,
          char *v, char divider  = ',', uint8_t index=0);
  boolean parseReplyQuoted(FlashStringPtr toreply,
          char *v, int maxlen, char divider, uint8_t index);

  boolean sendParseReply(FlashStringPtr tosend,
       FlashStringPtr toreply,
       uint16_t *v, char divider = ',', uint8_t index=0);

  static boolean _incomingCall;
  static void onIncomingCall();

  Stream *mySerial;
};

class eMGing_3G : public eMGing_900_808 {

 public:
  eMGing_3G (int8_t r) : eMGing_900_808(r) { _type = EMGING_3G_A; }

    boolean getBattVoltage(uint16_t *v);
    boolean playToolkitTone(uint8_t t, uint16_t len);
    boolean hangUp(void);
    boolean pickUp(void);
    boolean enableGPRS(boolean onoff);
    boolean enableGPS(boolean onoff);

 protected:
    boolean parseReply(FlashStringPtr toreply,
		       float *f, char divider, uint8_t index);

    boolean sendParseReply(FlashStringPtr tosend,
			   FlashStringPtr toreply,
			   float *f, char divider = ',', uint8_t index=0);
};

#endif
