# LineFollower
![image](https://user-images.githubusercontent.com/114825358/209218976-168dc312-c215-438f-8b81-d909546ab9aa.png)
<br />

## specifications
microcontroller: ATmega328 (Arduino Nano)
<br />
motors: Gear motor 9V - 10:1
<br />
h-bridge: Adafruit TB6612
<br />
sensors: QTR-8A
<br />
batteries: Lipo 3S 11,1V
<br />
wireless communication: HC-05
<br />
distance sensor - motors: 110mm
<br />
weight: 220g
<br />
speed: 0,906 m/s
<br />

## tips
<br />
1) Zorg dat de batterij zo dicht mogelijk bij de motoren staan. Zorg dat deze een beetje voor de hartlijn van de motoren ligt zodat de line follower geen wheelie maakt maar toch het grootste gewicht ligt bij de aandrijving.
<br />
<br />
2) Door de motoren breder uit elkaar te plaatsen kan hij de bochten makkelijker nemen.
<br />
<br />
3) Zorg dat de sensoren niet te ver en niet te dicht t.o.v. de motoren liggen. Als dit zich bevindt verder van de motoren zal je makkelijker aan een grotere snelheid komen maar kan je wel in de problemen geraken in scherpe bochten.
<br />
<br />
4) Hoe kleiner de diameter van de wielen hoe sneller de line follower zal gaan.
<br />
<br />
5) In het begin is er gebruik gemaakt van een bout maar deze bracht veel weerstand met zich mee, daarom is er overgeschakeld naar een bol (deze heeft maar 1 contactpunt met het papier, dus de weerstand is hier minimaal).
<br />
<br />
6) Bij de finaal code zal er een code te vinden zijn met library en zonder. De line follower is even snel met beide codes, alleen voor het programma kleiner te maken is het handig om met de library te werken.
<br />
<br />
7) Ook is er een finaal code geupload met een boost en brake principe, deze zal steeds voor de lange rechte stukken versnellen en dan bij de eerst volgende bocht kort remmen.
<br />
<br />
8) Tip voor de motoren, kies voor de gearmotoren 1000rpm / 30:1.
<br />
<br />
