#include <ESP8266WiFi.h>
#include <espnow.h>
#include <DHT.h>

// DHT setup
#define DHTPIN 4 // gpiopin2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    char a[32];
    float temperature;
    float humidity;
    float oxygenContent;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Variable to store the master MAC address
uint8_t masterAddress[] = {0x08, 0xB6, 0x1F, 0x34, 0xC4, 0x10}; // Replace with your master's MAC address

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(sendStatus == 0 ? "Delivery Success" : "Delivery Fail");
}

// Dummy function to calculate oxygen content
float calculateOxygenContent(float temperature, float humidity) {
    // Replace with actual calculation
    return 20.95 + 0.01 * (temperature + humidity); // Example calculation
}

void setup() {
    // Initialize Serial Monitor
    Serial.begin(115200);

    // Initialize DHT sensor
    dht.begin();

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register send callback
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb(OnDataSent);

    // Register peer
    esp_now_add_peer(masterAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

    // Prepare data to send
    strcpy(myData.a, "Hello Master");
}

void loop() {
    // Read DHT11 sensor data
    myData.temperature = dht.readTemperature();
    myData.humidity = dht.readHumidity();
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(myData.temperature) || isnan(myData.humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Calculate oxygen content
    myData.oxygenContent = calculateOxygenContent(myData.temperature, myData.humidity);

    // Send message via ESP-NOW
    uint8_t result = esp_now_send(masterAddress, (uint8_t *) &myData, sizeof(myData));
    
    if (result == 0) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }

    // Print data to Serial Monitor
    Serial.print("Temperature: ");
    Serial.println(myData.temperature);
    Serial.print("Humidity: ");
    Serial.println(myData.humidity);
    Serial.print("Oxygen Content: ");
    Serial.println(myData.oxygenContent);
    
    // Wait for a while before sending again
    delay(2000);
}
