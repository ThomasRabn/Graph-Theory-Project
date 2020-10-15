#include "Graphe.h"

/// FONCTION PERMETTANT DE PASSER D'UN INTERVALLE A UN AUTRE, fonction map() sur l'IDE Arduino
float mapping(float value, float minIn, float maxIn, float minOut, float maxOut) {
    return (value - minIn) * (maxOut - minOut) / (maxIn - minIn) + minOut;
}

Graphe::Graphe(std::string nomFichier, std::string nomFichier2){
    std::ifstream fichier{nomFichier};
    std::ifstream fichier2{nomFichier2};
    if (!fichier) /// Si on a un probleme on lance une erreur
        throw std::runtime_error("Erreur d'ouverture du fichier " + nomFichier + ", verifiez le nom et le fichier");
    if (!fichier2) /// Si on a un probleme on lance une erreur
        throw std::runtime_error("Erreur d'ouverture du fichier " + nomFichier2 + ", verifiez le nom et le fichier");
    int ordre;
    fichier >> ordre;  /// On met la valeur présente dans le fichier dans ordre
    if (fichier.fail())
        throw std::runtime_error("Probleme de lecture de l'ordre du graphe");
    int id, x, y;
    /// On lit les sommets et on met les valeurs de x et de y dans la map du graphe
    for(int i = 0; i < ordre; ++i){
        fichier >> id;  if(fichier.fail()) throw std::runtime_error("Probleme de lecture de l'id du sommet " + i);
        fichier >> x;   if(fichier.fail()) throw std::runtime_error("Probleme de lecture de l'abscisse du sommet " + i);
        fichier >> y;   if(fichier.fail()) throw std::runtime_error("Probleme de lecture de l'ordonnee du sommet " + i);
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

/// ALGORITHMES

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

    unsigned int nbAdd = 1; /// on compte le nombre d'arete pour savoir quand tous les sommets sont reliés
    unsigned int boucle = 0; /// en fonction du nombre de boucle qu'on a fait on peut savoir qu'elle arete sélectionner (par ordre croissant)
    while (nbAdd < sommets.size()){

        auto it = aretes.find(aretesCroissante[boucle]);

        int s1 = sommetsConnexe[it->second->getS1()]; /// on sélectionne le sommet 1 de l'arete
        int s2 = sommetsConnexe[it->second->getS2()]; /// on sélectionne le sommet 2 de l'arete
        if ( (s1 != s2) && (aretesFinaux.find(it->second->getIndex()) == aretesFinaux.end()) ){ /// si les sommets 1 et 2 ne sont pas connexes et qu'on peut ajouter l'arete alors:
            nbAdd++;
            aretesFinaux.insert(*it);
            for (unsigned int i=0 ; i<sommetsConnexe.size() ; ++i){
                if (sommetsConnexe[i] == s2){
                    sommetsConnexe[i] = s1; /// on met tous les sommets de même connexité que le sommet 2 à la valeur de connexité du sommet 1
                }
            }
        }
        boucle++;
    }

    std::vector<Arete*> vecAretesFinales;
    for (auto it: aretesFinaux){
      vecAretesFinales.push_back(it.second);
    }
    Graphe myGraphe {vecSommets, vecAretesFinales};
    return myGraphe;
}

///         DIJKSTRA        ///

/// DIJKSTRA PIRE
std::vector < std::vector < std::vector <int> > >Graphe::pireDijkstra(unsigned int indexOfPoids)
{
    unsigned int i, z;
    /// ce que cette fonction retourne
    /// sommet départ < sommet arrivé< liste des sommets parcourus < int > > >
    std::vector < std::vector < std::vector <int> > > retour;

    /// ce que nous retourne le parcoursDijkstra
    /// <Id sommet arrivé, Id arete empruntée>
    std::vector < std::pair <int, float> > recupParcours;

    /// notre liste de prédecesseur, pour connaître le trajet sommet par sommet
    /// <Id sommet arrivé, Id sommet départ>
    std::unordered_map <int, int > l_pred;

    /// toutes les arêtes sont actives, nécessaire pour parcours dijkstra
    std::vector <bool> myBool(m_sommets.size(), 1);

    for(i=0; i<m_sommets.size(); ++i)       /// pour chaque sommet on applique Dijkstra
    {
        std::vector < std::vector <int> >* newSommet = new std::vector < std::vector < int > >;

        recupParcours = parcoursDijkstra(indexOfPoids, m_sommets[i], &myBool, 1);
        std::sort(recupParcours.begin(), recupParcours.end());

        l_pred.clear();

        /// on transvase dans notre liste de prédécesseurs
        for(z = 0; z < recupParcours.size(); ++z)
        {
            if(recupParcours[z].second != -1)
            {
                l_pred.insert({ recupParcours[z].first,
                                autreSommet(m_aretes[recupParcours[z].second],
                                            m_sommets[recupParcours[z].first])});
            }

        }

        /// on défile notre liste de prédecesseurs afin de connaître les chemins de sommet en sommet
        /// on les ajouteras ensuite à notre vector retour
        for(auto s:l_pred)
        {
            /// le vector contenant le chemin
            std::vector < int >* newChemin = new std::vector < int >;

            if(s.second != -1)  /// on ne cherche pas de chemin depuis lui même
            {
                (*newChemin).push_back(s.first);

                /// pour transvaser depuis la map vers le vector
                std::pair<int, int> pred = s;

                /// depuis le sommet duquel on est jusqu'au sommet de départ
                while(pred.second != m_sommets[i]->getIndex())
                {
                    pred =*l_pred.find(pred.second);

                    /// on ajoute le nouveau sommet au chemin
                    (*newChemin).push_back(pred.first);
                }
            }
            (*newChemin).push_back(m_sommets[i]->getIndex());
            (*newSommet).push_back(*newChemin);
            delete(newChemin);
        }
        retour.push_back(*newSommet);
        delete(newSommet);
    }

    /// affichage matrice résultat
    /*
    unsigned int j,k;
    for(i = 0; i <retour.size(); ++i)
    {
       for(j = 0; j < retour[i].size(); ++j)
       {
            for(k = 0; k < retour[i][j].size(); ++k)
            {
                std::cout << "{ " << retour[i][j][k] << " }";
            }
            std::cout << std::endl;
       }
       std::cout << std::endl;
    }
    std::cout << std::endl;
    */

    return retour;
}

/// DIJKSTRA PARTERO
/// cette fonction reçoit la matrice de bool contenant tous les graphes possibles obtenus pas Pareto
/// elle applique ensuite sur chaque sommet de chaque graphe Dijkstra
/// afin de conaître le poids total des plus court chemins pour chaque graphe
std::vector < std::vector < std::vector < float > > > Graphe::dijkstra(unsigned int indexOfPoids, std::vector<std::vector<bool>*> myBool)
{
    unsigned int i, j, k;
    /// notre vecteur de matrices que nous retournous
    /// de la forme : numeroGraphe < X < Y < poids du chemin entre X et Y > > >
    std::vector < std::vector < std::vector < float > > > vectorMatrice;

    /// <sommet d'arrivé, distance depuis départ>, c'est ce que retourne Parcours Dijkstra
    std::vector < std::pair <int, float> > recupParcours;

    /// Défile les graphes possibles
    for (i = 0; i < myBool.size(); ++i)
    {
        /// matrice que l'on ajoutera à notre vector de matrice
        std::vector <std::vector < float > >* newGraphe = new std::vector <std::vector < float > >;

        /// Défile les sommets du graphe selectionné
        for (j = 0; j < m_sommets.size(); ++j)
        {
            /// vector que l'on ajoutera à notre matrice
            std::vector < float >* newAretes = new std::vector < float >;

            /// on applique dijkstra sur ce sommet
            recupParcours = parcoursDijkstra(indexOfPoids, m_sommets[j], myBool[i], 0);

            /// on trie ce parcours par index de sommet croissant
            std::sort(recupParcours.begin(), recupParcours.end());

            /// on ajoute dans la matrice la distance de tous les sommets par rapport au sommet de départ
            for(k = 0; k<recupParcours.size(); ++k)
                (*newAretes).push_back(recupParcours[k].second);

            (*newGraphe).push_back(*newAretes);
            delete (newAretes);
        }
        vectorMatrice.push_back(*newGraphe);
        delete (newGraphe);
    }
    return vectorMatrice;
}

/// fonction permetant de trier les aretes selon leur poids
void Graphe::trierPoidsAretes(std::vector <Arete*> aretesConnectees, unsigned int poids)
{
    /// if et else car std::sort refuse une variable en arguement
    if(poids == 0)
        std::sort(aretesConnectees.begin(), aretesConnectees.end(), [](Arete* s1, Arete* s2){return s1->getPoids(0) > s2->getPoids(0);});
    else
        std::sort(aretesConnectees.begin(), aretesConnectees.end(), [](Arete* s1, Arete* s2){return s1->getPoids(1) > s2->getPoids(1);});
}

/// fonction permettant de conaître l'index de l'autre sommet lié à une arête
int Graphe::autreSommet(Arete* a, Sommet* s)
{
    if(a->getS1() == s->getIndex())
        return a->getS2();
    return a->getS1();
}

/// DIJKSTRA - PARCOURS
///     POIDS est soit le COUT (0), soit la DISTANCE (1)
///     SOMMET depuis lequel on applique l'algorythme
///     MYBOOL indique si une arête est prise ou non

/// mode 0, retourne <sommet Arrivé, poids depuis départ>
/// mode 1, retourne <sommet Arrivé, arete empruntee>
std::vector < std::pair <int, float> > Graphe::parcoursDijkstra(unsigned int poids, Sommet* depart, std::vector<bool>* myBool, int mode)
{
    /// la vector que l'on retourne : <Id sommet arrivé, Distance depuis départ>
    std::vector < std::pair <int, float> > l_pred;

    /// On peut alors l'insérer le sommet de départ
    if(mode == 0) l_pred.push_back(std::make_pair(depart->getIndex(), 0));
    else l_pred.push_back(std::make_pair(depart->getIndex(), -1));

    /// la priority_queue sera dans l'ordre croissant, avec (Poids, Sommet)
    std::priority_queue< std::pair<float, Sommet*>, std::vector < std::pair<float, Sommet*> > , std::greater< std::pair<float, Sommet*> > > pq;

    /// vector indiquand si un sommet à déjà était exploré, pour ne pas le réajouter dans la file de priorité
    std::vector <int> marque;
    bool boolMarque;

    /// La priority_queue reçoit le sommet sur lequel elle va appliquer Dijkstra et met sa distance à 0
    pq.push(std::make_pair(0, depart));

    /// un sommet tampon
    Sommet* s;
    /// vector de tri pour le poids des arêtes
    std::vector <Arete*> aretesConnectees;

    /// définit une variable pour l'infini (valeur max de la variable)
    float infinity = std::numeric_limits<float>::max();
    /// le vecteur contenant les distances de chaques sommet (toutes à l'infini au départ)
    std::vector <float> distance;

    /// Tous les sommets sont à une distance infinie du sommet de départ (celui ci est à 0)
    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        if(i == (unsigned int)depart->getIndex()) distance.push_back(0);
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
        aretesConnectees.clear();

        /// on ajoute les aretes du sommet actuel
        for(unsigned int i=0; i<s->getAretesSommet().size(); ++i)
            if((*myBool)[s->getAretesSommet()[i]])
                aretesConnectees.push_back(m_aretes[s->getAretesSommet()[i]]);

        /// on les trie selon leur poids, un algorythme de tri plus performant fonctionne pour les valeurs 0 et 1
        trierPoidsAretes(aretesConnectees, poids);

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
                        if(mode == 0) l_pred[i].second = distance[autreSommet(v, s)];
                        else l_pred[i].second = v->getIndex();

                        existe = 1;
                        break;
                    }

                /// Si ce sommet n'était pas encore dans la map on l'ajoute
                if(!existe)
                {
                    if(mode == 0)
                        l_pred.push_back(std::make_pair(autreSommet(v, s), distance[autreSommet(v, s)]));

                    else
                        l_pred.push_back(std::make_pair(autreSommet(v, s), v->getIndex()));
                }

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

std::vector<float> Graphe::resultatGraphe(std::vector<std::vector<std::vector<float>>> matriceBruteForce) {
    std::vector<float> poidsRetour(matriceBruteForce.size(), 0);
    for (unsigned int i = 0 ; i < matriceBruteForce.size() ; ++i){
        for (unsigned int j = 0 ; j < matriceBruteForce[i].size() ; ++j){
            for (unsigned int k = j ; k < matriceBruteForce[i][j].size() ; ++k){
                poidsRetour[i] += matriceBruteForce[i][j][k];
                /// Commenter la ligne au dessus et decommenter celle du dessous permet d'avoir les memes resultats que sur le pdf du projet
                //poidsRetour[i] += (2*matriceBruteForce[i][j][k]);
            }
        }
    }
    return poidsRetour;
}

/// Fonction retournant un vector de float des poids totaux
std::vector<float> Graphe::resultatGraphe() {
    std::vector<bool> vecBool (m_aretes.size(), 1);
    return resultatGraphe(vecBool);
}

/// Fonction retournant un vector de float des poids totaux en fonction d'un vecteur de booleens (1 = arete; 0 = pas arete)
std::vector<float> Graphe::resultatGraphe(std::vector<bool> vecBool) {
    if (!(getAretes().empty())) {
        int nbPoids = getAretes()[0]->getNbPoids(); /// on obtient le nombre de poids que les arètes possèdent
        std::vector<float> resultatPoids(nbPoids,0);
        for (int i = 0 ; i<nbPoids ; ++i) {
            for (unsigned int j = 0; j < getAretes().size(); ++j) {
                if(vecBool[j])  { resultatPoids[i] += m_aretes[j]->getPoids(i); }
            }
        }
        return resultatPoids; /// on renvoie un tableau de poids pour connaître le résultat de notre graphe par poids
    } else{
        std::vector<float> resultatPoids;
        return resultatPoids; /// on renvoie un tableau de poids vide
    }
}

/// Retourne un vecteur de bool contenant tous le sous-graphes de Ordre-1 aretes. vec[i] = 1 -> arete n°i ajoutee.
std::vector<std::vector<bool>*> Graphe::ensembleGraphesPartiels(int toggleAbove, std::vector<bool> vec1, std::vector<bool> vec2) {
    std::vector<std::vector<bool>*> graphesPartiels;
    std::vector<bool> etats (m_aretes.size(), 0), allTrue(m_aretes.size(), 1);
    unsigned int nbrAretes = 0; /// Suit le nombre d'aretes presentes dans le graphe partiel créé
    bool run = 1;

    if(toggleAbove == 2 && ((vec1.size() < m_aretes.size()) || (vec2.size() < m_aretes.size())))      { std::cout << "ERREUR. SORTIE."; run = 0; } /// Si on active le mode double kruskal et qu'il n'y a pas les bonnes tailles on quitte

    while(run) {
        if(toggleAbove == 2) { /// Entre la dedans si on prend en compte les graphes de kruskal
            if (nbrAretes == m_sommets.size()-1) {
                unsigned int compteur = 0;
                for(unsigned int i = 0; i < etats.size(); ++i) {
                    if(vec1[i] == 0 || vec2[i] == 0) { /// Si au moins un des deux graphes ne contient pas l'arete on dit que c'est ok
                        ++compteur;
                    }
                    else if(etats[i] == 1) { /// Si les deux graphes on l'arete et que la configuration du moment l'a aussi, on dit que c'est ok
                        ++compteur;
                    }
                    else{ break; } /// Sinon on abandonne, la configuration ne repond pas a nos attentes
                }
                if(compteur == etats.size())                                 graphesPartiels.push_back(new std::vector<bool>{etats}); /// Si tous les bits de la configuration ont ete valide on l'ajoute
            }
        }
        else{ /// Si on est pas dans le mode double kruskal
            if (nbrAretes == m_sommets.size()-1)                             graphesPartiels.push_back(new std::vector<bool>{etats}); /// On ajoute si le nombre d'aretes est egal a ordre-1
            else if(nbrAretes > m_sommets.size()-1 && toggleAbove == 1)      graphesPartiels.push_back(new std::vector<bool>{etats}); /// Si on est en mode kruskal on ajoute aussi si le nbr d'arete est superieur a ordre-1
        }

        if (etats == allTrue)   run = 0; /// Arrete la boucle si on a fait tous les tests
        else{ /// Augmente de 1 la valeur binaire enregistrée dans le tableau de booléens (Poids le plus lourd a la fin)
            for(unsigned int i = 0; i < etats.size(); ++i) {
                if(etats[i])    { etats[i] = 0; nbrAretes--; }
                else            { etats[i] = 1; nbrAretes++; break; }
            }
        }
    }

    return graphesPartiels;
}

/// Retourne un vecteur de pointeurs sur vecteurs de booleens representant les configurations couvrantes
std::vector<std::vector<bool>*> Graphe::ensembleArbresCouvrants(std::vector<std::vector<bool>*> vec) {
    std::vector<std::vector<bool>*> arbresCouvrants; /// Vecteur stockant tous les pointeurs vers vecteur de booleens permettant de savoir quelles aretes on ajoute
    std::vector<int> allRight(m_sommets.size(), 0);
    int indiceVec1 = 0; /// int permettant d'acceder a une case specifique du tableau pour les sommets
    int indiceVec2 = 0;
    for(unsigned int i = 0; i < vec.size(); ++i) { /// Pour chaque configuration
        std::vector<int> vecSommets; /// On crée un tableau ne contenant aucun sommet
        for(unsigned int j = 0; j < m_sommets.size(); ++j) {
            vecSommets.push_back(j);
        }

        for (unsigned int j = 0; j < (*(vec[i])).size(); ++j) { /// Pour chaque arete
            if ((*(vec[i]))[j]) { /// Si l'arete existe on ajoute les sommets qui sont reliés
                indiceVec1 = getAretes()[j]->getS1();
                indiceVec2 = getAretes()[j]->getS2();
                if (vecSommets[indiceVec1]<vecSommets[indiceVec2]) { /// Si la composante connexe du sommet 1 est inferieure a celle du sommet 2 on echange toutes les composantes connexes des sommets qui ont celle de 1 en celle de 2
                    for(unsigned int l = 0; l < vecSommets.size(); l++) {
                        if((vecSommets[l] == vecSommets[indiceVec2])&&((int)l != indiceVec2)) { vecSommets[l] = vecSommets[indiceVec1]; }
                    }
                    vecSommets[indiceVec2] = vecSommets[indiceVec1];
                }
                else { /// Si la composante connexe du sommet 2 est inferieure a celle du sommet 1 on echange toutes les composantes connexes des sommets qui ont celle de 2 en celle de 1
                    for(unsigned int l = 0; l < vecSommets.size(); l++) {
                        if((vecSommets[l] == vecSommets[indiceVec1])&&((int)l != indiceVec1)) { vecSommets[l] = vecSommets[indiceVec2]; }
                    }
                    vecSommets[indiceVec1] = vecSommets[indiceVec2];
                }
                for(unsigned int m = 0; m < vecSommets.size(); m++) {
                }

                if (vecSommets == allRight)         { arbresCouvrants.push_back(vec[i]); break; } /// Si tous les sommets sont ajoutes on ajoute la configuration a arbresCouvrants et on sort de la boucle
            }
        }
    }
    return arbresCouvrants;
}

/// Fonction pour lancer un pareto. Mode 0 : normal, mode 1 : dijkstra, mode 2 : double kruskal
void Graphe::affichagePareto(int mode, int indicePoidsDijkstra) {
    std::vector<std::vector<float>*> vecPoidsSolutions;
    std::vector<std::vector<bool>*> vecSolutionsTaille;
    if(mode == 1)                   vecSolutionsTaille = ensembleGraphesPartiels(true); /// Retourne tous les graphes partiels d'au moins ordre-1 aretes (mode Dijkstra)
    else if(mode == 0)              vecSolutionsTaille = ensembleGraphesPartiels();/// Retourne tous les graphes partiels de ordre-1 aretes (mode Pareto simple)
    else{ /// Mode en fonction d'un double Kruskal
        Graphe kruskal1 = parcourKruskal(0); /// On lance kruskal pour les deux poids
        Graphe kruskal2 = parcourKruskal(1);
        std::vector<bool> boolKruskal1 = grapheToBool(kruskal1); /// On transforme les graphes de kruskal en vecteur de booleens
        std::vector<bool> boolKruskal2 = grapheToBool(kruskal2);
        vecSolutionsTaille = ensembleGraphesPartiels(2, boolKruskal1, boolKruskal2); /// On utilise la fonction overload
    }
    std::cout << vecSolutionsTaille.size();
    std::vector<std::vector<bool>*> vecSolutionsCouvrantes = ensembleArbresCouvrants(vecSolutionsTaille); /// Retourne tous les arbres couvrant faisant partie de vecSolutionsTaille

    /// PERMET DE FAIRE LE TRI EN CREANT DES VECTEURS DE FLOAT CONTENANT LES POIDS. MEILLEURE OPTIMISATION QUE CREER UN GRAPHE POUR TESTER -> GAIN DE TEMPS
    if(mode == 1) { /// POUR LE MODE DIJKSTRA
        std::vector<std::vector<std::vector<float>>> vecDij = dijkstra(indicePoidsDijkstra, vecSolutionsCouvrantes);
        std::vector<float>* sommesTempsParcours = new std::vector<float>;
        *sommesTempsParcours = resultatGraphe(vecDij);
        for(unsigned int i = 0; i < vecSolutionsCouvrantes.size(); ++i) {
            std::vector<float>* poidsSolution =  new std::vector<float>;
            for(unsigned int j = 0; j < 2; ++j) {
                if ((int)j == indicePoidsDijkstra) {
                    (*poidsSolution).push_back((*sommesTempsParcours)[i]);
                } else{
                    float resultatPoids = 0;
                    for (unsigned int k = 0; k < getAretes().size(); ++k) {
                        if((*(vecSolutionsCouvrantes[i]))[k])  { resultatPoids += m_aretes[k]->getPoids(j); }
                    }
                    (*poidsSolution).push_back(resultatPoids);
                }
            }
            (*poidsSolution).push_back(i);
            vecPoidsSolutions.push_back(poidsSolution);
        }
    } else { /// POUR LES AUTRES MODES
        for(unsigned int i = 0; i < vecSolutionsCouvrantes.size(); ++i) {
            std::vector<float>* poidsSolution =  new std::vector<float>;
            *poidsSolution = resultatGraphe(*(vecSolutionsCouvrantes[i]));
            (*poidsSolution).push_back(i);
            vecPoidsSolutions.push_back(poidsSolution);
        }
    }
    std::vector<std::vector<float>*> vecPoidsSolutionsNonTriees = vecPoidsSolutions;
    /// On tri le vecteur de poids pour mettre en premier ceux avec le poids[0] le plus petit (utilisation d'une fonction lambda)
    std::sort(vecPoidsSolutions.begin(), vecPoidsSolutions.end(), [](const auto& cour, const auto& suiv) {
        for(unsigned int i = 0; i < (*cour).size()-1; ++i) {
            if ((*cour)[i] != (*suiv)[i]) {
                return (*cour)[i] < (*suiv)[i];
            }
        }
        return false;
    });

    /// Rempli un vecteur de int par les indices des solutions non dominees (l'indice dans vecSolutionsCouvrantes);
    ///maxY, min Y, minX et maxX permettent de savoir quelles sont les extremums a placer sur le tableau
    int maxY = (*vecPoidsSolutions[0])[1];
    int minX = (*vecPoidsSolutions[0])[0];
    int minY = maxY;
    int maxX = minX;
    std::vector<int> vecIndicesSolutionsNonDominees = giveSolutionsNonDominees(vecPoidsSolutions, maxX, minY);

    /// Affiche tous les graphes solutions sur un fichier SVG
    bool toggleDijkstra = 0; // Creation d'un booléen permettant de savoir si on est en mode dijkstra
    if (mode == 1)      toggleDijkstra = 1; // Si mode = dijkstra le booleen vaut 1
    dessinerPareto(vecIndicesSolutionsNonDominees, vecSolutionsCouvrantes, vecPoidsSolutions, minX, maxX, minY, maxY, vecPoidsSolutionsNonTriees, toggleDijkstra);

    /// On désalloue la mémoire (on ne désalloue pas vecSolutionsCouvrantes car il est inclu dans vecSolutionTaille)
    for(auto ptr : vecSolutionsTaille) {
        delete ptr;
    }
    for(auto ptr : vecPoidsSolutions) {
        delete ptr;
    }
}

/// Renvoie un vecteur de int contenant les indices (dans vecPoidsSolutions) ds solutions non dominees
std::vector<int> Graphe::giveSolutionsNonDominees(std::vector<std::vector<float>*> vecPoidsSolutions, int& maxX, int& minY) {
    std::vector<int> vecIndicesSolutionsNonDominees;
    vecIndicesSolutionsNonDominees.push_back((*vecPoidsSolutions[0]).back());
    for(unsigned int i = 1; i < vecPoidsSolutions.size(); ++i) { /// Pour chaque configuration
        if ((*vecPoidsSolutions[i])[0] != maxX) { /// Si on avance en x
            if ((*vecPoidsSolutions[i])[1] < minY) { /// Si on diminue en y
                vecIndicesSolutionsNonDominees.push_back((*vecPoidsSolutions[i]).back()); /// On ajoute l'indice au vecteur
                maxX = (*vecPoidsSolutions[i])[0]; /// On remplace les maxX et minY
                minY = (*vecPoidsSolutions[i])[1];
            }
        }
    }
    return vecIndicesSolutionsNonDominees;
}

void Graphe::dessinerPareto(std::vector<int> vecIndicesSolutionsNonDominees, std::vector<std::vector<bool>*> vecSolutionsCouvrantes,std::vector<std::vector<float>*> vecPoidsSolutions,
                            int minX, int maxX, int minY, int maxY, std::vector<std::vector<float>*> vecPoidsSolutionsNonTriees, bool toggleDijkstra)
{
    /// Creation des variables et ajouts des axes du graphique
    Svgfile svgout("output.svg", 5000, 20000);
    dessiner(svgout, 275, -50);
    const int debutX = 1100;
    const int debutY = 100;
    const int largeur = 800;
    const int hauteur = 800;
    svgout.addLine(debutX-50, hauteur+debutY, debutX+largeur, hauteur+debutY);
    svgout.addLine(debutX, debutY, debutX, debutY+hauteur+50);

    /// Changement des coeff et tailles pour le diagramme
    int rayonFaux = 2, rayonVrai = 5;
    float coeffBasX, coeffHautX, coeffBasY, coeffHautY; /// CoeffBas : petit -> Proche de 0; coeffHaut -> petit : proche de la fin des axes
    if(toggleDijkstra) { /// Si on est en imode dijkstra on a pas les memes coeff que si on n'est pas en mode dijkstra
        coeffBasX = 0.1; coeffHautX = 0; coeffBasY = 0.1; coeffHautY = 0.2;
    } else{
        if(vecSolutionsCouvrantes.size() < 100) {
            coeffBasX = 0.3; coeffHautX = 0.5; coeffBasY = 0.3; coeffHautY = 0.5;
        } else if (vecSolutionsCouvrantes.size() < 1000) {
            coeffBasX = 0.1; coeffHautX = 0.5; coeffBasY = 0.1; coeffHautY = 0.7;
        } else{
            coeffBasX = 0.1; coeffHautX = 0.5; coeffBasY = 0.1; coeffHautY = 0.6;
        }
    }

    /// DESSIN DE L'ENSEMBLE DES SOLUTIONS SUR LE GRAPHIQUE
    float lastX = -1, lastY = -1, coeffRayon = 1;
    if(!toggleDijkstra) {
        for(auto it : vecPoidsSolutions) {
            if ((*it)[0] != lastX || (*it)[1] != lastY) {
                /// On map les valeurs en fonction des extremes
                float posX = mapping((*it)[0], minX, maxX, debutX+coeffBasX*largeur, debutX+largeur-coeffHautX*largeur);
                float posY = mapping((*it)[1], minY, maxY, debutY+hauteur-coeffBasY*hauteur, debutY+hauteur-(1-coeffHautY)*hauteur);
                lastX = (*it)[0];
                lastY = (*it)[1];
                /// On dessine sur le graphique
                svgout.addDisk(posX, posY, rayonFaux*coeffRayon, 100, svgout.makeRGB(255,0,0));
                coeffRayon = 1;
            } else {
                if(coeffRayon < 6.6) coeffRayon+=.02;
            }
        }
    } else{
        for(auto it : vecPoidsSolutions) {
            if ((*it)[0] != lastX || ((*it)[1] != lastY && vecPoidsSolutions.size() < 100) || (*it)[1] - lastY > 5) {
                /// On map les valeurs en fonction des extremes
                float posX = mapping((*it)[0], minX, maxX, debutX+coeffBasX*largeur, debutX+largeur-coeffHautX*largeur);
                float posY = mapping((*it)[1], minY, maxY, debutY+hauteur-coeffBasY*hauteur, debutY+hauteur-(1-coeffHautY)*hauteur);
                lastX = (*it)[0];
                lastY = (*it)[1];
                /// On dessine sur le graphique
                svgout.addDisk(posX, posY, rayonFaux*coeffRayon, 100, svgout.makeRGB(255,0,0));
                coeffRayon = 1;
            } else {
                if(coeffRayon < 6.6) coeffRayon+=.1;
            }
        }
    }

    /// DESSIN DES SOLUTIONS NON DOMINEES SUR LE GRAPHIQUE
    for(auto it : vecIndicesSolutionsNonDominees) { /// On repasse les solutions non dominees en vert
        std::vector<float> vecPoidsNonDominees = *(vecPoidsSolutionsNonTriees[it]);
        float posX = mapping(vecPoidsNonDominees[0], minX, maxX, debutX+coeffBasX*largeur, debutX+largeur-coeffHautX*largeur);
        float posY = mapping(vecPoidsNonDominees[1], minY, maxY, debutY+hauteur-coeffBasY*hauteur, debutY+hauteur-(1-coeffHautY)*hauteur);
        svgout.addDisk(posX, posY, rayonVrai, 100, svgout.makeRGB(0,200,0));
    }

    /// DESSIN DES GRAPHES DES SOLUTIONS NON DOMINEES ET ECRITURE DE LEUR POIDS
    int x = 100, y = 400;
    maxX = -1; minX = std::numeric_limits<int>::max();
    minY = minX;
    for(unsigned int i = 0; i < vecIndicesSolutionsNonDominees.size(); ++i) {
        std::vector<bool> solution = *(vecSolutionsCouvrantes[vecIndicesSolutionsNonDominees[i]]);
        dessiner(svgout, solution, x, y, 0, 0.5);
        for(unsigned int indSommet = 0; indSommet < m_sommets.size(); ++indSommet) {
            if(m_sommets[indSommet]->getX() < minX)     minX = m_sommets[indSommet]->getX();
            if(m_sommets[indSommet]->getX() > maxX)     maxX = m_sommets[indSommet]->getX();
            if(m_sommets[indSommet]->getY() < minY)     minY = m_sommets[indSommet]->getY();
        }
        std::string poids = "(";
        for(unsigned int j = 0; j <  (*vecPoidsSolutionsNonTriees[vecIndicesSolutionsNonDominees[i]]).size()-1; ++j) {
            std::stringstream number;
            number << (int)(*vecPoidsSolutionsNonTriees[vecIndicesSolutionsNonDominees[i]])[j];
            poids.append(number.str());
            if(j != (*vecPoidsSolutionsNonTriees[vecIndicesSolutionsNonDominees[i]]).size()-2)   poids.append(" ; ");
            else                                                poids.append(")");
            svgout.addText(x+(minX+maxX)/4-25, y+minY/2-20, poids);
        }
        if ((i+1)%3 == 0)   { y+=250; x = 100; }
        else                { x+=300; }
    }
}

/// Dessine un graphe sur le fichier svgout
void Graphe::dessiner(Svgfile& svgout, int x, int y, bool toggleText, float coeffTaille) {
    std::vector<bool> vecBool(m_aretes.size(), 1);
    dessiner(svgout, vecBool, x, y, toggleText, coeffTaille);
}

/// Dessine un graphe sur le fichier en prenant en compte ou non les poids et en augmentant la taille ou non du graphe
void Graphe::dessiner(Svgfile& svgout, std::vector<bool> vecBool, int x, int y, bool toggleText, float coeffTaille) {
    int s1, s2, x1, y1, x2, y2;
    /// DESSIN DES ARETES
    for(unsigned int i = 0; i < m_aretes.size(); ++i) {
        if (vecBool[i]) {
            /// On prend les infos
            s1 = m_aretes[i]->getS1();
            s2 = m_aretes[i]->getS2();
            x1 = m_sommets[s1]->getX()*coeffTaille+x;
            y1 = m_sommets[s1]->getY()*coeffTaille+y;
            x2 = m_sommets[s2]->getX()*coeffTaille+x;
            y2 = m_sommets[s2]->getY()*coeffTaille+y;

            /// On dessine une ligne
            svgout.addLine(x1, y1, x2, y2);
            int dy = y2-y1;

            /// Affichage du poids
            if(toggleText) {
                std::string poids = "(";
                std::vector<float> vecPoids = m_aretes[i]->getPoids();
                for(unsigned int j = 0; j < vecPoids.size(); ++j) {
                    std::stringstream number;
                    number << std::fixed << std::setprecision(2) << vecPoids[j];
                    poids.append(number.str());
                    if(j != vecPoids.size()-1)    poids.append(" ; ");
                    else                          poids.append(")");
                }

                /// Permet de savoir où sur l'écran on affiche le texte
                if((dy > 5) || (dy < -5)) {
                    svgout.addText(((x1 + x2)/2)+5, ((y1+y2)/2), poids);
                } else {
                    svgout.addText(((x1 + x2)/2)-50, ((y1+y2)/2)-10, poids);
                }
            }
        }
    }

    /// DESSIN DES SOMMETS
    for(const auto& it : m_sommets) {
        x1 = it->getX()*coeffTaille+x;
        y1 = it->getY()*coeffTaille+y;
        svgout.addDisk(x1, y1, coeffTaille*7);
    }
}

/// Transorme un graphe en vecteur de booleen (1 = arete; 0 = pas d'arete)
std::vector<bool> Graphe::grapheToBool(Graphe g) {
    std::vector<bool> vecBool;
    for(unsigned int i = 0; i < getAretes().size(); ++i) {
        bool ajoute = 0;
        for(int j = i; j >= 0; --j) {
            if(g.getAretes().size() > (unsigned int)j) {
                if ((g.getAretes()[j])->getIndex() == (int)i) {
                    ajoute = 1;
                    break;
                }
            }
        }
        if (ajoute == 1)    vecBool.push_back(1);
        else                vecBool.push_back(0);
    }
    return vecBool;
}

/// On vide la memoire quand on a fini avec le graphe
void Graphe::libererMemoire() {
    for(auto ptr : m_sommets) {
        delete ptr;
    }
    for(auto ptr : m_aretes) {
        delete ptr;
    }
}

void Graphe::pireCheminDijkstra(Graphe origine, Graphe &optimisable){
    std::vector<std::vector<bool>*> myBool; /// on crée le vecteur de bool pour l'envoyer à la fonction
    std::vector < bool >* newChemin = new std::vector < bool >;
    *newChemin = optimisable.grapheToBool(optimisable);
    myBool.push_back(newChemin); /// on ajoute les arêtes du graphe optimal pour lancer dijkstra
    std::vector < std::vector < std::vector < float > > >vecTemps = optimisable.dijkstra(1, myBool);

    std::vector<std::vector<bool>*> myBoolOrigine;
    std::vector < bool >* newCheminOrigine = new std::vector < bool >;
    *newCheminOrigine = origine.grapheToBool(origine);
    myBoolOrigine.push_back(newCheminOrigine); /// on ajoute les arêtes du graphe optimal pour lancer dijkstra
    std::vector < std::vector < std::vector < float > > >vecTempsOrigine = origine.dijkstra(1, myBoolOrigine);

    float maxTemps = 0;
    unsigned int x=0, y=0;
    for (unsigned int i = 0 ; i<vecTemps[0].size() ; ++i){
        for (unsigned int j = i ; j<vecTemps[0][i].size() ; ++j){
            if ( vecTemps[0][i][j] > maxTemps ){
                maxTemps = vecTemps[0][i][j];
                x = i;
                y = j;
            }
        }
    }
    std::cout << "Le plus long chemin est celui du sommet " << x << " au sommet " << y
              << " avec un temps optimal pour le moment de: " << maxTemps << std::endl;

    std::cout << "Dans le graphe d'origine le temps pour se rendre du sommet " << x << " au sommet " << y
              << " est de: " << vecTempsOrigine[0][x][y] << std::endl;
    if (vecTempsOrigine[0][x][y] == maxTemps)std::cout << "Le chemin est donc deja optimal en fonction du temps, on ne peut pas l'ameliorer sans creer de nouvelles aretes" << std::endl;
    else {
        if(x > y) y = origine.m_sommets.size() - y - 1;
        if(x < y) y = origine.m_sommets.size() - x - 1;
        std::vector<std::vector<std::vector<int>>> vecChemin = origine.pireDijkstra(1);
        std::cout << "Le meilleurs chemin est : " << std::endl;

        for (unsigned int i=0 ; i<vecChemin[x][y].size() ; ++i){
            if (i==vecChemin[x][y].size()-1){
                std::cout << vecChemin[x][y][i];
            }
            else std::cout << vecChemin[x][y][i] << " -> ";
        }
        std::cout << std::endl;
    }
    delete(newChemin);
    delete(newCheminOrigine);
}