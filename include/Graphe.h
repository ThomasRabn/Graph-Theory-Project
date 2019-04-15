#ifndef GRAPHE_H
#define GRAPHE_H

#include <iostream>
#include <unordered_map>
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include "Sommet.h"
#include "Arete.h"
#include "svgfile.h"

class Graphe
{
  public:
    Graphe(std::string nomFichier, std::string nomFichier2);

    /// GETTERS
    std::unordered_map<int, Sommet*> getSommets() const { return m_sommets; }
    std::unordered_map<int, Arete*> getAretes() const { return m_aretes; }

    /// Affichage
    void dessiner(Svgfile& svgout);

    ~Graphe();

    /// Algorithme
    Graphe parcourKruskal();

  protected:

  private:
      std::unordered_map<int, Sommet*> m_sommets; /// L'ensemble des Sommets qui composent le graphe
      std::unordered_map<int, Arete*> m_aretes;   /// L'ensemble des Arêtes qui composent le graphe
};

#endif // GRAPHE_H
