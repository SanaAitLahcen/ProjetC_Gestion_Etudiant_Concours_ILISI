#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

// Constantes pour la taille maximale des lignes et le nombre d'universités
#define MAX_LINE_LENGTH 1024
#define MAX_UNIVERSITIES 12

// Structure pour représenter une université
typedef struct 
{
    char **Facultes;          // Tableau dynamique de noms de facultés
    int NombreFacultes;       // Nombre actuel de facultés
    int CapaciteFacultes;     // Capacité actuelle du tableau Facultes
} Universite;

// Tableau des universités prédéfinies avec leurs noms et fichiers associés
const char *Universites[MAX_UNIVERSITIES][2] = 
{
  {"UNIVERSITE ABDELMALEK ESSAADI TANGER", "UNIVERSITE_ABDELMALEK_ESSAADI_TANGER.txt"},
  {"UNIVERSITE CADI AYYAD MARRAKECH", "UNIVERSITE_CADI_AYYAD_MARRAKECH.txt"},
  {"UNIVERSITE CHOUAIB DOUKKALI EL JADIDA", "UNIVERSITE_CHOUAIB_DOUKKALI_EL_JADIDA.txt"},
  {"UNIVERSITE HASSAN 1ER SETTAT", "UNIVERSITE_HASSAN_1ER_SETTAT.txt"},
  {"UNIVERSITE HASSAN II DE CASABLANCA", "UNIVERSITE_HASSAN_II_DE_CASABLANCA.txt"},
  {"UNIVERSITE IBN TOFAIL KENITRA", "UNIVERSITE_IBN_TOFAIL_KENITRA.txt"},
  {"UNIVERSITE IBN ZOHR AGADIR", "UNIVERSITE_IBN_ZOHR_AGADIR.txt"},
  {"UNIVERSITE MOHAMMED 1ER OUJDA", "UNIVERSITE_MOHAMMED_1ER_OUJDA.txt"},
  {"UNIVERSITE MOHAMMED V RABAT", "UNIVERSITE_MOHAMMED_V_RABAT.txt"},
  {"UNIVERSITE MOULAY ISMAIL MEKNES", "UNIVERSITE_MOULAY_ISMAIL_MEKNES.txt"},
  {"UNIVERSITE SIDI MOHAMMED BEN ABDELLAH FES", "UNIVERSITE_SIDI_MOHAMMED_BEN_ABDELLAH_FES.txt"},
  {"UNIVERSITE SULTAN MOULAY SLIMANE BENI MELLAL", "UNIVERSITE_SULTAN_MOULAY_SLIMANE_BENI_MELLAL.txt"}
};