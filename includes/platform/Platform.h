/***************************************************
  # Platform.h eMGing para Shield eMGing 900/808

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


Codeado por Gustavo Marek para eMGing Electrónica Argentina.  
Licencia BSD, Todo el texto aquí descripto debe estar incluído en cualquier tipo de distribución de esta u otra librería

Ubicar la carpeta de la librería ( que debe incluír los archivos: eMGing_900_808.cpp e eMGing_900_808.h ) y colocarla en la
siguiente ubicación: " documentos/Arduino/librerías/ " si no existe crear la subcarpeta y luego de copiar alli la librería
reiniciar el IDE Arduino para que tome la nueva librería.


Entren a la página oficial donde encontrarán los tutoriales y los esquemas de conexión para ambos módulos,
o pueden ver los links siguientes para entrar directamente:

  * http://www.emging.com.ar/2016/07/06/shield-gsm-gprs-sim900/
  * http://www.emging.com.ar/2016/07/20/shield-gsm-gprs-gps-sim808/
 ****************************************************/



#ifndef EMGING_LIBRARY_SRC_INCLUDES_PLATFORM_FONAPLATFORM_H_
#define EMGING_LIBRARY_SRC_INCLUDES_PLATFORM_FONAPLATFORM_H_

#include "../Config.h"

// only "standard" config supported in this release -- namely AVR-based arduino type affairs
#include "PlatStd.h"



#ifndef DEBUG_PRINT
// debug is disabled

#define DEBUG_PRINT(...)
#define DEBUG_PRINTLN(...)

#endif


#ifndef prog_char_strcmp
#define prog_char_strcmp(a, b)					strcmp((a), (b))
#endif

#ifndef prog_char_strstr
#define prog_char_strstr(a, b)					strstr((a), (b))
#endif

#ifndef prog_char_strlen
#define prog_char_strlen(a)						strlen((a))
#endif


#ifndef prog_char_strcpy
#define prog_char_strcpy(to, fromprogmem)		strcpy((to), (fromprogmem))
#endif


#endif /* EMGING_LIBRARY_SRC_INCLUDES_PLATFORM_PLATFORM_H_ */
