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

typedef struct ver Way;
typedef struct edges Edge;
typedef struct knod Knode;
typedef struct probab Prob;

struct edges{
  char* src;
  char* dst;
  Edge* next;
};

struct knod{
  char* name;
  Knode* next;
  Way* wege;
  int anzout;
  int anzin;
};

struct ver{
  Knode* kn;
  int inc;
  int outg;
};

struct probab{
  Knode* kn;
  double p;
};


void simrand(unsigned int N, unsigned int p, Knode** klist, int anzv);
void simmark(unsigned int N, unsigned int p, char *filename);
void stat(Knode** klist, int anzv, int anze, char* g_name);
char* readgraph(char* filename, Edge** liste);
void freeedges (Edge** liste);
void printedges(Edge** liste);
Knode* erstelleknoten(char* nam);
void edgestograph(Knode** klist, Edge** liste, int* e);
void printknodes(Knode** klist, int anzv);
Knode* fuegeknotenhinzu(Knode** kliste, char* nam);
void verbindungen(Knode** klist, Edge** liste, int anzv);
void freeknodes(Knode** klist);
void anzahlderausgänge(Knode** klist, int anzv);
void anzahldereingänge(Knode** klist, int anzv);
int min(int a, int b);
int max(int a, int b);
Knode* nextknode(Knode* v, Knode** klist, int anzv);
Knode* bored(Knode** klist, int anzv);
Prob** wahrscheinlichkeitszuweisung(Knode** randlist, int anzv, Knode** klist, int N);

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

  if (nf == 1){
    Edge* x;
    Edge** liste = &x;
    *liste = NULL;
    char* g_name;
    Knode* y;
    Knode** klist = &y;
    *klist = NULL;
    int anze = 0;
    int anzv = 0;
    g_name = readgraph(filename, liste);
//    printedges(liste);
    edgestograph(klist, liste, &anze);
    if (klist != NULL && *klist != NULL){
      Knode* w = *klist;
      while (w != NULL){
        anzv++;
        w = w->next;
      }
    }
//    printf("Anzahl Knoten: %d\t Anzahl Wege: %d\n", anzv, anze);
    verbindungen(klist, liste, anzv);
    anzahlderausgänge(klist, anzv);
    anzahldereingänge(klist, anzv);
//    printknodes(klist, anzv);
    if (ir){
      simrand(N, P, klist, anzv);
    }
    if (im){
      simmark(N, P, filename);
    }
    if (is){
      stat(klist, anzv, anze, g_name);
    }
    free(g_name);
    freeknodes(klist);
    freeedges(liste);
  }
  exit(0);
}


void simrand(unsigned int N, unsigned int p, Knode** klist, int anzv){
  if (klist == NULL || *klist == NULL){
    return;
  }
  Knode* v;
  Knode** randlist = calloc(N-1, sizeof(Knode*));
  unsigned int k;
  unsigned int b;
  v = bored(klist, anzv);
  //v ist nun der Startknoten
  for (unsigned int i = 1; i < N; i++){
    if (v->anzout == 0){
      v = bored(klist, anzv);
    } else {
      k = 100/p;
      b = randu(k);
      if (b == 0){
        v = bored(klist, anzv);
      } else {
        v = nextknode(v, klist, anzv);
      }
    }
    randlist[i-1] = v;
  }
  Prob** pr = wahrscheinlichkeitszuweisung(randlist, anzv, klist, N-1);
  for(int i; i<anzv; i++){
    free(pr[i]);
  }
  free(pr);
  pr = NULL;
  free(randlist);
  randlist = NULL;
  return;
}

void simmark(unsigned int N, unsigned int p, char *filename){
  printf("m\n");
  return;
}

void stat(Knode** klist, int anzv, int anze, char* g_name){
  printf("%s:\n", g_name);
  printf("- num nodes: %d\n", anzv);
  printf("- num edges: %d\n", anze);
  int mi = 2147483647, Mi = 0, mo = 2147483647, Mo = 0;
  Knode* v = *klist;
  if (v == NULL){
    mi = 0;
    mo = 0;
  }
  while(v != NULL){
    mo = min(mo, v->anzout);
    Mo = max(Mo, v->anzout);
    v = v->next;
  }
  v = *klist;
  while(v != NULL){
    mi = min(mi, v->anzin);
    Mi = max(Mi, v->anzin);
    v = v->next;
  }
  printf("- indegree: %d-%d\n", mi, Mi);
  printf("- outdegree: %d-%d\n", mo, Mo);
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
  k->wege = NULL;
  return(k);
}

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

void printknodes(Knode** klist, int anzv){
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
    printf("Knoten: %s\t", w->name);
    for(int i = 0; i < anzv; i++){
      printf("Einkommend: %d\tAusgehend: %d\n", (w->wege + i)->inc, (w->wege + i)->outg);
    }
    w = w->next;
    n++;
  }
  return;
}

Knode* fuegeknotenhinzu(Knode** kliste, char* nam){
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

void verbindungen(Knode** klist, Edge** liste, int anzv){
  if(klist == NULL || *klist == NULL || liste == NULL || *liste == NULL){
    return;
  }
  Knode *k = *klist, *h;
  Edge* e;
  //Erstelle eine Liste mit Ways für jeden Knoten
  while(k != NULL){
    k->wege = calloc(anzv, sizeof(Way));
    if (k->wege == NULL){
      return;
    }
    h = *klist;
    for (int i = 0; i < anzv; i++){
      (k->wege + i)->kn = h;
      h = h->next;
    }
    k = k->next;
  }
  k = *klist;
  while(k != NULL){
    //Füge alle ausgehenden Ways in die Liste
    e = *liste;
    while(e != NULL){
      if (!strcmp(e->src, k->name)){
        for (int i = 0; i < anzv; i++){
          if(!strcmp(e->dst, ((k->wege + i)->kn)->name)){
            ((k->wege + i)-> outg) = ((k->wege + i)-> outg) + 1;
            break;
          }
        }
      }
      e = e->next;
    }
    //Füge alle eingehenden Ways in die Liste
    e = *liste;
    while(e != NULL){
      if (!strcmp(e->dst, k->name)){
        for (int i = 0; i < anzv; i++){
          if(!strcmp(e->src, ((k->wege + i)->kn)->name)){
            ((k->wege + i)-> inc) = ((k->wege + i)-> inc) + 1;
            break;
          }
        }
      }
      e = e->next;
    }
  k = k->next;
  }
  return;
}

void freeknodes(Knode** klist){
  if (klist == NULL){
    return;
  }
  Knode *v = *klist, *k;
  while(v != NULL){
    free(v->name);
    free(v->wege);
    k = v->next;
    v->next = NULL;
    free(v);
    v = k;
  }
  klist = NULL;
  return;
}

Knode* knotenzunamen(Knode** klist, char* nam){
  if (klist == NULL || *klist == NULL){
    return(NULL);
  }
  Knode* v = *klist;
  while(v != NULL){
    if(!strcmp(nam, v->name)){
      return(v); 
    }
    v = v->next;
  }
  printf("Not there!\n");
  return(NULL);
}

void anzahlderausgänge(Knode** klist, int anzv){
  if(klist == NULL || *klist == NULL){
    return;
  }
  Knode* v = *klist;
  int a;
  while(v != NULL){
    a = 0;
    for (int i = 0; i < anzv; i++){
      a = a + ((v->wege + i)->outg);
    }
    v->anzout = a;
    v = v->next;
  }
  return;
}

void anzahldereingänge(Knode** klist, int anzv){
  if(klist == NULL || *klist == NULL){
    return;
  }
  Knode* v = *klist;
  int a;
  while(v != NULL){
    a = 0;
    for (int i = 0; i < anzv; i++){
      a = a + ((v->wege + i)->inc);
    }
    v->anzin = a;
    v = v->next;
  }
  return;
}

int min(int a, int b){
  if(a<b){
    return(a);
  }
  return(b);
}

int max(int a, int b){
  if(a>b){
    return(a);
  }
  return(b);
}

Knode* bored(Knode** klist, int anzv){
  unsigned int k = anzv;
  unsigned int b = randu(k);
  Knode* v = *klist;
  while(b > 0){
    v = v->next;
    b--;
  }
  return(v);
}

Knode* nextknode(Knode* v, Knode** klist, int anzv){
  unsigned int k = v->anzout;
  unsigned int b = randu(k);
  b++;
  Way* w = v->wege;
  for(int i = 0; i < anzv; i++){
    if ((w + i)->outg < b){
      b = b - ((w + i)->outg);
    } else {
      return((w + i)->kn);
    }
  }
  return(NULL);
}

Prob** wahrscheinlichkeitszuweisung(Knode** randlist, int anzv, Knode** klist, int N){
  if (klist == NULL || *klist == NULL){
    return (NULL);
  }
  Prob** pr = calloc(anzv, sizeof(Prob*));
  Knode* v = *klist;
  Prob *pp = NULL;
  for (int i = 0; i<anzv; i++){
    pp = calloc(1, sizeof(Prob));
    pp->kn = v;
    *(pr + i) = pp;
    v = v->next;
   // printf("%s\n", (pr[i]->kn)->name);
  }
  for (int i = 0; i<anzv; i++){
    v = (*(pr + i))->kn;
   // printf("\n\n%s: ", v->name);
    (*(pr + i))->p = 0.0;
    for (int j = 0; j<N; j++){
      if (*(randlist + j) == v){
       // printf("%s - ", (*(randlist + j))->name);
        (*(pr + i))->p = (*(pr + i))->p + 1.0;
      }
    }
    (*(pr + i))->p = ((*(pr + i))->p)/N;
    printf("%s\t%.10f\n", ((*(pr + i))->kn)->name, (*(pr + i))->p);
   // printf("\n%.10f\n\n", (*(pr + i))->p);
  }
  return(pr);
}
