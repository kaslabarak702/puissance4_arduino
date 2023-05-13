#include <Gamebuino-Meta.h>

const int COLONNES_GRILLE = 7; // Taille de la grille (7 colonnes x 6 lignes)
const int LIGNES_GRILLE = 6;
const int TAILLE_CELLULE = 8; // Taille d'une cellule de la grille

const int LARGEUR_ECRAN = gb.display.width();
const int HAUTEUR_ECRAN = gb.display.height();

const int DECALAGE_GRILLE_X = 0; // Décalage horizontal de la grille
const int DECALAGE_GRILLE_Y = HAUTEUR_ECRAN * 0.2; // Décalage vertical de la grille

const int LARGEUR_GRILLE = LIGNES_GRILLE * TAILLE_CELLULE; // Largeur de la grille
const int HAUTEUR_GRILLE = (HAUTEUR_ECRAN - DECALAGE_GRILLE_Y); // Hauteur de la grille

int grillePions[LIGNES_GRILLE][COLONNES_GRILLE];


void dessinerGrille() {
  gb.display.clear();

  // Dessin de la grille
  for (int i = 0; i < COLONNES_GRILLE; i++) {
    for (int j = 0; j < LIGNES_GRILLE; j++) {
      int xCellule = DECALAGE_GRILLE_X + i * TAILLE_CELLULE;
      int yCellule = DECALAGE_GRILLE_Y + j * TAILLE_CELLULE;

      gb.display.drawRect(xCellule, yCellule, TAILLE_CELLULE, TAILLE_CELLULE);
    }
  }
};



void dessinerPions() {
  for (int i = 0; i < COLONNES_GRILLE; i++) {
    for (int j = 0; j < LIGNES_GRILLE; j++) {
      int xCellule = DECALAGE_GRILLE_X + i * TAILLE_CELLULE;
      int yCellule = DECALAGE_GRILLE_Y + j * TAILLE_CELLULE;
      int numeroJoueur = grillePions[j][i]; // Récupérer le numéro du joueur associé au pion
      if (numeroJoueur > 0 && numeroJoueur < 3) {
        if (numeroJoueur == 1) {
          gb.display.setColor(Color::red); // Couleur du joueur 1 (rouge)
        } else if (numeroJoueur == 2) {
          gb.display.setColor(Color::yellow); // Couleur du joueur 2 (jaune)
        }

        gb.display.fillCircle(xCellule + TAILLE_CELLULE / 2 - 1, yCellule + TAILLE_CELLULE / 2 - 1, TAILLE_CELLULE / 2 - 2);
      }
    }
  }
}


int** chercherCaseJouable() {
  int nombreCasesJouables = 0;
  boolean uneTrouvee = false;
  // Compter le nombre de cases jouables
  for (int colonne = 0; colonne < COLONNES_GRILLE; colonne++) {
    for (int ligne = LIGNES_GRILLE - 1; ligne >= 0; ligne--) {
      if (grillePions[ligne][colonne] == 0 && !uneTrouvee) {
        nombreCasesJouables++;
        uneTrouvee = true;
      }
    }
    uneTrouvee = false;
  }

  // Allouer un tableau 2D dynamique pour stocker les coordonnées des cases jouables
  int** casesJouables = new int*[nombreCasesJouables];
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
  /* TODO DEBUG SUPPRIMER*/
  for (int i = 0; i < nombreCasesJouables; i++) {
    int x = casesJouables[i][0];
    int y = casesJouables[i][1];

    // Afficher la coordonnée x avec un espace après
    gb.display.print(x);
    gb.display.print(":");
    gb.display.print(y);
    gb.display.print("/");
  }

  // Libération de la mémoire allouée pour casesJouables
  for (int i = 0; i < nombreCasesJouables; i++) {
    delete[] casesJouables[i];
  }
  delete[] casesJouables;

  return casesJouables;
}




void setup() {
  gb.begin();
  /* TODO DEBUG SUPPRIMER données de test*/
  /*int colonneExclue = 2; // Choisissez la colonne à exclure (entre 0 et COLONNES_GRILLE-1)
    for (int colonne = 0; colonne < COLONNES_GRILLE; colonne++) {
    if (colonne != colonneExclue) {
      for (int ligne = LIGNES_GRILLE - 1; ligne >= 0; ligne--) {
        int joueur = (colonne % 2) + 1; // Alternance des joueurs 1 et 2

        grillePions[ligne][colonne] = joueur;
      }
    }
    }*/
  /*for (int colonne = 0; colonne < COLONNES_GRILLE; colonne++) {
    if (colonne ==2) {
      for (int ligne = LIGNES_GRILLE - 1; ligne >= 0; ligne--) {
        int joueur = (rand() % 2) + 1; // Génère un nombre aléatoire entre 1 et 2

        grillePions[ligne][colonne] = joueur;
      }
    }
  }*/
}


void loop() {
  while (!gb.update());
  dessinerGrille();
  dessinerPions();
  //chercherCaseJouable();
  tourJouer();
}

int indexCurseur = 0;
int valeurCurseur = 0;
int curseurX = 4;
int curseurY = 2;

void tourJouer() {
  //gb.display.clear();

  int** colonnesJouables = chercherCaseJouable();
  int tailleColonnesJouable = 7;

  if ((gb.buttons.pressed(BUTTON_LEFT)) && indexCurseur > 0) {
    indexCurseur -= 1;
  }
  if ((gb.buttons.pressed(BUTTON_RIGHT)) && indexCurseur < tailleColonnesJouable && indexCurseur > 0) {
    indexCurseur += 1;
  }
  valeurCurseur = colonnesJouables[indexCurseur][0];

  if ((gb.buttons.pressed(BUTTON_A)) && (indexCurseur < tailleColonnesJouable) && (indexCurseur < tailleColonnesJouable)) {
    for (int i = LIGNES_GRILLE; i >= 0; i--) {
      if (grillePions[i][valeurCurseur] == 0) {
        grillePions[i][valeurCurseur] = 1;
        dessinerPions();
        break;
      }
    }
  }

  dessinerCurseur();
}



void dessinerCurseur() {
  // Dessin du curseur
  gb.display.println(valeurCurseur);
  float xCellule = valeurCurseur * TAILLE_CELLULE + TAILLE_CELLULE / 2 - curseurX / 2;
  gb.display.drawRect(xCellule, 4, curseurX, curseurY);
}






