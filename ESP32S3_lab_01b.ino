/**********************************************************************
  Filename    : ESP32S3_lab_1b
  Target      : ESP32-S3-WROOM
  Author      : Martin Harnevie
  Description : input from potentiometer via ADC, with PWM output to control LED
  Logbook     :
  20261020 000 - works for ESP32-S3-WROOM
  20261021 001 - touchup with round robin
  **********************************************************************/

const uint8_t   KPinLedExternal =           0x0e; // 14; // external LED pin
const uint8_t   KPinLedInternal =           0x02; // internal LED pin
const uint8_t   KPinAdcPotentiometer =      0x01;

// ADC & PWM
const uint32_t  KPwmFrequency = 25;
const uint8_t   KPwmChannel = 0;
const uint8_t   KPwmBitResolution = 12;

uint16_t valueAdc;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // initialise PWM control for LED
  ledcAttachChannel( KPinLedExternal, KPwmFrequency, KPwmBitResolution, KPwmChannel );

  // initialise heartbeat
  pinMode( KPinLedInternal, OUTPUT );
}

void loop() {

  doReadPrintAndControlLed(333);

  doHeartBeat();

}

void doReadPrintAndControlLed( uint32_t aInterval ) {
  uint32_t currentTime = millis();
  static uint32_t previousTime = currentTime;
  if( currentTime - previousTime < aInterval ) return; else previousTime = currentTime;

  valueAdc = analogRead( KPinAdcPotentiometer );
  Serial.println( valueAdc );
  ledcWrite( KPinLedExternal , valueAdc );
}

void doHeartBeat() {
  uint32_t currentTime = millis();
  static uint32_t previousTime = currentTime;
  static bool ledOnB = false;
  const uint32_t KHeartBeatInterval = 1000; // millis
  const uint32_t KHeartBeatBlink = 28;    // millis

  // once per interval
  if( !ledOnB && ( currentTime - previousTime >= KHeartBeatInterval )) {
    digitalWrite(KPinLedInternal, HIGH);
    previousTime = currentTime;
    ledOnB = true;
  }
  
  // off after heartbeat
  if( ledOnB && ( currentTime - previousTime >= KHeartBeatBlink )) {
    digitalWrite(KPinLedInternal, LOW);
    ledOnB = false;
  }
}
