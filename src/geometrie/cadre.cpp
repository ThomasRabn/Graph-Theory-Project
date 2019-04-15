#include "cadre.h"
#include <iostream>

Cadre::Cadre()
    : m_hautGauche{}, m_basDroite{}
{ }

Cadre::Cadre(const Coords& hautGauche, const Coords& basDroite)
    : m_hautGauche{hautGauche}, m_basDroite{basDroite}
{ }

void Cadre::afficher() const
{
    m_hautGauche.afficher();
    m_basDroite.afficher();
}

void Cadre::saisir()
{
    std::cout << "Coords haut gauche\n";
    m_hautGauche.saisir();
    std::cout << "Coords bas droite\n";
    m_basDroite.saisir();
}

bool Cadre::contient(const Coords& c) const
{
    return  !neContientPas(c);
}

bool Cadre::neContientPas(const Coords& c) const
{
    return  c.estAGauche(m_hautGauche)
         || c.estADroite(m_basDroite)
         || c.estAuDessus(m_hautGauche)
         || c.estAuDessous(m_basDroite) ;
}
