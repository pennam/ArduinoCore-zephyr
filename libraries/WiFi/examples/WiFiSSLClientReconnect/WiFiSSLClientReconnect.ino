/*
  SSL client reconnection test

  This sketch connects to TLS servers several times in a row, using the
  builtin CA bundle, a custom CA certificate, both interleaved, and two
  clients open at the same time. Every connection must succeed.

  Regression test for https://github.com/arduino/ArduinoCore-zephyr/issues/708

  The builtin CA tests require the CA bundle to be present in the
  filesystem (/wlan:/cacert.pem, see the FlashFormat sketch).
*/

#include <WiFi.h>
#include <ZephyrSSLClient.h>
#include <errno.h>
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

const char builtin_ca_host[] = "www.google.com";
const char custom_ca_host[] = "www.howsmyssl.com";
const int port = 443;
const int iterations = 3;

ZephyrSSLClient client;
ZephyrSSLClient client2;

int passed = 0;
int failed = 0;

// ISRG Root X1 (Let's Encrypt root CA certificate)
const char isrg_root_x1[] =
"-----BEGIN CERTIFICATE-----\n"
"MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n"
"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n"
"WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n"
"ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n"
"MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n"
"h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n"
"0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n"
"A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n"
"T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n"
"B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n"
"B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n"
"KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n"
"OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n"
"jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n"
"qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n"
"rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n"
"HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n"
"hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n"
"ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n"
"3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n"
"NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n"
"ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n"
"TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n"
"jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n"
"oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n"
"4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n"
"mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n"
"emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n"
"-----END CERTIFICATE-----\n";

bool request(ZephyrSSLClient &c, const char *host) {
  c.print("HEAD / HTTP/1.1\r\nHost: ");
  c.print(host);
  c.print("\r\nConnection: close\r\n\r\n");

  String line;
  unsigned long start = millis();
  while (millis() - start < 10000) {
    if (c.available()) {
      char ch = c.read();
      if (ch == '\n') {
        break;
      }
      line += ch;
    } else if (!c.connected()) {
      break;
    }
  }
  line.trim();
  Serial.print("  -> ");
  Serial.println(line.length() ? line : String("<no response>"));
  return line.startsWith("HTTP/1.");
}

bool openClient(ZephyrSSLClient &c, const char *host, const char *cert) {
  errno = 0;
  unsigned long start = millis();
  int ok = cert ? c.connect(host, port, cert) : c.connect(host, port);
  Serial.print("  connect(");
  Serial.print(host);
  Serial.print(cert ? ", custom CA" : ", builtin CA");
  Serial.print(") = ");
  Serial.print(ok);
  Serial.print(" errno=");
  Serial.print(errno);
  Serial.print(" in ");
  Serial.print(millis() - start);
  Serial.println(" ms");
  return ok;
}

void check(const char *name, bool ok) {
  Serial.print(ok ? "PASS: " : "FAIL: ");
  Serial.println(name);
  ok ? passed++ : failed++;
}

bool runOnce(const char *host, const char *cert) {
  bool ok = openClient(client, host, cert) && request(client, host);
  client.stop();
  return ok;
}

void testRepeated(const char *name, const char *host, const char *cert) {
  for (int i = 1; i <= iterations; i++) {
    String label = String(name) + " #" + i;
    Serial.println(label);
    check(label.c_str(), runOnce(host, cert));
  }
}

void testTwoClients() {
  const char *name = "two clients open at the same time";
  Serial.println(name);
  bool ok1 = openClient(client, custom_ca_host, isrg_root_x1);
  bool ok2 = openClient(client2, builtin_ca_host, nullptr);
  bool ok = ok1 && ok2 && request(client, custom_ca_host) && request(client2, builtin_ca_host);
  client.stop();
  client2.stop();
  check(name, ok);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("Communication with WiFi module failed!");
    while (true)
      ;
  }

  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(3000);
  }
  Serial.print("Connected, IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  testRepeated("builtin CA", builtin_ca_host, nullptr);
  testRepeated("custom CA", custom_ca_host, isrg_root_x1);

  // Interleave: the builtin CA must still be used after a custom CA was set
  // and the custom CA must be re-registered after a builtin-only connection
  for (int i = 1; i <= iterations; i++) {
    String label = String("interleaved #") + i;
    Serial.println(label);
    bool ok = runOnce(builtin_ca_host, nullptr) && runOnce(custom_ca_host, isrg_root_x1);
    check(label.c_str(), ok);
  }

  testTwoClients();

  Serial.println();
  Serial.print("Passed: ");
  Serial.print(passed);
  Serial.print(", Failed: ");
  Serial.println(failed);
  Serial.println(failed ? "TEST FAILED" : "TEST PASSED");
}

void loop() {
}
