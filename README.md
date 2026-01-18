# Manura - Hand exoskeleton
Deze repository documenteert het volledige iteratieve ontwerpproces van een exoskelet voor handondersteuning. Hierin vind je alle CAD-modellen (per iteratie) en de bijbehorende aansturingssoftware. Van elk prototype is de laatste iteratie gebruikt in het finale ontwerp.

## Inhoudsopgave
- [CAD](#CAD)
  - [Main hub](#onderdeel-1-main-hub)
  - [Lower connector](#onderdeel-2-lower-connector)
  - [Hand link](#onderdeel-3-hand-link)
- [Arduino code](#Arduino)
  - [Arduino UNO](#arduino-uno)
  - [Arduino Nano 33 IoT](#arduino-nano-33-iot)
  - [Finale code](#finale-code)

---

## CAD
Onder de map [CAD](/CAD) zijn de bestanden terug te vinden. Aangezien er tussen de verschillende iteraties door met dezelfde bestanden voor standaardcomponenten wordt gewerkt staan deze allemaal in dezelfde map. Onderstaande structuurlijst geeft de benodigde bestanden weer per iteratie per onderdeel.

Voor dat de main hub werd ontworpen was er een test gedaan om te bevestigen of het oprolmechanisme zou werken. Na deze test werd duidelijk dat de boven- en onderkant een groot verschil in oprollen van touw nodig hadden. Dit zou mogelijk zijn door middel van een conische drum, of een drum die uit twee verschillende groottes bestaat.

### Onderdeel 1: Main hub
Dit onderdeel van het prototype omvat alle elektronica en componenten om het exoskelet aan te sturen

#### Main Hub 1
Voor de eerste iteratie wordt gebruikgemaakt van een oprolmechanisme met zelf-uitlijnende tandwielen, gemonteerd op een polsbeschermer. Het tandwiel is aan weerszijden voorzien van een groef om de draad tijdens het oprollen nauwkeurig te geleiden.

<p align="center">
 <img width="60%" align="center" alt="image" src="https://github.com/user-attachments/assets/5ca2c63e-3491-48a1-bc5d-001d24397fd1" />
</p>

Bestanden:
```
MAIN_HUB
 ├── SERVO_MINI
 |    ├── LOBE_GEAR_2
 |    └── Single row radial deep groove ball bearing ISO 1002 11 4_826
 |         └── Single row radial deep groove ball bearing ISO 1002 11 4_826_PART_1 -5
 ├── GEARBOX
 ├── Single row radial deep groove ball bearing ISO 1002 11 4_826
 |    └── Single row radial deep groove ball bearing ISO 1002 11 4_826_PART_1 -5
 ├── HOUSING
 └── Hexalobular socket cheese head screw ISO 14579 M5x45
```

#### Main Hub 2
Deze iteratie adresseert de ruimtebeperkingen van het voorgaande ontwerp. Om de interne ruimte efficiënter te benutten, zijn de servo's nu geschrankt gepositioneerd in plaats van in een lineaire opstelling. Daarnaast is de behuizing uitgebreid met een geïntegreerd batterijcompartiment aan de onderzijde en is er binnenin de hub een specifieke bevestigingsplaats voor de Arduino gereserveerd.

<p align="center">
  <img width="60%" alt="image" src="https://github.com/user-attachments/assets/26991686-25ca-46f6-b5db-5fb8e81760a8" />  
</p>

Bestanden:
```
MAIN_HUB_2
 ├── SERVO_MINI
 |    ├── LOBE_GEAR_2
 |    └── Single row radial deep groove ball bearing ISO 1002 11 4_826
 |         └── Single row radial deep groove ball bearing ISO 1002 11 4_826_PART_1 -5
 ├── GEARBOX_2
 ├── Single row radial deep groove ball bearing ISO 1002 11 4_826
 |    └── Single row radial deep groove ball bearing ISO 1002 11 4_826_PART_1 -5
 ├── HOUSING_2
 ├── ARDUINO_NANO
 |    └── nano-board_step
 ├── HOUSING_2
 ├── BATTERY_3.7V
 ├── ARM
 ├── PCB
 ├── SPACER
 ├── Hexagon socket countersunk head screw ISO 10642 M5x16
 ├── Hexagon socket countersunk head screw ISO 10642 M5x45
 └── Hexalobular socket cheese head screw ISO 14579 M5x45
```

#### Main Hub 3
Bij de voorgaande iteratie bleek de uitlijning van de tandwielen problematisch, wat leidde tot doorslippen en mechanisch falen. Vanwege deze instabiliteit is voor de derde iteratie overgestapt op een volledig nieuw aandrijfconcept. De tandwieloverbrenging heeft plaatsgemaakt voor een pulleysysteem, waarbij de pulley's rechtstreeks op de assen van de servo's zijn gemonteerd.
<p align="center">
  <img width="60%" alt="image" src="https://github.com/user-attachments/assets/e220b8b0-afea-405c-a555-ec314513ea71" />
</p>

Bestanden: 
```
MAIN_HUB_3
 ├── SERVO_MINI
 |    ├── LOBE_GEAR_2
 |    └── Single row radial deep groove ball bearing ISO 1002 11 4_826
 |         └── Single row radial deep groove ball bearing ISO 1002 11 4_826_PART_1 -5
 ├── Single row radial deep groove ball bearing ISO 1002 11 4_826
 |    └── Single row radial deep groove ball bearing ISO 1002 11 4_826_PART_1 -5
 ├── PULLEY_MG
 |    ├── PULLEY_LINK_2
 |    └── SERVO_MINI
 |         ├── LOBE_GEAR_2
 |         └── Single row radial deep groove ball bearing ISO 1002 11 4_826
 |              └── Single row radial deep groove ball bearing ISO 1002 11 4_826_PART_1 -5
 ├── ROPE
 └── Hexalobular socket cheese head screw ISO 14579 M5x45
```

#### Main Hub 4
Het pulleysysteem bleek in de praktijk gevoelig voor verstrikking wanneer de draadspanning wegviel. Om dit op te lossen zijn de poelies in deze iteratie vervangen door lobes (asymmetrische spoelen). Door de specifieke vormgeving, met een lange en een korte zijde, genereren deze componenten exact de juiste trekkarakteristiek en slaglengte voor de corresponderende zijde van de vinger.

<p align="center">
  <img width="60%" alt="image" src="https://github.com/user-attachments/assets/8ae25c38-6bc5-4d9e-aebd-7632a1705cc3" />
</p>

Bestanden:
```
MAIN_HUB_4
 ├── SERVO_MINI
 |    ├── LOBE_GEAR_2
 |    └── Single row radial deep groove ball bearing ISO 1002 11 4_826
 |         └── Single row radial deep groove ball bearing ISO 1002 11 4_826_PART_1 -5
 ├── Single row radial deep groove ball bearing ISO 1002 11 4_826
 |    └── Single row radial deep groove ball bearing ISO 1002 11 4_826_PART_1 -5
 ├── Hexagon socket countersunk head screw ISO 10642 M5x16
 └── Hexagon socket countersunk head screw ISO 10642 M5x45
```

#### Main Hub 5
Dit prototype bouwt constructief voort op de vorige iteratie, maar is uitgerust met grotere en krachtigere servo's. De voorgaande configuratie bleek ondergedimensioneerd en leverde onvoldoende koppel om de vingers effectief te laten buigen.
<p align="center">
  <img width="60%" alt="image" src="https://github.com/user-attachments/assets/b19eacd4-3df4-4714-af01-f3fba31fc90d" />
</p>

Bestanden:
```
MAIN_HUB_5
 ├── SERVO
 |    ├── LEVER_CONNECTOR
 |    ├── LEVER
 |    ├── LEVER_2
 |    ├── Single row radial deep groove ball bearing ISO 1002 11 4_826
 |    |    └── Single row radial deep groove ball bearing ISO 1002 11 4_826_PART_1 -5
 |    └── LOBE_GEAR_2
 ├── CONNECTOR
 └── Hexagon socket countersunk head screw ISO 10642 M5x45
```

#### Main Hub 6
In deze laatste iteratie lag de focus op het reduceren van de totale breedte, aangezien het voorgaande prototype te omvangrijk bleek voor montage op de arm. Het mechanisme is aangepast naar een lever. Dit ontwerp behoudt de functionaliteit, maar zorgt voor een aanzienlijk slanker profiel. Tevens elimineert deze constructie het risico dat de draad uit de geleidingsgroef loopt, wat de betrouwbaarheid verhoogt.
<p align="center">
  <img width="60%" alt="image" src="https://github.com/user-attachments/assets/2f4e146f-d094-43cf-9c5e-708422162ac4" />
</p>

Bestanden:
```
MAIN_HUB_6
 ├── SERVO
 |    ├── LEVER_CONNECTOR
 |    ├── LEVER
 |    ├── LEVER_2
 |    ├── Single row radial deep groove ball bearing ISO 1002 11 4_826
 |    |    └── Single row radial deep groove ball bearing ISO 1002 11 4_826_PART_1 -5
 |    └── LOBE_GEAR_2
 ├── TOP_SHELL
 ├── BACK_PART
 ├── FRONT_PART
 ├── BATTERY_MOUNT
 └── Hexagon socket countersunk head screw ISO 10642 M5x45
```
---

### Onderdeel 2: Lower connector
Deze connector is geplaatst op de onderkant van de polsbeschermer en zorgt ervoor dat de bedrading geleid wordt.
<p>
  
</p>

Bestanden: 
```
├── LOWER_CONNECTOR
```
---

### Onderdeel 3: Hand link
Als laatste onderdeel zijn er de links die op de vingers worden geplaatst. Deze links geleiden de bedrading aan de boven- en onderkant van de vinger.
#### Hand link 1
De allereerste links zijn gemaakt dat er per vingerkoot een link is. Aan de boven en onderkant van de links zijn er 3 gaten. 2 om elastisch de links bij elkaar te houden en 1 om de bedrading met de servos te geleiden.
<p align="center">
   <img height="400px"  alt="image" src="https://github.com/user-attachments/assets/51fbea11-d2c2-4f26-9da1-c4d7a501f1fb" />
</p>

Bestanden:
```
├── HAND_LINK
```

#### Hand link 2
De tweede links zijn zeer gelijkaardig aan de eerste met als enige verschil dat deze geschaald zijn naar de juiste grootte.
<p align="center">
</p>

Bestanden:
```
├── HAND_LINK_2
```

#### Hand link 3
Om een extra veiligheid in te bouwen zodat de vinger niet overstrekt kan worden door het systeem is er een ingebouwde stop bij deze links. Deze gaan blokkeren vanaf dat de vinger zou overstrekken.
<p align="center">
<img width="60%" alt="image" src="https://github.com/user-attachments/assets/4f11bfb2-24ab-446a-afc4-8488cb33a39f" />
</p>

Bestanden:
```
├── HAND_LINK_3
```
#### Hand link 4
Bij de vorige links waren de rotatiecentra niet op de juiste plek, dit zorgde ervoor dat de links niet juist draaide en de beweging moeilijker maakten. Dit werd opgelost door het rotatiecentrum te verplaatsen en de links op te splitsen. Om ervoor te zorgen dat ze goed aansluiten rond de vinger van de gebruiker worden ze met velcrostrips vastgeklemd.
<p align="center">
<img width="60%" alt="image" src="https://github.com/user-attachments/assets/0d2ca307-5093-43a1-b5d8-74a5873b3583" />
</p>

Bestanden:
```
├── HAND_LINK_4
```
#### Hand link 5
Bij alle vorige prototypes zou de gebruiker elke link apart moeten bevestigen. De bedoeling van dit project is om het gebruiksgemak te verhogen. Dit wordt vertaald naar de links door deze op een handschoen te bevestigen. Zo kan de gebruiker alle links in een beweging rond de vinger bevestigen.
<p align="center">
<img width="60%" alt="image" src="https://github.com/user-attachments/assets/6882e563-60be-494b-9b03-468cb3fcf138" />
</p>

Bestanden:
```
├── HAND_LINK_5
```

#### Hand link 6
Om de bewegingsvrijheid van de gebruiker niet te beperken, is er in deze iteratie gekozen voor TPU. Dit flexibele materiaal zorgt ervoor dat de links kunnen stretchen met de bewegingen van de handschoen en de vinger.
<p align="center">
<img width="60%" alt="image" src="https://github.com/user-attachments/assets/a0495eef-63f3-4c9f-add8-0c133abdfa34" />
</p>

Bestanden:
```
├── HAND_LINK_6
```

#### Hand link 7
Bij deze versie is een cleat aan het ontwerp toegevoegd. Dit mechanisme maakt het mogelijk om de draadspanning eenvoudig te fixeren en later weer af te stellen.
<p align="center">
<img width="60%" alt="image" src="https://github.com/user-attachments/assets/9b7261ba-2129-40f5-a199-82e51352c28b" />
</p>

Bestanden:
```
├── HAND_LINK_7
```

#### Hand link 8
Dit ontwerp bouwt grotendeels voort op de voorgaande versie, maar breidt de keten uit met een vierde link die op de pols rust. Dit zorgt voor een vlottere overgang van de hand naar de main hub en een betere geleiding van de kabels.
<p></p>

Bestanden:
```
├── HAND_LINK_8
```

#### Hand link 9
In deze laatste iteratie zijn de links dunner gemaakt waardoor het overbodige materiaal niet wordt verbruikt. Zo wordt er enkel gebruik gemaakt van de voorste link en wordt de onderste bekabeling geleid door een connectie onderaan de pols. Dit is de [lower connection](#onderdeel-2-lower-connector).
<p align="center">
<img width="60%" alt="image" src="https://github.com/user-attachments/assets/e2cc5717-4154-4e82-8d8b-0aef2b5aa82b" />

</p>

Bestanden:
```
├── HAND_LINK_9
```

---


## Arduino
De arduino bestanden zijn terug te vinden in de map [Arduino](/Arduino). Deze codes zijn gebruikt voor componenten te testen om zo tot de definitieve, finale code te kommen. Eerst werd er gewerkt met een Arduino UNO, later in het project is er overgeschakeld naar een Arduino Nano 33 IoT voor een draadloze ontwikkeling. 

### Arduino UNO
Deze scripts richten zich op het testen van individuele hardware-componenten en het ontwikkelen van de motorlogica.
Erm Validatiecode voor de ERM-component (Eccentric Rotating Mass). Deze test dient om de outputkarakteristieken te analyseren en de toepasbaarheid binnen het exoskelet te evalueren.
```
├── erm.ino
```

Een eerste test met een stepmotor.
De motor wordt aangestuurd met een drukknop:
- eerste druk: motor begint te draaien
- tweede druk: motor stopt
- derde druk: motor draait één volledige omwenteling achteruit

```
├── stepmotor.ino
```

Uitbreiding op de basiscode waarbij een geheugenfunctie is toegevoegd. Het systeem houdt het aantal voorwaartse omwentelingen bij en draait bij de 'reverse'-instructie exact hetzelfde aantal slagen terug naar de nulpositie.
```
├── RoHs_drukknop_toeren_tellen.ino
```

Toevoeging van een softwarematige veiligheidsbegrenzing. Er is een maximumlimiet ingesteld voor het aantal rotaties om mechanische overbelasting te voorkomen wanneer de stop-input uitblijft.
```
├── stepmotor_met_limiet_instellen.ino
```
Validatie van de besturingslogica (limieten en tellers) toegepast op de 28BYJ-48 stappenmotorhardware.
```
├── 28BYJ-48_stappenmotor.ino
```

Porting van de besturingslogica (inclusief limieten en veiligheid) naar een MG90S-servo, ter voorbereiding op de definitieve aandrijving.
```
├── servo_met_drukknop.ino
```

### Arduino Nano 33 IoT
Deze scripts focussen op draadloze communicatie (WiFi/BLE), integratie met de webinterface en stroommanagement.
Proof-of-concept voor draadloze actuatie van vier servo’s via een lokale webserver over WiFi.
Functionaliteit: Schakelen tussen drie hardcoded bewegingspatronen (Stop / Servo 1&2 bewegen / Servo 3&4 bewegen).
```
├── servo_wifi.ino
```
Hybride besturing waarbij het protocol wordt gesplitst: de selectie van het patroon verloopt via de webinterface, maar de activering (start/stop) vereist een fysieke drukknopbevestiging voor extra veiligheid.
```
├── wifi_met_knop.ino
```

Overstap naar het Bluetooth Low Energy (BLE) protocol voor energie-efficiëntie. Aansturing van vijf servo’s met vijf specifieke revalidatiepatronen, selecteerbaar via de nRF Connect smartphone-app.
```
├── servo_BLE_5pattern.ino
```
Combinatie van BLE-communicatie en fysieke interactie. Net als bij de WiFi-variant worden patronen digitaal geselecteerd (via nRF Connect), maar wordt de cyclus gestart en onderbroken via een fysieke knop op de hub.
```
├── servo_Bluetooth_met_knop.ino
```
Eerste volledige integratie met de custom webinterface via BLE. Zowel de patroonselectie als de start/stop-commando's worden volledig remote via de website aangestuurd.
```
├── eerste_code_voor_website.ino
```
Optimalisatie van de voorgaande code voor stand-alone werking. De firmware is aangepast om stabiel te functioneren op batterijvoeding, onafhankelijk van USB-connectie met een pc.
```
├── werkt_op_batterij.ino
```

Uitbreiding van het communicatieprotocol. De webinterface stuurt nu dynamische parameters door: naast het type bewegingspatroon wordt ook de gewenste tijdsduur van de oefening variabiliseerd.
```
├── website_INPUT_pattern_en_tijd
```

### Finale code
De definitieve release candidate. Deze versie bevat dezelfde functionaliteit als de voorgaande iteratie, maar is voorzien van uitgebreide code-commentaar en documentatie per sectie voor onderhoudbaarheid en overdracht.

Deze code vind je [hier](/Arduino/laatste_code.ino) terug.
Deze Arduino-code stuurt **5 servo motoren** aan met een **website** verbonden via **Bluetooth Low Energy (BLE)**.  
De servo’s bewegen **vloeiend zonder `delay()`**, zodat je animaties soepel blijven lopen terwijl BLE en input gewoon blijven werken.

Ideaal voor interactieve installaties, kinetische sculpturen of Arduino-gestuurde prototypes met meerdere servo’s.


#### Features

- 5 servo’s met soepele beweging  
- Aansturing met website verbonden via Bluetooth (BLE)  
- 5 verschillende bewegingspatronen  
- Geen `delay()` → geen blokkering  


#### Benodigde hardware

- Arduino Nano 33 IoT  
- 5x Servo motoren  
- Externe 5–6V voeding voor servo’s
- Externe 5V voeding voor arduino
- Verbindingen

Pin mapping:
| Servo | Pin | Vinger |
|------|-----|-----|
| s1 | 3 |Duim|
| s2 | 5 |Wijsvinger|
| s3 | 6 |Middelvinger|
| s4 | 9 |Ringvinger|
| s5 | 10 |Pink|


#### Servobeweging

Elke servo gebruikt een **SmoothServo-struct** die zorgt voor vloeiende beweging in plaats van abrupt bewegen.

De snelheid wordt bepaald via:

```cpp
const int SERVO_SNELHEID = 10;
````

Hoe lager dit getal, hoe sneller de servo beweegt.

De servo’s ontvangen hun INPUT via de website, waar zowel het type bewegingspatroon als de duur van de uitvoering wordt ingesteld.


#### Beschikbare patronen

| Patroon | Naam|Beschrijving                                            |
| ------- |-----------------|------------------------------------------- |
| 1       |Volledige vuist  |Alle servo's bewegen tegelijk herhalend van 0° naar 180° en van 180° naar 0°|
| 2       |Vinger per vinger|Servo's bewegen één voor één van 0° naar 180° en van 180° naar 0°|
| 3       |Pinch grip       |Servo's 1 en 2 bewegen herhalend van 0° naar 180° en van 180° naar 0° |
| 4       |Flexiegolf       |Servo's sluiten en openen in een golf beweging |
| 5       |Vuist zonder duim|Alle servo's bewegen herhalend van 0° naar 180° en van 180° naar 0° buiten servo1|

---

### Mogelijke uitbreidingen

* Extra patronen
* Manuele bediening met een fysieke knop
* Sterkte van de servo's aanpassen via de website

