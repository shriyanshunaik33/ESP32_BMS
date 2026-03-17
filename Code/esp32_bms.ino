#include "arduino_secrets.h"
#include "arduino_secrets.h"
#include "thingProperties.h"

// Pin Definitions
const int batteryVoltagePin = 35;  // Analog pin for battery voltage
const int thermistorPin = 34;      // Analog pin for thermistor
const int relayPin = 12;           // Digital pin for relay control

// Battery Parameters
const float maxBatteryVoltage = 4.2;   // Max voltage for a Li-ion cell
const float minBatteryVoltage = 3.0;   // Min voltage for a Li-ion cell
const float maxTemperature = 45.0;     // Max safe temperature in Celsius
const float voltageHysteresis = 0.1;   // Hysteresis to prevent rapid relay switching

bool chargingEnabled = true; // Tracks relay status

// Voltage Divider Parameters
const float R1 = 10000.0;  // Resistor R1 in voltage divider (10kΩ)
const float R2 = 10000.0;  // Resistor R2 in voltage divider (10kΩ)

// Thermistor Parameters (for a generic 10kΩ NTC Thermistor)
const float thermistorNominal = 10000.0;    // Resistance at 25°C (10kΩ)
const float temperatureNominal = 25.0;      // Nominal temperature in Celsius
const float bCoefficient = 3950.0;        // Beta coefficient (B-value) - check your thermistor's datasheet for the exact value
const float seriesResistor = 10000.0;       // Series resistor (10kΩ)

// **Declare missing Cloud variables**
float batteryVoltageCloud;  // Battery voltage to be sent to the cloud
bool chargingStatus;        // Charging status flag
float batteryPercentage;    // Battery percentage to be sent to the cloud
float temperature;          // Temperature reading to be sent to the cloud

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);  // Start with relay ON (charging enabled)

  // Initialize Arduino IoT Cloud
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  // Read battery voltage
  float batteryVoltage = readBatteryVoltage();
  batteryPercentage = calculateBatteryPercentage(batteryVoltage);

  // Read temperature
  float temperatureValue = readTemperature();
  temperature = temperatureValue;  

  // Update Cloud variables
  batteryVoltageCloud = batteryVoltage;
  chargingStatus = chargingEnabled;  // Sync relay state with cloud

  // Charging Control Logic with Hysteresis
  if ((batteryVoltage >= maxBatteryVoltage || temperatureValue >= maxTemperature)) {
    chargingEnabled = false;  // Stop charging
  } else if (batteryVoltage <= (maxBatteryVoltage - voltageHysteresis)) {
    chargingEnabled = true;   // Resume charging when voltage drops below threshold
  }

  digitalWrite(relayPin, chargingEnabled ? HIGH : LOW);

  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage);
  Serial.print("V, Battery %: ");
  Serial.print(batteryPercentage);
  Serial.print("%, Temperature: ");
  Serial.print(temperatureValue);
  Serial.print("°C, Charging: ");
  Serial.println(chargingEnabled ? "ON" : "OFF");

  delay(1000);  // Wait for 1 second before next reading
}

float readBatteryVoltage() {
  float sum = 0;
  const int numSamples = 10;

  for (int i = 0; i < numSamples; i++) {
    sum += analogReadMilliVolts(batteryVoltagePin);  // Read in millivolts
    delay(5);
  }

  float averageMilliVolts = sum / numSamples;
  float voltage = (averageMilliVolts / 1000.0);  // Convert mV to V
  voltage = voltage * (R1 + R2) / R2;  // Adjust for voltage divider
  return voltage;
}

float calculateBatteryPercentage(float voltage) {
  float percentage = (voltage - minBatteryVoltage) / (maxBatteryVoltage - minBatteryVoltage) * 100;
  return constrain(percentage, 0, 100);
}

float readTemperature() {
  int analogValue = analogRead(thermistorPin);
  if (analogValue == 0) return -100.0; // Error case

  float voltage = (analogValue / 4095.0) * 3.3;
  float resistance = (voltage > 0) ? (seriesResistor * voltage / (3.3 - voltage)) : seriesResistor;
  float temperature = 1.0 / (log(resistance / thermistorNominal) / bCoefficient + 1.0 / (temperatureNominal + 273.15)) - 273.15;
  return temperature;
}