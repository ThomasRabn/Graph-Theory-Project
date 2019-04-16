#ifndef SOMMET_H
#define SOMMET_H

#include <vector>
#include <iostream>

class Sommet
{
  public:
    Sommet(int index, int x, int y);

    void setIndex(int index) { m_index = index; }

    /// GETTERS
    int getIndex() const { return m_index; }
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    void setSommet(int index, int x, int y) {
        ( index >= 0 )? this->m_index = index : this->m_index = -1;
        this->m_x = x;
        this->m_y = y;
    }

    /// Permet d'ajouter une arete dans le vecteur d'arete du sommet
    void addArete(int arete);

    /// Permet d'afficher en console les aretes adjacentes � un sommet
    void afficherAretes()const;

    ~Sommet();

  private:
    int m_index, m_x, m_y;  /// L'index du Sommet pour son r�f�rencement et les coordonn�es du Sommet en X/Y
    std::vector<int> m_aretesSommet; /// Vecteur d'aretes adjacentes
};

#endif // SOMMET_H
