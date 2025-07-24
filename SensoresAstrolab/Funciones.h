/*______________________________________________________________________________________________________________________
-------------------------------------------------Funciones-para-MAX3185------------------------------------------------------------------
__________________________________________________________________________________________________________________________*/

/*@FunIniciarMAX31865
Inicia la lectura para la conexión con tres cables
*/
void FunIniciarMAX31865(Adafruit_MAX31865 & thermo){
  //Arranca para la lectura de datos para tres cables
thermo.begin(MAX31865_3WIRE); // Use MAX31865_2WIRE or MAX31865_4WIRE; 2 cables y 4 cables respectivamente
return ;
}

/*@FunObtenerStringDatosMAX31865
  Crea una cadena(Para la temperatura)
  Se usa método del objeto para obtener temperatura tipo float y guardar
  Se cambia tipo string y se retorna en cadena
*/
String FunObtenerStringDatosMAX31865(Adafruit_MAX31865 & thermo){
String datos;
float temperature = thermo.temperature(RNOMINAL, RREF);
//7 caracteres include (.) y (,)
datos += String(temperature) + ",";// Temperatura °C

return datos;
}

/*_______________________________________________________________________________________________________________________
----------------------------------FuncionesParaIna219--------------------------------------------------------------------
___________________________________________________________________________________________________________________________*/
/*@FunIniciarINA219
  Espera hasta empesar comunicación en el wire2(pines 24 SCL y 25 SDA)
  Calibra para que sea de 5V
*/
void FunIniciarINA219(Adafruit_INA219 & ina219){
  Wire.setSCL(19);
  Wire.setSDA(18);
  while(!ina219.begin(&Wire))  delay(10);
  ina219.setCalibration_32V_2A();
}

/*@FunObtenerStringDatosINA219
  Se crea variables para datos
  Se usa metodos del objeto para leer datos y gaurdar en sus respectivas variables
  Se cambia a varible string y se devuelve en una cadena
*/
String FunObtenerStringDatosINA219( Adafruit_INA219 &  ina219){
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  //float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  //power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  String datos = String(loadvoltage)+ "," ;// Voltage " V\n" , 
  datos += String(current_mA) + "," ;           // Corriente" mA\n";

  //datos += "Power: " + String(power_mW) + ",";               //" mW\n";
  return datos;
}

/*______________________________________________________________________________________________________________________________
-------------------------------------------FuncionesParaBME280-----------------------------------------------------  
_______________________________________________________________________________________________________________________________*/
/*@FunIniciarBME280
  Se espera hasta que haya comunicación 
  */
void FunIniciarBME280(Adafruit_BME280 & bme){
  while(! bme.begin(0x76, &Wire)) delay(10);
  return;
}
String FunObtenerStringDatosBME280(Adafruit_BME280 & bme)
{
  bme.takeForcedMeasurement();

  String datos = String( bme.readTemperature() ) + ",";//Temperatura "°C"
  datos += String( bme.readHumidity() ) + ",";           //Humedad " %"
  datos += String( bme.readPressure()/ 100.0F ) + ",";   //Presión " hPa"
  return datos;
}
/*___________________________________________________________________________________________________________________________
------------------------------FUNCIONES-PARA-GPS-gps6mv2--------------------------------------------
____________________________________________________________________________________________________________________________*/

String FunObtenerStringDatosGPS6mv2() {
  String cadena = "";
  // Imprime la latitud, longitud, altitud, fecha y hora
  cadena += String(gps.location.lat(), 4) + ",";
  cadena += String(gps.location.lng(), 4) + ",";
  cadena += String(gps.altitude.meters()) + ",";
  cadena += String(gps.date.month()) + "/" + String(gps.date.day()) + "/" + "24,";
  cadena += String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()) + ",";
  return cadena;
}

/*____________________________________________________________________________________________________________________________
-------------------------FUNCIONES-PARA-COMUNICACION-ENTRE-TEENSYS----------------------------------
____________________________________________________________________________________________________________________________*/

String FunObtenerStringDatosComunicacionTeensyTeensy(HardwareSerial &Serialx){
    if (Serialx.available() > 0){
    // Lee los datos recibidos
    String received = Serialx.readStringUntil('\n');
    
    // Imprime los datos en el monitor serie
    return received;
    }
    return "";

}
String FunObtenerComandosTeensyPlataforma( String & receivedMessage) {
  int i = 0;
  while (Serial1.available() > 0 && receivedMessage.length() < 5 && i< 5000) {
    char receivedChar = Serial1.read();  // Lee un carácter del puerto Serial1
    receivedMessage += receivedChar;  // Agrega el carácter al mensaje
    i++;
  }
  return receivedMessage;
}

/*____________________________________________________________________________________________________________________________
--------------------------Funcion_para_termistores------------------------------------------------- 
____________________________________________________________________________________________________________________________*/
String FunObtenerStringDatosTermistor(int Termocupla[]){

  String cadena = "";
  for (int i= 0; i<6; i++){
  int ReadADC1 = analogRead(Termocupla[i]);
  float T_1 = (ReadADC1*R0)/(1023-ReadADC1);  //Valor a partir de la lectura analógica con el dato convertido a digital
  //float Volt1 = 3.3*resTer1/(resTer1 + R0);      // voltaje calculado a partir del calculo de la resistencia es igual a ReadADC32*Resolucion
  T_1 =T_1/R0;
  T_1 = log(T_1);
  T_1 /= B;
  T_1 += 1.0 / T0;
  T_1 = 1.0 / T_1;
  T_1 -= 273.15;
  Termistores[i] = T_1;
  cadena += String(T_1) + ",";//Temperatura °C
  }
  return cadena;
}
/*____________________________________________________________________________________________________________________________
--------------------------------FUNCIONES_PARA_GUARDADO_SDcard--------------------------------------
____________________________________________________________________________________________________________________________*/
void FuncionEscribirEnSDcard(File & myFile, String messenger){
  if (!SD.begin(chipSelect)) {
  }
  
  // open the file. 
  myFile = SD.open("test.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    myFile.print(messenger);
    myFile.close();
  }
}
void FuncionLeerEnSDcard(File & myFile){
  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
    	Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  return;
}



/*____________________________________________________________________________________________________________________________
---------------------------------FUNCIONES_PARA_ACS712--------------------------------------------
____________________________________________________________________________________________________________________________*/
void FunIniciarACS712(ACS712 & ACS){
  ACS.autoMidPoint();
}
String FunObtenerStringDatosACS712( ACS712 & ACS){
  String cadena = String(ACS.mA_DC(),1);//Corriente A
  return cadena;
}

/*____________________________________________________________________________________________________________________________
------------------------ParaFuncionesPWm_HetingPad_1--------------------------------------------------------
____________________________________________________________________________________________________________________________*/
void FunPWMParaHeatingPad(float valorVoltage){
  int voltage = (valorVoltage/3.3) * 4095;
  analogWrite(2, voltage);
}
//Para controlarHetingpad1
void FunControlHeatingPad(float temp4 , float temp5, int fetPin){
  float temp = (temp4+temp5)/2;

  if(temp<15){
    analogWrite(fetPin,4095);
  }
  else if(temp>=15 && temp<20){
    analogWrite(fetPin,1360);
  }
  else {
    analogWrite(fetPin,0);
  }
}
/*____________________________________________________________________________________________________________________________
---------------------------FuncionesParaLecturaDecomandos---------------------------
____________________________________________________________________________________________________________________________*/
void FunProcesarComandos(String comandos) {
  if(comandos == "" ) return; 
  int cantidadComandos = contarComandos(comandos, ',');
  String comandosSeparados[cantidadComandos];
  
  splitString(comandos, ',', comandosSeparados, cantidadComandos);

  for (int i = 0; i < cantidadComandos; i++) {
    FunEjecutarComandos(comandosSeparados[i]);
  }
}

int contarComandos(String data, char separador) {
  int contador = 1; // Iniciamos en 1 porque el último elemento no tiene un delimitador después de él.
  for (int i = 0; i < data.length(); i++) {
    if (data.charAt(i) == separador) {
      contador++;
    }
  }
  return contador;
}

void splitString(String data, char separador, String* outputArray, int size) {
  int i = 0;
  int endIndex = data.indexOf(separador);
  int j = 0;

  while (endIndex >= 0) {
    outputArray[j] = data.substring(i, endIndex);
    j++;
    i = endIndex + 1;
    endIndex = data.indexOf(separador, i);
  }
  // Asegurarse de añadir la última parte del string
  outputArray[j] = data.substring(i);
}


void FunEjecutarComandos(const uint8_t* command) {
  if (command[0] == 0x15 && command[1] == 0xFF) {
    /*
    Sistema de apagado, 15xFF, Apagar todo, Corregir cualquier fallo que no pueda resolverse con los demás comandos establecidos.
    En caso de temperatura muy alta u otra avería que lo justifique.
    */
    Serial8.println("15xFF");
  } else if (command[0] == 0xE5 && command[1] == 0x12) {
    //Serial.println("Encendiendo todo el sistema");
    /*
    Sistema de encendido, E5x12, Encender todo, Reiniciar todo y funcionar correctamente.
    */
    Serial8.println("E5x12");
  } else if (command[0] == 0xD5 && command[1] == 0x03) {
    /*
    Sistema de encendido, almohadilla calefactora 1, 
    apagar, D5x03, 
    apagar el calentador del primer compartimento.
    Disminución de la temperatura y del consumo de energía. Almohadilla calefactora 1, apagar. 
    */
    Serial8.println("D5x03");
  } else if (command[0] == 0xC5 && command[1] == 0x04) {
    /*
    Almohadilla calefactora 1, 
    encender, C5x04.
    Encender el calentador del primer compartimento.
    Aumento de la temperatura y del consumo de energía.
    */
    Serial8.println("C5x04");
  } else if (command[0] == 0x55 && command[1] == 0x0B) {
    /*
    Almohadilla calefactora 1 
    encendida Plotter IV encendido, 55x0B, 
    Encienda todas las salidas del plotter IB.
    Habilite la generación de la señal PWM y lea los datos.
    */
    
    FunPWMParaHeatingPad(0.0);
  } else if (command[0] == 0x45 && command[1] == 0x0C) {
    /*
    Salida PWM iniciada Plotter IV 
    apagado, 45x0C,
    Apague todas las salidas del plotter IB. Deshabilite la señal PWM.
    */
    Serial.println("45x0C");
    FunPWMParaHeatingPad(3.3);
  } else if (command[0] == 0xC5 && command[1] == 0x04) {
    /*
    T= +5 min, C5x04, 
    Encienda el calentador del primer compartimento. 
    Aumento de la temperatura del compartimento y del consumo de energía.
    */
    Serial8.println("C5x04");
  } else {
    Serial.println(".");
  }
}


/*____________________________________________________________________________________________________________________________
---------------------------FuncionesParaLecturaDecomandos---------------------------
____________________________________________________________________________________________________________________________*/

void FunIniciarCeldas(){
  pinMode(ledPin_1, OUTPUT);
  pinMode(ledPin_2, OUTPUT);
  pinMode(ledPin_3, OUTPUT);
  pinMode(ledPin_4, OUTPUT);

  pinMode(CSSC_Pin_1, INPUT);
  pinMode(CSSC_Pin_2, INPUT);
  pinMode(CSSC_Pin_3, INPUT);
  pinMode(CSSC_Pin_4, INPUT);
  pinMode(fotodiodo_Pin_5, INPUT);
}

String FunLeerDatosCeldas(String & data){
  
  for (int pwmValue = 0; pwmValue <= 255; pwmValue += 5) {
    // Ajustamos la intensidad de los LEDs
    analogWrite(ledPin_1, pwmValue);
    analogWrite(ledPin_2, pwmValue);
    analogWrite(ledPin_3, pwmValue);
    analogWrite(ledPin_4, pwmValue);

    // Esperamos 01 segundo para estabilizar el cambio en la resistencia de la fotoresistencia.
    delay(1000);

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
    data += String(pwmValue)+","+
    String(V_CSSC_1)+","+
    String(V_CSSC_2)+","+
    String(V_CSSC_3)+","+
    String(V_CSSC_4)+",";
  }

  int Value_5 = analogRead(fotodiodo_Pin_5);
  //Hacemos lo mismo para los datos de las celdas
  float V_fotodiodo = Value_5 * (1.0 / 1023.0);
  data += String(V_fotodiodo)+",";

  return data;
}

/*____________________________________________________________________________________________________________________________
//FUNCIONES CUALQUIERA
____________________________________________________________________________________________________________________________*/
void FuncionCorregirCaracter(String & cadena, int tam){
  int i = tam - cadena.length();
  int j = 0;
  while (j < i)
  {
    cadena += " ";
    j++;
  }
}