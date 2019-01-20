
/* 
 * *************************
 * Capteur : DHT11
 * 
 * Les broches sont numérotées de la gauche vers la droite lorsque l'on regarde le capteur de face
 * 
 * Broche n°1 connectée au +3.3V
 * Broche n°2 (data) connectée à la broche 'D4' du NodeMcu (Pin 2 pour l'arduino) avec une résistance de 10 K reliée au +3.3v
 * Broche n°3 non connectée
 * Broche n°4 connectée à la masse (GND)
 * 
 * Installer la bibliothèque 'DHT sensor library by Adarftuit'
 * Cette librairie necessite une librairie supplémentaire qui est disponible à l'adresse : https://github.com/adafruit/Adafruit_Sensor
 * et qui doit être installée selon ce tutoriel : https://learn.adafruit.com/adafruit-all-about-arduino-libraries-install-use/installing-a-library-on-linux
 *
 * *************************
 *  Convertisseur de niveau I2C
 *  
 *  Le NodeMcu fonctionne sous 3.3V.
 *  L'afficheur fonctionne sous 5V et avec un bus I2C.
 *  La connexion ne peut pas être directe sans risquer d'endommager le NodeMcu.
 *  Un convertisseur de niveau est intercalé entre le NodeMcu et l'afficheur
 *  
 *  Broche 'BVCC' connectée au +3.3V : broche '3V' sur le NodeMcu
 *  Broche 'BSCL' connectée à la broche 'D1' du NodeMcu (Pin 5 pour l'arduino)
 *  Broche 'BSDA' connectée à la broche 'D2' du NodeMcu (Pin 4 pour l'arduino)
 *  Broche 'BGND' connectée à la masse : une broche 'G' du NodeMcu (GND)
 *  Broche 'AVCC' connectée au +5V : broche 'VV' sur le NodeMcu
 *  Broche 'AGND' connectée à la masse : une broche 'G' du NodeMcu (GND)
 *  Les broches 'ASCL', 'ASDA' et 'AGND' sont connectées à l'afficheur
 *  
 * *************************
 * Afficheur 2 lignes x 16 caractères sur bur I2C
 *  
 * Broche 'GND' connectée à la broche AGND du convertisseur (ou directement à la masse : une broche 'G' du NodeMcu)
 * Broche 'VCC' connectée à la broche AVCC du convertisseur (ou directement au +5V : broche 'VV' sur le NodeMcu)
 * Broche 'SDA' connectée à la broche ASDA du convertisseur
 * Broche 'SCL' connectée à la broche ASCL du convertisseur
 * 
 * Installer la bibliothèque 'LiquidCrystal I2C by Frank de Brabander'
 *  
 *  
 */

// Déclaration des librairies
#include <DHT.h>                      // pour communiquer avec le capteur DHT
#include <Wire.h>                     // pour utiliser le bus I2C
#include <LiquidCrystal_I2C.h>        // pour piloter l'afficheur
#include <ESP8266WiFi.h>              // pour le support du wifi
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>        // pour utliser le wifi

// Préparation du capteur DHT
#define DHTPIN 2                      // broche sur laquelle est raccordée la data du capteur (la broche 'D4' du NodeMcu correspond à la broche 2 de l'arduino)
#define DHTTYPE DHT11                 // précise la référence du capteur DHT (DHT11 ou DHT21 ou DHT22)
DHT dht(DHTPIN, DHTTYPE);             // Initialisation du capteur DHT

// Préparation de l'afficheur
LiquidCrystal_I2C lcd(0x27,16,2);     // Initialisation en présisant l'adresse 0x27 ou 0x3f sur le bus I2C et 16 caractères par 2 lignes

// Initialisation du wifi
ESP8266WiFiMulti WiFiMulti;  

const int ROUGE = 14;
const int ORANGE = 12;
const int VERT = 13;

// fonction de démarrage
void setup() {
  // Démarrage du bus série
  Serial.begin(115200);               // vitesse
  Serial.println("DHT11 LCD LED");          // écriture d'un petit message...
  Serial.println("vers VPS php");
  
  // Démarrage du capteur DHT11
  dht.begin();

  // Démarrage de l'afficheur
  lcd.init();       
  lcd.backlight();                    // mise en route du rétro éclairage
  lcd.setCursor(0,0);                 // positionne le curseur sur la 1ère ligne et 1ère colonne
  lcd.print("DHT11 LCD LED");               // écriture d'un petit message...
  lcd.setCursor(0,1);                 // positionne le curseur sur la 2ème ligne et 1ère colonne
  lcd.print("vers VPS php");          // écriture d'un petit message...

  // Connexion au réseau wifi
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP( "****", "******");    

  // Controle des LEDs
  pinMode(ROUGE, OUTPUT);
  pinMode(ORANGE, OUTPUT);
  pinMode(VERT, OUTPUT);
  
}

// boucle infinie
void loop() {
  delay(10000);                        // attendre 10 seconde 
  
  float t = dht.readTemperature();    // mesurer la température (en ° Celsius)
  float h = dht.readHumidity();       // mesurer l'humidité (en %)
  
  if (isnan(h) || isnan(t)) {         // test si des valeurs ont été récupérées    
    // s'il y a un problème...    
    Serial.println("Failed to read from DHT sensor!");    // affiche un message d'erreur
    return;                            // quitte pour retenter une lecture  
    
  } else {
    // si tout va bien...
    
    // conversion des valeurs en entier
    int temp = int(t);
    int humi = int(h);
    
    // affichage des valeurs dans le bus série
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print(" *C\t");
    Serial.print("Humidité : ");
    Serial.print(h);
    Serial.println(" %");
  
    // des valeurs sur l'afficheur
    lcd.clear();
    lcd.setCursor(0,0);                 // 1ère colonne, 1ère ligne
    lcd.print("Temp. : ");
    lcd.setCursor(8,0);                 // 9ème colonne, 1ère ligne
    lcd.print(int(t));                  // conversion en entier avant d'afficher
    lcd.setCursor(11,0);                // 12ème colonne, 1ère ligne
    lcd.print("*C");
    lcd.setCursor(0,1);                 // 1ère colonne, 2èmee ligne
    lcd.print("Humi. : ");
    lcd.setCursor(8,1);                 // 9ème colonne, 2èmee ligne
    lcd.print(int(h));;                 // conversion en entier avant d'afficher
    lcd.setCursor(11,1);                // 12ème colonne, 2èmee ligne
    lcd.print("%");
  
    // Tout éteindre
    digitalWrite(ROUGE, LOW);
    digitalWrite(ORANGE, LOW);
    digitalWrite(VERT, LOW);
  
    // allumer les leds
    if (t <= 20) digitalWrite(VERT, HIGH);
    if (t > 20 && t < 26) digitalWrite(ORANGE, HIGH); 
    if (t >= 26) digitalWrite(ROUGE, HIGH);

    // envoi des données vers internet
    if((WiFiMulti.run() == WL_CONNECTED)) {   // Si le wifi est connecté
      
          Serial.println("Connexion wifi ok");
          
          HTTPClient http;                    // préparation d'une requète HTTP au serveur

          Serial.print("HTTP begin... ");
          http.begin("http://51.75.126.49/dht11/store_data.php");   // connexion au serveur
          
          http.addHeader("Content-Type", "application/json");    // envoi d'un entête pour préciser notre format de données : json
          
          String data = "{\"temperature\":\"" + String(temp) + "\",\"humidite\":\"" + String(humi) + "\"}";    // préparation du json {"temperature":"22","humidite":"55"}
          Serial.println(data);
          
          int httpCode = http.POST(data);    // envoi les données et récupère un code de retour
          
          if(httpCode == 200 ) {              // si le code de retour est bon (200)
             Serial.println("POST : ok");
          } else {                            // si le code de retour n'est pas bon (différent de 200)
             Serial.print("POST : failed, error : ");
             Serial.println(http.errorToString(httpCode).c_str());
          }
          http.end();                    // on ferme la connexion au serveur
    } else  {
        Serial.println("Echec connexion wifi");
    }
    
  }

  
}
