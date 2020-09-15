## Quel est ce projet ?
Ce projet consiste en la réalisation d'une course de vitesse et "d'intelligence" réalisée uniquement par un algorithme. En effet, un algorithme écrit en C# va communiquer avec le serveur pour ralier le plus rapidement possible différents checkpoints. Attention ces checkpoints sont d'une taille variable. Et chaque case de ce checkpoint aura une valeur. <br>
Le but du jeu ? Arriver au dernier checkpoint avec le moins de points possible. Un déplacement est égal à 1 point et les cases peuvent donner plus ou moins de points. A chaque tour l'algorithme peut accélérer ou ralentir de 1 seulement (Exemple : si sa vitesse en y est de 5 au prochain tour elle sera de 4 5 ou 6 seulement).

## Demo
![Preview](https://github.com/ThomasCorcoral/Projet_Racer_C/blob/master/racer.png)

## Comment a-t-il été réalisé ?

Ce projet a été intégralement réalisé en C#.<br>
Le viewer, le server ainsi que les trois niveaux (race-dumb, race-mid, race-hi) ont été fournis par M. Julien Bernard. 

## Guide d'installation

#### Etape 1
Clonez le projet GitHub et rendez vous dans le dossier

#### Etape 2
Compilez racer.c (gcc -Wall -std=c99 -g -o racer racer.c)

#### Etape 3
Ouvre <b>2</b> terminaux

#### Etape 4 
Dans le premier terminal lancez le viewer (./race-viewer)

#### Etape 5
Dans le second fichier lancez le serveur avec les participants souhaitez<br>
Par exemple avec 1 seul participant : ./race-server ./race<br>
Avec tous les participants : ./race-server ./racer ./race-dumb ./race-mid ./race-hi<br>

#### Etape 6
Pour afficher à nouveau le viewer, il vous faudra le relancer dans le premier terminal (./race-viewer)
