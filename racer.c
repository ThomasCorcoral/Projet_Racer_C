#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define BUFSIZE 256

// gcc -Wall -std=c99 -O2 -g -o racer-fast racer-fast.c
// ./racer-fast

struct vec2		// Structure d'un vecteur (ou un nombre complexe), comporte 2 entiers
{
	int x;
	int y;
};

struct objective	// Un objectif comportant un point et une marge(en x et en y), il nous faut 4 points soit 2 vecteurs 
{
	struct vec2 pos;		// Position du l'objectif
	struct vec2 size;		// Marge de ce dernier
};

struct car			// Structure du joueur, on y trouve sa position (en x et en y) et sa vitesse (en x et en y)
{
	struct vec2 pos;		// Position du joueur
	struct vec2 spd;		// Vitesse du joueur
};

struct circuit		// Structure pour le plateau de jeu 
{
	int *data;				// Tableau de valeurs 
	size_t size;			// Taille du tableau
};

void circuit_create(struct circuit *self, int size)		// Fonction simple qui crée un circuit avec la taille de la grille et réalise une allocation mémoire
{
	self->size = size;
	self->data = calloc(size * size, sizeof(int));		// size * size car il y a size² valeurs à rentrer
}

void circuit_destroy(struct circuit *self)				// Permet de détruire le circuit à la fin du programme
{
	self->size = 0;
	free(self->data);
}

int sumFirstInteger(int n, int speed)		// renvoie la somme des entiers jusqu'à n à partir de speed exemple : speed = 2; n = 5; 
{											// la fonction va retourner 2 + 3 + 4 + 5 = 14
	return ((n * (n + 1)) - ((speed -1) * speed)) / 2 ;
}

_Bool NeedToBrake(int speed, int difference)		// renvoie true si le joueur doit freiner. Elle se base sur le fait que s'il conserve cette vitesse,
{   												// il ne pourra pas atteindre le point qui se situe à "difference" cases
    return sumFirstInteger(speed, 1)-1 > difference;
}

_Bool ReleaseThePedal(int speed, int difference)	// Est vrai si le joueur doit garder la même vitesse
{
    return sumFirstInteger(speed, 1) + speed == difference; // + speed car cela signifie que si on garde cette vitesse on tombe sur la case
}

int findNextMoveFwd(int speed, int difference)		// Pour les mouvements en avant. Fait appel aux fonctions booléennes ci-dessus
{ 
    if(ReleaseThePedal(speed, difference))	/// Vérification que la vitesse ne doit pas être gardé
    {
        return speed;
    }
    else if(NeedToBrake(speed+1, difference))	// On demande si la voiture va allé trop loin si l'on augmente la vitesse. D'ou le +1
    {
        return speed-1;
    }
    else		// Sinon, on accélère. 
    {
        return speed+1;
    }
}

int findNextMoveBwd(int speed, int difference)		// Même fonction que la précédente, permet de gérer les déplacements en arrière
{
    if(ReleaseThePedal(-speed, -difference))
    {
        return speed;
    }
    else if(NeedToBrake(-speed+1, -difference))
    {
        return speed+1;
    }
    else
    {
        return speed-1;
    }
}

int applySpeed(int difference, int speed)		// fonction qui applique la vitesse
{
	if(difference == 0 && speed > -2 && speed < 2)	// On arrête la voiture si la différence est nule, tout en vérifiant
	{												// que -1 <= vitesse <= 1 car sinon on va être DISQUALIFIED
		return 0;
	}

	if(difference >= 0)		// Si on avance en avant alors on applique la fonction adéquate
	{
		return findNextMoveFwd(speed, difference);
	}
	else
	{
		return findNextMoveBwd(speed, difference);
	}
}

int determineDiff(int posJoueur, int posObjectif, int marge)	// permet de déterminer la distance entre notre personnage et le checkpoint tout
{																// en prenant en compte la marge de l'objectif
	if(posJoueur >= posObjectif && posJoueur <= (posObjectif + marge - 1))	// Si la position sur l'un des axes est dans le checkpoint, on renvoie 0
	{
		return 0;
	}
	else
	{
        if(posJoueur < posObjectif)			// Si le joueur est avant le checkpooint (sur l'un des deux axes)
        {
		    return posObjectif - posJoueur;
        }
        else		// sinon il est après et l'on prend en compte la marge
        {
            return (posObjectif + marge - 1) - posJoueur;
        }
        
	}
}

bool isReachable(struct vec2 dif , struct objective obj , int size , struct vec2 to_reach)		// Vérifie qu'un point est atteignable
{
    bool res_x;
    bool res_y;
    int difference_to_wall = to_reach.x;    // on initialise avec dif.x negatif et on a 0 - to_reach.x donc to_reach.x
    int dif_from_start = (obj.pos.x + obj.size.x - 1) - to_reach.x + 1; // distance avec la paroie de l'objectif
    if(dif.x > 0)		// Si la différence en x est positive 
    {
        //par rapport à size
        difference_to_wall = (size - 1) - to_reach.x;		// Distance entre la bordure et le mur
        dif_from_start = to_reach.x - obj.pos.x + 1;		// distance entre la première paroie de l'objectif
    }
    if(difference_to_wall < sumFirstInteger(dif_from_start, 0)) // si l'on est hors terrain en arrivant avec une vitesse sufisante pour le toucher
    {   // on renvoie false
        res_x = false;
    }
    else
    {
        res_x = true;
    }

    // meme chose avec les y

    difference_to_wall = to_reach.y;
    dif_from_start = (obj.pos.y + obj.size.y - 1) - to_reach.y + 1;

    if(dif.y > 0)
    {
        int difference_to_wall = (size - 1) - to_reach.y;
        int dif_from_start = to_reach.y - obj.pos.y + 1;
    }
    if(difference_to_wall < sumFirstInteger(dif_from_start, 0))
    {
        res_y = false;
    }
    else
    {
        res_y = true;
    }
    // On renvoie res_x ou res_y car en fonction de l'angle si un seul des deux est inateignable on peut quand meme tombre dessus
    return res_x || res_y ;
}

struct vec2 determineBest(struct objective obj, struct vec2 dif, struct circuit game)
{
    struct vec2 best;

	struct vec2 to_reach;

    if(abs(dif.x) > abs(dif.y))
    {
        if(dif.x < 0)
        {
            best.x = obj.pos.x + obj.size.x - 1 ;
        }
        else
        {
            best.x = obj.pos.x;
        }
		best.y = obj.pos.y;
		to_reach.x = best.x;
        for(int y = obj.pos.y; y < obj.pos.y + obj.size.y; ++y)
        {
            to_reach.y = y;
            if(game.data[best.x + best.y * game.size] > game.data[to_reach.x + to_reach.y * game.size] && isReachable( dif , obj , game.size , to_reach))
            {
                best.y = y;
            }
        }
    }
    else if(abs(dif.x) < abs(dif.y))
    {
        if(dif.y < 0)
        {
            best.y = obj.pos.y + obj.size.y - 1 ;
        }
        else
        {
            best.y = obj.pos.y;
        }
        to_reach.y = best.y;
		best.x = obj.pos.x;
        for(int x = obj.pos.x ; x < obj.pos.x + obj.size.x; ++x)
        {
            to_reach.x = x;
            if(game.data[best.x + best.y * game.size] > game.data[to_reach.x + to_reach.y * game.size] && isReachable( dif , obj , game.size , to_reach))
            {
                best.x = x;
            }
        }
    }
    else
    {
        if(dif.x <= 0 && dif.y <= 0)
        {
            best.x = obj.pos.x;
            best.y = obj.pos.y;
            to_reach.y = best.y;
            for(int x = obj.pos.x ; x < obj.pos.x + obj.size.x; ++x)
            {
                to_reach.x = x;
                if(game.data[best.x + best.y * game.size] > game.data[to_reach.x + to_reach.y * game.size] && isReachable( dif , obj , game.size , to_reach))
                {
                    best.x = x;
                }
            }
            to_reach.x = obj.pos.x;
            for(int y = obj.pos.y ; y < obj.pos.y + obj.size.y; ++y)
            {
                to_reach.y = y;
                if(game.data[best.x + best.y * game.size] > game.data[to_reach.x + y * game.size] && isReachable( dif , obj , game.size , to_reach))
                {
                    best.y = y;
                }
            }
        }
        else if(dif.x >= 0 && dif.y <= 0)
        {
            best.x = obj.pos.x + obj.size.x - 1;
            best.y = obj.pos.y;
            to_reach.y = best.y;
            for(int x = obj.pos.x ; x < obj.pos.x + obj.size.x; ++x)
            {
                to_reach.x = x;
                if(game.data[best.x + best.y * game.size] > game.data[to_reach.x + to_reach.y * game.size] && isReachable( dif , obj , game.size , to_reach))
                {
                    best.x = x;
                }
            }
            to_reach.x = (obj.pos.x + obj.size.x - 1);
            for(int y = obj.pos.y ; y < obj.pos.y + obj.size.y; ++y)
            {
                to_reach.y = y;
                if(game.data[best.x + best.y * game.size] > game.data[ to_reach.x + to_reach.y * game.size] && isReachable( dif , obj , game.size , to_reach))
                {
                    best.y = y;
                }
            }
        }
        else if(dif.x <= 0 && dif.y >= 0)
        {
            best.x = obj.pos.x;
            best.y = obj.pos.y + obj.size.y - 1 ;
            to_reach.y = best.y;
            for(int x = obj.pos.x ; x < obj.pos.x + obj.size.x; ++x)
            {
                to_reach.x = x;
                if(game.data[best.x + best.y * game.size] > game.data[to_reach.x + to_reach.y * game.size] && isReachable( dif , obj , game.size , to_reach))
                {
                    best.x = x;
                }
            }
            to_reach.x = (obj.pos.x + obj.size.x - 1);
            for(int y = obj.pos.y ; y < obj.pos.y + obj.size.y; ++y)
            {
                to_reach.y = y;
                if(game.data[best.x + best.y * game.size] > game.data[ to_reach.x + to_reach.y * game.size] && isReachable( dif , obj , game.size , to_reach))
                {
                    best.y = y;
                }
            }
        }
        else
        {
            best.x = obj.pos.x + obj.size.y - 1;
            best.y = obj.pos.y + obj.size.y - 1;
            to_reach.y = best.y;
            for(int x = obj.pos.x ; x < obj.pos.x + obj.size.x; ++x)
            {
                to_reach.x = x;
                if(game.data[best.x + best.y * game.size] > game.data[to_reach.x + to_reach.y * game.size] && isReachable( dif , obj , game.size , to_reach))
                {
                    best.x = x;
                }
            }
            to_reach.x = (obj.pos.x + obj.size.x - 1);
            for(int y = obj.pos.y ; y < obj.pos.y + obj.size.y; ++y)
            {
                to_reach.y = y;
                if(game.data[best.x + best.y * game.size] > game.data[ to_reach.x + to_reach.y * game.size] && isReachable( dif , obj , game.size , to_reach))
                {
                    best.y = y;
                }
            }
        }
        
    }
	fprintf(stderr,"best x : %i\n best y : %i\n", best.x, best.y);
    return best;
}

int determineDiffNoMarge(int posJoueur, int posObjectif)
{
	if(posJoueur == posObjectif)
	{
		return 0;
	}
	return posObjectif - posJoueur;
}

int main() 
{
	struct vec2 difference;
	struct vec2 best;
	struct car driver;
	struct objective obj;
	struct circuit grille;

	setbuf(stdout, NULL);
	char buf[BUFSIZE];
	
	// get the size
	fgets(buf, BUFSIZE, stdin);
	int size = atoi(buf);
	circuit_create(&grille, size);

	for (int i = 0; i < size * size; ++i) 
	{
		fgets(buf, BUFSIZE, stdin);
		int value = atoi(buf);

		grille.data[i] = value;
	}

	// get the position
	fgets(buf, BUFSIZE, stdin);
	driver.pos.x = atoi(buf);
	fgets(buf, BUFSIZE, stdin);
	driver.pos.y = atoi(buf);

	driver.spd.x = 0;
	driver.spd.y = 0;
	
	// get the objective
	fgets(buf, BUFSIZE, stdin);
	obj.pos.x = atoi(buf);
	fgets(buf, BUFSIZE, stdin);
	obj.pos.y = atoi(buf);
	fgets(buf, BUFSIZE, stdin);
	obj.size.x = atoi(buf);
	fgets(buf, BUFSIZE, stdin);
	obj.size.y = atoi(buf);

	difference.x = determineDiff(driver.pos.x, obj.pos.x, obj.size.x);
	difference.y = determineDiff(driver.pos.y, obj.pos.y, obj.size.y);

	best = determineBest(obj,difference, grille);

	for ( ; ; ) 
	{
		
		difference.x = determineDiffNoMarge(driver.pos.x, best.x);
		difference.y = determineDiffNoMarge(driver.pos.y, best.y);

		driver.spd.x = applySpeed(difference.x, driver.spd.x);
		driver.spd.y = applySpeed(difference.y, driver.spd.y);

		driver.pos.x += driver.spd.x;
		driver.pos.y += driver.spd.y;
		
		printf("%i\n%i\n", driver.pos.x, driver.pos.y);
		// get the response
		fgets(buf, BUFSIZE, stdin);
		
		if (strcmp(buf, "ERROR\n") == 0) 
		{
			break;
		}
		else if (strcmp(buf, "CHECKPOINT\n") == 0) 
		{
			fgets(buf, BUFSIZE, stdin);
			obj.pos.x = atoi(buf);
			fgets(buf, BUFSIZE, stdin);
			obj.pos.y = atoi(buf);
			fgets(buf, BUFSIZE, stdin);
			obj.size.x = atoi(buf);
			fgets(buf, BUFSIZE, stdin);
			obj.size.y = atoi(buf);

			difference.x = determineDiff(driver.pos.x, obj.pos.x, obj.size.x);
			difference.y = determineDiff(driver.pos.y, obj.pos.y, obj.size.y);

			best = determineBest(obj,difference, grille);
		}
	}
	circuit_destroy(&grille);
	return 0;
}