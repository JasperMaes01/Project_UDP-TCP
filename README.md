# Project_UDP-TCP
## Opdracht 1:
Maak een UDP random nummer generator die na een reeks nummers het hoogste getal terug verwacht te komen.
[Zie foto BB]
## Opdracht 2:
Maak een TCP random bewerking generator die de oplossing terug verwacht te krijgen.
[Zie foto BB]
## Beoordeling:
- UDP socket is correct aangemaakt en verbonden
- UDP client stuurt eerst "GO" en wordt ontvangen door UDP server
- UDP server stuurt max. 42 random getallen in network-byte-order en UDP client ontvangt die (kan minder zijn door verlies, dus vang op met timeout)
- UDP client zoekt grootste waarde en stuurt die terug, UDP server verwacht de grootste waarde terug te krijgen
- UDP client stuurt steeds na een timeout van 1 seconden opnieuw het getal, UDP server verwacht binnen de 3 seconden het antwoord opnieuw
- Na random aantal herzendingen, begint UDP server aan volgende reeks random getallen, UDP-client herhaalt zichzelf niet meer en ontvangt deze random getallen
- UDP client zoekt opnieuw grootste waarde uit nieuwe reeks getallen en stuurt die terug, UDP server verwacht deze nieuwe grootste waarde terug te krijgen
- UDP client stuurt opnieuw steeds na een timeout van 1 seconden opnieuw het getal, UDP server verwacht binnen de 3 seconden het antwoord opnieuw
- Na random aantal herzendingen, stuurt UDP server OK en verbinding wordt aan beide kanten mooi afgesloten
- Aansluitend aan de UDP communicatie start het TCP gedeelte zonder aparte executables of manuele interventie
- TCP socket is correct aangemaakt en verbonden
- TCP client stuurt een random bewerking (i.e. getal, bewerking, getal) die de TCP server ontvangt en splitst
- TCP server maakt de bewerking en stuurt de oplossing naar de TCP client die ze accepteert en controleert
- TCP client stuurt een tweede random bewerking (i.e. getal, bewerking, getal) die de TCP server ontvangt en splitst
- TCP server maakt de tweede bewerking en stuurt de oplossing naar de TCP client die ze accepteert en controleert
- TCP client stuurt continue random bewerkingen (i.e. getal, bewerking, getal) die de TCP server steeds ontvangt en splitst tot een random aantal waarop er "STOP" gestuurd wordt die netjes ontvangen wordt (i.e. niet crashen)
- TCP server stuurt "OK" terug, waarop de TCP client "KTNXBYE" stuurt
- TCP client sluit mooi de verbinding af en server wacht op het afsluiten om ook mooi af te sluiten
- Het geheel werk volledig zonder crashen en er wordt efficiënt met de resources (i.e. memory en sockets) van de computer gebruik gemaakt
- Code staat professioneel op GitHub (i.e. meerdere nuttige commits en uitleg)
