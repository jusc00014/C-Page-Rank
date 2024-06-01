/*
 * main.c
 *
 * Programming 2 - Project 3 (PageRank)
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "utils.h"

typedef struct edges Edge;
struct edges{
  char* src;
  char* dst;
  Edge* next;
};

typedef struct knod Knode;
struct knod{
  char* name;
  Knode* next;
  int* incomming;
  int* outgoing;
  int anzout;
};



void simrand(unsigned int N, unsigned int p, char *filename);
void simmark(unsigned int N, unsigned int p, char *filename);
void stat(char *filename);
char* readgraph(char* filename, Edge** liste);
void freeedges (Edge** liste);
void printedges(Edge** liste);
Knode* erstelleknoten(char* nam);
void edgestograph(Knode** klist, Edge** liste, int* e);
void printknodes(Knode** klist);
Knode* fuegeknotenhinzu(Knode** kliste, char* nam);

int main(int argc, char *const *argv) {
  // initialize the random number generator
  rand_init();

  int opt;
  unsigned int P = 10;
  unsigned int N = 0;
  unsigned int M = 0;
  int nf = 0, ir = 0, im = 0, is = 0;
  char * filename = NULL;
  while ((opt = getopt(argc, argv, "hr:m:sp:"))!= -1){
    switch (opt) {
      case 'h':
        printf("Available command line parameters:\n -h\t (Print an overview of the availible command line parameters)\n -r N\t (Simulate N steps of the random surfer and output the result)\n -m N\t (Simulate N steps of the Markov chain and output the result)\n -s\t (Compute and print the statistic of the graph)\n -p  P\t (Set the parameter p to P percent, default: P = 10)\n");
        break;
      case 'r':
        if(optarg != NULL) {
          N = atoi(optarg);
        } else {
          fprintf(stderr, "Option -r requires an argument.\n");
          exit(EXIT_FAILURE);
        }
        ir = 1;
        nf = 1;
        break;
      case 'm':
        if(optarg != NULL) {
          M = atoi(optarg);
        } else {
          fprintf(stderr, "Option -m requires an argument.\n");
          exit(EXIT_FAILURE);
        }
        im = 1;
        nf = 1;
        break;
      case 's':
        is = 1;
        nf = 1;
        break;
      case 'p':
        if(optarg != NULL) {
          P = atoi(optarg);
        } else {
          fprintf(stderr, "Option -p requires an argument.\n");
          exit(EXIT_FAILURE);
        }
        break;
    }
  }

  if (optind < argc){
    filename = argv[optind];
  }

  if (nf == 1 && filename == NULL){
    printf("Give file\n");
    return(1);
  }

  Edge* x;
  Edge** liste = &x;
  *liste = NULL;
  char* g_name;
  Knode* y;
  Knode** klist = &y;
  *klist = NULL;
  int anze;

  if (nf == 1){
    g_name = readgraph(filename, liste);
  }

  printedges(liste);
  edgestograph(klist, liste, &anze);
  printknodes(klist);

  if (ir){
    simrand(N, P, filename);
  }
  if (im){
    simmark(N, P, filename);
  }
  if (is){
    stat(filename);
  }
  freeedges(liste);
  free(g_name);
  exit(0);
}

void simrand(unsigned int N, unsigned int p, char *filename){
  printf("r\n");
  return;
}

void simmark(unsigned int N, unsigned int p, char *filename){
  printf("m\n");
  return;
}

void stat(char *filename){
  printf("s\n");
  return;
}

char* readgraph(char* filename, Edge** liste){
  //Überprüfen der File Struktur
  FILE *data = fopen(filename, "r");
  if (data == NULL){
    printf("File not readable!\n");
    return(NULL);
  }
  char* y = (char*)calloc(257, sizeof(char));
  if (y == NULL){
    printf("Speicherfehler y\n");
    fclose(data);
    return(NULL);
  }
  char* z = (char*)calloc(257, sizeof(char));
  if (z == NULL){
    printf("Speicherfehler z\n");
    free(y);
    fclose(data);
    return(NULL);
  }
  char arr[4];
  if (fscanf(data, "%256s", y) != 1)
  {
    fclose(data);
    printf("File has wrong format!\n");
    free(z);
    free(y);
    return(NULL);
  }
  if (strcmp(y, "digraph"))
  {
    printf("File has wrong format!\n");
    free(z);
    free(y);
    fclose(data);
    return(NULL);
  }
  char *g_name = (char*)calloc(257, sizeof(char));
  if (g_name == NULL)
  {
    free(z);
    free(y);
    free(g_name);
    printf("Speicherfehler g_name\n");
    fclose(data);
    return(NULL);
  }
  if (fscanf(data, "%256s", g_name) != 1) {
    free(z);
    free(y);
    free(g_name);
    fclose(data);
    printf("File has wrong format!\n");
    return(NULL);
  }
  if (fscanf(data, "%256s", y) != 1) {
    free(z);
    free(y);
    free(g_name);
    fclose(data);
    printf("File has wrong format!\n");
    return(NULL);
  }
  if (strcmp(y, "{")) {
    free(z);
    free(y);
    free(g_name);
    fclose(data);
    printf("File has wrong format!\n");
    return(NULL);
  }
  //Einlesen der Kanten
  while ((fscanf(data, "%256s", y) != 1)||(strcmp(y, "}"))){
    //Pfeil einlesen
    if (fscanf(data, "%3s", arr) != 1) {
      free(y);
      free(z);
      free(g_name);
      freeedges(liste);
      fclose(data);
      printf("File has wrong format!\n");
      return(NULL);
    }
    if (strcmp(arr, "->")) {
      free(y);
      free(z);
      free(g_name);
      freeedges(liste);
      fclose(data);
      printf("File has wrong format!\n");
      return(NULL);
    }
    //Ziel einlesen
    if (fscanf(data, " %256[^;]", z) != 1) {
      free(y);
      free(z);
      free(g_name);
      freeedges(liste);
      fclose(data);
      printf("File has wrong format!\n");
      return(NULL);
    }
    char semi[3];
    if (fscanf(data, "%2s", semi) != 1 || strcmp(semi, ";")) {
      free(y);
      free(z);
      free(g_name);
      freeedges(liste);
      fclose(data);
      printf("File has wrong format!\n");
      return(NULL);
    }
    //Daten hinzufügen
    Edge* x = (Edge*)calloc(1, sizeof(Edge));
    if (x == NULL) {
      free(y);
      free(z);
      free(g_name);
      freeedges(liste);
      printf("Speicherfehler Edge x\n");
      fclose(data);
      return(NULL);
    }
    x->src = (char*)calloc(257,sizeof(char));
    if (x->src == NULL) {
      free(y);
      free(z);
      free(g_name);
      freeedges(liste);
      printf("Speicherfehler x->src\n");
      fclose(data);
      return(NULL);
    }
    x->dst = (char*)calloc(257,sizeof(char));
    if (x->dst == NULL) {
      free(y);
      free(z);
      free(g_name);
      freeedges(liste);
      printf("Speicherfehler x->dst\n");
      fclose(data);
      return(NULL);
    }
    strcpy(x->src, y);
    strcpy(x->dst, z);
    x->next = NULL;
    if (*liste == NULL) {
      *liste = x;
    }
    else{
      Edge* t = *liste;
      while(t->next != NULL){
        t = t->next;
      }
      t->next = x;
    }
  }
  free(y);
  free(z);
  fclose(data);
  return (g_name);
}

void freeedges (Edge** liste){
  if (*(liste) == NULL || liste == NULL){
    return;
  }
  Edge *w = *liste, *k;
  while (w != NULL){
    free(w->src);
    free(w->dst);
    k = w->next;
    w->next = NULL;
    free(w);
    w = k;
  }
  *liste = NULL;
  return;
}

void printedges(Edge** liste){
  if (*(liste) == NULL || liste == NULL){
    return;
  }
  Edge *w = *liste;
  while (w != NULL){
    printf("Src: %s\t", w->src);
    printf("Dst: %s\n", w->dst);
    w = w->next;
  }
  return;
}

Knode* erstelleknoten(char* nam){
  Knode* k = calloc(1,sizeof(Knode));
  if (k == NULL)
  {
    printf("Speicherfehler k\n");
    return(NULL);
  }
  k->name = calloc(257,sizeof(char));
  if (k->name == NULL)
  {
    free(k);
    printf("Speicherfehler k->name\n");
    return(NULL);
  }
  strcpy(k->name, nam);
  k->next = NULL;
  k->anzout = 0;
  k->incomming = NULL;
  k->outgoing = NULL;
  return(k);
}
/*
void freeknodes(Knode **klist) {
    if (klist == NULL || *klist == NULL) {
        return;
    }

    Knode *w = *klist;
    In *i1, *i2;
    Out *o1, *o2;

    while (w != NULL) {
        // Gebe Liste der Incoming Edges frei
        i1 = w->incomming;
        while (i1 != NULL) {
            i1->kn = NULL;
            i2 = i1->next;
            i1->next = NULL;
            free(i1);
            i1 = i2;
        }

        // Gebe Liste der Outgoing Edges frei
        o1 = w->outgoing;
        while (o1 != NULL) {
            o1->kn = NULL;
            o2 = o1->next;
            o1->next = NULL;
            free(o1);
            o1 = o2;
        }

        // Gebe Knoten frei
        Knode *nächster = w->next;
        free(w->name); // Angenommen, es gibt ein Feld 'name' im Knode, das auch freigegeben werden muss
        w->next = NULL;
        free(w);
        w = nächster;
    }

    // Setze den Listenkopf auf NULL, da die Liste jetzt leer ist
    *klist = NULL;
}*/
/*
Knode* fugeknotenhinzu(Knode** kliste, char* nam){
  Knode* z;
  if (kliste == NULL){
    return(NULL);
  }
  if (*kliste == NULL){
    z = erstelleknoten(nam);
    *kliste = z;
    return(z);
  }
  z = *kliste;
  //printf("415 ok\n");
  int u = 0;
  printf("z: %s\n", z->name);
  while(z->next != NULL && *(z->next) != NULL){
    //printf("417 ok\n");
    u++;
    if (u>10){
      break;
    }
    if (!strcmp(z->name, nam)){
      //printf("419 ok\n");
      return(z);
    }
    //printf("422 ok\n");
    printf("%s\n", z->name);
    z = *(z->next);
    printf("%s\n", z->name);
    //printf("424 ok\n");
  }
  Knode* x = erstelleknoten(nam);
  *(z->next) = x;
  return(x);
}
*/
void edgestograph(Knode** klist, Edge** liste, int* e){
  if (liste == NULL || *liste == NULL){
    return;
  }
  Edge* w = *liste;
  Knode *k, *h;
  *e = 0;
  while(w != NULL){
    *e = *e +1;
    if (w->src == NULL || w->dst == NULL){
      printf("Fehler in den edges");
      return;
    }
    k = fuegeknotenhinzu(klist, w->src);
    h = fuegeknotenhinzu(klist, w->dst);
    w = w->next;
  }
  return;
}

void printknodes(Knode** klist){
  if (klist == NULL || *klist == NULL){
    return;
  }
  Knode* w = *klist;
  int n = 1;
  while(w != NULL){
    if (w->name == NULL){
      printf("Fehler in den Knoten");
      return;
    }
    printf("%d:\t%s\n", n, w->name);
    w = w->next;
    n++;
  }
  return;
}
/*
Knode* fuegeknotenhinzu(Knode** kliste, char* nam){
  if (kliste == NULL){
    return(NULL);
  }
  Knode *k, *v;
  if(*kliste == NULL){
    printf("Erstes Element: %s\n", nam);
    k = erstelleknoten(nam);
    *kliste = k;
    return(k);
  }
  k = *kliste;
  while(k->next != NULL){
    if (k->name == NULL){
      printf("Knotenfehler");
      return(NULL);
    }
    printf("\n505 ok\n\n");
    if(!strcmp(k->name, nam)){
      printf("Nichts neues %s\n", nam);
      return(k);
    }
    k = k->next;
  }
  if (k->name == NULL){
    printf("Knotenfehler");
    return(NULL);
  }
  if(!strcmp(k->name, nam)){
    printf("Nichts neues %s\n", nam);
    return(k);
  }
  v = erstelleknoten(nam);
  printf("Neuer Knoten: %s\n", nam);
  k->next = v;
  return(v);
}*/

Knode* fuegeknotenhinzu(Knode** kliste, char* nam){
  Knode* z;
  if (*kliste == NULL){
    z = erstelleknoten(nam);
    *kliste = z;
    return(z);
  }
  z = *kliste;
  while(z->next != NULL){
    if (!strcmp(z->name, nam)){
      return(z);
    }
    z = z->next;
  }
  if (!strcmp(z->name, nam)){
    return(z);
  }
  Knode* x = erstelleknoten(nam);
  z->next = x;
  return(x);
}
