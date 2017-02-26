#include <sys/ipc.h>
#include <sys/shm.h>
#define TRUE 1
#define FALSE  0
#define PROCESS1 0
#define PROCESS2 1
int *naWantPossess;
int *nPriority;
int *naA;
int *naB;
int *naC;

int *nMoves;
int *nDisks;
