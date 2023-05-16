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

void setup() {
  gb.begin();
  /* TODO DEBUG SUPPRIMER données de test*/
  /*int colonneExclue = 3; // Choisissez la colonne à exclure (entre 0 et COLONNES_GRILLE-1)
    for (int i = 5; i > 1;i--){
    grillePions[i][i]=1;
    }*/

}

int joueurEnCours = 1;
int joueurPrecedent = 3;
boolean aJoue = false;
boolean partieTerminee = false;

void loop() {
  while (!gb.update());
  gb.display.clear();
  dessinerGrille();
  dessinerPions();

  if (!partieTerminee) {
    positionnerCouleur(joueurEnCours);
    tourJoueur();

    gb.display.print("C'est au joueur ");
    gb.display.print(joueurEnCours);

    // Alternance des joueurs
    if (aJoue) {
      if (joueurEnCours == 1) {
        joueurEnCours = 2 ;
        joueurPrecedent = 1;
      } else {
        joueurEnCours = 1;
        joueurPrecedent = 2;
      }

      aJoue = false;
    }
    


    if (grillePleine()) {
      partieTerminee = true;
    }
  } else {
    gb.display.setColor(Color::green);
    gb.display.println("Partie terminee : ");
    if (grillePleine()) {
      gb.display.println("egalite!");
    } else {
      positionnerCouleur(joueurEnCours);
      gb.display.print("le joueur ");
      gb.display.print(joueurPrecedent);
      gb.display.print(" gagne!");
    }
    //ETAT FIN DE PARTIE joueur 1, joueur2,grille pleine
    //SCORING
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
  }
}

int absCurseur = 0;
int indexCurseur = 0;
int curseurX = 4;
int curseurY = 2;

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
    if (trouverLaPuissance4(joueurEnCours) == joueurEnCours) {
      partieTerminee = true;
    }
  }
  // Libération de la mémoire allouée pour casesJouables
  for (int i = 0; i < nombreCasesJouables; i++) {
    delete[] casesJouables[i];
  }
  delete[] casesJouables;
  dessinerCurseur();
}



void dessinerCurseur() {
  // Dessin du curseur
  float xCellule = absCurseur * TAILLE_CELLULE + TAILLE_CELLULE / 2 - curseurX / 2;
  gb.display.drawRect(xCellule, 10, curseurX, curseurY);
}



int trouverLaPuissance4(int joueur_actuel) {
  int joueur_gagnant = 0;
  int pionJoueurValide = 0;
  
  for (int colonne = 0; colonne < COLONNES_GRILLE; colonne++) {
    for (int ligne = 0; ligne < LIGNES_GRILLE; ligne++) {
      if (grillePions[ligne][colonne] != joueur_actuel) {
        continue;
      };

      //verification des pions a droite
      if (grillePions[ligne][colonne+1] == joueur_actuel) {
        pionJoueurValide = 0;
        for (int decalage = 0; decalage <= 3 ; decalage++) {
          if (grillePions[ligne][colonne + decalage] == joueur_actuel) {
            pionJoueurValide++;
          }
        }
        if (pionJoueurValide == 4 ) {
          joueur_gagnant = joueur_actuel;
        }
      }
      
      //verification des pions en diagonale basse droite
      if (grillePions[ligne+1][colonne+1] == joueur_actuel) {
        pionJoueurValide = 0;
        for (int decalage = 0; decalage <= 3 ; decalage++) {
          if (grillePions[ligne + decalage][colonne + decalage] == joueur_actuel) {
            pionJoueurValide++;
          }
        }
        if (pionJoueurValide == 4 ) {
          joueur_gagnant = joueur_actuel;
        }
      }
      
      //verification des pions en diagonale haute droite
      if (grillePions[ligne-1][colonne+1] == joueur_actuel) {
        pionJoueurValide = 0;
        for (int decalage = 0; decalage <= 3 ; decalage++) {
          if (grillePions[ligne - decalage][colonne + decalage] == joueur_actuel) {
            pionJoueurValide++;
          }
        }
        if (pionJoueurValide == 4 ) {
          joueur_gagnant = joueur_actuel;
        }
      }
      
      //verification des pions au dessous
      if (grillePions[ligne+1][colonne] == joueur_actuel) {
        pionJoueurValide = 0;
        for (int decalage = 0; decalage <= 3 ; decalage++) {
          if (grillePions[ligne + decalage][colonne] == joueur_actuel) {
            pionJoueurValide++;
          }
        }
        if (pionJoueurValide == 4 ) {
          joueur_gagnant = joueur_actuel;
        }
      }

    }
  }

  return joueur_gagnant;
}