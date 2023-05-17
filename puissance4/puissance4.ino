#include <Gamebuino-Meta.h>

/*
    Implémentation pour GAMEBUINO en META4 d'un puissance 4 complet.

    Les parametres du programme sont ajustables ci dessous (a part quelques problemes dus a la taille de l'écran,
    vous pouvez choisir librement la taille de la grille, entre autres!).

    Auteurs : Victor BARDIN, Laurent THILLOU, Felix laterrot

    Evolutions envisagées : rajouter plus de joueurs, tres peu de code serait changé

*/

const int COLONNES_GRILLE = 7; // Taille de la grille (7 colonnes x 6 lignes)
const int LIGNES_GRILLE = 6;

const int TAILLE_CELLULE = 8; // Taille d'une cellule de la grille
const int LARGEUR_ECRAN = gb.display.width();
const int HAUTEUR_ECRAN = gb.display.height();
const int DECALAGE_GRILLE_X = 0; // Décalage horizontal de la grille (modification déconseillée)
const int DECALAGE_GRILLE_Y = HAUTEUR_ECRAN * 0.25; // Décalage vertical de la grille (modification déconseillée)
const int LARGEUR_GRILLE = LIGNES_GRILLE * TAILLE_CELLULE; // Largeur de la grille
const int HAUTEUR_GRILLE = (HAUTEUR_ECRAN - DECALAGE_GRILLE_Y); // Hauteur de la grille
int grillePions[LIGNES_GRILLE][COLONNES_GRILLE]; //la version logique du tableau de jeu

int joueurEnCours = 1;
int scores[3]; // nuls / joueur 1 / joueur 2
boolean aJoue = false;

boolean partieTerminee = false;

//le curseur permet au joueurEnCours de choisir dans quelle colonne jouer
int absCurseur = 0; //abscisse du curseur
int indexCurseur = 0;


void setup() {
  gb.begin();
}

void loop() {
  while (!gb.update());
  gb.display.clear();
  dessinerGrille();
  dessinerPions();
  afficherScores();

  if (!partieTerminee) {
    //on joue un tour pour le joueurEnCours
    dessinerCurseur();
    positionnerCouleur(joueurEnCours);
    jouerUnTour();

    //on vérifie si la partie se termine
    if (verifierPuissance4(joueurEnCours) == joueurEnCours) {
      partieTerminee = true;
      scores[joueurEnCours]++;
    }
    if (grillePleine()) {
      partieTerminee = true;
      scores[0]++;
    }
    
    // puis on change de joueur
    if (aJoue && !partieTerminee) {
      changerDeJoueur();
      aJoue = false;
    }
  } else { //la partie est terminee
    gb.display.setCursor(1, 1);
    gb.display.setColor(Color::green);
    gb.display.println("Partie terminee : ");
    if (grillePleine()) {
      positionnerCouleur(0);
      gb.display.print("egalite! ");
    } else {
      positionnerCouleur(joueurEnCours);
      gb.display.print("J");
      gb.display.print(joueurEnCours);
      gb.display.print(" gagne! ");

    }
    positionnerCouleur(0);
    gb.display.print("App. sur A ");
    if (gb.buttons.pressed(BUTTON_A)) {
      viderLaGrille();
      partieTerminee = false;
    }
  }
}

void afficherScores() {
  positionnerCouleur(0);
  gb.display.drawRect(LARGEUR_ECRAN - 22, DECALAGE_GRILLE_Y, 22, HAUTEUR_GRILLE);

  positionnerCouleur(0);
  gb.display.setCursor(LARGEUR_ECRAN - 20, 20);
  gb.display.print("SCORE");
  gb.display.setCursor(LARGEUR_ECRAN - 20, 30);
  gb.display.print("NUL:");
  gb.display.print(scores[0]);

  positionnerCouleur(1);
  gb.display.setCursor(LARGEUR_ECRAN - 20, 40);
  gb.display.print("J1:");
  gb.display.print(scores[1]);

  positionnerCouleur(2);
  gb.display.setCursor(LARGEUR_ECRAN - 20, 50);
  gb.display.print("J2:");
  gb.display.print(scores[2]);
}

void changerDeJoueur() {
  if (joueurEnCours == 1) {
    joueurEnCours = 2 ;
  } else {
    joueurEnCours = 1;
  }
}

void viderLaGrille() {
  for (int i = 0; i < LIGNES_GRILLE; i++) {
    for (int j = 0; j < COLONNES_GRILLE; j++) {
      grillePions[i][j] = 0;
    }
  }
}

void dessinerGrille() {
  for (int i = 0; i < COLONNES_GRILLE; i++) {
    for (int j = 0; j < LIGNES_GRILLE; j++) {
      int xCellule = DECALAGE_GRILLE_X + i * TAILLE_CELLULE;
      int yCellule = DECALAGE_GRILLE_Y + j * TAILLE_CELLULE;
      gb.display.drawRect(xCellule, yCellule, TAILLE_CELLULE, TAILLE_CELLULE);
    }
  }
}



void dessinerPions() {
  for (int i = 0; i < COLONNES_GRILLE; i++) {
    for (int j = 0; j < LIGNES_GRILLE; j++) {
      int xCellule = DECALAGE_GRILLE_X + i * TAILLE_CELLULE;
      int yCellule = DECALAGE_GRILLE_Y + j * TAILLE_CELLULE;
      int numeroJoueur = grillePions[j][i]; // Récupérer le numéro du joueur associé au pion
      if (numeroJoueur > 0 && numeroJoueur < 3) {
        positionnerCouleur(numeroJoueur);
        gb.display.fillCircle(xCellule + TAILLE_CELLULE / 2 - 1, yCellule + TAILLE_CELLULE / 2 - 1, TAILLE_CELLULE / 2 - 2);
      }
    }
  }
}

int compterCasesJouables() {
  int nombreCasesJouables = 0;
  boolean uneTrouvee = false; //on ne compte qu'une ou 0 case jouable par colonne
  for (int colonne = 0; colonne < COLONNES_GRILLE; colonne++) {
    for (int ligne = LIGNES_GRILLE - 1; ligne >= 0; ligne--) {
      if (!uneTrouvee && grillePions[ligne][colonne] == 0) {
        nombreCasesJouables++;
        uneTrouvee = true;
      }
    }
    uneTrouvee = false;
  }
  return nombreCasesJouables;
}

int** chercherCaseJouable(int nombreCasesJouables) {

  boolean uneTrouvee = false;
  int** casesJouables = new int*[nombreCasesJouables];  // tableau 2D dynamique pour stocker les coordonnées des cases jouables
  for (int i = 0; i < nombreCasesJouables; i++) {
    casesJouables[i] = new int[2];
  }

  int index = 0;
  // Remplir le tableau avec les coordonnées des cases jouables
  for (int colonne = 0; colonne < COLONNES_GRILLE; colonne++) {
    for (int ligne = LIGNES_GRILLE - 1; ligne >= 0; ligne--) {
      if (grillePions[ligne][colonne] == 0 && !uneTrouvee) {
        casesJouables[index][0] = colonne;
        casesJouables[index][1] = ligne;
        index++;
        uneTrouvee = true;
      }
    }
    uneTrouvee = false;
  }
  return casesJouables;
}

boolean grillePleine() {
  boolean plein = true;
  for (int i = 0; i < 7; i++) {
    if (grillePions[0][i] == 0) {
      plein = false;
    }
  }
  return plein;
}

void positionnerCouleur(int joueur) {
  if (joueur == 1 ) {
    gb.display.setColor(Color::red); // Couleur du joueur 1 (rouge)
  } else if (joueur == 2 ) {
    gb.display.setColor(Color::yellow); // Couleur du joueur 2 (jaune)
  } else {
    gb.display.setColor(Color::white); // Couleur par défaut (blanc)
  }
}



void jouerUnTour() {
  
  int nombreCasesJouables = compterCasesJouables();
  int** casesJouables = chercherCaseJouable(nombreCasesJouables);

  //déplacement du curseur
  if (gb.buttons.pressed(BUTTON_LEFT) && indexCurseur > 0) {
    indexCurseur -= 1;
  }else if (gb.buttons.pressed(BUTTON_RIGHT) && indexCurseur < nombreCasesJouables - 1) {
    indexCurseur += 1;
  }
  absCurseur = casesJouables[indexCurseur][0];

  //jeu dans une colonne
  if (gb.buttons.pressed(BUTTON_A) && indexCurseur < nombreCasesJouables) {
    grillePions[casesJouables[indexCurseur][1]][absCurseur] = joueurEnCours;
    aJoue = true;

  }
  
  //libération de la mémoire allouée pour casesJouables
  for (int i = 0; i < nombreCasesJouables; i++) {
    delete[] casesJouables[i];
  }
  delete[] casesJouables;
}



void dessinerCurseur() {
  positionnerCouleur(joueurEnCours);
  float xCellule = absCurseur * TAILLE_CELLULE + TAILLE_CELLULE / 2 - TAILLE_CELLULE / 4 - 1;
  gb.display.fillCircle(xCellule + TAILLE_CELLULE / 2 - 1, DECALAGE_GRILLE_Y - TAILLE_CELLULE / 2 - 1, TAILLE_CELLULE / 2 - 2);
}



int verifierPuissance4(int joueur) {
  //puissances 4 horizontales
  for (int i = 0; i < LIGNES_GRILLE; i++) {
    for (int j = 0; j < COLONNES_GRILLE - 3; j++) {
      if (grillePions[i][j] == joueur && grillePions[i][j + 1] == joueur && grillePions[i][j + 2] == joueur && grillePions[i][j + 3] == joueur) {
        return joueur;
      }
    }
  }

  //puissances 4 verticales
  for (int i = 0; i < LIGNES_GRILLE - 3; i++) {
    for (int j = 0; j < COLONNES_GRILLE; j++) {
      if (grillePions[i][j] == joueur && grillePions[i + 1][j] == joueur && grillePions[i + 2][j] == joueur && grillePions[i + 3][j] == joueur) {
        return joueur;
      }
    }
  }

  //puissances 4 diagonales bas droite
  for (int i = 0; i < LIGNES_GRILLE - 3; i++) {
    for (int j = 0; j < COLONNES_GRILLE - 3; j++) {
      if (grillePions[i][j] == joueur && grillePions[i + 1][j + 1] == joueur && grillePions[i + 2][j + 2] == joueur && grillePions[i + 3][j + 3] == joueur) {
        return joueur;
      }
    }
  }

  //puissances 4 diagonales haut droite
  for (int i = 0; i < LIGNES_GRILLE - 3; i++) {
    for (int j = 3; j < COLONNES_GRILLE; j++) {
      if (grillePions[i][j] == joueur && grillePions[i + 1][j - 1] == joueur && grillePions[i + 2][j - 2] == joueur && grillePions[i + 3][j - 3] == joueur) {
        return joueur;
      }
    }
  }

  //aucun pattern trouve
  return 0;
}

