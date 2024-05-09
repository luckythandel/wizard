//Include the libraries
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//#include <NRFLite.h>
#define IR D8
#define PAGE "<!DOCTYPE html> <html lang=\"en\"> <head>   <meta charset=\"UTF-8\">   <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">   <title>Control Panel</title>   <style>     body {       font-family: Arial, sans-serif;       margin: 0;       padding: 0;       display: flex;       justify-content: center;       align-items: center;       height: 100vh;       background-color: #f0f0f0;     }     .button {       display: inline-block;       padding: 10px 20px;       font-size: 18px;       cursor: pointer;       margin: 10px;       border: none;       border-radius: 5px;       background-color: #4CAF50;       color: white;       text-align: center;       text-decoration: none;       transition: background-color 0.3s;     }     .button:hover {       background-color: #45a049;     }   </style> </head> <body>   <div>     <button class=\"button\" id=\"frontBtn\">Front</button>     <button class=\"button\" id=\"backBtn\">Back</button>     <button class=\"button\" id=\"leftBtn\">Left</button>     <button class=\"button\" id=\"rightBtn\">Right</button>     <button class=\"button\" id=\"irOnBtn\">IR ON</button>     <button class=\"button\" id=\"irOffBtn\">IR OFF</button>   </div>    <script>     function sendCommand(command) {       fetch('http://192.168.1.1/' + command)         .then(response => {           if (!response.ok) {             throw new Error('Network response was not ok');           }           console.log('Command sent successfully');         })         .catch(error => {           console.error('There was a problem with the fetch operation:', error);         });     }          document.getElementById('frontBtn').addEventListener('click', function() {       sendCommand('front');     });      document.getElementById('backBtn').addEventListener('click', function() {       sendCommand('back');     });      document.getElementById('leftBtn').addEventListener('click', function() {       sendCommand('left');     });      document.getElementById('rightBtn').addEventListener('click', function() {       sendCommand('right');     });      document.getElementById('irOnBtn').addEventListener('click', function() {       sendCommand('ir_on');     });      document.getElementById('irOffBtn').addEventListener('click', function() {       sendCommand('ir_off');     });   </script> </body> </html> "
//Initializing the variables
boolean x = 0;
boolean IRPin = 1;
/* Put your SSID & Password */
const char* ssid = "wizard";  // Enter SSID here
const char* password = "12345678";  //Enter Password here
ESP8266WebServer server(80);
/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

int directionOfMovement = 0;
int MotorA__INA1 = D1;
int MotorA__INA2 = D2;
int MotorA__INA3 = D3;
int MotorA__INA4 = D4;

String message;
const static uint8_t RADIO_ID = 0;       // Our radio's id.  The transmitter will send to this id.
const static uint8_t PIN_RADIO_CE = 7;
const static uint8_t PIN_RADIO_CSN = 8;

struct RadioPacket { // Any packet up to 32 bytes can be sent.
  uint8_t FromRadioId;
  uint32_t Data;
  uint32_t FailedTxCount;

};

//Create NRF object
//NRFLite _radio;
//RadioPacket _radioData;

void setup() {

  Serial.begin(115200);
  //Set the pin modes
  pinMode(MotorA__INA1, OUTPUT);
  pinMode(MotorA__INA2, OUTPUT);
  pinMode(MotorA__INA3, OUTPUT);
  pinMode(MotorA__INA4, OUTPUT);
  pinMode(IR, INPUT);

 WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/front", front);
  server.on("/back", back);
  server.on("/left", left);
  server.on("/right", right);
  server.on("/ir_on", ir_on);
  server.on("/ir_off", ir_off);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");

//   if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)) {

//     Serial.println("Cannot communicate with radio");
//     while (1); // Wait here forever.

//   }
  initalStage();
}

void handle_OnConnect() {
  server.send(200, "text/html", PAGE); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}


void initalStage()
{
  digitalWrite(MotorA__INA1, LOW);
  digitalWrite(MotorA__INA2, LOW);
  digitalWrite(MotorA__INA3, LOW);
  digitalWrite(MotorA__INA4, LOW);

}

void ir_on()
{
  IRPin=1;
}

void ir_off()
{
  IRPin=0;
}

void IR_Motor()
{
  int val = digitalRead(IR);
  Serial.print(val);
  if(val == 0)
  {
    right();
    delay(1500);
  }
  front();
}

void loop() {
  if(IRPin){
    IR_Motor();
  }
  server.handleClient();
  
  // while (_radio.hasData()) {

  //   _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name.

  //   message = _radioData.Data;
  //   Serial.println(message);
  //   directionOfMovement = message.toInt();
  //   moveAccordingly();

  // }
   
}


void front() {
  
  digitalWrite(MotorA__INA1, HIGH);
  digitalWrite(MotorA__INA2, LOW);
  digitalWrite(MotorA__INA3, HIGH);
  digitalWrite(MotorA__INA4, LOW);

}

void back() {

  digitalWrite(MotorA__INA1, LOW);
  digitalWrite(MotorA__INA2, HIGH);
  digitalWrite(MotorA__INA3, LOW);
  digitalWrite(MotorA__INA4, HIGH);

}

void left() {

  digitalWrite(MotorA__INA1, HIGH);
  digitalWrite(MotorA__INA2, LOW);
  digitalWrite(MotorA__INA3, LOW);
  digitalWrite(MotorA__INA4, HIGH);

}
void right() {

  digitalWrite(MotorA__INA1, LOW);
  digitalWrite(MotorA__INA2, HIGH);
  digitalWrite(MotorA__INA3, HIGH);
  digitalWrite(MotorA__INA4, LOW);

}
