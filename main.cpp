#include "svgfile.h"
#include "Graphe.h"

int main()
{
    //int x = 0, y = 0;
    Graphe myGraphe{"files/triville.txt", "files/triville_weights_0.txt"};

    myGraphe.affichagePareto(0);
    return 0;
}
/**for (int i = 0 ; i<myGraphe.getAretes()[0]->getNbPoids() ; ++i){
            Graphe tmp = myGraphe.parcourKruskal(i);
            tmp.dessiner(svgout, x*i, y*i);
            std::string text = "poids totales: ";
            text += std::to_string(tmp.resultatGraphe()[i]);
            svgout.addText(x+(500/2)-50,y+50 , text);
            compteur++;
            if ( x>= 2500)   { y+=500; x = 0; }
            else                { x+=500; }
}**/
