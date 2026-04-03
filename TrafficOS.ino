<<<<<<< HEAD
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* WIFI_SSID     = "Joshua-2.4G";
const char* WIFI_PASSWORD = "joshua762002";

const char* SUPABASE_URL = "https://vszeihhsgzmqadnrwckq.supabase.co";
const char* SUPABASE_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InZzemVpaGhzZ3ptcWFkbnJ3Y2txIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NzQ0MzM0OTYsImV4cCI6MjA5MDAwOTQ5Nn0.w3u5N-uRh6YZeLddDYRM1WJ2Ij-ds_Y-tDTBvlzPn40";

#define RED_PIN    13
#define YELLOW_PIN 12
#define GREEN_PIN  14

#define POLL_INTERVAL 2000

bool lastRed    = false;
bool lastYellow = false;
bool lastGreen  = false;

unsigned long lastPollTime = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(RED_PIN,    OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN,  OUTPUT);
  digitalWrite(RED_PIN,    LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN,  LOW);

  connectToWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Disconnected! Reconnecting...");
    connectToWiFi();
    return;
  }

  unsigned long now = millis();
  if (now - lastPollTime >= POLL_INTERVAL) {
    lastPollTime = now;
    fetchTrafficState();
  }
}

void connectToWiFi() {
  Serial.print("[WiFi] Connecting to: ");
  Serial.println(WIFI_SSID);
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  delay(500);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Connected!");
    Serial.print("[WiFi] IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n[WiFi] Failed. Will retry...");
  }
}

// NEW: Insert a default row if it doesn't exist
void createDefaultRow() {
  HTTPClient http;
  String endpoint = String(SUPABASE_URL) + "/rest/v1/traffic_light_control";
  http.begin(endpoint);
  http.addHeader("apikey", SUPABASE_KEY);
  http.addHeader("Authorization", String("Bearer ") + SUPABASE_KEY);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Prefer", "return=minimal");

  // JSON body with id=1 and all required columns (others use table defaults)
  String body = "{\"id\":1,\"red\":false,\"yellow\":false,\"green\":false,\"mode\":\"manual\"}";

  int code = http.POST(body);
  if (code == 201 || code == 409) {   // 201 Created, 409 Conflict (already exists)
    Serial.println("[Supabase] Default row inserted (or already present).");
  } else {
    Serial.printf("[Supabase] Insert failed, HTTP %d\n", code);
    String resp = http.getString();
    Serial.println("[Supabase] Response: " + resp);
  }
  http.end();
}

void fetchTrafficState() {
  HTTPClient http;

  String endpoint = String(SUPABASE_URL) +
    "/rest/v1/traffic_light_control?id=eq.1&select=red,yellow,green";

  http.begin(endpoint);
  http.addHeader("apikey",        SUPABASE_KEY);
  http.addHeader("Authorization", String("Bearer ") + SUPABASE_KEY);
  http.addHeader("Content-Type",  "application/json");

  int code = http.GET();

  if (code == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.print("[Supabase] ");
    Serial.println(payload);

    // If no row exists, create one
    if (payload == "[]") {
      Serial.println("[Supabase] No row with id=1 found. Creating default row...");
      createDefaultRow();
      http.end();
      return;   // Will retry on next poll
    }

    DynamicJsonDocument doc(512);
    DeserializationError err = deserializeJson(doc, payload);

    if (!err && doc.size() > 0) {
      bool red    = doc[0]["red"]    | false;
      bool yellow = doc[0]["yellow"] | false;
      bool green  = doc[0]["green"]  | false;

      if (red != lastRed || yellow != lastYellow || green != lastGreen) {
        lastRed    = red;
        lastYellow = yellow;
        lastGreen  = green;

        digitalWrite(RED_PIN,    red    ? HIGH : LOW);
        digitalWrite(YELLOW_PIN, yellow ? HIGH : LOW);
        digitalWrite(GREEN_PIN,  green  ? HIGH : LOW);

        Serial.printf("[LIGHTS] 🔴%s 🟡%s 🟢%s\n",
          red    ? "ON" : "OFF",
          yellow ? "ON" : "OFF",
          green  ? "ON" : "OFF"
        );
      } else {
        Serial.println("[LIGHTS] No change.");
      }
    } else {
      Serial.println("[JSON] Parse error or unexpected response!");
    }

  } else {
    Serial.print("[HTTP] Error: ");
    Serial.println(code);
  }

  http.end();
=======
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* WIFI_SSID     = "Joshua-2.4G";
const char* WIFI_PASSWORD = "joshua762002";

const char* SUPABASE_URL = "https://vszeihhsgzmqadnrwckq.supabase.co";
const char* SUPABASE_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InZzemVpaGhzZ3ptcWFkbnJ3Y2txIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NzQ0MzM0OTYsImV4cCI6MjA5MDAwOTQ5Nn0.w3u5N-uRh6YZeLddDYRM1WJ2Ij-ds_Y-tDTBvlzPn40";

#define RED_PIN    13
#define YELLOW_PIN 12
#define GREEN_PIN  14

#define POLL_INTERVAL 2000

bool lastRed    = false;
bool lastYellow = false;
bool lastGreen  = false;

unsigned long lastPollTime = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(RED_PIN,    OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN,  OUTPUT);
  digitalWrite(RED_PIN,    LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN,  LOW);

  connectToWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Disconnected! Reconnecting...");
    connectToWiFi();
    return;
  }

  unsigned long now = millis();
  if (now - lastPollTime >= POLL_INTERVAL) {
    lastPollTime = now;
    fetchTrafficState();
  }
}

void connectToWiFi() {
  Serial.print("[WiFi] Connecting to: ");
  Serial.println(WIFI_SSID);
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  delay(500);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Connected!");
    Serial.print("[WiFi] IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n[WiFi] Failed. Will retry...");
  }
}

// NEW: Insert a default row if it doesn't exist
void createDefaultRow() {
  HTTPClient http;
  String endpoint = String(SUPABASE_URL) + "/rest/v1/traffic_light_control";
  http.begin(endpoint);
  http.addHeader("apikey", SUPABASE_KEY);
  http.addHeader("Authorization", String("Bearer ") + SUPABASE_KEY);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Prefer", "return=minimal");

  // JSON body with id=1 and all required columns (others use table defaults)
  String body = "{\"id\":1,\"red\":false,\"yellow\":false,\"green\":false,\"mode\":\"manual\"}";

  int code = http.POST(body);
  if (code == 201 || code == 409) {   // 201 Created, 409 Conflict (already exists)
    Serial.println("[Supabase] Default row inserted (or already present).");
  } else {
    Serial.printf("[Supabase] Insert failed, HTTP %d\n", code);
    String resp = http.getString();
    Serial.println("[Supabase] Response: " + resp);
  }
  http.end();
}

void fetchTrafficState() {
  HTTPClient http;

  String endpoint = String(SUPABASE_URL) +
    "/rest/v1/traffic_light_control?id=eq.1&select=red,yellow,green";

  http.begin(endpoint);
  http.addHeader("apikey",        SUPABASE_KEY);
  http.addHeader("Authorization", String("Bearer ") + SUPABASE_KEY);
  http.addHeader("Content-Type",  "application/json");

  int code = http.GET();

  if (code == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.print("[Supabase] ");
    Serial.println(payload);

    // If no row exists, create one
    if (payload == "[]") {
      Serial.println("[Supabase] No row with id=1 found. Creating default row...");
      createDefaultRow();
      http.end();
      return;   // Will retry on next poll
    }

    DynamicJsonDocument doc(512);
    DeserializationError err = deserializeJson(doc, payload);

    if (!err && doc.size() > 0) {
      bool red    = doc[0]["red"]    | false;
      bool yellow = doc[0]["yellow"] | false;
      bool green  = doc[0]["green"]  | false;

      if (red != lastRed || yellow != lastYellow || green != lastGreen) {
        lastRed    = red;
        lastYellow = yellow;
        lastGreen  = green;

        digitalWrite(RED_PIN,    red    ? HIGH : LOW);
        digitalWrite(YELLOW_PIN, yellow ? HIGH : LOW);
        digitalWrite(GREEN_PIN,  green  ? HIGH : LOW);

        Serial.printf("[LIGHTS] 🔴%s 🟡%s 🟢%s\n",
          red    ? "ON" : "OFF",
          yellow ? "ON" : "OFF",
          green  ? "ON" : "OFF"
        );
      } else {
        Serial.println("[LIGHTS] No change.");
      }
    } else {
      Serial.println("[JSON] Parse error or unexpected response!");
    }

  } else {
    Serial.print("[HTTP] Error: ");
    Serial.println(code);
  }

  http.end();
>>>>>>> 56793a2bb8d93dc23abb06274b0d0800106fab7a
}