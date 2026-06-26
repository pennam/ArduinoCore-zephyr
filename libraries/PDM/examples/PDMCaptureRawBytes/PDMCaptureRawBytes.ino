/*
  Nano 33 BLE Microphone to Serial Streamer

  Circuit:
  - Arduino Nano 33 BLE board

  How to use this example:

  1. Upload this sketch to the board. Microphone acquisition starts
     immediately and raw audio samples are streamed over the serial port.

  2. Make sure no serial monitor or other program is keeping the serial
     port open.

  3. Run the host script provided in the library extras folder:
       extras/PDMSerialAudioRecorder/PDMSerialAudioRecorder.py
     It reads the samples streamed by this sketch and packs them into a
     WAV file you can play back on your PC.

  Notes:

  - The host script uses /dev/ttyACM0 as the default serial device; change
    it if your board enumerates as a different device.
  - The host script records about 5 seconds of audio.
*/

#include <PDM.h>
// default number of output channels
static const char channels = 1;
// default PCM output frequency
static const int frequency = 16000;

// Buffer to read samples into
// *** For better performance set the user buffer dimension to the dimension
// of the buffer used by PDM library this way ***
short sampleBuffer[PDM_NUMBER_OF_SAMPLES];

// Number of bytes read
volatile int samplesRead;

void setup() {
	Serial.begin(115200);
	while (!Serial)
		;

	// Configure the data receive callback
	PDM.onReceive(onPDMdata);

	// Initialize PDM:
	// - one channel (Mono)
	// - 16 kHz sample rate (Standard for voice)
	if (!PDM.begin(channels, frequency)) {
		Serial.println("Failed to start PDM!");
		while (1)
			;
	}
}

void loop() {
	// Wait for samples to be read
	if (samplesRead) {

		// Write the raw bytes to the Serial port
		// We send byte-by-byte to keep it fast and binary
		Serial.write((uint8_t *)sampleBuffer, samplesRead * 2);
		// Clear the read count
		samplesRead = 0;
	}
}

// Callback function: Handling the PDM on receive event
// The PDM library will call this callback function as soon as the internal
// buffer is ready and can be read
// It is user responsibility to read from PDM as fast as possible otherwise
// data will be lost
void onPDMdata() {
	// Read into the sample buffer
	int bytesRead = PDM.read(sampleBuffer, sizeof(sampleBuffer));

	// 16-bit, 2 bytes per sample
	samplesRead = bytesRead / 2;
}
