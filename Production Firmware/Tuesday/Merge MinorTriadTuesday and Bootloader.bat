@echo off
echo Writing wave file
HexToWave Tuesday-minortriadfix.hex MinorTriadTuesday.wav > nul
echo Merging AudioBoot.hex with Tuesday.hex 
HexFileMerger Tuesday-minortriadfix.hex  AudioBoot.hex "AudioBoot and MinorTriadTuesday together.hex" >nul
echo Done!