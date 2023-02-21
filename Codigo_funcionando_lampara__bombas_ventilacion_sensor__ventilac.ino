
// Con Arduino UNO,(RTC), Modulo Rele, Modulo I2C para LCD 2x16 
// Conexioni2C :Arduino UNO  SCL-> A5   SDA->A4  VCC->5V  GND->GND
// Conexion RTC :Arduino UNO    GND->GND VCC->5V SCL-> SCL  SDA->SDA los dos pines despues del ...12,13,GND,AREF,SDA,SCL
// Conexion Rele:Arduino UNO GND->GND  VCC->5V  IN->7
// NOTA: se debe cargar dos veces este programa 1. Con la linea  RTC.adjust(DateTime(__DATE__, __TIME__)); 
//                                              2. Sin esa linea

#include <Wire.h> 
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <DHT.h>


LiquidCrystal_I2C lcd(0x3F,16,2);    // inicializa la interfaz I2C del LCD 16x2
RTC_DS1307 RTC;                     // inicializa el modulo RTC


int r_diaSemana;                     // almacena el resultado del dia de la semana calculado
const int intractor = 7;             // Pin 7 encargado de activar los intractores
const int extractor = 8;             // Pin 8 encargado de activar los extractores
const int lampara   = 6;             // Pin 6 encargado de activar la lampara
const int bombas    = 5;             // Pin 5 encargado de activar las bombas de agua

unsigned long aguarda =2000;

int segundo;
int minuto;
int hora;

int sensor = 2;
float temp;
int humedad;
DHT dht (sensor, DHT11);

//////////////////////////////// Horario Durante el Día //////////////////////////////////////////////////////////////////////////////////////////////
// Hora en la que se enciende el sistema de ventilacion escrito  h1=Hora, m1=Minutos, s1=Segundos
// Cuando no se programa ninguna hora se debe dejar escrito el numero 99  

int h1_c1=9;     int m1_c1=10;   
int h2_c1=9;     int m2_c1=40;   
int h3_c1=10;    int m3_c1=10;   
int h4_c1=10;    int m4_c1=40;   
int h5_c1=11;    int m5_c1=10;   
int h6_c1=11;    int m6_c1=40;   
int h7_c1=12;    int m7_c1=10;   
int h8_c1=12;    int m8_c1=40;   
int h9_c1=13;    int m9_c1=10;   
int h10_c1=13;   int m10_c1=40;  
int h11_c1=14;   int m11_c1=10; 
int h12_c1=14;   int m12_c1=40; 
int h13_c1=15;   int m13_c1=10;  
int h14_c1=15;   int m14_c1=40;   
int h15_c1=16;   int m15_c1=10;   
int h16_c1=16;   int m16_c1=40;   
int h17_c1=17;   int m17_c1=10;   
int h18_c1=17;   int m18_c1=40;   
int h19_c1=18;   int m19_c1=10;   
int h20_c1=18;   int m20_c1=40;   
int h21_c1=19;   int m21_c1=10;   
int h22_c1=19;   int m22_c1=40;  
int h23_c1=20;   int m23_c1=10; 
int h24_c1=20;   int m24_c1=40; 
int h25_c1=21;   int m25_c1=10;  
int h26_c1=21;   int m26_c1=40;


//////////////////////////////// Horario Durante la noche ///////////////////////////////////////////////////////////////////////////////////////////
// Hora en la que se enciende el sistema de ventilacion escrito  h1=Hora, m1=Minutos, s1=Segundos

int h1_c2=22;   int m1_c2=25;   
int h2_c2=23;   int m2_c2=25;   
int h3_c2=24;   int m3_c2=25;   
int h4_c2=1;    int m4_c2=25;   
int h5_c2=2;    int m5_c2=25;   
int h6_c2=3;    int m6_c2=25;  
int h7_c2=4;    int m7_c2=25;   
int h8_c2=5;    int m8_c2=25;   
int h9_c2=6;    int m9_c2=25;   
int h10_c2=7;   int m10_c2=25;  
int h11_c2=8;   int m11_c2=25;  

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////Horario para bombas//////////////////////////////////////////////////////////////////////////////////////////////////////////
int h1_c3=22;    int m1_c3=00;  int s1_c3=0;


////////////////////////////////// Void Setup() ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup () 
{   
  pinMode(extractor, OUTPUT);                    // Configura como salida el pin 8
  pinMode(intractor, OUTPUT);                    // Configura como salida el pin 7
  pinMode(lampara,   OUTPUT);                    // Configura como salida el pin 6
  pinMode(bombas,    OUTPUT);                    // Configura como salida el pin 5
  
  Wire.begin();
  dht.begin();                               
  RTC.begin();                                   // Inicia la comunicaci¢n con el RTC
  RTC.adjust(DateTime(__DATE__, __TIME__));      // Lee la fecha y hora del PC (Solo en la primera carga)
                                                 // el anterior se usa solo en la configuracion inicial luego se pone como comentario
                                                 // y se vuelve a cargar el programa sin esa linea.
  Serial.begin(9600);                            // Establece la velocidad de datos del puerto serie a 9600
  lcd.init();
  lcd.backlight();                               // Coloca luz de fondo al LCD
  lcd.clear();                                   // Borra el  LCD
  lcd.setCursor(0,0);
  lcd.print("SmartGreenhouse");
  lcd.setCursor(7,1);
  lcd.print(":P");
  delay(2500);
  lcd.clear();

  digitalWrite(bombas,HIGH);
 
} 
////////////////////////////////// Void loop() /////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  DateTime now = RTC.now();          // Obtiene la fecha y hora del RTC

  int contacto1 = analogRead(A0);    //Lee el valor de los contactos para escoger el horario
  int contacto2 = analogRead(A1);
  int contacto3 = analogRead(A2);

  Serial.print(now.year(), DEC);     // Año
  Serial.print('/');
  Serial.print(now.month(), DEC);    // Mes
  Serial.print('/');
  Serial.print(now.day(), DEC);      // Dia
  Serial.print(' ');
  Serial.print(now.hour(), DEC);     // Horas
  Serial.print(':');
  Serial.print(now.minute(), DEC);   // Minutos
  Serial.print(':');
  Serial.print(now.second(), DEC);   // Segundos
  Serial.println();
  lcd.setCursor(0,0);
  lcd.clear();                                                  
  lcd.setCursor(6,0);
  lcd.print(". ");
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC);
  segundo=now.second();
  minuto=now.minute();
  hora=now.hour();


  humedad = dht.readHumidity();
  temp = dht.readTemperature(); 
  
  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.setCursor(2,1);
  lcd.print(temp);
  aguarda = millis();
  
  lcd.setCursor(9,1);
  lcd.print("H:");
  lcd.setCursor(11,1);
  lcd.print(humedad);
  lcd.print("%");
  aguarda = millis();
  


 int r_diaSemana=dia_de_semana();               // Llama a la funcion que calcula el dia de la semana y lo almacena en r_diaSemana

   if (contacto1 >= 1000)                       // Si el contacto 1 esta activo (Horario durante dia)
    {  
      horario_dia();                            // Llama a la funcion que activa el horario dia
    }
    else
    {
      lcd.setCursor(0,0);
      lcd.print(" ");
    }
    
   if (contacto2 >= 1000)                       // Si el contacto 2 esta activo (Horario durante noche)
    {
      horario_noche();                          // Llama a la funcion que activa el horario noche
    }
    else
    {
       lcd.setCursor(2,0);
       lcd.print(" ");
    }

    if(contacto3 >= 1000)                       // Si el contacto 3 esta activo (Activacion de bombas cada 3 dias) 
    {
      bombas_funcionando();                     //Llama a la funcion que activa las bombas
    }
    else
    {
       lcd.setCursor(4,0);
       lcd.print(" ");
    }
      
 
 digitalWrite(intractor, HIGH);                 // apaga los intractores
 digitalWrite(extractor,HIGH);                  // apaga los extractores
 digitalWrite(bombas,HIGH);
 
 delay(1000);                                   // La informaci¢n se actualiza cada 1/2 seg.
 lcd.setCursor(9,0);                            // Borra parte de la pantalla del LCD
 lcd.print("                ");
 lcd.setCursor(8,1);
 lcd.print("                ");


//////////////////////////////////Funcion que regula el horario de encendido de la lampara//////////////////////////////////////////////////////////////////////////
if((hora>=9)&&(hora<21))
{
  digitalWrite(lampara,LOW);  
}
  else
  {  
    digitalWrite(lampara,HIGH);
  }
}   
//////////////////////////////// Fin del void loop(), inician las Funciones/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Funcion que activa la ventilacion //////////////////////////////////////////////////////////////////////////////////////////////////

void activar_ventilacion_dia()
{
  lcd.clear(); 
  lcd.setCursor(2,0);
  lcd.print("Ventilacion");
  lcd.setCursor(3,1);
  lcd.print("Encendida");
  Serial.println("Ventilacion Activa");
  
  for(int i=0; i<=9; i++)
  { 
    digitalWrite(intractor,LOW); 
    digitalWrite(extractor,LOW);
    delay(60000); 
  }
}
////////////////////////////////////Funcion que activa la ventilacion////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void activar_ventilacion_noche()
{
  lcd.clear(); 
  lcd.setCursor(2,0);
  lcd.print("Ventilacion");
  lcd.setCursor(3,1);
  lcd.print("Encendida");
  Serial.println("Ventilacion Activa");
  
  for(int i=0; i<=9; i++)
  {
    digitalWrite(intractor, LOW); 
    digitalWrite(extractor,LOW);
    delay(60000); 
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////Funcion que activa las bombas//////////////////////////////////////////////////////////////////////////////////////////////////////
void activar_bombas()
{
      digitalWrite(bombas,LOW);
      lcd.clear(); 
      lcd.setCursor(0,0);
      lcd.print("Riego encendido");
      lcd.setCursor(8,1);
      lcd.print(":)");
      Serial.println("Bombas activadas");
      delay(25000);
      digitalWrite(bombas,HIGH);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Calcula el dia de la Semana /////////////////////////////////////////////////////////////////////////////////////////////////////////
int dia_de_semana()
{
  // Encuentra el dia de la semana de una fecha
  int n_dia;
  int r_dia;
  int n_mes;
  int t_mes;
  int n_anno;
  int d_anno;
  int t_siglo=6;

  DateTime now = RTC.now(); //fecha y hora del RTC
  lcd.setCursor(13,1);
  n_anno=(now.year()-2000);
  d_anno=n_anno/4;
  n_dia=now.day();
  n_mes=now.month();

 switch (n_mes) 
 {
    case 1:
      t_mes=0;
      break;
    case 2:
      t_mes=3;
      break;
    case 3:
      t_mes=3;
      break;
    case 4:
      t_mes=6;
      break;
    case 5:
      t_mes=1;
      break;
    case 6:
      t_mes=4;
      break;
    case 7:
      t_mes=6;
      break;
    case 8:
      t_mes=2;
      break;
    case 9:
      t_mes=5;
      break;
    case 10:
      t_mes=0;
      break;
    case 11:
      t_mes=3;
      break;
    case 12:
      t_mes=5;
      break;
    default: 
      t_mes=t_mes;
    break;
 }

 r_dia=n_dia+t_mes+n_anno+d_anno+t_siglo;
 r_dia = r_dia % 7;

 switch (r_dia) 
 {
      case 1:
        Serial.print(" Lun ");
      break;
      case 2:
       Serial.println(" Mar ");
      break;
      case 3:
       Serial.print(" Mie ");
      break;
      case 4:
       Serial.print(" Jue ");
      break;
      case 5:
       Serial.print(" Vie ");
      break;
      case 6:
       Serial.print(" Sab ");
      break;
      case 0:
       Serial.print(" Dom ");
      break;
      default: 
       lcd.print("---");
       Serial.print(" ---");
      break;
 } 
 return r_dia; 
}
/////////////////////Condicional del Horario dia  /////////////////////////////////////////////////////////////////////////////////////////////////////
void horario_dia()
{ 
   lcd.setCursor(0,0);
   lcd.print("D");
   Serial.print("Horario_Dia ");
   if ((hora==h1_c1)  && (minuto==m1_c1))   activar_ventilacion_dia();
   if ((hora==h2_c1)  && (minuto==m2_c1))   activar_ventilacion_dia();
   if ((hora==h3_c1)  && (minuto==m3_c1))   activar_ventilacion_dia();
   if ((hora==h4_c1)  && (minuto==m4_c1))   activar_ventilacion_dia();
   if ((hora==h5_c1)  && (minuto==m5_c1))   activar_ventilacion_dia();
   if ((hora==h6_c1)  && (minuto==m6_c1))   activar_ventilacion_dia();
   if ((hora==h7_c1)  && (minuto==m7_c1))   activar_ventilacion_dia();
   if ((hora==h8_c1)  && (minuto==m8_c1))   activar_ventilacion_dia();
   if ((hora==h9_c1)  && (minuto==m9_c1))   activar_ventilacion_dia();
   if ((hora==h10_c1) && (minuto==m10_c1))  activar_ventilacion_dia();
   if ((hora==h11_c1) && (minuto==m11_c1))  activar_ventilacion_dia();
   if ((hora==h12_c1) && (minuto==m12_c1))  activar_ventilacion_dia();
   if ((hora==h13_c1) && (minuto==m13_c1))  activar_ventilacion_dia();
   if ((hora==h14_c1) && (minuto==m14_c1))  activar_ventilacion_dia();
   if ((hora==h15_c1) && (minuto==m15_c1))  activar_ventilacion_dia();
   if ((hora==h16_c1) && (minuto==m16_c1))  activar_ventilacion_dia();
   if ((hora==h17_c1) && (minuto==m17_c1))  activar_ventilacion_dia();
   if ((hora==h18_c1) && (minuto==m18_c1))  activar_ventilacion_dia();
   if ((hora==h19_c1) && (minuto==m19_c1))  activar_ventilacion_dia();
   if ((hora==h20_c1) && (minuto==m20_c1))  activar_ventilacion_dia();
   if ((hora==h21_c1) && (minuto==m21_c1))  activar_ventilacion_dia();
   if ((hora==h22_c1) && (minuto==m22_c1))  activar_ventilacion_dia();
   if ((hora==h23_c1) && (minuto==m23_c1))  activar_ventilacion_dia();
   if ((hora==h24_c1) && (minuto==m24_c1))  activar_ventilacion_dia();
   if ((hora==h25_c1) && (minuto==m25_c1))  activar_ventilacion_dia();
   if ((hora==h26_c1) && (minuto==m26_c1))  activar_ventilacion_dia(); 
}
  
////////////////////Condicional del Horario noche  ///////////////////////////////////////////////////////////////////////////////////////////////////
void horario_noche()
{ 
   lcd.setCursor(2,0);
   lcd.print("N");
   Serial.print("Horario_Noche ");
   if ((hora==h1_c2)  && (minuto==m1_c2))   activar_ventilacion_noche();
   if ((hora==h2_c2)  && (minuto==m2_c2))   activar_ventilacion_noche();
   if ((hora==h3_c2)  && (minuto==m3_c2))   activar_ventilacion_noche();
   if ((hora==h4_c2)  && (minuto==m4_c2))   activar_ventilacion_noche();
   if ((hora==h5_c2)  && (minuto==m5_c2))   activar_ventilacion_noche();
   if ((hora==h6_c2)  && (minuto==m6_c2))   activar_ventilacion_noche();
   if ((hora==h7_c2)  && (minuto==m7_c2))   activar_ventilacion_noche();
   if ((hora==h8_c2)  && (minuto==m8_c2))   activar_ventilacion_noche();
   if ((hora==h9_c2)  && (minuto==m9_c2))   activar_ventilacion_noche();
   if ((hora==h10_c2) && (minuto==m10_c2))  activar_ventilacion_noche();
   if ((hora==h11_c2) && (minuto==m11_c2))  activar_ventilacion_noche();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////Condicional activacion de las bombas///////////////////////////////////////////////////////////////////////////////////////////////////
void bombas_funcionando()
{
  lcd.setCursor(4,0);
  lcd.print("B");
  if((r_diaSemana==0)||(r_diaSemana==3))
  {
    Serial.print("Bombas C/3");
    if((hora==h1_c3)&&(minuto==m1_c3)&&(segundo==s1_c3)) activar_bombas();
  }    
    digitalWrite(bombas,HIGH);  
}
