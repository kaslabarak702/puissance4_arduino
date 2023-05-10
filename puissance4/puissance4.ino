#include <Gamebuino-Meta.h>



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
  
  // put your main code here, to run repeatedly:
  while (!gb.update());
  gb.display.clear();

  gb.display.setColor(WHITE);

  for (int x = 0; x < GRILLE_TAILLEX +1; x += 1){
    
    int displayY = x * (colonne_largeur + 6) + 1;
    gb.display.fillRect(ligne_largeur, displayY, ligne_largeur, ligne_hauteur);
  }
}
