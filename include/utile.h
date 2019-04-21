#ifndef UTILE_H_INCLUDED
#define UTILE_H_INCLUDED

#include "svgfile.h"
#include "Graphe.h"
#include <windows.h>
#include <conio.h>
#include <cstdlib>

void gotoXY(int x,int y); // Utilise dans le menu interactif
void menu(); /// Permet de se déplacer dans un menu et de faire des choix
void choisirFichiers(std::string& fichier, std::string& fichierPoids); /// Permet de choisir un fichier de graphe
void choisirFichiersExtension(std::string& fichier, std::string& fichierPoids, std::string& fichierAOptimiser, std::string& fichierPoidsAOptimiser); /// Choix spécial pour l'extension
int choisirPoids(Graphe* myGraphe); /// Permet de lancer un menu pour choisir sur quel poids lancer l'algo

template<typename T>
void entreeClavier(T& value) {
    do{
        if(!(std::cin >> value)) {
            std::cout << "Mauvaise entree" << std::endl;
            std::cin.clear(); // efface les bits d'erreurs
            std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' ); // supprime la ligne erronee dans le buffer
        } else{ break; }
    } while(true);
}

#endif // UTILE_H_INCLUDED
