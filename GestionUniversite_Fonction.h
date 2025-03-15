
#include"GestionUniversite_Structure.h"


// Tableau pour stocker les données des universités
Universite DonneesUniversites[MAX_UNIVERSITIES];

// Variables pour gérer les facultés non prédéfinies
char **AutresFacultes = NULL; // Tableau dynamique pour les facultés non prédéfinies
int NombreAutresFacultes = 0; // Nombre actuel de facultés non prédéfinies
int CapaciteAutresFacultes = 0; // Capacité actuelle du tableau AutresFacultes

// Initialise une université avec une capacité initiale de 10 facultés
void InitialiserUniversite(int Index) 
{
    DonneesUniversites[Index].CapaciteFacultes = 10; // Capacité initiale
    DonneesUniversites[Index].NombreFacultes = 0;    // Aucune faculté au départ
    DonneesUniversites[Index].Facultes = (char**) malloc(DonneesUniversites[Index].CapaciteFacultes * sizeof(char*)); // Allocation mémoire
}

// Ajoute une faculté à une université donnée
void AjouterFaculte(Universite *Universite, const char *NomFaculte) 
{
    // Si le tableau est plein, double sa capacité
    if (Universite->NombreFacultes >= Universite->CapaciteFacultes) 
    {
        Universite->CapaciteFacultes *= 2;
        Universite->Facultes = (char**)realloc(Universite->Facultes, Universite->CapaciteFacultes * sizeof(char*));
    }
    // Ajoute la faculté au tableau
    Universite->Facultes[Universite->NombreFacultes++] = strdup(NomFaculte);
}

// Ajoute une faculté à la liste des autres facultés (non prédéfinies)
void AjouterAutreFaculte(const char *NomFaculte) 
{
    // Si le tableau est plein, double sa capacité
    if (NombreAutresFacultes >= CapaciteAutresFacultes) 
    {
        CapaciteAutresFacultes = CapaciteAutresFacultes ? CapaciteAutresFacultes * 2 : 10;
        AutresFacultes = (char**) realloc(AutresFacultes, CapaciteAutresFacultes * sizeof(char*));
    }
    // Ajoute la faculté au tableau
    AutresFacultes[NombreAutresFacultes++] = strdup(NomFaculte);
}

// Nettoie une chaîne de caractères en enlevant les espaces inutiles et les guillemets
void NettoyerChaine(char *Chaine) 
{
    if (!Chaine) return;

    int Longueur = strlen(Chaine);
    // Supprime les guillemets autour de la chaîne si présents
    if (Longueur > 1 && Chaine[0] == '"' && Chaine[Longueur - 1] == '"') 
    {
        memmove(Chaine, Chaine + 1, Longueur - 2);
        Chaine[Longueur - 2] = '\0';
    }

    // Supprime les espaces au début de la chaîne
    char *Debut = Chaine;
    while (isspace((unsigned char)*Debut)) Debut++;

    // Supprime les espaces à la fin de la chaîne
    char *Fin = Chaine + strlen(Debut) - 1;
    while (Fin > Debut && isspace((unsigned char)*Fin)) Fin--;
    *(Fin + 1) = '\0';

    // Déplace la chaîne nettoyée au début du tableau
    memmove(Chaine, Debut, Fin - Debut + 2);
}

// Trouve l'index d'une université dans le tableau des universités
int TrouverIndexUniversite(const char *NomUniversite) 
{
    for (int i = 0; i < MAX_UNIVERSITIES; i++) 
    {
        if (strcmp(Universites[i][0], NomUniversite) == 0) 
        {
            return i; // Retourne l'index si l'université est trouvée
        }
    }
    return -1; // Retourne -1 si l'université n'est pas trouvée
}

// Extrait un champ spécifique d'une ligne CSV
void ExtraireChamp(const char *Ligne, int ChampCible, char *Sortie) 
{
    int Index = 0, DansGuillemets = 0;
    const char *Debut = Ligne, *Fin = Ligne;

    // Parcourt la ligne pour trouver le champ cible
    while (*Fin && Index <= ChampCible) 
    {
        if (*Fin == '"' && (Fin == Ligne || *(Fin-1) != '\\')) 
        {
            DansGuillemets = !DansGuillemets; // Gère les guillemets
        } 
        else if (*Fin == ';' && !DansGuillemets) 
        {
            if (Index == ChampCible) break; // Champ cible trouvé
            Index++;
            Debut = Fin + 1;
        }
        Fin++;
    }

    // Copie le champ dans la sortie
    size_t Longueur = Fin - Debut;
    strncpy(Sortie, Debut, Longueur);
    Sortie[Longueur] = '\0';
    NettoyerChaine(Sortie); // Nettoie la chaîne extraite
}

// Traite une ligne du fichier CSV
void TraiterLigne(const char *Ligne) 
{
    char Universite[MAX_LINE_LENGTH], Faculte[MAX_LINE_LENGTH];

    // Extrait les champs "Université" et "Faculté" de la ligne
    ExtraireChamp(Ligne, 22, Universite);
    ExtraireChamp(Ligne, 23, Faculte);

    if (strlen(Faculte) == 0) return; // Ignore les lignes sans faculté

    // Trouve l'index de l'université
    int Index = TrouverIndexUniversite(Universite);
    if (Index != -1) 
    {
        // Vérifie si la faculté existe déjà dans l'université
        for (int i = 0; i < DonneesUniversites[Index].NombreFacultes; i++) 
        {
            if (strcmp(DonneesUniversites[Index].Facultes[i], Faculte) == 0) return;
        }
        // Ajoute la faculté à l'université
        AjouterFaculte(&DonneesUniversites[Index], Faculte);
    } 
    else 
    {
        // Vérifie si la faculté existe déjà dans la liste des autres facultés
        for (int i = 0; i < NombreAutresFacultes; i++) 
        {
            if (strcmp(AutresFacultes[i], Faculte) == 0) return;
        }
        // Ajoute la faculté à la liste des autres facultés
        AjouterAutreFaculte(Faculte);
    }
}

// Écrit les données des universités et des autres facultés dans des fichiers
void EcrireFichiers() 
{
    // Parcourt toutes les universités prédéfinies
    for (int i = 0; i < MAX_UNIVERSITIES; i++) 
    {
        // Construit le chemin du fichier dans le répertoire "Universites"
        char CheminFichier[MAX_LINE_LENGTH];
        snprintf(CheminFichier, sizeof(CheminFichier), "Universites/%s", Universites[i][1]);

        // Ouvre le fichier en mode écriture
        FILE *Fichier = fopen(CheminFichier, "w");
        if (!Fichier) 
        {
            printf("Erreur : Impossible de créer le fichier %s\n", CheminFichier);
            continue;
        }

        // Écrit les facultés dans le fichier
        for (int j = 0; j < DonneesUniversites[i].NombreFacultes; j++) 
        {
            fprintf(Fichier, "%s\n", DonneesUniversites[i].Facultes[j]);
        }
        fclose(Fichier);
    }

    // Écrit les autres facultés dans un fichier séparé
    FILE *FichierAutres = fopen("Universites/Autre.txt", "w");
    if (FichierAutres) 
    {
        for (int i = 0; i < NombreAutresFacultes; i++) 
        {
            fprintf(FichierAutres, "%s\n", AutresFacultes[i]);
        }
        fclose(FichierAutres);
    } 
    else 
    {
        printf("Erreur : Impossible de créer le fichier Universites/Autre.txt\n");
    }
}

// Libère la mémoire allouée pour les données des universités et des autres facultés
void LibererMemoire() 
{
    // Libère la mémoire pour chaque université
    for (int i = 0; i < MAX_UNIVERSITIES; i++) 
    {
        for (int j = 0; j < DonneesUniversites[i].NombreFacultes; j++) 
        {
            free(DonneesUniversites[i].Facultes[j]);
        }
        free(DonneesUniversites[i].Facultes);
    }
    // Libère la mémoire pour les autres facultés
    for (int i = 0; i < NombreAutresFacultes; i++) 
    {
        free(AutresFacultes[i]);
    }
    free(AutresFacultes);
}

// Initialise toutes les universités
void InitialiserToutesUniversites() 
{
    for (int i = 0; i < MAX_UNIVERSITIES; i++) 
    {
        InitialiserUniversite(i);
    }
}

// Traite un fichier CSV
void TraiterCSV(const char *NomFichier) 
{
    // Ouvre le fichier CSV en mode lecture
    FILE *CSV = fopen(NomFichier, "r");
    if (!CSV) return;

    char Ligne[MAX_LINE_LENGTH];
    fgets(Ligne, sizeof(Ligne), CSV); // Ignore l'en-tête

    // Lit chaque ligne du fichier CSV
    while (fgets(Ligne, sizeof(Ligne), CSV)) {
        TraiterLigne(Ligne); // Traite la ligne
    }
    fclose(CSV);
}