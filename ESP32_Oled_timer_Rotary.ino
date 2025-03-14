#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define startButton 26
#define encoder_A 33
#define encoder_B 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int hours;
int minutes;
int seconds;
unsigned long previousMillis = 0;
const long  interval = 1000;

int encoderCount = 0;
int encoder_ALast;
int value_A;
boolean bCW;

int buttonState;
int lastButtonState = HIGH;
int State = LOW;

long totalSeconds = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(startButton, INPUT_PULLUP); // start button
  pinMode (encoder_A, INPUT_PULLUP);
  pinMode (encoder_B, INPUT_PULLUP);
  encoder_ALast = digitalRead(encoder_A);
}

void loop() {
  // encoder gomb kezelés-----------------------
  buttonState = digitalRead(startButton);
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      State = !State;
    }
  }
  lastButtonState = buttonState;

  // encoder kezelés-----------------------------------
  value_A = digitalRead(encoder_A);
  if (value_A != encoder_ALast) {
    if (digitalRead(encoder_B) != value_A) {
      encoderCount ++;
      totalSeconds = max(0, encoderCount * 60);
    }
    else {
      encoderCount--;
      totalSeconds = max(0, encoderCount * 60);
    }
  }
  encoder_ALast = value_A;

  

  if (State == 1) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (totalSeconds > 0) {
          totalSeconds--;
      } else {
        showTimeoutScreen();
        return;
      }
    }
  }

  display.clearDisplay();
  hours = totalSeconds / 3600;
  minutes = (totalSeconds % 3600) / 60;
  seconds = totalSeconds % 60;
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(String(encoderCount));
  display.println(String(totalSeconds));
  display.println(String(seconds));
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 30);
  display.printf("%02d:%02d:%02d", hours, minutes, seconds);
  display.display();
}

void showTimeoutScreen() {
  display.clearDisplay();
  display.fillScreen(SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setTextSize(2);
  display.setCursor(15, 25);
  display.display();
}
