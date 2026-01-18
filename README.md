# Case7_Exoskeleton
Deze repository documenteert het volledige iteratieve ontwerpproces van een exoskelet voor handondersteuning. Hierin vind je alle CAD-modellen (per iteratie) en de bijbehorende aansturingssoftware.

## Inhoudstafel
- [CAD](#CAD)
  - [Main Hub](#Onderdeel_1:_Main_hub)
- [Arduino code](#Arduino)

## CAD
Onder de map [CAD](/CAD) zijn de bestanden terug te vinden. Aangezien er tussen de verschillende iteraties door met dezelfde bestanden voor standaardcomponenten wordt gewerkt staan deze allemaal in dezelfde map. Onderstaande structuurlijst geeft de benodigde bestanden weer per iteratie per onderdeel.

### Onderdeel 1: Main hub
#### Main Hub 1
Dit is de eerste iteratie van de main hub. 
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
Dit is de tweede iteratie van de main hub. 
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
Dit is de derde iteratie van de main hub. 
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
Dit is de vierde iteratie van de main hub. 
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
Dit is de vijfde iteratie van de main hub. 
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
Dit is de zesde iteratie van de main hub. 
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

### Lower connector
Van dit bestand is er een onderdeel: LOWER_CONNECTOR

### HAND_LINK
Van de hand link zijn er negen iteraties. 
```
├── HAND_LINK
├── HAND_LINK_2
├── HAND_LINK_3
├── HAND_LINK_4
├── HAND_LINK_5
├── HAND_LINK_6
├── HAND_LINK_7
├── HAND_LINK_8
└── HAND_LINK_9
```



## Arduino

