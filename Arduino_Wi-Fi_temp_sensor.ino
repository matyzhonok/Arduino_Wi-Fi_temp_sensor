/*
    Данный скетч отображает состояние подлюченных датчиков и позволяет управлять светодиодом.
*/

#include <ESP8266WiFi.h>


#ifndef STASSID
#define STASSID "robot"
#define STAPSK  "SZCP%ca2kQxeqfWk3x2M"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;


// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

/* 
 * МОИ ПЕРЕМЕННЫЕ 
 */

// IP-адрес сервера (платы)
String MyIP;
// Состояние светодиода
boolean LED_Status;


// ------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  delay(500);

  // prepare LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);

  // Подключаемся к wi-fi сети
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Получаем ip-адрес от DHCP");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  
  // Запускаем web-сервер и запоминаем его IP-адрес
  server.begin();
  MyIP = WiFi.localIP().toString();
  Serial.print(F("Web-сервер запущен по адресу: "));
  Serial.println(MyIP);
}


/* ------------------------------------------------------------------------------------------*/

void loop() {
  // Получаем время от начала работы
  String timeAfterStartStr = millis_ToString(millis());


  
  // Ждём подключения клиента
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F(""));
  Serial.println(F("Новый запрос"));
  
  client.setTimeout(5000); // default is 1000

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.print("Запрос от клиента: ");
  Serial.println(req);

  // Match the request
  LED_Status = !digitalRead(LED_BUILTIN);
  Serial.print("Статус светодиода: ");
  Serial.println(LED_Status);

  if (req.indexOf(F("LED=OFF")) != -1) {
    SensorLedOff();
  } else if (req.indexOf(F("LED=ON")) != -1) {
    SensorLedOn();
  } else if (req.indexOf(F("favicon.ico")) != -1) {
    client.stop();
    Serial.println(F("Favicon!"));
  } else {  
    Serial.println(F("invalid request"));
  }

  // Set LED according to the request
  digitalWrite(LED_BUILTIN, !LED_Status);

  // read/ignore the rest of the request
  // do not client.flush(): it is for output only, see below
  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }

  if (req.indexOf(F("LED=status&API=yes")) != -1){
    String str;
    if (LED_Status) {
      str = "On";
    } else {
      str = "Off";
    }
    print_to_web(str, client, false);
  } else {
    String str;
    // Send the response to the client
    // it is OK for multiple small client.print/write,
    // because nagle algorithm will group them into one single packet
    /*if (LED_Status){
      str = "{\"sensor\": {\r\n\"name\":\"LED\",\r\n\"status\":\"ON\"\r\n}\r\n}";
    } else {
      str = "{\"sensor\": {\r\n\"name\":\"LED\",\r\n\"status\":\"OFF\"\r\n}\r\n}";
    };*/
    if (LED_Status){
      str = "Светодиод ВКЛЮЧЁН. (<a href='http://" + MyIP + "/LED=OFF'>выключить</a>)<br />";
    } else {
      str = "Светодиод ВЫКЛЮЧЁН. (<a href='http://" + MyIP + "/LED=ON'>включить</a>)<br />";
    };
    str = str + "<p align=\"right\">Времени с начала работы: " + timeAfterStartStr + "</p>";

    print_to_web(str, client, true);
  }

  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client"));
  Serial.println(F("-------------------------"));
}
