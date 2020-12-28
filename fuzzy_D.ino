// 1. Baca data sensor: 
// PosisiBolaX = Potensio1 36 (0 - 1023)
// PosisiBolaY = Potensio2 39 (0 - 1023)

// 2. Fuzzyfikasi 
// fuzzyPosisiBolaX ()
// fuzzyPosisiBolaY ()

#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#define DEKAT   0
#define SEDANG  1
#define JAUH    2

const char* ssid = "KISHELL"; // Nama WIFI
const char* pass = "bayardulu"; // Password Wifi

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 36;
const int potPin1 = 39;

// variable for storing the potentiometer value
int potValue = 0;
int potValue1 = 0;

float posisiXB[2];
float posisiYB[2];

float posisiXB_dekat;
float posisiXB_sedang;
float posisiXB_jauh;
float posisiYB_dekat;
float posisiYB_sedang;
float posisiYB_jauh;

long dataXB;
long dataYB;
float temp;
float defuz, pwm;

//rulebase
float rule [3][3];
float rule00, rule01, rule02;
float rule10, rule11, rule12;
float rule20, rule21, rule22;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  delay(500);
  //delay(1000);
}

void loop() {
  bacaDataSensor();
  
  fuzzyPosisiBolaY();
//Serial.print("posisiYB1 dekat = "); Serial.println(posisiYB[DEKAT]);
//  Serial.print("posisiYB1 sedang = "); Serial.println(posisiYB[SEDANG]);
//  Serial.print("posisiYB1 jauh = "); Serial.println(posisiYB[JAUH]);
  fuzzyPosisiBolaX();
//  Serial.print("posisiXB dekat = "); Serial.println(posisiXB_dekat);
//  Serial.print("posisiXB sedang = "); Serial.println(posisiXB_sedang);
//  Serial.print("posisiXB jauh = "); Serial.println(posisiXB_jauh);
//  Serial.print("posisiYB dekat = "); Serial.println(posisiYB_dekat);
//  Serial.print("posisiYB sedang = "); Serial.println(posisiYB_sedang);
//  Serial.print("posisiYB jauh = "); Serial.println(posisiYB_jauh);
  RuleEva();
  Defuzzy();
  database();
  delay(500);
}

long maps(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void bacaDataSensor() {
  // Reading potentiometer value
  potValue = analogRead(potPin);
  //potValue = map(potValue, 0 , 4095, 0, 1023);
  dataXB = maps(potValue, 0, 4095, 0, 1023);
  Serial.print("dataXB = ");
  Serial.print(dataXB);
  
  potValue1 = analogRead(potPin1);
  dataYB = maps(potValue1, 0 , 4095, 0, 1023);
  Serial.print("\t dataYB = ");
  Serial.println(dataYB);

}

void fuzzyPosisiBolaX()
{
    
  // untuk posisi bola X dekat
  // output posisiXB[DEKAT] antara 0 - 1 
  if(dataXB <= 256)
    posisiXB_dekat = 1;
  else if ((dataXB > 256) && (dataXB <= 512)) {
//    Serial.print("\n masuk kondisi\n");
    posisiXB_dekat = ((float)(512 - dataXB)/(float)(512 - 256));
//    Serial.print(posisiXB[DEKAT]);
  }
  else 
    posisiXB_dekat = 0;

//  Serial.print("posisiYB111 => ");
//  Serial.print(" [DEKAT] = "); Serial.print(posisiYB_dekat);
//  Serial.print(" [SEDANG] = "); Serial.print(posisiYB_sedang);
//  Serial.print(" [JAUH] = "); Serial.println(posisiYB_jauh);
//  
  // untuk posisi bola x sedang   
  // output posisiXB[SEDANG] antara 0 - 1
  if (dataXB <= 256) 
    posisiXB_sedang = 0;
  else if ((dataXB > 256) && (dataXB <= 512)) {
    posisiXB_sedang = (float)(dataXB - 256)/(float)(512 - 256); 
  }
  else if ((dataXB > 512) && (dataXB <= 768)) {
    posisiXB_sedang = (float)(768 - dataXB) / (float)(768 - 512);
  }
  else 
    posisiXB_sedang = 0;          


  //  untuk posisi bola x jauh
  // output posisiXB[JAUH] antara 0 - 1
  if (dataXB <= 512)
    posisiXB_jauh = 0;
  else if ((dataXB > 512) && (dataXB <= 768)) {
    posisiXB_jauh = (float)(dataXB - 512) / (float)(768 - 512);
  }
  else 
    posisiXB_jauh = 1;

  // Cek      
  Serial.print("posisiXB => ");
  Serial.print(" [DEKAT] = "); Serial.print(posisiXB_dekat);
  Serial.print(" [SEDANG] = "); Serial.print(posisiXB_sedang);
  Serial.print(" [JAUH] = "); Serial.print(posisiXB_jauh);
  Serial.println("\n----------------------");

  
   
}

void fuzzyPosisiBolaY()
{
  // untuk posisi bola Y Dekat
  if (dataYB <= 256) 
    posisiYB_dekat = 1;
  else if ((dataYB > 256) && (dataYB <= 512)) {
    posisiYB_dekat = (float)(512 - dataYB) / (float)(512 - 256);     
  } else {
    posisiYB_dekat = 0;
  }

  // untuk posisi bola Y Sedang
  if (dataYB <= 256) {
    posisiYB_sedang = 0;    
  } else if ((dataYB > 256) && (dataYB <= 512)) {
    posisiYB_sedang = (float)(dataYB - 256)/ (float)(512 - 256);
  } else if ((dataYB > 512) && (dataYB <= 768)) {
    posisiYB_sedang = (float)(768 - dataYB) / (float)(768 - 512);
  } else {
    posisiYB_sedang = 0;
  }

  // untuk posisi bola Y Jauh
  if (dataYB <= 512) {
    posisiYB_jauh = 0; 
  } else if ((dataYB > 512) && (dataYB <= 768)) {
    posisiYB_jauh = (float)(dataYB - 512) / (float)(768 - 512);
  } else {
    posisiYB_jauh = 1;
  }

  // Cek      
  Serial.print("posisiYB => ");
  Serial.print(" [DEKAT] = "); Serial.print(posisiYB_dekat);
  Serial.print(" [SEDANG] = "); Serial.print(posisiYB_sedang);
  Serial.print(" [JAUH] = "); Serial.print(posisiYB_jauh);
  Serial.println("\n----------------------");
  
}

float minimum(float a, float b){
    if(a<b){
      return a;
      }
      else return b;
  }

void RuleEva (){
  int i, j;
  //Serial.print("eva posisiYB dekat = "); Serial.println(posisiYB[0]);
  //Serial.print("eva posisiYB sedang = "); Serial.println(posisiYB[1]);
  //Serial.print("eva posisiYB jauh = "); Serial.println(posisiYB[2]);
  //Serial.print("eva posisiXB dekat = "); Serial.println(posisiXB[0]);
  //Serial.print("eva posisiXB sedang = "); Serial.println(posisiXB[1]);
  //Serial.print("eva posisiXB jauh = "); Serial.println(posisiXB[2]);
  
  
//  for ( i=0; i<=2; i=i+1)
//   {
//     for ( j=0; j<=2; j=j+1)
//     {
//       temp = minimum(posisiXB[i], posisiYB[j]);
//       rule [i][j] = temp;
//       //Serial.print("posisiXB = "); Serial.println(posisiXB[DEKAT]);
//       //Serial.print("posisiYB = "); Serial.println(posisiYB[JAUH]);
//     } 
//   } 

   
//   rule00 = rule [0][0]; // (dekat,dekat = Lambat)
//   rule01 = rule [0][1]; // (dekat,sedang = Lambat)
//   rule02 = rule [0][2]; // (dekat,jauh = Sedang)
//   
//   rule10 = rule [1][0]; // (sedang,dekat = Lambat)
//   rule11 = rule [1][1]; // (sedang,sedang = Sedang)
//   rule12 = rule [1][2]; // (sedang,jauh = Cepat)
//   
//   rule20 = rule [2][0]; // (jauh,dekat = Sedang)
//   rule21 = rule [2][1]; // (jauh,sedang = Cepat)
//   rule22 = rule [2][2]; // (jauh,jauh= Cepat)

   rule00 = rule[0][0] = minimum(posisiXB_dekat, posisiYB_dekat); // (dekat,dekat = Lambat)
   rule01 = rule[0][1] = minimum(posisiXB_dekat, posisiYB_sedang); // (dekat,sedang = Lambat)
   rule02 = rule[0][2] = minimum(posisiXB_dekat, posisiYB_jauh); // (dekat,jauh = Sedang)
   
   rule10 = rule[1][0] = minimum(posisiXB_sedang, posisiYB_dekat); // (sedang,dekat = Lambat)
   rule11 = rule[1][1] = minimum(posisiXB_sedang, posisiYB_sedang); // (sedang,sedang = Sedang)
   rule12 = rule[1][2] = minimum(posisiXB_sedang, posisiYB_jauh); // (sedang,jauh = Cepat)
   
   rule20 = rule[2][0] = minimum(posisiXB_jauh, posisiYB_dekat); // (jauh,dekat = Sedang)
   rule21 = rule[2][1] = minimum(posisiXB_jauh, posisiYB_sedang); // (jauh,sedang = Cepat)
   rule22 = rule[2][2] = minimum(posisiXB_jauh, posisiYB_jauh); // (jauh,jauh= Cepat)


//   Serial.print(" rule00 = "); Serial.println(rule00);
//   Serial.print(" rule01 = "); Serial.println(rule01);
//   Serial.print(" rule02 = "); Serial.println(rule02);
//
//   Serial.print(" rule10 = "); Serial.println(rule10);
//   Serial.print(" rule11 = "); Serial.println(rule11);
//   Serial.print(" rule12 = "); Serial.println(rule12);
//
//   Serial.print(" rule20 = "); Serial.println(rule20);
//   Serial.print(" rule21 = "); Serial.println(rule21);
//   Serial.print(" rule22 = "); Serial.println(rule22);
  }

void Defuzzy () {
  // metode sugeno (weighted average)
  float lambat = 100;
  float sedang = 200;
  float cepat = 250;
  RuleEva();
  //pwm = (rule00 * lambat) + (rule01 * lambat)+ (rule02 *lambat)+ (rule10 * lambat)+ (rule11 * sedang)+ (rule12 * cepat) + (rule20 * cepat)+ (rule21 * cepat)+ (rule22 * cepat);
  pwm = (rule00 * lambat) + (rule01 * lambat)+ (rule02 *sedang)+ (rule10 * lambat)+ (rule11 * sedang)+ (rule12 * cepat) + (rule20 * sedang)+ (rule21 * cepat)+ (rule22 * cepat);
  //Serial.print(" pwm1 = "); Serial.print(pwm);
  
  defuz = 0;
  int i, j;
  for ( i=0; i<=2; i=i+1)
  {
    for ( j=0; j<=2; j=j+1)
    {
      defuz = defuz + rule [i][j];
    } 
  } 
  pwm = pwm / defuz;
  Serial.print(" pwm = "); Serial.print(pwm);
  Serial.println("\n----------------------");
}

void database(){
  
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin("http://192.168.100.5/fuzzy/simpan-data.php?PWM="+String(pwm)+"&posisix_dekat="+String(posisiXB_dekat)+"&posisix_sedang="+String(posisiXB_sedang)+"&posisix_jauh="+String(posisiXB_jauh)+"&posisiy_dekat="+String(posisiYB_dekat)+"&posisiy_sedang="+String(posisiYB_sedang)+"&posisiy_jauh="+String(posisiYB_jauh)+"&dataxb="+String(dataXB)+"&datayb="+String(dataYB));
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "data=";
    Serial.println("httpRequestData~ ");
    //Serial.println(httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
}
