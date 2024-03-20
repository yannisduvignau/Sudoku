/*
 * Fichier : sudoku.h
 * But : Entête du module Sudoku
 * Auteurs : Yannis Duvignau et Mattin Guiheneuf
 * Date Dernière Modif : 18/01/2022
 * Remarques : Nous certifions être les auteurs de ce code source et attestons ne pas avoir utilisé d'extraits de code provenant d'internet.
               Ce code s'appuie uniquement sur les notions de programmation vues dans le cadre du module R1.01 - Initiation au développement (Partie 1 et Partie 2).
*/ 

#ifndef SUDOKU_H
#define SUDOKU_H

// VARIABLES
const unsigned short int NB_CASES = 9; // Le nombre de cases / la taille des tableaux employés. Le nombre de cases de nos tableaux sont de taille de 9.
const unsigned short int DEC_INT_ASCII = 48; /* Le décalage, par rapport à la table Ascii, pour convertir un chiffre (en caractère) en chiffre (en entier).
                                                Le décalage est de 48.*/
const unsigned short int COTE_ZONE = 3; // Le côté du carre/de la zone qui est de 3. Utilisé pour la vérification sur la zone.

struct Prop
{
    unsigned short int abscisse;
    unsigned short int ordonnee;
    unsigned short int element;
};

/*------------------------------------------------------------------------
                    Déclaration des sous-programmes
------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
            Getters (accès) aux champs de l'enregistrement
------------------------------------------------------------------------*/

short int getAbscisse(const Prop& pProp);
short int getOrdonnee(const Prop& pProp);
short int getElement(const Prop& pProp);
// But : Retourne respectivement l'abscisse, l'ordonnée et l'élément de la proposition

/*------------------------------------------------------------------------
                    Primitives de commparaison
------------------------------------------------------------------------*/

bool verifAbandon(Prop pProp);
// But : Vérifier que la proposition du joueur ne correspond pas à l'abandon de la partie.

bool verifErreurSaisie(Prop pProp);
// But : Vérifier que toutes les valeurs, saisies par le joueur lors de la proposition, correspondent à une valeur entière comprise entre 1 et 9, les valeurs du jeu.

bool verifErreurGrilleDep(char tab[NB_CASES][NB_CASES], Prop pProp);
// But : Vérifier que la proposition du joueur ne correspond pas à un élément présent dans la grille de départ.

bool verifLaComp(char tab[NB_CASES][NB_CASES], Prop pProp);
// But : Vérifier si la proposition est compatible. A faux si l'élément n'apparait ni sur la ligne ni sur la colonne ni dans la zone, vrai sinon.

bool recherchePointGrille(char tab[NB_CASES][NB_CASES]);
// But : Recherche le premier point dans la grille, afin de savoir si la grille est complète ou non. Si un point est trouvé alors vrai, sinon faux.

/*------------------------------------------------------------------------
                            Modificateurs
------------------------------------------------------------------------*/

int carre(int nombre, int exposant);
//But : Calculer le carré d'un nombre, grâce à un nombre et un exposant.

/*------------------------------------------------------------------------
                    Entrées / Sorties / Exportations
------------------------------------------------------------------------*/

void afficherTitreRegles();
// But : Afficher le titre du jeu et les règles du jeu du Sudoku

void afficherGrille(char tab[NB_CASES][NB_CASES], char tabVerif[NB_CASES][NB_CASES]);
// But : Afficher la grille du Sudoku

void afficherValPossibles(char tab[NB_CASES][NB_CASES], Prop pProp, char tabValPossible[NB_CASES], char tabValPasPossible[NB_CASES], bool tabBool[NB_CASES]);
// But : 

bool estDejaDedans(char tabValPasPossible[NB_CASES], char pElement);

#endif