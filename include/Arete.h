#ifndef ARETE_H
#define ARETE_H

#include <vector>
#include "Sommet.h"

class Arete
{
  public:
    Arete(int index, int s1, int s2);

    void setArete(int index, int s1, int s2) {
        ( index >= 0 )? this->m_index = index : this->m_index = -1;
        this->m_s1 = s1;
        this->m_s2 = s2;
    }
    void addPoids(float poids) { this->m_poids.push_back(poids); }

    /// GETTERS
    int getS1() const    { return m_s1; }
    int getS2() const    { return m_s2; }
    std::vector<float> getPoids() const { return m_poids; }
    int getIndex() const { return m_index; }

    ~Arete();

  protected:

  private:
      int m_index;                  /// index de l'arête pour son référencement
      int m_s1, m_s2;               /// ici on a les deux sommets qui forment l'arête
      std::vector<float> m_poids;   /// poids[0] = coûts et poids[1] = ?(tmpsTrajet)
};

#endif // ARETE_H
