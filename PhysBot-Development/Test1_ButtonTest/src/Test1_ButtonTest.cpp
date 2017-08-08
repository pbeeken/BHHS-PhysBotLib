#include <Streaming.h>
#include <PhysBotButton.h>

PhysBotButton button;

unsigned long start = 0L;
int buttonCount = 0;

void setup() {
  Serial.begin(115200);
  Serial << "Welcome to experiment 1!" << endl;
  Serial << "------------------------" << endl;
  Serial << "\tstate:" << (button.buttonIsDown() ? "D" : "U") << endl;
  // put your setup code here, to run once:
}

void loop() {
  int bp = button.readPushes();
  switch( bp ) {
    case 0:
      if ( millis() > start + 1000 ) {
        Serial << ".";
        start = millis();
      }
      break;
    case 1:
      Serial << "single release " << buttonCount++ << endl;
      delay( 400 );
      break;
    case 2:
      Serial << "double release " << buttonCount++ << endl;
      delay( 400 );
      break;
   default:
      Serial << "Multiple release [" << bp << "]" << endl;
      delay( 400 );
  }

}
