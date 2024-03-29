// Declaration des variables

char menu = 0;
boolean testleds = 0;
long temps; //t0 stockage des millisecondes du début de la partie (référence du temps)
long temps2; //variable pour vérifier si 2 secondes passent entre chaque appui de bouton
long temps3 = 0; //durée de la partie jusqu'a maintenant en secondes
boolean temps4 = 0; //0 si 30 secondes 1 si 60 secondes
long temps5; //durée de la partie 30 ou 60 secondes
long temps6; //3 secondes avant la fin de la partie
long temps7; //variable pour tone a chaque secondes quand il en reste moins de 3
boolean x; //inconnue à trouver
boolean niveau; //0 Si facile et 1 si difficile
char niveau2; //le nombre de boutons (12 ou 8) en fonction de la difficulté (1 ou 0)
byte bestScore;
byte bestScore30lv1; //Meilleur score 30 secondes 8 boutons
byte bestScore60lv1; //Meilleur score 60 secondes 8 boutons
byte bestScore30lv2; //Meilleur score 30 secondes 12 boutons
byte bestScore60lv2; //Meilleur score 60 secondes 12 boutons
char alea; //numéro du bouton aléatoire qui s'allume
long score; //score

int Son[] = {
  262,
  294,
  330,
  349,
  392,
  440,
  494,
  523,
  587,
  659,
  698,
  784
}; // notes pour musique de début

// Declaration des sorties

const int buzzer = 34; //haut parleur à la pin 34
char led[] = {
  22,
  23,
  24,
  25,
  26,
  27,
  28,
  29,
  30,
  31,
  32,
  33 
}; // Pins des Leds respectivement de 1 a 12
char led1 = 22; //led du bouton niveau
char led2 = 23; //led du bouton start
char led3 = 24; //led du bouton temps

// Declaration des entrees

char bouton[] = {
  42,
  43,
  44,
  45,
  46,
  47,
  48,
  49,
  50,
  51,
  52,
  53
}; // Pins des boutons respectivement de 1 a 12
char boutonNiveau = 42; // bouton niveau
char boutonStart = 43; // bouton start
char boutonTemps = 44; // bouton temps

// Declaration des notes pour musique best score

unsigned int C6 = 1047;
unsigned int Ab5 = 831;
unsigned int Bb5 = 932;
unsigned int G5 = 784;
unsigned int F5 = 698;
unsigned int E5 = 659;
unsigned int Eb5 = 622;
unsigned int D5 = 587;
unsigned int Db5 = 554;
unsigned int C5 = 523;
unsigned int B4 = 494;
unsigned int Bb4 = 466;
unsigned int nA4 = 440;
unsigned int Ab4 = 415;
unsigned int G4 = 392;
unsigned int Gb4 = 370;
unsigned int F4 = 349;
unsigned int E4 = 330;
unsigned int Eb4 = 311;
unsigned int D4 = 294;
unsigned int Db4 = 277;
unsigned int C4 = 262;
unsigned int B3 = 247;
unsigned int Bb3 = 233;
unsigned int nA3 = 220;
unsigned int GS3 = 208;
unsigned int G3 = 196;
unsigned int Gb3 = 185;
unsigned int F3 = 175;
unsigned int E3 = 165;
char speakerPin = 34;

// Inclusion des librairies

#include <EEPROM.h> // ajout de la librairie EEPROM pour sauvegarder les meilleurs scores

#include <Wire.h>  // ajout de la librairie I2C pour communiquer avec le LCD

#include <LiquidCrystal_I2C.h> // ajout de la librairie LCD

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Adresse I2C 0x27 a remplacer par la votre si differente

void setup() {
  // Initialisation des entree sorties
  for (int i = 0; i < 12; i++) {
    pinMode(led[i], OUTPUT);
    pinMode(bouton[i], INPUT_PULLUP);
    digitalWrite(led[i], LOW);
  }

  lcd.begin(20, 4); // Initialisation du LCD
  randomSeed(analogRead(0)); // Initialisation de la graine aléatoire utilisée
  niveau = 0; //mode facile (8 boutons) par défaut
  pinMode(buzzer, OUTPUT); //initialisation du haut parleur en tant que sortie

  // Animation a l'allumage

  //on allume toutes les leds une par une avec un bruit
  for (char i = 0; i < sizeof(led); i++) {
    digitalWrite(led[i], HIGH);
    tone(buzzer, Son[i]);
    delay(300);
  }
  // on coupe le son et on attend un peu
  noTone(buzzer);
  delay(150);
  // puis on les éteint de même, mais plus rapidement
  for (char i = sizeof(led); i > 0; i--) {
    tone(buzzer, Son[i - 1]);
    delay(150);
    digitalWrite(led[i - 1], LOW);
  }
  // ainsi que le son
  noTone(buzzer);
}

void loop() {
  x = 0;
  temps3 = 0;
  temps6 = 3;
  // Lecture des meilleurs scores
  bestScore30lv1 = EEPROM.read(0);
  bestScore60lv1 = EEPROM.read(1);
  bestScore30lv2 = EEPROM.read(2);
  bestScore60lv2 = EEPROM.read(3);

  // Alumages des 3 boutons du menu
  digitalWrite(led2, HIGH);
  digitalWrite(led1, HIGH);
  digitalWrite(led3, HIGH);

  //tant que on n'a pas démarré la partie (appuyé sur start)
  while (digitalRead(boutonStart) == HIGH) {
    if (digitalRead(bouton[11]) == LOW && digitalRead(bouton[10]) == LOW) {
      modeusine();
    } // Si 11 et 12 apuyés passer en menu maintenance

    // Affichage du menu sur LCD
    lcd.setCursor(0, 0);
    lcd.print("Press Start to begin");
    lcd.setCursor(0, 1);
    if (niveau == 0) {
      lcd.print("Facile 8 boutons");
    } else {
      lcd.print("Difficile 12 boutons");
    }
    lcd.setCursor(0, 2);
    lcd.print("Duree:");
    lcd.print(temps5);
    lcd.print(" Secondes");
    lcd.setCursor(0, 3);
    lcd.print("Score:");
    lcd.print(score);
    lcd.print(" ");
    lcd.print("Best:");
    lcd.print(bestScore);
    lcd.print("  ");

    //Changer la Durée
    if (digitalRead(boutonTemps) == LOW) {
      temps4 = !temps4;
      while (digitalRead(boutonTemps) == LOW) {}
    }

    //changer la difficulté (nombre de boutons)
    if (digitalRead(boutonNiveau) == LOW) {
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      niveau = !niveau;
      while (digitalRead(boutonNiveau) == LOW) {}
    }
    //Changer niveau2(nombre de boutons) en fonction de niveau (difficulté)
    if (niveau == 1) {
      niveau2 = 12;
    } else {
      niveau2 = 8;
    }

    //Changer temps5(durée) en fonction de temps4 (0 si 30 sec et 1 si 60 sec)
    if (temps4 == 1) {
      temps5 = 60;
    } else {
      temps5 = 30;
    }

    //affichage du Meilleur score en fonction du mode choisi
    if (temps4 == 0 && niveau == 0) {
      bestScore = bestScore30lv1;
    }
    if (temps4 == 1 && niveau == 0) {
      bestScore = bestScore60lv1;
    }
    if (temps4 == 0 && niveau == 1) {
      bestScore = bestScore30lv2;
    }
    if (temps4 == 1 && niveau == 1) {
      bestScore = bestScore60lv2;
    }
  }

  //La partie commence
  lcd.clear();
  //On éteint les leds du menu
  digitalWrite(led2, LOW);
  digitalWrite(led1, LOW);
  digitalWrite(led3, LOW);

  //décompte au démarage
  for (int i = 0; i < 3; i++) {
    tone(buzzer, 1000);
    delay(200);
    for (int i = 0; i < niveau2; i++) {
      digitalWrite(led[i], HIGH);
    }
    noTone(buzzer);
    delay(800);
    for (int i = 0; i < niveau2; i++) {
      digitalWrite(led[i], LOW);
    }
  }
  tone(buzzer, 2500);
  delay(1000);
  noTone(buzzer);
  //fin décompte au démarage
  //initialisation du temps
  temps = millis(); //on sauvegarde la référence de temps
  temps2 = millis();
  x = 0;
  score = 0;
  while ((millis() - temps) < (1000 * temps5)) { //on regarde si la durée de la partie est écoulé
    alea = random(0, niveau2); //on choisi un numéro aléatoire de bouton à allumer entre 0 et le nombre de boutons total
    digitalWrite(led[alea], HIGH); //on allume la led correspondant au bouton choisi aléatoirement
    //tant que l'on à appuyé sur aucun bouton et que 2 secondes ne sont pas passées
    while (digitalRead(bouton[0]) == HIGH && digitalRead(bouton[1]) == HIGH && digitalRead(bouton[2]) == HIGH && digitalRead(bouton[3]) == HIGH && digitalRead(bouton[4]) == HIGH && digitalRead(bouton[5]) == HIGH && digitalRead(bouton[6]) == HIGH && digitalRead(bouton[7]) == HIGH && digitalRead(bouton[8]) == HIGH && digitalRead(bouton[9]) == HIGH && digitalRead(bouton[10]) == HIGH && digitalRead(bouton[11]) == HIGH && (temps2 + 2000) > millis()) {
      //Le lcd affiche le score et le temps restant
      //on affiche le score
      lcd.setCursor(0, 0);
      lcd.print("Score=");
      lcd.print(score);
      //on affiche le temps restant
      lcd.setCursor(0, 1);
      lcd.print("Temps Restant:");
      if (millis() - temps - (1000 * temps3) > 1000) { //si une seconde est passé
        temps3++; //on augmente la durée de la partie jusqu'à maintenant d'une seconde
        lcd.print("   "); //on clear l'ancienne valeur de temps restant
        lcd.setCursor(14, 1); //on se place derriere les deux points de "Temps Restant:"
        lcd.print(temps5 - temps3); //on affiche la nouvelle valeur du temps restant (temps total - temps écoulé)
      }

      //décompte si il reste moins de 3 secondes (buzz toutes les secondes)
      if ((millis() - temps) > (1000 * temps5 - (temps6 * 1000))) {
        temps6--;
        tone(buzzer, 1000);
        temps7 = millis(); //variable pour la durée du buzz (300ms)
      }
      if (millis() - temps7 > 300) {
        noTone(buzzer);
      } //au bout de 300ms on arrete le buzz
    } //fin du temps que l'on à appuyé sur aucun bouton et que 2 secondes ne sont pas passées

    if (digitalRead(bouton[0]) == LOW || digitalRead(bouton[1]) == LOW || digitalRead(bouton[2]) == LOW || digitalRead(bouton[3]) == LOW || digitalRead(bouton[4]) == LOW || digitalRead(bouton[5]) == LOW || digitalRead(bouton[6]) == LOW || digitalRead(bouton[7]) == LOW || digitalRead(bouton[8]) == LOW || digitalRead(bouton[9]) == LOW || digitalRead(bouton[10]) == LOW || digitalRead(bouton[11]) == LOW) {
      //Si on a appuyé sur un bouton
      if (digitalRead(bouton[alea]) == LOW) { //et que le bouton appuyé est le bon
        score++;
      }
    }
    while (digitalRead(bouton[0]) == LOW || digitalRead(bouton[1]) == LOW || digitalRead(bouton[2]) == LOW || digitalRead(bouton[3]) == LOW || digitalRead(bouton[4]) == LOW || digitalRead(bouton[5]) == LOW || digitalRead(bouton[6]) == LOW || digitalRead(bouton[7]) == LOW || digitalRead(bouton[8]) == LOW || digitalRead(bouton[9]) == LOW || digitalRead(bouton[10]) == LOW || digitalRead(bouton[11]) == LOW) {
      //tant que on n'a pas lâché le bouton (pour ne pas bloquer l'affichage du temps et pour rester coincé dans une boucle pour ne pas faire un score infini)
      //buzz si 3 secondes restantes
      if ((millis() - temps) > (1000 * temps5 - (temps6 * 1000))) {
        temps6--;
        tone(buzzer, 1000);
        temps7 = millis();
      }
      if (millis() - temps7 > 300) {
        noTone(buzzer);
      }
      //affichage du score et du temps restant
      lcd.setCursor(0, 0);
      lcd.print("Score=");
      lcd.print(score);
      lcd.setCursor(0, 1);
      lcd.print("Temps Restant:");
      if (millis() - temps - (1000 * temps3) > 1000) {
        temps3++;
        lcd.print("   ");
        lcd.setCursor(14, 1);
        lcd.print(temps5 - temps3);
      }
    } //quand on a enfin lache le bouton
    //buzz si 3 secondes restantes
    if ((millis() - temps) > (1000 * temps5 - (temps6 * 1000))) {
      temps6--;
      tone(buzzer, 1000);
      temps7 = millis();
    }
    if (millis() - temps7 > 300) {
      noTone(buzzer);
    }
    //affichage du score et du temps restant
    lcd.setCursor(0, 0);
    lcd.print("Score=");
    lcd.print(score);
    lcd.setCursor(0, 1);
    lcd.print("Temps Restant:");
    if (millis() - temps - (1000 * temps3) > 1000) {
      temps3++;
      lcd.print("   ");
      lcd.setCursor(14, 1);
      lcd.print(temps5 - temps3);
    }
    temps2 = millis(); //on sauvegarde la référence du temps par rapport aux 2 secondes maximum entre 2 appuis de bouton
    digitalWrite(led[alea], LOW); //on éteint la led du bouton qu'il fallait appuyer
  } //fin de la partie

  //animation fin
  for (char i = 0; i < 12; i++) { //on allume toutes les leds
    digitalWrite(led[i], HIGH);
  }
  // puis on les éteint une à une en jouant le son associé
  for (char i = 12; i > 0; i--) {
    tone(buzzer, Son[i - 1]);
    delay(150);
    digitalWrite(led[i - 1], LOW);
  }
  noTone(buzzer); // et on coupe le son
  temps2 = millis();
  digitalWrite(led[alea], LOW);
  if (x == 0) {
    lcd.clear();
    //incrémentation du score si c'est le meilleur
    if (score > bestScore) {
      bestScore = score;
      if (temps4 == 0 && niveau == 0) {
        EEPROM.write(0, bestScore);
      }
      if (temps4 == 1 && niveau == 0) {
        EEPROM.write(1, bestScore);
      }
      if (temps4 == 0 && niveau == 1) {
        EEPROM.write(2, bestScore);
      }
      if (temps4 == 1 && niveau == 1) {
        EEPROM.write(3, bestScore);
      }
      musiquebest(); //joue la musique du meilleur score
    }

  }
  lcd.print("Score=");
  lcd.print(score);
  lcd.setCursor(0, 1);
  lcd.print("Best Score=");
  lcd.print(bestScore);
  x = !x;
  delay(2500);
  lcd.clear();
} //fin de la boucle loop on revient au menu

//fait à la fin et à ce moment là j'ai compris l'utilité des fonctions

void modeusine() {
  while (digitalRead(bouton[11]) == LOW && digitalRead(bouton[10]) == LOW) {}
  digitalWrite(led[0], LOW);
  digitalWrite(led[1], LOW);
  digitalWrite(led[2], LOW);
  while (digitalRead(bouton[11]) == HIGH || digitalRead(bouton[10]) == HIGH) {
    if (digitalRead(bouton[2]) == LOW) {
      menu++;
      while (digitalRead(bouton[2]) == LOW) {}
    }
    if (menu >= 2) {
      menu = 0;
    }
    if (menu == 0) {
      lcd.setCursor(0, 0);
      lcd.print("Bouton1:On/Off leds ");
      lcd.setCursor(0, 1);
      lcd.print("Bouton2:test boutons");
      lcd.setCursor(0, 2);
      lcd.print("Bouton4:test HP     ");
      lcd.setCursor(0, 3);
      lcd.print("Bouton5:Raz scores  ");
      if (digitalRead(bouton[0]) == LOW) {
        while (digitalRead(bouton[0]) == LOW) {}
        allumetout();
      }
      if (digitalRead(bouton[1]) == LOW) {
        while (digitalRead(bouton[1]) == LOW) {}
        testboutons();
      }
      if (digitalRead(bouton[3]) == LOW) {
        while (digitalRead(bouton[3]) == LOW) {}
        musiquebest();
      }
      if (digitalRead(bouton[4]) == LOW) {
        while (digitalRead(bouton[4]) == LOW) {}
        EEPROM.write(0, 0);
        EEPROM.write(1, 0);
        EEPROM.write(2, 0);
        EEPROM.write(3, 0);
      }
    }
    if (menu == 1) {
      lcd.setCursor(0, 0);
      lcd.print("Bouton1:Reset 8b30s ");
      lcd.setCursor(0, 1);
      lcd.print("Bouton2:Reset 8b60s ");
      lcd.setCursor(0, 2);
      lcd.print("Bouton4:Reset 12b30s");
      lcd.setCursor(0, 3);
      lcd.print("Bouton5:Reset 12b60s");
      if (digitalRead(bouton[0]) == LOW) {
        while (digitalRead(bouton[0]) == LOW) {}
        EEPROM.write(0, 0);
      }
      if (digitalRead(bouton[1]) == LOW) {
        while (digitalRead(bouton[1]) == LOW) {}
        EEPROM.write(1, 0);
      }
      if (digitalRead(bouton[3]) == LOW) {
        while (digitalRead(bouton[3]) == LOW) {}
        EEPROM.write(2, 0);
      }
      if (digitalRead(bouton[4]) == LOW) {
        while (digitalRead(bouton[4]) == LOW) {}
        EEPROM.write(3, 0);
      }

    }
  }
  while (digitalRead(bouton[11]) == LOW && digitalRead(bouton[10]) == LOW) {}
  digitalWrite(led2, HIGH);
  digitalWrite(led1, HIGH);
  digitalWrite(led3, HIGH);
  lcd.clear();
}
void testboutons() {
  if (testleds == 1) {
    for (int i = 0; i < 12; i++) {
      digitalWrite(led[i], LOW);
    }
    testleds = !testleds;
  }
  for (int i = 0; i < 12; i++) {
    digitalWrite(led[i], HIGH);
    while (digitalRead(bouton[i]) == HIGH) {}
    digitalWrite(led[i], LOW);
  }
}
void allumetout() {
  testleds = !testleds;
  if (testleds == 1) {
    for (int i = 0; i < 12; i++) {
      digitalWrite(led[i], HIGH);
    }
  }
  if (testleds == 0) {
    for (int i = 0; i < 12; i++) {
      digitalWrite(led[i], LOW);
    }
  }
}
void musiquebest() {
  buzz(speakerPin, G3, 166);
  buzz(speakerPin, C4, 166);
  buzz(speakerPin, E4, 166);
  buzz(speakerPin, G4, 166);
  buzz(speakerPin, C5, 166);
  buzz(speakerPin, E5, 166);
  buzz(speakerPin, G5, 500);
  buzz(speakerPin, E5, 500);
  buzz(speakerPin, E3, 166);
  buzz(speakerPin, C4, 166);
  buzz(speakerPin, Eb4, 166);
  buzz(speakerPin, Ab4, 166);
  buzz(speakerPin, C5, 166);
  buzz(speakerPin, Eb5, 166);
  buzz(speakerPin, Ab5, 500);
  buzz(speakerPin, Eb5, 500);
  buzz(speakerPin, Bb3, 166);
  buzz(speakerPin, D4, 166);
  buzz(speakerPin, F4, 166);
  buzz(speakerPin, Bb4, 166);
  buzz(speakerPin, D5, 166);
  buzz(speakerPin, F5, 166);
  buzz(speakerPin, Bb5, 500);
  delay(50);
  buzz(speakerPin, Bb5, 166);
  delay(50);
  buzz(speakerPin, Bb5, 166);
  delay(50);
  buzz(speakerPin, Bb5, 166);
  buzz(speakerPin, C6, 1300);
}
void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
}