/*
  BasicDemo.ino - Non-blocking demo using millis() for the LMD18200 motor driver.
  This example ramps the motor speed up and down, applies a brake,
  and continuously monitors motor current and thermal status with CSV serial feedback.
  Type "stop" (followed by ENTER) on the Serial monitor at any time to trigger an emergency stop.
*/

#include <LMD18200.h>

// Pin definitions
const uint8_t MOTOR_PWM_PIN = 6;
const uint8_t MOTOR_DIR_PIN = 7;
const uint8_t MOTOR_BRAKE_PIN = 8;
const uint8_t MOTOR_CSENSE_PIN = A6;
const uint8_t MOTOR_TFLAG_PIN = 9;

const float RESISTOR_VALUE = 4420; // 4.42k ohm resistor for current sense

// Instantiate the motor driver
LMD18200 motor(MOTOR_PWM_PIN, MOTOR_DIR_PIN, MOTOR_BRAKE_PIN, MOTOR_CSENSE_PIN, MOTOR_TFLAG_PIN);

// Define demo states (MONITOR state removed for continuous sensing)
enum DemoState
{
    RAMP_UP,
    RAMP_DOWN,
    BRAKE
};

DemoState state = RAMP_UP;
unsigned long stateStartTime = 0;
unsigned long lastUpdateTime = 0;
unsigned long lastFeedbackTime = 0;
int currentSpeed = 0;

const int rampStep = 5;                     // Speed increment/decrement
const unsigned long rampInterval = 200;     // 50 ms between speed updates
const unsigned long brakeDuration = 5000;   // Brake duration (2000 ms)
const unsigned long feedbackInterval = 100; // Feedback every 200 ms

// Check for emergency stop command from Serial.
void checkSerialCommand()
{
    if (Serial.available() > 0)
    {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.equalsIgnoreCase("stop"))
        {
            Serial.println("E-STOP activated. Shutting down motor.");
            motor.brake();
            // Enter an infinite loop (cannot be exited)
            while (true)
            {
                delay(1000);
            }
        }
    }
}

// Helper to convert state enum to a string.
String stateToString(DemoState s)
{
    switch (s)
    {
    case RAMP_UP:
        return "RAMP_UP";
    case RAMP_DOWN:
        return "RAMP_DOWN";
    case BRAKE:
        return "BRAKE";
    default:
        return "UNKNOWN";
    }
}

void setup()
{
    Serial.begin(115200);
    motor.begin();
    Serial.println("LMD18200 Basic Demo - Non Blocking");
    stateStartTime = millis();
    lastUpdateTime = millis();
    lastFeedbackTime = millis();
}

void loop()
{
    unsigned long currentTime = millis();
    checkSerialCommand();

    // State machine for motor control
    switch (state)
    {
    case RAMP_UP:
        if (currentTime - lastUpdateTime >= rampInterval)
        {
            currentSpeed += rampStep;
            if (currentSpeed >= 255)
            {
                currentSpeed = 255;
                state = RAMP_DOWN;
                stateStartTime = currentTime;
            }
            motor.write(currentSpeed);
            lastUpdateTime = currentTime;
        }
        break;

    case RAMP_DOWN:
        if (currentTime - lastUpdateTime >= rampInterval)
        {
            currentSpeed -= rampStep;
            if (currentSpeed <= 0)
            {
                currentSpeed = 0;
                state = BRAKE;
                stateStartTime = currentTime;
                motor.brake();
            }
            else
            {
                motor.write(currentSpeed);
            }
            lastUpdateTime = currentTime;
        }
        break;

    case BRAKE:
        if (currentTime - stateStartTime >= brakeDuration)
        {
            motor.releaseBrake();
            state = RAMP_UP;
            currentSpeed = 0;
            lastUpdateTime = currentTime;
            stateStartTime = currentTime;
        }
        break;
    }

    // Print CSV-style feedback every feedbackInterval
    if (currentTime - lastFeedbackTime >= feedbackInterval)
    {
        float currentReading = motor.readCurrent(RESISTOR_VALUE);
        String thermalStatus = motor.isThermalWarning() ? "active" : "normal";
        Serial.print("t:");
        Serial.print(currentTime);
        Serial.print(", state:");
        Serial.print(stateToString(state));
        Serial.print(", speed:");
        Serial.print(currentSpeed);
        Serial.print(", current:");
        Serial.print(currentReading, 3);
        Serial.print(", thermal:");
        Serial.println(thermalStatus);
        lastFeedbackTime = currentTime;
    }
}
