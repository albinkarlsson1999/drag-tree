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
const unsigned long STAGE_TIME = ;
const unsigned long AMBER_STEP = ;
const unsigned long RANDOM_MIN = ;
const unsigned long RANDOM_MAX = ;
const unsigned long RESULT_TIME = ;
const unsigned long IGNORE_TIME = 100;


// ----- LAMPS FUNCTIONS -----
void lampOn(int pin) { digitalWrite(pin, LOW); }
void lampOff(int pin) { digitalWrite(pin, HIGH); }
void allOff() {
  for (int i = 0; i < NUM_RELAYS; i++) lampOff(RELAYS[i]);
}


// ----- PLAYERS -----
struct Player {
  const char* name;
  int buttonPin;
  int redLamp;
  bool wasPressed;          // 
  bool redLight;            // Bad start
  bool done;                // If player pressed after green
  unsigned long reaction;   // microseconds
};

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
  return digitalRead(p.buttonPin) == LOW;
}

bool newPress(Player &p) {
  bool now = isHeld(p);
  bool result = now && !p.wasPressed;
  p.wasPressed = now;
  return result;
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
    Serial.println(": RED LIGHT!!")
  }
}



int lastP1 = HIGH;
int lastP2 = HIGH;

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
  for (int i = 0; i < NUM_RELAYS; i++) {
    Serial.print("Relay ");
    Serial.println(i + 1);

    digitalWrite(RELAYS[i], LOW);
    delay(6000);
    digitalWrite(RELAYS[i], HIGH);
    delay(200);
  }

  int p1 = digitalRead(BTN_PLAYER_1);
  if (p1 != lastP1) {
    Serial.print("Player 1: ");
    Serial.println(p1 == LOW ? "Pressed" : "Released" );
    lastP1 = p1;
  }
  
  int p2 = digitalRead(BTN_PLAYER_2);
  if (p2 != lastP2) {
    Serial.print("Player 2: ");
    Serial.println(p2 == LOW ? "Pressed" : "Released");
    lastP2 = p2;
  }
}