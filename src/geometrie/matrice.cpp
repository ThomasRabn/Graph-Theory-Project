#include "matrice.h"
#include "cmath"

/// Constructeur par défaut, matrice identité
Matrice::Matrice()
    // Mystérieusement il faut ajouter une paire de { }
    : m_mat{{ {1, 0}, {0, 1} }}
{ }

/// Constructeur avec tous les coefficients
Matrice::Matrice(std::array<Coords, 2> mat)
    : m_mat{mat}
{ }

/// Fabrique de matrice transposée
Matrice Matrice::transpose() const
{
    return {{ Coords{m_mat[0].getX(),  m_mat[1].getX()},
              Coords{m_mat[0].getY(),  m_mat[1].getY()} }};
}

/// Fabrique de matrice de rotation, angle radians, sens trigo
Matrice Matrice::rotation(double angle)
{
    double ca = cos(angle);
    double sa = sin(angle);

    return {{ Coords{ca,  sa},
              Coords{-sa, ca} }};
}

/// Fabrique de matrice d'homothétie
Matrice Matrice::homothetie(double rapport)
{
    return {{ Coords{rapport,  0},
              Coords{0,        rapport} }};
}

/// Fabrique de matrice de symetrie
Matrice Matrice::symetrie(double angle)
{
    angle*=2;
    double ca = cos(angle);
    double sa = sin(angle);

    return {{ Coords{ca,  -sa},
              Coords{-sa, -ca} }};
}

/// Produit matrice vecteur (appliquer la matrice)
Coords operator*(const Matrice& mat, const Coords& v)
{
    return { mat.m_mat[0]*v, mat.m_mat[1]*v };
}

/// Produit matrice matrice : composition (au sens maths)
Matrice operator*(const Matrice& a, const Matrice& b)
{
    Matrice bt = b.transpose();

    return {{ Coords{ a.m_mat[0]*bt.m_mat[0], a.m_mat[0]*bt.m_mat[1] },
              Coords{ a.m_mat[1]*bt.m_mat[0], a.m_mat[1]*bt.m_mat[1] } }};
}
