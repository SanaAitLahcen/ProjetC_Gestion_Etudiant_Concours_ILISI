#ifndef STOCKETUDIANT_STRUCT_H // Garde d'inclusion
#define STOCKETUDIANT_STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Définir les constantes
#define MAX_UNIVERSITIES 50 // Ajuster si nécessaire
#define MAX_FACULTIES 100   // Ajuster si nécessaire
#define MAX_NAME_LENGTH 200
#define MAX_LINE_LENGTH 2048

// Définir les structures
typedef struct Bac3 
{
    char typedep[50];
    float notes[2];
    char optiondiplome[50];
} Bac3;

typedef struct Etd 
{
    char nom[50];
    char prenom[50];
    int annebac;
    char option[50];
    float notes[4];
    char typediplome[50];
    Bac3* licence;
    struct Etd* svt;
} Etudiant;

// Structure for faculty information
typedef struct 
{
    char nom[MAX_NAME_LENGTH];
} InfoFaculte;

// Global arrays to hold faculty and university data
InfoFaculte* facultes = NULL;
int numFacultes = 0;
char universities[MAX_UNIVERSITIES][2][MAX_NAME_LENGTH]; // [index][0=name, 1=filename]
int numUniversities = 0;

#endif 