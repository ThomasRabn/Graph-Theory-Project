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

/// Retourne un vecteur de Graphes contenant tous le sous-graphes possibles
std::vector<Graphe*> Graphe::ensembleGraphesPartiels() {
    std::vector<Graphe*> graphesPartiels;
    std::vector<bool> etats, allTrue;
    for(unsigned int i = 0; i < m_aretes.size(); ++i) {
        etats.push_back(0);
        allTrue.push_back(1);
    }
    bool run = 1;
    while(run) {
        std::unordered_map<int, Arete*> aretes;
        unsigned int compteur = 0; /// Permet de trouver l'arete que l'on veut dans la map d'arete (est un id arbitraire)
        //std::cout << std::endl;
        for(const auto& it : etats) {
            //std::cout << it;
            if(it) {
                Arete* areteCourante = m_aretes.find(compteur)->second;
                int id = areteCourante->getIndex();
                int sommet1 = areteCourante->getS1();
                int sommet2 = areteCourante->getS2();
                std::vector<float> vecPoids = areteCourante->getPoids();
                aretes.insert({id, new Arete{id, sommet1, sommet2, vecPoids}});
            }
            compteur++;
        }
        graphesPartiels.push_back(new Graphe{m_sommets, aretes});

        if (etats == allTrue)   run = 0; /// Arrete la boucle si on a fait tous les tests
        else{ /// Augmente de 1 la valeur binaire enregistrée dans le tableau de booléens (Poids le plus lourd a la fin)
            std::vector<bool> changementEtat; // Une case contient un 1 si l'etat a été changé ce tour, un 0 sinon
            for(unsigned int i = 0; i < etats.size(); ++i)  { changementEtat.push_back(0); }
            for(unsigned int i = 0; i < etats.size(); ++i) {
                if (i == 0)  { etats[i] = !etats[i]; changementEtat[i] = 1; }
                else{
                    if ((changementEtat[i-1] == 1) && (etats[i-1] == 0))    { etats[i] = !etats[i]; changementEtat[i] = 1; }
                }
            }
        }
    }

    return graphesPartiels;
}

void Graphe::dessiner(Svgfile& svgout, int x, int y) {
    int s1, s2, x1, y1, x2, y2;
    /// DESSIN DES ARETES
    for(const auto& it : m_aretes) {
        /// On prend les infos
        s1 = it.second->getS1();
        s2 = it.second->getS2();
        x1 = m_sommets.find(s1)->second->getX()+x;
        y1 = m_sommets.find(s1)->second->getY()+y;
        x2 = m_sommets.find(s2)->second->getX()+x;
        y2 = m_sommets.find(s2)->second->getY()+y;

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
        x1 = it.second->getX()+x;
        y1 = it.second->getY()+y;
        svgout.addDisk(x1, y1, 7);
    }
}

Graphe::~Graphe()
{
    for(auto& ptr : m_sommets) {
        delete ptr.second;
    }
    for(auto& ptr : m_aretes) {
        delete ptr.second;
    }
}
