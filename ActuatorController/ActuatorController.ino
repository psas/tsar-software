#include <stdint.h>

// Constants
const int RECEIVE_PAYLOAD_LENGTH = 5; // The length in bytes of the actuator commands
const int RECEIVE_PACKET_LENGTH = 7 + RECEIVE_PAYLOAD_LENGTH; // The length in bytes of the preamble, payload, and checksum
const int SEND_PAYLOAD_LENGTH = 6;
const int SEND_PACKET_LENGTH = SEND_PAYLOAD_LENGTH + 10;
const uint8_t MAXIMUM_LIFETIME_CHECKSUM_ERRORS = 25;


enum FailureCause {
	FC_NO_FAILURE = 0,
	FC_TOO_LONG_SINCE_LAST_HEARTBEAT = 1,
	FC_TOO_MANY_LIFETIME_CHECKSUM_ERRORS = 2,
	FC_PANIC_COMMAND = 3,
};

enum FailureMode {
	FM_NO_FAILURE = 0,

	
	FM_OTHER_FAILURE = 255,
};

enum Command {
	CMD_DO_NOTHING = 0,
	CMD_PANIC = 1,
};

// Global variables
int consecutiveChecksumFailure = 0;
int lifetimeChecksumFailure = 0;
FailureMode safestNextFailureMode = FM_OTHER_FAILURE;
uint8_t command = -1;
uint8_t actuatorValues[RECEIVE_PAYLOAD_LENGTH] = {0};
FailureMode failureMode = FM_NO_FAILURE;
FailureCause failureCause = FC_NO_FAILURE;
unsigned long lastHeartbeatReceived = 0;
unsigned long lastHeartbeatSent = 0;
uint8_t current_packet[RECEIVE_PACKET_LENGTH] = {0};

// Sesnors
uint8_t sensor1 = 0;
uint8_t sensor2 = 0;
uint8_t sensor3 = 0;
uint8_t sensor4 = 0;
uint16_t sensor5 = 0;

// Demo variables
const byte FAILURE_LED = 10;
const byte STATUS_LED = LED_BUILTIN;

// Generate a checksum value from a array of bytes
long generate_checksum(uint8_t message[], int numBytes) {
	long result = 0;

	for (int i=0; i<numBytes; ++i) {
		result += (((long)message[i])*((long)message[i])*((long)message[i]))%251;
	}

	return result;
}

// Discard all bytes in buffer up to a newline character
inline void consume_until_newline() {
	while(Serial.peek() != '\n')
		Serial.read();
}

unsigned long bytes_to_long(uint8_t bytes[]) {
	long result = 0;
	
	result += ((long)bytes[3] << 24);
	result += ((long)bytes[2] << 16);
	result += ((long)bytes[1] << 8);
	result += bytes[0];

	return result;
}

// Convert a long into a 4-byte array 
void inline long_to_bytes(uint8_t buf[], long data) {
	buf[0] = data & 255;
	buf[1] = (data >> 8)  & 255;
	buf[2] = (data >> 16) & 255;
	buf[3] = (data >> 24) & 255;
}

// One-time serial and pin setup
void setup() {
	// Setup serial communication
	Serial.begin(9600);
	pinMode(STATUS_LED, OUTPUT);
	pinMode(FAILURE_LED, OUTPUT);
//  while (!Serial); // Wait for serial port to connect. Needed for native USB
}

// Process received heartbeat
void processUARTBuffer() {
	// Ignore all incoming heartbeats in case of failure
	if (failureMode) {
		return;
	}
	
	// Check if a full heartbeat packet is available
	if (Serial.available() >= RECEIVE_PACKET_LENGTH)
	{
		// Check if the start of the buffer is the beginning of a packet
		if (Serial.peek() == '\n') {
			Serial.readBytes(current_packet, RECEIVE_PACKET_LENGTH);

			// Calculate checksums
			unsigned long calculated_checksum = generate_checksum(current_packet, RECEIVE_PACKET_LENGTH-4);
			unsigned long* p = (unsigned long*)&current_packet[RECEIVE_PACKET_LENGTH-4];//Temp variable to make compiler happy
			unsigned long packet_checksum = *p;
		
			// Check for checksum mismatch
			if (calculated_checksum != packet_checksum) {
				++consecutiveChecksumFailure;
				++lifetimeChecksumFailure;
			} else {
				safestNextFailureMode = (FailureMode) current_packet[1];
				command = current_packet[2];
				memcpy(actuatorValues, &current_packet[3], RECEIVE_PAYLOAD_LENGTH);
				
				consecutiveChecksumFailure = 0;
				lastHeartbeatReceived = millis();
			}
		
		// Input is corrupted. Ignore all bytes until the packet start character is found.
		} else {
			consume_until_newline();
		}
	}
}

void processCommand() {
	switch (command) {
		case CMD_DO_NOTHING:
			break; // Do nothing.
			
		case CMD_PANIC:
			failureMode = safestNextFailureMode;
			failureCause = FC_PANIC_COMMAND;
			break;
	}
}

void stateActualization() {
	// Do useful things
	if (millis() % (long)pow(2, actuatorValues[0]) > (long)pow(2, actuatorValues[0]) / 2) {
		digitalWrite(STATUS_LED, HIGH);
	} else {
		digitalWrite(STATUS_LED, LOW);
	}

	switch (failureCause) {
		case FC_NO_FAILURE:
		case FC_PANIC_COMMAND:
			break; // Do nothing for these failure causes

		case FC_TOO_LONG_SINCE_LAST_HEARTBEAT:
			if (millis() & 128) {
				digitalWrite(FAILURE_LED, HIGH);
			} else {
				digitalWrite(FAILURE_LED, LOW);
			}
			break;

		case FC_TOO_MANY_LIFETIME_CHECKSUM_ERRORS:
			analogWrite(FAILURE_LED, (millis() & 2047) / 8);
			break;
	}

	// TODO: Implement alarm things
}

void sensorAcquisition() {
	// Get some senses
	sensor1 = actuatorValues[0]; //random(256);
	sensor2 = random(256);
	sensor3 = random(256);
	sensor4 = random(256);
	sensor5 = random(65536);
}

void checkHeartbeatErrors() {	
	if (lastHeartbeatReceived > 0 && millis() - lastHeartbeatReceived > 500) {
		failureCause = FC_TOO_LONG_SINCE_LAST_HEARTBEAT;
		failureMode = safestNextFailureMode;
	}

	if (lifetimeChecksumFailure > MAXIMUM_LIFETIME_CHECKSUM_ERRORS) {
		failureCause = FC_TOO_MANY_LIFETIME_CHECKSUM_ERRORS;
		failureMode = safestNextFailureMode;
	}
}

void emergencyDetect() {
	// Check all of the sensor state registers for error triggers. If trigger occurs, enter appropriate failure mode with appropriate failure cause.
}

void sendHeartbeat() {
	uint8_t packet[SEND_PACKET_LENGTH] = {0};
	
	packet[0] = '\n';
	packet[1] = safestNextFailureMode;
	packet[2] = failureMode;
	packet[3] = failureCause;
	packet[4] = consecutiveChecksumFailure;
	packet[5] = lifetimeChecksumFailure;

	//memcpy(&packet[6], sensorValues, SEND_PAYLOAD_LENGTH);
	//Copy sensor values into the correct locations
	packet[6] = sensor1;
	packet[7] = sensor2;
	packet[8] = sensor3;
	packet[9] = sensor4;
	uint8_t buf[4];
	long_to_bytes(buf, sensor5);
	memcpy(&packet[10], buf, 2);

	long checksum = generate_checksum(packet, SEND_PACKET_LENGTH-4);
	memcpy(&packet[SEND_PACKET_LENGTH-4], &checksum, 4);

	Serial.write(packet, SEND_PACKET_LENGTH);
	lastHeartbeatSent = millis();
}

void loop() {
	processUARTBuffer();
	processCommand();
	stateActualization();
	sensorAcquisition();

	// Restart loop if haven't received heartbeat yet
	if (!lastHeartbeatReceived) {
		return;
	}

	if (!failureMode) {
		checkHeartbeatErrors();
		emergencyDetect();
	}

	// Send heartbeat if more than 10 milliseconds has passed
	if (millis() - lastHeartbeatSent > 10) {
		sendHeartbeat();
	}
 }

