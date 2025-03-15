#include"GestionUniversite_Fonction.h"

int main() 
{
    // Initialise toutes les universités
    InitialiserToutesUniversites();
    // Traite le fichier CSV
    TraiterCSV("ILISI_2018_19.csv");
    // Écrit les données dans les fichiers
    EcrireFichiers();
    // Libère la mémoire allouée
    LibererMemoire();

    printf("Fichiers générés avec succès!\n");
    return 0;
}