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


void simrand(unsigned int N, unsigned int p, char *filename);
void simmark(unsigned int N, unsigned int p, char *filename);
void stat(char *filename);

int main(int argc, char *const *argv) {
  // initialize the random number generator
  rand_init();

  int opt;
  unsigned int p = 10;
  unsigned int N = 0;
  unsigned int M = 0;
  int nf = 0, ir = 0, im = 0, is = 0;
  char * filename = NULL;
  while ((opt = getopt(argc, argv, "hr:m:sp"))!= -1){
    switch (opt) {
      case 'h':
        printf("Available command line parameters:\n -h\t (Print an overview of the availible command line parameters)\n -r N\t (Simulate N steps of the random surfer and output the result)\n -m N\t (Simulate N steps of the Markov chain and output the result)\n -s\t (Compute and print the statistic of the graph)\n -p  P\t (Set the parameter p to P percent, default: P = 10)");
        break;
      case 'r':
        N = atoi(optarg);
        ir = 1;
        nf = 1;
        break;
      case 'm':
        M = atoi(optarg);
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
    printf("Give file");
    //return(1);
  }
  if (ir){
    simrand(N,p,filename);
  }
  if (im){
    simmark(N,p,filename);
  }
  if (is){
    stat(filename);
  }

  // TODO: Implement me
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