boolean ledvalue[12] = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
int led[12] = {0, 1 ,2 ,5 ,6 ,7, 8, 9, 10, 11, 12, 13};
int duration = 1000; // turntime, set to 1 sec by default
int previous_fork = 0; // timestamp of previous passing by of the fork
int current_fork; // timestamp of current passing by of the fork
unsigned long previous_timestamp = 0; // timestamp of previous loop-begin
unsigned long current_timestamp; // timestamp of current loop-begin, both [...]Millis are used for the delay-workaround

void resetleds(){ // reseting the position of the lighted up leds, should be used when the fork is detected to make to light-position dependent on the fork
  for (int i = 0; i<=11; i++){
    if(i <= 3){
      ledvalue[i] = 1;
    }
    else{
      ledvalue[i] = 0;
    }
  }
}

void ISRoutine(){ //called when falling signal on D3
  current_fork = millis();
  if((current_fork - previous_fork)>=10){
    duration = current_fork - previous_fork;
  }
  previous_fork = current_fork;
  resetleds();
}

void setleds(){ // writing ledvalues to leds
  for(int j = 0; j<=11; j++){ 
    digitalWrite(led[j], ledvalue[j]);
  }
}

void shift(){ //shifting the ledvalue-array from the bottom to the top; should be called every turntime/12; results in a clockwise running light around the tire (seen from right side)
 int temp = ledvalue[11];
 for(int i = 11; i>=0; i--){
   ledvalue[i]=ledvalue[i-1];
  }
 ledvalue[0] = temp;
}

void setup() { // setting up pinModes
  pinMode(3, INPUT);
  attachInterrupt(1,ISRoutine,FALLING);
  for(int j = 0; j<=11; j++){
    pinMode(led[j], OUTPUT); 
  }
}

void loop() {
  current_timestamp = millis();
  if((current_timestamp - previous_timestamp) > duration/12) {
    previous_timestamp = current_timestamp;
    setleds();
    shift();
  }
}