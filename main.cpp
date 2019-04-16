#include <iostream>
#include "svgfile.h"
#include "Graphe.h"

int main()
{
    Svgfile svgout;
    Graphe g{"files/broadway.txt", "files/broadway_weights_0.txt"};
    g.dessiner(svgout);
    return 0;
}
