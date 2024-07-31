#include <AsyncTimer.h>

#define LASER_TRIGGER_PIN A0
#define AIR_ASSIST_RELAY_PIN A4
#define SMOKE_FAN_RELAY_PIN A5

#define AIR_ASSIST_TIMEOUT 3500
#define SMOKE_FAN_TIMEOUT 10000

AsyncTimer timer;

unsigned short airAssistTimerId = 0;
unsigned short smokeFanTimerId = 0;

auto airAssistOffLambda = [] { 
  Serial.println("Stopping Air Assist");
  digitalWrite(AIR_ASSIST_RELAY_PIN, LOW);
  airAssistTimerId = 0;
};

auto smokeFanOffLambda = [] {
  Serial.println("Stopping Smoke Fan"); 
  digitalWrite(SMOKE_FAN_RELAY_PIN, LOW);
  smokeFanTimerId = 0; 
};


void setup() {
  Serial.begin(9600);

  pinMode(LASER_TRIGGER_PIN, INPUT_PULLUP);

  pinMode(AIR_ASSIST_RELAY_PIN, OUTPUT);
  pinMode(SMOKE_FAN_RELAY_PIN, OUTPUT);
  digitalWrite(AIR_ASSIST_RELAY_PIN, LOW);
  digitalWrite(SMOKE_FAN_RELAY_PIN, LOW);
}

void loop() {
  timer.handle();

  if(!digitalRead(LASER_TRIGGER_PIN)) {
    Serial.println(F("Input Pin is low"));

    digitalWrite(AIR_ASSIST_RELAY_PIN, HIGH);
    digitalWrite(SMOKE_FAN_RELAY_PIN, HIGH);

    if(airAssistTimerId == 0) {
      Serial.println(F("Starting Air Assist Timer"));
      airAssistTimerId = timer.setTimeout(airAssistOffLambda, AIR_ASSIST_TIMEOUT);
    }
    else {
      Serial.println(F("Resetting Air Assist Timer"));
      timer.reset(airAssistTimerId);
    }

    if(smokeFanTimerId == 0) {
      Serial.println(F("Starting Smoke Fan Timer"));
      smokeFanTimerId = timer.setTimeout(smokeFanOffLambda, SMOKE_FAN_TIMEOUT);
    }
    else {
      Serial.println(F("Resetting Smoke Fan Timer"));
      timer.reset(smokeFanTimerId);
    }
  }
}