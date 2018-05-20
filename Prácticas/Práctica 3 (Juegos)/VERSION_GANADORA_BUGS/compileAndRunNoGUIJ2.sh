#!/bin/bash
cd MancalaEngine;
make && cd .. ;
java -jar MancalaNoGUI.jar -p1 GreedyBot/GreedyBot -p2 MancalaEngine/MancoBot -t 2;
