#include <iostream>
#include "svgfile.h"
#include "Graphe.h"

#include "Graphe.h"
#include "Sommet.h"
#include "Arete.h"

int main()
{
    Svgfile svgout;
    Graphe myGraphe{"files/broadway.txt", "files/broadway_weights_0.txt"};
    Graphe test = myGraphe.parcourKruskal(0);
    test.dessiner(svgout);
    return 0;
}
