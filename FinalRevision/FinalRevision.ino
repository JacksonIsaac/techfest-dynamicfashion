
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <adafruit_feather.h>
#include <adafruit_mqtt.h>

#include <ArduinoJson.h>
#include "certificates.h"


#define PIN PC7
#define WLAN_SSID         "jibran"
#define WLAN_PASS         "jibran123"
#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

AdafruitMQTT mqtt;

// ======================================================
#define AWS_IOT_MQTT_HOST              "aibtvt9q42xda.iot.eu-west-1.amazonaws.com"
#define AWS_IOT_MQTT_PORT              8883
#define AWS_IOT_MQTT_CLIENT_ID         "Adafruit_WiFI"
#define AWS_IOT_MY_THING_NAME          "Adafruit_WiFI"
// ======================================================

#define AWS_IOT_MQTT_TOPIC             "$aws/things/" AWS_IOT_MY_THING_NAME "/#"
#define AWS_IOT_MQTT_TOPIC_HEARTBEAT   "$aws/things/" AWS_IOT_MY_THING_NAME "/heartbeat"
#define MQTT_CONN_KEEPALIVE             10000
#define TOPIC             "adafruit/feather"
#define PUBLISH_MESSAGE   "Hello from Adafruit WICED Feather"
#define WILL_MESSAGE      "Goodbye!!"

//=========================================================

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);
const uint16_t colors[] = {
  matrix.Color(255, 0, 0), //0 = Red
  matrix.Color(0, 255, 0), //1 = Green
  matrix.Color(0, 0, 255), //2 = Blue 
  matrix.Color(255,255,255), //3 = White 
  matrix.Color(255, 255, 0), //4 = Yellow
  matrix.Color(0, 255, 255), //5 = Cyan
  matrix.Color(255, 0, 255), //6 = Mergenta
  matrix.Color(192, 192, 192), //7 = Silver
  matrix.Color(128, 0, 0), //8= Maroon
  matrix.Color(128, 128, 0), //9 = Olive
  matrix.Color(0, 128, 0), //10 = Green
  matrix.Color(128, 0, 128), //11 = Purple
  matrix.Color(0, 128, 128), //12 = Teal
  matrix.Color(0, 0, 128), //13 = Navy
  matrix.Color(255, 69, 0), //14 = Orange
  0 //15 = Black or nothing
  };
int x    = matrix.width();
int pass = 15;
const char* yourText = "Adidas + Osram";

int Foreground = 1; 
int Background = 15; 
int pixelPerChar = 6;
int maxDisplacement = strlen(yourText) * pixelPerChar + matrix.width();

const char aws_private_key[] = 
"-----BEGIN RSA PRIVATE KEY-----\n"
"MIIEpAIBAAKCAQEAiqY14zlVhqlzzw4NEIvh/Wgjg20xgLmCtyMTugbaxJF55hOE\n"
"P7TZlP90J0FddpAUaiWl3sXQZJXvjgiLEbKwIguKHKfilxIdn25/+0JS44K7cQNa\n"
"PuzdXfs/omCxqcxAbNsCPs/yKebrb02Zoj5DcB9EuO8rcLRU1SFe8gtG1EcS6RjV\n"
"kJ3iomrOUz+MgivNIUVe3/8IcQGHkwIZ4A/duZKAxuiu3Z9jPcg8qLZTOstUcbj1\n"
"tguHDyeVfhdOyV1HvfbZZlsAR2cSf6nZU3Audz858MO6RWKHbDxocBQvD+AjXvWC\n"
"AscsQBfjLTG8HyKMFnpw+w6/ZcL5Mv2S9zZmOQIDAQABAoIBAHWKOK7cITRbHXGQ\n"
"IOVBISHaEhlODaRqIHWvR/7x5q60AZpaO0hiwf59nfPVhy2HMbdXGUpFesh7D63i\n"
"cjtbt9P04m4uqXqXAgPyAo94h5qWbt99Dxm/Zl9RK3416Qfgv1FJTZsAYPTC7h7a\n"
"s38Gt9cquvDo2XPffPd+JeaeRrqYEX4vB5d/aiLg0oeYsEkWnt2Bt0f2fAVfkHpR\n"
"2gccoivUSl2ULDQ01IqIr2AdLe+RZvXrVn5MEvha58Ua/QGeIW3I8tmtDMFRAahc\n"
"sd69uGz32+AgxUAzukD3nr+9ra79x+nIummbYdJikd6TdHmmBu9GjLmGWPjt+X+l\n"
"JCzprT0CgYEAzzrB86Eun7KTUWFnkFqRotMrUZqFXclWiMxk5Y0kPtEP2VDZx07O\n"
"TXuFjJ+jCxr8w9p8ZHpOzC5F5HimFshWZZ3SFVVFriW+rK+AxXcvQxWpTgjyJCUB\n"
"H5r/9iiFheLNXTfrhSN6c/gkChaHYmmMDIkIHMTKPLpraO1TEZfrLBsCgYEAq0ea\n"
"dwZRBdGdhpdhIlcAkA1S5x3LccTN76rswBgv+r9AMjkcCGxG85TslW1Ee5Dg7u6n\n"
"HA5mOTl/bBShYffhRAV9qNr0+QHjgpwRkWVBs7ZUU6mAQQDyXxBnYxKIyeWfvP+8\n"
"ox33LiXAlNahanQKdAuw7epwKWAIp4wltQ6FdDsCgYEAxrLJO4hGAzwfwz1JJwGg\n"
"eOZPt+wYiqe9wGdzKyOdKFoe/zllx5RJfcO9duuuUryqtkRtV45qtZ58x6fjYR7w\n"
"sHiV8nyT1no8jKmvKCeyenlvn/I18vBEVoCxgt6xwIGKdiJjQoKVLFM2gzG82GDp\n"
"ty9BRwxt+bKpwK8mIk1vYhcCgYAiZg0FnoUjMAf9AeYGKYb5Z1OhCwDS5n2dhqxp\n"
"n/Hy3/oanBGfN2/Ad8ZT9E/uqRgjpFZ3cgThtUR8BmCq9keNi7TC9saAQ/rXPlTg\n"
"6H8bgqDI9J/HblM++GJrKUvDkJRWNt5BvEDqSba1ra1ZXdjDmZ19Crpf9OOIEsN3\n"
"62SuBQKBgQCgwseL69p7A8YEIkdl8WfSaIhEuD6hk0JGfcqflpi9ioVkZjbSOoML\n"
"svKG9GHwkm4twbpWN2XHPmN1cZ0phMdO1orJZntJVbmTpB8y3pH8dn1FeJaoaNaY\n"
"t00U/YbEBJiJXNsSCL9EJUmfr2YFOeehH+AeP+fiUtA57ZeoCHqvng==\n"
"-----END RSA PRIVATE KEY-----";









void setup()
{
  
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
 
  
  Serial.begin(115200);

  // Wait for the USB serial port to connect. Needed for native USB port only
  // while (!Serial) delay(1);

  Serial.println("AWS IOT Example\r\n");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Tell the MQTT client to auto print error codes and halt on errors
  mqtt.err_actions(true, true);

  // Set ClientID
  mqtt.clientID(AWS_IOT_MQTT_CLIENT_ID);

  // Set the disconnect callback handler
  mqtt.setDisconnectCallback(disconnect_callback);

  // default RootCA include certificate to verify AWS (
  Feather.useDefaultRootCA(true);

  // Setting Indentity with AWS Private Key & Certificate
  mqtt.tlsSetIdentity(aws_private_key, local_cert, LOCAL_CERT_LEN);

  // Connect with SSL/TLS
  //Serial.printf("Connecting to " AWS_IOT_MQTT_HOST " port %d ... ", AWS_IOT_MQTT_PORT);
  mqtt.connectSSL(AWS_IOT_MQTT_HOST, AWS_IOT_MQTT_PORT);
 // Serial.println("OK");

 // Serial.print("Subscribing to " AWS_IOT_MQTT_TOPIC " ... ");
  mqtt.subscribe(AWS_IOT_MQTT_TOPIC, MQTT_QOS_AT_MOST_ONCE, subscribed_callback); // Will halted if an error occurs
 // Serial.println("OK");
  

}

void init_1(void){
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
//  matrix.fillScreen(0);
  //  matrix.setPassThruColor(colors[Background]);
 matrix.clear();
  }

void loop()
{
 

  matrix.fillScreen(colors[Background]);
  matrix.setCursor(x, 0);
  
  matrix.print(F(yourText));

  
  if(--x < -maxDisplacement) {
    x = matrix.width();
   
    matrix.setTextColor(colors[Foreground]);
  
  }
  matrix.show();
  delay(100);

}



void subscribed_callback(UTF8String topic, UTF8String message)
{
Serial.print(topic);
//const int stringSize = message.len+1;
StaticJsonBuffer<1000> jsonBuffer;
JsonObject& root = jsonBuffer.parseObject(message.data);
int codeerr = root["code"];
if (codeerr != 400 ){

yourText = root["message"];
Foreground = root["foreground"];
Background = root["background"];
init_1();
maxDisplacement = strlen(yourText) * pixelPerChar + matrix.width();

Serial.print(yourText);


}
else {
  
  yourText = yourText;
  maxDisplacement = strlen(yourText) * pixelPerChar + matrix.width();
Foreground = Foreground;
Background = Background;
init_1();

}

}

void disconnect_callback(void)
{
  Serial.println();
  Serial.println("-----------------------------");
  Serial.println("DISCONNECTED FROM MQTT BROKER");
  Serial.println("-----------------------------");
  Serial.println();
   mqtt.connectSSL(AWS_IOT_MQTT_HOST, AWS_IOT_MQTT_PORT);
   yourText = yourText;
  maxDisplacement = strlen(yourText) * pixelPerChar + matrix.width();
  Foreground = Foreground;
  Background = Background;
}

bool connectAP(void)
{
  // Attempt to connect to an AP
  Serial.print("Please wait while connecting to: '" WLAN_SSID "' ... ");
  
  if ( Feather.connect(WLAN_SSID, WLAN_PASS) )

  
  {
    Serial.println("Connected!");
  }
  else
  {
//     Serial.printf("Failed! %s (%d)", Feather.errstr(), Feather.errno());
    Serial.println();
  }
  Serial.println();

  return Feather.connected();
}
