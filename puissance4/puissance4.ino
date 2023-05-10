#include <Gamebuino-Meta.h>

class Pion {
  private:
    int coordonnee_i; // Coordonnée i du pion
    int coordonnee_j; // Coordonnée j du pion
    int numero_joueur; // Numéro du joueur associé au pion
    
  public:
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


const int GRILLE_TAILLEY = 7;
const int GRILLE_TAILLEX = 6;
int grille[GRILLE_TAILLEX][GRILLE_TAILLEY] = {{1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1}};


float ligne_hauteur = 2;
float ligne_largeur = 50;

float colonne_hauteur = 5;
float colonne_largeur = 2;

void setup() {
  // put your setup code here, to run once:
  gb.begin();
}

void loop() {
//tant que gagner == false
//Verif cases jouable (colonne vide, 
  
  while (!gb.update());
  gb.display.clear();

  gb.display.setColor(WHITE);

  for (int x = 0; x < GRILLE_TAILLEX +1; x += 1){
    
    int displayY = x * (colonne_largeur + 6) + 1;
    gb.display.fillRect(ligne_largeur, displayY, ligne_largeur, ligne_hauteur);
  }
}
