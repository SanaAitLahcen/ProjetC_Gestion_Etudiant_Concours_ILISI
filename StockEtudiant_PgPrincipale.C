#include"StockEtudiant_Traitement.h"
#include"ViderContenuFichier.h"


int main(int argc, char *argv[]) 
{
    // Vérifier le nombre d'arguments
    if (argc != 4) 
    {
        printf("Usage: %s <fichier_facultes> <fichier_universites> <fichier_csv>\n", argv[0]);
        return 1;
    }

    // Charger les facultés
    if (chargerFacultes(argv[1]) != 0) 
    {
        return 1;
    }

    // Charger les universités
    if (chargerUniversites(argv[2]) != 0) 
    {
        return 1;
    }

    // Vider le contenu des fichiers de sortie
    viderContenuFichiers();

    // Lire le fichier CSV
    lireCSV(argv[3]);

    // Libérer la mémoire allouée
    if (facultes != NULL) 
    {
        free(facultes);
    }

    return 0;
}