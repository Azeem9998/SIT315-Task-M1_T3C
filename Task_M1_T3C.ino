const int BUTTON_PIN = 2;             // the number of the button pin
const int LED_PIN = 4;                // the number of the LED pin
const int LIGHT_SENSOR_PIN = A0;      // the number of the light sensor pin
const int PIR_SENSOR_PIN = 3;         // the number of the PIR sensor pin
const int BUZZER_PIN = 5;             // the number of the buzzer pin

volatile int buttonState = HIGH;      // variable for reading the pushbutton status
volatile int lightSensorValue = 0;    // variable for reading the light sensor value
volatile int pirSensorValue = 0;      // variable for reading the PIR sensor value
volatile bool ledState = LOW;         // variable to store the LED state
volatile bool motionDetected = false; // variable to indicate motion detection

void handleInterrupt() {
  // Read the state of the pushbutton value
  buttonState = digitalRead(BUTTON_PIN);

  // Read the light sensor value
  lightSensorValue = analogRead(LIGHT_SENSOR_PIN);

  // Read the PIR sensor value
  pirSensorValue = digitalRead(PIR_SENSOR_PIN);

  // Control LED according to the state of button, light sensor, and PIR sensor
  if (buttonState == LOW) {
    // If the button is pressed, toggle the LED state
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    Serial.println(ledState ? "LED turned on" : "LED turned off");

    if (ledState == HIGH && lightSensorValue > 500) {
      ledState = LOW;
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED turned on after button press in low light");
    }
  } else {
    // If the button is not pressed, maintain LED state if light is low or PIR sensor is triggered
    if ((ledState == HIGH && lightSensorValue < 500) || pirSensorValue == HIGH) {
      digitalWrite(LED_PIN, HIGH);
    }
  }
}

void motionInterrupt() {
  motionDetected = digitalRead(PIR_SENSOR_PIN) == HIGH;

  // If motion is detected, beep the buzzer and print to serial monitor
  if (motionDetected) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("Motion detected!");
    delay(1000);  // Keep the buzzer on for 1 second
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void setup() {
  // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);

  // Initialize the pushbutton pin as an input with pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize the light sensor pin as an input
  pinMode(LIGHT_SENSOR_PIN, INPUT);

  // Initialize the PIR sensor pin as an input
  pinMode(PIR_SENSOR_PIN, INPUT);

  // Initialize the buzzer pin as an output
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize serial communication
  Serial.begin(9600);

  // Attach the interrupt to the handleInterrupt function
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, CHANGE);

  // Attach the interrupt to the motionInterrupt function
  attachInterrupt(digitalPinToInterrupt(PIR_SENSOR_PIN), motionInterrupt, CHANGE);
}

void loop() {
  // The loop is empty as the functionality is handled in the interrupt service routine
}
