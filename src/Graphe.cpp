#include "Graphe.h"

/// FONCTION PERMETTANT DE PASSER D'UN INTERVALLE A UN AUTRE
float mapping(float value, float minIn, float maxIn, float minOut, float maxOut) {
    return (value - minIn) * (maxOut - minOut) / (maxIn - minIn) + minOut;
}

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

/*************************
    *************************/

///Pour Dijkstra au final faut que tu nous redonne tous les chemins du sommet de Depart vers tous les autres
std::vector < std::vector < std::pair<int, int> > > Graphe::dijkstra(unsigned int indexOfPoids, Sommet* depart)
{
    std::vector < std::vector <std::pair<int, int> > > tab;
    std::unordered_map<int, int> recupParcours = parcoursDijkstra(indexOfPoids, depart);


    std::cout << "DIJ a partir de : " << depart->getIndex() << std::endl << std::endl;

    /// CONTENU DE LA MAP
    for(auto const& a : recupParcours)
    {
        std::cout << "{" << a.first << ", " << a.second << "}" << std::endl;
    }
    std::cout << std::endl;

    /// AFFICHER CHEMINS
    for(auto s:recupParcours){

        if(s.second!=-1)
        {
            std::cout<<s.first<<" <--- ";
            std::pair<int, int> pred=s;
            while(pred.second!=depart->getIndex()){
                pred=*recupParcours.find(pred.second);
                std::cout<<pred.first<<" <--- ";
            }
            std::cout<< depart->getIndex() <<std::endl;
        }
    }
    std::cout << std::endl;

    /// TRANSVASER SOMMET
    for(auto s:recupParcours)
    {
        std::vector < std::pair<int, int> > add;

        if(s.second!=-1)
        {
            add.push_back(std::make_pair(s.first, -1));

            std::pair<int, int> pred=s;
            while(pred.second != depart->getIndex())
            {
                pred =*recupParcours.find(pred.second);
                add.push_back(std::make_pair(pred.first, -1));
            }
        }
        add.push_back(std::make_pair(depart->getIndex(), -1));
        tab.push_back(add);
    }
    std::cout << std::endl;

    /// AJOUT DES ARETES
    for (unsigned int i = 0; i < tab.size(); i++)
    {
        for (unsigned int j = 0; j < tab[i].size(); j++)
        {

        }
    }

    /// PRINT TAB
    for (unsigned int i = 0; i < tab.size(); i++)
    {
        for (unsigned int j = 0; j < tab[i].size(); j++)
            std::cout << "{" << tab[i][j].first << ", " << tab[i][j].second << "}";
        std::cout << std::endl;
    }

    return tab;
}

/// fonction permetant de trier les aretes selon leur poids
/*
void Graphe::trierPoidsAretes(std::vector <Arete*> aretesConnectees, unsigned int poids)
{
    std::sort(aretesConnectees.begin(), aretesConnectees.end(), [](Arete* s1, Arete* s2)
    {
        return s1->getPoids(0) > s2->getPoids(0);
    });
}*/
/// Cette fonction remplace le std::sort non opérationel
std::vector <Arete*> Graphe::trierPoidsAretes(std::vector <Arete*> aretesConnectees, unsigned int poids)
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

/// fonction permettant de conaître l'autre sommet lié à une arete
int Graphe::autreSommet(Arete* a, Sommet* s)
{
    if(a->getS1() == s->getIndex())
        return a->getS2();
    return a->getS1();
}

/// POIDS est soit le COUT (0), soit la DISTANCE (1)
std::unordered_map<int, int> Graphe::parcoursDijkstra(unsigned int poids, Sommet* depart)
{
    /// la map que l'on retourne avec les ID du sommet et de son prédécesseur
    std::unordered_map<int, int> l_pred;

    /// On peut alors l'insérer dans la map le sommet de départ
    l_pred.insert({depart->getIndex(), -1});

    /// la priority_queue sera dans l'ordre croissant, avec (Poids, Sommet)
    std::priority_queue< std::pair<float, Sommet*>, std::vector < std::pair<float, Sommet*> > , std::greater< std::pair<float, Sommet*> > > pq;
    //std::priority_queue < std::pair<float, Sommet*> > pq;

    /// vector indiquand si un sommet à déjà était exploré, pour ne pas le réajouter dans la file de priorité
    std::vector <int> marque;
    int boolMarque;     /// non(0) - oui(1)

    /// La priority_queue reçoit le sommet sur lequel elle va appliquer Dijkstra et met sa distance à 0
    pq.push(std::make_pair(0, depart));

    /// définit une variable pour l'infini (~2B pour un int)
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
        if(i == 0) distance.push_back(0);
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
        for(size_t i=0; i<s->getAretesSommet().size(); ++i)
            aretesConnectees.push_back(m_aretes[s->getAretesSommet()[i]]);




        ///AJOUT if(vectBool[getAreteSommet[i]]




        /// on les trie selon leur poids
        //trierPoidsAretes(aretesConnectees, poids);
        aretesConnectees = trierPoidsAretes(aretesConnectees, poids);

        /// Ajoute les sommets à la file de priorité et à la map de retour
        for(const auto& v : aretesConnectees)
        {
            /// on indique leur distance par rapport au sommet initial, si elle est meilleure, on l'ajoute à la map
            if(distance[s->getIndex()] + v->getPoids(poids) < distance[autreSommet(v, s)])
            {
                /// si c'est bien le chemin le plus court, il reçoit cette nouvelle distance
                distance[autreSommet(v, s)] = distance[s->getIndex()] + v->getPoids(poids);

                /// Si ce sommet n'était pas encore dans la map on l'ajoute
                if(l_pred.find(autreSommet(v, s)) == l_pred.end()){
                    l_pred.insert({autreSommet(v, s), s->getIndex()});
                }

                /// Si le sommet était déjà dans la map, on change son antécédent
                else
                {
                    auto it = l_pred.find(autreSommet(v, s));
                    it->second = s->getIndex();
                }
                /// on ajoute à la file de priorité si il n'est pas déjà marqué
                boolMarque = 0;
                for(unsigned int z = 0; z<marque.size(); ++z)
                    /// si il est déjà marqué, boolMarque devient vrai
                    if(marque[z] == autreSommet(v,s)) boolMarque = 1;

                if(boolMarque == 0)
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

std::vector<float> Graphe::resultatGraphe() {
    std::vector<bool> vecBool (m_aretes.size(), 1);
    return resultatGraphe(vecBool);
}

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
std::vector<std::vector<bool>*> Graphe::ensembleGraphesPartiels(bool toggleAbove) {
    std::vector<std::vector<bool>*> graphesPartiels;
    std::vector<bool> etats (m_aretes.size(), 0), allTrue(m_aretes.size(), 1);
    unsigned int nbrAretes = 0; /// Suit le nombre d'aretes presentes dans le graphe partiel créé
    bool run = 1;

    while(run) {
        if (nbrAretes == m_sommets.size()-1)                        graphesPartiels.push_back(new std::vector<bool>{etats});
        else if(nbrAretes > m_sommets.size()-1 && toggleAbove)      graphesPartiels.push_back(new std::vector<bool>{etats});

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
                if (vecSommets[indiceVec1]<vecSommets[indiceVec2]) {
                    for(unsigned int l = 0; l < vecSommets.size(); l++) {
                        if((vecSommets[l] == vecSommets[indiceVec2])&&((int)l != indiceVec2)) { vecSommets[l] = vecSommets[indiceVec1]; }
                    }
                    vecSommets[indiceVec2] = vecSommets[indiceVec1];
                }
                else {
                    for(unsigned int l = 0; l < vecSommets.size(); l++) {
                        if((vecSommets[l] == vecSommets[indiceVec1])&&((int)l != indiceVec1)) { vecSommets[l] = vecSommets[indiceVec2]; }
                    }
                    vecSommets[indiceVec1] = vecSommets[indiceVec2];
                }
                for(unsigned int m = 0; m < vecSommets.size(); m++) {
                }

                if (vecSommets == allRight)         { arbresCouvrants.push_back(vec[i]); break; } // Si tous les sommets sont ajoutes on ajoute la configuration a arbresCouvrants et on sort de la boucle
            }
        }
    }
    return arbresCouvrants;
}

void Graphe::affichagePareto(bool toggleDijkstra, int indicePoidsDijkstra) {
    std::vector<std::vector<float>*> vecPoidsSolutions;
    std::vector<std::vector<bool>*> vecSolutionsTaille;
    if(toggleDijkstra)  vecSolutionsTaille = ensembleGraphesPartiels(true); /// Retourne tous les graphes partiels d'au moins ordre-1 aretes
    else                vecSolutionsTaille = ensembleGraphesPartiels(false);/// Retourne tous les graphes partiels de ordre-1 aretes
    std::vector<std::vector<bool>*> vecSolutionsCouvrantes = ensembleArbresCouvrants(vecSolutionsTaille); /// Retourne tous les arbres couvrant faisant partie de vecSolutionsTaille

    /// PERMET DE FAIRE LE TRI EN CREANT DES VECTEURS DE FLOAT CONTENANT LES POIDS. MEILLEURE OPTIMISATION QUE CREER UN GRAPHE POUR TESTER -> GAIN DE TEMPS
    if(toggleDijkstra) {
        for(unsigned int i = 0; i < vecSolutionsCouvrantes.size(); ++i) {
            std::vector<float>* poidsSolution =  new std::vector<float>;
            *poidsSolution = resultatGraphe(*(vecSolutionsCouvrantes[i]), indicePoidsDijkstra);
            (*poidsSolution).push_back(i);
            vecPoidsSolutions.push_back(poidsSolution);
        }
    } else {
        for(unsigned int i = 0; i < vecSolutionsCouvrantes.size(); ++i) {
            std::vector<float>* poidsSolution =  new std::vector<float>;
            *poidsSolution = resultatGraphe(*(vecSolutionsCouvrantes[i]));
            (*poidsSolution).push_back(i);
            vecPoidsSolutions.push_back(poidsSolution);
        }
    }
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
    dessinerPareto(vecIndicesSolutionsNonDominees, vecSolutionsCouvrantes, vecPoidsSolutions, minX, maxX, minY, maxY);

    /// On désalloue la mémoire (on ne désalloue pas vecSolutionsCouvrantes car il est inclu dans vecSolutionTaille)
    for(auto ptr : vecSolutionsTaille) {
        delete ptr;
    }
    for(auto ptr : vecPoidsSolutions) {
        delete ptr;
    }
}

std::vector<int> Graphe::giveSolutionsNonDominees(std::vector<std::vector<float>*> vecPoidsSolutions, int& maxX, int& minY) {
    std::vector<int> vecIndicesSolutionsNonDominees;
    vecIndicesSolutionsNonDominees.push_back((*vecPoidsSolutions[0]).back());
    for(unsigned int i = 1; i < vecPoidsSolutions.size(); ++i) {
        if ((*vecPoidsSolutions[i])[0] != maxX) {
            if ((*vecPoidsSolutions[i])[1] < minY) {
                vecIndicesSolutionsNonDominees.push_back((*vecPoidsSolutions[i]).back());
                maxX = (*vecPoidsSolutions[i])[0];
                minY = (*vecPoidsSolutions[i])[1];
            }
        }
    }
    return vecIndicesSolutionsNonDominees;
}

void Graphe::dessinerPareto(std::vector<int> vecIndicesSolutionsNonDominees, std::vector<std::vector<bool>*> vecSolutionsCouvrantes,
                            std::vector<std::vector<float>*> vecPoidsSolutions, int minX, int maxX, int minY, int maxY)
{
    Svgfile svgout("output.svg", 5000, 20000);
    dessiner(svgout, 275, -50);
    const int debutX = 1100;
    const int debutY = 100;
    const int largeur = 800;
    const int hauteur = 800;
    svgout.addLine(debutX, hauteur+debutY-30, debutX+largeur, hauteur+debutY-30);
    svgout.addLine(debutX+30, debutY, debutX+30, debutY+hauteur);

    int rayonFaux = 2, rayonVrai = 5;
    float coeffBasX, coeffHautX, coeffBasY, coeffHautY; /// CoeffBas : petit -> Proche de 0; coeffHaut -> petit : proche de la fin des axes
    if(vecSolutionsCouvrantes.size() < 100) {
        coeffBasX = 0.25; coeffHautX = 0.5; coeffBasY = 0.25; coeffHautY = 0.5;
    } else if (vecSolutionsCouvrantes.size() < 1000) {
        coeffBasX = 0.1; coeffHautX = 0.5; coeffBasY = 0.1; coeffHautY = 0.7;
    } else{
        coeffBasX = 0.1; coeffHautX = 0.5; coeffBasY = 0.1; coeffHautY = 0.6;
    }

    float lastX = -1, lastY = -1, coeffRayon = 1;
    for(auto it : vecPoidsSolutions) {
        if ((*it)[0] != lastX || (*it)[1] != lastY) {
            float posX = mapping((*it)[0], minX, maxX, debutX+coeffBasX*largeur, debutX+largeur-coeffHautX*largeur);
            float posY = mapping((*it)[1], minY, maxY, debutY+hauteur-coeffBasY*hauteur, debutY+hauteur-(1-coeffHautY)*hauteur);
            lastX = (*it)[0];
            lastY = (*it)[1];
            svgout.addDisk(posX, posY, rayonFaux*coeffRayon, 100, svgout.makeRGB(255,0,0));
            coeffRayon = 1;
        } else {
            coeffRayon+=.02;
        }
    }
    for(auto it : vecIndicesSolutionsNonDominees) {
        std::vector<float> vecPoidsNonDominees = resultatGraphe(*(vecSolutionsCouvrantes[it]));
        float posX = mapping(vecPoidsNonDominees[0], minX, maxX, debutX+coeffBasX*largeur, debutX+largeur-coeffHautX*largeur);
        float posY = mapping(vecPoidsNonDominees[1], minY, maxY, debutY+hauteur-coeffBasY*hauteur, debutY+hauteur-(1-coeffHautY)*hauteur);
        svgout.addDisk(posX, posY, rayonVrai, 100, svgout.makeRGB(0,200,0));
    }

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
        for(unsigned int j = 0; j <  (resultatGraphe(solution)).size(); ++j) {
            std::stringstream number;
            number << (int)(resultatGraphe(solution))[j];
            poids.append(number.str());
            if(j != (resultatGraphe(solution)).size()-1)    poids.append(" ; ");
            else                                            poids.append(")");
            svgout.addText(x+(minX+maxX)/4-25, y+minY/2-20, poids);
        }
        if ((i+1)%3 == 0)   { y+=250; x = 100; }
        else                { x+=300; }
    }
}

void Graphe::dessiner(Svgfile& svgout, int x, int y, bool toggleText, float coeffTaille) {
    std::vector<bool> vecBool(m_aretes.size(), 1);
    dessiner(svgout, vecBool, x, y, toggleText, coeffTaille);
}

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

Graphe::~Graphe()
{ }
