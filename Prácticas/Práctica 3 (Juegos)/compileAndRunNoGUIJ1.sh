#!/bin/bash
cd MancalaEngine;
make && cd .. ;
java -jar MancalaNoGUI.jar -p1 MancalaEngine/MancoBot -p2 GreedyBot/GreedyBot -t 2;
