BIANCALANA Paul
RABIAN Thomas
WOJCIECHOWKI Pierre

# ProjetPiscine
Le meilleur projet piscine

-Avec le graphe � manhattan � combien de passes doit effectuer un algorithme quadratique ? Quel est la borne
inf�rieure du temps d�ex�cution sur une machine � 3GHz ? Est-ce sup�rieur ou inf�rieur au temps
d�une soutenance ?
--D'apres nos test il y a 16 777 216 sous graphes possibles pour le graphe de manhattan sans optimisation.
93825 secondes avec un un algo quadratique et un processeur 3GHz. Soit 1564 minutes o� 26 heures. 

-Est il possible d�avoir une situation dans laquelle prendre toutes les ar�tes du graphe de d�part ne constitue pas une des extr�mit�s de la fronti�re de Pareto ? Si oui exhiber un
exemple, si non dire pourquoi.
--Oui il est possible qu'une telle situation existe, prenons l'exemple de broadway si on augment les poids de l'ar�te 2 de 10 on obtient un chemin inutile car trop couteux compar� aux autres chemins.


-Conseil sur l�impl�mentation de la d�termination efficace de la fronti�re de Pareto : la
m�thode sugg�r�e dans la pr�sentation, comparer 2 � 2 les solutions et �liminer les solution
domin�es pourrait faire croire qu�on a un algorithme quadratique. Dans le cas particulier de poids
anti-corr�l�s (page 7 en bas) toutes les solutions sont Pareto optimales et donc il n�y en a aucune
d��limin�e : dans ce cas l�algorithme est bien quadratique. Questions : avec le graphe
� manhattan � combien de passes doit effectuer un algorithme quadratique ? Quel est la borne
inf�rieure du temps d�ex�cution sur une machine � 3GHz ? Est-ce sup�rieur ou inf�rieur au temps
d�une soutenance ?
Mais dans un cas de poids al�atoires non corr�l�s, comme vous pouvez le voir sur le nuage de
points ci dessus, de tr�s nombreuses solutions sont nettement domin�es. Passez en revue chaque
sommet et pour chacun de ces sommets passez en revue tous les autres et �liminez directement ceux
qui sont domin�s. D�s qu�un bon sommet est trouv� l�ensemble est d�cim�, plus on avance plus il
devient facile d�avancer : la boucle externe se raccourci et la boucle interne aussi. Au final on se
retrouve typiquement avec une complexit� lin�aire ou log-lin�aire (� d�terminer� voir Extensions)