


const int pinCount = 8;
const int pins[pinCount] = {11, 10, 9, 8, 7, 6, 5, 4};
long pinConnections[pinCount];

const int buttonPin = 2;
volatile bool doCheck = false;

int combination[pinCount];

void check() { // runs when button is pressed : checks wire connections
  doCheck = true;
}

void performCheck() {
  for (int i = 0; i < pinCount; i++) { // we will get each pins connections
    pinMode(pins[i], INPUT_PULLUP); // current pin will be reading
    
    for (int j = 0; j < pinCount; j++) { // all other pins must be in output state
      if (i != j) {
        pinMode(pins[j], OUTPUT);
        bool allCorrect = true;
        for (int k = 0; k < 16; k++) { // repeat 16 times. no reason why it should be 16.
          int targetState;
          if (k % 2 == 0) {
            targetState = LOW;
          } else {
            targetState = HIGH;
            }
          digitalWrite(pins[j], targetState); // the target pin is set, 16 times, and we check if things match
          delayMicroseconds(10);
          allCorrect = allCorrect && (digitalRead(pins[i]) == targetState);
        }
        if (allCorrect) {
          pinConnections[i] += pow(2, j); // set to the actual pin tested
        }
      }
    }
    if (!foundMatch) {
      pinConnections[i] = pins[i]; // If we haven't found anything, just set it to itself
    }
  }
}

void setCombination() { // Sets a random combination
  performCheck();
  for (int i = 0; i < pinCount; i++) { // just set normal values
    combination[i] = pins[i];  
  }
  for (int i = 0; i < pinCount; i++) { // swaps each with a random thing
    int randomTarget = random(0, pinCount);
    int temp = combination[i];
    combination[i] = combination[randomTarget];
    combination[randomTarget] = temp;
  }
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), check, FALLING); // button is usually pulled up, so we check when it falls
  Serial.begin(9600);
  performCheck();

  // Doing random things to initialize the random seed;
  long randomSum = 0;
  for (int i = 0; i < pinCount; i++) {
    randomSum += pins[i] * pinConnections[i];
  }
  randomSum += analogRead(0);
  randomSum += micros();
  randomSeed(randomSum);

  setCombination();
  Serial.println("CODE BEGIN");
  for (int i = 0; i < pinCount; i++) {
      Serial.print(pins[i]);
      Serial.print("<->");
      Serial.print(combination[i]);
      Serial.print("  ");
    }
  Serial.println("CODE END");
}

void loop() {
  if (doCheck) {
    performCheck();
  
    for (int i = 0; i < pinCount; i++) {
      Serial.print(pins[i]);
      Serial.print("<->");
      Serial.print(pinConnections[i]);
      Serial.print("  ");
    }
    Serial.println();

    doCheck = false;
  }
}
