#include "SD.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;

void setup(){
tmrpcm.speakerPin = 46;
Serial.begin(115200);
if (!SD.begin(SD_ChipSelectPin)) {
Serial.println("SD fail");
return;
}

//tmrpcm.setVolume(10);
Serial.println("playing");
tmrpcm.play("xyz.wav");
}

void loop(){  }
