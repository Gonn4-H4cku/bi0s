#include <WiFi.h>
#include <esp_now.h>
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    float temperature;
    float humidity;
    float oxygenContent;
} struct_message;

// Create a struct_message called incomingReadings
struct_message incomingReadings;

// Callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));

    // Print MAC Address of the sender
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", 
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    // Print the data received
    Serial.print("From ");
    Serial.print(macStr);
    Serial.print(" : ");
    Serial.print("Message: ");
    Serial.print(incomingReadings.a);
    Serial.print(" | Temperature: ");
    Serial.print(incomingReadings.temperature);
    Serial.print(" | Humidity: ");
    Serial.print(incomingReadings.humidity);
    Serial.print(" | Oxygen Content: ");
    Serial.println(incomingReadings.oxygenContent);
}

void setup() {
    // Initialize Serial Monitor
    Serial.begin(115200);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register receive callback
    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
    // No need to add code to loop here for this example
}
