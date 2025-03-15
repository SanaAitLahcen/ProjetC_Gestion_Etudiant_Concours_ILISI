#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 200

// Fonction pour vider un fichier
void viderFichier(const char* chemin) 
{
    FILE* f = fopen(chemin, "w");
    if (f) {
        fclose(f); // Ouvre et ferme pour vider
        printf("Fichier vidé : %s\n", chemin); // Message de débogage
    } 
    else 
    {
        printf("Erreur : Impossible de vider le fichier %s\n", chemin);
    }
}

// Fonction pour vider tous les fichiers dans un répertoire (Windows)
void viderRepertoire(const char* cheminRepertoire, const char* motif) 
{
    char commande[MAX_NAME_LENGTH];
    snprintf(commande, sizeof(commande), "dir /B %s\\%s", cheminRepertoire, motif);

    FILE* pipe = _popen(commande, "r");
    if (!pipe) 
    {
        printf("Erreur : Impossible d'ouvrir le répertoire %s\n", cheminRepertoire);
        return;
    }

    char ligne[MAX_NAME_LENGTH];
    while (fgets(ligne, sizeof(ligne), pipe)) 
    {
        // Supprimer le saut de ligne à la fin
        size_t len = strlen(ligne);
        if (len > 0 && ligne[len - 1] == '\n') 
        {
            ligne[len - 1] = '\0';
        }

        // Construire le chemin complet du fichier
        char cheminFichier[MAX_NAME_LENGTH];
        snprintf(cheminFichier, sizeof(cheminFichier), "%s\\%s", cheminRepertoire, ligne);

        // Vider le fichier
        viderFichier(cheminFichier);
    }

    _pclose(pipe);
}

void viderContenuFichiers() 
{
    // Vider les fichiers dans FaculteBac+2
    viderRepertoire("FaculteBac+2", "*.txt");

    // Vider les fichiers dans FaculteBac+3
    viderRepertoire("FaculteBac+3", "*.txt");

    // Vider les fichiers dans Autre
    viderRepertoire("Autre", "*.txt");
}