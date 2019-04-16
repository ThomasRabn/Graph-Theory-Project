#include "Graphe.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <map>
#include <utility>
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

    /// On vérifie que les tailles sont les memes
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
    /// On ajoute les aretes au graphe et aux sommets
    for(int i = 0; i < taille; ++i){
        /// On sort les infos du fichier
        fichier >> id;      if(fichier.fail()) throw std::runtime_error("Probleme de lecture de l'id de l'arete " + i);
        fichier >> s1;      if(fichier.fail()) throw std::runtime_error("Probleme de lecture du sommet 1 de l'arete " + i);
        fichier >> s2;      if(fichier.fail()) throw std::runtime_error("Probleme de lecture du sommet 2 de l'arete " + i);
        /// On insert les infos
        m_aretes.insert({id,new Arete{id,s1,s2}}); // On ajoute le numero l'arete dans le tableau
        m_sommets.find(s1)->second->addArete(id);  // On ajoute le numero de l'arete dans le sommet 1
        m_sommets.find(s2)->second->addArete(id);  // On ajoute le numero de l'arete dans le sommet 2
    }

    /// On ajoute le poids des aretes
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

Graphe Graphe::parcourKruskal(unsigned int indexOfPoids) {

    std::unordered_map<int, Sommet*> sommets = getSommets();
    std::unordered_map<int, Arete*> aretes = getAretes();
    std::unordered_map<int, Arete*> aretesFinaux;
    std::vector<int> aretesCroissante;

    std::vector<int> sommetsConnexe; /// l'indice du vector est le numéro du sommet et sa valeur est son indice de connexité

    while (!aretes.empty()){ /// dans cette boucle on tries les arètes en fonction de leurs poids
        float minP = aretes.begin()->second->getPoids(indexOfPoids);
        int minId = aretes.begin()->second->getIndex();

        for (auto it: aretes){
          if ( it.second->getPoids(indexOfPoids) < minP){
            minP = it.second->getPoids(indexOfPoids);
            minId = it.second->getIndex();
          }
        }
        aretesCroissante.push_back(minId);
        aretes.erase(aretes.find(minId));
    }
    aretes = getAretes(); /// pour reprendre le tableau car on vient de le modifier

    for (const auto& it: sommets){/// on insère dans le vector les index des sommets
      sommetsConnexe.push_back(it.second->getIndex()); ///première valeur pour l'index du sommet et la seconde valeur pour sa "couleur"
    }
    std::sort(sommetsConnexe.begin(), sommetsConnexe.end()); /// on trie le vector pour alligner l'indice à sa valeur de connexité

    /*for (const auto& current: aretesCroissante)std::cout << current << " / ";
    std::cout << std::endl;*/

    unsigned int nbAdd = 1;
    unsigned int boucle = 0;
    while (nbAdd < sommets.size()){

        //std::cout << "-------------------------------------------" << std::endl;
        auto it = aretes.find(aretesCroissante[boucle]);

        //std::cout << "Index de l'aretes en cours " << it->second->getIndex() << std::endl;

        int s1 = sommetsConnexe[it->second->getS1()];
        int s2 = sommetsConnexe[it->second->getS2()];
        if ( (s1 != s2) && (aretesFinaux.find(it->second->getIndex()) == aretesFinaux.end()) ){

            nbAdd++;
            //std::cout << "on insere l'arete: " << it->second->getIndex() << std::endl;
            aretesFinaux.insert(*it);

            for (unsigned int i=0 ; i<sommetsConnexe.size() ; ++i){

                if (sommetsConnexe[i] == s2){
                    //std::cout << "on modifie la valeur " << i << " de " << s2 << " a " << s1 <<  std::endl;
                    sommetsConnexe[i] = s1;
                }
            }

            /*std::cout << "tableau de connexite     ";
            for (const auto& i:sommetsConnexe)std::cout << i << '/';
            std::cout << std::endl;*/
        }
        boucle++;
    }
    /*std::cout << "tableau de connexite     ";
    for (const auto& i:sommetsConnexe)std::cout << i << '/';
    std::cout << std::endl;
    for (const auto& i:aretesFinaux)std::cout << i.second->getIndex() << '/';*/
    Graphe myGraphe {sommets, aretesFinaux};
    return myGraphe;
}

void Graphe::dessiner(Svgfile& svgout) {
    int s1, s2, x1, y1, x2, y2;
    /// DESSIN DES ARETES
    for(const auto& it : m_aretes) {
        /// On prend les infos
        s1 = it.second->getS1();
        s2 = it.second->getS2();
        x1 = m_sommets.find(s1)->second->getX();
        y1 = m_sommets.find(s1)->second->getY();
        x2 = m_sommets.find(s2)->second->getX();
        y2 = m_sommets.find(s2)->second->getY();

        /// On dessine une ligne
        svgout.addLine(x1, y1, x2, y2);
        int dy = y2-y1;
        std::string poids = "(";

        /// Affichage du poids
        std::vector<float> vecPoids = it.second->getPoids();
        for(unsigned int i = 0; i < vecPoids.size(); ++i) {
            std::stringstream number;
            number << std::fixed << std::setprecision(2) << vecPoids[i];
            poids.append(number.str());
            if(i != vecPoids.size()-1)    poids.append(" ; ");
            else                        poids.append(")");
        }

        /// Permet de savoir où sur l'écran on affiche le texte
        if((dy > 5) || (dy < -5)) {
            svgout.addText(((x1 + x2)/2)+5, ((y1+y2)/2), poids);
        } else {
            svgout.addText(((x1 + x2)/2)-50, ((y1+y2)/2)-10, poids);
        }
    }

    /// DESSIN DES SOMMETS
    for(const auto& it : m_sommets) {
        x1 = it.second->getX();
        y1 = it.second->getY();
        svgout.addDisk(x1, y1, 7);
    }
}

Graphe::~Graphe()
{}
