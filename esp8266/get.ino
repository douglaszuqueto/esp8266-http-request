// ############# LIBRARIES ############### //

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// ############# VARIABLES ############### //

const char* SSID = ""; // ssid
const char* PASSWORD = "C"; // wifi password

String BASE_URL = "http://192.168.15.3:3000/";

// ############# PROTOTYPES ############### //

void initSerial();
void initWiFi();
void httpGet();

// ############### OBJECTS ################# //

WiFiClient client;
HTTPClient http;

// ############## SETUP ################# //

void setup() {
  initSerial();
  initWiFi();
}

// ############### LOOP ################# //

void loop() {
  Serial.println("[GET] /temperatures - sending request...");
  Serial.println("");

  httpGet(BASE_URL + "temperatures");

  Serial.println("");
  delay(1000);

}

// ############# HTTP REQUEST ################ //

void httpGet(String url)
{
  String payload = makeRequest(url);

  if (!payload) {
    return;
  }

  Serial.println("##[RESULT]## ==> " + payload);

}

String makeRequest(String url)
{
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode < 0) {
    Serial.println("request error - " + httpCode);
    return "";

  }

  if (httpCode != HTTP_CODE_OK) {
    return "";
  }

  return http.getString();

  http.end();
}

// ###################################### //

// implementacao dos prototypes

void initSerial() {
  Serial.begin(115200);
}

void initWiFi() {
  delay(10);
  Serial.println("Conectando-se em: " + String(SSID));

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado na Rede " + String(SSID) + " | IP => ");
  Serial.println(WiFi.localIP());
}
