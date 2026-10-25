// Drag tree step 1: read the buttons and print changes

// Buttons
const int BTN_PLAYER_1 = 26;
const int BTN_PLAYER_2 = 27;

// Lamp relays
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