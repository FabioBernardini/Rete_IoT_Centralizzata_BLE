from bluepy import btle

import uuid
import time

import binascii

print ("Connessione in corso")

nodoTemperatura = btle.Peripheral("8d:b5:d2:06:1c:00",btle.ADDR_TYPE_PUBLIC)

time.sleep(0.5)

serviziNodo = nodoTemperatura.getServices()

servizioNodoTemperatura = None

for servizio in serviziNodo:
    
    if servizio.uuid == "cbf6e4aa-5656-11eb-ae93-0242ac130002":
        servizioNodoTemperatura = servizio



print(servizioNodoTemperatura)

caratteristicheServizio = servizioNodoTemperatura.getCharacteristics()

caratteristicaRichieste = None
caratteristicaTemperatura = None


for caratteristica in caratteristicheServizio:
    print(caratteristica.uuid)

    if str(caratteristica.uuid) == "1aa17986-5728-11eb-ae93-0242ac130002":
        print("caratteristica richieste presa")
        caratteristicaRichieste = caratteristica
    elif str(caratteristica.uuid) == "9dcfe490-5657-11eb-ae93-0242ac130002":
        caratteristicaTemperatura = caratteristica

try:
    print("scrivo")
    
    caratteristicaRichieste.write(val="1".encode("utf-8"), withResponse=True)
except Exception as error:
    
    print("errore in fase di scrittura sulla caratteristica: "+error)
    

time.sleep(2)

print("questo è ciò che leggo: "+caratteristicaTemperatura.read().decode("utf-8"))

time.sleep(2)
nodoTemperatura.disconnect()
