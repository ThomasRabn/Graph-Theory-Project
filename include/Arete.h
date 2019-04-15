#ifndef ARETE_H
#define ARETE_H

#include <vector>
#include "Sommet.h"

class Arete
{
  public:
    Arete(int index, int s1, int s2);
    int getIndex() const { return m_index; }
    void setArete(int index, int s1, int s2) {
        ( index >= 0 )? this->m_index = index : this->m_index = -1;
        this->m_s1 = s1;
        this->m_s2 = s2;
    }
    void addPoids(float poids) { this->m_poids.push_back(poids); }
    ~Arete();

  protected:

  private:
      int m_index;                  /// index de l'ar�te pour son r�f�rencement
      int m_s1, m_s2;               /// ici on a les deux sommets qui forment l'ar�te
      std::vector<float> m_poids;   /// poids[0] = co�ts et poids[1] = ?(tmpsTrajet)
};

#endif // ARETE_H
