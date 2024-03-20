/*
 * Fichier : sudoku.cpp
 * But : Corps du module Sudoku
 * Auteurs : Yannis Duvignau et Mattin Guiheneuf
 * Date Dernière Modif : 18/01/2023
 * Remarques : Nous certifions être les auteurs de ce code source et attestons ne pas avoir utilisé d'extraits de code provenant d'internet.
               Ce code s'appuie uniquement sur les notions de programmation vues dans le cadre du module R1.01 - Initiation au développement (Partie 1 et Partie 2).
 */

#include "sudoku.h"
#include "game-tools.h"

// VARIABLES
unsigned short int abscisseZone; // L’abscisse de la zone qui va être parcouru
unsigned short int ordonneeZone; // L’ordonnée de la zone qui va être parcouru

/*------------------------------------------------------------------------
    Sous-Programmes nécessaires à la réalisation des sous-programmes
------------------------------------------------------------------------*/

bool estDejaDedans(char tabValPasPossible[NB_CASES], char pElement)
{
    for (unsigned short int i = 0; i <= (NB_CASES - 1); i++)
    {
        if (tabValPasPossible[i] == pElement)
        {
            return true;
        }
    }
    return false;
}

/*------------------------------------------------------------------------
                    Déclaration des sous-programmes
------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
    Sous-Programmes nécessaires à la réalisation des sous-programmes
------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
            Getters (accès) aux champs à l'enregistrement
------------------------------------------------------------------------*/
short int getAbscisse(const Prop &pProp)
{
    return pProp.abscisse;
}

short int getOrdonnee(const Prop &pProp)
{
    return pProp.ordonnee;
}

short int getElement(const Prop &pProp)
{
    return pProp.element;
}

/*------------------------------------------------------------------------
                    Primitives de commparaison
------------------------------------------------------------------------*/

bool verifAbandon(Prop pProp)
{
    if ((getAbscisse(pProp) == 0) && (getOrdonnee(pProp) == 0) && (getElement(pProp) == 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool verifErreurSaisie(Prop pProp)
{
    if (((getAbscisse(pProp) >= 'A') && (getAbscisse(pProp) <= 'Z')) || ((getOrdonnee(pProp) >= 'A') && (getOrdonnee(pProp) <= 'Z')) || ((getElement(pProp) >= 'A') && (getElement(pProp) <= 'Z')))
    {
        return true;
    }
    else if ((!((getAbscisse(pProp) == 0) && (getOrdonnee(pProp) == 0) && (getElement(pProp) == 0))) && ((getAbscisse(pProp) == 0) || (getOrdonnee(pProp) == 0) || (getElement(pProp) == 0)))
    {
        return true;
    }
    else if (((getAbscisse(pProp) < 0) || (getAbscisse(pProp) > 9)) || ((getOrdonnee(pProp) < 0) || (getOrdonnee(pProp) > 9)) || ((getElement(pProp) < 0) || (getElement(pProp) > 9)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool verifErreurGrilleDep(char tab[NB_CASES][NB_CASES], Prop pProp)
{
    if (tab[getAbscisse(pProp) - 1][getOrdonnee(pProp) - 1] != '.')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool verifLaComp(char tab[NB_CASES][NB_CASES], Prop pProp)
{
    //  tab[NB_CASES][NB_CASES], pProp >> Faire la recherche

    // tab[NB_CASES][NB_CASES], pProp >> Recherche Ligne/Colonne
    for (unsigned short int i = 0; i <= (NB_CASES - 1); i++)
    {
        // Recherche Ligne
        if (tab[getAbscisse(pProp) - 1][i] == char(getElement(pProp) + DEC_INT_ASCII))
        {
            return false;
        }

        // Recherche Colonne
        if (tab[i][getOrdonnee(pProp) - 1] == char(getElement(pProp) + DEC_INT_ASCII))
        {
            return false;
        }
    }

    // tab[NB_CASES][NB_CASES], pProp, DEC_INT_ASCII >> Vérifier la zone
    // pProp >> Positionner à l'élément en haut à gauche de la zone >> abscisseZone, ordonneeZone
    // pProp.abscisse >> Positionner l'abscisse >> abscisseZone
    abscisseZone = static_cast<unsigned short int>((static_cast<int>(getAbscisse(pProp)) - 1) - ((static_cast<int>(getAbscisse(pProp)) - 1) % 3));

    // pProp.ordonnee >> Positionner l'ordonnée >> ordonneeZone
    ordonneeZone = static_cast<unsigned short int>((static_cast<int>(getOrdonnee(pProp)) - 1) - ((static_cast<int>(getOrdonnee(pProp)) - 1) % 3));

    // tab[NB_CASES][NB_CASES], pProp.element, abscisseZone, ordonneeZone, DEC_INT_ASCII, COTE_ZONE >> Recherche Zone
    for (unsigned short int i = abscisseZone; i < abscisseZone + COTE_ZONE; i++)
    {
        for (unsigned short int j = ordonneeZone; j < ordonneeZone + COTE_ZONE; j++)
        {
            if (tab[i][j] == char(getElement(pProp) + DEC_INT_ASCII))
            {
                return false;
            }
        }
    }

    // La valeur n'est ni dans la ligne ni dans la colonne ni dans la zone
    return true;
}

bool recherchePointGrille(char tab[NB_CASES][NB_CASES])
{
    // TRAITEMENTS

    // Effectuer la recherche
    // Déterminer si on a analysé tous les éléments de la structure
    for (unsigned short int i = 0; i <= (NB_CASES - 1); i++)
    {
        for (unsigned short int j = 0; j <= (NB_CASES - 1); j++)
        {
            // Vérifier si on a trouvé ce qu'on cherche
            if (tab[i][j] == '.')
            {
                return true;
            }
        }
    }
    return false;
}

/*------------------------------------------------------------------------
                            Modificateurs
------------------------------------------------------------------------*/

int carre(int nombre, int exposant)
{
    int carreNombre;
    carreNombre = nombre;

    if ((exposant == 0) && (nombre == 10))
    {
        return 1;
    }

    for (int i = 0; i < exposant - 1; i++)
    {
        carreNombre = carreNombre * nombre;
    }
    return carreNombre;
}

/*------------------------------------------------------------------------
                    Entrées / Sorties / Exportations
------------------------------------------------------------------------*/

void afficherTitreRegles()
{
    cout << endl;
    afficherTexteEnCouleur("                                        ------------------------------------ \n                                       ||              SUDOKU              ||\n                                        ------------------------------------ ", bleu, true);
    cout << endl
         << endl; // Longue ligne pour limiter les appels de bibliothèques

    cout << "Le Sudoku est une grille (ici de 9x9) que vous devez completer avec des valeurs, de sorte que :" << endl
         << "   -	Chaque ligne," << endl
         << "   -	Chaque colonne," << endl
         << "   -	Et chaque zone (3x3) de la grille" << endl
         << "   Contienne les valeurs 1, 2, 3, 4, 5, 6, 7, 8 et 9." << endl
         << "Une grille incomplete vous est proposee en debut de partie."
         << " A vous de la completer en proposant une valeur a placer sur la grille." << endl
         << "Vous gagnez lorsque vous avez completer entierement la grille,"
         << " vous perdez lorsque vous avez epuise votre nombre de droits a l erreur defini par vous-meme." << endl
         << endl;
}

void afficherGrille(char tab[NB_CASES][NB_CASES], char tabVerif[NB_CASES][NB_CASES])
{
    cout << endl;
    cout << "      1 2 3   4 5 6   7 8 9     " << endl;
    cout << "    -------------------------   " << endl;
    for (int i = 0; i <= (NB_CASES - 1); i++)
    {
        cout << " " << i + 1 << "  | ";
        for (int j = 0; j <= (NB_CASES - 1); j++)
        {
            if (tab[i][j] == '.')
            {
                cout << tab[i][j];
            }
            else
            {
                if (tabVerif[i][j] == tab[i][j])
                {
                    afficherTexteEnCouleur(tabVerif[i][j], rouge);
                }
                else
                {
                    afficherTexteEnCouleur(tab[i][j], bleu);
                }
            }
            cout << " ";
            if ((j + 1) % 3 == 0)
            {
                cout << "| ";
            }
            if ((j + 1) % 9 == 0)
            {
                cout << i + 1 << endl;
            }
        }
        if ((i + 1) % 3 == 0)
        {
            cout << "    -------------------------   " << endl;
        }
    }
    cout << "      1 2 3   4 5 6   7 8 9     " << endl
         << endl;
}

void afficherValPossibles(char tab[NB_CASES][NB_CASES], Prop pProp, char tabValPasPossible[NB_CASES], char tabValPossible[NB_CASES], bool tabBool[NB_CASES])
{
    // VARIABLES
    unsigned short int compteInd; //

    // TRAITEMENTS
    // Initialiser le compteur d'indice
    compteInd = 0;

    // Parcourir la ligne, la colonne et la zone
    // Parcours de la ligne et de la colonne
    for (unsigned short int i = 0; i <= (NB_CASES - 1); i++)
    {
        // Parcours de la ligne
        if (tab[getAbscisse(pProp) - 1][i] != '.')
        {
            // Ajouter les chiffres trouvés (sans doublon) à un tableau de valeurs pas possibles
            if (estDejaDedans(tabValPasPossible, tab[getAbscisse(pProp) - 1][i]) == false)
            {
                // Ajouter le chiffre trouvé
                tabValPasPossible[compteInd] = tab[getAbscisse(pProp) - 1][i];

                // Incrémenter le compteur d'indice de 1
                compteInd = static_cast<unsigned short int>(compteInd + 1);
            }
        }

        // Parcours de la colonne
        if (tab[i][getOrdonnee(pProp) - 1] != '.')
        {
            // Ajouter les chiffres trouvés (sans doublon) à un tableau de valeurs pas possibles
            if (estDejaDedans(tabValPasPossible, tab[i][getOrdonnee(pProp) - 1]) == false)
            {
                // Ajouter le chiffre trouvé
                tabValPasPossible[compteInd] = tab[i][getOrdonnee(pProp) - 1];

                // Incrémenter le compteur d'indice de 1
                compteInd = static_cast<unsigned short int>(compteInd + 1);
            }
        }
    }

    // Parcours de la zone
    // Se positionner à l'élément en haut à gauche de la zone
    // Positionner l'abscisse
    abscisseZone = static_cast<unsigned short int>((static_cast<int>(getAbscisse(pProp)) - 1) - ((static_cast<int>(getAbscisse(pProp)) - 1) % 3) + 1);

    // Positionner l'ordonnée
    ordonneeZone = static_cast<unsigned short int>((static_cast<int>(getOrdonnee(pProp)) - 1) - ((static_cast<int>(getOrdonnee(pProp)) - 1) % 3) + 1);

    // Identifier les valeurs pas possibles
    for (unsigned short int i = abscisseZone; i < (abscisseZone + COTE_ZONE); i++)
    {
        for (unsigned short int j = ordonneeZone; j < (ordonneeZone + COTE_ZONE); j++)
        {
            if (tab[i - 1][j - 1] != '.')
            {
                if (estDejaDedans(tabValPasPossible, tab[i - 1][j - 1]) == false)
                {
                    tabValPasPossible[compteInd] = tab[i - 1][j - 1];

                    // Incrémenter le compteur d'indice de 1
                    compteInd = static_cast<unsigned short int>(compteInd + 1);
                }
            }
        }
    }

    // Associer les valeurs trouvées à des booléens dans un tableau de bouléen
    for (unsigned short int i = 0; i <= (NB_CASES - 1); i++)
    {
        for (unsigned short int j = 0; j <= (NB_CASES - 1); j++)
        {
            if (tabValPossible[i] == tabValPasPossible[j])
            {
                tabBool[i] = true;
            }
        }
    }

    // Afficher les valeurs qui conviennent
    for (unsigned short int i = 0; i <= (NB_CASES - 1); i++)
    {
        if (tabBool[i] == false)
        {
            cout << tabValPossible[i] << " ";
        }
    }
}
