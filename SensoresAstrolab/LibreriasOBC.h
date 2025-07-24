/*---------------------------------------------------LIBRERIAS---------------------------------------------------------------------------*/
#include <Wire.h>

/*Libreria
Instalar libreria Adafruit INA219 by adafruit
*/
#include <Adafruit_INA219.h>

/*Libreria
Instalar Adafruit BME280 Library by Adafruit
*/
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/*Libreria
Instalar Adafruit MAX31865 Library by Adfruit
*/
#include <Adafruit_MAX31865.h>
/*
  10->CS
  11->SDI
  12->SDO
  13->CLK
  VCC->5V
*/

/*Librerias para GPS6mv2
Instalar TinyGPSPlusPlus by Ress
*/
#include <TinyGPSPlus.h>

/*Libreria para ACS712
Instalar ACS712 by Rob Tillaart
*/
#include "ACS712.h"
/*
Instalar SD by Arduino
*/
#include <SD.h>
#include <SPI.h>
//Encabezado de Funciones
#include "EncabezadoDeFunciones.h"
//Funciones
#include "Funciones.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
