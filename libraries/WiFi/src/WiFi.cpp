#include "WiFi.h"

WiFiClass WiFi;

String WiFiClass::firmwareVersion() {
#if defined(ARDUINO_PORTENTA_C33)
	return "v1.5.0";
#else
	return "v0.0.0";
#endif
}
