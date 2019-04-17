#include "svgfile.h"
#include "Graphe.h"

int main()
{
    //int x = 0, y = 0;
    Graphe myGraphe{"files/manhattan.txt", "files/manhattan_weights_0.txt"};

    myGraphe.affichagePareto();

    /*if(1) {
        Svgfile svgout;
        myGraphe.dessiner(svgout);
    }*/

    /*std::vector<std::vector<bool>*> vec = myGraphe.ensembleGraphesPartiels();
    std::cout << "TERMINE";
    Svgfile svgout("output.svg", 100000, 500000);
    std::vector<Arete*> aretesPartielles;
    for(unsigned int i = 0; i < vec.size(); ++i) {
        aretesPartielles.clear();
        for (unsigned int j = 0; j < (*(vec[i])).size(); ++j) {
            if ((*(vec[i]))[j]) aretesPartielles.push_back((myGraphe.getAretes())[j]);
        }
        Graphe graphePartiel{myGraphe.getSommets(), aretesPartielles};
        graphePartiel.dessiner(svgout, x , y);
        if ((i+1)%6 == 0)   { y+=450; x = 0; }
        else                { x+=450; }
    }
    std::cout << vec.size();*/
    return 0;
}
