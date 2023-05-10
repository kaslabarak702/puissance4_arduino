  #include <Gamebuino-Meta.h>
  
  class Pion {
    private:
      int coordonnee_i; // Coordonnée abscisse du pion
      int coordonnee_j; // Coordonnée ordonnée du pion
      int numero_joueur; // Numéro du joueur associé au pion
      
    public:
      Pion() {
        coordonnee_i = 0;
        coordonnee_j = 0;
        numero_joueur = -1;
      }
    
      Pion(int i, int j, int joueur) {
        coordonnee_i = i;
        coordonnee_j = j;
        numero_joueur = joueur;
      }
      
      int getCoordonneeI() {
        return coordonnee_i;
      }
      
      int getCoordonneeJ() {
        return coordonnee_j;
      }
      
      void setCoordonnees(int i, int j) {
        coordonnee_i = i;
        coordonnee_j = j;
      }
      
      int getNumeroJoueur() {
        return numero_joueur;
      }
      
      void setNumeroJoueur(int joueur) {
        numero_joueur = joueur;
      }
  };
 
  
  const int COLONNES_GRILLE = 7; // Taille de la grille (7 colonnes x 6 lignes)
  const int LIGNES_GRILLE = 6;
  const int TAILLE_CELLULE = 8; // Taille d'une cellule de la grille
  
  const int LARGEUR_ECRAN = gb.display.width();
  const int HAUTEUR_ECRAN = gb.display.height();
  
  const int DECALAGE_GRILLE_X = 0; // Décalage horizontal de la grille
  const int DECALAGE_GRILLE_Y = HAUTEUR_ECRAN * 0.2; // Décalage vertical de la grille
  
  const int LARGEUR_GRILLE = LIGNES_GRILLE * TAILLE_CELLULE; // Largeur de la grille
  const int HAUTEUR_GRILLE = (HAUTEUR_ECRAN - DECALAGE_GRILLE_Y); // Hauteur de la grille
  
  Pion grillePions[LIGNES_GRILLE][COLONNES_GRILLE]; 
  
  
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
        int numeroJoueur = grillePions[j][i].getNumeroJoueur(); // Récupérer le numéro du joueur associé au pion
        if(numeroJoueur > 0&&numeroJoueur<3){
          if (numeroJoueur == 1) {
            gb.display.setColor(Color::red); // Couleur du joueur 1 (rouge)
          } else if (numeroJoueur == 2) {
            gb.display.setColor(Color::yellow); // Couleur du joueur 2 (jaune)
          }
          
          gb.display.fillCircle(xCellule + TAILLE_CELLULE/2-1, yCellule + TAILLE_CELLULE/2-1, TAILLE_CELLULE/2 - 2);
        }
      }
    }
  }

  void setup() {
    gb.begin();
    grillePions[0][0] = Pion(0,0,1);
    grillePions[0][0] = Pion(0,0,2);
    grillePions[0][0] = Pion(0,0,1);
  }

  
void loop() {
  while (!gb.update());
  dessinerGrille();
  dessinerPions();

  tourJouer();
}

int indexCurseur = 0;
int valeurCurseur = 1;
int curseurX = 4;
int curseurY = 2;

void tourJouer() {
  //gb.display.clear();
  
  int colonnesJouables[7] = {1, 2, 3, 4, 5, 6, 7};
  int tailleColonnesJouable = 7;

  if((gb.buttons.pressed(BUTTON_LEFT)) && indexCurseur - 1 >= 0){
    indexCurseur -= 1;
  }
  if((gb.buttons.pressed(BUTTON_RIGHT)) && indexCurseur < tailleColonnesJouable-1){
    indexCurseur += 1;
  }
  valeurCurseur = colonnesJouables[indexCurseur];
  
  dessinerCurseur();
}



void dessinerCurseur(){
  // Dessin du curseur
  gb.display.print(valeurCurseur);
  float xCellule = valeurCurseur * TAILLE_CELLULE - TAILLE_CELLULE/2 - curseurX/2;
  gb.display.drawRect(xCellule, 4, curseurX, curseurY);
}

  



 
