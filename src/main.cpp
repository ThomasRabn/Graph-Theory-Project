#include "geometrie/transformation.h"
#include "graphisme/couleur.h"
#include "svg/svgfile.h"
#include "util/util.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void transfoTest();

int main()
{
    transfoTest();
    return 0;
}

/// Code initial pour comprendre les transformations géométriques
void transfoTest()
{
    /// Sortie graphique dans le fichier output.svg
    /// ( options à voir svgfile.h au niveau du constructeur )
    Svgfile svgout;

    /// Dessin du repère cartésien (facultatif)
    svgout.addGrid();

    /// Mutation de couleur
    Couleur jaune{255, 200, 0};
    for (double x=100; x<=900; x+=50)
    {
        svgout.addDisk(x, 100, 30, jaune);
        /// Chaque canal modifié aléatoirement de [-40...+40]
        jaune.muter(40);
    }

    /// Coordonnées aléatoire
    for (double x=100; x<=900; x+=100)
    {
        Coords posBase{x, 200};

        for (int i=0; i<8; ++i)
        {
            /// Coords::aleatoire est une méthode statique de classe
            /// qui renvoie un objet (c'est une sorte de constructeur)
            /// Voir Coords .h et .cpp
            Coords posAlea = posBase + Coords::aleatoire(50);

            svgout.addLine(posBase.getX(), posBase.getY(),
                           posAlea.getX(), posAlea.getY(), "black");
        }
    }

    /// On défini des positions initiales d'un triangle...
    Coords c1{100, 300};
    Coords c2{200, 500};
    Coords c3{100, 500};
    svgout.addTriangle(c1.getX(), c1.getY(), c2.getX(), c2.getY(), c3.getX(), c3.getY(), "green");

    /// Translation de vecteur 200 0
    {
        Coords vecteur{200, 0};
        Transformation transfo{ Translation{vecteur} };
        transfo.appliquer(c1);
        transfo.appliquer(c2);
        transfo.appliquer(c3);
        svgout.addTriangle(c1.getX(), c1.getY(), c2.getX(), c2.getY(), c3.getX(), c3.getY(), "red");
    }

    /// Rotation de centre 300 500 d'angle +30° (sens trigo)
    {
        Coords centre{300, 500};
        double angle = 30;
        Transformation transfo{ Rotation{centre, angle} };
        transfo.appliquer(c1);
        transfo.appliquer(c2);
        transfo.appliquer(c3);
        svgout.addTriangle(c1.getX(), c1.getY(), c2.getX(), c2.getY(), c3.getX(), c3.getY(), "blue");
    }

    /// Symétrie de centre 400 500 par rapport à l'axe à 90° (vertical)
    {
        Coords centre{400, 500};
        double angle = 90;
        Transformation transfo{ Symetrie{centre, angle} };
        transfo.appliquer(c1);
        transfo.appliquer(c2);
        transfo.appliquer(c3);
        svgout.addTriangle(c1.getX(), c1.getY(), c2.getX(), c2.getY(), c3.getX(), c3.getY(), "orange");
    }

    /// Symétrie de centre 500 500 par rapport à l'axe à 0° (horizontale)
    /// ici on conserve les coordonnées de départ en utilisant la méthode résultat
    /// qui donne une copie transformée
    {
        Coords centre{500, 500};
        double angle = 0;
        Transformation transfo{ Symetrie{centre, angle} };
        Coords d1 = transfo.resultat(c1);
        Coords d2 = transfo.resultat(c2);
        Coords d3 = transfo.resultat(c3);
        svgout.addTriangle(d1.getX(), d1.getY(), d2.getX(), d2.getY(), d3.getX(), d3.getY(), "pink");
    }

    /// Homothetie de centre 200 400 de rapport 1.5
    {
        Coords centre{200, 400};
        double rapport = 1.5;
        Transformation transfo{ Homothetie{centre, rapport} };
        transfo.appliquer(c1);
        transfo.appliquer(c2);
        transfo.appliquer(c3);
        svgout.addTriangle(c1.getX(), c1.getY(), c2.getX(), c2.getY(), c3.getX(), c3.getY(), "brown");
    }

    /// Homotheties de centre 650 450 de rapport 0.8
    Couleur coul{128,128,128};
    for (int i=0; i<80; ++i)
    {
        Coords centre{650, 450};
        double rapport = 0.9;
        Transformation transfo{ Homothetie{centre, rapport} };
        transfo.appliquer(c1);
        transfo.appliquer(c2);
        transfo.appliquer(c3);
        svgout.addTriangle(c1.getX(), c1.getY(), c2.getX(), c2.getY(), c3.getX(), c3.getY(), coul);
        coul.muter(20);
    }

    /// L'objet svgout est automatiquement libéré à la sortie
    /// de ce sous-programme : le fichier output.svg est alors fermé
}


