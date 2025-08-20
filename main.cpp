#include <iostream>
#include<array>
#include "graphics.h"
const int LARG = 1080;
const int HAUT = 920;
using namespace std;

const int MAX = 15; // L'utilisateur choisit entre 10 et 15 cases maximum


struct cases {
    int estBateau = 1; // 0 si non 1 si oui
    int estTouchee = 0; // 0 si non 1 si oui
    int estRevelee = 0;
    int id = -1; // id du bateau
};

struct bateau {
    int taille;
    int nbCasesTouchees = 0;
};

struct parametres {
    int tirSimple = 1; // 1 si oui 0 si non
    int taille = 1; // Taille de la grille : 1 -> 10x10, 0 -> 15x15
    int jeuContreOrdi = 1; // 1 si oui 0 si non
};

struct gadgets {
    int scannerUtilsable = 1 ; // 1 si oui 0 si non
    int bombeUtilisable = 1 ;
};

using tabCouleursCases = array<colors, 5>;
using grille = array<array<cases,MAX>,MAX>; // Tableau 2D pour sauvegarder l'état des cases (vide [15], bateau, touché, repéré, ...)
using tableauBateaux10x10 = array<bateau,7>; // 7 bateaux au max si 10x10
using tableauBateaux15x15 = array<bateau,9>; // 9 bateaux au max si 15x15

void outTexte (colors couleurArriere, colors couleurTexte, int x, int y, const char* texte) {
    setbkcolor(couleurArriere);
    setcolor(couleurTexte);
    outtextxy(x, y, texte);
}

void grilleJoueur1(const parametres &parametres, const grille &joueur1) {
    int var = (parametres.taille == 1) ? 40 : 30;
    int n = (parametres.taille == 1) ? 10 : 15;

    int px = var, py = var;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // --- Affichage de fond selon l'état révélé
            if (joueur1[i][j].estRevelee == 1) {
                if (joueur1[i][j].estBateau == 1) {
                    setcolor(GREEN); // Révélé + bateau
                    setfillstyle(SOLID_FILL, GREEN);
                } else {
                    setcolor(LIGHTGRAY); // Révélé mais vide
                    setfillstyle(SOLID_FILL, LIGHTGRAY);
                }
            } else {
                setcolor(WHITE); // Non révélé
                setfillstyle(SOLID_FILL, WHITE);
            }
            bar(px, py, px + var, py + var);

            // --- Ajout croix rouge ou grise si tiré dessus
            if (joueur1[i][j].estTouchee == 1) {
                if (joueur1[i][j].estBateau == 1) {
                    setcolor(RED); // Tir réussi
                } else {
                    setcolor(DARKGRAY); // Tir raté
                }
                line(px, py, px + var, py + var);
                line(px, py + var, px + var, py);
            }

            px += var;
        }
        px = var;
        py += var;
    }
}

void contourGrilleJoueur1 (const parametres &parametres, const grille &joueur1) {
    int var; // pour définir une taille différente pour les grilles.
    int n;
    if (parametres.taille == 1) { // 1 si 10x10
        var = 40;
        n = 10;
    }
    else {
        var = 30;
        n = 15;
    }

    int px = var, py = var;
    for(int i = 0 ; i <= n ; i++) {
        setcolor(DARKGRAY);
        line(px,py,px,py+(var*n));
        px += var;
    }

    px = var, py = var;
    for(int i = 0 ; i <= n ; i++) {
        setcolor(DARKGRAY);
        line(px,py,px+(var*n),py);
        py += var;
    }
}


void grilleJoueur2(const parametres &parametres, const grille &joueur2) {
    int var = (parametres.taille == 1) ? 40 : 30;
    int n = (parametres.taille == 1) ? 10 : 15;

    int px = var, py = var * (n + 2); // grille joueur 2 en dessous de joueur 1

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // --- Affichage de fond selon l'état révélé
            if (joueur2[i][j].estRevelee == 1) {
                if (joueur2[i][j].estBateau == 1) {
                    setcolor(GREEN); // Révélé + bateau
                    setfillstyle(SOLID_FILL, GREEN);
                } else {
                    setcolor(LIGHTGRAY); // Révélé mais vide
                    setfillstyle(SOLID_FILL, LIGHTGRAY);
                }
            } else {
                setcolor(WHITE); // Non révélé
                setfillstyle(SOLID_FILL, WHITE);
            }
            bar(px, py, px + var, py + var);

            // --- Ajout croix rouge ou grise si tiré dessus
            if (joueur2[i][j].estTouchee == 1) {
                if (joueur2[i][j].estBateau == 1) {
                    setcolor(RED); // Tir réussi
                } else {
                    setcolor(DARKGRAY); // Tir raté
                }
                line(px, py, px + var, py + var);
                line(px, py + var, px + var, py);
            }

            px += var;
        }
        px = var;
        py += var;
    }
}

void contourGrilleJoueur2 (const parametres &parametres, const grille &joueur2) {
    int var; // pour définir une taille différente pour les grilles.
    int n;
    if (parametres.taille == 1) { // 1 si 10x10
        var = 40;
        n = 10;
    }
    else {
        var = 30;
        n = 15;
    }

    int px = var , py = (var * n) + 2 * var;

    for(int i = 0 ; i <= n ; i++) {
        setcolor(DARKGRAY);
        line(px,py,px,py+(var*n));
        px += var;
    }

    px = var , py = (var * n) + 2 * var;
    for(int i = 0 ; i <= n ; i++) {
        setcolor(DARKGRAY);
        line(px,py,px+(var*n),py);
        py += var;
    }
}




void bouton(const char* texte, colors couleurBouton, colors couleurBordure,  int x, int y, int tailleBoutonLargeur, int tailleBoutonHauteur) {
    int largeur = tailleBoutonLargeur;
    int hauteur = tailleBoutonHauteur;

    setcolor(couleurBouton);
    bar(x,y,x+largeur,y+hauteur);

    setcolor(couleurBordure);
    rectangle(x,y,x+largeur,y+hauteur);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);  // Police du texte

    // Calcul de la position du texte pour le centrer
    int textWidth = textwidth(texte);
    int textHeight = textheight(texte);

    int textX = x + (largeur - textWidth) / 2;
    int textY = y + (hauteur - textHeight) / 2;

    // Affichage du texte
    setbkcolor(WHITE);
    setcolor(DARKGRAY);
    outtextxy(textX, textY, texte);
}

void affichageMenu(parametres &parametres) {

    // Bloc en haut a gauche
    outTexte(WHITE, DARKGRAY, 50, 20, "Mode de jeu");
    bouton("Jouer contre l'ordinateur",WHITE,(parametres.jeuContreOrdi == 1) ? GREEN : DARKGRAY,50,50,250,50);
    bouton("Jouer contre un autre joueur",WHITE,(parametres.jeuContreOrdi != 1) ? GREEN : DARKGRAY,50,120,250,50);

    // Bloc en bas a gauche
    outTexte(WHITE, DARKGRAY, 50, 220, "Taille des grilles de jeu");
    bouton("10x10",WHITE,(parametres.taille == 1) ? GREEN : DARKGRAY,50,245,50,50);
    bouton("15x15",WHITE,(parametres.taille != 1) ? GREEN : DARKGRAY,125,245,50,50);

    // Bloc en haut a droite
    outTexte(WHITE, DARKGRAY, 400, 20, "Bataille Navale");
    bouton("JOUER",WHITE,DARKGRAY,400,50,200,50);
    bouton("Regles du jeu",WHITE,DARKGRAY,425,120,150,50);


    // Bloc en bas a droite
    outTexte(WHITE, DARKGRAY, 300, 220, "Mode de tir");
    bouton("Tir simple",WHITE,(parametres.tirSimple == 1) ? GREEN : DARKGRAY,300,245,100,50);
    bouton("Tir double",WHITE,(parametres.tirSimple != 1) ? GREEN : DARKGRAY,425,245,100,50);

    bouton("Quitter",WHITE,DARKGRAY,564,257,70,25);
}

void menu(parametres &parametres, bool &jouer , bool &regles) {
    int hautm = 325, largm = 675;

    jouer = false;
    regles = false;

    opengraphsize(largm, hautm);
    bar(0, 0, largm, hautm);
    affichageMenu(parametres);

    int x, y;


    while (jouer == false && regles == false) {
        getmouse(x, y);
        cout << x << y;

        // Jouer contre l'ordinateur
        if (x > 50 && x < 300 && y > 50 && y < 100) {
            parametres.jeuContreOrdi = 1;
            affichageMenu(parametres);
        }

        // Jouer contre un autre joueur
        if (x > 50 && x < 300 && y > 120 && y < 170) {
            parametres.jeuContreOrdi = 0;
            affichageMenu(parametres);
        }

        // Taille 10x10
        if (x > 50 && x < 100 && y > 245 && y < 295) {
            parametres.taille = 1;
            affichageMenu(parametres);
        }

        // Taille 15x15
        if (x > 125 && x < 175 && y > 245 && y < 295) {
            parametres.taille = 0;
            affichageMenu(parametres);
        }

        // Bouton JOUER
        if (x > 400 && x < 600 && y > 50 && y < 100) {
            jouer = true;
            closegraph();
        }

        // Bouton Règles du jeu
        if (x > 425 && x < 575 && y > 120 && y < 170) {
            regles = true;
        }

        // Mode Tir simple
        if (x > 299 && x < 401 && y > 245 && y < 295) {
            parametres.tirSimple = 1;
            affichageMenu(parametres);
        }

        // Mode Tir double
        if (x > 424 && x < 526 && y > 245 && y < 295) {
            parametres.tirSimple = 0;
            affichageMenu(parametres);
        }

        // Quitter
        if (x > 563 && x < 635 && y > 256 && y < 282) {
                closegraph();
        }
    }
}


void confirmation(bool &confirm) {
    int x, y;
    bool ok = false;
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    bar(600, 400, 900, 500);
    bouton("Valider", WHITE, DARKGRAY, 625, 460, 100, 30);
    bouton("Annuler", WHITE, DARKGRAY, 775, 460, 100, 30);

    outtextxy(700, 425, "Valider le tir ?");

    do {
        getmouse(x, y);
        if ((x >= 625 && x <= 725) && (y >= 460 && y <= 490)) {
            confirm = true;
            ok = true;
        } else if ((x >= 775 && x <= 875) && (y >= 460 && y <= 490)) {
            confirm = false;
            ok = true;
        }
    } while (!ok);

    // Nettoyage de la zone après le clic
    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, WHITE);
    bar(600, 400, 900, 500);
}

void tips(char* texte) {
    int x, y;
    setcolor(DARKGRAY);

    outtextxy(745, 425, texte);


}




void tir(grille &grille, const parametres &parametres, int &succes, bool joueur1, tableauBateaux10x10 &tableauBateauxJoueur) {
    succes = 0;
    int x, y;
    int var = (parametres.taille == 1) ? 40 : 30;
    int n = (parametres.taille == 1) ? 10 : 15;
    int j, i, jc, ic;
    bool confirm = true;

    // Attente du clic
    getmouse(x, y);

    if(joueur1) {
        while((x > n*var + var || x < var) || (y > n*var + var || y < var)) {
            getmouse(x, y);
        }
    } else {
        while((x > n*var + var || x < var) || (y > n*var + n*var + var + var || y < n*var + var + var)) {
            getmouse(x, y);
        }
    }

    ic = (y - var) / var;
    jc = (x - var) / var;

    if(joueur1) {
        i = (y - var) / var;
        j = (x - var) / var;
    } else {
        i = (y - (var * n) - 2 * var) / var;
        j = (x - var) / var;
    }

    if(grille[i][j].estTouchee != 0) {
        tips("Erreur : cette case a déjà été ciblée.");
        succes = -1;
        return;
    } else {
        setcolor(BLUE);
        setfillstyle(SOLID_FILL, BLUE);
        bar(var + jc * var + 1, var + ic * var + 1, var + (jc + 1) * var, var + (ic + 1) * var);

        confirmation(confirm);
        if(confirm) {
            grille[i][j].estTouchee = 1;
            if(grille[i][j].estBateau == 1) {
                tableauBateauxJoueur[grille[i][j].id].nbCasesTouchees++;
            }
            succes = 1;
        } else {
            setcolor(WHITE);
            bar(var + jc * var, var + ic * var, var + (jc + 1) * var, var + (ic + 1) * var);
            succes = 0;
        }
    }
}


void bombe(parametres &parametres, grille &grille, gadgets &gadgetJoueur, int &succes, bool joueur1, tableauBateaux10x10 &tableauBateauxJoueur) {
    succes = 0;
    int x, y;
    int var = (parametres.taille == 1) ? 40 : 30;
    int n = (parametres.taille == 1) ? 10 : 15;
    int j, i, jc, ic;
    bool confirm;

    getmouse(x, y);

    if (joueur1) {
        while ((x > n * var + var || x < var) || (y > n * var + var || y < var)) {
            getmouse(x, y);
        }
    } else {
        while ((x > n * var + var || x < var) || (y > n * var + n * var + var + var || y < n * var + var + var)) {
            getmouse(x, y);
        }
    }

    ic = (y - var) / var;
    jc = (x - var) / var;

    if (joueur1) {
        i = (y - var) / var;
        j = (x - var) / var;
    } else {
        i = (y - (var * n) - 2 * var) / var;
        j = (x - var) / var;
    }

    // Interdiction de viser une case déjà touchée
    if (grille[i][j].estTouchee != 0) {
        tips("Erreur : cette case a déjà été ciblée.");
        succes = -1;
        return;
    }

    // Affichage de la zone de bombe en BLEU
    setcolor(BLUE);
    setfillstyle(SOLID_FILL, BLUE);
    bar(var + jc * var + 1, var + ic * var + 1, var + (jc + 1) * var - 1, var + (ic + 1) * var - 1);

    setcolor(LIGHTBLUE);
    setfillstyle(SOLID_FILL, LIGHTBLUE);

    // Droite
    if (j + 1 < n)
        bar(var + (jc + 1) * var + 1, var + ic * var + 1, var + (jc + 2) * var - 1, var + (ic + 1) * var - 1);

    // Gauche
    if (j - 1 >= 0)
        bar(var + (jc - 1) * var + 1, var + ic * var + 1, var + jc * var - 1, var + (ic + 1) * var - 1);

    // Haut
    if (i - 1 >= 0)
        bar(var + jc * var + 1, var + (ic - 1) * var + 1, var + (jc + 1) * var - 1, var + ic * var - 1);

    // Bas
    if (i + 1 < n)
        bar(var + jc * var + 1, var + (ic + 1) * var + 1, var + (jc + 1) * var - 1, var + (ic + 2) * var - 1);

    // Confirmation
    confirmation(confirm);
    if (confirm) {
        // Marquage des cases touchées
        grille[i][j].estTouchee = 1;
        if (grille[i][j].estBateau == 1)
            tableauBateauxJoueur[grille[i][j].id].nbCasesTouchees++;

        if (j + 1 < n) {
            grille[i][j + 1].estTouchee = 1;
            if (grille[i][j + 1].estBateau == 1)
                tableauBateauxJoueur[grille[i][j + 1].id].nbCasesTouchees++;
        }

        if (j - 1 >= 0) {
            grille[i][j - 1].estTouchee = 1;
            if (grille[i][j - 1].estBateau == 1)
                tableauBateauxJoueur[grille[i][j - 1].id].nbCasesTouchees++;
        }

        if (i + 1 < n) {
            grille[i + 1][j].estTouchee = 1;
            if (grille[i + 1][j].estBateau == 1)
                tableauBateauxJoueur[grille[i + 1][j].id].nbCasesTouchees++;
        }

        if (i - 1 >= 0) {
            grille[i - 1][j].estTouchee = 1;
            if (grille[i - 1][j].estBateau == 1)
                tableauBateauxJoueur[grille[i - 1][j].id].nbCasesTouchees++;
        }

        gadgetJoueur.bombeUtilisable = 0;
        succes = 1;
    } else {
        // Annulation
        setcolor(WHITE);
        bar(var + jc * var, var + ic * var, var + (jc + 1) * var, var + (ic + 1) * var);
        succes = 0;
    }
}


void radar(parametres &parametres, grille &grille, gadgets &gadgetJoueur, int &succes, bool joueur1, tableauBateaux10x10 &tableauBateauxJoueur) {
    succes = 0;
    int x, y;
    int var = (parametres.taille == 1) ? 40 : 30;
    int n = (parametres.taille == 1) ? 10 : 15;
    int j, i, jc, ic;
    bool confirm;

    // Attente du clic
    getmouse(x, y);

    if (joueur1) {
        while ((x > n * var + var || x < var) || (y > n * var + var || y < var)) {
            getmouse(x, y);
        }
    } else {
        while ((x > n * var + var || x < var) || (y > n * var + n * var + var + var || y < n * var + var + var)) {
            getmouse(x, y);
        }
    }

    ic = (y - var) / var;
    jc = (x - var) / var;

    if (joueur1) {
        i = (y - var) / var;
        j = (x - var) / var;
    } else {
        i = (y - (var * n) - 2 * var) / var;
        j = (x - var) / var;
    }

    // Empêche d'utiliser le radar sur une case déjà touchée
    if (grille[i][j].estTouchee != 0) {
        tips("Erreur : cette case a déjà été ciblée, radar inutilisable ici.");
        succes = -1;
        return;
    }

    // Affichage zone radar (centre bleu foncé, autour bleu clair)
    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            int ii = i + di;
            int jj = j + dj;

            if (ii >= 0 && ii < n && jj >= 0 && jj < n) {
                int pixelY = var + (ic + di) * var;
                int pixelX = var + (jc + dj) * var;

                if (di == 0 && dj == 0) {
                    setcolor(BLUE);
                    setfillstyle(SOLID_FILL, BLUE);
                } else {
                    setcolor(LIGHTBLUE);
                    setfillstyle(SOLID_FILL, LIGHTBLUE);
                }

                bar(pixelX + 1, pixelY + 1, pixelX + var, pixelY + var);
            }
        }
    }

    // Confirmation du joueur
    confirmation(confirm);
    if (confirm) {
        for (int di = -1; di <= 1; ++di) {
            for (int dj = -1; dj <= 1; ++dj) {
                int ii = i + di;
                int jj = j + dj;

                if (ii >= 0 && ii < n && jj >= 0 && jj < n) {
                    grille[ii][jj].estRevelee = 1;
                }
            }
        }
        gadgetJoueur.scannerUtilsable = 0;
        succes = 1;
    } else {
        setcolor(WHITE);
        bar(var + jc * var, var + ic * var, var + (jc + 1) * var, var + (ic + 1) * var);
        succes = 0;
    }
}







int main() {

    int succes = 0;
    bool regles, jouer, joueur = false;

    parametres parametres;

    grille joueur1;
    grille joueur2;

    gadgets gadgetJoueur1;
    gadgets gadgetJoueur2;

    menu(parametres, regles, jouer);

    tableauBateaux10x10 tableauBateauxJoueur1_10;
    tableauBateaux10x10 tableauBateauxJoueur2_10;

    tableauBateaux15x15 tableauBateauxJoueur1_15;
    tableauBateaux15x15 tableauBateauxJoueur2_15;

    if(parametres.taille == 1) {


        tableauBateauxJoueur1_10[0].taille = 2;
        tableauBateauxJoueur1_10[1].taille = 2;
        tableauBateauxJoueur1_10[2].taille = 2;
        tableauBateauxJoueur1_10[3].taille = 3;
        tableauBateauxJoueur1_10[4].taille = 3;
        tableauBateauxJoueur1_10[5].taille = 4;
        tableauBateauxJoueur1_10[6].taille = 5;

    // ************************************************************************

        tableauBateauxJoueur2_10[0].taille = 2;
        tableauBateauxJoueur2_10[1].taille = 2;
        tableauBateauxJoueur2_10[2].taille = 2;
        tableauBateauxJoueur2_10[3].taille = 3;
        tableauBateauxJoueur2_10[4].taille = 3;
        tableauBateauxJoueur2_10[5].taille = 4;
        tableauBateauxJoueur2_10[6].taille = 5;
    }

    else  {


        tableauBateauxJoueur1_15[0].taille = 2;
        tableauBateauxJoueur1_15[1].taille = 2;
        tableauBateauxJoueur1_15[2].taille = 2;
        tableauBateauxJoueur1_15[3].taille = 3;
        tableauBateauxJoueur1_15[4].taille = 3;
        tableauBateauxJoueur1_15[5].taille = 3;
        tableauBateauxJoueur1_15[6].taille = 4;
        tableauBateauxJoueur1_15[7].taille = 4;
        tableauBateauxJoueur1_15[8].taille = 5;

    // ************************************************************************


        tableauBateauxJoueur2_15[0].taille = 2;
        tableauBateauxJoueur2_15[1].taille = 2;
        tableauBateauxJoueur2_15[2].taille = 2;
        tableauBateauxJoueur2_15[3].taille = 3;
        tableauBateauxJoueur2_15[4].taille = 3;
        tableauBateauxJoueur2_15[5].taille = 3;
        tableauBateauxJoueur2_15[6].taille = 4;
        tableauBateauxJoueur2_15[7].taille = 4;
        tableauBateauxJoueur2_15 [8].taille = 5;
    }





    opengraphsize(LARG,HAUT);
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    bar(0,0,LARG,HAUT);
    grilleJoueur1(parametres,joueur1);
    contourGrilleJoueur1(parametres,joueur1);
    grilleJoueur2(parametres,joueur2);
    contourGrilleJoueur2(parametres,joueur2);

    while(succes == 0) {
//        radar(parametres, joueur2, gadgetJoueur2,succes,joueur);
        tir(joueur2, parametres, succes,joueur,tableauBateauxJoueur1_10);
        grilleJoueur2(parametres,joueur2);
        contourGrilleJoueur2(parametres,joueur2);

    }
    tips("Impossible de tirer ici");



    getch();

}









