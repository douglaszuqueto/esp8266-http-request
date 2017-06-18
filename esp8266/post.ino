// ############# LIBRARIES ############### //

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// ############# VARIABLES ############### //

const char* SSID = "VIVO-8594"; // rede wifi
const char* PASSWORD = "C6620E8594"; // senha da rede wifi

String BASE_URL = "http://192.168.15.3:3000/";

// ############# PROTOTYPES ############### //

void initSerial();
void initWiFi();
void httpRequest(String path);

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
  Serial.println("[POST] /sensors - sending request...");
  Serial.println("");

  httpRequest("sensors");

  Serial.println("");
  delay(1000);

}

// ############# HTTP REQUEST ################ //

void httpRequest(String path)
{
  String payload = makeRequest(path);

  if (!payload) {
    return;
  }

  Serial.println("##[RESULT]## ==> " + payload);

}

String makeRequest(String path)
{
  http.begin(BASE_URL + path);
  http.addHeader("content-type", "application/x-www-form-urlencoded");

  String body = "id=7890&name=NTC&type=temperature&value=10";

  int httpCode = http.POST(body);

  if (httpCode < 0) {
    Serial.println("request error - " + httpCode);
    return "";

  }

  if (httpCode != HTTP_CODE_OK) {
    return "";
  }

  String response =  http.getString();
  http.end();

  return response;
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
