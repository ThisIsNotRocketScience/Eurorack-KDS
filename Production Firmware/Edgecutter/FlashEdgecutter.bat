@echo off
echo Flashing Edgecutter!
"c:\Program Files (x86)\SEGGER\JLink_V490e\JFlash.exe" -open"AudioBoot and Edgecutter together.hex" -auto -startapp -exit
echo Done!