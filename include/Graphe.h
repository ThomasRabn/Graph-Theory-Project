#ifndef GRAPHE_H
#define GRAPHE_H

#include <unordered_map>
#include <iomanip>
#include <queue>
#include <sstream>
#include <algorithm>
#include <limits>
#include "Sommet.h"
//#include "Arete.h"
#include "svgfile.h"

class Graphe
{
  private:
      std::vector<Sommet*> m_sommets; /// L'ensemble des Sommets qui composent le graphe
      std::vector<Arete*> m_aretes;   /// L'ensemble des Aretes qui composent le graphe

  public:
    Graphe(std::string nomFichier, std::string nomFichier2);
    Graphe(std::vector<Sommet*> m_sommets, std::vector<Arete*> m_aretes);
    ~Graphe();

    /// GETTERS
    std::vector<Sommet*> getSommets() const { return m_sommets; }
    std::vector<Arete*>  getAretes()  const { return m_aretes; }

    /// Affichage
    void dessiner(Svgfile& svgout, int x = 0, int y = 0, bool toggleText = 0, float coeffTaille = 1);
    void dessiner(Svgfile& svgout, std::vector<bool> vecBool, int x = 0, int y = 0, bool toggleText = 0, float coeffTaille = 1);

    /// Algorithmes
    std::vector<float> resultatGraphe(std::vector<std::vector<std::vector<float>>> matriceBruteForce); /// ensemble des résultats des poids pour la partie 3
    std::vector<float> resultatGraphe(); /// nous retourne un tableau de float qui correspond au résultat du grapheen fonction de chacun des poids possibles
    std::vector<float> resultatGraphe(std::vector<bool> vecBool); /// Retourne comme la fonction non overload mais permet de prendre en compte un tableau de bool (1 = arete prise en compte; 0 = pas prise en compte)

    Graphe parcourKruskal(unsigned int indexOfPoids);

    /// Fonction pour Pareto
    std::vector<std::vector<bool>*> ensembleGraphesPartiels(bool toggleAbove = 0); /// Retourne un vecteur de vecteur de booleens correspondants à tous les graphes partiels de ordre-1 arete
    std::vector<std::vector<bool>*> ensembleArbresCouvrants(std::vector<std::vector<bool>*> vec); /// Retourne un vecteur de bool contenant les configurations couvrantes
    void dessinerPareto(std::vector<int> vecIndicesSolutionsNonDominees, std::vector<std::vector<bool>*> vecSolutionsCouvrantes, /// Dessine sur un fichier svg ce qu'il faut pour Pareto
                        std::vector<std::vector<float>*> vecPoidsSolutions, int minX, int maxX, int minY, int maxY);
    std::vector<int> giveSolutionsNonDominees(std::vector<std::vector<float>*> vecPoidsSolutions, int& maxX, int& minY); /// Retourne un vecteur de int contenant les indices des solutions optimums (qui sont stockees à la fin de vecPoidsSolutions)
    void affichagePareto(bool toggleDijkstra = 0, int indicePoidsDijkstra = 1);

    /*************************
    *************************/

    void trierPoidsAretes(std::vector <Arete*> aretesConnectees, unsigned int poids);
    std::vector <Arete*> trierPoidsAretes2(std::vector <Arete*> aretesConnectees, unsigned int poids);

    int autreSommet(Arete* a, Sommet* s);
    std::vector < std::pair <int, float> > parcoursDijkstra(unsigned int indexOfPoids, Sommet* depart, std::vector<bool>* myBool);

    std::vector < std::vector < std::vector < float > > > dijkstra(unsigned int indexOfPoids, std::vector<std::vector<bool>*> myBool);

    /*************************
    *************************/


    ///Extension d'optimisation du pire chemin en fonction du temps
    Graphe ajoutPireCheminOptimisable(Graphe grapheEntry, Graphe grapheTotal);
};

#endif // GRAPHE_H
