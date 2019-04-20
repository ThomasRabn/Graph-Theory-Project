#include "svgfile.h"
#include "Graphe.h"
#include <windows.h>
#include <conio.h>
#include <cstdlib>

/// TROUVE SUR INTERNET
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // Utilise dans le menu interactif
COORD CursorPosition; // Utilise dans le menu interactif
void gotoXY(int x,int y); // Utilise dans le menu interactif

void menu();
void entreeClavier(auto& value);
void choisirFichiers(std::string& fichier, std::string& fichierPoids);

int main()
{
    menu();
    return 0;
}

void afficherInstructions(std::string fichier, std::string fichierPoids, int& x, int& y) {
    system("cls");
    x = 0; y = 1;
    std::cout << "Que voulez-faire ? Fichier ouvert : " << fichier << "; Fichier poids ouvert : " << fichierPoids << std::endl
              << "0) Quitter" << std::endl
              << "1) Choisir un fichier texte" << std::endl
              << "2) Dessiner le graphe" << std::endl
              << "3) Lancer Kruskal" << std::endl
              << "4) Lancer Pareto simple" << std::endl
              << "5) Lancer Pareto en choisissant une ponderation en tant que temps de trajet" << std::endl;
}

void menu() {
    bool run = 1;
    int x, y;

    std::string fichier = "files/broadway.txt", fichierPoids = "files/broadway_weights_0.txt";
    afficherInstructions(fichier, fichierPoids, x, y);

    do{
        gotoXY(x, y);
        system("pause>nul");  // Arrete tout (nul permet de ne pas avoir d'affichage)

        if (GetAsyncKeyState(VK_DOWN) && y < 6) {
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
                Graphe myGraphe{fichier, fichierPoids};
                Svgfile svgout;
                myGraphe.dessiner(svgout, 0, 0, 1, 2);
                std::cout << "Termine !"; system("pause>nul");
                afficherInstructions(fichier, fichierPoids, x, y);
            }

            else if(y == 4) {
                //Kruskal();
            }

            else if(y == 5) {
                system("cls");
                Graphe myGraphe{fichier, fichierPoids};
                myGraphe.affichagePareto();
                std::cout << "Termine !"; system("pause>nul");
                afficherInstructions(fichier, fichierPoids, x, y);
            }

            else if(y == 6) {
                system("cls");
                Graphe myGraphe{fichier, fichierPoids};
                myGraphe.affichagePareto(1, 1);
                std::cout << "Termine !"; system("pause>nul");
                afficherInstructions(fichier, fichierPoids, x, y);
            }
            continue;
        }

    } while(run);
}

void choisirFichiers(std::string& fichier, std::string& fichierPoids) {
    system("cls");
    int x = 0, y = 1;
    bool run = 1;

    std::cout << "Quel fichier voulez vous choisir ? Fichier actuel : " << fichier << "; Fichier poids actuel : " << fichierPoids << std::endl
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
void gotoXY(int x, int y)
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console,CursorPosition);
}

void entreeClavier(auto& value) {
    do{
        if(!(std::cin >> value)) {
            std::cout << "Mauvaise entree" << std::endl;
            std::cin.clear(); // efface les bits d'erreurs
            std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' ); // supprime la ligne erronee dans le buffer
        } else{ break; }
    } while(true);
}

/*if(!(std::cin >> choix)) {
    std::cout << "Mauvaise entree" << std::endl;
    std::cin.clear(); // efface les bits d'erreurs
    std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' ); // supprime la ligne erronee dans le buffer
}*/
