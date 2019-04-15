#ifndef CADRE_H_INCLUDED
#define CADRE_H_INCLUDED

#include "coords.h"

/// Cadre rectangulaire (utile par exemple pour sélectionner)
/// Les attributs sont des Coords hautGauche et basDroit
class Cadre
{
    public :
        Cadre();
        Cadre(const Coords& position, const Coords& taille);

        void afficher() const;
        void saisir();

        /// Bordure incluse (dedans ou sur le bord)
        bool contient(const Coords& c) const;

        /// Bordure exclue (ni dedans ni sur le bord)
        bool neContientPas(const Coords& c) const;

    private :
        Coords m_hautGauche;
        Coords m_basDroite;
};

#endif // CADRE_H_INCLUDED
