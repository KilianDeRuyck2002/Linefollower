# H-Bridge proof of concept

Minimale hard- & software die aantoont dat 2 motoren onafhankelijk van elkaar kunnen draaien, en (traploos) regelbaar zijn in snelheid en draairichting.

Programma:
- Voorwaarts van 0 -> 255 -> 0
- Achterwaarts van 0 -> 255 -> 0
- Links van 0 -> 255 -> 0
- Rechts van 0 -> 255 -> 0

Sidenote: DRV8833 mag een maximale spanning van 10,8V bedragen.
=> Indien dit hoger is, dan zal de bluetooth module maar 3 weken meegaan zoals in mijn geval geweest.
