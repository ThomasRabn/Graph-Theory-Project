#include "transformation.h"
#include <cmath>

/// Une honte : PI n'est toujours pas en standard dans std:: !
const double PI = 3.1415926535897932384626;

/// Ceci pourrait aller dans util ?
static double deg2rad(double deg)
{
    return (PI/180)*deg;
}

/// Transformation identité
Transformation::Transformation()
    : m_matrice{}, m_centre{}, m_translation{}
{ }

Transformation::Transformation(const Translation& translation)
    : m_matrice{},
      m_centre{},
      m_translation{translation.vecteur}
{ }

Transformation::Transformation(const Rotation& rotation)
    : m_matrice{ Matrice::rotation( deg2rad(rotation.angle) ) },
      m_centre{rotation.centre},
      m_translation{}
{ }

Transformation::Transformation(const Homothetie& homothetie)
    : m_matrice{ Matrice::homothetie(homothetie.rapport) },
      m_centre{homothetie.centre},
      m_translation{}
{ }

Transformation::Transformation(const Symetrie& symetrie)
    : m_matrice{ Matrice::symetrie( deg2rad(symetrie.angle) ) },
      m_centre{symetrie.centre},
      m_translation{}
{ }

void Transformation::appliquer(Coords& c) const
{
    c = resultat(c);
}

Coords Transformation::resultat(const Coords& c) const
{
    return m_matrice*(c-m_centre) + m_centre + m_translation;
}
