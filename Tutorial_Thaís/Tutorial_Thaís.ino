#include <ESP8266WiFi.h>

// ----------------------------------------------------------------------------------------------
// Nome da rede e Senha da mesma.
// Caso a rede não tenha senha, deixe "".
char ssid[] = "brisa-1793225";
char pass[] = "74slqtvz";
String _ID = "AKfycby46UO39-bGC4J02msPFeuqOGYLikBlteWawpKJhmQ_YGhe7j6_F6jD-tifua4scHVv4w"; //--> spreadsheet script ID
// ----------------------------------------------------------------------------------------------
//Não modifique.
const char* host = "script.google.com"; 
// ----------------------------------------------------------------------------------------------
//Definir o intervalo de envio dos dados.
#define UPDATE_INTERVAL_HOUR  (0)
#define UPDATE_INTERVAL_MIN   (0)
#define UPDATE_INTERVAL_SEC   (5)

#define UPDATE_INTERVAL_MS    ( ((UPDATE_INTERVAL_HOUR*60*60) + (UPDATE_INTERVAL_MIN * 60) + UPDATE_INTERVAL_SEC ) * 1000 )

// ----------------------------------------------------------------------------------------------
//Defina as variáveis globais aqui para melhor organização.

const int trigPin = 12;
const int echoPin = 14;

#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;

// ----------------------------------------------------------------------------------------------
void update_google_sheet()
{
    Serial.print("connecting to ");
    Serial.println(host);
  
    // Use WiFiClient class to create TCP connections
    WiFiClientSecure client;
    const int httpPort = 443; // 80 is for HTTP / 443 is for HTTPS!
    
    client.setInsecure(); // this is the magical line that makes everything work
    
    if (!client.connect(host, httpPort)) { //works!
      Serial.println("connection failed");
      return;
    }
       
// ----------------------------------------------------------------------------------------------
//Processando os dados e enviando para a planilha.
    String url = "/macros/s/" + _ID + "/exec?distanceCm=";   
    url += String(distanceCm);

    //Caso você tenha mais dados, basta: 
  
    // url += "&Variável="; nomeDaVariávelContidaNoScriptdaSheet
    // url += String(Variável);
    
    Serial.print("Requesting URL: ");
    Serial.println(url);
  
    // Faz requisição ao servidor.
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
  
    Serial.println();
    Serial.println("closing connection");  
}

// ----------------------------------------------------------------------------------------------
void setup()
{
  // Monitor Serial
  Serial.begin(115200);

  // Declare os pinos
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input


// ----------------------------------------------------------------------------------------------
//Conexão com a rede.
  Serial.print("Connecting");
  WiFi.begin(ssid, pass); //--> Connect to your WiFi router
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, LOW); //Usando o led da esp para explicitar a conexão
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
  }

}

// ----------------------------------------------------------------------------------------------
//Variáveis que definem o tempo.
unsigned long time_ms;
unsigned long time_1000_ms_buf;
unsigned long time_sheet_update_buf;
unsigned long time_dif;

void loop()
{
  time_ms = millis();
  time_dif = time_ms - time_1000_ms_buf;

  // Lê e printa o dado no monitor serial a cada segundo
  if ( time_dif >= 1000 ) // 1sec 
  {
    time_1000_ms_buf = time_ms;
// ----------------------------------------------------------------------------------------------
//Aqui você colocará o seu código.    
      // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculate the distance
    distanceCm = duration * SOUND_VELOCITY/2;
    
    // Prints the distance on the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(distanceCm);                          /*Demo values that has to be sent to google sheets, you can use sensor values*/
//Até aqui.
// ---------------------------------------------------------------------------------------------- 
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  // Update data to google sheet in specific period
  time_ms = millis();
  time_dif = time_ms - time_sheet_update_buf;  
  if ( time_dif >= UPDATE_INTERVAL_MS ) // Specific period
  {
    time_sheet_update_buf = time_ms;
    update_google_sheet();
  }
 
}