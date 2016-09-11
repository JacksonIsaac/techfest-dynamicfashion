/*********************************************************************
 This is an example for our Feather WIFI modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

/* This example demonstrates how to use  the ping() function
 * to send a PING requesr to a remote host
 */

#include "adafruit_feather.h"

#define WLAN_SSID            "jibran"
#define WLAN_PASS            "jibran123"

// Ping target by hostname
const char target_hostname[] = "adafruit.com";

// Ping target by IP String
const char target_ip_str[] = "8.8.8.8";

// Ping target by IPAddress object
IPAddress target_ip(8, 8, 4, 4);

/**************************************************************************/
/*!
    @brief  Network disconnect callback
*/
/**************************************************************************/
void disconnect_callback(void)
{
  Serial.println("disconnect_callback(): Disconnected");
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  // wait for serial port to connect. Needed for native USB port only
  while (!Serial) delay(1);

  Serial.println(F("Ping Example\r\n"));

  // Print all software versions
  Feather.printVersions();

  // Set disconnection callback
  Feather.setDisconnectCallback(disconnect_callback);

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  if ( Feather.connected() )
  {
    uint8_t resp_ms;

    // Ping hostname
    Serial.printf("Pinging %s : ", target_hostname);
    resp_ms = Feather.ping(target_hostname);
    if (resp_ms != 0)
    {
      Serial.printf("time = %d ms", resp_ms);
    }else
    {
      Serial.printf("error = 0x%04X", Feather.errno());
    }
    Serial.println();

    // Ping IP Address in String format
    Serial.printf("Pinging %s      : ", target_ip_str);
    resp_ms = Feather.ping(target_ip_str);
    if (resp_ms != 0)
    {
      Serial.printf("time = %d ms", resp_ms);
    }else
    {
      Serial.printf("error = 0x%04X", Feather.errno());
    }
    Serial.println();

    // Ping specific IP Address
    Serial.printf("Pinging %s      : ", target_ip.toCharArray());
    resp_ms = Feather.ping(target_ip);
    if (resp_ms != 0)
    {
      Serial.printf("time = %d ms", resp_ms);
    }else
    {
      Serial.printf("error = 0x%04X", Feather.errno());
    }
    Serial.println();

    Serial.println();
    Serial.println("Try again in 10 seconds");
    Serial.println();
    delay(10000);
  }
}

/**************************************************************************/
/*!
    @brief  Connect to defined Access Point
*/
/**************************************************************************/
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
    Serial.printf("Failed! %s (%d)", Feather.errstr(), Feather.errno());
    Serial.println();
  }
  Serial.println();

  return Feather.connected();
}
