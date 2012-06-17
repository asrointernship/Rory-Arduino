Rory-Arduino
============

x1, y1 = boven links
x2, y2 = boven rechts

Protocol:
241 = Screen reset
242 gevolgd door een getal van 1-3 (selecteren welke scherm)
243 = textbox, daarna volgt de font, y1 en dan de content als het om een (sub)titel gaat anders sturen we de font, y1, x1, x2, y2 en daarna de content. Om de stream af te sluiten sturen we 244.
245 = pixels doorsturen. Dit sturen we bij het begin van de stream en 244 weer om het af te sluiten.


Ik ben vertrokken van deze lib: http://code.google.com/p/arduino-t6963c/
Je kan daar tevens ook de tool vinden om fonts aan te maken: http://code.google.com/p/arduino-t6963c/downloads/list

