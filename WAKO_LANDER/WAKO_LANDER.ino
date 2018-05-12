//WAKO LANDER
//Inspiré du fameux Lunar Lander ;)
//François-Xavier Talgorn - 01/2018
//www.kitco.from

// Nécessaire pour l'environnement Kitco
#include "kitco.h"
#include <EEPROM.h>

//Déclarations des constantes
#define TAILLE_LANDER 3 // Largeur en pixel

//Les modes du jeu
#define ACCUEIL 0
#define PARTIE 1
#define ANNONCE_NIVEAU 2

//Constantes liées aux plateformes
#define PLATEFORME1_X_COORD 10 //coordonnées en X du début plateforme
#define PLATEFORME2_X_COORD 35
#define PLATEFORME3_X_COORD 60
#define HAUTEUR_PLATEFORME 3

#define AUCUNE_TOUCHE 0

//Note sur l'utilisation de la fonction drawX11Bitmap()
//Pour générer les valeurs hexas de l'image (format .XBM):
//Utiliser Gimp pour exporter la bitmap au format XBM par défaut (pas X10 flavour).
//Ouvrir le fichier texte produit dans un éditeur de texte et recopier les lignes
//dans le code arduino.
// *** Attention : Hauteur et Largeur doivent être MULTIPLES DE 2 ***

//SplashScreen Acceuil
#define title_width 66
#define title_height 24
static unsigned char title_bits[] = {
   0xe0, 0x79, 0xfe, 0xf1, 0xdf, 0xf3, 0xfc, 0x03, 0x00, 0x20, 0x49, 0x01,
   0x0a, 0x50, 0x92, 0x02, 0x04, 0x00, 0x20, 0x49, 0x01, 0x0a, 0x50, 0x9a,
   0x02, 0x04, 0x00, 0x20, 0x49, 0x79, 0x8a, 0x5f, 0xce, 0xf2, 0x04, 0x00,
   0x20, 0x49, 0x01, 0xca, 0x40, 0x40, 0x92, 0x04, 0x00, 0x20, 0x4f, 0x01,
   0xca, 0x40, 0x40, 0x92, 0x04, 0x00, 0x20, 0x49, 0x79, 0x8a, 0x5f, 0xce,
   0xf2, 0x04, 0x00, 0x20, 0x40, 0x49, 0x0a, 0x50, 0x9a, 0x02, 0x04, 0x00,
   0x20, 0x40, 0x49, 0x0a, 0x50, 0x92, 0x02, 0x04, 0x00, 0xc0, 0x3f, 0xcf,
   0xf3, 0xdf, 0xf3, 0xfc, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x8f,
   0x73, 0xfc, 0xe1, 0xbf, 0xff, 0x00, 0x09, 0x08, 0x50, 0x92, 0x02, 0x12,
   0xa0, 0x00, 0x01, 0x09, 0x08, 0x50, 0x94, 0x02, 0x14, 0xa0, 0x00, 0x01,
   0x09, 0xc8, 0x53, 0x98, 0x72, 0x94, 0xbf, 0x3c, 0x01, 0x09, 0x08, 0x50,
   0x92, 0xd2, 0x14, 0x82, 0x04, 0x01, 0x09, 0x08, 0x50, 0x86, 0xd2, 0x14,
   0x82, 0x04, 0x01, 0xf9, 0xcb, 0x53, 0x8a, 0x72, 0x94, 0xbf, 0x3c, 0x01,
   0x01, 0x4a, 0x52, 0x92, 0x02, 0x14, 0xa0, 0x24, 0x01, 0x01, 0x4a, 0x52,
   0x92, 0x02, 0x12, 0xa0, 0x24, 0x01, 0xfe, 0x7b, 0xde, 0xf3, 0xfc, 0xe1,
   0xbf, 0xe7, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#define level_width 16
#define level_height 4
static unsigned char level_bits[] = {
   0x6e, 0xfd, 0x6e, 0x7d, 0x9e, 0xfd, 0x98, 0x71 };

#define max_width 16
#define max_height 4
static unsigned char max_bits[] = {
   0x24, 0xda, 0xaa, 0x66, 0x2e, 0xe6, 0xae, 0x5a };

#define arrow_width 16
#define arrow_height 9
static unsigned char arrow_bits[] = {
   0xe0, 0x07, 0xff, 0xff, 0xfe, 0x7f, 0xfc, 0x3f, 0xf8, 0x1f, 0xf0, 0x0f,
   0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01 };

#define annonce_bottom_width 84
#define annonce_bottom_height 9
static unsigned char annonce_bottom_bits[] = {
   0x3f, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xfe, 0xff, 0x0f, 0xbf,
   0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xfe, 0xdf, 0x0f, 0xbf, 0x58,
   0x99, 0xdd, 0x8c, 0xff, 0xff, 0xcd, 0x66, 0xdb, 0x0f, 0xbf, 0x56, 0x6e,
   0x5d, 0xef, 0xff, 0xff, 0xb5, 0x5a, 0xdd, 0x0f, 0xbf, 0x50, 0x6f, 0xd5,
   0xcc, 0xff, 0xff, 0x85, 0x5a, 0xde, 0x0f, 0xbf, 0x56, 0x6f, 0xe3, 0xeb,
   0xff, 0xff, 0xb5, 0x5a, 0xfd, 0x0f, 0xbf, 0x58, 0x9f, 0x6b, 0x8c, 0xff,
   0xff, 0xb5, 0x66, 0xdb, 0x0f, 0xbf, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xfd, 0xfe, 0xff, 0x0f, 0x3f, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01,
   0xfe, 0xff, 0x0f };

#define bravo_width 52
#define bravo_height 12
static unsigned char bravo_bits[] = {
   0x7e, 0xf8, 0xe0, 0xcf, 0x60, 0xfe, 0x0c, 0xff, 0xfc, 0xf1, 0xdf, 0x60,
   0xff, 0x0d, 0xc3, 0x8c, 0x31, 0xd8, 0x60, 0x83, 0x0d, 0xc3, 0x8c, 0x31,
   0xd8, 0x60, 0x83, 0x0d, 0xc3, 0x8c, 0x31, 0xd8, 0x60, 0x83, 0x0d, 0x7f,
   0xfc, 0xf1, 0xdf, 0x71, 0x83, 0x0d, 0xff, 0xfc, 0xf3, 0x9f, 0x31, 0x83,
   0x0d, 0xc3, 0x0d, 0x37, 0x98, 0x3b, 0x83, 0x0d, 0x83, 0x0d, 0x36, 0x18,
   0x1b, 0x83, 0x0d, 0x83, 0x0d, 0x36, 0x18, 0x1f, 0x83, 0x01, 0xff, 0x0d,
   0x36, 0x18, 0x0e, 0xff, 0x0d, 0xff, 0x0c, 0x36, 0x18, 0x04, 0xfe, 0x0c };

#define BoomGood_width 44
#define BoomGood_height 22
static unsigned char BoomGood_bits[] = {
   0x7e, 0xf8, 0xf8, 0x7c, 0x7c, 0x07, 0xfe, 0xfc, 0xfd, 0xfd, 0x7e, 0x07,
   0xce, 0xde, 0xdd, 0xfd, 0x7e, 0x07, 0xce, 0xde, 0xdd, 0xed, 0x7e, 0x07,
   0xce, 0xde, 0xdd, 0xdd, 0x6e, 0x07, 0xce, 0xde, 0xdd, 0xdd, 0x76, 0x07,
   0xce, 0xde, 0xdd, 0xdd, 0x76, 0x07, 0xfe, 0xde, 0xdd, 0xdd, 0x77, 0x07,
   0xce, 0xde, 0xdd, 0xdd, 0x77, 0x07, 0xce, 0xde, 0xdd, 0xdd, 0x77, 0x07,
   0xce, 0xde, 0xdd, 0x99, 0x77, 0x07, 0xce, 0xde, 0xd9, 0x98, 0x77, 0x00,
   0xce, 0x5c, 0x00, 0x00, 0x63, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x40, 0x07,
   0x7e, 0x80, 0x00, 0x09, 0x00, 0x06, 0x00, 0xc8, 0x5c, 0xac, 0x01, 0x04,
   0x00, 0x88, 0x50, 0x9c, 0x03, 0x00, 0x80, 0xf5, 0xf0, 0x54, 0x23, 0x00,
   0x40, 0x3d, 0x8f, 0xf7, 0x8e, 0x00, 0x78, 0xdf, 0xff, 0xf7, 0xbf, 0x02,
   0x7e, 0xff, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f };

#define DIEGood_width 14
#define DIEGood_height 12
static unsigned char DIEGood_bits[] = {
   0xf0, 0x03, 0x0c, 0x0c, 0x02, 0x10, 0x02, 0x10, 0x39, 0x27, 0x19, 0x26,
   0x01, 0x20, 0xc1, 0x20, 0xce, 0x1c, 0x08, 0x04, 0x28, 0x05, 0xf0, 0x03 };

#define left_button_width 10
#define left_button_height 10
static unsigned char left_button_bits[] = {
   0xff, 0x03, 0x01, 0x02, 0x11, 0x02, 0x19, 0x02, 0xfd, 0x02, 0x19, 0x02,
   0x11, 0x02, 0x01, 0x02, 0xff, 0x03, 0x00, 0x00 };

#define right_button_width 10
#define right_button_height 10
static unsigned char right_button_bits[] = {
   0xff, 0x03, 0x01, 0x02, 0x21, 0x02, 0x61, 0x02, 0xfd, 0x02, 0x61, 0x02,
   0x21, 0x02, 0x01, 0x02, 0xff, 0x03, 0x00, 0x00 };

#define A_button_width 12
#define A_button_height 10
static unsigned char A_button_bits[] = {
   0xff, 0x07, 0x01, 0x04, 0x71, 0x04, 0x89, 0x04, 0xf9, 0x04, 0x89, 0x04,
   0x89, 0x04, 0x01, 0x04, 0xff, 0x07, 0x00, 0x00 };

#define B_button_width 12
#define B_button_height 10
static unsigned char B_button_bits[] = {
   0xff, 0x07, 0x01, 0x04, 0x79, 0x04, 0x89, 0x04, 0x79, 0x04, 0x89, 0x04,
   0x79, 0x04, 0x01, 0x04, 0xff, 0x07, 0x00, 0x00 };

#define DOLLARSmall_width 14
#define DOLLARSmall_height 12
static unsigned char DOLLARSmall_bits[] = {
   0x40, 0x01, 0xf0, 0x0f, 0xf8, 0x0f, 0x5c, 0x01, 0x5c, 0x01, 0xf8, 0x07,
   0xf0, 0x0f, 0x40, 0x1d, 0x40, 0x1d, 0xfc, 0x0f, 0xfc, 0x07, 0x40, 0x01 };

#define Sound_intro_width 55
#define Sound_intro_height 14
static unsigned char Sound_intro_bits[] = {
   0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbd, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x24, 0x80, 0x33, 0x29, 0x1d, 0x4c, 0x02, 0x3c, 0x44, 0x48,
   0x69, 0x25, 0xd2, 0x02, 0x24, 0x80, 0x4b, 0xa9, 0x25, 0x52, 0x03, 0x24,
   0xc4, 0x33, 0x26, 0x1d, 0x4c, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x9d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x80,
   0x33, 0x29, 0x1d, 0x8c, 0x73, 0x1c, 0x44, 0x48, 0x69, 0x25, 0x52, 0x08,
   0x24, 0x80, 0x4b, 0xa9, 0x25, 0xd2, 0x39, 0x1c, 0xc4, 0x33, 0x26, 0x1d,
   0x4c, 0x08 };

#define fuel_width 18
#define fuel_height 6
static unsigned char fuel_bits[] = {
   0x07, 0x00, 0x00, 0x51, 0x17, 0x00, 0x53, 0x13, 0x01, 0x51, 0x11, 0x00,
   0x21, 0x77, 0x01, 0x00, 0x00, 0x00 };


//Variables globales
byte modeJeu = ACCUEIL;
byte sound = 1;//1 sound, 0 no sound

//Paramètres des niveaux de jeu
typedef struct s_niveau {
  int score;//Points gagnés pour un atterrissage réussi
  float vitesse_max_atteros;//Vitesse verticale max pour atterir
  byte taille_plateforme_min;//En pixel
  byte taille_plateforme_max;//En pixel
  byte consommation_fuel;//Unités consommées à chaque déplacement
} Niveau;

Niveau t_niveau[]={{0,0.0,0,0,0}, {500,0.5,7,9,3}, {1000,0.4,5,9,3}, {3000,0.3,5,8,4},{5000,0.2,4,8,5},{10000,0.15,3,7,6}};//Caractéristiques des niveaux () / Pas de niveau 0
byte changement_de_niveau[]={0,1,3,7,13,22};//Nb atterissages atteints => niveau = index. Exemple: 7 atterissages = niveau 3
byte niveau;
byte alerte_niveau;

byte taille_plateforme_min;//En fonction du niveau du jeu
byte taille_plateforme_max;//En fonction du niveau du jeu

//Score
long score = 0;
long record;//Best score saved to eprom

byte facteur_bonus;//Bonus de score en fonction de la largeur des plateformes

//Variables associées au lander, initialisée dans initPartie()
byte lander_X_position;//Position horizontale du lander
byte lander_Y_position;//Position verticale du lander

float vitesse_horizontale = 0;
float vitesse_verticale = 0;
float horizontal_offset_deplacement = 0;
float vertical_offset_deplacement = 0;
float quantite_fuel=100;//Valeur sans unité.
float unite_de_fuel;//Une unite de fuel à afficher = largeur_jauge / quantite_fuel

int nb_atterissages = 0;
float vitesse_landing_max;

//Variables associées au terrain
byte crete[84];//La crête est dessinée sur toute la largeur de l'écran
byte plateformes[3];//Trois plateformes max. Si '1' dans la case du tableau, on crée une plateforme
byte coordonnees_limites_des_plateformes[3];//coordonnées X de fin des plateformes

//Structure pour éléments des plateformes
typedef struct plateforme {
  byte taille;
  byte coordonnee_limite;
  byte facteur_bonus;
} Plateforme;

//Trois plateformes max
Plateforme t_plateformes[3];

//UI
float LARGEUR_JAUGE = 66;// Largeur de l'affichage du carburant en pixels

void setup() {
  // Cette commande est nécessaire pour intialiser Kitco à son démarrage
  initialiserKitco(1);
  lcdBegin();
  setContrast(60);
  lumiereEcran(255);
  read_record();
  if(detecteTouche()==TOUCHE_A) {//Réinit du score en maintenant 'A' appuyez avant d'allumer la kitco
    score=5000;
    saveScore();
    }
  }

void initPartie(){
  if(nb_atterissages == 0) {
    score=0;//Ne pas réinitialiser le score si on a commencé un nouveau jeu
    niveau=1;
  }

  //PLATEFORMES D'ATTTERISSAGES
  taille_plateforme_min = t_niveau[niveau].taille_plateforme_min;
  taille_plateforme_max = t_niveau[niveau].taille_plateforme_max;
  creerPlateformes();

  //CRETE DU TERRAIN
  creerCrete();
  
  //LANDER
  //Le lander démarre à une vitesse horizontale aléatoire
  randomSeed(millis());//Initialisation du générateur aléatoire
  byte ip=random(1,3);//1 ou 2
  switch(ip){
    case 1:
      horizontal_offset_deplacement=(float) random(2,3)/10;
    break;
    case 2:
      horizontal_offset_deplacement=(float) random(2,3)/10 * (-1.0);
    break;
    default:
      horizontal_offset_deplacement=random(2,3)/10;
  }

  vertical_offset_deplacement=0;
  lander_X_position=random(20, 60);
  lander_Y_position=random(5, 12);
  vitesse_horizontale=0.3;
  vitesse_verticale=0;
  vitesse_landing_max = t_niveau[niveau].vitesse_max_atteros;

  //JAUGE FUEL
  quantite_fuel=800;
  unite_de_fuel=LARGEUR_JAUGE/quantite_fuel;//Pour l'affichage de la quantite de fuel restante
}

void saveScore(){
    byte four = (score & 0xFF);
    byte three = ((score >> 8) & 0xFF);
    byte two = ((score >> 16) & 0xFF);
    byte one = ((score >> 24) & 0xFF);
    //Write the 4 bytes into the eeprom memory.
    EEPROM.write(0, four);
    EEPROM.write(0 + 1, three);
    EEPROM.write(0 + 2, two);
    EEPROM.write(0 + 3, one);
    effacerEcran(BLANC);

    effacerEcran(BLANC);
    creerRectangle(0,0,83,22,1,NOIR);
    creerRectangle(2,2,80,20,0,BLANC);//Liseret blanc

    char str[12];
    char t_record[7];
    strcpy(str, "$");
    sprintf(t_record, "%ld", score);
    strcat(str, t_record);
    //On calcule la longueur de la chaine pour la centrer horizontalement
    byte x_pos = ( 84 - (strlen(str) * 6) ) / 2;
    ecrireEcran((const char*) str, x_pos, 12, BLANC);

    ecrireEcran("PRESS ANY KEY",3,27,NOIR);
    ecrireEcran("TO LEAVE.",15,37,NOIR);

    byte CLIGNOTANT=0;
    byte choix;
    do {
        choix=3;//3=pas de choix

        if (((millis()/500)%2)==0) {
            ecrireEcran("RECORD SAVED",6,4,CLIGNOTANT);
            if(sound==1) frequenceBuzzer(300,8);
        }
        else {
            creerRectangle(3,3,78,10,1,NOIR);
        }

        if(detecteTouche() != AUCUNE_TOUCHE) {
            //Attente relache touche
            do { delay(1);} while(detecteTouche() != AUCUNE_TOUCHE);
            choix=1;
            modeJeu=ACCUEIL;
        }
    rafraichirEcran();
    } while (choix==3);
    nb_atterissages=0;//=> le score sera réinitalisé au prochain lancement du jeu dans initPartie()
}

void alertLevelUp(int niveau){
    effacerEcran(BLANC);
    creerRectangle(0,0,83,22,1,NOIR);
    creerRectangle(2,2,80,20,0,BLANC);//Liseret blanc

    ecrireEcran("NOW YOU PLAY",6,27,NOIR);
    ecrireEcran("LEVEL #",17,37,NOIR);
    ecrireLettre('0'+niveau,59,37,NOIR);

    byte CLIGNOTANT=0;
    byte choix;
    do {
        choix=3;//3=pas de choix

        if (((millis()/500)%2)==0) {
            ecrireEcran("LEVEL UP!",16,8,CLIGNOTANT);
            if(sound==1) frequenceBuzzer(300,8);
        }
        else {
            creerRectangle(3,8,78,15,1,NOIR);
        }

        if(detecteTouche() != AUCUNE_TOUCHE) {
            //Attente relache touche
            do { delay(1);} while(detecteTouche() != AUCUNE_TOUCHE);
            choix=1;
            modeJeu=ANNONCE_NIVEAU;
        }
    rafraichirEcran();
    } while (choix==3);
  rafraichirEcran();
}

void gestion_niveau(int atterrissages){
  byte index;
  for( index=1; index < 6; index++){//Pas de niveau 0
    if( (atterrissages > 1) && (atterrissages == changement_de_niveau[index]) ) {
      niveau=index;//Le jeu commence au niveau 1
      vitesse_landing_max=t_niveau[niveau].vitesse_max_atteros;
      if( alerte_niveau == 0) {//Annonce 'Level Up' depuis dernier atterissage ?
        alertLevelUp(niveau);
        alerte_niveau=1;
      }
      break;
    }
  }
}

void creerPlateformes() {
  t_plateformes[0] = {0,0,0}, t_plateformes[1] = {0,0,0}, t_plateformes[2] = {0,0,0};
  
  randomSeed(millis());//Initialisation du générateur aléatoire

  //CREATION DES PLATEFORMES D'ATTTERISSAGE
  //On tire aléatoirement le nombre de plateformes entre 1 et 3
  //On tire alétoirement leurs tailles en pixel entre taille_plateforme_min et taille_plateforme_max
  //On tire aléatoirement la coordonnée horizontale des plateformes parmi 3 zones pré-existantes
  
  //Tirage du nombre et emplacement des plateformes
  byte nb_plateformes = random(0,3);//Tire de 0 à 2 inclut soit 1 à 3 plateformes

  if (nb_plateformes == 2) {//Donc 3 plateformes
    t_plateformes[0].taille=1;//Taille=1 => plateforme existe, taille réelle définie dans creerPlatforme()
    t_plateformes[1].taille=1;
    t_plateformes[2].taille=1;
  } else if (nb_plateformes==1)
    { //Si 2 plateformes alors la première sur zone 1 ou 2, la troisième en zone 3
      byte alea_plateforme = random(0,2);
      t_plateformes[alea_plateforme].taille=1;//Plateforme en zone 1 ou 2
      t_plateformes[2].taille=1;//Et une plateforme en zone 3
    } else t_plateformes[2].taille=1;//nb_plateformes = 1, forcément en zone 3
}

byte creerPlateforme(byte index, byte x_coord, byte y_coord) {
  byte taillePlateforme = taille_plateforme_min + random( 0, taille_plateforme_max - taille_plateforme_min );
  t_plateformes[index].taille = taillePlateforme;
  
  byte i;
  for (i=0; i<taillePlateforme; i++) {
    crete[x_coord+i]=y_coord-HAUTEUR_PLATEFORME;//Création de la plateforme dans tableau crete[]
  }

  if( taillePlateforme <= 3)
  {
    t_plateformes[index].facteur_bonus = 5;
  } else
  if( taillePlateforme >3 && taillePlateforme <= 5)
  {
    t_plateformes[index].facteur_bonus = 3;
  } else
  if( taillePlateforme >5 && taillePlateforme <= 9)
  {
    t_plateformes[index].facteur_bonus = 1;
  }

  //On stocke la coordonnée limite X de la plateforme
  byte coord_limite = x_coord + taillePlateforme;
  t_plateformes[index].coordonnee_limite = coord_limite;

  return x_coord + taillePlateforme;//rang_du_pixel = la coordonnée X précédente + taille plateforme
}

void creerCrete() {
  //On crée une ligne de crête par marche aléatoire
  //A chacune des coordonnées d'une zone de plateforme, on teste si on doit en dessiner et créer une
  randomSeed(millis());//Initialisation du générateur aléatoire
  
  const byte MONTEE=0;
  const byte HORIZONTALE=1;
  const byte DESCENTE=2;

  byte altitude_min = 36;//Coordonnée Y de l'écran
  byte altitude_max = 26;//Coordonnée Y de l'écran
  
  //byte altitude_pixel_initial = random(altitude_max+1, altitude_min-1);
  byte altitude_pixel_initial = random(altitude_max+1, altitude_min-1);
  byte altitude_pixel_courant = altitude_pixel_initial;
  
  byte rang_du_pixel=0;//0 à 83 pour les 84 pixels de large
  byte nb_pixels_dans_segment;
  byte direction_du_segment;
  
  byte offset_direction=0;
  
  bool encore = true;//On ajoute encore un pixel à la crête ?

  //On crée la crête
  do {
    nb_pixels_dans_segment = random(2,6);
    direction_du_segment = random(0,3);
    
    int nbPixels=0;//Pour compter les pixels ajoutés à la crête
    
    while (nbPixels < nb_pixels_dans_segment) {
      
      //Doit-on créer une plateforme ?
      if ( (rang_du_pixel == PLATEFORME1_X_COORD) && t_plateformes[0].taille == 1 ) {//Plateforme en zone 1 ?
        rang_du_pixel = creerPlateforme(0, rang_du_pixel, altitude_pixel_courant);
        break;//On sort du while
      }
      
      if ( (rang_du_pixel == PLATEFORME2_X_COORD) && t_plateformes[1].taille == 1 ) {//Plateforme en zone 2 ?
        rang_du_pixel = creerPlateforme(1, rang_du_pixel, altitude_pixel_courant);
        break;//On sort du while
      }

      if ( (rang_du_pixel == PLATEFORME3_X_COORD) && t_plateformes[2].taille == 1 ) {//Plateforme en zone 3 ?
        rang_du_pixel = creerPlateforme(2, rang_du_pixel, altitude_pixel_courant);
        break;//On sort du while
      }
      
      //Test limites altitudes de crete atteintes
      if (rang_du_pixel > 0){//Le premier pixel est forcément dans les limites,
        if ( crete[rang_du_pixel-1] == altitude_max) {//et on ne veut pas tester crête[-1], qui n'existe pas si premier pixel
          direction_du_segment = DESCENTE;
         } else if ( crete[rang_du_pixel-1] == altitude_min) {
          direction_du_segment = MONTEE;
         }
      }
      
      switch (direction_du_segment) {
        case HORIZONTALE:
          offset_direction = 0;
          break;
          
        case MONTEE:
          offset_direction = -1;
          break;
                  
        case DESCENTE:
          offset_direction = +1;
          break;
          
        default:
          offset_direction = 0;
      }

      crete[rang_du_pixel] = altitude_pixel_courant + offset_direction;

      // On s'arrête de créer quand on atteint le bord droit de l'écran
      if ( rang_du_pixel >= 83 ) { encore = false; break; }
      
      altitude_pixel_courant = crete[rang_du_pixel];
      rang_du_pixel++;
      nbPixels++;//index comparé à rang_du_pixel
    }//fin while
    
  } while ( encore == true); 
}


void dessineCrete(){
  for ( int i=0; i < 84; i++) { ligneEcran(i, crete[i], i, 47, NOIR); }

  if(t_plateformes[0].taille >= 3)//On dessine un trait blanc sous les plateformes existantes pour les souligner
    {
      //ligneEcran(PLATEFORME1_X_COORD, crete[PLATEFORME1_X_COORD]+1, t_plateformes[0].coordonnee_limite-1, crete[PLATEFORME1_X_COORD]+1, BLANC);
      ligneEcran(PLATEFORME1_X_COORD, crete[PLATEFORME1_X_COORD]+1, t_plateformes[0].coordonnee_limite-1, crete[PLATEFORME1_X_COORD]+1, BLANC);
    }
  if(t_plateformes[1].taille >= 3)
    {
      ligneEcran(PLATEFORME2_X_COORD, crete[PLATEFORME2_X_COORD]+1, t_plateformes[1].coordonnee_limite-1, crete[PLATEFORME2_X_COORD]+1, BLANC);
    }
  if(t_plateformes[2].taille >= 3)
    {
      ligneEcran(PLATEFORME3_X_COORD, crete[PLATEFORME3_X_COORD]+1, t_plateformes[2].coordonnee_limite-1, crete[PLATEFORME3_X_COORD]+1, BLANC);
    }
}


void dessineBonus(){
  //Affiche le bonus de score en fonction de la taille de la plateforme
  //Ce code devrait être factorisé...
  if( t_plateformes[0].taille >= 3 ) {
    byte largeur_plateforme = t_plateformes[0].taille;

    if( largeur_plateforme <= 3)
    {
      ecrireEcran("x5", PLATEFORME1_X_COORD - 3,40,BLANC);
    } else
    if( largeur_plateforme >3 && largeur_plateforme <= 5)
    {
      //ecrireEcran("x2", PLATEFORME2_X_COORD - ( (10 - (coordonnees_limites_des_plateformes[0] - PLATEFORME2_X_COORD)) /2 ),40,BLANC);
      ecrireEcran("x3", PLATEFORME1_X_COORD - ( (10 - (t_plateformes[0].coordonnee_limite - PLATEFORME1_X_COORD)) /2 ),40,BLANC);
    } else
    if( largeur_plateforme >5 && largeur_plateforme<= 9)
    {
      ecrireEcran("x1", PLATEFORME1_X_COORD - ( (9 - (t_plateformes[0].coordonnee_limite - PLATEFORME1_X_COORD)) /2 ),40,BLANC);
    }
  }

  if( t_plateformes[1].taille >= 3 ) {
    byte largeur_plateforme = t_plateformes[1].taille;
    
    if( largeur_plateforme <= 3)
    {
      ecrireEcran("x5", PLATEFORME2_X_COORD - 3,40,BLANC);
    } else
    
    if( largeur_plateforme >3 && largeur_plateforme <= 5)
    {
      ecrireEcran("x3", PLATEFORME2_X_COORD - ( (10 - (t_plateformes[1].coordonnee_limite - PLATEFORME2_X_COORD)) /2 ),40,BLANC);
    } else
    
    if( largeur_plateforme >5 && largeur_plateforme<= 9)
    {
      ecrireEcran("x1", PLATEFORME2_X_COORD - ( (9 - (t_plateformes[1].coordonnee_limite - PLATEFORME2_X_COORD)) /2 ),40,BLANC);
    }
  }

  if( t_plateformes[2].taille >= 3 ) {
    byte largeur_plateforme = t_plateformes[2].taille;
    if( largeur_plateforme <= 3)
    {
      ecrireEcran("x5", PLATEFORME3_X_COORD - 3,40,BLANC);
    } else
    if( largeur_plateforme >3 && largeur_plateforme <= 5)
    {
      ecrireEcran("x3", PLATEFORME3_X_COORD - ( (10 - (t_plateformes[2].coordonnee_limite - PLATEFORME3_X_COORD)) /2 ),40,BLANC);
    } else
    if( largeur_plateforme >5 && largeur_plateforme<= 9)
    {
      ecrireEcran("x1", PLATEFORME3_X_COORD - ( (9 - (t_plateformes[2].coordonnee_limite - PLATEFORME3_X_COORD)) /2 ),40,BLANC);
    }
  }
}

void afficheFuel(){
  //Niveau de fuel restant
  drawX11Bitmap(fuel_bits, fuel_width, fuel_height, 0,0);
  if ( quantite_fuel > 0){
    creerRectangle( 18, 4,
                    18+(quantite_fuel * unite_de_fuel),
                    2,
                    1,
                    NOIR );
  } else
  {
    //Beeps et clignotements ?
    //Todo...
  }
}

  //Affiche le lander
void dessineLander(){
  //Un rectangle noir...
  creerRectangle(
    lander_X_position + vitesse_horizontale-1,
    lander_Y_position + vitesse_verticale,
    lander_X_position + vitesse_horizontale + TAILLE_LANDER + 1,
    lander_Y_position + vitesse_verticale + TAILLE_LANDER,
    1,
    NOIR );
    //moins des pixels blancs !

      //Ligne horizontale supérieure
    ligneEcran(
      lander_X_position + vitesse_horizontale,
      lander_Y_position + vitesse_verticale,
      lander_X_position + vitesse_horizontale + TAILLE_LANDER - 1,
      lander_Y_position + vitesse_verticale,
      BLANC);
      //Ligne gauche verticale
    ligneEcran(
      lander_X_position + vitesse_horizontale - 1,
      lander_Y_position + vitesse_verticale + 2,
      lander_X_position + vitesse_horizontale - 1,
      lander_Y_position + vitesse_verticale + 3,
      BLANC);
      //Ligne droite verticale
    ligneEcran(
      lander_X_position + vitesse_horizontale + TAILLE_LANDER,
      lander_Y_position + vitesse_verticale + 2,
      lander_X_position + vitesse_horizontale + TAILLE_LANDER,
      lander_Y_position + vitesse_verticale + 3,
      BLANC);
      //Ligne centrale verticale
    ligneEcran(
      lander_X_position + vitesse_horizontale + 1,
      lander_Y_position + vitesse_verticale + 2,
      lander_X_position + vitesse_horizontale + 1,
      lander_Y_position + vitesse_verticale + 3,
      BLANC);
}

void afficheJeu(){       
  effacerEcran(BLANC);
  afficheFuel(); 
  dessineCrete();
  dessineBonus();
  dessineLander();
  rafraichirEcran();
}

//Largeur et Hauteur de l'image doit être multiple de 2
//Fonction à finaliser pour enlever cette contrainte...
void drawX11Bitmap(unsigned char *data_buffer, byte width, byte height, byte orig_x, byte orig_y){
  unsigned char *byte_to_read = &data_buffer[0];//Pointer to data_buffer
  byte  w_index=0;//Loop index
  byte h_index=0;//...
  int b;//loop index to iterate over bits in a byte

    for(h_index=0; h_index<height;h_index++){
      for(w_index=0; w_index<width-1; w_index+=8, byte_to_read++){//On itère sur des octets, pas des pixels
        for(b=0; b<8; b++){//On parse chacun des bits de l'octet courant
            setPixel(orig_x+b+w_index, orig_y+h_index, (*byte_to_read>>b) & B00000001);
        }
      }
    }
} 

void afficheAccueil() {
  effacerEcran(BLANC);
  drawX11Bitmap(title_bits, title_width, title_height, 9,4);
  drawX11Bitmap(Sound_intro_bits, Sound_intro_width, Sound_intro_height, 14, 31);
  creerRectangle(0,0,83,47,0,NOIR);
  rafraichirEcran();
}

byte detecteTouche() {
  if ( toucheHaut() )   { return TOUCHE_HAUT; }else
  if ( toucheBas() )    { return TOUCHE_BAS; } else
  if ( toucheGauche() ) { return TOUCHE_GAUCHE; }else
  if ( toucheDroite() ) { return TOUCHE_DROITE; }else
  if ( toucheA() )      { return TOUCHE_A; }else
  if ( toucheB() )      { return TOUCHE_B; }else
  return AUCUNE_TOUCHE;
}

byte attenteTouche() {//Avec attente de la relache
  byte touche;
  do {
      touche = detecteTouche();
    } while (touche == AUCUNE_TOUCHE);
    delay(2);
    while (detecteTouche() != AUCUNE_TOUCHE) {};//Attente relache touche
    return touche;
}


void afficheInfos(){
  effacerEcran(BLANC);
  ecrireEcran(" Instructions ", 0, 1, BLANC);
  creerRectangle(0,0,83,47,0,NOIR);

  drawX11Bitmap(A_button_bits,A_button_width, A_button_height, 16, 12);
  drawX11Bitmap(left_button_bits, left_button_width, left_button_height, 3, 22);
  drawX11Bitmap(right_button_bits, right_button_width, right_button_height, 30, 22);
  ligneEcran(18, 32, 24, 32, NOIR);//plateforme
  vertical_offset_deplacement=0;
  horizontal_offset_deplacement=0;
  vitesse_verticale=0;
  vitesse_horizontale=0;
  lander_X_position=20;
  lander_Y_position=24;
  dessineLander();

  creerRectangle(0,39, 83, 47, 1, NOIR);//Zone bas écran
  ligneEcran(42, 8, 42, 39,NOIR);//Separation verticale

  drawX11Bitmap(B_button_bits,A_button_width, A_button_height, 58, 12);
  ecrireEcran("choose", 45, 22, NOIR);
  ecrireEcran("site", 51, 30, NOIR);

  char str[12];
  char t_record[7];
  strcpy(str, "Best:$");
  read_record();
  sprintf(t_record, "%ld", record);
  strcat(str, t_record);
  //On calcule la longueur de la chaine pour la centrer horizontalement
  byte x_pos = ( 84 - (strlen(str) * 6) ) / 2;
  ecrireEcran((const char*) str, x_pos, 40, BLANC);
  rafraichirEcran();

  byte color;
  int anim;
  randomSeed(millis());//Initialisation du générateur aléatoire
  while (detecteTouche()==AUCUNE_TOUCHE) {
    anim=random(0,2);
    if(anim==0) color=BLANC; else color=NOIR;
    ligneEcran(
    lander_X_position + 1,
    lander_Y_position +3,
    lander_X_position + 1,
    lander_Y_position + TAILLE_LANDER+2,
    color );
    if(color==BLANC) creerRectangle(16, 12, 27, 22, 1, BLANC); else drawX11Bitmap(A_button_bits,A_button_width, A_button_height, 16, 12);
    rafraichirEcran();
    int d=random(30,50);
    delay(d);
  }
}

void loopAccueil() {
  afficheAccueil();

  byte choix=attenteTouche();
  if(choix == TOUCHE_A) {
    sound=1;
  }else if(choix == TOUCHE_B) {
    sound=0;
  } else {
    sound=1;
  }

  afficheInfos();
  attenteTouche();
  modeJeu = ANNONCE_NIVEAU;
}

void animationPoussee(){
    ligneEcran(
    lander_X_position + vitesse_horizontale + 1,
    lander_Y_position + vitesse_verticale+3,
    lander_X_position + vitesse_horizontale + 1,
    lander_Y_position + vitesse_verticale + TAILLE_LANDER+2,
    NOIR );
    rafraichirEcran();
}

void modifieJeu(){
    vertical_offset_deplacement += 0.01;//On ne fait rien => le lander descend. 
    
    switch ( detecteTouche() ) {
      case TOUCHE_A:
      {
        if(quantite_fuel - t_niveau[niveau].consommation_fuel > 0) {
          quantite_fuel-= t_niveau[niveau].consommation_fuel;
          vertical_offset_deplacement -= 0.03;
          animationPoussee();
        }
        //vitesse_verticale = vitesse_verticale - vertical_offset_deplacement;
        //On ajoute 0.03 car 0.01 va être oté après ce switch pour le faire descendre si on n'appuie sur rien.
        //Or on veut que le lander monte, pas qu'il annule simplement la descente,
        //(ce qui le ferait simplement rester sur place en appyuant sur 'A').
        break;
      }
      case TOUCHE_GAUCHE:
      {
        if(quantite_fuel- t_niveau[niveau].consommation_fuel > 0) {
          quantite_fuel-= t_niveau[niveau].consommation_fuel;
          horizontal_offset_deplacement -= +0.01;
        }
        break;
      }
      case TOUCHE_DROITE:
      {
        if(quantite_fuel - t_niveau[niveau].consommation_fuel > 0) {
          quantite_fuel-= t_niveau[niveau].consommation_fuel;
          horizontal_offset_deplacement += +0.01;
        }
        break;
      }
      default:;
    }

    vitesse_horizontale = vitesse_horizontale + horizontal_offset_deplacement;  
    vitesse_verticale = vitesse_verticale + vertical_offset_deplacement;

    if(vertical_offset_deplacement >= vitesse_landing_max) {
      LEDRouge(1);
    } else
    {
      LEDRouge(0);
    }
}

bool testAtterissage(byte index_min) {
  if( (//le lander se pose entre x_min et x_max d'une des 3 plateformes...
    ( t_plateformes[0].taille >= 3 && index_min >= PLATEFORME1_X_COORD && ( (index_min + TAILLE_LANDER) <= t_plateformes[0].coordonnee_limite) ) ||
    ( t_plateformes[1].taille >= 3 && index_min >= PLATEFORME2_X_COORD && ( (index_min + TAILLE_LANDER) <= t_plateformes[1].coordonnee_limite) ) ||
    ( t_plateformes[2].taille >= 3 && index_min >= PLATEFORME3_X_COORD && ( (index_min + TAILLE_LANDER) <= t_plateformes[2].coordonnee_limite) ))&&
    ( vertical_offset_deplacement <= vitesse_landing_max) ) {//ET à une vitesse verticale inférieure à vitesse_landing_max, alors OK
    return true;
  }
  return false;
}

byte testContact() { //0 si pas de contact, 1 pour Landing OK ou 2 pour crash
  byte index_crete = (byte) lander_X_position + vitesse_horizontale;
  byte altitude_du_lander = lander_Y_position + vitesse_verticale + TAILLE_LANDER;
  
  byte contact = 0;
  for( int i=0; i < TAILLE_LANDER; i++) {//On teste chacun des pixels de la base du lander
    if( (altitude_du_lander) >= crete[index_crete + i] ) {// On touche la crête...
      testAtterissage(index_crete)?contact=1:contact=2;//Atterissage réussi ou crash ?
    }
  }
  return contact;
}

void ecrireCentre(char* str, byte y_pos, byte color){
  byte x_pos = ( 84 - (strlen(str) * 6) ) / 2;
  ecrireEcran((const char*) str, x_pos, y_pos, color);
}

void  confirmSave(){
  effacerEcran(BLANC);
  creerRectangle(0,0,83,47,0,NOIR);
  drawX11Bitmap(DOLLARSmall_bits, DOLLARSmall_width, DOLLARSmall_height, 2, 4);
  creerRectangle(0, 0, 17, 19, 0,NOIR);
  ligneEcran(0, 19, 83, 19, 1);
  ecrireEcran("Take cash", 20, 3, NOIR);
  ecrireEcran("and leave?", 20, 10, NOIR);
  drawX11Bitmap(A_button_bits,A_button_width, A_button_height, 10, 26);
  drawX11Bitmap(B_button_bits,B_button_width, B_button_height, 10, 36);
  ecrireEcran("Yes!",23,27,NOIR);
  ecrireEcran("No...",23,37,NOIR);
  rafraichirEcran();

  byte choix=attenteTouche();
  if(choix == TOUCHE_A) {
    choix=1;
    saveScore();
    modeJeu=ACCUEIL;
  }else
  if(choix == TOUCHE_B) {
    choix=1;
    modeJeu=ANNONCE_NIVEAU;
  }

  setContrast(60);
  lumiereEcran(255);
} 

void animationCrash(){
  effacerEcran(BLANC);
  drawX11Bitmap(BoomGood_bits, BoomGood_width, BoomGood_height, 20, 13);
  rafraichirEcran();
  if(sound==1) {frequenceBuzzer(200,150);}
  delay(100);
  if(sound==1) {frequenceBuzzer(200,100);}
  delay(100);
  if(sound==1) {frequenceBuzzer(100,800);}
  LEDRouge(0);
  
  long f=600;
  for(byte l=60; l>0; l--, f-=20){
    lumiereEcran(l);
    setContrast(l-(byte) (60/255));
    rafraichirEcran();
    delay(6);
    if(sound==1) frequenceBuzzer(600+f, 6);
  }
  

  attenteTouche();
  effacerEcran(BLANC);
  drawX11Bitmap(DIEGood_bits, DIEGood_width, DIEGood_height, 4, 4);

  ecrireEcran("You died", 18, 18, NOIR);
  ecrireEcran("a little bit", 6, 27, NOIR);
  ecrireEcran("richer!", 21, 36, NOIR);

  creerRectangle(0,0,83,47,0,NOIR);

  rafraichirEcran();
  setContrast(60);
  lumiereEcran(255);
  attenteTouche();
  modeJeu = ACCUEIL;
} 

char*  infoScore(){
  //On construit la chaine a afficher
  char str[14];
  char *p_str = str;
  char s_bonus[3];
  char s_score[4];

  sprintf(str, "%d", t_niveau[niveau].score);
  strcat(str, (const char*)"x");
  sprintf(s_bonus, "%d", t_plateformes[identifie_plateforme()].facteur_bonus);
  strcat(str,  s_bonus);
  strcat(str, (const char*)"=$");
  sprintf(s_score, "%d", t_niveau[niveau].score * t_plateformes[identifie_plateforme()].facteur_bonus);
  strcat(str, s_score);
  p_str=str;
  return p_str;
}

void wait(long time) {
  for(long i=0; i<time; i++){

  }
}
void animationLanding(){
  effacerEcran(BLANC);
  drawX11Bitmap(bravo_bits, bravo_width, bravo_height, 16, 4);
  dessineCrete();

  lander_Y_position -=1;//Pour recaler après test collision et afficher correctement le lander
  dessineLander();

  const char *str=infoScore();
  //On calcule la longueur de la chaine pour la centrer horizontalement
  byte x_pos = ( 84 - (strlen(str) * 6) ) / 2;
  ecrireEcran((const char*) str, x_pos, 40, BLANC);
  rafraichirEcran();
  if(sound==1) frequenceBuzzer(300,100);
  delay(100);
  if(sound==1) frequenceBuzzer(300,100);
  if(sound==1) frequenceBuzzer(600,400);

  LEDVerte(1);
  for(int i=0;i<2;i++){
    ecrireEcran((const char*) str, x_pos, 40, BLANC);
    rafraichirEcran();
    delay(400);
    dessineCrete();
    rafraichirEcran();
    delay(100);
  } 
  ecrireEcran((const char*) str, x_pos, 40, BLANC);
  rafraichirEcran();
  LEDVerte(0);
  attenteTouche();
  while(detecteTouche() != AUCUNE_TOUCHE){};
}

 void read_record()
{
//Read the 4 bytes from the eeprom memory.
long four = EEPROM.read(0);
long three = EEPROM.read(0 + 1);
long two = EEPROM.read(0 + 2);
long one = EEPROM.read(0 + 3);
//Return the recomposed long by using bitshift.
record = ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void alert_new_record(){
  effacerEcran(BLANC);
  creerRectangle(0,0,83,22,1,NOIR);
  creerRectangle(2,2,80,20,0,BLANC);
  ecrireEcran("NEW RECORD!",11,4, BLANC);

  char str[12];
  char t_record[7];
  strcpy(str, "$");
  sprintf(t_record, "%ld", score);
  strcat(str, t_record);
  //On calcule la longueur de la chaine pour la centrer horizontalement
  byte x_pos = ( 84 - (strlen(str) * 6) ) / 2;
  ecrireEcran((const char*) str, x_pos, 12, BLANC);

  drawX11Bitmap(A_button_bits,A_button_width, A_button_height, 10, 26);
  drawX11Bitmap(B_button_bits,B_button_width, B_button_height, 10, 36);
  ecrireEcran("Continue",23,27,NOIR);
  ecrireEcran("$$$&Stop",23,37,NOIR);

  byte CLIGNOTANT=0;
  byte choix;
  do {
    choix=3;// no choix

    if (((millis()/500)%2)==0) {
      ecrireEcran("NEW RECORD!",11,4,CLIGNOTANT);
      if(sound==1) frequenceBuzzer(300,8);
    }
    else {
      creerRectangle(3,3,78,10,1,NOIR);
    }

    if(detecteTouche() == TOUCHE_A) {
      //Attente relache touche
      do { delay(1);} while(detecteTouche() != AUCUNE_TOUCHE);
      choix=1;
      modeJeu=ANNONCE_NIVEAU;
    }else
    if(detecteTouche() == TOUCHE_B) {
      do { delay(1);} while(detecteTouche() != AUCUNE_TOUCHE);
      choix=1;
      confirmSave();
    }
    rafraichirEcran();
  } while (choix==3);
}

int identifie_plateforme(){
  byte index_plateforme = -1;
  
  int position_en_x = lander_X_position + vitesse_horizontale;
  if( position_en_x >= PLATEFORME1_X_COORD && position_en_x < PLATEFORME2_X_COORD) { index_plateforme = 0;}
  else if ( position_en_x >= PLATEFORME2_X_COORD && position_en_x < PLATEFORME3_X_COORD) { index_plateforme = 1;}
  else if ( position_en_x >= PLATEFORME3_X_COORD ) { index_plateforme = 2;}
  return index_plateforme;
}

void affiche_score(){//Devrait être factorisé: affichage du score, du record et le centrage 
  char str[12];
  char t_record[7];
  char t_score[7];

  effacerEcran(BLANC);
  creerRectangle(0,0,83,47,0,NOIR);//Cadre écran
  creerRectangle(0,0,83,14,1,NOIR);
  ecrireEcran("  You gained  ", 0, 4, BLANC);

  drawX11Bitmap(arrow_bits, arrow_width, arrow_height, 35, 15);
  //Centrer le score
  strcpy(str, "$");
  sprintf(t_score, "%ld", score);
  strcat(str, t_score);
  //On calcule la longueur de la chaine pour la centrer horizontalement
  byte x_pos = ( 84 - (strlen(str) * 6) ) / 2;
  ecrireEcran((const char*) str, x_pos, 26, NOIR);

  creerRectangle(0,39, 83, 47, 1, NOIR);//Zone bas écran

  strcpy(str, "Best:$");
  read_record();
  sprintf(t_record, "%ld", record);
  strcat(str, t_record);
  //On calcule la longueur de la chaine pour la centrer horizontalement
  x_pos = ( 84 - (strlen(str) * 6) ) / 2;
  ecrireEcran((const char*) str, x_pos, 40, BLANC);
  rafraichirEcran();
}

void afficheRecord(){

    ecrireLettre('0'+record%10,30,25,NOIR);
  if (record>=10) ecrireLettre('0'+(record/10)%10,24,25,NOIR);
  if (record>=100) ecrireLettre('0'+(record/100)%10,18,25,NOIR);
  if (record>=1000) ecrireLettre('0'+(record/1000) %10,12,25,NOIR);
  if (record>=10000) ecrireLettre('0'+(record/10000)%10,6,25,NOIR);
  if (record>=100000) ecrireLettre('0'+(record/100000)%10,0,25,NOIR);
}

void test_collision(){
  byte contact = testContact();
  switch ( contact ) {
    case 1://Atterissage réussi
      ++nb_atterissages;
      score+=t_niveau[niveau].score * t_plateformes[identifie_plateforme()].facteur_bonus;
      
      animationLanding();//Affiche success
      alerte_niveau=0;//Flag pour annoncer le niveau une seule fois.

      if(score > record) {
        alert_new_record();
      } else {
      affiche_score();
      rafraichirEcran();
      attenteTouche();
      modeJeu = ANNONCE_NIVEAU;}
      break;

    case 2://Crash
      animationCrash();
      nb_atterissages=0;//=> le score sera réinitalisé au prochain lancement du jeu dans initPartie()
      initPartie();
      modeJeu=ACCUEIL;
      break;
    default:;
  }
}

void loopPartie() {
  test_collision();
  modifieJeu();
  afficheJeu();
}

int calculeGain(){
  byte bonus=0;
  //get max of bonus in plateforms

bonus = t_plateformes[0].facteur_bonus;
  for(int i=0; i<3; i++){
    if(t_plateformes[i].facteur_bonus > bonus) {
      bonus = t_plateformes[i].facteur_bonus;
    }
  }
return bonus * t_niveau[niveau].score;
}

void annonce_niveau(){
  int gain_max=0;
  gestion_niveau(nb_atterissages);

  effacerEcran(BLANC);
  initPartie();
  creerRectangle(0,0,83,10,1,NOIR);
  drawX11Bitmap(level_bits, level_width, level_height, 2, 5);
  drawX11Bitmap(max_bits, max_width, max_height, 34, 5);
  drawX11Bitmap(arrow_bits, arrow_width, arrow_height, 34, 11);

  //drawX11Bitmap(annonce_top_bits, annonce_top_width, annonce_top_height, 0, 0);
  ecrireLettre('0'+niveau,20,2,BLANC);
  dessineCrete();
  gain_max = calculeGain();

  ecrireLettre('$',53,2,BLANC);
  if(gain_max<1000) {
    ecrireLettre('0'+gain_max%10,71,2,BLANC);
    if (gain_max>=10) ecrireLettre('0'+(gain_max/10)%10,65,2,BLANC);
    if (gain_max>=100) ecrireLettre('0'+(gain_max/100)%10,59,2,BLANC);
  } else {
    ecrireLettre('0'+gain_max%10,77,2,BLANC);
    if (gain_max>=10) ecrireLettre('0'+(gain_max/10)%10,71,2,BLANC);
    if (gain_max>=100) ecrireLettre('0'+(gain_max/100)%10,65,2,BLANC);
    if (gain_max>=1000) ecrireLettre('0'+(gain_max/1000) %10,59,2,BLANC);
  }

  drawX11Bitmap(annonce_bottom_bits, annonce_bottom_width, annonce_bottom_height, 0, 38);
  rafraichirEcran();

  switch (attenteTouche()) {
    case TOUCHE_A:
      {
        modeJeu=PARTIE;
        break;
      }
    case TOUCHE_B:
      {
        modeJeu=ANNONCE_NIVEAU;
      }
    default:;
  }

}

// Loop est la boucle principale, va se lancer en boucle après Setup
void loop() {
  switch (modeJeu) {
    case ACCUEIL:
      loopAccueil();
      break;
    case ANNONCE_NIVEAU:
      annonce_niveau();
      break;
    case PARTIE:
      loopPartie();
      break;
    default:
        break; 
  }
}
