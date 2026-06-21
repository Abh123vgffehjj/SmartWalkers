#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <TinyGPSPlus.h>
#include <PulseSensorPlayground.h>

#define PULSE_PIN 34   // Pin connected to Pulse Sensor
#define FORCE_PIN 35   // Pin connected to Force Sensor
#define RXD2 16        // GPS Rx Pin
#define TXD2 17        // GPS Tx Pin

// Heart rate threshold for emergency alert
int threshold = 60;  // Example: 60 bpm for emergency alert

// Variables for GPS data
double lastLat = 0.0;
double lastLon = 0.0;
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);  // Use UART2 for GPS

// Pulse sensor setup
PulseSensorPlayground pulseSensor;
int pulseBPM = 0;
bool pulseSignalAvailable = false;

// MPU6050 for Step Counting
Adafruit_MPU6050 mpu;
int stepCount = 0;
float previousAccZ = 0.0;
const float stepThreshold = 1.1;  // Adjust this based on sensitivity required

// Calories Burned Variables
double caloriesBurned = 0.0;
double weight = 70.0;  // in kg (Adjust this based on user's weight)
double MET = 1.0;      // Metabolic equivalent for walking (adjust based on intensity)

void setup() {
  Serial.begin(115200);   // Initialize Serial communication
  gpsSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);  // Initialize GPS
  Serial.println("ESP32 Health Monitor Initialized.");

  // Pulse sensor setup
  pulseSensor.analogInput(PULSE_PIN);       // Set Pulse Sensor input pin
  pulseSensor.setThreshold(550);            // Set sensitivity threshold (tune as needed)
  if (pulseSensor.begin()) {
    Serial.println("Pulse Sensor initialized successfully.");
  } else {
    Serial.println("Pulse Sensor failed to initialize!");
  }

  // MPU6050 Setup
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1);  // Halt execution if MPU6050 is not found
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  // Pulse Sensor Handling
  pulseSignalAvailable = pulseSensor.sawStartOfBeat();  // Detect if a new beat is found

  if (pulseSignalAvailable) {
    pulseBPM = pulseSensor.getBeatsPerMinute();   // Get the heart rate in BPM

    // Display heart rate in Serial Monitor
    Serial.print("Heart Rate: ");
    Serial.print(pulseBPM);
    Serial.println(" BPM");

    // Emergency alert if heart rate drops below threshold
    if (pulseBPM < threshold) {
      Serial.println("Warning: Heart rate below threshold!");
    }
  } else {
    Serial.println("No valid heart rate detected.");
  }

  // GPS Handling
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isValid()) {
    double latitude = gps.location.lat();
    double longitude = gps.location.lng();

    // Calculate distance traveled from the last coordinates
    double distance_traveled = gps.distanceBetween(lastLat, lastLon, latitude, longitude);
    Serial.print("Distance Traveled: ");
    Serial.print(distance_traveled);
    Serial.println(" meters");

    // Update last GPS coordinates
    lastLat = latitude;
    lastLon = longitude;
  } else {
    Serial.println("Waiting for GPS signal...");
  }

  // Force Sensor Reading
  int forceValue = analogRead(FORCE_PIN);
  Serial.print("Force Sensor Value: ");
  Serial.println(forceValue);

  // Step Counting with MPU6050
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float accZ = a.acceleration.z;
  if (abs(accZ - previousAccZ) > stepThreshold) {
    stepCount++;  // Increment step count when threshold is exceeded
  }
  previousAccZ = accZ;

  // Display Step Count
  Serial.print("Step Count: ");
  Serial.println(stepCount);

  // Calories Burned Calculation
  caloriesBurned = calculateCaloriesBurned(stepCount, pulseBPM, MET);
  Serial.print("Calories Burned: ");
  Serial.println(caloriesBurned);

  Serial.println();  // Add a blank line between each loop output

  delay(1000);  // Delay to avoid spamming output
}

// Function to calculate calories burned
double calculateCaloriesBurned(int steps, int heartRate, double met) {
  // Basic calorie calculation formula (adjust this for better accuracy)
  double calories = (met * 3.5 * weight / 200) * (steps / 100.0);
  return calories;
}
