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

void simrand(unsigned int N, unsigned int p, char *filename);
void simmark(unsigned int N, unsigned int p, char *filename);
void stat(char *filename);
char* readgraph(char* filename, Edge** liste);
void freeedges (Edge** liste);
void printedges(Edge** liste);

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

  if (nf == 1){
    g_name = readgraph(filename, liste);
  }

  printedges(liste);
  printedges(liste);
  printedges(liste);
  printedges(liste);

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

//Implementierung als Edges
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
    if (fscanf(data, "%256[^;]", z) != 1) {
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