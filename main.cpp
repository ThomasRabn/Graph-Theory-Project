#include <iostream>

#include "Graphe.h"
#include "Sommet.h"
#include "Arete.h"

int main()
{
    Graphe myGraphe{"files/broadway.txt", "files/broadway_weights_0.txt"};
    myGraphe.parcourKruskal(0);
    return 0;
}
