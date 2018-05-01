@echo off
echo Writing wave file
HexToWave Edgecutter.hex Edgecutter.wav > nul
echo Merging AudioBoot.hex with Edgecutter.hex 
HexFileMerger Edgecutter.hex  AudioBoot.hex "AudioBoot and Edgecutter together.hex" >nul
echo Done!