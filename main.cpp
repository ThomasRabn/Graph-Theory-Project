//#include <iostream>
//#include "svgfile.h"
#include "Graphe.h"
//#include "Sommet.h"
//#include "Arete.h"
//#include <vector>

std::vector <std::string> choix();
std::vector<std::vector<bool>*> remplirBool(std::vector<std::vector<bool>*> myBool, int taille);
void printVector3(std::vector < std::vector < std::vector < float > > > testDij, int taille);

int main()
{
    std::vector <std::string> parametres = choix();
    Graphe myGraphe {parametres[0], parametres[1]};

    std::vector < std::vector < std::vector < float > > > testDij;

    std::vector<std::vector<bool>*> myBool;

    myBool = remplirBool(myBool, myGraphe.getAretes().size());

    testDij = myGraphe.dijkstra(0, myBool);

    printVector3(testDij, myGraphe.getSommets().size());

    /**
    unsigned int i(0), j;
    for(j=0; j<1000000; j++)
        for(i=0; i<15; i++)
            test = myGraphe.dijkstra(i, myGraphe.getSommets()[i]);
    */

    return 0;
}

std::vector <std::string> choix()
{
    std::vector <std::string> parametres;

    int choix;
    std::string poids;

    std::cout   << "    ~ Choix ville ~"    << std::endl    << std::endl
                << "1 => Broadway"          << std::endl
                << "2 => Cubetown"          << std::endl
                << "3 => Manhattan"         << std::endl
                << "4 => Triville"          << std::endl    << std::endl
                << "    choix : ";

    std::cin    >> choix;

    std::cout   << std::endl                << std::endl
                << "    ~ Choix poids ~"    << std::endl    << std::endl
                << "0"                      << std::endl
                << "1 (choix != 2)"         << std::endl
                << "2 (choix != 2 & 4)"     << std::endl    << std::endl
                << "    poids : ";

    std::cin    >> poids;
    std::cout   << std::endl << std::endl;

    if(choix == 1) {parametres.push_back("files/broadway.txt"); parametres.push_back("files/broadway_weights_"  + poids + ".txt");}
    if(choix == 2) {parametres.push_back("files/cubetown.txt"); parametres.push_back("files/cubetown_weights_"  + poids + ".txt");}
    if(choix == 3) {parametres.push_back("files/manhattan.txt");parametres.push_back("files/manhattan_weights_" + poids + ".txt");}
    if(choix == 4) {parametres.push_back("files/triville.txt"); parametres.push_back("files/triville_weights_"  + poids + ".txt");}

    return parametres;
}

std::vector<std::vector<bool>*> remplirBool(std::vector<std::vector<bool>*> myBool, int taille)
{
    for (int i = 0; i < 1; ++i)
	{
		std::vector<bool>* v = new std::vector<bool>;
		for (int j = 0; j < taille; ++j)
            (*v).push_back(1);

		myBool.push_back(v);
		delete(v);
	}
	return myBool;
}

void printVector3(std::vector < std::vector < std::vector < float > > > testDij, int taille)
{
    for(int i=0; i<taille; i++) std::cout << "  " << i;
    std::cout << std::endl;

    for (unsigned int i = 0; i < testDij.size(); i++)
    {
        for (unsigned int j = 0; j < testDij[i].size(); j++)
        {
            std::cout << j << " ";

            for (unsigned int k = 0; k < testDij[i][j].size(); k++)
            {
                std::cout << testDij[i][j][k] << "  ";
            }
            std::cout << std::endl;
        }
    }
}
