#!/bin/bash
STR=$(date -d "last week" +"%y-%m")
mv /home/pi/wineroom/wine.log /home/pi/wineroom/$STR-wine.log
mail -a /home/pi/wineroom/$STR-wine.log -s "monthly wine room log" your-email@gmail.com


