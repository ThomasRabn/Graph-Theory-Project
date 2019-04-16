#include <iostream>
#include "svgfile.h"
#include "Graphe.h"
#include "Sommet.h"
#include "Arete.h"
#include <vector>

int main()
{
    int x = 0, y = 0;
    Graphe myGraphe{"files/manhattan.txt", "files/manhattan_weights_0.txt"};
    if(1) {
        Svgfile svgout;
        myGraphe.dessiner(svgout);
    }

    std::vector<Graphe*> vec = myGraphe.ensembleGraphesPartiels();
    std::cout << "TERMINE";
    /*Svgfile svgout("output.svg", 100000, 500000);
    for(unsigned int i = 0; i < vec.size(); ++i) {
        vec[i]->dessiner(svgout, x , y);
        if ((i+1)%8 == 0)   { y+=200; x = 0; }
        else                { x+=200; }
    }
    std::cout << vec.size();*/
    return 0;
}
