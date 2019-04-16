#include "Sommet.h"

Sommet::Sommet(int index, int x, int y){
    setSommet(index, x, y);
}

void Sommet::addArete(int arete) {
    m_aretesSommet.push_back(arete);
}

/// Permet d'afficher en console les aretes adjacentes à un sommet
void Sommet::afficherAretes() const {
    std::cout << "Sommet : " << m_index << std::endl;
    std::cout << "Aretes :" << std::endl;
    for(const auto& it : m_aretesSommet) {
        std::cout << it << std::endl;
    }
    std::cout << std::endl;
}

Sommet::~Sommet()
{}
