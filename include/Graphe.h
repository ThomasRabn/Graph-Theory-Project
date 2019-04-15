#ifndef GRAPHE_H
#define GRAPHE_H

#include <unordered_map>
#include "Sommet.h"
#include "Arete.h"

class Graphe
{
  public:
    Graphe(std::string nomFichier, std::string nomFichier2);

    std::unordered_map<int, Sommet*> getSommets() const { return m_sommets; }
    std::unordered_map<int, Arete*> getAretes() const { return m_aretes; }

    ~Graphe();

    /// algorithme
    Graphe parcourKruskal();

  protected:

  private:
      std::unordered_map<int, Sommet*> m_sommets; /// L'ensemble des Sommets qui composent le graphe
      std::unordered_map<int, Arete*> m_aretes;   /// L'ensemble des Arêtes qui composent le graphe
};

#endif // GRAPHE_H
