#include "Graphe.h"
#include <fstream>
//#include <iostream>
#include <queue>
#include <stack>
//#include <unordered_map>
#include <map>
#include <utility>
#include <unordered_set>
//#include <algorithm>

    /*************************
    *************************/

#include <limits>

    /*************************
    *************************/

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
    for(unsigned int i = 0; i < aretesFinaux.size(); ++i) {
        vecAretesFinales.push_back(aretesFinaux.find(i)->second);
    }
    Graphe myGraphe {vecSommets, vecAretesFinales};
    return myGraphe;
}


    /*************************
    *************************/

/// DIJKSTRA
/// cette fonction reçoit la matrice de bool contenant tous les graphes possibles obtenus pas Pareto
/// elle applique ensuite sur chaque graphe Dijkstra
/// ce qui permet de remplir des matrices avec les poids entre chaque sommets (== graphe complet)
/// elle retourne sous forme de vector toutes ces matrices
std::vector < std::vector < std::vector < float > > > Graphe::dijkstra(unsigned int indexOfPoids, std::vector<std::vector<bool>*> myBool)
{
    /// notre vecteur de matrices que nous retournous
    /// de la forme : numeroGraphe < X < Y < poids du chemin entre X et Y > > >
    std::vector < std::vector < std::vector < float > > > vectorMatrice;

    /// <sommet d'arrivé, distance depuis départ>, c'est ce que retourne Parcours Dijkstra
    std::vector < std::pair <int, float> > recupParcours;

    /// Défile les graphes possibles
    for (unsigned int i = 0; i < myBool.size(); ++i)
    {
        /// matrice que l'on ajoutera à notre vector de matrice
        //std::vector < std::vector < float > > newGraphe;
        std::vector <std::vector < float > >* newGraphe = new std::vector <std::vector < float > >;

        /// Défile les sommets du graphe selectionné
        for (unsigned int j = 0; j < m_sommets.size(); ++j)
        {
            /// vector que l'on ajoutera à notre matrice
            //std::vector < float > newAretes;
            std::vector < float >* newAretes = new std::vector < float >;

            /// on applique dijkstra sur ce sommet
            recupParcours = parcoursDijkstra(indexOfPoids, m_sommets[j], myBool[i]);

            /// on trie ce parcours par index de sommet croissant
            std::sort(recupParcours.begin(), recupParcours.end());

            /** voir si dijkstra fonctionne
            for(unsigned int u = 0; u<recupParcours.size(); ++u)
            {
                std::cout << "sommet : " << j << " - (arrive, poids) = (" << recupParcours[u].first
                        << ", " << recupParcours[u].second << ")" << std::endl;
            }
            */

            /// on ajoute dans la matrice la distance de tous les sommets par rapport au sommet de départ
            for(unsigned int k; k<recupParcours.size(); ++k)
                (*newAretes).push_back(recupParcours[k].second);

            (*newGraphe).push_back(*newAretes);
            //delete (newAretes);
        }
        vectorMatrice.push_back(*newGraphe);
        //delete (newGraphe);
    }
    return vectorMatrice;
}

/// fonction permetant de trier les aretes selon leur poids de façon rapide si ce dernier est 0 ou 1
void Graphe::trierPoidsAretes(std::vector <Arete*> aretesConnectees, unsigned int poids)
{
    if(poids == 0)
        std::sort(aretesConnectees.begin(), aretesConnectees.end(), [](Arete* s1, Arete* s2){return s1->getPoids(0) > s2->getPoids(0);});
    else
        std::sort(aretesConnectees.begin(), aretesConnectees.end(), [](Arete* s1, Arete* s2){return s1->getPoids(1) > s2->getPoids(1);});
}
/// Cette fonction plus lente remplace le std::sort si : poids > 2
std::vector <Arete*> Graphe::trierPoidsAretes2(std::vector <Arete*> aretesConnectees, unsigned int poids)
{
    std::vector <Arete*> retour;
    float minimum;
    int rang;

    while(!aretesConnectees.empty())
    {
        minimum = aretesConnectees[0]->getPoids(poids);
        rang = 0;
        for(unsigned int i=0; i<aretesConnectees.size(); ++i)
            if(minimum > aretesConnectees[i]->getPoids(poids))
            {
                minimum = aretesConnectees[i]->getPoids(poids);
                rang = i;
            }
        retour.push_back(aretesConnectees[rang]);
        aretesConnectees.erase(aretesConnectees.begin() + rang);
    }
    return retour;
}

/// fonction permettant de conaître l'index de l'autre sommet lié à une arête
int Graphe::autreSommet(Arete* a, Sommet* s)
{
    if(a->getS1() == s->getIndex())
        return a->getS2();
    return a->getS1();
}

/// PARCOURS DIJKSTRA
///     POIDS est soit le COUT (0), soit la DISTANCE (1)
///     SOMMET depuis lequel on applique l'algorythme
///     MYBOOL indique si une arête est prise ou non
std::vector < std::pair <int, float> > Graphe::parcoursDijkstra(unsigned int poids, Sommet* depart, std::vector<bool>* myBool)
{
    /// la vector que l'on retourne : <Id sommet arrivé, Distance depuis départ>
    std::vector < std::pair <int, float> > l_pred;

    /// On peut alors l'insérer le sommet de départ
    l_pred.push_back(std::make_pair(depart->getIndex(), 0));

    /// la priority_queue sera dans l'ordre croissant, avec (Poids, Sommet)
    std::priority_queue< std::pair<float, Sommet*>, std::vector < std::pair<float, Sommet*> > , std::greater< std::pair<float, Sommet*> > > pq;

    /// vector indiquand si un sommet à déjà était exploré, pour ne pas le réajouter dans la file de priorité
    std::vector <int> marque;
    bool boolMarque;

    /// La priority_queue reçoit le sommet sur lequel elle va appliquer Dijkstra et met sa distance à 0
    pq.push(std::make_pair(0, depart));

    /// définit une variable pour l'infini (valeur max de la variable)
    float infinity = std::numeric_limits<float>::max();
    /// le vecteur contenant les distances de chaques sommet (toutes à l'infini au départ)
    std::vector <float> distance;

    /// un sommet tampon
    Sommet* s;
    /// vector de tri pour le poids des arêtes
    std::vector <Arete*> aretesConnectees;

    /// Tous les sommets sont à une distance infinie du sommet de départ (celui ci est à 0)
    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        if(i == depart->getIndex()) distance.push_back(0);
        else distance.push_back(infinity);
    }

    /// Execution de Dijkstra (tant qu'il y a au moins un sommet à explorer)
    while(!pq.empty())
    {
        /// le sommet tampon reçoit la valeur du sommet que l'on traite
        s = pq.top().second;
        /// le sommet traiter est retiré de la file
        pq.pop();

        /// vide le vector d'arete du précédent sommet

        /// on ajoute les aretes du sommet actuel
        for(unsigned int i=0; i<s->getAretesSommet().size(); ++i)
            if((*myBool)[s->getAretesSommet()[i]])
                aretesConnectees.push_back(m_aretes[s->getAretesSommet()[i]]);

        /// on les trie selon leur poids, un algorythme de tri plus performant fonctionne pour les valeurs 0 et 1
        if(poids == 0 || poids == 1) trierPoidsAretes(aretesConnectees, poids);
        else aretesConnectees = trierPoidsAretes2(aretesConnectees, poids);

        /// Ajoute les sommets à la file de priorité et à la map de retour
        for(const auto& v : aretesConnectees)
        {
            /// on indique leur distance par rapport au sommet initial, si elle est meilleure, on l'ajoute à la map
            if(distance[s->getIndex()] + v->getPoids(poids) < distance[autreSommet(v, s)])
            {
                /// si c'est bien le chemin le plus court, il reçoit cette nouvelle distance
                distance[autreSommet(v, s)] = distance[s->getIndex()] + v->getPoids(poids);

                /// variable pour savoir si le sommet que l'on veut ajouter existe déjà dans le vector retour
                bool existe(0);

                for(unsigned int i = 0; i<l_pred.size(); ++i)
                    if(l_pred[i].first == m_sommets[autreSommet(v, s)]->getIndex())
                    {
                        /// le sommet existe déjà, on change son arête empruntée
                        l_pred[i].second = distance[autreSommet(v, s)];
                        existe = 1;
                        break;
                    }

                /// Si ce sommet n'était pas encore dans la map on l'ajoute
                if(!existe) l_pred.push_back(std::make_pair(autreSommet(v, s), distance[autreSommet(v, s)]));

                /// on ajoute à la file de priorité le sommet voisin si il n'est pas déjà marqué
                boolMarque = 0;

                for(unsigned int i = 0; i<marque.size(); ++i)
                    if(marque[i] == autreSommet(v,s)) boolMarque = 1;

                if(!boolMarque)
                {
                    pq.push(std::make_pair(distance[autreSommet(v, s)], m_sommets[autreSommet(v,s)]));
                    marque.push_back(autreSommet(v,s));
                }
            }
        }
    }

    return l_pred;
}


    /*************************
    *************************/


/// Retourne un vecteur de Graphes contenant tous le sous-graphes possibles
std::vector<Graphe*> Graphe::ensembleGraphesPartiels() {
    std::vector<Graphe*> graphesPartiels;
    std::vector<bool> etats, allTrue;
    std::vector<bool> changementEtat; // Une case contient un 1 si l'etat a été changé ce tour, un 0 sinon
    for(unsigned int i = 0; i < m_aretes.size(); ++i) {
        etats.push_back(0);
        allTrue.push_back(1);
        changementEtat.push_back(0);
    }
    bool run = 1;
    while(run) {
        std::vector<Arete*> aretes;
        unsigned int compteur = 0; /// Permet de trouver l'arete que l'on veut dans la map d'arete (est un id arbitraire)
        //std::cout << std::endl;
        for(const auto& it : etats) {
            //std::cout << it;
            if(it) {
                Arete* areteCourante = m_aretes[compteur];
                aretes.push_back(areteCourante);
            }
            compteur++;
        }

        graphesPartiels.push_back(new Graphe{m_sommets, aretes});

        if (etats == allTrue)   run = 0; // Arrete la boucle si on a fait tous les tests
        else{ /// Augmente de 1 la valeur binaire enregistrée dans le tableau de booléens (Poids le plus lourd a la fin)
            for(unsigned int i = 0; i < etats.size(); ++i) {
                changementEtat[i] = 0;
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

Graphe::~Graphe()
{
    /*
    for(auto& ptr : m_sommets) {
        delete ptr.second;
    }
    for(auto& ptr : m_aretes) {
        delete ptr.second;
    }*/
}
