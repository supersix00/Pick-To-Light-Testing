// Tested with Adesso NuScan 5200TU
// 0076300342210 (Breakout Box) and 00763000533052 (Side ACC kit) Barcode configuration testing
// Jonah Kelley

const int greenLED = 14;
const int redLED = 12; 
const int ledPins[] = {15, 16, 17, 18, 19}; // SLOCs, need to make some sort of directory, expand to have multiple LEDs from same pins
int SLOCTest[sizeof(ledPins)];
const int buttonPins[] = {4, 5, 21, 22, 23};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);
int buttonState[numLeds];
int litSLOCs[numLeds];
int pressedLeds = 0;
int litLeds = 0;
int savedLitLeds = 0;
int matchedKitIndex = -1;
//int SLOC = 2999; // temporary
//int savedSLOC = 2999;
String barcode = "";
String savedBarcode = "";
String GTIN = "";
String Batch = "";

struct KitConfig {
  String serial;
  bool leds[numLeds];
  String GTINs[numLeds];
  int SLOCs[numLeds];
};

KitConfig kitConfigs[] = {
  { "123456", { false, true, true, true, true }},     
  { "55555P", { true, true, false, false, false }},
  { "00763000533052", { true, true, true, true, true }, { "00763000340285", "00763000340292", "00763000342210", "00763000351854", "00763000379094" }, { 3000, 4239, 3024, 3044, 4827}}, // Side ACC kit, random nums for SLOCs to test
  { "00763000342210", { true, true, false, false, false }, { "M014704C001", "M704430B020"}, { 3000, 4239}} // Breakout Box
};

const int numKits = sizeof(kitConfigs) / sizeof(kitConfigs[0]);

void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  for (int i = 0; i < numLeds; i++){
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  Serial.begin(115200); // initialize, change to 115200 according to YT video?
}

void loop() {
  if (Serial.available()) { // check if there is serial input
    char num = Serial.read(); // read serial input
    if (isAlphaNumeric(num)) { // if barcode is alphanumeric
      barcode += num;
    }

    if (num == '\n') {
      digitalWrite(greenLED, HIGH); // turn on LED
      digitalWrite(redLED, HIGH);

      delay(500);

      digitalWrite(greenLED, LOW); // turn off LED
      digitalWrite(redLED, LOW);
      delay(250);

      if (barcode.length() > 14){
        GTIN = barcode.substring(2, 16); // specific to 2210
        Batch = barcode.substring(18); // specific to 2210
      } else { // if they only scan GTIN, I didnt test these
        GTIN = barcode;
      }

      Serial.println(" ");
      Serial.print("Recieved: ");
      Serial.println(barcode);

      Serial.print("GTIN: ");
      Serial.println(GTIN);

      Serial.print("Batch: ");
      Serial.println(Batch);

      Serial.println(" ");

      processBarcode(GTIN);

      Serial.print("There are ");
      Serial.print(litLeds); // CONSTRAINTS: Assigns digital arduino pin to SLOC so amount of SLOCs is limited, this way might need multiple arduinos with different SLOCs coded
      Serial.print(" components for kit "); // Also assumes that all GTINs come from separate SLOCs
      Serial.print(GTIN);

      savedBarcode = GTIN;

      barcode = "";
      Serial.println(" in SLOCs: "); // UPDATE: Display quantity of same GTIN in same SLOC for picker to see how many they need to pick, still press button once

      if (matchedKitIndex != -1) {
        for (int i = 0; i < numLeds; i++) {
          if (kitConfigs[matchedKitIndex].leds[i]) {
            Serial.print(kitConfigs[matchedKitIndex].SLOCs[i]); // placeholder SLOCs to be replaced
            Serial.print(" - GTIN / M Part: ");
            Serial.print(kitConfigs[matchedKitIndex].GTINs[i]);
            Serial.println(", Batch: _____"); // batch from SAP, multiple batches shown?
          }
        }
      }

      savedLitLeds = litLeds;
      GTIN = "";
      Batch = "";
      litLeds = 0; // reset so it doesn't add to the previous scan
    }
  }
  for (int i = 0; i < numKits; i++){
    for (int j = 0; j < numLeds; j++){
      if (digitalRead(buttonPins[j]) == LOW){
        if (buttonState[j] == 0 && litSLOCs[j] == 1){  // button hasn't been pressed AND is lit
          digitalWrite(ledPins[j], LOW);
          if (matchedKitIndex != -1) {
            Serial.print("GTIN / M Part ");
            Serial.print(kitConfigs[matchedKitIndex].GTINs[j]);
            Serial.print(" consumed from SLOC "); // UPDATE TO SHOW COMPONENT GTIN SOMEHOW
            Serial.println(kitConfigs[matchedKitIndex].SLOCs[j]); // UPDATE just goes in order, figure out how to assign slocs to button presses
          }
          buttonState[j] = 1;
          litSLOCs[j] = 0; // reset for next scans
          pressedLeds = pressedLeds + 1;
          if (pressedLeds == savedLitLeds){
            delay(250);
            Serial.println(" ");
            Serial.print("Picking complete. All materials for GTIN ");
            Serial.print(savedBarcode);
            Serial.println(" consumed."); 
            Serial.println("=====================================================================================================================================================================================================");
            pressedLeds = 0;
            savedLitLeds = 0;
            matchedKitIndex = -1;
          }
        }
      }
    }
  }

  for (int i = 0; i < numLeds; i++){
    buttonState[i] = 0; // reset for next scans
  }
}

void processBarcode(const String& GTIN) {
  bool kitFound = false;
  for (int i = 0; i < numKits; i++) {
    if (GTIN == kitConfigs[i].serial) {
      lightUpKitComponents(kitConfigs[i]);
      matchedKitIndex = i;
      kitFound = true;
      break;
    }
  }
  if (kitFound == false) {
    turnOffAllLeds();
    digitalWrite(redLED, HIGH); // turn on red LED for invalid barcode
    delay(1000);
    digitalWrite(redLED, LOW); // turn off red LED
    delay(1000);
    //turnOffAllLeds();
  } else {
    digitalWrite(greenLED, HIGH); // turn on green LED for valid barcode
    delay(1000);
    digitalWrite(greenLED, LOW); // turn off green LED
    delay(1000);
    //delay(4000);
    //turnOffAllLeds();
  }
}

void lightUpKitComponents(const KitConfig& kitConfig) {
  for (int i = 0; i < numLeds; i++) {
    if (kitConfig.leds[i]) {
      digitalWrite(ledPins[i], HIGH); // light LED if coded for that barcode to light
      litSLOCs[i] = 1; // stores which LEDs are lit
      litLeds = litLeds + 1;
    } else {
      digitalWrite(ledPins[i], LOW); // keep LED unlit if not in scope of barcode
    }
  }
}

void turnOffAllLeds() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}