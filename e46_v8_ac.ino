#include <SPI.h>
#include <mcp2515_can.h>

const int SPI_CS_PIN = 9;

mcp2515_can CAN(SPI_CS_PIN);

const int COMPRESSOR_RELAY_PIN = 7;
const int FAN_RELAY_PIN = 6;

const float THROTTLE_THRESHOLD = 80.0;

bool acRequested = false;
bool acActive = false;
bool throttleLock = false;
bool fanActive = false;

void setup() {
  Serial.begin(115200);
  pinMode(COMPRESSOR_RELAY_PIN, OUTPUT);
  pinMode(FAN_RELAY_PIN, OUTPUT);
  digitalWrite(COMPRESSOR_RELAY_PIN, HIGH);
  digitalWrite(FAN_RELAY_PIN, HIGH);

  while (CAN_OK != CAN.begin(CAN_500KBPS)) {
    Serial.println("CAN BUS Shield init fail");
    delay(100);
  }
  Serial.println("CAN BUS Shield init ok!");

  CAN.init_Mask(0, 0, 0x7FF);
  CAN.init_Mask(1, 0, 0x7FF);

  CAN.init_Filt(0, 0, 0x329);
  CAN.init_Filt(1, 0, 0x615);
}

void loop() {
  unsigned char len = 0;
  unsigned char buf[8];

  if (CAN.checkReceive() == CAN_MSGAVAIL) {
    CAN.readMsgBuf(&len, buf);
    unsigned long canId = CAN.getCanId();

    if (canId == 0x615) {
      bool acOn = (buf[0] & 0x80) || (buf[4] & 0x80);

      if (acOn && !acActive && !throttleLock) {
        acRequested = true;
        activateCompressor(true);
        activateFan(true);
      } else if (!acOn && acActive) {
        acRequested = false;
        activateCompressor(false);
        activateFan(false);
      }
    }

    if (canId == 0x329) {
      unsigned char throttlePosition = buf[5];
      float throttlePercentage = map(throttlePosition, 0x00, 0xFE, 0, 99.2);

      if (throttlePercentage > THROTTLE_THRESHOLD && !throttleLock) {
        throttleLock = true;
        activateCompressor(false);
        Serial.println("Compressor turned off due to high throttle, throttleLock engaged");
      } else if (throttlePercentage <= THROTTLE_THRESHOLD && throttleLock) {
        throttleLock = false;
        Serial.println("Throttle below threshold, throttleLock disengaged");
        if (acRequested) {
          activateCompressor(true);
        }
      }
    }
  }
}

void activateCompressor(bool on) {
  if (on && !acActive) {
    digitalWrite(COMPRESSOR_RELAY_PIN, LOW);
    acActive = true;
    Serial.println("Compressor ON");
  } else if (!on && acActive) {
    digitalWrite(COMPRESSOR_RELAY_PIN, HIGH);
    acActive = false;
    Serial.println("Compressor OFF");
  }
}

void activateFan(bool on) {
  if (on && !fanActive) {
    fanActive = true;
    digitalWrite(FAN_RELAY_PIN, LOW);
    Serial.println("Fan ON");
  } else if (!on && fanActive) {
    fanActive = false;
    digitalWrite(FAN_RELAY_PIN, HIGH);
    Serial.println("Fan OFF");
  }
}
