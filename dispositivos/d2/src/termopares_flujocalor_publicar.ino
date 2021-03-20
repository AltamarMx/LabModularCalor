#include "PlayingWithFusion_MAX31856.h"
//LibrerÃa del shield termopares
#include "PlayingWithFusion_MAX31856_STRUCT.h" //LibrerÃa del shield
//termopares
#include "SPI.h"
//LibrerÃa SPI
#include <WiFiEsp.h>
//LibrerÃa wifi ESP
#include <WiFiEspClient.h>
//LibrerÃa cliente ESP

#include <WiFiEspUdp.h> //LibrerÃa wifi ESP

#include "SoftwareSerial.h"//LibrerÃa para usar un puerto serial adicional
#include <PubSubClient.h> //LibrerÃa PubSubClient

#define WIFI_AP "INTERNET"//Definir el nombre de la red wifi a conectar
#define WIFI_PASSWORD "xy54321xy"//Definir contraseÃ±a de la red wifi a conectar
#define TOKEN "mKxObNetiDJvSySIlBdB"//Token de acceso de dispositivo de Thinsboard XulHZmjatV0UCx4fuQef
char thingsboardServer[] = "iot.ier.unam.mx";//Ip del servidor de Thingsboard
WiFiEspClient espClient;//Definir el nombre del cliente ESP
PubSubClient client(espClient);//Definir el nombre del subcliente
SoftwareSerial soft(2, 5); // RX, TX,Puerto serial por el que se comunica el ESP-01
int status = WL_IDLE_STATUS;
unsigned long lastSend;

double tmp0;
double tmp1;
double tmp2;
double tmp3;
//Configuraciones para el shield de termopares
uint8_t TC0_CS = 10;
uint8_t TC1_CS = 9;
uint8_t TC2_CS = 8;
uint8_t TC3_CS = 7;
uint8_t TC0_FAULT = 2;// No es usado pero es una configuraciÃ3n necesaria
uint8_t TC0_DRDY = 2;// No es usado pero es una configuraciÃ3n necesaria
const int sensorPin = A0;//Seleccionar la entrada para el sensor de flujo de calor
int sensorValue;//Variable que almacena el valor raw (0 a 1023)
float value;//Variable que almacena el voltaje (0.0 a 5.0)
float microvolts;//Variable para almacenar el voltaje en microvolts
float flujo;//Variable para representar el flujo de calor
//Configuraciones para el shield de termopares
PWF_MAX31856  thermocouple0(TC0_CS, TC0_FAULT, TC0_DRDY);
PWF_MAX31856  thermocouple1(TC1_CS, TC0_FAULT, TC0_DRDY);
PWF_MAX31856  thermocouple2(TC2_CS, TC0_FAULT, TC0_DRDY);
PWF_MAX31856  thermocouple3(TC3_CS, TC0_FAULT, TC0_DRDY);

void setup()//FunciÃ3n de configuraciones iniciales
{
delay(1000);//Retardo 1 segundo
Serial.begin(9600);//Iniciar la comunicaciÃ3n serial a 9600 baudios
InitWiFi();//Ejecutar función InitWiFi
client.setServer( thingsboardServer, 1883 );//Definir tipo de servidor y puerto
lastSend = 0;
Serial.println("Playing With Fusion: MAX31856, SEN-30007/8");//Imprimir mensaje
SPI.begin();//Iniciar comunicaciÃ3n SPI
SPI.setClockDivider(SPI_CLOCK_DIV16);//ConfiguraciÃ3n del SPI
SPI.setDataMode(SPI_MODE3);//ConfiguraciÃ3n del SPI
//Configuraciones para el shield de termopares
  thermocouple0.MAX31856_config(K_TYPE, CUTOFF_60HZ, AVG_SEL_4SAMP);
  thermocouple1.MAX31856_config(K_TYPE, CUTOFF_60HZ, AVG_SEL_4SAMP);
  thermocouple2.MAX31856_config(K_TYPE, CUTOFF_60HZ, AVG_SEL_4SAMP);
  thermocouple3.MAX31856_config(K_TYPE, CUTOFF_60HZ, AVG_SEL_4SAMP);
}
void loop()//Ciclo principal infinito
{
status = WiFi.status();
if ( status != WL_CONNECTED) {//Evaluar conexiÃ3n
while ( status != WL_CONNECTED) {//Intentar conectar
Serial.print("Attempting to connect to WPA SSID: ");//Imprimir mensaje
Serial.println(WIFI_AP);//IMprimir nombre de la red
status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);//Conectar a la red
delay(500);//Retardo
}
Serial.println("Connected to AP");//Imprimir mensaje
}
if ( !client.connected() ) {//Si no se logra conectar entonces reintentar
reconnect();//Ejecutar funciÃ3n para reconectar
}
if ( millis() - lastSend > 1000 ) {
LeerFlujoDeCalor();//Ejecutar la funciÃ3n que obtiene el flujo de calor
leerTermopar();//Ejeutar la funciÃ3n que adquiere los datos de los termopares
EnviarDatos();//Ejecutar la funciÃ3n que envia los datos a Thingsboard
lastSend = millis();
}
client.loop();
}
void LeerFlujoDeCalor(){//FunciÃ3n para leer el flujo de calor
sensorValue = analogRead(sensorPin);// realizar la lectura
value = fmap(sensorValue, 0, 1023, 0.0,5.0);// cambiar escala a 0.0 - 5.0
microvolts=(value-2.500)*1000000;//Convertir lecturas a voltaje, restar referencia
flujo=(microvolts/1000)/1.44;//Calcular el flujo de calor
}
void leerTermopar(){//FunciÃ3n para obtener datos de termopares
delay(500);//Retardo
//Definiciones para el shield de termopares
static struct var_max31856 TC_CH0, TC_CH1, TC_CH2, TC_CH3;
struct var_max31856 *tc_ptr;
// Leer canal 0
tc_ptr = &TC_CH0;
//Poner apuntador
thermocouple0.MAX31856_update(tc_ptr);
//Actualizar canal 0 del MAX31856
// Leer canal 1
tc_ptr = &TC_CH1;
//Poner apuntador
thermocouple1.MAX31856_update(tc_ptr);
//Actualizar canal 1 del MAX31856
// Leer canal 2
tc_ptr = &TC_CH2;
//Poner apuntador
thermocouple2.MAX31856_update(tc_ptr);
// Leer canal 3
//Actualizar canal 2 del MAX31856
tc_ptr = &TC_CH3;
//Poner apuntador
thermocouple3.MAX31856_update(tc_ptr);
//Actualizar canal 3 del MAX31856
// ##### Imprmir la informaciÃ3n por el puerto serial ####
// Thermocouple channel 0
Serial.print("Thermocouple 0: ");
//Imprimir encabezado del termopar 0
if(TC_CH0.status)
{
//Evaluar y mostrar errores en caso de haber
Serial.println("fault(s) detected");
Serial.print("Fault List: ");
if(0x01 & TC_CH0.status){Serial.print("OPEN ");}
if(0x02 & TC_CH0.status){Serial.print("Overvolt/Undervolt ");}
if(0x04 & TC_CH0.status){Serial.print("TC Low ");}
if(0x08 & TC_CH0.status){Serial.print("TC High ");}
if(0x10 & TC_CH0.status){Serial.print("CJ Low ");}
if(0x20 & TC_CH0.status){Serial.print("CJ High ");}
if(0x40 & TC_CH0.status){Serial.print("TC Range ");}
if(0x80 & TC_CH0.status){Serial.print("CJ Range ");}
Serial.println(" ");
}
else //Si no hay errores imprimir los datos
{
Serial.println("no faults detected");
//Temperatura interna del MAX31856
tmp0 = (double)TC_CH0.ref_jcn_temp * 0.015625;//Convertir a doble
Serial.print("Tint = ");
//Imprimir temperatura interna
if((-100 > tmp0) || (150 < tmp0)){
  Serial.println("unknown fault");
  }
else{Serial.println(tmp0);}//Temperatura del termopar
tmp0 = (double)TC_CH0.lin_tc_temp * 0.0078125;//Convertir a doble
Serial.print("TC Temp = ");
//Imprimir temperatura de termopar
Serial.println(tmp0);
}
//La misma explicaciÃ3n que el termopar cero
Serial.print("Thermocouple 1: ");
if(TC_CH1.status)
{
Serial.println("fault(s) detected");
Serial.print("Fault List: ");
if(0x01 & TC_CH1.status){Serial.print("OPEN ");}
if(0x02 & TC_CH1.status){Serial.print("Overvolt/Undervolt ");}
if(0x04 & TC_CH1.status){Serial.print("TC Low ");}
if(0x08 & TC_CH1.status){Serial.print("TC High ");}
if(0x10 & TC_CH1.status){Serial.print("CJ Low ");}
if(0x20 & TC_CH1.status){Serial.print("CJ High ");}
if(0x40 & TC_CH1.status){Serial.print("TC Range ");}
if(0x80 & TC_CH1.status){Serial.print("CJ Range ");}
Serial.println(" ");
}
else
{
Serial.println("no faults detected");
tmp1 = (double)TC_CH1.ref_jcn_temp * 0.015625;
Serial.print("Tint = ");
if((-100 > tmp1) || (150 < tmp1)){Serial.println("unknown fault");}
else{Serial.println(tmp1);}
tmp1 = (double)TC_CH1.lin_tc_temp * 0.0078125;
Serial.print("TC Temp = ");
Serial.println(tmp1);}
Serial.print("Thermocouple 2: ");
if(TC_CH2.status)
{
Serial.println("fault(s) detected");
Serial.print("Fault List: ");
if(0x01 & TC_CH2.status){Serial.print("OPEN ");}
if(0x02 & TC_CH2.status){Serial.print("Overvolt/Undervolt ");}
if(0x04 & TC_CH2.status){Serial.print("TC Low ");}
if(0x08 & TC_CH2.status){Serial.print("TC High ");}
if(0x10 & TC_CH2.status){Serial.print("CJ Low ");}
if(0x20 & TC_CH2.status){Serial.print("CJ High ");}
if(0x40 & TC_CH2.status){Serial.print("TC Range ");}
if(0x80 & TC_CH2.status){Serial.print("CJ Range ");}
Serial.println(" ");
}
else
{
Serial.println("no faults detected");
tmp2 = (double)TC_CH2.ref_jcn_temp * 0.015625;
Serial.print("Tint = ");
if((-100 > tmp2) || (150 < tmp2)){Serial.println("unknown fault");}
else{Serial.println(tmp2);}
tmp2 = (double)TC_CH2.lin_tc_temp * 0.0078125;
Serial.print("TC Temp = ");
Serial.println(tmp2);
}
Serial.print("Thermocouple 3: ");
if(TC_CH3.status)
{
Serial.println("fault(s) detected");Serial.print("Fault List: ");
if(0x01 & TC_CH3.status){Serial.print("OPEN ");}
if(0x02 & TC_CH3.status){Serial.print("Overvolt/Undervolt ");}
if(0x04 & TC_CH3.status){Serial.print("TC Low ");}
if(0x08 & TC_CH3.status){Serial.print("TC High ");}
if(0x10 & TC_CH3.status){Serial.print("CJ Low ");}
if(0x20 & TC_CH3.status){Serial.print("CJ High ");}
if(0x40 & TC_CH3.status){Serial.print("TC Range ");}
if(0x80 & TC_CH3.status){Serial.print("CJ Range ");}
Serial.println(" ");
}
else
{
Serial.println("no faults detected");
tmp3 = (double)TC_CH3.ref_jcn_temp * 0.015625;
Serial.print("Tint = ");
if((-100 > tmp3) || (150 < tmp3)){Serial.println("unknown fault");}
else{Serial.println(tmp3);}
tmp3 = (double)TC_CH3.lin_tc_temp * 0.0078125;
Serial.print("TC Temp = ");
Serial.println(tmp3);
}
}
void EnviarDatos(){//FunciÃ3n para enviar datos al servidor de Thingsboard
//Asignar los valores a nuevas variables
String termopar0 = String(tmp0);
String termopar1 = String(tmp1);
String termopar2 = String(tmp2);
String termopar3 = String(tmp3);
String FlujoDeCalor = String(flujo);
//Organizar los datos para enviar a ThingsboardString 
String payload = "{";
payload += "\"termopar0\":"; payload += tmp0;payload += ",";
payload += "\"termopar1\":"; payload += tmp1;payload += ",";
payload += "\"termopar2\":"; payload += tmp2;payload += ",";
payload += "\"termopar3\":"; payload += tmp3;payload += ",";
payload += "\"FlujoDeCalor\":"; payload += flujo;payload += "}";
char attributes[100];
payload.toCharArray( attributes, 100 );
client.publish( "v1/devices/me/telemetry", attributes );//Enviar los datos
Serial.println( attributes );//Mostrar datos enviados en puerto serial
}
void InitWiFi()
{
  
soft.begin(9600);//Iniciar puerto serial para mÃ3dulo ESP
WiFi.init(&soft);//iniciar mÃ3fulo ESP
//Verificar la conexiÃ3n del ESP
if (WiFi.status() == WL_NO_SHIELD) {
Serial.println("WiFi shield not present");
//No continuar
while (true);
}
Serial.println("Connecting to AP ...");
//Intentar conectar a la red
while ( status != WL_CONNECTED) {
Serial.print("Attempting to connect to WPA SSID: ");
Serial.println(WIFI_AP);
//COnectar a la red
status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
delay(500);}
Serial.println("Connected to AP");
}
void reconnect() {
//Ciclo hasta lograr volver a conectar
while (!client.connected()) {
Serial.print("Connecting to ThingsBoard node ...");
//Intentar conectar
if ( client.connect("Arduino Uno Device", TOKEN, NULL) ) {
Serial.println( "[DONE]" );
} 
else {
Serial.print( "[FAILED] [ rc = " );
Serial.print( client.state() );
Serial.println( " : retrying in 5 seconds]" );
//Esperar 5 segundos y reintentar
delay( 5000 );
}
}
}
float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
