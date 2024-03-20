/*
 * Fichier : main.cpp
 * But : Permet d'éxécuter le jeu du Sudoku
 * Auteurs : Yannis Duvignau et Mattin Guiheneuf
 * Date Dernière Modif : 18/01/2023
 * Remarques : Nous certifions être les auteurs de ce code source et attestons ne pas avoir utilisé d'extraits de code provenant d'internet.
               Ce code s'appuie uniquement sur les notions de programmation vues dans le cadre du module R1.01 - Initiation au développement (Partie 1 et Partie 2).
 */

#include <iostream>
#include "sudoku.h"
#include "game-tools.h"
using namespace std;



int main(void)
{
    // VARIABLES
    unsigned short int nbErreur; // Le nombre d’erreurs du joueur au cours du jeu.
    unsigned short int nbTour;   // Le nombre de tours accomplis par le joueur.

    enum stadePartie
    {
        enCours,
        choixAbandon,
        maxErreur,
        victoire
    };
    stadePartie stadeActuelPartie; /* Le stade actuel auquel appartient le joueur durant une partie.
                                      Il peut prendre les valeurs suivantes : enCours, choixAbandon, erreurMax ou victoire.*/

    string stringNbErreurMax;                    // 

    int nbErreurMax;                             // Le nombre d’erreur maximum que le joueur s’autorise.
    const unsigned short int NBR_ERREUR_MIN = 3; // Le nombre d’erreurs minimum autorisé que le joueur peut saisir.

    char charAbscisse; /* L’abscisse de la proposition récupérée après sa saisie.
                          Il sera ensuite converti en entier, mais il est utilisé seulement pour faire des vérifications pour les erreurs de saisie (sur les caractères). */
    char charOrdonnee; /* L’ordonnée de la proposition récupérée après sa saisie.
                          Il sera ensuite converti en entier, mais il est utilisé seulement pour faire des vérifications pour les erreurs de saisie (sur les caractères)*/
    char charElement;  /* L’élément de la proposition récupérée après sa saisie. 
                          Il sera ensuite converti en entier, mais il est utilisé seulement pour faire des vérifications pour les erreurs de saisie (sur les caractères)*/

    const unsigned short int NB_TOUR_MIN_FIN = 51; /* Le nombre de tours minimum pour pouvoir compléter la grille.
                                                      Il a été calculé au préalable pour optimiser le jeu et éviter de faire des vérifications inutiles : 
                                                      Il y a 81 cases et 30 chiffres pré-rentrées donc le joueur peut finir le jeu en 51 coups minimum soit 51 tours.*/

    enum etatProp
    {
        enTrait,
        abandon,
        erreurSaisie,
        erreurGrilleDep,
        valComp,
        valIncomp
    };
    etatProp etatActuelProp; /* L’état de la proposition actuel.
                                Détermine s'il y a une erreur de saisie, une condition d’abandon, une erreur sur la grille de départ,
                                la valeur est incompatible avec la grille (l’élément ne peut pas être placé),
                                la valeur est compatible avec la grille (l’élément peut être placé) et en traitement (durant un tour, l’état de la proposition est en cours de traitement).
                                Il peut donc prendre les valeurs suivantes : erreurSaisie, erreurGrilleDep, abandon, enTrait, valIncomp ou valComp. */

    Prop propJoueur; // Proposition du joueur qui consiste à récupérer la saisie d'une abscisse, d'une ordonnée et d'un élément.

    char grilleJeu[NB_CASES][NB_CASES] = {{'5', '3', '.', '.', '7', '.', '.', '.', '.'},
                                          {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
                                          {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
                                          {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
                                          {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
                                          {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
                                          {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
                                          {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
                                          {'.', '.', '.', '.', '8', '.', '.', '7', '9'}}; // Le tableau à 2 dimensions dans lequel va se dérouler le jeu.
    char grilleBase[NB_CASES][NB_CASES] = {{'5', '3', '.', '.', '7', '.', '.', '.', '.'},
                                           {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
                                           {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
                                           {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
                                           {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
                                           {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
                                           {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
                                           {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
                                           {'.', '.', '.', '.', '8', '.', '.', '7', '9'}}; // Le tableau à 2 dimensions de début de jeu, contenant les chiffres de départ.
    char tabValComp[NB_CASES] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};             // Tableau de 1 à 9 utilisé pour comparer les valeurs lors de l’affichage des valeurs possibles (scénario 2)
                                                                                              
    char tabValIncomp[NB_CASES];                                                           /* Le tableau dans lequel on va ajouter (sans doublon) toutes les valeurs trouvées sur ligne, colonne et carré pour des coordonnées [x, y] rentrées.
                                                                                              Ce tableau est utilisé pour afficher les valeurs possibles lorsque l’utilisateur se trompe (scénario 2)*/
    bool tabBoolVerif[NB_CASES];                                                           // Le tableau dans lequel les valeurs incompatibles présentent dans le tableau tabValIncomp seront égale à faux, vrai sinon. Utiliser pour trouver les valeurs possible (scénario 2)

    // TRAITEMENTS

    // DEC_INT_ASCII >> Initialiser la partie >> nbErreur, nbTour, stadeActuelPartie, nbErreurMax supérieur ou égal à 3

    // Initialiser les paramètres de base >> nbErreur, nbTour, stadeActuelPartie

    nbErreur = 0;
    nbTour = 1;
    stadeActuelPartie = enCours;

    // Afficher le titre en bleu et les règles

    afficherTitreRegles();

    // DEC_INT_ASCII >> Saisie et Vérification du nombre d'erreurs autorisées >> nbErreurMax supérieur ou égal à 3

    // Saisie du nombre d'erreurs maximum affiché en rouge >> stringNbErreurMax
    cout << endl << endl
         << "Le nombre d'erreurs maximum autorisees doit etre superieur ou egale a 3. " << endl
         << "Si vous entrez autre chose qu'une valeur entiere superieure a 3, le nombre d'erreurs maximum autorisees sera initialise par defaut a 3" << endl
         << endl;
    afficherTexteEnCouleur(" Choix du nombre d'erreurs autorisees (>= 3) : ", rouge, false);
    cin >> stringNbErreurMax;

    // stringNbErreurMax, DEC_INT_ASCII, NBR_ERREUR_MIN >> Vérifier que le nombre d'erreurs maximum saisi soit valable >> nbErreurMax
    // Vérifier que le nombre ne soit pas un nombre négatif
    if (stringNbErreurMax[0] == '-')
    {
        // nbErreurMax n'est pas validé
        nbErreurMax = NBR_ERREUR_MIN;
    }
    else
    {
        // Faire les vérifications sur stringNbErreurMax
        for (unsigned int i = 0; i < stringNbErreurMax.size(); i++)
        {
            // Vérifier que le nombre d'erreurs maximum, saisi par le joueur, soit bien un nombre
            if (stringNbErreurMax[i] >= 'a' && stringNbErreurMax[i] <= 'z')
            {
                // nbErreurMax n'est pas validé
                nbErreurMax = NBR_ERREUR_MIN;
            }
            else
            {
                nbErreurMax = nbErreurMax + (int(stringNbErreurMax[i]) - static_cast<int>(DEC_INT_ASCII)) * (carre(10, static_cast<int>(stringNbErreurMax.size()) - static_cast<int>(i) - 1));
            }
            // Vérif nbErreurMax
            if (nbErreurMax < NBR_ERREUR_MIN)
            {
                nbErreurMax = NBR_ERREUR_MIN;
            }
        }
        //Vérifier que l'utilisateur ne rentre pas un nombre à virgule
        for (unsigned int i = 0; i < stringNbErreurMax.size(); i++)
        {
            if (stringNbErreurMax[i] == '.' || stringNbErreurMax[i] == ',')
            {
                // nbErreurMax n'est pas validé
                nbErreurMax = NBR_ERREUR_MIN;
                break;
            }
        }
    }

    afficherTexteEnCouleur("                                         Vous pouvez faire jusqu'a ", rouge, false);
    afficherNombreEnCouleur(nbErreurMax, rouge, false);
    afficherTexteEnCouleur(" erreurs", rouge, true);

    // Attendre une action du joueur pour commencer la partie

    cout << endl
         << "Appuyer sur une touche pour continuer ...";
    pause();
    effacer();

    /* nbErreur, nbTour, stadeActuelPartie, nbErreurMax supérieur ou égal à 3, NB_TOUR_MIN_FIN,  tabValIncomp, tabValComp, tabBoolVerif,
       grilleJeu, grilleBase, NB_CASES, DEC_INT_ASCII >> Jouer la Partie >> stadeActuelPartie*/

    do
    {
        // Initialiser les paramètres de gestion d'un tour

        etatActuelProp = enTrait;

        // ititialiser grilleJeu

        for (int elementI = 0; elementI <= (NB_CASES - 1); elementI++)
        {
            tabBoolVerif[elementI] = false;
        }

        for (int elementI = 0; elementI <= (NB_CASES - 1); elementI++)
        {
            tabValIncomp[elementI] = '0';
        }

        /* nbTour, nbErreur, nbErreurMax, grilleJeu, grilleBase, NB_CASES, etatActuelProp, stadeActuelPartie,
           NB_TOUR_MIN_FIN,  tabValIncomp, tabValComp, tabBoolVerif, DEC_INT_ASCII >> Jouer un tour */

        // nbTour, nbErreur, nbErreurMax, grilleJeu, grilleBase, NB_CASES >> Afficher les règles du jeu, la grille et le menu d'évolution de la partie 

        // Afficher le titre en bleu et les règles du jeu
        afficherTitreRegles();

        // grilleJeu, grilleBase >> Afficher la grille
        afficherGrille(grilleJeu, grilleBase);

        // nbTour, nbErreur, nbErreurMax >> Afficher le menu d'évolution de la partie

        // nbTour >> Afficher le nombre de tours
        cout << " Tour " << nbTour << ",";

        // nbErreur, nbErreurMax >> Afficher le nombre d'erreurs
        cout << " Erreur : " << nbErreur << "/" << nbErreurMax << ", ";

        cout << "Abandon" << endl;


        // DEC_INT_ASCII >> Saisie-Vérif de la proposition >> propJoueur, charAbscisse, charOrdonnee, charElement, etatActuelProp 

        // Saisie de la proposition >> charAbscisse, charOrdonnee, charElement
        cout << " Proposition (cf. x y i) ? ";
        cin >> charAbscisse;
        cin >> charOrdonnee;
        cin >> charElement;

        // charAbscisse, charOrdonnee, charElement, DEC_INT_ASCII >> Conversion des coordonnées et de l'élément à placer >> propJoueur
        // charAbscisse, DEC_INT_ASCII >> Conversion de l'abscisse >> propJoueur.abscisse
        propJoueur.abscisse = static_cast<unsigned short int>(int(charAbscisse) - static_cast<int>(DEC_INT_ASCII));

        // charOrdonnee, DEC_INT_ASCII >> Conversion de l'ordonnée >> propJoueur.ordonnee
        propJoueur.ordonnee = static_cast<unsigned short int>(int(charOrdonnee) - static_cast<int>(DEC_INT_ASCII));

        // charElement, DEC_INT_ASCII >> Conversion de l'élément >> propJoueur.element
        propJoueur.element = static_cast<unsigned short int>(int(charElement) - static_cast<int>(DEC_INT_ASCII));

        // propJoueur, etatActuelProp, stadeActuelPartie, grilleBase >> Vérification de la proposition >> [etatActuelProp], [stadeActuelPartie]
        if (verifAbandon(propJoueur))
        {
            // etatActuelProp, stadeActuelPartie >> Le joueur abandonne >> etatActuelProp, stadeActuelPartie 
            etatActuelProp = abandon;
            stadeActuelPartie = choixAbandon;
        }
        else if (verifErreurSaisie(propJoueur))
        {
            // etatActuelProp >> Erreur de Saisie >> etatActuelProp
            etatActuelProp = erreurSaisie;
        }
        else if (verifErreurGrilleDep(grilleBase, propJoueur))
        {
            // etatActuelProp >> Erreur Saisie Grille Départ >> etatActuelProp
            etatActuelProp = erreurGrilleDep;
        }
        else
        {
            // La proposition est plausible
        }

        if (etatActuelProp == enTrait)
        {
            // etatActuelProp, grilleJeu, propJoueur, NB_CASES >> Contrôler si la valeur est compatible avec la grille du jeu >> etatActuelProp
            if (verifLaComp(grilleJeu, propJoueur))
            {
                // etatActuelProp >> La valeur saisie est compatible >> etatActuelProp
                etatActuelProp = valComp;
            }
            else
            {
                // etatActuelProp >> La valeur saisie est incompatible >> etatActuelProp
                etatActuelProp = valIncomp;
            }
        }

        // etatActuelProp, grilleJeu, tabValComp, tabValIncomp, tabBoolVerif, propJoueur? DEC_INT_ASCII >> Afficher le résultat de la proposition

        switch (etatActuelProp)
        {
        case abandon:
            cout << " A B A N D O N ! !\n";
            break;

        case erreurGrilleDep:
            cout << " Erreur # Saisie sur grille de depart #\n";
            break;

        case erreurSaisie:
            cout << " E R R E U R  D E  S A I S I E  ! ! !\n";
            break;

        case valIncomp:
            cout << " Erreur # Valeur incompatible #" << endl
                 << " Valeurs possibles : ";
            afficherValPossibles(grilleJeu, propJoueur, tabValIncomp, tabValComp, tabBoolVerif);
            break;

        case valComp:
            if (grilleJeu[propJoueur.abscisse - 1][propJoueur.ordonnee - 1] == '.')
            {
                // Pas de valeurs précedemment à cet emplacement
                cout << " O U I !";
            }
            else
            {
                // grilleJeu, propJoueur, DEC_INT_ASCII >> Une valeur précedemment à cet emplacement
                cout << " O U I !    Valeur " << grilleJeu[propJoueur.abscisse - 1][propJoueur.ordonnee - 1] << " modifiee en " << char(propJoueur.element + DEC_INT_ASCII);
            }
        default :
            break;
        }

        // nbTour, etatActuelProp, nbErreur, nbErreurMax, stadeActuelPartie, grilleJeu, propJoueur, DEC_INT_ASCII >> Fin du tour

        // nbTour >> Passer au tour suivant >> nbTour
        nbTour = static_cast<unsigned short int>(nbTour + 1);

        // etatActuelProp, nbErreur, grilleJeu, propJoueur >> Adapter le résultat à l'état de la proposition >> [nbErreur], [grilleJeu]

        if (etatActuelProp == erreurSaisie || etatActuelProp == erreurGrilleDep || etatActuelProp == valIncomp)
        {
            // nbErreur >> Incrémenter nbErreur de 1 >> nbErreur
            nbErreur = static_cast<unsigned short int>(nbErreur + 1);
        }
        else if (etatActuelProp == valComp)
        {
            // grilleJeu >> Modifier la grille >> grilleJeu
            grilleJeu[propJoueur.abscisse - 1][propJoueur.ordonnee - 1] = char(propJoueur.element + DEC_INT_ASCII);
        }

        // nbErreur, nbErreurMax, stadeActuelPartie >> Vérifier le nombre d'erreurs >> [stadeActuelPartie]
        if (nbErreur == nbErreurMax)
        {
            stadeActuelPartie = maxErreur;
            cout << "P E R D U  ! ! ! plus de " << nbErreurMax << " erreurs" << endl;
        }
       
        if (nbTour > NB_TOUR_MIN_FIN)
        {
            // grilleJeu >> Vérifier si la grille est complète >> [stadeActuelPartie]
            if (! recherchePointGrille(grilleJeu))
            {
                effacer();
                // nbTour, nbErreur, nbErreurMax, grilleJeu, grilleBase, NB_CASES >> Afficher les règles du jeu, la grille et le menu d'évolution de la partie 

                // Afficher le titre en bleu et les règles du jeu
                afficherTitreRegles();

                // grilleJeu, grilleBase >> Afficher la grille
                afficherGrille(grilleJeu, grilleBase);

                // nbTour, nbErreur, nbErreurMax >> Afficher le menu d'évolution de la partie

                // nbTour >> Afficher le nombre de tours
                cout << " Tour " << nbTour << ",";

                // nbErreur, nbErreurMax >> Afficher le nombre d'erreurs
                cout << " Erreur : " << nbErreur << "/" << nbErreurMax;

                stadeActuelPartie = victoire;
                cout << "\n B R A V O  ! ! ! !";
            }
        }

        // Attendre que le joueur appuie sur une touche
        cout << "\n Appuyez sur une touche pour continuer ...";
        pause();
        effacer();
                
    } while (stadeActuelPartie == enCours);

    // Finaliser la partie

    // Afficher le titre et les règles du jeu
    afficherTitreRegles();

    // Afficher le résultat de la partie
    if (stadeActuelPartie == choixAbandon)
    {
        // Le joueur a abandonné
        cout << "Vous avez abandonne." << endl
             << "Fin de la partie\n";
    }
    else if (stadeActuelPartie == maxErreur)
    {
        // Le joueur a atteint son nombre maximum d'erreurs
        cout << "Vous avez fait " << nbErreurMax << " erreurs, vous avez perdu." << endl
             << "Fin de la partie\n";
    }
    else
    {
        // Le joueur a gagné
        cout << "Felicitations" << endl
             << "Vous avez completer la grille entierement, vous avez gagne." << endl
             << "Fin de la partie\n";
    }

    return 0;
}