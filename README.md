# Lichtwecker

Disclaimer: This project is far from finished. If you stumble across feel free to ask what is going on. 

The Lichtwecker (Light alarm clock) is build from scratch using an esp32, an sd card reader, a small tft screen and a speaker. Plan is to do a PCB an publish all parts in the future, but at the moment I'm still working on the basics.

Features planned:
- Light alarm clock
	- Set Time and duration for rising light intensity
	- Set different alarms for different weekdays 
- Speaker
	- Play Sound during the light alarm clock and as a wake up tune
	- Play music for sleeping
	- All files will be stored on the SD Card, I won't publish any due to copyright reasons
- Time
	- Can be set manually or using an NTP Server
	- Usage of DCF77 is scraped as it took me a long time to get it working very unstable
- Maybe I add more stuff