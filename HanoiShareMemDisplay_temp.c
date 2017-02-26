#include <stdio.h>
#include <stdlib.h>
#include "HanoiShareMem.h"

void Display();
void AllocateShareMemory(int nShareMemID);

int main(int argc, char *argv[]) {

  int nShareMemID;

  if (argc != 2) {
    printf("Usage: %s ShareMemoryID\n", argv[0]);
    exit(1);
  }

  //引数で指定された円盤数にする
  nShareMemID = atoi(argv[1]);

  //共有メモリ確保と領域割り振り
  AllocateShareMemory(nShareMemID);

  //結果出力
  printf("Number of Moves: %d\n", *nMoves);
  printf("Number of Disks: %d\n", *nDisks);
  Display();

  return 0;
}

void AllocateShareMemory(int nShareMemID) {

  //メモリ領域割り振り
  nDisks = shmat(nShareMemID,0,SHM_R|SHM_W);

  if (nDisks == (void*)-1) {
    printf("Share memory cannot be referred\n");
    exit(1);
  }

  //nMoves, naA, naB, naCにも割り振る
   nMoves=nDisks+1;
  naA=nMoves+2;
  naB=naA+(*nDisks);
  naC=naB+(*nDisks);
  naWantPossess=naC+(*nDisks);
  nPriority=naWantPossess+2;
}

void Display(void){
  int i;
//プロセス2が使用の意志を表示する
naWantPossess[PROCESS2] = TRUE; 

//プロセス1に優先権を与えて処理を促す
*nPriority = PROCESS1;    

//プロセス1に使用意志があり かつ 優先権が有れば whileループで共有メモリへのアクセス保留
while (naWantPossess[PROCESS1] && *nPriority == PROCESS1) { }

/*** 共有メモリ資源へのアクセス ***/
printf("Move: %d\n\n", *nMoves);
  for(i=*nDisks-1; i>=0; --i){
    printf("|%2d|   |%2d|   |%2d|\n", naA[i], naB[i], naC[i]);
  }

  printf("----   ----   ----\n");
  printf("  A      B      C\n");


//プロセス2が使用の意志を撤回する
naWantPossess[PROCESS2] = FALSE; 
}
