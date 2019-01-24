


const int pinCount = 8;
const int pins[pinCount] = {11, 10, 9, 8, 7, 6, 5, 4};
int pinConnections[pinCount];

const int buttonPin = 2;
volatile bool doCheck = false;

int combination[pinCount];

void check() { // runs when button is pressed : checks wire connections
  doCheck = true;
}

void performCheck() {
  for (int i = 0; i < pinCount; i++) { // we will get each pins connections
    pinMode(pins[i], INPUT_PULLUP); // current pin will be reading
    bool foundMatch = false;
    for (int j = 0; j < pinCount; j++) { // all other pins must be in output state
      if (i != j) {
        pinMode(pins[j], OUTPUT);
        bool allCorrect = true;
        for (int k = 0; k < 16; k++) { // repeat 16 times. no reason why it should be 16.
          int targetState;
          if (k % 3 == 0) {
            targetState = LOW;
          } else {
            targetState = HIGH;
            }
          digitalWrite(pins[j], targetState); // the target pin is set, 16 times, and we check if things match
          delayMicroseconds(10);
          allCorrect = allCorrect && (digitalRead(pins[i]) == targetState);
        }
        if (allCorrect) {
          pinConnections[i] = pins[j]; // set to the actual pin tested
          foundMatch = true;
          break;
        }
      }
    }
    if (!foundMatch) {
      pinConnections[i] = pins[i]; // If we haven't found anything, just set it to itself
    }
  }
}

void swap(int arr[], int index1, int index2) {
  int temp = arr[index1];
  arr[index1] = arr[index2];
  arr[index2] = temp;
}

void setCombination() { // Sets a random combination
  performCheck();
  int combinationPinsLeft = pinCount;
  int possibleIndexes[pinCount];
  for (int i = 0; i < pinCount; i++) { // just set normal values
    combination[i] = pins[i];
    possibleIndexes[i] = i;
  }
  
  int currentPinIndex = 0;
  while (combinationPinsLeft - currentPinIndex > 1) {
    int targetPinIndex = random(currentPinIndex, combinationPinsLeft);
    if (targetPinIndex == currentPinIndex) {
      swap(possibleIndexes, currentPinIndex, combinationPinsLeft-1); // combinationPinsLeft is a count, to get an index, do -1
      currentPinIndex++;
      combinationPinsLeft--;
    } else {
      
    }
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
