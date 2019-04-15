#include "Graphe.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include<unordered_map>
#include<unordered_set>
#include <algorithm>

//bool comparePoids(const Arete* a, const Arete* b, unsigned int indexOfPoids) { return ((a->getPoids(indexOfPoids) > b->getPoids(indexOfPoids)); } ///fonction pour comparer 2 poids entre eux

Graphe::Graphe(std::string nomFichier, std::string nomFichier2){
    std::ifstream fichier{nomFichier};
    std::ifstream fichier2{nomFichier2};
    if (!fichier) /// Si on a un probleme on lance une erreur
        throw std::runtime_error("Erreur d'ouverture du fichier " + nomFichier + ", vérifiez le nom et le fichier");
    if (!fichier2) /// Si on a un probleme on lance une erreur
        throw std::runtime_error("Erreur d'ouverture du fichier " + nomFichier2 + ", vérifiez le nom et le fichier");
    int ordre;
    fichier >> ordre;  /// On met la valeur présente dans le fichier dans ordre
    if (fichier.fail())
        throw std::runtime_error("Probleme de lecture de l'ordre du graphe");
    int id, x, y;
    /// On lit les sommets et on met les valeurs de x et de y dans la map du graphe
    for(int i = 0; i < ordre; ++i){
        fichier >> id;  if(fichier.fail()) throw std::runtime_error("Probleme de lecture de l'id du sommet " + i);
        fichier >> x;   if(fichier.fail()) throw std::runtime_error("Probleme de lecture de l'abscisse du sommet " + i);
        fichier >> y;   if(fichier.fail()) throw std::runtime_error("Probleme de lecture de l'ordonnée du sommet " + i);
        m_sommets.insert({id,new Sommet{id,x,y}});
    }

    int taille, taille2;
    fichier >> taille;
    if (fichier.fail())
        throw std::runtime_error("Probleme de lecture de la taille du graphe dans " + nomFichier);
    fichier2 >> taille2;
    if (fichier2.fail())
        throw std::runtime_error("Probleme de lecture de la taille du graphe dans " + nomFichier2);
    if (taille != taille2)
        throw std::runtime_error("La taille du graphe n'est pas la meme dans " + nomFichier + " et dans " + nomFichier2);

    int s1, s2;
    for(int i = 0; i < taille; ++i){
        fichier >> id;      if(fichier.fail()) throw std::runtime_error("Probleme de lecture de l'id de l'arete " + i);
        fichier >> s1;      if(fichier.fail()) throw std::runtime_error("Probleme de lecture du sommet 1 de l'arete " + i);
        fichier >> s2;      if(fichier.fail()) throw std::runtime_error("Probleme de lecture du sommet 2 de l'arete " + i);
        m_aretes.insert({id,new Arete{id,s1,s2}});
    }

    int nbrPoids;
    float poids;
    fichier2 >> nbrPoids;
    for(int i = 0; i < taille; ++i){
        fichier2 >> id;
        for(int j = 0; j < nbrPoids; ++j){
            fichier2 >> poids;
            ((m_aretes.find(id))->second)->addPoids(poids);
        }
    }
}

Graphe::Graphe(std::unordered_map<int, Sommet*> sommets, std::unordered_map<int, Arete*> aretes)
    :m_sommets{sommets}, m_aretes{aretes}
{}


Graphe::~Graphe()
{}

Graphe Graphe::parcourKruskal(unsigned int indexOfPoids) {

    std::unordered_map<int, Sommet*> sommets = getSommets();
    std::unordered_map<int, Arete*> aretes = getAretes();

    std::unordered_map<int, Arete*> aretesFinaux;

    unsigned int nbAdd = 0;
    //std::sort(aretes.begin(), aretes.end(), comparePoids);

    while (nbAdd < sommets.size()){
        for (auto it: aretes){
            //std::cout << it.second->getS1() << std::endl;
            if ( it.second->getS1() != it.second->getS2() ){

                std::cout << "S1 " << it.second->getS1() << " et S2 " << it.second->getS2() << std::endl;

                aretesFinaux.insert(it);

                it.second->setS2(it.second->getS1());

                std::cout << "S1 " << it.second->getS1() << " et S2 " << it.second->getS2() << std::endl;

                std::cout << "--------------------------" << std::endl;
            }
        }
        nbAdd++;
    }

    Graphe myGraphe {sommets, aretes};
    return myGraphe;
}







