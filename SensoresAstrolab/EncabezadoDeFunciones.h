/*_____________________________________________________________________________________________________________________
--------------------------------------------------------------------VARIABLES------------------------------------------------------------
____________________________________________________________________________________________________________________*/

//Para MAX31865
#define CS_PIN 10
#define DI_PIN 11
#define DO_PIN 12
#define CLK_PIN 13

#define RREF 430.0 // Reference resistor value for PT100
#define RNOMINAL 100.0 // Nominal resistance at 0 degrees Celsius for PT100

//Para BME280
#define SEALEVELPRESSURE_HPA ( 1013.25)

//Para termistores
#define Resolucion 3.3/1023
#define R0 114000
#define T0 300.15
#define B 3950

//Para GPS
static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
//Para SDcard
const int chipSelect = BUILTIN_SDCARD;
//Para  termistores
int PinesTermistor[] = {15 , 16, 17, 20, 21, 22};
float Termistores[6] = {0};
/*Definimos objetos*/
//Objeto de INA219
Adafruit_INA219 SensorCorriente_Ina219;

//Objeto de INA219
Adafruit_BME280 Sensor01Bme280;

//Objeto de MAX31865
Adafruit_MAX31865 SensorMAX31865 = Adafruit_MAX31865(CS_PIN, DI_PIN, DO_PIN, CLK_PIN);

//Objeto de GPS
TinyGPSPlus gps;
//Objeto ACS712
ACS712  SensorACS712(23, 5.0, 1023, 100);
//  ACS712  ACS(25, 3.3, 4095, 185);
//Objeto de archivo en SDcard
File ArchivoSDcard;
//  ESP 32 example (might requires resistors to step down the logic voltage)


/*Variables para termistor */
#define Resolucion 3.3/1023
#define R0 114000
#define T0 300.15
#define B 3950
float resTer1 = 0;
float Volt1 = 0;
float T_1 = 0;

//Para los datos
String DatosIna219, DatosMax31865, DatosGps, DatosTermistores, DatosACS712, DatosBme280, DatosCadenaTeensySlave, DatosCadenaComandos;



//Definimos los 4 pines PWM al que están conectados los 4 LEDs
//Los pines escogidos son los del Arduino 01 (De acuerdo al microcontrolador que tengas cambias estos parámetros)
const int ledPin_1 = 24; // Pin PWM al que está conectado el LED_01
const int ledPin_2 = 25; // Pin PWM al que está conectado el LED_02
const int ledPin_3 = 33; // Pin PWM al que está conectado el LED_03
const int ledPin_4 = 36; // Pin PWM al que está conectado el LED_04

//Definimos los 4 pines de entrada analógica al que están conectados las 4 CSSC.
const int CSSC_Pin_1 = 41; // Pin analógico al que está conectado la CSSC_01
const int CSSC_Pin_2 = 40; // Pin analógico al que está conectado la CSSC_02
const int CSSC_Pin_3 = 39; // Pin analógico al que está conectado la CSSC_03
const int CSSC_Pin_4 = 38; // Pin analógico al que está conectado la CSSC_04
const int fotodiodo_Pin_5 = 37; // Pin analógico al que está conectado la CSSC_04

/*_____________________________________________________________________________________________________________________
-----------------------------------------PROTOTIPOS DE FUNCIONES-------------------------------------------------------------------------
_______________________________________________________________________________________________________________________*/

/*Prototipos de INA219

*Verificación de conexion; se usa wire2
*Calibra datos para 5V
*/
void FunObtenerDatosINA219( Adafruit_INA219 &  ina219);
String FunObtenerStringDatosINA219( Adafruit_INA219 &  ina219);

//Prototipos de BME280-------------------------------------------------------------------
void FunIniciarBME280(Adafruit_BME280 & bme);
String FunObtenerStringDatosBME280(Adafruit_BME280 & bme);

//Prototipos de MAX3185----------------------------------------------------------------------
void FunIniciarMAX31865(Adafruit_MAX31865 & thermo);
String FunObtenerStringDatosMAX31865(Adafruit_MAX31865 & thermo);

//Prototipos de GPS---------------------------------------------------------------------------
String FunObtenerStringDatosGPS6mv2();

//Prototipos para termistores--------------------------------------------------------------------
/*
  Entrada: Pin de activación 15 -20("PinesTermistor")
  Salida: String del valor de temperatura de Termistor
*/
String FunObtenerStringDatosTermistor(int  Termocupla[]);

//Prototipos comunicacion entre teensy--------------------------------------------------------
/*
  Entrada: Agregar con qué serial se hará la comunicación("Serial8")
  Salida: String de la cadena del teensy esclavo
*/
String FunObtenerComandosTeensyPlataforma( String & receivedMessage);
String FunObtenerStringDatosComunicacionTeensyTeensy(HardwareSerial &Serialx);

//Prototipos para corriente------------------------------------------------------------------
/*
  Entrada: Objeto del ACS712("SensorACS712")
  Inicia la conexión
*/
void FunIniciarACS712(ACS712 & ACS);
/*
  Entrada: Objeto del ACS712("SensorACS712")
  Salida: String de la medida de corriente
*/
String FunObtenerStringDatosACS712( ACS712 & ACS);

//FUNCIONE CREADAS PARA FACILITAR OPERACIONES
void FuncionCorregirCaracter(String & cadena, int tam);
// Para  guardar en SDcard------------------------------------------------------------------
/*
  Entrada: Objeto tipo File ( Archivo)
  Lee el contenido de el archivo "tst.txt"
*/
void FuncionLeerEnSDcard(File & myFile);
/*
  Entrada: Objeto tipo File ( Archivo) , String que quieres guardar (Data)
  Guarda el string en la tarjeta SD
*/
void FuncionEscribirEnSDcard(File & myFile, String messenger);

//Para HeatingPad/------------------------------------------------------------------
/*
Entrada: Valor de voltage al que quieres que sea la señal PWN 0 -3.3V
*/
void FunPWMParaHeatingPad(float valorVoltage);
void FunControlHeatingPad(float temp4 , float temp5, int fetPin);
void FunControlHeatingPad(float temp4, int fetPin);
//Para LecturaDecomandos-------------------------------------------------------------------
void FunProcesarComandos(String comandos);
void FunEjecutarComandos(String comando);
int contarComandos(String data, char separador);
void splitString(String data, char separador, String* outputArray, int size);



void FunIniciarCeldas();
String FunLeerDatosCeldas(String &);