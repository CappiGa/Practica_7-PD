#include <Arduino.h>
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourcePROGMEM.h"
#include "sampleaac.h"  // Este archivo debe contener el array con el audio AAC

AudioFileSourcePROGMEM *in;
AudioGeneratorAAC *aac;
AudioOutputI2S *out;

void setup() {
  Serial.begin(115200);

  in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac));
  aac = new AudioGeneratorAAC();
  out = new AudioOutputI2S();
  out->SetGain(0.125);
  out->SetPinout(26, 25, 22);  // DIN, BCLK, LRC

  aac->begin(in, out);
}

void loop() {
  if (aac->isRunning()) {
    aac->loop();
  } else {
    aac->stop();
    Serial.println("Sound Generator");
    delay(1000);
  }
}