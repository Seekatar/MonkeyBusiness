#define LOGMSG_LEN 300
char logMsgBuffer[LOGMSG_LEN];

void logMsg(const char *msg, ...)
{
  va_list args;
  va_start( args, msg );
  vsnprintf( logMsgBuffer, LOGMSG_LEN, msg, args );

  Serial.println(logMsgBuffer);

  va_end(args);
}


#include "MonkeyProcessor.h"
MonkeyProcessor _processor = MonkeyProcessor(logMsg);

#define CrossPower2Pin 14
#define CrossPower1Pin 27
#define PowerPin 12

void setup() {
  Serial.begin(115200);
  while ( !Serial )
  {
    delay(100);    
  }
  Serial.println("Serial ok");
  
  pinMode(CrossPower2Pin,OUTPUT);
  pinMode(CrossPower1Pin,OUTPUT);
  pinMode(PowerPin,OUTPUT);
  digitalWrite(PowerPin,HIGH);
  digitalWrite(CrossPower1Pin,HIGH);
  digitalWrite(CrossPower2Pin,HIGH);

  _processor.initialize();

  Serial.println("Processor initialized");

}

void forward( int ms )
{
  digitalWrite(PowerPin,LOW);
  delay(ms);
  digitalWrite(PowerPin,HIGH);
}

void backward( int ms )
{
  digitalWrite(CrossPower1Pin,LOW);
  digitalWrite(CrossPower2Pin,LOW);
  digitalWrite(PowerPin,LOW);
  delay(ms);
  digitalWrite(PowerPin,HIGH);
  digitalWrite(CrossPower1Pin,HIGH);
  digitalWrite(CrossPower2Pin,HIGH);
}

void checkSerial()
{
  char s[200];
  if ( Serial.available() )
  {
    int ms = Serial.parseInt();
    Serial.print("Read ");
    Serial.println(ms);
    if ( ms == 1 )
    {
      backward( 100 );
      delay(200);
      forward( 90 );
    }
    else if ( ms == 2)
    {
      forward(1000);
      delay(200);
      backward(1000);
    }
    else if ( ms > 0 )
      forward(ms);
    else
      backward(-ms);
  }
}

void loop() {

  checkSerial();

  MonkeyProcessor::Action action = _processor.process();

  if ( action == MonkeyProcessor::Action::Clap )
  {
    forward( 4000 );
  }
}
