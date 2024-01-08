#include <ArduinoBLE.h>
#include <Arduino.h>
#include <Arduino_BMI270_BMM150.h>

BLEService customService("19b10000-e8f2-537e-4f6c-d104768a1214");                                            
BLEStringCharacteristic sendCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1214", BLEWrite | BLENotify, 20);    
BLEStringCharacteristic receiveCharacteristic("19b10002-e8f2-537e-4f6c-d104768a1214", BLEWrite | BLENotify, 20);

float acc_x, acc_y, acc_z;

void check_steps()
{
    if (IMU.accelerationAvailable())
    {
        IMU.readAcceleration(acc_x, acc_y, acc_z);
    }
}

void setup()
{
    Serial.begin(9600);

    // Start BLE communication
    if (!BLE.begin())
    {
        Serial.println("Failed to initialize BLE!");
        Serial.flush();

        while (1)
            ;
    }

    if (!IMU.begin())
    {
        Serial.println("Failed to initialize IMU!");
        Serial.flush();

        while (1)
            ;
    }

    // Set the local name and advertising parameters
    BLE.setLocalName("IoT Data Collector");
    BLE.setAdvertisedService(customService);

    // Add the characteristics to the service
    customService.addCharacteristic(sendCharacteristic);
    customService.addCharacteristic(receiveCharacteristic);

    // Add the service
    BLE.addService(customService);

    // Start advertising
    BLE.advertise();
    Serial.println("Bluetooth device active, waiting for connections...");
    Serial.flush();
}

void loop()
{
    // Wait for a BLE central to connect
    BLEDevice central = BLE.central();

    // If a central is connected
    if (central)
    {
        Serial.print("Connected to central: ");
        Serial.println(central.address());

        // While the central is still connected
        while (central.connected())
        {
            // Check for incoming data
            if (receiveCharacteristic.written())
            {
                String receivedData = receiveCharacteristic.value();
                Serial.println("Received data: " + receivedData);

                // Process the received data (you can add your own logic here)

                // Clear the receive characteristic
                receiveCharacteristic.writeValue("");
            }

            // Send data
            String data = String(acc_x) + "," + String(acc_y) + "," + String(acc_z);
            sendCharacteristic.writeValue(data);
            Serial.println("Sending data: " + data);

            // Wait
            delay(100);
        }

        // When the central disconnects, print a message
        Serial.print("Disconnected from central: ");
        Serial.println(central.address());
    }
}
