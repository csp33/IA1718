#!/bin/bash

#Compilo
cd MancalaEngine;
make >/dev/null && cd .. ;
#Juego las partidas y guardo la salida.
printf "\tJ1: MancoBot VS J2:GreedyBot\n";
j1=$(java -jar MancalaNoGUI.jar -p1 MancalaEngine/MancoBot -p2 GreedyBot/GreedyBot -t 2 | tail -n6 | head -n5);
echo "$j1"
printf "***************************************\n"
printf "\tJ1: GreedyBot VS J2:MancoBot\n";
j2=$(java -jar MancalaNoGUI.jar -p1 GreedyBot/GreedyBot -p2 MancalaEngine/MancoBot -t 2 | tail -n6 | head -n5);

echo "$j2"

puntos_manco=0;
puntos_greedy=0;

#Me quedo con los ganadores

j1=$(echo "$j1" | tail -n1)
j2=$(echo "$j2" | tail -n1)

#Sumo los puntos

#Si empatan no sumo nada

if ! [[ $j1 =~ Empate ]]
then
  if [ ${j1:17:1} -eq "1" ]
  then
    let "puntos_manco++";
  elif [ ${j1:17:1} -eq "2" ]
  then
    let "puntos_greedy++"
  fi
fi

if ! [[ $j2 =~ Empate ]]
then
  if [ ${j2:17:1} -eq "2" ]
  then
    let "puntos_manco++";
  elif [ ${j2:17:1} -eq "1" ]
  then
    let "puntos_greedy++"
  fi
fi


#Imprimo mensaje

printf "          ***************************************\n" | lolcat;
printf "          *\t    Resultado final:            *\n" | lolcat;
echo "          *    MancoBot: $puntos_manco. GreedyBot: $puntos_greedy        *"| lolcat;
printf "          ***************************************\n" | lolcat;
