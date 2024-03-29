# Batak Remastered
## Projet Batak Machine
Ce projet se base sur le [projet batak machine](https://tutos.ouiaremakers.com/posts/tutoriel-diy-batak-machine-l-entrainement-aux-reflexes) de [quentingard](https://tutos.ouiaremakers.com/users/quentingard).

Le but de la Batak Machine est l'entrainement aux reflexes ( principalement utilisé dans les sports mécaniques comme la F1 ou autre ).

## Les trucs Utiles
### Les Circuits imprimés

à venir

### Le Code

Le code est disponible [ICI](https://github.com/Creanics/batakRemastered/code)

## Les différences avec le code d'origine

- Les entrées sont utilisée en "INPUT_PULLUP" au lieu de "INPUT"
  - Conséquences
    - Les boutons sont "alimentés" avec un fil GND à la place d'un fil +5V
    - La carte des entrées n'a plus d'utilité, elle résolvait le problème du rebond des bouton ( maintenant résolu par le mode "INPUT_PULLUP" )
- Les numéros des pins utilisés sont modifiés
  - Conséquences
    - Câblage simplifié sur les circuit imprimés

## Les différences avec le Hardware d'origine
- Suppression de la carte des entrées
  - Raison: utilisation du mode "INPUT_PULLUP"
  - Conséquences
    - Pas besoin des condensateurs
    - Pas besoin des résistances 100Ohms
    - Pas besoin des résistances 10KOhms

- Création d'une carte des sorties
  - Aucune différence ( à part que c'est plus propre 😉 
  )

- Création d'une carte de connexions
  - Raison
    - Simplicité de câblage
    - Utilisation de conecteurs 4fils ( 2 pour la led + 2 pour le bouton )

- Création d'une carte pour l'Arduino
  - Raison
    - Centralisation des composants "simple" ( buzzer, alimentation... )
    - Câblage facile de l'arduino

Raisons de la création de (plusieurs) PCB :
- Câblage plus facile
- Assemblage plus joli 😉
- Réparation facilité
  - En comparaison de faire un seul gros PCB ( si une carte casse pas besoin de tout remplacer... )