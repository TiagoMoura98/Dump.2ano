#include <DHT.h>
#include <Adafruit_Sensor.h>

#define DHTPIN 15      // Pin where the DHT11 is connected
#define DHTTYPE DHT11  // DHT 11 sensor
#define LED1 16
#define LED2 17
#define LED3 18
#define LED4 19
#define LED5 20

DHT dht(DHTPIN, DHTTYPE);

float initialTemp = 0;       // Initial temperature
float initialHumidity = 0;   // Initial humidity
bool isProcessing = false;   // Flag to indicate if an operation is being processed
int operationNumber = 0;     // Operation number

void setup() {
  Serial.begin(9600); // Start the serial communication at 9600 baud rate
  pinMode(LED_BUILTIN, OUTPUT); // Set the built-in LED pin as output
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // Turn the built-in LED off

  dht.begin();
  delay(2000); // Allow sensor to stabilize

  // Get initial readings
  initialTemp = dht.readTemperature();
  initialHumidity = dht.readHumidity();

  // Retry until valid initial readings are obtained
  while (isnan(initialTemp) || isnan(initialHumidity)) {
    Serial.println("Failed to read initial sensor values, retrying...");
    delay(2000);
    initialTemp = dht.readTemperature();
    initialHumidity = dht.readHumidity();
  }

  // Display initial readings
  Serial.print("Initial Temperature: ");
  Serial.print(initialTemp);
  Serial.print(" °C, Initial Humidity: ");
  Serial.print(initialHumidity);
  Serial.println(" %");
}

void blink_internal_led() {
  digitalWrite(LED_BUILTIN, LOW); 
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH); 
}

void turn_on_leds(String cmd) {
  // Parse the command to control LEDs
  int led_states[5];
  sscanf(cmd.c_str(), "%*[^,],%d,%d,%d,%d,%d", &led_states[0], &led_states[1], &led_states[2], &led_states[3], &led_states[4]);

  digitalWrite(LED1, led_states[0] ? LOW : HIGH);
  digitalWrite(LED2, led_states[1] ? LOW : HIGH);
  digitalWrite(LED3, led_states[2] ? LOW : HIGH);
  digitalWrite(LED4, led_states[3] ? LOW : HIGH);
  digitalWrite(LED5, led_states[4] ? LOW : HIGH);
}

void turn_off_leds() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  digitalWrite(LED5, HIGH);
}

void process_operation(String command) {
  isProcessing = true;
  unsigned long startTime = millis(); // Record the start time

  while (millis() - startTime < 10000) { // Loop for 10 seconds
    blink_internal_led(); // Blink the internal LED
    delay(500); // Delay for 500 milliseconds

    // Check for STATUS command during operation
    if (Serial.available() > 0) {
      String statusCommand = Serial.readStringUntil('\n');
      if (statusCommand == "STATUS") {
        Serial.print("Operation in progress:  ");
        Serial.println(command); // Print operation number in binary
        turn_on_leds(command);
        delay(2000); // Sleep for 2 seconds
        turn_off_leds();
      }
    }
  }

  turn_on_leds(command);
  delay(2000); // Sleep for 2 seconds
  turn_off_leds();

  isProcessing = false;
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  if (Serial.available() > 0) { // Check if data is available on the serial port
    String command = Serial.readStringUntil('\n'); // Read the incoming string until newline
    String commandtemp = command;

    // Read current sensor values
    float currentTemp = dht.readTemperature();
    float currentHumidity = dht.readHumidity();

    // Check for valid readings
    if (isnan(currentTemp) || isnan(currentHumidity)) {
      Serial.println("Failed to read current sensor values!");
      return;
    }

    // Create data string
    String data = "TEMP&unit:celsius&value:" + String(currentTemp) + "#HUM&unit:percentage&value:" + String(currentHumidity);

  

      if (command.startsWith("ON")) {
        Serial.println("Machine is turning on.");
        digitalWrite(LED_BUILTIN, HIGH); // Turn the built-in LED on
        turn_on_leds(command);
        delay(2000); // Sleep for 2 seconds
        turn_off_leds();
      } else if (command.startsWith("OP")) {
        Serial.println(data); // Send the data back to the serial port
        turn_on_leds(command);
        delay(2000); // Sleep for 2 seconds
        turn_off_leds();
        process_operation(commandtemp);
      } else if (command.startsWith("OFF")) {
        digitalWrite(LED_BUILTIN, LOW); // Turn the built-in LED off
        turn_off_leds();
      } else if (command.startsWith("STATUS")) {
        if (isProcessing) {
          Serial.print("Operation in progress: ");
          Serial.println(command); // Print operation number in binary
        } else {
          Serial.println("Machine is on.");
        }
    }
  }
}