#include <iostream>
#include <algorithm>
#include <ctime>
#include <array>
#include "graphics.h"

const int LARG = 1080;
const int HAUT = 1000;
using namespace std;

const int MAX = 15; // L'utilisateur choisit entre 10 et 15 cases maximum
using directions = array<array<int,2>,4>;

struct cases {
    int estBateau = 0; // 0 si non 1 si oui
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
    int scannerUtilisable = 1 ; // 1 si oui 0 si non
    int bombeUtilisable = 1 ;
};

using tabCouleursCases = array<colors, 5>;
using grille = array<array<cases,MAX>,MAX>; // Tableau 2D pour sauvegarder l'état des cases (vide [15], bateau, touché, repéré, ...)
using tableauBateaux = array<bateau,9>; // 9 bateaux au max si 15x15

void outTexte (colors couleurArriere, colors couleurTexte, int x, int y, const char* texte) {
    setbkcolor(couleurArriere);
    setcolor(couleurTexte);
    outtextxy(x, y, texte);
}

void afficherRegle() {



    opengraphsize(1200, 940); // Taille adaptée au contenu

    setfillstyle(SOLID_FILL, WHITE);
    bar(0, 0, 1200, 940);  // Boîte de fond des règles

    setbkcolor(WHITE);
    setcolor(DARKGRAY);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

    int x = 70, y = 70;
    int ligne = 20;

    outtextxy(x, y, "REGLES DU JEU : BATAILLE NAVALE");
    y += 2 * ligne;

    outtextxy(x, y, "Objectif :");
    y += ligne;
    outtextxy(x + 20, y, "Détruire tous les bateaux ennemis avant que les votres ne soient coulés.");
    y += 2 * ligne;

    outtextxy(x, y, "Placement des bateaux :");
    y += ligne;
    outtextxy(x + 20, y, "- Vous placez plusieurs bateaux sur votre grille.");
    y += ligne;
    outtextxy(x + 20, y, "- En mode 10x10 : 1x5 cases, 1x4 cases, 2x3 cases, 3x2 cases");
    y += ligne;
    outtextxy(x + 20, y, "- En mode 15x15 : 1x5 cases, 2x4 cases, 3x3 cases, 3x2 cases");
    y += ligne;
    outtextxy(x + 20, y, "- Les bateaux doivent etre placés à l'intérieur de la grille,");
    y += ligne;
    outtextxy(x + 40, y, "horizontalement ou verticalement.");
    y += ligne;
    outtextxy(x + 20, y, "- Les bateaux ne doivent pas se toucher (sauf par les coins) ni se chevaucher.");
    y += 2 * ligne;

    outtextxy(x, y, "Deroulement du jeu :");
    y += ligne;
    outtextxy(x + 20, y, "- Vous avez une grille de tir pour chaque joueur (precisé en haut des grilles).");
    y += ligne;
    outtextxy(x + 20, y, "- A chaque tour, vous pouvez :");
    y += ligne;
    outtextxy(x + 40, y, "* Soit tirer sur une case");
    y += ligne;
    outtextxy(x + 40, y, "* Soit utiliser un gadget");
    y += 2 * ligne;

    outtextxy(x, y, "Gadgets (utilisables une fois chacun) :");
    y += ligne;
    outtextxy(x + 20, y, "- Radar : révèle une zone ennemie (vert si bateau, gris sinon).");
    y += ligne;
    outtextxy(x + 20, y, "- Bombe : attaque une zone étendue.");
    y += 2 * ligne;

    outtextxy(x, y, "Légende des couleurs :");
    y += ligne;
    outtextxy(x + 20, y, "- Blanc : Case inconnue pour le joueur");
    y += ligne;
    outtextxy(x + 20, y, "- Bleu : Case sélectionnée");
    y += ligne;
    outtextxy(x + 20, y, "- Vert : Bateau repéré par radar");
    y += ligne;
    outtextxy(x + 20, y, "- Gris : Zone radar sans bateau");
    y += ligne;
    outtextxy(x + 20, y, "- Rouge : Bateau coulé");
    y += ligne;
    outtextxy(x + 20, y, "- Croix noire : Tir raté");
    y += ligne;
    outtextxy(x + 20, y, "- Croix rouge : Touché");
    y += 3 * ligne;

    setcolor(RED);
    outtextxy(x, y, "Appuyez sur une touche pour continuer...");
    getch();

    closegraph(); // Ferme la fenêtre après validation
}








void grilleJoueur1(const parametres &parametres, const grille &joueur1, const tableauBateaux &tableauBateauxJoueur1) {
    int var = (parametres.taille == 1) ? 40 : 30;
    int n = (parametres.taille == 1) ? 10 : 15;

    int px = var, py = var;

    outTexte(WHITE, DARKGRAY, var, var / 2, "Grille du joueur 1");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            int couleur = WHITE;

            if (joueur1[i][j].estBateau == 1) {
                int id = joueur1[i][j].id;
                if (tableauBateauxJoueur1[id].nbCasesTouchees == tableauBateauxJoueur1[id].taille) {
                    couleur = RED; // Bateau entièrement coulé
                } else if (joueur1[i][j].estRevelee == 1) {
                    couleur = GREEN; // Case révélée et bateau touché, mais pas encore coulé
                } else {
                    couleur = WHITE; // Bateau non révélé, non coulé
                }
            } else if (joueur1[i][j].estRevelee == 1) {
                couleur = LIGHTGRAY; // Mer révélée
            }

            setcolor(couleur);
            setfillstyle(SOLID_FILL, couleur);
            bar(px, py, px + var, py + var);

            // Croix si la case a été tirée dessus
            if (joueur1[i][j].estTouchee == 1) {
                setcolor(joueur1[i][j].estBateau == 1 ? RED : DARKGRAY);
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


void grilleJoueur2(const parametres &parametres, const grille &joueur2, const tableauBateaux &tableauBateauxJoueur2) {
    int var = (parametres.taille == 1) ? 40 : 30;
    int n = (parametres.taille == 1) ? 10 : 15;

    int px = var, py = var * (n + 2); // grille joueur 2 en dessous de joueur 1

    outTexte(WHITE, DARKGRAY, var, py - var / 2, "Grille du joueur 2");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int couleur = WHITE;

            if (joueur2[i][j].estBateau == 1) {
                int id = joueur2[i][j].id;
                if (tableauBateauxJoueur2[id].nbCasesTouchees == tableauBateauxJoueur2[id].taille) {
                    couleur = RED; // Bateau entièrement coulé
                } else if (joueur2[i][j].estRevelee == 1) {
                    couleur = GREEN; // Case révélée et bateau touché mais pas coulé
                } else {
                    couleur = WHITE; // Bateau non révélé, non coulé
                }
            } else if (joueur2[i][j].estRevelee == 1) {
                couleur = LIGHTGRAY; // Mer révélée
            }

            setcolor(couleur);
            setfillstyle(SOLID_FILL, couleur);
            bar(px, py, px + var, py + var);

            // Croix si la case a été tirée dessus
            if (joueur2[i][j].estTouchee == 1) {
                setcolor(joueur2[i][j].estBateau == 1 ? RED : DARKGRAY);
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
    setfillstyle(SOLID_FILL, WHITE);
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
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

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
    bouton("Tir simple", WHITE, (parametres.tirSimple == 1) ? GREEN : DARKGRAY, 300, 245, 100, 50);
    bouton("Tir triple", WHITE, (parametres.tirSimple != 1) ? GREEN : DARKGRAY, 425, 245, 100, 50);


    bouton("Quitter",WHITE,DARKGRAY,564,257,70,25);
}

void menu(parametres &parametres, bool &jouer) {


    int hautm = 325, largm = 675;

    jouer = false;

    opengraphsize(largm, hautm);
    bar(0, 0, largm, hautm);
    affichageMenu(parametres);

    int x, y;


    while (jouer == false) {
        getmouse(x, y);

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
                closegraph();
                afficherRegle();
                opengraphsize(largm, hautm);
                bar(0, 0, largm, hautm);
                affichageMenu(parametres);
        }

        // Mode Tir simple
        if (x > 299 && x < 401 && y > 245 && y < 295) {
            parametres.tirSimple = 1;
            affichageMenu(parametres);
        }

        // Mode Tir triple
        if (x > 424 && x < 526 && y > 245 && y < 295) {
            parametres.tirSimple = 0;
            affichageMenu(parametres);
        }

        // Quitter
        if (x > 563 && x < 635 && y > 256 && y < 282) {
                closegraph();
                exit(0);
        }
    }
}

void tips(const char* texte, const parametres &parametres) { // pour le jeu
    int var = (parametres.taille == 1) ? 40 : 30;
    int n = (parametres.taille == 1) ? 10 : 15;
    int x, y;
    setcolor(WHITE);

    setfillstyle(SOLID_FILL, WHITE);
    bar(n*var + var + 2,0,LARG,650);

    setcolor(DARKGRAY);

    outtextxy(n*var + var +var + 1, 425, texte);


}

int compterBateauxCoules(const parametres &parametres, const tableauBateaux &tableauBateaux)
{
    int nbBateauxTotal = (parametres.taille == 1) ? 7 : 9;
    int coules = 0;
    for (int i = 0; i < nbBateauxTotal; ++i)
    {
        if (tableauBateaux[i].nbCasesTouchees == tableauBateaux[i].taille)
        {
            coules++;
        }
    }
    return coules;
}

void tipsp(const char* texte, const parametres &parametres) { // pour le placement des bateaux
    int var = (parametres.taille == 1) ? 40 : 30;
    int n = (parametres.taille == 1) ? 10 : 15;
    setcolor(WHITE);

    setfillstyle(SOLID_FILL, WHITE);
    bar(n*var + var + 2,400,LARG,450);

    setcolor(DARKGRAY);

    outtextxy(n*var + var +var + 1, 425, texte);


}

bool peutPlacer(const parametres &parametres, const grille &grillejoueur, int id, int jd, int ia, int ja)
{
    bool placer = true;
    if (id != ia && jd != ja)
        placer = false; // Placement doit être en ligne ou colonne

    int imin = min(id, ia);
    int imax = max(id, ia);
    int jmin = min(jd, ja);
    int jmax = max(jd, ja);
    int ni;
    int nj;
    int n = (parametres.taille == 1) ? 10 : 15;

    directions direc;
    direc[0][0] = -1;
    direc[0][1] = 0;
    direc[1][0] = 1;
    direc[1][1] = 0;
    direc[2][0] = 0;
    direc[2][1] = -1;
    direc[3][0] = 0;
    direc[3][1] = 1;
    if(placer)
    {
        for (int i = imin; i <= imax; ++i)
        {
            for (int j = jmin; j <= jmax; ++j)
            {
                // verifie si la case elle-même est un bateau
                if (grillejoueur[i][j].estBateau)
                    placer = false;

                if(placer)
                {
                    // verifie les 4 cases adjacentes (haut, bas, gauche, droite)
                    for (int d = 0; d < 4; ++d)
                    {
                        ni = i + direc[d][0];
                        nj = j + direc[d][1];

                        if (ni >= 0 && ni < n &&
                            nj >= 0 && nj < n)
                        {
                            if (grillejoueur[ni][nj].estBateau)
                                return false;
                        }
                    }
                }
            }
        }
    }

    return placer; // Aucun conflit détecté donc peut placer
}

void casesDeviennentBateau(grille &grilleJoueur, int idepart, int jdepart, int iarrivee, int jarrivee, int idBateau)
{
    if(jdepart == jarrivee)//cas 1 et 2 où jdepart == jarrivee
    {
        if(idepart > iarrivee)
        {
            while(idepart >= iarrivee)
            {
                grilleJoueur[idepart][jdepart].estBateau = 1;
                grilleJoueur[idepart][jdepart].id = idBateau;
                idepart--;
            }
        }
        else
        {
            while(idepart <= iarrivee)
            {
                grilleJoueur[idepart][jdepart].estBateau = 1;
                grilleJoueur[idepart][jdepart].id = idBateau;
                idepart++;
            }
        }
    }
    else //cas 3 et 4 où idepart == iarrivee
    {
        if(jdepart > jarrivee)
        {
            while(jdepart >= jarrivee)
            {
                grilleJoueur[idepart][jdepart].estBateau = 1;
                grilleJoueur[idepart][jdepart].id = idBateau;
                jdepart--;
            }
        }
        else
        {
            while(jdepart <= jarrivee)
            {
                grilleJoueur[idepart][jdepart].estBateau = 1;
                grilleJoueur[idepart][jdepart].id = idBateau;
                jdepart++;
            }
        }
    }
}

void afficherBateaux(const parametres &parametres, const grille &grilleJoueur)
{
    int var = (parametres.taille == 1) ? 40 : 30;
    int n = (parametres.taille == 1) ? 10 : 15;
    int px = var, py = var;
    setcolor(BLUE);
    setfillstyle(SOLID_FILL, BLUE);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if(grilleJoueur[i][j].estBateau ==1)
                bar(px, py, px + var, py + var);
            px += var;
        }
        px = var;
        py += var;
    }
}

void retireBateau(const parametres &parametres, grille &grilleJoueur, int idBateau)
{
    int n = (parametres.taille == 1) ? 10 : 15;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if(grilleJoueur[i][j].id == idBateau)
            {
                grilleJoueur[i][j].id = -1;
                grilleJoueur[i][j].estBateau = 0;
            }
        }
    }
}

void tipsBateau(const parametres &parametres, const tableauBateaux &tableauBateauxJoueur, int h, bool joueur1Actif)//Pour l'affichage des bateau à placer
{
    int nbBateaux  = (parametres.taille == 1) ? 7 : 9;
    char texte[50]; sprintf(texte, "Taille du bateau à placer :  %d ",tableauBateauxJoueur[h].taille);
    char texte2[50]; sprintf(texte2, "Nombre de bateaux à placer :  %d ",nbBateaux - h);
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    bar(LARG-500,0,LARG,300);
    bar(0,0,LARG,50);
    setcolor(DARKGRAY);
    if(joueur1Actif)
        outtextxy(LARG-500,250,"Placez vos bateaux Joueur 1");
    else
        outtextxy(LARG-500,250,"Placez vos bateaux Joueur 2");

    outtextxy(LARG-500,50,"Les bateaux vous sont donnés par ordre croissant de taille");
    outtextxy(LARG-500,100,texte);
    outtextxy(LARG-500,150,texte2);
}

void placementBateaux(const parametres &parametres, grille &grilleJoueur,const tableauBateaux &tableauBateauxJoueur, bool joueur1Actif)
{
    int x, y, i, j, ic, jc;
    int nbBateaux  = (parametres.taille == 1) ? 7 : 9;
    int var = (parametres.taille == 1) ? 40 : 30;
    int n = (parametres.taille == 1) ? 10 : 15;
    bool confirmer;
    int h = 0;
    bool cas1, cas2, cas3, cas4;

    int x_quitter = LARG - 100;
    int y_quitter = HAUT - 80;

    int x_regles = LARG - 200;
    int y_regles = HAUT - 500;


    setcolor(DARKGRAY);
    setbkcolor(WHITE);
    tipsp("Cliquez sur une case pour choisir l'emplacement d'une extrémité du bateau",parametres);

    while(h<nbBateaux)
    {
        confirmer = true;
        bouton("Règles", WHITE, DARKGRAY, x_regles, y_regles, 150, 50);
        bouton("Quitter", WHITE, DARKGRAY, x_quitter, y_quitter, 70, 25);
        grilleJoueur1(parametres,grilleJoueur,tableauBateauxJoueur);
        tipsBateau(parametres,tableauBateauxJoueur,h,joueur1Actif);
        afficherBateaux(parametres,grilleJoueur);
        contourGrilleJoueur1(parametres,grilleJoueur);
        if(h >= 1)
            bouton("Annuler dernier placement", WHITE, DARKGRAY, x_regles - 390, y_regles,250,50);

        getmouse(x, y);
        if (x > x_regles && x < x_regles + 150 && y > y_regles && y < y_regles + 50) //"Règles" est cliqué cliquées
        {
            closegraph();
            afficherRegle();
            opengraphsize(LARG, HAUT);
            bar(0, 0, LARG, HAUT);
            confirmer = false;
        }
        if(x >  x_regles - 390 &&  x < x_regles -140 && y > y_regles && y < y_regles + 50) //"Annuler" est cliqué
        {
            h--;
            retireBateau(parametres,grilleJoueur,h);
            tipsp("Dernier placement annulé",parametres);
            confirmer = false;
        }
        if((x >= n*var + var || x <= var) || (y >= n*var + var || y <= var))//si le clic sort de la grille
            confirmer = false;

        if(x > x_quitter && x < x_quitter + 70 && y > y_quitter && y < y_quitter + 25)
        {
            closegraph();
            exit(0);
        }

        else if(confirmer) // si on est bien dans la grille
        {
            i = (y - var) / var;
            j = (x - var) / var;
            if(grilleJoueur[i][j].estBateau == 1)
            {
                tipsp("Erreur : cette case est déjà un bateau",parametres);
            }
            else if(!peutPlacer(parametres,grilleJoueur,i,j,i,j))
            {
                tipsp("Erreur : Impossible de placer un bateau adjacent à un autre",parametres);
            }
            else if (confirmer)
            {
                cas1 = false; cas2 = false; cas3 = false; cas4 = false;
                tipsp("Cliquez sur une des cases bleues pour choisir l'autre extrémité", parametres);

                setcolor(LIGHTBLUE);
                setfillstyle(SOLID_FILL,LIGHTBLUE);

                if((i + tableauBateauxJoueur[h].taille-1)< n && peutPlacer(parametres,grilleJoueur,i,j,i+tableauBateauxJoueur[h].taille-1,j))
                {
                    bar(var + j * var + 1, var + (i+tableauBateauxJoueur[h].taille-1) * var + 1, var + (j + 1) * var, var + (i+tableauBateauxJoueur[h].taille) * var);
                    cas1 = true;
                }
                if((i - tableauBateauxJoueur[h].taille+1)>= 0 && peutPlacer(parametres,grilleJoueur,i,j,i-tableauBateauxJoueur[h].taille+1,j))
                {
                    bar(var + j * var + 1, var + (i-tableauBateauxJoueur[h].taille+1) * var + 1, var + (j + 1) * var, var + (i-tableauBateauxJoueur[h].taille+2) * var);
                    cas2 = true;
                }
                if((j + tableauBateauxJoueur[h].taille-1)< n && peutPlacer(parametres,grilleJoueur,i,j,i,j+tableauBateauxJoueur[h].taille-1))
                {
                    bar(var + (j+tableauBateauxJoueur[h].taille-1) * var, var + i * var, var + (j+tableauBateauxJoueur[h].taille) * var, var + (i + 1) * var);
                    cas3 = true;
                }
                if((j - tableauBateauxJoueur[h].taille+1)>= 0 && peutPlacer(parametres,grilleJoueur,i,j,i,j-tableauBateauxJoueur[h].taille+1))
                {
                    bar(var + (j-tableauBateauxJoueur[h].taille+1) * var, var + i * var, var + (j-tableauBateauxJoueur[h].taille+2) * var, var + (i + 1) * var);
                    cas4 = true;
                }
                contourGrilleJoueur1(parametres,grilleJoueur);
                confirmer = false;
                if(!cas1 && !cas2 && !cas3 && !cas4)
                {
                    tipsp("Impossible de placer le bateau à cet endroit",parametres);
                }
                else
                {
                    setcolor(BLUE);
                    setfillstyle(SOLID_FILL,BLUE);
                    bar(var + j * var, var + i * var, var + (j + 1) * var, var + (i + 1) * var);
                    while(!confirmer)
                    {
                        getmouse(x, y);
                        if((x > n*var + var || x < var) || (y > n*var + var || y < var)) continue;

                        ic = (y - var) / var;
                        jc = (x - var) / var;
                        if(((ic == (i+tableauBateauxJoueur[h].taille-1)) && (jc == j) && cas1) ||
                           ((ic == (i-tableauBateauxJoueur[h].taille+1)) && (jc == j) && cas2) ||
                           ((ic == i) && (jc == (j+tableauBateauxJoueur[h].taille-1)) && cas3) ||
                           ((ic == i) && (jc == (j-tableauBateauxJoueur[h].taille+1)) && cas4))
                        {
                            casesDeviennentBateau(grilleJoueur,i,j,ic,jc,h);
                            confirmer = true;
                            h++;
                            tipsp("Cliquez sur une des cases pour placer le prochain bateau",parametres);
                        }
                        else
                        {
                            tipsp("Erreur : Sélectionnez une des cases bleues",parametres);
                        }
                    }
                }
            }
        }
    }
    tipsp("Tous les bateaux ont été placés",parametres);
}



void confirmation(bool &confirm) {
    int x, y;
    bool ok = false;
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    bar(600, 400, LARG, 500);
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






void tir(grille &grille, const parametres &parametres, int &succes, bool joueur1, tableauBateaux &tableauBateauxJoueur,int x, int y) {
    succes = 0;
    int var = (parametres.taille == 1) ? 40 : 30;
    int n = (parametres.taille == 1) ? 10 : 15;
    int j, i, jc, ic;
    bool confirm = true;

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
        tips("Erreur : cette case a déjà été ciblée.",parametres);
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





void bombe(parametres &parametres, grille &grille, gadgets &gadgetJoueur, int &succes, bool joueur1, tableauBateaux &tableauBateauxJoueur) {
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
        tips("Erreur : cette case a déjà été ciblée.",parametres);
        succes = -1;
        return;
    }

    // Affichage de la zone de bombe en BLEU
    setcolor(BLUE);
    setfillstyle(SOLID_FILL, BLUE);
    bar(var + jc * var + 1, var + ic * var + 1, var + (jc + 1) * var, var + (ic + 1) * var);

    setcolor(LIGHTBLUE);
    setfillstyle(SOLID_FILL, LIGHTBLUE);

    // Droite
    if (j + 1 < n)
        bar(var + (jc + 1) * var + 1, var + ic * var + 1, var + (jc + 2) * var, var + (ic + 1) * var);

    // Gauche
    if (j - 1 >= 0)
        bar(var + (jc - 1) * var + 1, var + ic * var + 1, var + jc * var, var + (ic + 1) * var);

    // Haut
    if (i - 1 >= 0)
        bar(var + jc * var + 1, var + (ic - 1) * var + 1, var + (jc + 1) * var, var + ic * var);

    // Bas
    if (i + 1 < n)
        bar(var + jc * var + 1, var + (ic + 1) * var + 1, var + (jc + 1) * var, var + (ic + 2) * var);

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


void radar(parametres &parametres, grille &grille, gadgets &gadgetJoueur, int &succes, bool joueur1, tableauBateaux &tableauBateauxJoueur) {
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
        tips("Erreur : cette case a déjà été ciblée, radar inutilisable ici.",parametres);
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
        gadgetJoueur.scannerUtilisable = 0;
        succes = 1;
    } else {
        setcolor(WHITE);
        bar(var + jc * var, var + ic * var, var + (jc + 1) * var, var + (ic + 1) * var);
        succes = 0;
    }
}

void affichageJeu(const parametres &parametres, grille &joueur1, grille &joueur2, gadgets &gadgetJoueur1, gadgets &gadgetJoueur2, tableauBateaux &tableauBateauxJoueur1, tableauBateaux &tableauBateauxJoueur2, bool joueur1Actif) {
    int var = (parametres.taille == 1) ? 40 : 30;
    int n = (parametres.taille == 1) ? 10 : 15;
    int nbBateauxTotal = (parametres.taille == 1) ? 7 : 9;
    int bateauxCoulesJ1 = compterBateauxCoules(parametres, tableauBateauxJoueur1);
    int bateauxCoulesJ2 = compterBateauxCoules(parametres, tableauBateauxJoueur2);
    char statsJ1[50];
    sprintf(statsJ1, "Bateaux joueur 1 : %d/%d coulés", bateauxCoulesJ1, nbBateauxTotal);
    char statsJ2[50];
    sprintf(statsJ2, "Bateaux joueur 2 : %d/%d coulés", bateauxCoulesJ2, nbBateauxTotal);


    // Affichage des deux grilles et contours
    grilleJoueur1(parametres, joueur1, tableauBateauxJoueur1);
    contourGrilleJoueur1(parametres, joueur1);
    grilleJoueur2(parametres, joueur2, tableauBateauxJoueur2);
    contourGrilleJoueur2(parametres, joueur2);

    //Affichage du nombre de bateaux coulés
    outtextxy(LARG-590,200,statsJ1);
    outtextxy(LARG-590,600,statsJ2);

    // Affichage des gadgets uniquement pour le joueur actif
    if (joueur1Actif)
    {
        // Joueur 1 : on affiche ses gadgets
        if (gadgetJoueur1.scannerUtilisable == 1)
            bouton("Radar", WHITE, DARKGRAY, n * var + 2 * var, n * var + 8 * var, 100, 50);
        else {
            setfillstyle(SOLID_FILL, WHITE);
            bar(n * var + 2 * var, n * var + 8 * var, n * var + 2 * var + 102, n * var + 8 * var + 52);
        }

        if (gadgetJoueur1.bombeUtilisable == 1)
            bouton("Bombe", WHITE, DARKGRAY, n * var + 2 * var, n * var + 10 * var, 100, 50);
        else {
            setfillstyle(SOLID_FILL, WHITE);
            bar(n * var + 2 * var, n * var + 10 * var, n * var + 2 * var + 102, n * var + 10 * var + 52);
        }
    }
    else
    {
        // Joueur 2 : on affiche ses gadgets
        if (gadgetJoueur2.scannerUtilisable == 1)
            bouton("Radar", WHITE, DARKGRAY, n * var + 2 * var, n * var + 8 * var, 100, 50);
        else {
            setfillstyle(SOLID_FILL, WHITE);
            bar(n * var + 2 * var, n * var + 8 * var, n * var + 2 * var + 102, n * var + 8 * var + 52);
        }

        if (gadgetJoueur2.bombeUtilisable == 1)
            bouton("Bombe", WHITE, DARKGRAY, n * var + 2 * var, n * var + 10 * var, 100, 50);
        else {
            setfillstyle(SOLID_FILL, WHITE);
            bar(n * var + 2 * var, n * var + 10 * var, n * var + 2 * var + 102, n * var + 10 * var + 52);
        }
    }
}



bool checkWinJoueur1(const parametres &parametres, const tableauBateaux &tableauBateauxJoueur1) {
    int nbBateaux = (parametres.taille == 1) ? 7 : 9;

    int i = 0;

    bool rep = true;

    while(i < nbBateaux && rep == true) {
        if (tableauBateauxJoueur1[i].nbCasesTouchees != tableauBateauxJoueur1[i].taille) {
            rep = false;
        }
        i++;
    }
    return rep;
}


bool checkWinJoueur2(const parametres &parametres, const tableauBateaux &tableauBateauxJoueur2) {
    int nbBateaux = (parametres.taille == 1) ? 7 : 9;

    int i = 0;

    bool rep = true;

    while(i < nbBateaux && rep == true) {
        if (tableauBateauxJoueur2[i].nbCasesTouchees != tableauBateauxJoueur2[i].taille) {
            rep = false;
        }
        i++;
    }
    return rep;
}


void jeu(parametres &parametre, grille &joueur1, grille &joueur2, gadgets &gadgetJoueur1, gadgets &gadgetJoueur2, tableauBateaux &tableauBateauxJoueur1, tableauBateaux &tableauBateauxJoueur2) {
    int x, y;
    int var = (parametre.taille == 1) ? 40 : 30;
    int n = (parametre.taille == 1) ? 10 : 15;
    int succes = 0;

    int x_quitter = LARG - 100;
    int y_quitter = HAUT - 80;
    int x_regles = LARG - 140;
    int y_regles = HAUT - 500;

    while (!checkWinJoueur1(parametre, tableauBateauxJoueur1) && !checkWinJoueur2(parametre, tableauBateauxJoueur2)) {
        // ---------------------
        // Tour du joueur 1
        // ---------------------
        tips("Tour du Joueur 1",parametre);
        succes = 0;
        while (succes != 1) {
            affichageJeu(parametre, joueur1, joueur2, gadgetJoueur1, gadgetJoueur2, tableauBateauxJoueur1, tableauBateauxJoueur2, true);
            bouton("Règles", WHITE, DARKGRAY, x_regles, y_regles, 100, 50);
            bouton("Quitter", WHITE, DARKGRAY, x_quitter, y_quitter, 70, 25);
            getmouse(x, y);

            if(x > x_quitter && x < x_quitter + 70 && y > y_quitter && y < y_quitter + 25) {
                closegraph(); exit(0);
            }
            if (x > x_regles && x < x_regles + 100 && y > y_regles && y < y_regles + 50) {
                closegraph(); afficherRegle(); opengraphsize(LARG, HAUT); bar(0,0,LARG,HAUT); continue;
            }

            if ((x > n * var + 2 * var && x < n * var + 2 * var + 100) && (y > n * var + 8 * var && y < n * var + 8 * var + 50) && gadgetJoueur1.scannerUtilisable == 1) {
                bouton("Radar", WHITE, GREEN, n * var + 2 * var, n * var + 8 * var, 100, 50);
                radar(parametre, joueur2, gadgetJoueur1, succes, false, tableauBateauxJoueur2);
                if(succes == 1) gadgetJoueur1.scannerUtilisable = 0;
            }
            else if ((x > n * var + 2 * var && x < n * var + 2 * var + 100) && (y > n * var + 10 * var && y < n * var + 10 * var + 50) && gadgetJoueur1.bombeUtilisable == 1) {
                bouton("Bombe", WHITE, GREEN, n * var + 2 * var, n * var + 10 * var, 100, 50);
                bombe(parametre, joueur2, gadgetJoueur1, succes, false, tableauBateauxJoueur2);
                if(succes == 1) gadgetJoueur1.bombeUtilisable = 0;
            }
            else {
                tir(joueur2, parametre, succes, false, tableauBateauxJoueur2, x, y);
                if (parametre.tirSimple == 0 && succes == 1) {
                    for (int i = 0; i < 2; i++) {
                        if (checkWinJoueur2(parametre, tableauBateauxJoueur2)) break;
                        int tirSuppSucces = 0;
                        while(tirSuppSucces != 1) {
                            char tipText[100]; sprintf(tipText, "Tour du Joueur 1 - Tir %d/3", i + 2); tips(tipText, parametre);
                            affichageJeu(parametre, joueur1, joueur2, gadgetJoueur1, gadgetJoueur2, tableauBateauxJoueur1, tableauBateauxJoueur2, true);
                            bouton("Règles", WHITE, DARKGRAY, x_regles, y_regles, 100, 50);
                            getmouse(x, y);
                            tir(joueur2, parametre, tirSuppSucces, false, tableauBateauxJoueur2, x, y);
                        }
                    }
                }
            }
        }

        if (checkWinJoueur2(parametre, tableauBateauxJoueur2)) break;

        // ---------------------
        // Tour du joueur 2
        // ---------------------
        tips("Tour du Joueur 2",parametre);
        succes = 0;
        while (succes != 1) {
            affichageJeu(parametre, joueur1, joueur2, gadgetJoueur1, gadgetJoueur2, tableauBateauxJoueur1, tableauBateauxJoueur2, false);
            bouton("Règles", WHITE, DARKGRAY, x_regles, y_regles, 100, 50);
            getmouse(x, y);

            if(x > x_quitter && x < x_quitter + 100 && y > y_quitter && y < y_quitter + 50) {
                closegraph(); exit(0);
            }
            if (x > x_regles && x < x_regles + 100 && y > y_regles && y < y_regles + 50) {
                closegraph(); afficherRegle(); opengraphsize(LARG, HAUT); bar(0,0,LARG,HAUT); continue;
            }

            if ((x > n * var + 2 * var && x < n * var + 2 * var + 100) && (y > n * var + 8 * var && y < n * var + 8 * var + 50) && gadgetJoueur2.scannerUtilisable == 1) {
                bouton("Radar", WHITE, GREEN, n * var + 2 * var, n * var + 8 * var, 100, 50);
                radar(parametre, joueur1, gadgetJoueur2, succes, true, tableauBateauxJoueur1);
                if(succes == 1) gadgetJoueur2.scannerUtilisable = 0;
            }
            else if ((x > n * var + 2 * var && x < n * var + 2 * var + 100) && (y > n * var + 10 * var && y < n * var + 10 * var + 50) && gadgetJoueur2.bombeUtilisable == 1) {
                bouton("Bombe", WHITE, GREEN, n * var + 2 * var, n * var + 10 * var, 100, 50);
                bombe(parametre, joueur1, gadgetJoueur2, succes, true, tableauBateauxJoueur1);
                if(succes == 1) gadgetJoueur2.bombeUtilisable = 0;
            }
            else {
                tir(joueur1, parametre, succes, true, tableauBateauxJoueur1, x, y);
                if (parametre.tirSimple == 0 && succes == 1) {
                    for (int i = 0; i < 2; i++) {
                        if (checkWinJoueur1(parametre, tableauBateauxJoueur1)) break;
                        int tirSuppSucces = 0;
                        while(tirSuppSucces != 1) {
                            char tipText[100]; sprintf(tipText, "Tour du Joueur 2 - Tir %d/3", i + 2); tips(tipText, parametre);
                            affichageJeu(parametre, joueur1, joueur2, gadgetJoueur1, gadgetJoueur2, tableauBateauxJoueur1, tableauBateauxJoueur2, false);
                            bouton("Règles", WHITE, DARKGRAY, x_regles, y_regles, 100, 50);
                            getmouse(x, y);
                            tir(joueur1, parametre, tirSuppSucces, true, tableauBateauxJoueur1, x, y);
                        }
                    }
                }
            }
        }
}
    }


// Tir aléatoire (ne cible jamais deux fois la même case)
void tirAleatoire(grille &grille, const parametres &parametres, int &succes, tableauBateaux &tableauBateauxJoueur) {
    int n = (parametres.taille == 1) ? 10 : 15;
    succes = 0;

    while (succes != 1) {
        int i = rand() % n;
        int j = rand() % n;

        if (grille[i][j].estTouchee == 0) {
            grille[i][j].estTouchee = 1;
            if (grille[i][j].estBateau == 1) {
                tableauBateauxJoueur[grille[i][j].id].nbCasesTouchees++;
            }
            succes = 1;
        }
    }
}

bool peutPlacerAleatoirement(const grille &grille, int i, int j, int taille, bool horizontal, int n) {
    if (horizontal) {
        if (j + taille > n) return false;
        for (int k = -1; k <= taille; ++k) {
            for (int di = -1; di <= 1; ++di) {
                int ni = i + di;
                int nj = j + k;
                if (ni >= 0 && ni < n && nj >= 0 && nj < n) {
                    if (grille[ni][nj].estBateau) return false;
                }
            }
        }
    } else {
        if (i + taille > n) return false;
        for (int k = -1; k <= taille; ++k) {
            for (int dj = -1; dj <= 1; ++dj) {
                int ni = i + k;
                int nj = j + dj;
                if (ni >= 0 && ni < n && nj >= 0 && nj < n) {
                    if (grille[ni][nj].estBateau) return false;
                }
            }
        }
    }
    return true;
}




void placerBateauxAleatoirement(grille &grille, tableauBateaux &tableau, const parametres &parametres) {
    int n = (parametres.taille == 1) ? 10 : 15;

    // Tri décroissant des bateaux par taille (facultatif mais conseillé)
    sort(tableau.begin(), tableau.end(), [](const bateau &a, const bateau &b) {
        return a.taille > b.taille;
    });

    for (int b = 0; b < tableau.size(); ++b) {
        if (tableau[b].taille <= 0) break;

        bool place = false;
        int essais = 0;
        int maxEssais = 1000;

        while (!place && essais < maxEssais) {
            essais++;
            int i = rand() % n;
            int j = rand() % n;
            bool horizontal = rand() % 2;

            if (peutPlacerAleatoirement(grille, i, j, tableau[b].taille, horizontal, n)) {
                for (int k = 0; k < tableau[b].taille; ++k) {
                    int x = i + (horizontal ? 0 : k);
                    int y = j + (horizontal ? k : 0);
                    grille[x][y].estBateau = 1;
                    grille[x][y].id = b;
                }
                place = true;
            }
        }

        if (!place) {
            exit(1); // Stoppe le programme proprement
        }
    }
}


// Mode de jeu contre ordinateur
void jeuContreOrdinateur(parametres &parametre, grille &joueur1, grille &joueur2, gadgets &gadgetJoueur1, gadgets &gadgetJoueur2,tableauBateaux &tableauBateauxJoueur1, tableauBateaux &tableauBateauxJoueur2){
    int x, y;
    int var = (parametre.taille == 1) ? 40 : 30;
    int n = (parametre.taille == 1) ? 10 : 15;
    int succes = 0;

    int x_quitter = LARG - 100;
    int y_quitter = HAUT - 80;
    int x_regles = LARG - 140;
    int y_regles = HAUT - 500;

    while (!checkWinJoueur1(parametre, tableauBateauxJoueur1) &&
           !checkWinJoueur2(parametre, tableauBateauxJoueur2)) {

        // Joueur 1 (humain)
        tips("Votre tour",parametre);
        succes = 0;
        while (succes != 1) {
            affichageJeu(parametre, joueur1, joueur2, gadgetJoueur1, gadgetJoueur2, tableauBateauxJoueur1, tableauBateauxJoueur2, true);
            bouton("Règles", WHITE, DARKGRAY, x_regles, y_regles, 100, 50);
            bouton("Quitter", WHITE, DARKGRAY, x_quitter, y_quitter, 70, 25);
            getmouse(x, y);

            if(x > x_quitter && x < x_quitter + 70 && y > y_quitter && y < y_quitter + 25) {
                closegraph();
                exit(0);
            }
            if (x > x_regles && x < x_regles + 100 && y > y_regles && y < y_regles + 50) {
                closegraph();
                afficherRegle();
                opengraphsize(LARG, HAUT);
                bar(0,0,LARG,HAUT);
                continue;
            }

            if ((x > n * var + 2 * var && x < n * var + 2 * var + 100) && (y > n * var + 8 * var && y < n * var + 8 * var + 50) && gadgetJoueur1.scannerUtilisable == 1) {
                bouton("Radar", WHITE, GREEN, n * var + 2 * var, n * var + 8 * var, 100, 50);
                radar(parametre, joueur2, gadgetJoueur1, succes, false, tableauBateauxJoueur2);
                    affichageJeu(parametre, joueur1, joueur2, gadgetJoueur1, gadgetJoueur2, tableauBateauxJoueur1, tableauBateauxJoueur2, true);

            }
            else if ((x > n * var + 2 * var && x < n * var + 2 * var + 100) && (y > n * var + 10 * var && y < n * var + 10 * var + 50) && gadgetJoueur1.bombeUtilisable == 1) {
                bouton("Bombe", WHITE, GREEN, n * var + 2 * var, n * var + 10 * var, 100, 50);
                bombe(parametre, joueur2, gadgetJoueur1, succes, false, tableauBateauxJoueur2);
                affichageJeu(parametre, joueur1, joueur2, gadgetJoueur1, gadgetJoueur2, tableauBateauxJoueur1, tableauBateauxJoueur2, true);

            }
            else {
                tir(joueur2, parametre, succes, false, tableauBateauxJoueur2, x, y);
                if (parametre.tirSimple == 0 && succes == 1) {
                    for (int i = 0; i < 2; i++) {
                        if (checkWinJoueur2(parametre, tableauBateauxJoueur2)) break;
                        int tirSuppSucces = 0;
                        while(tirSuppSucces != 1) {
                            char tipText[100]; sprintf(tipText, "Votre tour - Tir %d/3", i + 2); tips(tipText, parametre);
                            affichageJeu(parametre, joueur1, joueur2, gadgetJoueur1, gadgetJoueur2, tableauBateauxJoueur1, tableauBateauxJoueur2, true);
                            bouton("Règles", WHITE, DARKGRAY, x_regles, y_regles, 100, 50);
                            getmouse(x, y);
                            tir(joueur2, parametre, tirSuppSucces, false, tableauBateauxJoueur2, x, y);
                            affichageJeu(parametre, joueur1, joueur2, gadgetJoueur1, gadgetJoueur2, tableauBateauxJoueur1, tableauBateauxJoueur2, true);

                        }
                    }
                }
            }
        }

        if (checkWinJoueur2(parametre, tableauBateauxJoueur2)) break;

        // Ordinateur
        affichageJeu(parametre, joueur1, joueur2, gadgetJoueur1, gadgetJoueur2, tableauBateauxJoueur1, tableauBateauxJoueur2, true);
        tips("Tour de l'ordinateur...",parametre);
        delay(1000);
        succes = 0;

        if (gadgetJoueur2.scannerUtilisable && rand() % 4 == 0) {
            int i = rand() % n, j = rand() % n;
            for (int di = -1; di <= 1; ++di) for (int dj = -1; dj <= 1; ++dj) {
                int ii = i + di, jj = j + dj; if (ii >= 0 && ii < n && jj >= 0 && jj < n) joueur1[ii][jj].estRevelee = 1;
            }
            gadgetJoueur2.scannerUtilisable = 0;
            succes = 1;
        }
        else if (gadgetJoueur2.bombeUtilisable && rand() % 4 == 0) {
            int i = rand() % n, j = rand() % n;
            if (joueur1[i][j].estTouchee == 0) {
                joueur1[i][j].estTouchee = 1; if (joueur1[i][j].estBateau) tableauBateauxJoueur1[joueur1[i][j].id].nbCasesTouchees++;
                if (i > 0) { joueur1[i - 1][j].estTouchee = 1; if (joueur1[i - 1][j].estBateau) tableauBateauxJoueur1[joueur1[i - 1][j].id].nbCasesTouchees++; }
                if (i < n - 1) { joueur1[i + 1][j].estTouchee = 1; if (joueur1[i + 1][j].estBateau) tableauBateauxJoueur1[joueur1[i + 1][j].id].nbCasesTouchees++; }
                if (j > 0) { joueur1[i][j - 1].estTouchee = 1; if (joueur1[i][j - 1].estBateau) tableauBateauxJoueur1[joueur1[i][j - 1].id].nbCasesTouchees++; }
                if (j < n - 1) { joueur1[i][j + 1].estTouchee = 1; if (joueur1[i][j + 1].estBateau) tableauBateauxJoueur1[joueur1[i][j + 1].id].nbCasesTouchees++; }
                gadgetJoueur2.bombeUtilisable = 0;
                succes = 1;
            }
        }

        if (succes != 1) {
            int nbTirs = (parametre.tirSimple == 1) ? 1 : 3;
            for (int i = 0; i < nbTirs; ++i) {
                if (checkWinJoueur1(parametre, tableauBateauxJoueur1)) break;
                int tirAISucces = 0;
                tirAleatoire(joueur1, parametre, tirAISucces, tableauBateauxJoueur1);
                delay(500);
            }
        }
        affichageJeu(parametre, joueur1, joueur2, gadgetJoueur1, gadgetJoueur2, tableauBateauxJoueur1, tableauBateauxJoueur2, true);
    }
}

void ecranFin(const parametres &parametres, bool &rejouerPartie, bool victoireJoueur1, const tableauBateaux &tableauBateauxJoueur1, const tableauBateaux &tableauBateauxJoueur2) {
    // Nettoyer l'écran
    setfillstyle(SOLID_FILL, WHITE);
    bar(0, 0, LARG, HAUT);
    setbkcolor(WHITE);
    setcolor(DARKGRAY);

    // Afficher le message de victoire
    if (victoireJoueur1) {
        outtextxy(LARG / 2 - textwidth("Victoire du joueur 1") / 2, HAUT / 2 - 150, "Victoire du joueur 1");
    } else {
        outtextxy(LARG / 2 - textwidth("Victoire du joueur 2 ou de l'ordinateur") / 2, HAUT / 2 - 150, "Victoire du joueur 2 ou de l'ordinateur");
    }

    // Afficher les statistiques des bateaux
    int nbBateauxTotal = (parametres.taille == 1) ? 7 : 9;
    int bateauxCoulesJ1 = compterBateauxCoules(parametres, tableauBateauxJoueur1);
    int bateauxCoulesJ2 = compterBateauxCoules(parametres, tableauBateauxJoueur2);

    char statsJ1[50];
    sprintf(statsJ1, "Bateaux joueur 1 : %d/%d coules", bateauxCoulesJ1, nbBateauxTotal);
    char statsJ2[50];
    sprintf(statsJ2, "Bateaux joueur 2 : %d/%d coules", bateauxCoulesJ2, nbBateauxTotal);

    outtextxy(LARG / 2 - textwidth(statsJ1) / 2, HAUT / 2 - 100, statsJ1);
    outtextxy(LARG / 2 - textwidth(statsJ2) / 2, HAUT / 2 - 70, statsJ2);

    // Dessiner les boutons
    int xBoutonRejouer = LARG / 2 - 150;
    int xBoutonQuitter = LARG / 2 + 50;
    int yBoutons = HAUT / 2;
    int largeurBouton = 100;
    int hauteurBouton = 50;

    bouton("Rejouer", WHITE, DARKGRAY, xBoutonRejouer, yBoutons, largeurBouton, hauteurBouton);
    bouton("Quitter", WHITE, DARKGRAY, xBoutonQuitter, yBoutons, largeurBouton, hauteurBouton);

    // Attendre le clic
    int x, y;
    bool clicValide = false;
    while (!clicValide) {
            getmouse(x, y);
            // Clic sur le bouton Rejouer
            if (x >= xBoutonRejouer && x <= xBoutonRejouer + largeurBouton && y >= yBoutons && y <= yBoutons + hauteurBouton) {
                rejouerPartie = true;
                clicValide = true;
            }

            // Clic sur le bouton Quitter
            if (x >= xBoutonQuitter && x <= xBoutonQuitter + largeurBouton && y >= yBoutons && y <= yBoutons + hauteurBouton) {
                rejouerPartie = false;
                clicValide = true;
            }

        delay(100);
    }
}


int main() {
    srand(time(NULL));
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

    bool rejouerPartie;

    do {
        // Initialisation / Réinitialisation des variables de jeu
        parametres parametres;
        parametres.jeuContreOrdi = 1;
        parametres.taille = 1;
        parametres.tirSimple = 1;

        grille joueur1 = {};
        grille joueur2 = {};

        gadgets gadgetJoueur1 = {1, 1};
        gadgets gadgetJoueur2 = {1, 1};

        tableauBateaux tableauBateauxJoueur1 = {};
        tableauBateaux tableauBateauxJoueur2 = {};

        bool jouer = false;
        menu(parametres, jouer);

        if (!jouer) {
            rejouerPartie = false;
            continue; // Sort de cette itération et vérifie la condition de la boucle
        }

        // Configuration des bateaux en fonction de la taille de la grille
        if(parametres.taille == 1) {
            tableauBateauxJoueur1[0].taille = 2; tableauBateauxJoueur2[0].taille = 2;
            tableauBateauxJoueur1[1].taille = 2; tableauBateauxJoueur2[1].taille = 2;
            tableauBateauxJoueur1[2].taille = 2; tableauBateauxJoueur2[2].taille = 2;
            tableauBateauxJoueur1[3].taille = 3; tableauBateauxJoueur2[3].taille = 3;
            tableauBateauxJoueur1[4].taille = 3; tableauBateauxJoueur2[4].taille = 3;
            tableauBateauxJoueur1[5].taille = 4; tableauBateauxJoueur2[5].taille = 4;
            tableauBateauxJoueur1[6].taille = 5; tableauBateauxJoueur2[6].taille = 5;
        } else {
            tableauBateauxJoueur1[0].taille = 2; tableauBateauxJoueur2[0].taille = 2;
            tableauBateauxJoueur1[1].taille = 2; tableauBateauxJoueur2[1].taille = 2;
            tableauBateauxJoueur1[2].taille = 2; tableauBateauxJoueur2[2].taille = 2;
            tableauBateauxJoueur1[3].taille = 3; tableauBateauxJoueur2[3].taille = 3;
            tableauBateauxJoueur1[4].taille = 3; tableauBateauxJoueur2[4].taille = 3;
            tableauBateauxJoueur1[5].taille = 3; tableauBateauxJoueur2[5].taille = 3;
            tableauBateauxJoueur1[6].taille = 4; tableauBateauxJoueur2[6].taille = 4;
            tableauBateauxJoueur1[7].taille = 4; tableauBateauxJoueur2[7].taille = 4;
            tableauBateauxJoueur1[8].taille = 5; tableauBateauxJoueur2[8].taille = 5;
        }

        opengraphsize(LARG, HAUT);
        setfillstyle(SOLID_FILL, WHITE);
        bar(0, 0, LARG, HAUT);

        placementBateaux(parametres, joueur1, tableauBateauxJoueur1,true);

        if (parametres.jeuContreOrdi) {
            placerBateauxAleatoirement(joueur2, tableauBateauxJoueur2, parametres);
        } else {
            tips("Au tour du Joueur 2. Le Joueur 1 ne regarde pas !", parametres);
            bouton("Continuer", WHITE, DARKGRAY, 600, 500, 150, 50);
            int x, y;
            bool continuer = false;
            while(!continuer) {

                    getmouse(x, y);
                    if(x > 600 && x < 750 && y > 500 && y < 550) continuer = true;

                delay(100);
            }
            bar(0,0,LARG,HAUT);
            placementBateaux(parametres, joueur2, tableauBateauxJoueur2,false);
        }

        // Lancement de la boucle de jeu principale
        if (parametres.jeuContreOrdi)
            jeuContreOrdinateur(parametres, joueur1, joueur2, gadgetJoueur1, gadgetJoueur2, tableauBateauxJoueur1, tableauBateauxJoueur2);
        else
            jeu(parametres, joueur1, joueur2, gadgetJoueur1, gadgetJoueur2, tableauBateauxJoueur1, tableauBateauxJoueur2);

        // --- Affichage de l'écran de fin ---
        bool victoireJ1 = checkWinJoueur2(parametres, tableauBateauxJoueur2);
        ecranFin(parametres, rejouerPartie, victoireJ1, tableauBateauxJoueur1, tableauBateauxJoueur2);

        closegraph();

    } while (rejouerPartie);

    return 0;
}







