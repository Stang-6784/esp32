#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#define SCREENWIDTH 128 // OLED display width, in pixels
#define SCREENHEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define Red_LED 16
#define Yellow_LED 17
#define Buzzer 18

Adafruit_BME280 bme; // I2C
Adafruit_SSD1306 display(SCREENWIDTH, SCREENHEIGHT, &Wire, OLED_RESET);
RTC_DS3231 rtc;

// float tempRead() {
// 	float tempSum = 0;

// 	for (int i = 0; i < 10; i++) {
// 		tempSum += bme.readTemperature();
// 		delay(100);
// 	}
// 	return tempSum / 10.0;
// }

// float humiRead() {
// 	float humiSum = 0;

// 	for (int i = 0; i < 10; i++) {
// 		humiSum += bme.readHumidity();
// 		delay(100);
// 	}
// 	return humiSum / 10.0;
// }

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22); // SDA, SCL pins for ESP32
	pinMode(Red_LED, OUTPUT);
	pinMode(Yellow_LED, OUTPUT);
	pinMode(Buzzer, OUTPUT);

	if (!bme.begin(0x76)) {
		Serial.println("ไม่พบ BME280 ตรวจสอบการเชื่อมต่อ!");
		while (1);
	}
    
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
        Serial.println("ไม่พบจอ OLED ตรวจสอบการเชื่อมต่อ!");
        while (1);
    }
	if (!rtc.begin()) {
		Serial.println("ไม่พบ RTC ตรวจสอบการเชื่อมต่อ!");
		while (1);
	}

	if (rtc.lostPower()) {
		Serial.println("RTC ยังไม่ถูกตั้งค่า กำลังตั้งค่าเวลาเริ่มต้น...");
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	}
        // Invert Display
         //display.invertDisplay(1);

    // pinMode(ledPinBlue, OUTPUT);
    Serial.println("Initializing BME280...");

    while (!bme.begin(0x76)) {
        Serial.print(".");
        }
    Serial.println("\nBME280 initialization successful!");
    Serial.println("Sensor is ready to read data:");
}


void loop() {
	DateTime now = rtc.now();
	int hour = now.hour();
	int min = now.minute();
	int sec = now.second();
	delay(500);

	// Read sensors once per loop to avoid toggling buzzer due to multiple slow reads
	float currentTemp = bme.readTemperature();
	// float currentHumi = humiRead();

	// Update display once with the read values and status
	display.clearDisplay();
	display.setTextSize(1);      // Normal 1:1 pixel scale
	display.setTextColor(SSD1306_WHITE); // Draw white text
	display.setCursor(0, 0);

	if (currentTemp >= 24.0) {
		digitalWrite(Red_LED, HIGH);
		digitalWrite(Yellow_LED, LOW);
		digitalWrite(Buzzer, HIGH);
		display.print("danger wheather");
	} else if (currentTemp >= 20.0) {
		digitalWrite(Red_LED, LOW);
		digitalWrite(Yellow_LED, HIGH);
		digitalWrite(Buzzer, LOW);	
		display.print("normal wheather");
	} else {
		digitalWrite(Red_LED, LOW);
		digitalWrite(Yellow_LED, LOW);
		digitalWrite(Buzzer, LOW);
		display.print("cold wheather");
	}
	Serial.print("Temperature: ");
	Serial.print(currentTemp);
	Serial.println(" °C");

	display.setCursor(0, 16);
	display.print("Time: ");
	if (hour < 10) display.print("0");
	display.print(hour);
	display.print(":");
	if (min < 10) display.print("0");
	display.print(min);
	display.print(":");
	if (sec < 10) display.print("0");
	display.print(sec);
	display.display();
}
// int ledPins[] = {23, 22, 21, 18, 19};

// void setup() {
//   for (int i = 0; i < 5; i++) {
//     pinMode(ledPins[i], OUTPUT);
//   }
// }

// void loop() {
//   forS (int brightness = 0; brightness <= 255; brightness += 5) {
//     for (int i = 0; i < 5; i++){
//       analogWrite(ledPins[i], brightness);
//       delay(20);
//     }
//   }

//   for (int brightness = 255; brightness >= 0; brightness -= 5) {
//     for (int i = 0; i < 5; i++) {
//       analogWrite(ledPins[i], brightness);
//       delay(20);
//     }
//   }
//   delay(1000);
// }