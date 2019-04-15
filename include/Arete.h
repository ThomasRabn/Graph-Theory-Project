#ifndef ARETE_H
#define ARETE_H

#include <vector>
#include "Sommet.h"

class Arete
{
  public:
    Arete(int index, int s1, int s2);

    int getIndex() const { return m_index; }
    int getPoids(unsigned int indexOfPoids) const { if(indexOfPoids < m_poids.size())return m_poids[indexOfPoids]; }
    int getS1() const { return m_s1; }
    int getS2() const { return m_s2; }
    void setArete(int index, int s1, int s2) {
        setIndex(index);
        this->m_s1 = s1;
        this->m_s2 = s2;
    }
    void setIndex(int index) {
        ( index >= 0 )? this->m_index = index : this->m_index = -1;
    }
    void setS1(int index) {
        ( index >= 0 )? this->m_s1 = index : this->m_s1 = -1;
    }
    void setS2(int index) {
        ( index >= 0 )? this->m_s2 = index : this->m_s2 = -1;
    }
    void addPoids(float poids) { this->m_poids.push_back(poids); }

    ~Arete();

  protected:

  private:
      int m_index;                  /// index de l'arête pour son référencement
      int m_s1, m_s2;               /// ici on a les deux sommets qui forment l'arête
      std::vector<float> m_poids;   /// poids[0] = coûts et poids[1] = ?(tmpsTrajet)
};

#endif // ARETE_H
