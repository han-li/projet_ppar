#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//#ifdef _OPENMP
#include <omp.h>
//#endif

#ifdef DEBUG
#define DEBUG_PRINT(x,y) printf("%s %d\n",x,y)
#else
#define DEBUG_PRINT(x,y) do{}while(0)
#endif

#define TOTAL 1572864
#define TAG 1

/* global size pour thread */
int sizex,*tabx;
pthread_t pid[4];


/* afficher le tableau */
void print_tab(int*tab,int size);

/* creer un tab qui contient size chiffres aleatoire */
int *create_random_tab(int size);

/* si a > b, echange les deux */
void compare_exchange(int *a,int* b);
void exchange(int *a,int* b);

/* trier le tab par la facon pair_impair*/
void sort(int *tab,int size);

/* trier par PRAM */
void sort2(int *tab,int size);

/* trier par PRAM en openmp */
void sort_omp(int *tab,int size);

/* verifier le tab soit bien trie ou pas */
int verify(int *tab,int size);

/* pour tous les donnees de tab1 et tab2 tries, tab1 garde les plus petits, tab2 garde les plus grands */
void separate(int **tab1,int **tab2,int size1,int size2);

/* tri_fusion */
void scinder(int *tab,int size,int **tab2,int *size1,int *size2);
void fusion(int *tab1,int *tab2,int size1,int size2);
void trifusion_tmp(int *tab,int size);
void trifusion(int *tab,int size);

/* tri_rapide */
int choix_pivot(int *tab,int first,int last);
int partitionner(int *tab,int first,int last,int pivot);
void trirapide_tmp(int *tab,int first,int last);
void trirapide(int *tab,int size);

/* verifier la fonction separate(...) */
int verify2(int *tab1,int *tab2,int size1,int size2);

/* preter la fonction de tme */
double my_gettimeofday();

/* trier le tab en façon de pair_impair en parallel */
void *sort_thread(void* arg);

void separate_thread(int *tab,int size);
