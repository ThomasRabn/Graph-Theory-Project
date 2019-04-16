#include "Arete.h"

Arete::Arete(int index, int s1, int s2){
    setArete(index, s1, s2);
}

Arete::Arete(int index, int s1, int s2, std::vector<float> vecPoids)
: m_index{index}, m_s1{s1}, m_s2{s2}, m_poids{vecPoids}
{}
