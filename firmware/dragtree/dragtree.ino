// Drag tree, two player reaction game

// -----  PINS  -----
const int BTN_PLAYER_1 = 26;
const int BTN_PLAYER_2 = 27;

const int LAMP_PRESTAGE = 16;
const int LAMP_STAGE = 17;
const int LAMP_AMBER_1 = 19;
const int LAMP_AMBER_2 = 18;
const int LAMP_AMBER_3 = 25;
const int LAMP_GREEN = 23;
const int LAMP_RED_LEFT = 21;
const int LAMP_RED_RIGHT = 22;

const int RELAYS[] ={
  LAMP_PRESTAGE, LAMP_STAGE,
  LAMP_AMBER_1, LAMP_AMBER_2, LAMP_AMBER_3,
  LAMP_GREEN, LAMP_RED_LEFT, LAMP_RED_RIGHT
};

const int NUM_RELAYS = sizeof(RELAYS) / sizeof(RELAYS[0]); 


// ----- TIMINGS (MS) -----
const unsigned long STAGE_TIME = 2000;
const unsigned long AMBER_STEP = 2000;
const unsigned long RANDOM_MIN = 1000;
const unsigned long RANDOM_MAX = 5000;
const unsigned long RESULT_TIME = 1000;
const unsigned long IGNORE_TIME = 100;
const unsigned long DEBOUNCE_TIME = 20;

// ----- TYPES -----
enum State { IDLE, PRESTAGE, STAGE, AMBERS, WAIT_RANDOM, GREEN, RESULT };
struct Player {
  const char* name;
  int buttonPin;
  int redLamp;
  bool wasPressed;          // Buttons mode last round, for flank detection
  bool redLight;            // Bad start
  bool done;                // If player pressed after green
  bool rawState;            // what the pin read last loop
  bool stableState;         // debounced, trusted state
  unsigned long changedAt;  // millis() when rawState last changed
  unsigned long reaction;   // microseconds
};

State state = IDLE;
unsigned long stateStart;
unsigned long greenTime;
unsigned long randomDelay;


// ----- LAMPS FUNCTIONS -----
void lampOn(int pin) { digitalWrite(pin, LOW); }
void lampOff(int pin) { digitalWrite(pin, HIGH); }
void allOff() {
  for (int i = 0; i < NUM_RELAYS; i++) lampOff(RELAYS[i]);
}


// ----- PLAYERS -----

Player p1 = {
  "Player 1",
  BTN_PLAYER_1, 
  LAMP_RED_LEFT, 
  false, 
  false, 
  false, 
  0
};

Player p2 = {
  "Player 2",
  BTN_PLAYER_2,
  LAMP_RED_RIGHT,
  false,
  false,
  false,
  0
};

bool isHeld(Player &p) {
  return p.stableState;
}

// Call ONCE per loop. Returns true when a debounced press begins.
bool updateButton(Player &p) {
  bool raw = digitalRead(p.buttonPin) == LOW;

  if (raw != p.rawState) {          // pin changed since last loop?
    p.rawState = raw;
    p.changedAt = millis();       // restart the stability clock
  }

  if (millis() - p.changedAt >= DEBOUNCE_TIME && p.rawState != p.stableState) {
    p.stableState = raw;     // accept the new state
    return raw;              // true only if it became pressed
  }
  return false;
}

void resetPlayer(Player &p) {
  p.redLight = false;
  p.done = false;
  p.reaction = 0;
}

void falseStart(Player &p) {
  if (!p.redLight) {
    p.redLight = true;
    lampOn(p.redLamp);
    Serial.print(p.name);
    Serial.println(": RED LIGHT!!");
    Serial.print("state=");
Serial.print(state);
Serial.print(" elapsed=");
Serial.println(millis() - stateStart);
  }
}

void printResult(Player &p) {
  Serial.print(p.name);
  if (p.redLight) {
    Serial.println(": RED LIGHT!!");
  } else {
    Serial.print(": ");
    Serial.print(p.reaction / 1000.0, 3);
    Serial.println(" ms");
  }
}


// ----- STATE MACHINE -----

void setState (State s) {
  state = s;
  stateStart = millis();
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < NUM_RELAYS; i++) {
    pinMode(RELAYS[i], OUTPUT);
    digitalWrite(RELAYS[i], HIGH);
  }

  pinMode(BTN_PLAYER_1, INPUT_PULLUP);
  pinMode(BTN_PLAYER_2, INPUT_PULLUP);
}

void loop() {
  bool p1New = updateButton(p1);
  bool p2New = updateButton(p2);
  unsigned long elapsed = millis() - stateStart;

  switch (state){

    case IDLE:
      if (isHeld(p1) && isHeld(p2)) {
        lampOn(LAMP_PRESTAGE);
        setState(PRESTAGE);
      }
    break;

    case PRESTAGE:
      if (!isHeld(p1) && !isHeld(p2)) {
        lampOn(LAMP_STAGE);
        setState(STAGE);
      }
    break;

    case STAGE:
      if (elapsed > IGNORE_TIME) {
        if (p1New) falseStart(p1);
        if (p2New) falseStart(p2);
      }
      if (elapsed > STAGE_TIME) {
        lampOn(LAMP_AMBER_1);
        setState(AMBERS);
      }
    break;

    case AMBERS:
      if (p1New) falseStart(p1);
      if (p2New) falseStart(p2);

      if (elapsed > AMBER_STEP) lampOn(LAMP_AMBER_2);
      if (elapsed > AMBER_STEP * 2) {
        lampOn(LAMP_AMBER_3);
        randomDelay = random(RANDOM_MIN, RANDOM_MAX);
        setState(WAIT_RANDOM);
      }
    break;

    case WAIT_RANDOM: 
      if (p1New) falseStart(p1);
      if (p2New) falseStart(p2);
      
      if (elapsed > randomDelay) {
        lampOn(LAMP_GREEN);
        greenTime = micros();
        setState(GREEN);
      }
    break;

    case GREEN:
      if (p1New && !p1.done && !p1.redLight) {
        p1.reaction = micros() - greenTime;
        p1.done = true;
      }

      if (p2New && !p2.done && !p2.redLight) {
        p2.reaction = micros() - greenTime;
        p2.done = true;
      }

      if ((p1.done || p1.redLight) && (p2.done || p2.redLight)) {
        printResult(p1);
        printResult(p2);
        if (p1.redLight && p2.redLight) {
          Serial.println("No winner, both red lights");
        } else if (p1.redLight) {
          Serial.println("Winner: Player 2");
        } else if (p2.redLight) {
          Serial.println("Winner: Player 1");
        } else if (p1.reaction < p2.reaction) {
          Serial.println("Winner: Player 1");
          Serial.print("With a difference of ");
          Serial.print((p2.reaction - p1.reaction) / 1000.0 , 3);
          Serial.println(" ms");
        } else if (p2.reaction < p1.reaction) {
          Serial.println("Winner: Player 2");
          Serial.print("With a difference of ");
          Serial.print((p1.reaction - p2.reaction) / 1000.0 , 3);
          Serial.println(" ms");
        } else {
          Serial.println("Tie!");
        }
        setState(RESULT);
      }
    break;

    case RESULT:
      if (elapsed > RESULT_TIME) {
        allOff();
        resetPlayer(p1);
        resetPlayer(p2);
        setState(IDLE);
      }
    break;

  }
}