#include "Graphe.h"

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
        m_sommets.push_back(new Sommet{id,x,y});
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
        m_aretes.push_back(new Arete{id,s1,s2}); // On ajoute le numero l'arete dans le tableau
        m_sommets[s1]->addArete(id);             // On ajoute le numero de l'arete dans le sommet 1
        m_sommets[s2]->addArete(id);             // On ajoute le numero de l'arete dans le sommet 2
    }

    /// On ajoute le poids des aretes
    int nbrPoids;
    float poids;
    fichier2 >> nbrPoids;
    for(int i = 0; i < taille; ++i){
        fichier2 >> id;
        for(int j = 0; j < nbrPoids; ++j){
            fichier2 >> poids;
            m_aretes[id]->addPoids(poids);
        }
    }
}

Graphe::Graphe(std::vector<Sommet*> sommets, std::vector<Arete*> aretes)
    :m_sommets{sommets}, m_aretes{aretes}
{}

Graphe Graphe::parcourKruskal(unsigned int indexOfPoids) {
    std::vector<Sommet*> vecSommets = getSommets();
    std::vector<Arete*> vecAretes = getAretes();

    std::unordered_map<int, Sommet*> sommets;
    std::unordered_map<int, Arete*> aretes;
    std::unordered_map<int, Arete*> aretesFinaux;
    std::vector<int> aretesCroissante;
    std::vector<int> sommetsConnexe; /// l'indice du vector est le numéro du sommet et sa valeur est son indice de connexité

      for(auto it : vecSommets) {
        sommets.insert({it->getIndex(), it});
      }
      for(auto it : vecAretes) {
        aretes.insert({it->getIndex(), it});
      }

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
    /// pour reprendre le tableau car on vient de le modifier
    for(auto it : vecAretes) {
        aretes.insert({it->getIndex(), it});
      }

    for (const auto& it: sommets){/// on insère dans le vector les index des sommets
      sommetsConnexe.push_back(it.second->getIndex()); ///première valeur pour l'index du sommet et la seconde valeur pour sa "couleur"
    }
    std::sort(sommetsConnexe.begin(), sommetsConnexe.end()); /// on trie le vector pour alligner l'indice à sa valeur de connexité


    /*for (const auto& current: aretesCroissante)std::cout << current << " / ";
    std::cout << std::endl;*/

    unsigned int nbAdd = 1; /// on compte le nombre d'arete pour savoir quand tous les sommets sont reliés
    unsigned int boucle = 0; /// en fonction du nombre de boucle qu'on a fait on peut savoir qu'elle arete sélectionner (par ordre croissant)
    while (nbAdd < sommets.size()){

        //std::cout << "-------------------------------------------" << std::endl;
        auto it = aretes.find(aretesCroissante[boucle]);
        //std::cout << "Index de l'aretes en cours " << it->second->getIndex() << std::endl;
        int s1 = sommetsConnexe[it->second->getS1()]; /// on sélectionne le sommet 1 de l'arete
        int s2 = sommetsConnexe[it->second->getS2()]; /// on sélectionne le sommet 2 de l'arete
        if ( (s1 != s2) && (aretesFinaux.find(it->second->getIndex()) == aretesFinaux.end()) ){ /// si les sommets 1 et 2 ne sont pas connexes et qu'on peut ajouter l'arete alors:
            nbAdd++;
            //std::cout << "on insere l'arete: " << it->second->getIndex() << std::endl;
            aretesFinaux.insert(*it);
            for (unsigned int i=0 ; i<sommetsConnexe.size() ; ++i){
                if (sommetsConnexe[i] == s2){
                    //std::cout << "on modifie la valeur " << i << " de " << s2 << " a " << s1 <<  std::endl;
                    sommetsConnexe[i] = s1; /// on met tous les sommets de même connexité que le sommet 2 à la valeur de connexité du sommet 1
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
    std::vector<Arete*> vecAretesFinales;
    for (auto it: aretesFinaux){
      vecAretesFinales.push_back(it.second);
    }
    Graphe myGraphe {vecSommets, vecAretesFinales};
    return myGraphe;
}

/// Retourne un vecteur de bool contenant tous le sous-graphes de Ordre-1 aretes. vec[i] = 1 -> arete n°i ajoutee.
std::vector<std::vector<bool>*> Graphe::ensembleGraphesPartiels() {
    std::vector<std::vector<bool>*> graphesPartiels;
    std::vector<bool> etats (m_aretes.size(), 0), allTrue(m_aretes.size(), 1);
    unsigned int nbrAretes = 0; // Suit le nombre d'aretes presentes dans le graphe partiel créé
    bool run = 1;

    while(run) {
        if (nbrAretes == m_sommets.size()-1)    graphesPartiels.push_back(new std::vector<bool>{etats});

        if (etats == allTrue)   run = 0; // Arrete la boucle si on a fait tous les tests
        else{ /// Augmente de 1 la valeur binaire enregistrée dans le tableau de booléens (Poids le plus lourd a la fin)
            for(unsigned int i = 0; i < etats.size(); ++i) {
                if(etats[i])    { etats[i] = 0; nbrAretes--; }
                else            { etats[i] = 1; nbrAretes++; break; }
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
        s1 = it->getS1();
        s2 = it->getS2();
        x1 = m_sommets[s1]->getX()+x;
        y1 = m_sommets[s1]->getY()+y;
        x2 = m_sommets[s2]->getX()+x;
        y2 = m_sommets[s2]->getY()+y;

        /// On dessine une ligne
        svgout.addLine(x1, y1, x2, y2);
        int dy = y2-y1;

        std::string poids = "(";
        /// Affichage du poids
        std::vector<float> vecPoids = it->getPoids();
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
        x1 = it->getX()+x;
        y1 = it->getY()+y;
        svgout.addDisk(x1, y1, 7);
    }
}

std::vector<float> Graphe::resultatGraphe(){
    int nbPoids = this->getAretes()[0]->getNbPoids(); /// on obtient le nombre de poids que les arètes possèdent
    std::vector<float> resultatPoids(nbPoids,0);
    for (int i = 0 ; i<nbPoids ; ++i){
        for (auto it: this->getAretes()){
            resultatPoids[i] += it->getPoids(i);
        }
    }
    return resultatPoids; /// on renvoie un tableau de poids pour connaître le résultat de notre graphe par poids
}

Graphe::~Graphe()
{
    /*for(auto& ptr : m_sommets) {
        delete ptr.second;
    }
    for(auto& ptr : m_aretes) {
        delete ptr.second;
    }*/
}
