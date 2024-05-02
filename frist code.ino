#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BLUE 0x001F

#define e_s1 3 //echo pin
#define t_s1 4 //Trigger pin

#define e_s2 5 //echo pin
#define t_s2 6 //Trigger pin
#define relay 7
long dis_a = 0, dis_b = 0;
int flag1 = 0, flag2 = 0;
int person = 0;

void ultra_read(int pin_t, int pin_e, long &ultra_time) {
  long time;
  pinMode(pin_t, OUTPUT);
  pinMode(pin_e, INPUT);
  digitalWrite(pin_t, LOW);
  delayMicroseconds(2);
  digitalWrite(pin_t, HIGH);
  delayMicroseconds(10);  // Reduced delay here for more responsive reading
  time = pulseIn(pin_e, HIGH);
  ultra_time = time / 29 / 2;
}

void setup() {
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.display();
  display.setTextSize(1.8); // Set text size to 2 (larger text)
  delay(2000);
}

void loop() {
  ultra_read(t_s1, e_s1, dis_a);
  ultra_read(t_s2, e_s2, dis_b);

  Serial.print("da:"); Serial.println(dis_a);
  Serial.print("db:"); Serial.println(dis_b);

  if (dis_a < 90 && flag1 == 0) {
    flag1 = 1;
    if (flag2 == 0) {
      person = person + 1;
    }
  }
  if (dis_b < 90 && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0) {
      if (person > 0) {
        person = person - 1;
      }
    }
  }

  if (dis_a > 90 && dis_b > 90 && flag1 == 1 && flag2 == 1) {
    flag1 = 0, flag2 = 0;
    delay(100);
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Have Person: ");
  display.print(person);
  display.print("  ");
  display.setCursor(0, 16);
  display.print("Light is ");

  if (person > 0) {
    digitalWrite(relay, HIGH);
    display.print("On ");
  } else {
    digitalWrite(relay, LOW);
    display.print("Off");
  }

  display.display();
}