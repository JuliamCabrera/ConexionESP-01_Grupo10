#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <PubSubClient.h>

const char* mqttServer = "10.0.3.201";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

#define TIMEOUT 5000 // mS
SoftwareSerial mySerial(7, 6); // RX, TX
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int button = 11;
int button_state = 0;

void setup() {
    pinMode(button, INPUT);
    Serial.begin(9600);
    mySerial.begin(9600);
    EnviarComando("AT+RST", "Listo"); // Reinicia el módulo ESP8266
    delay(5000);
    EnviarComando("AT+CWMODE=1", "OK"); // Configura el modo de conexión a WiFi como estación
    EnviarComando("AT+CIFSR", "OK"); // Obtiene la dirección IP del módulo
    EnviarComando("AT+CIPMUX=1", "OK"); // Habilita múltiples conexiones
    EnviarComando("AT+CIPSERVER=1,80", "OK"); // Inicia un servidor web en el puerto 80
    lcd.begin(16, 2);
    lcd.clear();
    client.setServer(mqttServer, mqttPort);
}

void loop() {
    button_state = digitalRead(button);

    if (button_state == HIGH) {
        mySerial.println("AT+CIPSEND=0,28");
        mySerial.println("<h1>¡Se presionó el botón!</h1>");
        delay(1000);
        EnviarComando("AT+CIPCLOSE=0", "OK"); // Cierra la conexión después de enviar datos
    }

    String IncomingString = "";
    boolean StringReady = false;

    while (mySerial.available()) {
        IncomingString = mySerial.readString();
        StringReady = true;
    }

    if (StringReady) {
        Serial.println("Cadena recibida: " + IncomingString);
        lcd.setCursor(0, 0);
        lcd.print(IncomingString);
        if (client.connect("ArduinoClient")) 
        {
            client.publish("Temp", IncomingString.c_str());
        }
    }
}

boolean EnviarComando(String cmd, String ack) {
    mySerial.println(cmd); // Envía el comando "AT+" al módulo
    return buscarRespuesta(ack);
}

boolean buscarRespuesta(String keyword) {
    byte current_char = 0;
    byte keyword_length = keyword.length();
    long deadline = millis() + TIMEOUT;
    while (millis() < deadline) {
        if (mySerial.available()) {
            char ch = mySerial.read();
            Serial.write(ch);
            if (ch == keyword[current_char]) {
                if (++current_char == keyword_length) {
                    Serial.println();
                    return true;
                }
            }
        }
    }
    return false; // Tiempo de espera agotado
}
