# Graph Thery Project - "Pool" Project

## The Project

This school project is focused on ***Graph Theory***.  It can draw a graph from a txt file, calculate the faster way between two points using Dijkstra's algorithm and many other things.

## Authors

Paul BIANCALANA\
Thomas RABIAN\
Pierre WOJCIECHOWSKI\


## Questions

#### Avec le graphe « manhattan » combien de passes doit effectuer un algorithme quadratique ? Quel est la borne inférieure du temps d’exécution sur une machine à 3GHz ? Est-ce supérieur ou inférieur au temps d’une soutenance ?
-> D'après nos test il y a 16 777 216 sous graphes possibles pour le graphe de manhattan sans optimisation. Cela prendrait 93825 secondes avec un un algorithme quadratique et un processeur 3GHz. Soit 1564 minutes ou 26 heures. 

#### Est il possible d’avoir une situation dans laquelle prendre toutes les arêtes du graphe de départ ne constitue pas une des extrémités de la frontière de Pareto ? Si oui exhiber un exemple, si non dire pourquoi.
-> Oui il est possible qu'une telle situation existe, prenons l'exemple de broadway si on augmente les poids de l'arête 2 de 10 on obtient un chemin inutile car trop couteux comparé aux autres chemins.
