@echo off
echo Writing wave file
HexToWave Tuesday.hex Tuesday.wav > nul
echo Merging AudioBoot.hex with Tuesday.hex 
HexFileMerger Tuesday.hex  AudioBoot.hex "AudioBoot and Tuesday together.hex" >nul
echo Done!