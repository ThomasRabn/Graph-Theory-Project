#include "svgfile.h"
#include "Graphe.h"

int main()
{
    //int x = 0, y = 0;
    Graphe myGraphe{"files/manhattan.txt", "files/manhattan_weights_0.txt"};

    myGraphe.affichagePareto(1);
    return 0;
}
