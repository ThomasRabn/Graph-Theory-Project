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

    std::vector<std::vector<bool>*> vec = myGraphe.ensembleGraphesPartiels();
    std::cout << "TERMINE";
    Svgfile svgout("output.svg", 100000, 500000);
    std::vector<Arete*> aretesPartielles;
    int k = 0;
    for(auto i : vec) {
        aretesPartielles.clear();
        for (auto j : *(i)) {
            if (j == 1) aretesPartielles.push_back((myGraphe.getAretes())[j]);
        }
        Graphe graphePartiel{myGraphe.getSommets(), aretesPartielles};
        graphePartiel.dessiner(svgout, x , y);
        if ((k+1)%8 == 0)   { y+=800; x = 0; }
        else                { x+=800; }
        if (k >= 15)    break;
        std::cout << k << std::endl;
        k++;
    }
    std::cout << vec.size();
    return 0;
}
