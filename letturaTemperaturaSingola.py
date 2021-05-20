""" 
    Progetto 3 Raspberry Pi

    Variante temperautra singola

    Lorenzo Neri
"""


# importo i pacchetti necessari al funzionamento
from bluepy import btle

import uuid
import time

import binascii

# inizializzo la connessione.
print ("Connessione in corso")

# creo una variabile per gestire la periferica, imposto l'indirizzo MAC
nodoTemperatura = btle.Peripheral("8d:b5:d2:06:1c:00",btle.ADDR_TYPE_PUBLIC)

time.sleep(0.5)

# ottengo la lista dei servizi disponibili
serviziNodo = nodoTemperatura.getServices()

servizioNodoTemperatura = None

# ciclo tutti i servizi presenti nella periferica a cui sono connesso
for servizio in serviziNodo:
    # se lo uuid coincide con quello impostato nel codice Arduino, allora è il servizio che ci interessa
    if servizio.uuid == "cbf6e4aa-5656-11eb-ae93-0242ac130002":
        servizioNodoTemperatura = servizio



print(servizioNodoTemperatura)

# stesso processo per le caratteristiche
caratteristicheServizio = servizioNodoTemperatura.getCharacteristics()

caratteristicaRichieste = None
caratteristicaTemperatura = None

# ciclo tutte le caratteristiche a mia disposizione
for caratteristica in caratteristicheServizio:
    print(caratteristica.uuid)
    # se gli uuid coincidono, assegno alle due variabili le due caratteristiche
    if str(caratteristica.uuid) == "1aa17986-5728-11eb-ae93-0242ac130002":
        print("caratteristica richieste presa")
        caratteristicaRichieste = caratteristica
    elif str(caratteristica.uuid) == "9dcfe490-5657-11eb-ae93-0242ac130002":
        caratteristicaTemperatura = caratteristica


# provo a scrivere sulla caratteristica "richieste" e ad ottenere la temperatura
try:
    print("scrivo")
    
    # scrivo il valore "1" sulla caratteristica "richieste"
    caratteristicaRichieste.write(val="1".encode("utf-8"), withResponse=True)
except Exception as error:
    # se ci fosse un errore, lo stampo
    print("errore in fase di scrittura sulla caratteristica: "+error)
    

time.sleep(2)

# ottengo il valore di temperatura proveniente dalla periferica, lo decodifico
print("questo è ciò che leggo: "+caratteristicaTemperatura.read().decode("utf-8"))

# attendo due secondi, dopodiché pratico la disconnessione
time.sleep(2)
nodoTemperatura.disconnect()