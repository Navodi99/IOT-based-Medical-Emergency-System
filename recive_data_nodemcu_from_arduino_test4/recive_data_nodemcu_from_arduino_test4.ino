//GPS
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//firebase
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Tharindu’s iPhone"
#define WIFI_PASSWORD "12345678tttt"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyAguWLrA9lFx0qSn3TdcPBTzVDRl7E7Dp0"

/* 3. Define the RTDB URL */
#define DATABASE_URL "test-1384f-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "navodil99@gmail.com"
#define USER_PASSWORD "12345678@"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;


//GPS
TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(4, 5); // The serial connection to the GPS device
//SoftwareSerial ss(3, 4);//rx tx
float latitude , longitude;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
int pm;

String inputString = "";
void setup() {
// Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }

  //Firebase
  Serial.println();
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Or use legacy authenticate method
  // config.database_url = DATABASE_URL;
  // config.signer.tokens.legacy_token = "<database secret>";

  // To connect without auth in Test Mode, see Authentications/TestMode/TestMode.ino

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);ss.begin(9600);
}
//void loop() { // run over and over
//  if (Serial.available()) {
//    Serial.write(Serial.read());
//  }
//}

  

void loop() {
        //firebase
        if (Firebase.ready() && !taskCompleted){
        taskCompleted = true;
        Serial.printf("Get shallow data... %s\n", Firebase.getShallowData(fbdo, "/") ? "ok" : fbdo.errorReason().c_str());
    
        if (fbdo.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
          printResult(fbdo); // see addons/RTDBHelper.h
          FirebaseJson updateData;
          FirebaseJson json;
          json.set("_data2","_value2");   

         //get
          while (Serial.available()) {
            char c = Serial.read();  // Read a character from the serial input
            // Accumulate characters in the input string
            inputString += c;
              // Check for a newline or carriage return, indicating the end of a value
              if (c == '\n' || c == '\r') {
                // Split the string based on the comma delimiter
                int commaIndex = inputString.indexOf(',');
                if (commaIndex != -1) {
                  String label = inputString.substring(0, commaIndex);  // Extract the label (e.g., 'T' or 'H')
                  String value = inputString.substring(commaIndex + 1);  // Extract the value (e.g., '46.36' or '154')
                  // Print the label and value separately
                  Serial.print("Label: ");
                  Serial.println(label);
                  Serial.print("Value: ");
                  Serial.println(value);
                  if(String(label)==String("H")) {
                    Serial.println("ok1");
                    updateData.set("heartRate",value);
                    if (Firebase.updateNode(fbdo, "/update", updateData)) {
                      Serial.println(fbdo.dataPath());
                      Serial.println(fbdo.dataType());                    
                      Serial.println(fbdo.jsonString());       
                    } else {
                      Serial.println(fbdo.errorReason());
                    }
                  }
                  if(String(label)==String("T")) {
                    Serial.println("ok2");
                    updateData.set("temperature",value);
                    if (Firebase.updateNode(fbdo, "/update", updateData)) {
                      Serial.println(fbdo.dataPath());
                      Serial.println(fbdo.dataType());
                      Serial.println(fbdo.jsonString()); 
                    } else {
                      Serial.println(fbdo.errorReason());
                    }
                  }
                  while (ss.available() > 0)
                  if (gps.encode(ss.read())){
                    if (gps.location.isValid()){
                      latitude = gps.location.lat();
                      lat_str = String(latitude , 6);
                      longitude = gps.location.lng();
                      lng_str = String(longitude , 6);
                      Serial.print("longitude:");
                      Serial.println(longitude);
                      updateData.set("longitude",longitude);
                    if (Firebase.updateNode(fbdo, "/update", updateData)) {
                      Serial.println(fbdo.dataPath());
                      Serial.println(fbdo.dataType());
                      Serial.println(fbdo.jsonString()); 
                    } else {
                      Serial.println(fbdo.errorReason());
                    }
                      Serial.print("latitude:");
                      Serial.println(latitude);
                      updateData.set("latitude",latitude);
                    if (Firebase.updateNode(fbdo, "/update", updateData)) {
                      Serial.println(fbdo.dataPath());
                      Serial.println(fbdo.dataType());
                      Serial.println(fbdo.jsonString()); 
                    } else {
                      Serial.println(fbdo.errorReason());
                    }
                    }
                  }
                  delay(100);
                inputString = "";  // Reset the input string for the next input   
              }
            } 
      }
  }
}
