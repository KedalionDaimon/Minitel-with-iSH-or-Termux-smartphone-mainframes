// LOLIN(WEMOS) D1 R32 & mini (ESP8266)
// ESP32 Wrover
// telnet 192.168.4.1 23
// or
// nc -C 192.168.4.1 23
#pragma GCC optimize ("-O3")
#pragma GCC push_options
#define SERIAL_BUFFER_SIZE 2048

#define SRL Serial
#include <WiFi.h>
const char* ssid = "Minitel";
const char* password = "MinitelTerminal";
WiFiServer wifiServer(8088);
WiFiClient client = wifiServer.available();
unsigned char ch;
void setup() {
  // SRL.begin(300, SERIAL_8N1); // NORMAL
  SRL.begin(1200, SERIAL_7E1); // MINITEL
  Serial.setRxBufferSize(SERIAL_BUFFER_SIZE);
  WiFi.softAP(ssid, password); delay(500);
  SRL.println("WiFi");
  wifiServer.begin();
  SRL.println("Net:Minitel Password:MinitelTerminal");
  SRL.print("IP:"); SRL.print(WiFi.softAPIP()); SRL.println(" Port:8088");
  SRL.println("Fnct-T A: ASCII. Fnct-T E toggle echo.");
  SRL.println("busybox nc 192.168.4.1 8088 -e stdbuf");
  SRL.println("-e0 -o0 sh -c 'socat TCP-LISTEN:8086");
  SRL.println(",reuseaddr,fork EXEC:sh,pty,stderr");
  SRL.println(",setsid,sigint,sane & sleep 1 &&");
  SRL.println("nc localhost 8086'");
  SRL.println("Set backspace to Ctrl-H with:");
  SRL.println("stty erase Ctrl-V,Ctrl-H");
    for (;;) {
    client = wifiServer.available();
    if (client) {
        while (client.connected()) {
        if (client.available()) {
          ch = client.read();
          if ((ch != '\r') && (ch != '\n')) {
            SRL.write((unsigned char) ch);
          } else if (ch == '\n') { // i.e. ignore CR and turn LF into CRLF
            SRL.write((unsigned char) '\r'); SRL.write((unsigned char) '\n');
          }
          Serial.flush();
        }
        if (Serial.available()) {
          ch = SRL.read();

          // Flip case... but pretty hopeless here.
          /*
          if ((ch > 96) && (ch < 123)) {
            ch = ch - 32;
          } else if ((ch > 64) && (ch < 91)) {
            ch = ch + 32;
          }
          */

          if ((ch != '\n') && (ch != '\r') && (ch != '@')) {
            client.write((unsigned char) ch);
          } else if (ch == '\r') { // MINITEL == \r
            client.write((unsigned char) '\n');
            SRL.write('\r'); SRL.write('\n');
          } else if (ch == '\n') { // MINITEL == \n
            SRL.write(12); // MINITEL; NORMALLY COMMENT OUT
          }
        }
}}}}
void loop() {}
