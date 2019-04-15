#ifndef GRAPHE_H
#define GRAPHE_H

#include <unordered_map>
#include "Sommet.h"
#include "Arete.h"

class Graphe
{
  public:
    Graphe();
    void setGraphe();
    Graphe getGraphe();
    ~Graphe();

  protected:

  private:
      std::unordered_map<int, Sommet*> m_sommets; /// L'ensemble des Sommets qui composent le graphe
      std::unordered_map<int, Arete*> m_aretes;   /// L'ensemble des Arêtes qui composent le graphe
};

#endif // GRAPHE_H
