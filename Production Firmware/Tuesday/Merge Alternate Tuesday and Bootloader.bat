@echo off
echo Writing wave file
HexToWave TuesdayAlt.hex TuesdayAlt.wav > nul
echo Merging AudioBoot.hex with Tuesday.hex 
HexFileMerger TuesdayAlt.hex AudioBoot.hex "AudioBoot and Tuesday Alt together.hex"
echo Done!