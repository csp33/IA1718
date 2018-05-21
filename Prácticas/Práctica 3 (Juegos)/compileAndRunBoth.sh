#!/bin/bash
cd MancalaEngine;
make >/dev/null && cd .. ;
printf "\tJ1: MancoBot VS J2:GreedyBot\n";
java -jar MancalaNoGUI.jar -p1 MancalaEngine/MancoBot -p2 GreedyBot/GreedyBot -t 2 | tail -n6 | head -n5;
printf "***************************************\n"
printf "\tJ1: GreedyBot VS J2:MancoBot\n";
java -jar MancalaNoGUI.jar -p1 GreedyBot/GreedyBot -p2 MancalaEngine/MancoBot -t 2 | tail -n6 | head -n5;
