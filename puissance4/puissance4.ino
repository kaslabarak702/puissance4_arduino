#include <Gamebuino-Meta.h>

const int COLONNES_GRILLE = 7; // Taille de la grille (7 colonnes x 6 lignes)
const int LIGNES_GRILLE = 6;
const int TAILLE_CELLULE = 8; // Taille d'une cellule de la grille
const int LARGEUR_ECRAN = gb.display.width();
const int HAUTEUR_ECRAN = gb.display.height();
const int DECALAGE_GRILLE_X = 0; // Décalage horizontal de la grille
const int DECALAGE_GRILLE_Y = HAUTEUR_ECRAN * 0.25; // Décalage vertical de la grille
const int LARGEUR_GRILLE = LIGNES_GRILLE * TAILLE_CELLULE; // Largeur de la grille
const int HAUTEUR_GRILLE = (HAUTEUR_ECRAN - DECALAGE_GRILLE_Y); // Hauteur de la grille
int grillePions[LIGNES_GRILLE][COLONNES_GRILLE];
int joueurEnCours = 1;
boolean aJoue = false;
boolean partieTerminee = false;

int absCurseur = 0;
int indexCurseur = 0;
int curseurX = 4;
int curseurY = 2;

int scores[3]; // nuls / joueur 1 / joueur 2

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
    dessinerCurseur();
    positionnerCouleur(joueurEnCours);
    tourJoueur();
    if (verifierPuissance4(joueurEnCours) == joueurEnCours) {
      partieTerminee = true;
      scores[joueurEnCours]++;
   }
    if (grillePleine()) {
      partieTerminee = true;
      scores[0]++;
    }
    // Alternance des joueurs
    if (aJoue&&!partieTerminee) {
      changerDeJoueur();
      aJoue = false;
    }
  } else { //la partie est terminee
    gb.display.setCursor(1,1);
    gb.display.setColor(Color::green);
    gb.display.println("Partie terminee : ");
    if (grillePleine()) {
      gb.display.println("egalite!");
    } else {
      
      positionnerCouleur(joueurEnCours);
      gb.display.print("le joueur ");
      gb.display.print(joueurEnCours);
      gb.display.print(" gagne!");
    }
    if (gb.buttons.pressed(BUTTON_A)) {
      viderLaGrille();
      partieTerminee=false;
    }
    //TODO SCORING
  }
}

void afficherScores(){
   gb.display.setFontSize(1);

  

  // Effacer la zone des scores
  positionnerCouleur(0);
  gb.display.drawRect(LARGEUR_ECRAN-22, DECALAGE_GRILLE_Y, 22, HAUTEUR_GRILLE);

  positionnerCouleur(0);
  gb.display.setCursor(LARGEUR_ECRAN -20, 20);
  gb.display.print("NUL:");
  gb.display.print(scores[0]);
  
  positionnerCouleur(1);
  gb.display.setCursor(LARGEUR_ECRAN -20, 30);
  gb.display.print("J1:");
  gb.display.print(scores[1]);
  
  positionnerCouleur(2);
  gb.display.setCursor(LARGEUR_ECRAN -20, 40);
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
        positionnerCouleur(numeroJoueur);
        gb.display.fillCircle(xCellule + TAILLE_CELLULE / 2 - 1, yCellule + TAILLE_CELLULE / 2 - 1, TAILLE_CELLULE / 2 - 2);
      }
    }
  }
}

int compterCasesJouables() {
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
  return nombreCasesJouables;
}

int** chercherCaseJouable(int nombreCasesJouables) {

  boolean uneTrouvee = false;
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
    /*gb.display.print(x);
      gb.display.print(":");
      gb.display.print(y);
      gb.display.print("/");*/
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
  }else if (joueur == 0 ) {
    gb.display.setColor(Color::white); // Couleur du joueur 2 (jaune)
  }
}



void tourJoueur() {

  int nombreCasesJouables = compterCasesJouables();
  int** casesJouables = chercherCaseJouable(nombreCasesJouables);

  if (gb.buttons.pressed(BUTTON_LEFT) && indexCurseur > 0) {
    indexCurseur -= 1;
  }

  if (gb.buttons.pressed(BUTTON_RIGHT) && indexCurseur < nombreCasesJouables - 1) {
    indexCurseur += 1;
  }

  absCurseur = casesJouables[indexCurseur][0];

  if (gb.buttons.pressed(BUTTON_A) && indexCurseur < nombreCasesJouables) {
    grillePions[casesJouables[indexCurseur][1]][absCurseur] = joueurEnCours;
    aJoue = true;
    
  }
  // Libération de la mémoire allouée pour casesJouables
  for (int i = 0; i < nombreCasesJouables; i++) {
    delete[] casesJouables[i];
  }
  delete[] casesJouables;
}



void dessinerCurseur() {
  // Dessin du curseur
  positionnerCouleur(joueurEnCours);
  float xCellule = absCurseur * TAILLE_CELLULE + TAILLE_CELLULE / 2 - curseurX / 2;
  gb.display.drawRect(xCellule, 10, curseurX, curseurY);
}


int verifierPuissance4(int joueur) {
  // Vérifier les puissances 4 horizontales
  for (int i = 0; i < LIGNES_GRILLE; i++) {
    for (int j = 0; j < COLONNES_GRILLE - 3; j++) {
      if (grillePions[i][j] == joueur && grillePions[i][j + 1] == joueur && grillePions[i][j + 2] == joueur && grillePions[i][j + 3] == joueur) {
        return joueur;
      }
    }
  }

  // Vérifier les puissances 4 verticales
  for (int i = 0; i < LIGNES_GRILLE - 3; i++) {
    for (int j = 0; j < COLONNES_GRILLE; j++) {
      if (grillePions[i][j] == joueur && grillePions[i + 1][j] == joueur && grillePions[i + 2][j] == joueur && grillePions[i + 3][j] == joueur) {
        return joueur;
      }
    }
  }

  // Vérifier les puissances 4 diagonales bas droite (\)
  for (int i = 0; i < LIGNES_GRILLE - 3; i++) {
    for (int j = 0; j < COLONNES_GRILLE - 3; j++) {
      if (grillePions[i][j] == joueur && grillePions[i + 1][j + 1] == joueur && grillePions[i + 2][j + 2] == joueur && grillePions[i + 3][j + 3] == joueur) {
        return joueur;
      }
    }
  }

  // Vérifier les puissances 4 diagonales haut droite (/)
  for (int i = 0; i < LIGNES_GRILLE - 3; i++) {
    for (int j = 3; j < COLONNES_GRILLE; j++) {
      if (grillePions[i][j] == joueur && grillePions[i + 1][j - 1] == joueur && grillePions[i + 2][j - 2] == joueur && grillePions[i + 3][j - 3] == joueur) {
        return joueur;
      }
    }
  }

  // Aucune puissance 4 trouvée
  return 0;
}

