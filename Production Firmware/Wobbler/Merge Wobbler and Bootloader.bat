@echo off
echo Writing wave file
HexToWave WobblerV2.hex Wobbler.wav > nul
echo Merging AudioBoot.hex with WobblerV2.hex 
HexFileMerger WobblerV2.hex  AudioBoot.hex "AudioBoot and Wobbler together.hex" >nul
echo Done!