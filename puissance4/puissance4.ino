#include <Gamebuino-Meta.h>

// Déclaration de la classe Pion
class Pion {
  private:
    int coordonnee_i; // Coordonnée i du pion
    int coordonnee_j; // Coordonnée j du pion
    int numero_joueur; // Numéro du joueur associé au pion
    
  public:
    // Constructeur
    Pion(int i, int j, int joueur) {
      coordonnee_i = i;
      coordonnee_j = j;
      numero_joueur = joueur;
    }
    
    // Méthode pour obtenir la coordonnée i du pion
    int getCoordonneeI() {
      return coordonnee_i;
    }
    
    // Méthode pour obtenir la coordonnée j du pion
    int getCoordonneeJ() {
      return coordonnee_j;
    }
    
    // Méthode pour définir les coordonnées i et j du pion
    void setCoordonnees(int i, int j) {
      coordonnee_i = i;
      coordonnee_j = j;
    }
    
    // Méthode pour obtenir le numéro du joueur associé au pion
    int getNumeroJoueur() {
      return numero_joueur;
    }
    
    // Méthode pour définir le numéro du joueur associé au pion
    void setNumeroJoueur(int joueur) {
      numero_joueur = joueur;
    }
};


const int GRILLE_TAILLEY = 7;
const int GRILLE_TAILLEX = 6;
int grille[GRILLE_TAILLEX][GRILLE_TAILLEY] = {{1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1}};


float ligne_hauteur = 2;
float ligne_largeur = 70;

float colonne_hauteur = 60;
float colonne_largeur = 2;

void setup() {
  // put your setup code here, to run once:
  gb.begin();
}

void loop() {
//tant que (!quelqunAgagné)
//Verif cases jouable (colonne vide)
//demander selection(aux 2 jouers alternativement
//fin du tour inverser la variable booleene
  
  // put your main code here, to run repeatedly:
  while (!gb.update());
  gb.display.clear();

  gb.display.setColor(WHITE);

  for (int x = 0; x < GRILLE_TAILLEX +1; x += 1){
    
    int displayY = x * (colonne_largeur + 6) + 1;
    
    gb.display.fillRect(4, displayY, ligne_largeur, ligne_hauteur);
  }
  for (int x = 0; x < GRILLE_TAILLEY +1; x += 1){
    
    int displayX = x * (ligne_hauteur + 6) + 1;
    
    gb.display.fillRect(displayX, 3, colonne_largeur, colonne_hauteur);
  }
}
