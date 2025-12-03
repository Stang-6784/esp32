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

Adafruit_BME280 bme; // I2C
Adafruit_SSD1306 display(SCREENWIDTH, SCREENHEIGHT, &Wire, OLED_RESET);
RTC_DS3231 rtc;

float tempRead() {
    
    float readTempAVG;
    float tempSum = 0;

    for (int i = 0; i <= 10; i++) {
        float temp = bme.readTemperature();
        tempSum += temp;
        // Serial.println(tempSum);
        // Serial.print("\n");
        readTempAVG = tempSum / 10;
        delay(100);
    }
    return readTempAVG; 
}

float humiRead() {
	float readHumiAVG;
	float humiSum = 0;

	for (int i = 0; i <= 10; i++) {
		float humi = bme.readHumidity();
		humiSum += humi;
		// Serial.println(humiSum);
		// Serial.print("\n");
		readHumiAVG = humiSum / 10;
		delay(100);
	}
	return readHumiAVG;
}

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22); // SDA, SCL pins for ESP32

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
    Serial.println("Initialzing BME280...");

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

	Serial.print("Time: ");
	if (hour < 10) Serial.print("0");
	Serial.print(hour);
	Serial.print(":");
	if (min < 10) Serial.print("0");
	Serial.print(min);
	Serial.print(":");
	if (sec < 10) Serial.print("0");
	Serial.println(sec);

	delay(500);
    // Serial.print("Temperature: ");
    // Serial.print(tempRead());
    // Serial.println(" °C");
    // Serial.println("\n");
	// Serial.print("Humanity:");
	// Serial.print("Humanity:");
	// Serial.print(humiRead());
	// Serial.println(" %");

	// Clear the buffer.
	display.clearDisplay();
	display.setTextSize(1);      // Normal 1:1 pixel scale
	display.setTextColor(SSD1306_WHITE); // Draw white text
	display.setCursor(0, 0);     // Start at top-left corner
	display.print(tempRead());
	display.print(" C");
	display.setCursor(0, 30);     // Start at top-left corner
	display.print(humiRead());
	display.print(" %");
	display.setCursor(0, 40);     // Start at top-left corner

    display.display();
}
// int ledPins[] = {23, 22, 21, 18, 19};

// void setup() {
//   for (int i = 0; i < 5; i++) {
//     pinMode(ledPins[i], OUTPUT);
//   }
// }

// void loop() {
//   for (int brightness = 0; brightness <= 255; brightness += 5) {
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