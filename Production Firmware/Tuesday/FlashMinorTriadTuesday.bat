@echo off
echo Flashing Tuesday!
"c:\Program Files (x86)\SEGGER\JLink_V490e\JFlash.exe" -open"AudioBoot and MinorTriadTuesday together.hex" -auto -startapp -exit
echo Done!