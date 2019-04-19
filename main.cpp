//#include <iostream>
//#include "svgfile.h"
#include "Graphe.h"
//#include "Sommet.h"
//#include "Arete.h"
//#include <vector>

int main()
{
    //Graphe myGraphe {"files/broadway.txt", "files/broadway_weights_0.txt"};
    Graphe myGraphe {"files/manhattan.txt", "files/manhattan_weights_0.txt"};

    std::vector < std::vector < std::pair<int, int> > > test;

    unsigned int i(0), j;

    //for(j=0; j<500000; j++)
        //for(i=0; i<15; i++)
            test = myGraphe.dijkstra(i, myGraphe.getSommets()[i]);

    //int x = 0, y = 0;
    /*
    Graphe myGraphe{"files/triville.txt", "files/triville_weights_0.txt"};
    if(1) {
        Svgfile svgout;
        myGraphe.dessiner(svgout);
    }


    std::vector<Graphe*> vec = myGraphe.ensembleGraphesPartiels();
    std::cout << "TERMINE";


    Svgfile svgout("output.svg", 100000, 500000);
    for(unsigned int i = 0; i < vec.size(); ++i) {
        vec[i]->dessiner(svgout, x , y);
        if ((i+1)%8 == 0)   { y+=200; x = 0; }
        else                { x+=200; }
    }
    std::cout << vec.size();*/
    return 0;
}
