#include <ArduinoBLE.h>

#define UUIDServizio "cbf6e4aa-5656-11eb-ae93-0242ac130002"

#define UUIDCaratteristicaTemperatura "9dcfe490-5657-11eb-ae93-0242ac130002"

#define UUIDCaratteristicaRichieste "1aa17986-5728-11eb-ae93-0242ac130002"

BLEService servizioTemperatura(UUIDServizio);

BLEStringCharacteristic caratteristicaTemperatura(UUIDCaratteristicaTemperatura, BLERead | BLENotify, 64);

BLEStringCharacteristic caratteristicaRichiestaCentrale(UUIDCaratteristicaRichieste, BLERead | BLEWrite, 64);


void setup() {
  
  Serial.begin(9600);    

  while (!Serial);

  Serial.println("Comunicazione seriale disponibile.");

  pinMode(LED_BUILTIN, OUTPUT);

  BLE.begin();
  
  BLE.setLocalName("NodoArduinoNano");

  BLE.setAdvertisedService(servizioTemperatura);
  
  servizioTemperatura.addCharacteristic(caratteristicaTemperatura);

  servizioTemperatura.addCharacteristic(caratteristicaRichiestaCentrale);

  BLE.addService(servizioTemperatura);

  caratteristicaTemperatura.setValue("22");

  caratteristicaRichiestaCentrale.setValue("0");
  
  BLE.advertise();

  Serial.print("NodoArduinoNano disponibile. Indirizzo MAC: ");
  Serial.println(BLE.address());
  Serial.println("Attendo connessioni...");
  
  
}


void loop() {
  
    BLEDevice centrale = BLE.central();

    while (centrale.connected()){
      
      Serial.print("Connessione avvenuta. MAC del nodo centrale: ");
      Serial.println(centrale.address());

      digitalWrite(LED_BUILTIN, HIGH);
  
      if (caratteristicaRichiestaCentrale.written()){
        Serial.println("Il nodo centrale ha scritto nella caratteristica");
        if (caratteristicaRichiestaCentrale.value() == "1"){
            delay(1000);

            String temperaturaRilevata = String(random(20,30));

            Serial.print("Temperatura rilevata: ");
            Serial.println(temperaturaRilevata);
      
            caratteristicaTemperatura.writeValue(temperaturaRilevata);

            delay(1000);
        }
      }
   
      delay(2000);

      
    }

    

        digitalWrite(LED_BUILTIN, LOW);
    

      // Mettiamo in attesa
      delay(2000);

}
