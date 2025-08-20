# **Licence Maths/Informatique 1 - UHA - Projet de cours- Informatique appliquée avancée**
Le but de ce projet était, pour la première fois, de réaliser de manière professionneles les demandes d'un client en respectant les méthodes du entreprises ainsi que ses usages.

Dans le projet il était donc nécéssaire de réaliser un échange avec le client, de réaliser un `Cahier des Charges`, un `maquettage`, une `études des fonctions et des structures`, ainsi qu'un `compte-rendu final`.

> [!WARNING]
> Ceci étant un projet scolaire, le jeu en lui-même n'est pas parfait, il peut contenir des comportements inatendus. 


# **Consignes du projet**

## Demande du client
Création d’un jeu de bataille navale

## Contraintes
Utilisation du langage `C++` et de la bibliothèque graphique `winBGIm` uniquement.
				
## Fonctionnalités décidées avec le client
Le jeu se joue à 2 joueurs (Humain ou Ordinateur), 2 modes de jeu seront disponibles :
  * Humain contre Ordinateur
  * Humain contre Humain sur une même machine

### But du jeu : 
  * Chaque joueur possède 2 grilles et 7 bateaux de longueurs différentes (*). Leur objectif est de détruire les bateaux de l’adversaire.

  * Au début de la partie les deux joueurs placent leurs bateaux sur leur grille inférieure.

  * Un joueur est tiré au hasard et il engage le combat en cliquant sur sa grille supérieure. Un indicateur apparaîtra sur la grille supérieure pour savoir si le tir à touché un bateau adverse ou non. 

  * C’est ensuite l’autre joueur qui doit tirer. Les tirs reçus sont quant à eux indiqués sur la grille inférieure. La partie se continue ainsi jusqu’à ce qu’un des joueurs ait perdu tous ses bateaux.

### Gadgets : 
  * Les joueurs disposent chacun de deux gadgets à usage unique. Ces derniers sont utilisables à tout moment pendant leur tour d’attaque. 
  * Pour les utiliser il suffit de cliquer sur leur bouton attitré en bas à droite de la fenêtre de jeu, alors le prochain tir du joueur sera le gadget sélectionné. Après l’utilisation d’un gadget, c'est au tour du joueur suivant.

Voici les deux gadgets disponibles : 
  * Le scanner : Il révèle l’emplacement des bateaux ennemis dans un périmètre de 3x3 cases. 
  * La bombe : Inflige des dégâts à toutes les cases dans le périmètre d’une croix, formée par des lignes de 3 cases.

### Options : 
Le joueur peut changer certains paramètres de jeu avant de lancer une partie :
* Il a le choix entre deux grilles de jeu de dimensions différentes (10x10 ou 15x15). Le nombre de bateaux varie en fonction de la dimension choisie : 

  * Grille de 10x10
    * 1 Bateau de 5 cases 
    * 1 Bateau de 4 cases
    * 2 Bateaux de 3 cases
    * 3 Bateaux de 2 cases

  * Grille de 15x15
    * 1 Bateau de 5 cases 
    * 2 Bateau de 4 cases
    * 3 Bateaux de 3 cases
    * 3 Bateaux de 2 cases

* Il a le choix entre deux modes de tir (tri simple ou triple tir). Le mode triple tir permet aux joueurs de tenter trois tirs au lieu d’un lors de leur tour d’attaque.  


### Boîte textuelle : 
Pour donner des indications aux joueurs sur ce qu’il doivent faire, une boîte textuelle est positionnée à droite des grilles.
Elle envoie les messages suivants : 
* “Placez vos bateaux”
* “A vous de tirer”
* “Touché”
* “Coulé”
* “Tir raté”

### Menu / Écran de fin : 
* Le menu permet au joueur d’appliquer les modifications évoquées précédemment à la partie (mode de jeu / taille des grilles / mode de tir). Par défaut les modifications sont les suivantes : Jeu contre Ordinateur/ grilles 10x10 / Tir simple. Lorsque qu’une modification est sélectionnée, son bouton est de couleur verte. Il suffit ensuite de cliquer sur jouer pour lancer la partie.

* L’écran de fin annonce lequel des deux joueurs est le vainqueur et affiche le nombre de bateaux détruits par chaque joueur. Un bouton rejouer est disponible ainsi qu’un bouton pour quitter le jeu.

## Informations complémentaires
### Fonctionnement du mode Humain contre Humain : 
L’écran affichera tour à tour les grilles des joueurs en fonction de celui qui doit effectuer une action (par exemple placer ses bateaux ou encore tirer). Avant que la grille ne change sur celle d’un autre joueur, une fenêtre sera ouverte pour cacher les grilles et permettre au joueur suivant de regarder l’écran sans voir la grille de son adversaire. Il devra ensuite appuyer sur un bouton pour entamer son tour.

### Fonctionnement du mode Humain contre Ordinateur : 
L’écran affichera uniquement les grilles de l’humain. Quant aux placements des bateaux de l’Ordinateur, ils seront choisis aléatoirement parmi une liste prédéfinie de grilles. En ce qui concerne le fonctionnement de l’Ordinateur lors de ses attaques, il tirera aléatoirement sur la grille du joueur. Si il y a une touche, il retentera dans un rayon proche de la case touchée. Sinon, il retentera un tir aléatoire.

### Comment placer ses bateaux : 
Les humains devront placer leurs bateaux 1 à 1 aux début de la partie, ils seront proposés du plus petit au plus grand. D’un point de vue pratique, le premier clic posera l’avant du bateau, le joueur Humain devra ensuite cliquer sur une autre case de la grille qui définira l’orientation du bateau par rapport à la première case.


## Parties prenantes
Matthieu IMHOF </br>
Clément GOUGE




