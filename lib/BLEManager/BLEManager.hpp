#if !defined(BLE_MANAGER_H_)
#define BLE_MANAGER_H_

#include <ArduinoBLE.h>

class BLEManager
{
private:
    BLEService customService;
    BLEStringCharacteristic sendCharacteristic;
    BLEStringCharacteristic receiveCharacteristic;

public:
    BLEManager() : customService("19b10000-e8f2-537e-4f6c-d104768a1214"),
                   sendCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1214", BLEWrite | BLENotify, 20),
                   receiveCharacteristic("19b10002-e8f2-537e-4f6c-d104768a1214", BLEWrite | BLENotify, 20) {}

    void setup()
    {
        // Start BLE communication
        if (!BLE.begin())
        {
            Serial.println("Failed to initialize BLE!");
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

    void sendData(float x, float y, float z)
    {
        String data = String(x) + "," + String(y) + "," + String(z);
        sendCharacteristic.writeValue(data);
    }

    BLEDevice central() { return BLE.central(); }
};

#endif // BLE_MANAGER_H_
