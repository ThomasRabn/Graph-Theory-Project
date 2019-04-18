#ifndef GRAPHE_H
#define GRAPHE_H

#include <unordered_map>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "Sommet.h"
#include "Arete.h"
#include "svgfile.h"

class Graphe
{
  public:
    Graphe(std::string nomFichier, std::string nomFichier2);
    Graphe(std::vector<Sommet*> m_sommets, std::vector<Arete*> m_aretes);

    /// GETTERS
    std::vector<Sommet*> getSommets() const { return m_sommets; }
    std::vector<Arete*>  getAretes()  const { return m_aretes; }

    /// Affichage
    void dessiner(Svgfile& svgout, int x = 0, int y = 0);

    ~Graphe();

    /// Algorithmes

    /// nous retourne un tableau de float qui correspond au résultat du grapheen fonction de chacun des poids possibles
    std::vector<float> resultatGraphe();
    /// Retourne comme la fonction non overload mais permet de prendre en compte un tableau de bool (1 = arete prise en compte; 0 = pas prise en compte)
    std::vector<float> resultatGraphe(std::vector<bool> vecBool);
    Graphe parcourKruskal(unsigned int indexOfPoids);
    std::vector<std::vector<bool>*> ensembleGraphesPartiels();
    std::vector<std::vector<bool>*> ensembleArbresCouvrants(std::vector<std::vector<bool>*> vec);
    void affichagePareto();

  protected:

  private:
      std::vector<Sommet*> m_sommets; /// L'ensemble des Sommets qui composent le graphe
      std::vector<Arete*> m_aretes;   /// L'ensemble des Aretes qui composent le graphe
};

#endif // GRAPHE_H
