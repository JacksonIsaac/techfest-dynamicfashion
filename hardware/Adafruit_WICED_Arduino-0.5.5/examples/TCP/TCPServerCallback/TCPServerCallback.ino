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

/* This example will start a TCP server on the feather, registering
 * a 'receive' callback and echoing back any incoming messages. To
 * run this demo:
 * - Change SSID/Pass
 * - Compile and run
 * - Use a TCP client on your PC such as netcast as follows:
 *  'echo "your message" | nc IP port'. e.g your Feather's IP is 192.168.1.100
 *   and PORT is 8888 then
 *     > echo "Hello Feather" | nc 192.168.100 8888
 */

#include <adafruit_feather.h>

#define WLAN_SSID            "yourSSID"
#define WLAN_PASS            "yourPass"

#define PORT                 8888                     // The TCP port to use

AdafruitTCPServer tcpserver(PORT);

/**************************************************************************/
/*!
    @brief  This callback is fired when there is a connection request from
            a TCP client. Use accept() to establish the connection and
            retrieve the client 'AdafruitTCP' instance.
*/
/**************************************************************************/
void connect_request_callback(void)
{
  uint8_t buffer[256];
  uint16_t len;

  AdafruitTCP client = tcpserver.available();

  if ( client )
  {
    // read data
    len = client.read(buffer, 256);

    // Print data along with peer's info
    Serial.print("[RX] from ");
    Serial.print(client.remoteIP());
    Serial.printf(" port %d : ", client.remotePort());
    Serial.write(buffer, len);
    Serial.println();

    // Echo back
    client.write(buffer, len);

    // call stop() to free memory by Client
    client.stop();
  }
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when the board comes out of reset
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the serial port to connect. Needed for native USB port only.
  while (!Serial) delay(1);

  Serial.println("TCP Server Example (Callbacks)\r\n");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  // Tell the TCP Server to auto print error codes and halt on errors
  tcpserver.err_actions(true, true);

  // Setup callbacks: must be done before begin()
  tcpserver.setConnectCallback(connect_request_callback);

  // Starting server at defined port
  tcpserver.begin();

  Serial.print("Listening on port "); Serial.println(PORT);
}

/**************************************************************************/
/*!
    @brief  This loop function runs over and over again
*/
/**************************************************************************/
void loop()
{

}

/**************************************************************************/
/*!
    @brief  Connect to the pre-defined access point
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
