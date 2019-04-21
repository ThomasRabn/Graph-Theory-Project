#ifndef GRAPHE_H
#define GRAPHE_H

#include <unordered_map>
#include <iomanip>
#include <queue>
#include <sstream>
#include <limits>
#include "time.h"
#include "Sommet.h"
#include "Arete.h"
#include "svgfile.h"

class Graphe
{
  private:
      std::vector<Sommet*> m_sommets; /// L'ensemble des Sommets qui composent le graphe
      std::vector<Arete*> m_aretes;   /// L'ensemble des Aretes qui composent le graphe

  public:
    Graphe(std::string nomFichier, std::string nomFichier2);
    Graphe(std::vector<Sommet*> m_sommets, std::vector<Arete*> m_aretes);
    ~Graphe() = default;
    void libererMemoire(); /// Permet d'automatiquement vider la memoire prise par le graphe (on ne le met pas dans le destructeur car sinon lorsqu'on supprime un graphe partiel on supprime le graphe supérieur aussi)

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
    std::vector<bool>  grapheToBool(Graphe g); /// Transforme un graphe en vecteur de booleen

    /// Kruskal
    Graphe parcourKruskal(unsigned int indexOfPoids);

    /// Fonction pour Pareto
    std::vector<std::vector<bool>*> ensembleGraphesPartiels(int toggleAbove = 0, std::vector<bool> vec1 = {0}, std::vector<bool> vec2 = {0}); /// Retourne un vecteur de vecteur de booleens correspondants à tous les graphes partiels de ordre-1 arete
    std::vector<std::vector<bool>*> ensembleArbresCouvrants(std::vector<std::vector<bool>*> vec); /// Retourne un vecteur de bool contenant les configurations couvrantes
    void dessinerPareto(std::vector<int> vecIndicesSolutionsNonDominees, std::vector<std::vector<bool>*> vecSolutionsCouvrantes, /// Dessine sur un fichier svg ce qu'il faut pour Pareto
                         std::vector<std::vector<float>*> vecPoidsSolutions, int minX, int maxX, int minY, int maxY, std::vector<std::vector<float>*> vecPoidsSolutionsNonTriees, bool toggleDijkstra);
    std::vector<int> giveSolutionsNonDominees(std::vector<std::vector<float>*> vecPoidsSolutions, int& maxX, int& minY); /// Retourne un vecteur de int contenant les indices des solutions optimums (qui sont stockees à la fin de vecPoidsSolutions)
    void affichagePareto(int toggleDijkstra = 0, int indicePoidsDijkstra = 1);

    /// fonctions permettant le fonctionnement de Dijkstra
    void trierPoidsAretes(std::vector <Arete*> aretesConnectees, unsigned int poids);
    int autreSommet(Arete* a, Sommet* s);

    /// Dijkstra
    std::vector < std::pair <int, float> > parcoursDijkstra(unsigned int poids, Sommet* depart, std::vector<bool>* myBool, int mode);
    std::vector < std::vector < std::vector < float > > > dijkstra(unsigned int indexOfPoids, std::vector<std::vector<bool>*> myBool);
    std::vector < std::vector < std::vector <int> > > pireDijkstra(unsigned int indexOfPoids);

    void pireCheminDijkstra(Graphe origine, Graphe &optimisable);
};

#endif // GRAPHE_H
