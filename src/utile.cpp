#include "utile.h"

/// TROUVE SUR INTERNET
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // Utilise dans le menu interactif
COORD CursorPosition; // Utilise dans le menu interactif
void gotoXY(int x,int y); // Utilise dans le menu interactif

/// Permett d'afficher des instructions
void afficherInstructions(std::string fichier, std::string fichierPoids, int& x, int& y) {
    system("cls");
    x = 0; y = 1;
    std::cout << "Que voulez-vous faire ? Fichier ouvert : " << fichier << "; Fichier poids ouvert : " << fichierPoids << std::endl
              << "0) Quitter" << std::endl
              << "1) Choisir un fichier texte" << std::endl
              << "2) Dessiner le graphe" << std::endl
              << "3) Lancer Kruskal" << std::endl
              << "4) Lancer Pareto simple" << std::endl
              << "5) Lancer Pareto en choisissant une ponderation en tant que temps de trajet" << std::endl
              << "6) Lancer Pareto en ne gardant que les aretes \"parfaites\" (double Kruskal)"  << std::endl
              << "7) Optimiser le pire chemin d'un graphe a partir d'un fichier texte"  << std::endl;
}

/// OUVRE UN MENU PROPOSANT DIFFERENTES CHOSES A FAIRE A L'UTILISATEUR
void menu() {
    bool run = 1;
    int x, y;

    std::string fichier = "files/broadway.txt", fichierPoids = "files/broadway_weights_0.txt", fichierAOptimiser = "files/broadwayTest.txt", fichierPoidsAOptimiser = "files/broadwayTestPoids.txt";
    afficherInstructions(fichier, fichierPoids, x, y);

    do{
        gotoXY(x, y);
        system("pause>nul");  // Arrete tout (nul permet de ne pas avoir d'affichage)

        if (GetAsyncKeyState(VK_DOWN) && y < 8) {
			++y;
			gotoXY(x,y);
			continue;
		}

		else if (GetAsyncKeyState(VK_UP) && y > 1) {
            --y;
            gotoXY(x, y);
            continue;
        }

        else if (GetAsyncKeyState(VK_RETURN)) {

            if(y == 1) { run = 0; system("cls"); }

            else if(y == 2) {
                choisirFichiers(fichier, fichierPoids);
                system("pause>nul");
                afficherInstructions(fichier, fichierPoids, x, y);
            }

            else if(y == 3) {
                system("cls");
                bool poids;
                std::cout << "Voulez-vous afficher les poids des aretes ? (1 : Oui / 0 : Non) : ";
                entreeClavier(poids);
                if(1) {
                    Graphe myGraphe{fichier, fichierPoids};
                    Svgfile svgout("output.svg", 10000, 10000);
                    myGraphe.dessiner(svgout, 450, 0, poids, 2);
                    myGraphe.libererMemoire();
                }
                std::cout << "Termine !"; system("pause>nul");
                afficherInstructions(fichier, fichierPoids, x, y);
            }

            else if(y == 4) {
                system("cls");
                Graphe myGraphe{fichier, fichierPoids};
                Graphe myKruskal = myGraphe.parcourKruskal(choisirPoids(&myGraphe));
                Svgfile svgout;
                myKruskal.dessiner(svgout);
                myGraphe.libererMemoire();
                std::cout << "Termine !"; system("pause>nul");
                afficherInstructions(fichier, fichierPoids, x, y);
            }

            else if(y == 5) {
                system("cls");
                Graphe myGraphe{fichier, fichierPoids};
                myGraphe.affichagePareto();
                myGraphe.libererMemoire();
                std::cout << "Termine !"; system("pause>nul");
                afficherInstructions(fichier, fichierPoids, x, y);
            }

            else if(y == 6) {
                system("cls");
                Graphe myGraphe{fichier, fichierPoids};
                myGraphe.affichagePareto(1, choisirPoids(&myGraphe));
                std::cout << "Termine !"; system("pause>nul");
                afficherInstructions(fichier, fichierPoids, x, y);
            }

            else if(y == 7){
                system("cls");
                Graphe myGraphe{fichier, fichierPoids};
                myGraphe.affichagePareto(2);
                myGraphe.libererMemoire();
                std::cout << "Termine !"; system("pause>nul");
                afficherInstructions(fichier, fichierPoids, x, y);
            }
            else if(y == 8){
                system("cls");
                choisirFichiersExtension(fichier, fichierPoids, fichierAOptimiser, fichierPoidsAOptimiser);
                Graphe myGraphe{fichier, fichierPoids};
                Graphe myGrapheOptimisable{fichierAOptimiser, fichierPoidsAOptimiser};
                myGraphe.pireCheminDijkstra(myGraphe, myGrapheOptimisable);
                myGraphe.libererMemoire();
                myGrapheOptimisable.libererMemoire();
                std::cout << "Termine !"; system("pause>nul");
                afficherInstructions(fichier, fichierPoids, x, y);
            }
            continue;
        }

    } while(run);
}

/// OUVRE UN MENU PERMETTANT A L'UTILISATEUR DE CHOISIR QUEL FICHIER UTILISER
void choisirFichiers(std::string& fichier, std::string& fichierPoids) {
    system("cls");
    int x = 0, y = 1;
    bool run = 1;

    std::cout << "Quel fichier voulez-vous choisir ? Fichier actuel : " << fichier << "; Fichier poids actuel : " << fichierPoids << std::endl
              << "0) Broadway" << std::endl
              << "1) Cubetown" << std::endl
              << "2) Triville" << std::endl
              << "3) Manhattan" << std::endl
              << "4) Autre" << std::endl;
    do{
        gotoXY(x, y);
        system("pause>nul");  // Arrete tout (nul permet de ne pas avoir d'affichage)

        if (GetAsyncKeyState(VK_DOWN) && y < 5) {
			++y;
			gotoXY(x,y);
			continue;
		}

		else if (GetAsyncKeyState(VK_UP) && y > 1) {
            --y;
            gotoXY(x, y);
            continue;
        }

        else if (GetAsyncKeyState(VK_RETURN)) {
            if(y == 1) {
                system("cls"); std::cout << "Broadway choisi." << std::endl << "Entrez le numero du fichier de poids: ";
                int chiffre;
                entreeClavier(chiffre);
                if (chiffre >= 0 && chiffre <= 2) {
                    fichier = "files/broadway.txt";
                    fichierPoids = "files/broadway_weights_"+std::to_string(chiffre)+".txt";
                    std::cout << "Enregistre !";
                } else {
                    std::cout << std::endl << "Mauvaise valeur, annulation";
                }
                run = 0;
            } else if(y == 2) {
                system("cls"); std::cout << "Cubetown choisi." << std::endl << "Le fichier de poids 0 a ete choisi par defaut" << std::endl;
                fichier = "files/cubetown.txt";
                fichierPoids = "files/cubetown_weights_0.txt";
                std::cout << "Enregistre !";
                run = 0;
            } else if(y == 3) {
                system("cls"); std::cout << "Triville choisi." << std::endl << "Entrez le numero du fichier de poids: ";
                int chiffre;
                entreeClavier(chiffre);
                if (chiffre >= 0 && chiffre <= 1) {
                    fichier = "files/triville.txt";
                    fichierPoids = "files/triville_weights_"+std::to_string(chiffre)+".txt";
                    std::cout << "Enregistre !";
                } else {
                    std::cout << std::endl << "Mauvaise valeur, annulation";
                }
                run = 0;
            } else if(y == 4) {
                system("cls"); std::cout << "Manhattan choisi." << std::endl << "Entrez le numero du fichier de poids: ";
                int chiffre;
                entreeClavier(chiffre);
                if (chiffre >= 0 && chiffre <= 2) {
                    fichier = "files/manhattan.txt";
                    fichierPoids = "files/manhattan_weights_"+std::to_string(chiffre)+".txt";
                    std::cout << "Enregistre !";
                } else {
                    std::cout << std::endl << "Mauvaise valeur, annulation";
                }
                run = 0;
            } else if(y == 5) {
                system("cls"); std::cout << "Entrez le chemin et le nom du fichier (ex : files/fichier.txt) (0 pour annuler) : ";
                std::string nom1;
                entreeClavier(nom1);
                std::cout << "Entrez le chemin et le nom du fichier de poids (0 pour annuler) : ";
                std::string nom2;
                entreeClavier(nom2);
                if (nom1 == "0" || nom2 == "0") { std::cout << std::endl << "Annule !"; }
                else{
                    fichier = nom1;
                    fichierPoids = nom2;
                    std::cout << std::endl << "Enregistre !";
                }
                run = 0;
            }
            continue;
        }
    } while(run);
}



/// FONCTION TROUVEE SUR http://www.cplusplus.com/forum/beginner/132595/
/// Permet de deplacer un le curseur de la console a la position (x;y)
void gotoXY(int x, int y)
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console,CursorPosition);
}

/// Retourne le poids choisi par l'utilisateur en verifiant qu'il y en a assez et que la saisie est bonne
int choisirPoids(Graphe* myGraphe) {
    system("cls");
    int choix = 0;
    bool run = 1;
    std::cout << "En fonction de quel poids voulez-vous le lancer ?" << std::endl;
    do{
        if(!(std::cin >> choix)) {
            std::cout << "Mauvaise entree" << std::endl;
            std::cin.clear(); // efface les bits d'erreurs
            std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' ); // supprime la ligne erronee dans le buffer
        }
        else if(choix >= (((*myGraphe).getAretes())[0])->getNbPoids() || choix < 0) {
            std::cout << "Il n'y a pas assez de poids ou la valeur n'est pas bonne" << std::endl;
        }
        else{
            run = 0;
        }
    } while(run);
    system("cls");

    return choix;
}

void choisirFichiersExtension(std::string& fichier, std::string& fichierPoids, std::string& fichierAOptimiser, std::string& fichierPoidsAOptimiser) {
    system("cls");
    int x = 0, y = 2;
    bool run = 1;

    std::cout << "Quel fichier voulez-vous choisir ? Fichier actuel : " << fichier << "; Fichier poids actuel : " << fichierPoids << "; Fichier a optimiser : " << fichierAOptimiser << "; Fichier poids a optimiser : " << fichierPoidsAOptimiser << std::endl
              << "1) Option de test avec un fichier par default " << std::endl
              << "2) Choisir nos fichier a la main " << std::endl;
    do{
        gotoXY(x, y);
        system("pause>nul");  // Arrete tout (nul permet de ne pas avoir d'affichage)

        if (GetAsyncKeyState(VK_DOWN) && y < 3) {
			++y;
			gotoXY(x,y);
			continue;
		}

		else if (GetAsyncKeyState(VK_UP) && y > 2) {
            --y;
            gotoXY(x, y);
            continue;
        }

        else if (GetAsyncKeyState(VK_RETURN)) {
            if(y == 2) {
                system("cls"); std::cout << "Broadway choisi." << std::endl;
                fichier = "files/broadway.txt";
                fichierPoids = "files/broadway_weights_0.txt";
                fichierAOptimiser = "files/broadwayTest.txt";
                fichierPoidsAOptimiser = "files/broadwayTestPoids.txt";
                std::cout << "Enregistre !";
                run = 0;
            }
            if(y == 3) {
                system("cls"); std::cout << "Entrez le chemin et le nom du fichier (ex : files/fichier.txt) (0 pour annuler) : ";
                std::string nom1;
                entreeClavier(nom1);
                std::cout << "Entrez le chemin et le nom du fichier de poids (0 pour annuler) : ";
                std::string nom2;
                entreeClavier(nom2);
                std::cout << "Entrez le chemin et le nom du fichier du graphe a optimiser (0 pour annuler) : ";
                std::string nom3;
                entreeClavier(nom3);
                std::cout << "Entrez le chemin et le nom du fichier du ficher de poids du graphe a optimiser (0 pour annuler) : ";
                std::string nom4;
                entreeClavier(nom4);
                if (nom1 == "0" || nom2 == "0" || nom3 == "0" || nom4 == "0") { std::cout << std::endl << "Annule !"; }
                else{
                    fichier = nom1;
                    fichierPoids = nom2;
                    fichierAOptimiser = nom3;
                    fichierPoidsAOptimiser = nom4;
                    std::cout << std::endl << "Enregistre !";
                }
                run = 0;
            }
            continue;
        }
    } while(run);
}
