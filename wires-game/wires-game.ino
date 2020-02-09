
#define CHECKCOUNT 16

#define PINCOUNT 8
const int pins[PINCOUNT] = {11, 10, 9, 8, 7, 6, 5, 4};
const int buttonPin = 2;

volatile bool doCheck = false;

class DisjointSet {
  public:
    DisjointSet();
    int find(int);
    void uni(int, int);
    void randomlyConnect(int);
  private:
    int parent[PINCOUNT];
};

DisjointSet::DisjointSet() {
  for (int i = 0; i < PINCOUNT; i++) {
    parent[i] = i;
  }
}

int DisjointSet::find(int a) {
  if (a == parent[a]) {
    return a;
  } else {
    parent[a] = find(parent[a]);
    return parent[a];
  }
}

void DisjointSet::uni(int a, int b) {
  parent[find(a)] = find(b);
}

void DisjointSet::randomlyConnect(int a) {
  if (a == 0) {
    return;
  } else {
    uni(random(PINCOUNT), random(PINCOUNT));
    randomlyConnect(a - 1);
  }
}

DisjointSet generated;
DisjointSet given;

void check() { // runs when button is pressed : checks wire connections
  doCheck = true;
}

void setup() {

  // set up checking button
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), check, FALLING); // button is usually pulled up, so we check when it falls
  
  // Doing random things to initialize the random seed;
  long randomSum = 0;
  randomSum += analogRead(0);
  randomSum += micros();
  randomSeed(randomSum);

  // Generate final answer
  generated.randomlyConnect(PINCOUNT);

  #ifdef DEBUG
  Serial.begin(9600);

  Serial.println("setup done");

  #endif
}

void loop() {
  if (doCheck) {
    #ifdef DEBUG
    Serial.println("do check");
    #endif

    doCheck = false;
  }
}
