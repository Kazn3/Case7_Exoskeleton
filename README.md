# Case7_Exoskeleton
Deze repository documenteert het volledige iteratieve ontwerpproces van een exoskelet voor handondersteuning. Hierin vind je alle CAD-modellen (per iteratie) en de bijbehorende aansturingssoftware.

## Inhoudstafel
- [CAD](#CAD)
  - [Main hub](#onderdeel-1-main-hub)
  - [Lower connector](#onderdeel-2-lower-connector)
  - [Hand link](#onderdeel-3-hand-link)
- [Arduino code](#Arduino)
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
Aangezien de links niet strak rond de vinger moet zitten om het exoskelet te doen werken is de velcro strip weggelaten en worden ze enkel op een handschoen gelijmd.
<p align="center">
<img width="60%" alt="image" src="https://github.com/user-attachments/assets/a0495eef-63f3-4c9f-add8-0c133abdfa34" />
</p>

Bestanden:
```
├── HAND_LINK_6
```

#### Hand link 7

```
├── HAND_LINK_7
```
#### Hand link 8
```
├── HAND_LINK_8
```
#### Hand link 9
```
├── HAND_LINK_9
```

---


## Arduino
De arduino bestanden zijn terug te vinden in de map [Arduino](/Arduino). 

### Finale code
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
const int SERVO_SNELHEID = 15;
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

