#ifndef SOMMET_H
#define SOMMET_H

class Sommet
{
  public:
    Sommet(int index, int x, int y);

    int getIndex() const { return m_index; }
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    void setSommet(int index, int x, int y) {
        ( index >= 0 )? this->m_index = index : this->m_index = -1;
        this->m_x = x;
        this->m_y = y;
    }

    ~Sommet();

  protected:

  private:
      int m_index, m_x, m_y; /// L'indexe du Sommet pour son référencement
                             /// Les cordonnés du Sommet en X/Y
};

#endif // SOMMET_H
