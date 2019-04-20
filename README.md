BIANCALANA Paul
RABIAN Thomas
WOJCIECHOWKI Pierre

# ProjetPiscine
Le meilleur projet piscine

-Avec le graphe « manhattan » combien de passes doit effectuer un algorithme quadratique ? Quel est la borne
inférieure du temps d’exécution sur une machine à 3GHz ? Est-ce supérieur ou inférieur au temps
d’une soutenance ?
--D'apres nos test il y a 16 777 216 sous graphes possibles pour le graphe de manhattan sans optimisation.
93825 secondes avec un un algo quadratique et un processeur 3GHz. Soit 1564 minutes où 26 heures. 

-Est il possible d’avoir une situation dans laquelle prendre toutes les arêtes du graphe de départ ne constitue pas une des extrémités de la frontière de Pareto ? Si oui exhiber un
exemple, si non dire pourquoi.
--Oui il est possible qu'une telle situation existe, prenons l'exemple de broadway si on augment les poids de l'arête 2 de 10 on obtient un chemin inutile car trop couteux comparé aux autres chemins.


-Conseil sur l’implémentation de la détermination efficace de la frontière de Pareto : la
méthode suggérée dans la présentation, comparer 2 à 2 les solutions et éliminer les solution
dominées pourrait faire croire qu’on a un algorithme quadratique. Dans le cas particulier de poids
anti-corrélés (page 7 en bas) toutes les solutions sont Pareto optimales et donc il n’y en a aucune
d’éliminée : dans ce cas l’algorithme est bien quadratique. Questions : avec le graphe
« manhattan » combien de passes doit effectuer un algorithme quadratique ? Quel est la borne
inférieure du temps d’exécution sur une machine à 3GHz ? Est-ce supérieur ou inférieur au temps
d’une soutenance ?
Mais dans un cas de poids aléatoires non corrélés, comme vous pouvez le voir sur le nuage de
points ci dessus, de très nombreuses solutions sont nettement dominées. Passez en revue chaque
sommet et pour chacun de ces sommets passez en revue tous les autres et éliminez directement ceux
qui sont dominés. Dès qu’un bon sommet est trouvé l’ensemble est décimé, plus on avance plus il
devient facile d’avancer : la boucle externe se raccourci et la boucle interne aussi. Au final on se
retrouve typiquement avec une complexité linéaire ou log-linéaire (à déterminer… voir Extensions)