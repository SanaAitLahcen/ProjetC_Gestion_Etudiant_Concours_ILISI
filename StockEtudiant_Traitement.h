#include"StockEtudiant_Struct.h"

// Nettoyer la chaine
void nettoyerChaine(char* chaine) 
{
  if (!chaine || strlen(chaine) == 0) return;

  int debut = 0, fin = strlen(chaine) - 1;
  while (isspace(chaine[debut]) || chaine[debut] == '"') debut++;
  while (fin >= debut && (isspace(chaine[fin]) || chaine[fin] == '"')) fin--;

  int len = fin - debut + 1;
  if (debut > 0) memmove(chaine, chaine + debut, len);
  chaine[len] = '\0';

  for (int i = 0; chaine[i]; i++) 
  {
      chaine[i] = toupper((unsigned char)chaine[i]);
  }
}


/********************************************************************************************************************************************/


//Cette fonction permet de comparer deux chaînes de caractères en ignorant les différences entre majuscules et minuscules.
int strcasecmp_portable(const char *s1, const char *s2) 
{
  int i = 0;
  while (tolower((unsigned char)s1[i]) == tolower((unsigned char)s2[i]) && s1[i] != '\0' && s2[i] != '\0') 
  {
      i++;
  }
  return tolower((unsigned char)s1[i]) - tolower((unsigned char)s2[i]);
}




/********************************************************************************************************************** */




// Function to load faculties from file
int chargerFacultes(const char* nomFichier) 
{
  FILE* fichier = fopen(nomFichier, "r");
  if (!fichier) 
  {
      perror("Erreur lors de l'ouverture du fichier des facultés");
      return -1;
  }

  char ligne[MAX_LINE_LENGTH];
  int count = 0;

  // First, count the number of lines in the file
  while (fgets(ligne, sizeof(ligne), fichier) != NULL) 
  {
      count++;
  }
  rewind(fichier); // Reset file pointer to the beginning

  // Allocate memory for the faculties array
  facultes = (InfoFaculte*)malloc(sizeof(InfoFaculte) * count);
  if (!facultes) 
  {
      perror("Erreur d'allocation de mémoire pour les facultés");
      fclose(fichier);
      return -1;
  }

  // Read the faculties from the file
  int i = 0;
  while (fgets(ligne, sizeof(ligne), fichier) != NULL && i < count) 
  {
      // Remove trailing newline character
      size_t len = strlen(ligne);
      if (len > 0 && ligne[len - 1] == '\n') 
      {
          ligne[len - 1] = '\0';
      }
      strncpy(facultes[i].nom, ligne, MAX_NAME_LENGTH - 1);
      facultes[i].nom[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null termination
      i++;
  }
  numFacultes = i;

  fclose(fichier);
  return 0;
}



/**********************************************************************************************************************/


// Function to load universities from file
int chargerUniversites(const char* nomFichier) 
{
  FILE* fichier = fopen(nomFichier, "r");
  if (!fichier) 
  {
      perror("Erreur lors de l'ouverture du fichier des universités");
      return -1;
  }

  char ligne[MAX_LINE_LENGTH];
  int i = 0;

  while (fgets(ligne, sizeof(ligne), fichier) != NULL && i < MAX_UNIVERSITIES) 
  {
      // Remove trailing newline character
      size_t len = strlen(ligne);
      if (len > 0 && ligne[len - 1] == '\n') 
      {
          ligne[len - 1] = '\0';
      }

      // Tokenize the line using comma as delimiter
      char* token = strtok(ligne, ";");
      if (token != NULL) 
      {
          strncpy(universities[i][0], token, MAX_NAME_LENGTH - 1);
          universities[i][0][MAX_NAME_LENGTH - 1] = '\0';

          token = strtok(NULL, ";");
          if (token != NULL) 
          {
              strncpy(universities[i][1], token, MAX_NAME_LENGTH - 1);
              universities[i][1][MAX_NAME_LENGTH - 1] = '\0';
          } 
          else 
          {
              printf("Warning: Missing filename for university %s\n", universities[i][0]);
              universities[i][1][0] = '\0'; // Set filename to empty string
          }
          i++;
      } 
      else 
      {
          printf("Warning: Invalid line in universities file: %s\n", ligne);
      }
  }
  numUniversities = i;

  fclose(fichier);
  return 0;
}





/******************************************************************************************************************************/


// Fonction pour trouver index de la faculté
int trouverIndiceFaculte(const char* faculte) 
{
  char facNettoyee[MAX_NAME_LENGTH];
  strcpy(facNettoyee, faculte);
  nettoyerChaine(facNettoyee);

  for (int i = 0; i < numFacultes; i++) 
  {
      if (strcasecmp_portable(facNettoyee, facultes[i].nom) == 0) 
      {
          return i;
      }
  }
  return -1;
}




/******************************************************************************************************************************/

// Fonction pour trouver index de l'université
int trouverIndiceUniversite(const char* universite) 
{
  char uniNettoyee[MAX_NAME_LENGTH];
  strcpy(uniNettoyee, universite);
  nettoyerChaine(uniNettoyee); // Nettoyer la chaîne (supprimer les espaces et convertir en majuscules)

  for (int i = 0; i < numUniversities; i++) 
  {
      char uniPredefinieNettoyee[MAX_NAME_LENGTH];
      strcpy(uniPredefinieNettoyee, universities[i][0]);
      nettoyerChaine(uniPredefinieNettoyee); // Nettoyer le nom prédéfini

      if (strcasecmp_portable(uniNettoyee, uniPredefinieNettoyee) == 0) 
      {
          return i; // Université trouvée
      }
  }
  return -1; // Université non trouvée
}



/******************************************************************************************************************************/

int etudiantExisteDansFichier(const char* nomFichier, Etudiant* etudiant) 
{
FILE* fichier = fopen(nomFichier, "r");
if (!fichier) 
{
    return 0;
}

char ligne[2048];
while (fgets(ligne, sizeof(ligne), fichier)) 
{
    char nom[50], prenom[50];
    int annebac;
    char option[50];
    float notes[4];
    float s5 = 0.0, s6 = 0.0;
    int num_fields = sscanf(ligne, "%49[^;];%49[^;];%d;%49[^;];%f;%f;%f;%f;%f;%f",
        nom, prenom, &annebac, option, &notes[0], &notes[1], &notes[2], &notes[3], &s5, &s6);

    if (num_fields >= 8) 
    {
        // Nettoyer les chaînes avant de comparer
        Etudiant etudiant_temp;
        strcpy(etudiant_temp.nom, nom);
        strcpy(etudiant_temp.prenom, prenom);
        etudiant_temp.annebac = annebac;
        strcpy(etudiant_temp.option, option);

        nettoyerChaine(etudiant_temp.nom);
        nettoyerChaine(etudiant_temp.prenom);
        nettoyerChaine(etudiant_temp.option);

        // Journalisation des données
        // printf("Ligne du fichier: Nom=%s, Prenom=%s, AnneeBac=%d, Option=%s\n", nom, prenom, annebac, option);
        // printf("Etudiant à comparer: Nom=%s, Prenom=%s, AnneeBac=%d, Option=%s\n", etudiant->nom, etudiant->prenom, etudiant->annebac, etudiant->option);

        // Comparer les données nettoyées
        if (strcasecmp_portable(etudiant_temp.nom, etudiant->nom) == 0 &&
            strcasecmp_portable(etudiant_temp.prenom, etudiant->prenom) == 0 &&
            annebac == etudiant->annebac &&
            strcasecmp_portable(etudiant_temp.option, etudiant->option) == 0) 
        {
            fclose(fichier);
            printf("Doublon trouvé: %s %s\n", etudiant->prenom, etudiant->nom);
            return 1; // L'étudiant existe
        }
    }
}

fclose(fichier);
return 0; // L'étudiant n'existe pas
}


/******************************************************************************************************************************/


// Fonction pour crée les étudiants dans leur fichiers
void ecrireEtudiantDansFichier(Etudiant* etudiant, const char* nomFichier, int niveau) 
{
  // Vérifier si l'étudiant existe déjà
  if (etudiantExisteDansFichier(nomFichier, etudiant)) 
  {
      printf("Student %s %s already exists in %s\n", etudiant->prenom, etudiant->nom, nomFichier);
      return;
  }

  FILE* fichier = fopen(nomFichier, "a");
  if (!fichier) 
  {
      perror("Erreur lors de l'ouverture du fichier");
      return;
  }

  // Vérifier si le fichier est vide
  fseek(fichier, 0, SEEK_END);
  long tailleFichier = ftell(fichier);
  if (tailleFichier == 0) 
  {
      // Écrire l'en-tête du fichier CSV
      fprintf(fichier, "Nom,Prenom,AnneeBac,OptionDiplome(DernierDiplome),NoteS1,NoteS2,NoteS3,NoteS4");
      if (niveau == 1) 
      {
          fprintf(fichier, ",NoteS5,NoteS6\n");
      } 
      else 
      {
          fprintf(fichier, "\n");
      }
  }

  // Écrire les données de l'étudiant
  fprintf(fichier, "%s,%s,%d,%s,%.2f,%.2f,%.2f,%.2f",
      etudiant->nom, etudiant->prenom, etudiant->annebac,
      etudiant->option, etudiant->notes[0], etudiant->notes[1],
      etudiant->notes[2], etudiant->notes[3]);

  // Si l'étudiant est Bac+3, ajouter les notes S5 et S6
  if (niveau == 1 && etudiant->licence) 
  {
      fprintf(fichier, ",%.2f,%.2f\n", etudiant->licence->notes[0], etudiant->licence->notes[1]);
  } 
  else 
  {
      fprintf(fichier, "\n");
  }

  fclose(fichier);
}




/******************************************************************************************************************************/


void nettoyerNomFichier(char* nom) 
{
for (int i = 0; nom[i]; i++) 
{
    if (isspace(nom[i]) || nom[i] == '/') 
    {
        nom[i] = '_';
    }
}
}

/******************************************************************************************************************************/


void determinerEtEcrireFichier(Etudiant* etudiant, const char* universite, const char* faculte, int niveau) 
{
  int facIndex = trouverIndiceFaculte(faculte);
  int uniIndex = trouverIndiceUniversite(universite);
  char nomFichier[MAX_NAME_LENGTH];
  char universiteNettoyee[MAX_NAME_LENGTH];

  // Nettoyer le nom de l'université
  strcpy(universiteNettoyee, universite);
  nettoyerNomFichier(universiteNettoyee);

  if (facIndex != -1) 
  {
      // Faculté trouvée, écrire dans le fichier correspondant
      if (niveau == 0) 
      {
          sprintf(nomFichier, "FaculteBac+2/%s_%s Bac+2.csv", universiteNettoyee, facultes[facIndex].nom);
      } 
      else 
      {
          sprintf(nomFichier, "FaculteBac+3/%s_%s Bac+3.csv", universiteNettoyee, facultes[facIndex].nom);
      }
      printf("Writing to file: %s\n", nomFichier);
      ecrireEtudiantDansFichier(etudiant, nomFichier, niveau);
  } 
  else if (uniIndex != -1) 
  {
      // Université trouvée, mais faculté non trouvée, écrire dans Autre/<Université>_Autre.csv
      sprintf(nomFichier, "Autre/%s_Autre.csv", universities[uniIndex][0]);
      printf("Writing to file: %s\n", nomFichier);
      ecrireEtudiantDansFichier(etudiant, nomFichier, niveau);
  } 
  else 
  {
      // Université non trouvée, écrire dans Autre/Autre.csv
      strcpy(nomFichier, "Autre/Autre.csv");
      printf("Writing to file: %s\n", nomFichier);
      ecrireEtudiantDansFichier(etudiant, nomFichier, niveau);
  }
}



/******************************************************************************************************************************/

// Fonction pour lire un fichier CSV
void lireCSV(const char* fichier) 
{
  // Ouvrir le fichier en mode lecture
  FILE* file = fopen(fichier, "r");
  if (!file) 
  {
      perror("Erreur lors de l'ouverture du fichier CSV");
      return;
  }

  char ligne[2048];
  fgets(ligne, sizeof(ligne), file); // Ignorer l'en-tête du fichier CSV

  // Lire chaque ligne du fichier
  while (fgets(ligne, sizeof(ligne), file)) 
  {
      // Allouer dynamiquement de la mémoire pour un étudiant
      Etudiant* etudiant = (Etudiant*)malloc(sizeof(Etudiant));
      if (!etudiant) 
      {
          perror("Erreur d'allocation mémoire pour un étudiant");
          fclose(file);
          return;
      }
      memset(etudiant, 0, sizeof(Etudiant));
      etudiant->svt = NULL;

      // Variables temporaires pour stocker les valeurs extraites
      char universite[MAX_NAME_LENGTH], faculte[MAX_NAME_LENGTH], niveauStr[50];
      float s5 = 0.0, s6 = 0.0;

      int field = 0; // Indice du champ actuel
      int i = 0;
      int start = 0;
      int len = strlen(ligne);
      int in_quotes = 0; // Vérifier si on est à l'intérieur d'une chaîne entre guillemets

      // Parcourir la ligne pour extraire les valeurs
      while (i <= len) 
      {
          // Si on rencontre un séparateur (;) ou la fin de la ligne
          if (ligne[i] == ';' || ligne[i] == '\0') 
          {
              if (!in_quotes) 
              {
                  char token[MAX_NAME_LENGTH];
                  strncpy(token, &ligne[start], i - start);
                  token[i - start] = '\0';
                  nettoyerChaine(token); // Nettoyer la chaîne extraite

                  // Associer les valeurs aux champs correspondants de l'étudiant
                  switch (field) {
                  case 2:  // Prénom
                      strncpy(etudiant->prenom, token, sizeof(etudiant->prenom) - 1);
                      break;
                  case 3:  // Nom
                      strncpy(etudiant->nom, token, sizeof(etudiant->nom) - 1);
                      break;
                  case 15: // Année du BAC
                      etudiant->annebac = atoi(token);
                      break;
                  case 19: // Type de diplôme
                      strncpy(etudiant->typediplome, token, sizeof(etudiant->typediplome) - 1);
                      break;
                  case 21: // Option du diplôme
                      strncpy(etudiant->option, token, sizeof(etudiant->option) - 1);
                      break;
                  case 22: // Université
                      strncpy(universite, token, sizeof(universite) - 1);
                      break;
                  case 23: // Faculté
                      strncpy(faculte, token, sizeof(faculte) - 1);
                      break;
                  case 18: // Niveau du diplôme
                      strncpy(niveauStr, token, sizeof(niveauStr) - 1);
                      break;
                  case 33: // Note S1
                      etudiant->notes[0] = atof(token);
                      break;
                  case 34: // Note S2
                      etudiant->notes[1] = atof(token);
                      break;
                  case 35: // Note S3
                      etudiant->notes[2] = atof(token);
                      break;
                  case 36: // Note S4
                      etudiant->notes[3] = atof(token);
                      break;
                  case 37: // Note S5
                      s5 = atof(token);
                      break;
                  case 38: // Note S6
                      s6 = atof(token);
                      break;
                  }
                  field++;
                  start = i + 1;
              }
          }
          else if (ligne[i] == '"') 
          {
              in_quotes = !in_quotes; // Basculer l'état si on rencontre un guillemet
          }
          i++;
      }

      // Déterminer le niveau du diplôme
      int niveau = -1;
      nettoyerChaine(niveauStr);

      if (strstr(niveauStr, "BAC+2") != NULL) 
      {
          niveau = 0;
      }
      else if (strstr(niveauStr, "BAC+3") != NULL) 
      {
          niveau = 1;
      }
      
      //si il s'agit d'un autre Bac+ il va stcoker dans répertoire Autre selon ca faculté et université
      else 
      {     
          printf("Niveau non reconnu: %s\n", niveauStr);
          free(etudiant);
          continue;
      }

      // Si l'étudiant a un niveau BAC+3, allouer une structure Bac3
      if (niveau == 1) 
      {
          etudiant->licence = (Bac3*)malloc(sizeof(Bac3));
          if (!etudiant->licence) 
          {
              perror("Erreur d'allocation mémoire pour la licence");
              free(etudiant);
              fclose(file);
              return;
          }
          memset(etudiant->licence, 0, sizeof(Bac3));
          strcpy(etudiant->licence->typedep, etudiant->typediplome);
          strcpy(etudiant->licence->optiondiplome, etudiant->option);
          etudiant->licence->notes[0] = s5;
          etudiant->licence->notes[1] = s6;
      }

      // Déterminer l'université et la faculté de l'étudiant et écrire dans le fichier correspondant
      determinerEtEcrireFichier(etudiant, universite, faculte, niveau);
      
      // Libération de la mémoire allouée dynamiquement
      if (etudiant->licence != NULL) 
      {
          free(etudiant->licence);
      }
      free(etudiant);
  }

  fclose(file); // Fermer le fichier après lecture
}
