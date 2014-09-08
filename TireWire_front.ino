#include <NewPing.h>
#define TRIGGER_PIN  4  // trigger-pin of the sonar sensor
#define ECHO_PIN     3  // echo-pin of the sonar sensor
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

boolean ledvalue[12] = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
int led[12] = {0, 1 ,2 ,5 ,6 ,7, 8, 9, 10, 11, 12, 13};

unsigned int pingSpeed = 20; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;     // Holds the next ping time.
int dist;
int fork= 0;
int forkprev = 0;
int forknow;
int duration = 1000;

unsigned long previousMillis = 0;
unsigned long currentMillis;

void resetleds(){
  for (int i = 0; i<=3; i++){
    ledvalue[i] = 1;
  }
  for (int i = 4; i<=11; i++){
    ledvalue[i] = 0;
  }
}

void shift(){
 int temp = ledvalue[11]; // 0
 for(int i = 11; i>=0; i--){
   ledvalue[i]=ledvalue[i-1];
  }
 ledvalue[0] = temp; //11
}

void setup() {
  pingTimer = millis();
  for(int j = 0; j<=11; j++)
  {
    pinMode(led[j], OUTPUT); 
  }
}

void echoCheck() { // Timer2 interrupt calls this every 24uS to check ping status.
  // Don't do anything here!
  if (sonar.check_timer()) { // if the ping received.
    // Here's where you can add code.
    dist = sonar.ping_result / US_ROUNDTRIP_CM;

    if (dist<50 && fork == 0){
      fork = 1;
      forknow = millis();
      duration = forknow - forkprev;
      forkprev = forknow;
      resetleds();
    }
    if(dist>50 && fork == 1){
      fork = 0;
    }
  }
  // Don't do anything here!
}

void setleds(){
  
  for(int j = 0; j<=11; j++)
  { 
  digitalWrite(led[j], ledvalue[j]);
  }
}

void loop() {
      
  currentMillis = millis();
  if((currentMillis - previousMillis) > duration/12) {
    previousMillis = currentMillis;
    setleds();
    shift();
    if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping
      pingTimer += pingSpeed;
      sonar.ping_timer(echoCheck); // Send out the ping, calls "echoCheck" function every 24uS
    }
  }
  else{
    if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping
      pingTimer += pingSpeed;
      sonar.ping_timer(echoCheck); // Send out the ping, calls "echoCheck" function every 24uS
    }
  }
}
