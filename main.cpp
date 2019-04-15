#include <iostream>
#include "svgfile.h"
#include "Graphe.h"

int main()
{
    Svgfile svgout;
    Graphe g{"files/cubetown.txt", "files/cubetown_weights_0.txt"};
    g.dessiner(svgout);
    return 0;
}
