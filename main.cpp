#include "svgfile.h"
#include "Graphe.h"

int main()
{
    //int x = 0, y = 0;
    Graphe myGraphe{"files/broadway.txt", "files/broadway_weights_0.txt"};

    myGraphe.affichagePareto();
    return 0;
}
