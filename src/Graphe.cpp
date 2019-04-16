#include "Graphe.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <algorithm>

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
    std::map<int, int> sommetsConnexe;

    std::vector<int> aretesCroissante;

    while (!aretes.empty()){
        float minP = aretes.begin()->second->getPoids(indexOfPoids);
        int minId = aretes.begin()->second->getIndex();

        for (auto it: aretes){
          if ( it.second->getPoids(indexOfPoids) < minP){
            minP = it.second->getPoids(indexOfPoids);
            minId = it.second->getIndex();
          }
        }

        aretesCroissante.push_back(minId);
        /// Cout test std::cout << "suppr " << minId << std::endl;
        aretes.erase(aretes.find(minId));
    }

    aretes = getAretes(); /// pour reprendre le tableau car on vient de le modifier
    for (const auto& it: sommets){
      sommetsConnexe.insert({it.second->getIndex(), it.second->getIndex()}); ///première valeur pour l'index du sommet et la seconde valeur pour sa "couleur"
    }

    unsigned int nbAdd = 0;

    while (nbAdd < sommets.size()){

        for (const auto& it: aretes){
          int s1 = sommetsConnexe.find(it.second->getS1())->second;
          int s2 = sommetsConnexe.find(it.second->getS2())->second;
          if ( s1 != s2 ){

            /// Cout test std::cout << "S1 " << s1 << " et S2 " << s2 << std::endl;
            aretesFinaux.insert(it);
            sommetsConnexe.find(it.second->getS2())->second = sommetsConnexe.find(it.second->getS1())->second;

            for (auto its: sommetsConnexe){
              if ( its.second == s2) its.second = s1;
            }

            /// Cout test std::cout << "S1 " << sommetsConnexe.find(it.second->getS1())->second << " et S2 " << sommetsConnexe.find(it.second->getS2())->second << std::endl;
            /// Cout test std::cout << "--------------------------" << std::endl;

          }
        }
        nbAdd++;
    }

    /// Cout test for (auto its: sommetsConnexe)std::cout << " valeur final " << its.second << std::endl;

    Graphe myGraphe {sommets, aretesFinaux};
    return myGraphe;
}







