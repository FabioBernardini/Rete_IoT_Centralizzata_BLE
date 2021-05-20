/*
  Fabio Bernardini

  Progetto 3: nodo BLE - variante con singola temperatura
  

*/

#include <ArduinoBLE.h>


// Salvo in una variabile lo UUID per il servizio

#define UUIDServizio "cbf6e4aa-5656-11eb-ae93-0242ac130002"

// Salvo in una variabile lo UUID per la caratteristica del servizio di temperatura

#define UUIDCaratteristicaTemperatura "9dcfe490-5657-11eb-ae93-0242ac130002"

// Salvo in una variabile lo UUID per la caratteristica del servizio di richieste da parte del nodo centrale

#define UUIDCaratteristicaRichieste "1aa17986-5728-11eb-ae93-0242ac130002"


// Creo il servizio BLE per la temperatura

BLEService servizioTemperatura(UUIDServizio);

// Aggiungo una caratteristica al servizio, il dato della temperatura stessa
// Specifichiamo anche che i client che si connettono, come il nostro nodo centrale, possono leggerne i dati


BLEStringCharacteristic caratteristicaTemperatura(UUIDCaratteristicaTemperatura, BLERead | BLENotify, 64);

BLEStringCharacteristic caratteristicaRichiestaCentrale(UUIDCaratteristicaRichieste, BLERead | BLEWrite, 64);


void setup() {
  // Imposto la comunicazione seriale per il BLE
  
  Serial.begin(9600);    

  // Attendo finché non è impostata
  while (!Serial);

  Serial.println("Comunicazione seriale disponibile.");

  

  // Imposto il LED saldato sulla scheda come output: ci serve come "segnale hardware" per capire
  // Quando il nodo centrale si connette alla scheda

  pinMode(LED_BUILTIN, OUTPUT);

  // Inizializzo la connettività BLE

  BLE.begin();
  

  // Imposto il nome del nodo
  BLE.setLocalName("NodoArduinoNano");

  // Annuncio la disponibilità dell servizio
  BLE.setAdvertisedService(servizioTemperatura);

  // Aggiungo le caratteristiche al servizio
  
  servizioTemperatura.addCharacteristic(caratteristicaTemperatura);

  servizioTemperatura.addCharacteristic(caratteristicaRichiestaCentrale);

  // Aggiungo il servizio
  BLE.addService(servizioTemperatura);

  // Impostiamo la temperatura iniziale
  caratteristicaTemperatura.setValue("22");

  // E il valore per le richieste da parte del centrale

  caratteristicaRichiestaCentrale.setValue("0");
  
  // Inizia l'annunciazione del servizio
  BLE.advertise();

  Serial.print("NodoArduinoNano disponibile. Indirizzo MAC: ");
  Serial.println(BLE.address());


  

  //Terminata l'impostazione...
  Serial.println("Attendo connessioni...");
  
  
}


void loop() {

    // Variabile usata per verificare connessioni alla scheda
    BLEDevice centrale = BLE.central();

    // Se il nodo centrale è connesso

    while (centrale.connected()){
      
      // Iniziamo a inviare i dati richiesti
      
      Serial.print("Connessione avvenuta. MAC del nodo centrale: ");
      Serial.println(centrale.address());

      // Illuminiamo il LED per mostrare la connessione in corso

      digitalWrite(LED_BUILTIN, HIGH);
      
      // se il centrale ha ordinato la richiesta temperatura, procediamo
  
      if (caratteristicaRichiestaCentrale.written()){
        Serial.println("Il nodo centrale ha scritto nella caratteristica");
        if (caratteristicaRichiestaCentrale.value() == "1"){
            delay(1000);

            String temperaturaRilevata = String(random(20,30));

            Serial.print("Temperatura rilevata: ");
            Serial.println(temperaturaRilevata);
      
            // impostiamo il valore della temperatura
      
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
