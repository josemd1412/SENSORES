#include "LibreriasOBC.h"
/*................................................................VOID_SETUP.......................................................*/
uint8_t DatosByteComparate[2]= {0xE5,0x12};
void setup(){ 
  //Para comunicación entre teensy-plataforma pin 0 y 1 "RS232"
  Serial1.begin(4800);
  
  //ParaGPS
  Serial7.begin(9600);
  
  //Para BME280
  Wire.begin();
  //Para HeatingPad 1
  pinMode(2, OUTPUT);
  analogWriteResolution(12);
  //Encender HeatingPAF
  FunPWMParaHeatingPad(3.3);
  
  //Funciones activar 
  FunIniciarACS712(SensorACS712);
  FunIniciarBME280(Sensor01Bme280);
  FunIniciarINA219(SensorCorriente_Ina219);
  FunIniciarMAX31865(SensorMAX31865);
  FunIniciarCeldas();
  
} 
/*................................................................VOID_LOOP.......................................................*/

void loop() {
  //Para reiniciar la cadena                                                                                                                                                                                                        
  String Data = "";
  static uint8_t DatosBytesCommand[2] = {0, 0};

  static size_t bytesRead = 0;
  DatosCadenaComandos = "";
  
  //Para comunicacion
  unsigned long start = millis();


  /*
  ______________________________________________________________________________________________________________________
  Para lectura de comandos y lectura GPS, pwm
  _______________________________________________________________________________________________________________________
  */
  FunIniciarCeldas();//Para que haya lecturas sin fallos
  /*Buble de 255 muestras */
  for (int pwmValue = 0; pwmValue <= 255; pwmValue += 5) {
      // Ajustamos la intensidad de los LEDs
    analogWrite(ledPin_1, pwmValue);
    analogWrite(ledPin_2, pwmValue);
    analogWrite(ledPin_3, pwmValue);
    analogWrite(ledPin_4, pwmValue)
  
  while (millis() - start < 1000) {
      // Lectura de comandos desde la plataforma
      if (Serial1.available() > 0 && bytesRead <2) {
        DatosBytesCommand[bytesRead] = Serial1.read();
        bytesRead++;
        if (bytesRead == 2) {
          DatosByteComparate[0]=DatosBytesCommand[0];
          DatosByteComparate[1]=DatosBytesCommand[1];
          bytesRead = 0; // Resetear el contador de bytes leídos
        }
      }

      // Lectura de datos desde el GPS
      if (Serial7.available() > 0) {
        gps.encode(Serial7.read());
      }
    }
    //Leemos los datos de las CSSCs
    int CSSC_Value_1 = analogRead(CSSC_Pin_1);
    int CSSC_Value_2 = analogRead(CSSC_Pin_2);
    int CSSC_Value_3 = analogRead(CSSC_Pin_3);
    int CSSC_Value_4 = analogRead(CSSC_Pin_4);

    //Hacemos lo mismo para los datos de las celdas
    float V_CSSC_1 = CSSC_Value_1 * (1.0 / 1023.0);
    float V_CSSC_2 = CSSC_Value_2 * (1.0 / 1023.0);
    float V_CSSC_3 = CSSC_Value_3 * (1.0 / 1023.0);
    float V_CSSC_4 = CSSC_Value_4 * (1.0 / 1023.0);

    // Imprimimos el valor PWM y los valores leídos en la consola
    Data += String(pwmValue)+","+
    String(V_CSSC_1)+","+
    String(V_CSSC_2)+","+
    String(V_CSSC_3)+","+
    String(V_CSSC_4)+",";
  }

  int Value_5 = analogRead(fotodiodo_Pin_5);
  //Hacemos lo mismo para los datos de las celdas
  float V_fotodiodo = Value_5 * (1.0 / 1023.0);
  Data += String(V_fotodiodo)+",";
  

  FunEjecutarComandos(DatosByteComparate);
  if (DatosByteComparate[0] ==0xE5  && DatosByteComparate[1] == 0x12){
    /*
    ____________________________________________________________________________________________________
    Obtencion, lectura y guardado de datos una Cadena String
    _________________________________________________________________________________________________________
    */
    
    //Se acumula la cadena de datos segun especidficaciones Nasa
    
    Data += FunObtenerStringDatosMAX31865(SensorMAX31865);
    Data += FunObtenerStringDatosINA219(SensorCorriente_Ina219);
    Data += FunObtenerStringDatosBME280(Sensor01Bme280);
    Data += FunObtenerStringDatosACS712(SensorACS712);
    Data += FunObtenerStringDatosGPS6mv2();
    
    //DatosTermistor
    for(int k=0;k<6;k++) pinMode(PinesTermistor[k], INPUT); //Configurar lecturas para no errores
    Data += FunObtenerStringDatosTermistor(PinesTermistor);
    
    Data += "\n";
    //Se envia datos a la plataforma
    //Serial.print(Data);

    /*____________________________________________________________________________________________________
    Se guarda datos en SDcard
    _________________________________________________________________________________________________________
    */
    FuncionEscribirEnSDcard(ArchivoSDcard, Data);
    
    FunControlHeatingPad(Termistores[3], Termistores[4], 2);
  }
}


