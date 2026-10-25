// Drag tree step 1: read the buttons and print changes

const int BTN_PLAYER_1 = 26;
const int BTN_PLAYER_2 = 27;
const int RELAYS[] = {16,17,18,19,21,22,23,25};
const int NUM_RELAYS = sizeof(RELAYS) / sizeof(RELAYS[0]); // Calculate the amount of relays

int lastP1 = HIGH;
int lastP2 = HIGH;

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < NUM_RELAYS; i++) {
    digitalWrite(RELAYS[i], HIGH);
    pinMode(RELAYS[i], OUTPUT);
  }

  pinMode(BTN_PLAYER_1, INPUT_PULLUP);
  pinMode(BTN_PLAYER_2, INPUT_PULLUP);
}

void loop() {
  for (int i = 0; i < NUM_RELAYS; i++) {
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