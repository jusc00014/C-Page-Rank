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

//Struktur für Edges
typedef struct edges Edge;
struct edges{
  char* src;
  char* dst;
  Edge* next;
};

typedef struct kante Kanten;
struct kante{
  char* name;
  int anzin;
  int anzout;
};

void simrand(unsigned int N, unsigned int p, Edge** liste);
void simmark(unsigned int N, unsigned int p, Edge** liste);
void stat(Edge** liste);
int readgraph(char* filename, Edge** liste);
void add(char* y, char* z, Edge** liste);
Kanten** outingoingedges(Edge** liste, int anz);

int main(int argc, char *const *argv) {
  // initialize the random number generator
  rand_init();

  int opt = 0;
  unsigned int p = 10;
  unsigned int N = 0;
  unsigned int M = 0;
  int nf = 0, ir = 0, im = 0, is = 0;
  char *filename = NULL;
  //Einlesen der Argumente und des Files
  while ((opt = getopt(argc, argv, "hr:m:sp"))!= -1){
    switch (opt) {
      case 'h':
        printf("Available command line parameters:\n -h\t (Print an overview of the availible command line parameters)\n -r N\t (Simulate N steps of the random surfer and output the result)\n -m N\t (Simulate N steps of the Markov chain and output the result)\n -s\t (Compute and print the statistic of the graph)\n -p  P\t (Set the parameter p to P percent, default: P = 10)\n");
        break;
      case 'r':
        N = atoi(optarg);
        ir = 1;
        nf = 1;
        break;
      case 'm':
        M = atoi(optarg);
        im = 1;
        nf = 1;
        break;
      case 's':
        is = 1;
        nf = 1;
        break;
      case 'p':
        p = atoi(optarg);
        break;
    }
  }
  if (optind < argc){
    filename =argv[optind];
  }
  if (nf == 1 && filename == NULL){
    printf("Give file\n");
//=    return(1);
  }
  Edge* x;
  Edge** liste = &x;
  *liste = NULL;
  int anz;
  if (ir || im || is){
//=    anz = readgraph(filename, liste);
  }
//Zu Testzwecken
    Edge* x1 = calloc(1, sizeof(Edge));
    x1->src = calloc(257,sizeof(char));
    x1->dst = calloc(257,sizeof(char));
    strcpy(x1->src, "CMS");
    strcpy(x1->dst, "dCMS");
    Edge* x2 = calloc(1, sizeof(Edge));
    x2->src = calloc(257,sizeof(char));
    x2->dst = calloc(257,sizeof(char));
    strcpy(x2->src, "dCMS");
    strcpy(x2->dst, "dGit");
    Edge* x3 = calloc(1, sizeof(Edge));
    x3->src = calloc(257,sizeof(char));
    x3->dst = calloc(257,sizeof(char));
    strcpy(x3->src, "dCMS");
    strcpy(x3->dst, "dGit");
    Edge* x4 = calloc(1, sizeof(Edge));
    x4->src = calloc(257,sizeof(char));
    x4->dst = calloc(257,sizeof(char));
    strcpy(x4->src, "dCMS");
    strcpy(x4->dst, "guide");
    Edge* x5 = calloc(1, sizeof(Edge));
    x5->src = calloc(257,sizeof(char));
    x5->dst = calloc(257,sizeof(char));
    strcpy(x5->src, "dCMS");
    strcpy(x5->dst, "forum");
    Edge* x6 = calloc(1, sizeof(Edge));
    x6->src = calloc(257,sizeof(char));
    x6->dst = calloc(257,sizeof(char));
    strcpy(x6->src, "dCMS");
    strcpy(x6->dst, "leaderboard");
    Edge* x7 = calloc(1, sizeof(Edge));
    x7->src = calloc(257,sizeof(char));
    x7->dst = calloc(257,sizeof(char));
    strcpy(x7->src, "leaderborad");
    strcpy(x7->dst, "dCMS");
    Edge* x8 = calloc(1, sizeof(Edge));
    x8->src = calloc(257,sizeof(char));
    x8->dst = calloc(257,sizeof(char));
    strcpy(x8->src, "leaderboard");
    strcpy(x8->dst, "dGit");
    Edge* x9 = calloc(1, sizeof(Edge));
    x9->src = calloc(257,sizeof(char));
    x9->dst = calloc(257,sizeof(char));
    strcpy(x9->src, "leaderboard");
    strcpy(x9->dst, "dGit");
    Edge* x10 = calloc(1, sizeof(Edge));
    x10->src = calloc(257,sizeof(char));
    x10->dst = calloc(257,sizeof(char));
    strcpy(x10->src, "guide");
    strcpy(x10->dst, "forum");
    Edge* x11 = calloc(1, sizeof(Edge));
    x11->src = calloc(257,sizeof(char));
    x11->dst = calloc(257,sizeof(char));
    strcpy(x11->src, "forum");
    strcpy(x11->dst, "guide");
    Edge* x12 = calloc(1, sizeof(Edge));
    x12->src = calloc(257,sizeof(char));
    x12->dst = calloc(257,sizeof(char));
    strcpy(x12->src, "forum");
    strcpy(x12->dst, "dCMS");
    liste = &x1;
    x1->next = x2;
    x2->next = x3;
    x3->next = x4;
    x4->next = x5;
    x5->next = x6;
    x6->next = x7;
    x7->next = x8;
    x8->next = x9;
    x9->next = x10;
    x10->next = x11;
    x11->next = x12;
    x12->next = NULL;
  Kanten** k = outingoingedges(liste, 24);
  for (int i=0; i<24; i++){
    printf("%s %d %d\n", (k[i]->name), (k[i]->anzin), (k[i]->anzout));
  }
//



  if (ir){
    simrand(N,p,liste);
  }
  if (im){
    simmark(M,p,liste);
  }
  if (is){
    stat(liste);
  }
  exit(0);
}

void simrand(unsigned int N, unsigned int p, Edge** liste){
  printf("r\n");
  return;
}
void simmark(unsigned int N, unsigned int p, Edge** liste){
  printf("m\n");
  return;
}
void stat(Edge** liste){

  printf("s\n");
  return;
}

//Implementierung als Edges
int readgraph(char* filename, Edge** liste){
  //Überprüfen der File Struktur
  FILE *data = fopen(filename, "r");
  if (data == NULL){
    printf("File not readable!\n");
    return(0);
  }
  char y[258];
  char z[258];
  char arr[3];
  if (fscanf(data, "%258s", y) != 1)
  {
    fclose(data);
    printf("File has wrong format!\n");
    return(0);
  }
  if (strcmp(y, "digraph"))
  {
    printf("File has wrong format!\n");
    fclose(data);
    return(0);
  }
  char g_name[258];
  if (fscanf(data, "%258s", g_name) != 1)
  {
    fclose(data);
    printf("File has wrong format!\n");
    return(0);
  }
  if (fscanf(data, "%258s", y) != 1)
  {
    fclose(data);
    printf("File has wrong format!\n");
    return(0);
  }
  if (strcmp(y, "{"))
  {
    fclose(data);
    printf("File has wrong format!\n");
    return(0);
  }
  //Einlesen der Kanten
  int anz = 0;
  while ((fscanf(data, "%258s", y) != 1)&&(strcmp(y, "}"))){
    anz++;
    //Pfeil einlesen
    if (fscanf(data, "%3s", arr) != 1)
    {
      fclose(data);
      printf("File has wrong format!\n");
      return(0);
    }
    if (strcmp(arr, "->"))
    {
      fclose(data);
      printf("File has wrong format!\n");
      return(0);
    }
    //Ziel einlesen
    if (fscanf(data, "%[^;]", z) != 1)
    {
      fclose(data);
      printf("File has wrong format!\n");
      return(0);
    }
    //Daten hinzufügen
    Edge* x = calloc(1, sizeof(Edge));
    x->src = calloc(257,sizeof(char));
    x->dst = calloc(257,sizeof(char));
    strcpy(x->src, y);
    strcpy(x->dst, z);
    x->next = NULL;
    if (*liste == NULL)
    {
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
  return (anz*2);
}


Kanten** outingoingedges(Edge** liste, int anz){
  //Lege eine Liste mit allen Kanten an
  Kanten** statistic = calloc(anz, sizeof(Kanten*));
  //Alle Werte in statistic sind nun Kanten
  for (int i=0; i<anz; i++){
    Kanten* k = calloc(1, sizeof(Kanten));
    k->name = calloc(256, sizeof(char));
    k->anzin = 0;
    k->anzout = 0;
    statistic[i] = k;
  }
  //Iteriere über alle Edges und schaue, ob src und dst schon in unserer Liste sind, wenn nicht füge sie hinzu
  Edge* z = *liste;
  while (z->next != NULL){
    for (int i=0; i<anz; i++){
      if (statistic[i] == NULL || statistic[i]->name == NULL){
        break;
      }
      if (! strcmp(statistic[i]->name, z->src)){
        (statistic[i]->anzout) = (statistic[i]->anzout)+1;
        break;
      }
      if ((statistic[i]->name)[0] == '\0'){
        strcpy(statistic[i]->name, z->src);
        (statistic[i]->anzout) = 1;
        break;
      }
    }
    for (int i=0; i<anz; i++){
      if (! strcmp(statistic[i]->name, z->dst)){
        (statistic[i]->anzin) = (statistic[i]->anzin)+1;
        break;
      }
      if ((statistic[i]->name)[0] == '\0'){
        strcpy(statistic[i]->name, z->src);
        (statistic[i]->anzin) = 1;
        break;
      }
    }
    z = z->next;
  }
      printf("%s\n", z->dst);
    for (int i=0; i<anz; i++){
      if (statistic[i] == NULL || statistic[i]->name == NULL){
        break;
      }
      if (! strcmp(statistic[i]->name, z->src)){
        (statistic[i]->anzout) = (statistic[i]->anzout)+1;
        break;
      }
      if ((statistic[i]->name)[0] == '\0'){
        strcpy(statistic[i]->name, z->src);
        (statistic[i]->anzout) = 1;
        break;
      }
    }
    for (int i=0; i<anz; i++){
      if (! strcmp(statistic[i]->name, z->dst)){
        (statistic[i]->anzin) = (statistic[i]->anzin)+1;
        break;
      }
      if ((statistic[i]->name)[0] == '\0'){
        strcpy(statistic[i]->name, z->src);
        (statistic[i]->anzin) = 1;
        break;
      }
    }
  return(statistic);
}