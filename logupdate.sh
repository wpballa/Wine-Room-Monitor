#!/bin/bash
STR=$(date -d "last month" +"%y-%m")
mv /home/pi/wineroom/wine.log /home/pi/wineroom/$STR-wine.log
mail -a /home/pi/wineroom/$STR-wine.log -s "monthly wine room log" wpballa.wine@gmail.com


