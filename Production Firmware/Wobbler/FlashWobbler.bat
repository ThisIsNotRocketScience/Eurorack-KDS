@echo off
echo Flashing Wobbler!
"c:\Program Files (x86)\SEGGER\JLink_V490e\JFlash.exe" -open"AudioBoot and Wobbler together.hex" -auto -startapp -exit
echo Done!