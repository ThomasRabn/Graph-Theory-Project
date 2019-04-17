#ifndef GRAPHE_H
#define GRAPHE_H

#include <iostream>
#include <vector>
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
    Graphe(std::vector<Sommet*> m_sommets, std::vector<Arete*> m_aretes);

    /// GETTERS
    std::vector<Sommet*> getSommets() const { return m_sommets; }
    std::vector<Arete*>  getAretes()  const { return m_aretes; }

    /// Affichage
    void dessiner(Svgfile& svgout, int x = 0, int y = 0);

    ~Graphe();

    /// Algorithme
    Graphe parcourKruskal(unsigned int indexOfPoids);
    std::vector<std::vector<bool>*> ensembleGraphesPartiels();


  protected:

  private:
      std::vector<Sommet*> m_sommets; /// L'ensemble des Sommets qui composent le graphe
      std::vector<Arete*> m_aretes;   /// L'ensemble des Ar�tes qui composent le graphe
};

#endif // GRAPHE_H
