#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "HanoiShareMem.h"

void AllocateShareMemory(int);
void Solve(int *,int *,int *,int);
void Initialize();
void SignalHandler(int);

int nShareMemID;

int main(int argc, char *argv[]) {

  int nRequestedNumDisks = 10;

  if (argc > 2) {
    printf("Usage: %s [ndisks]\n", argv[0]);
    exit(1);
  }

  //引数で指定された円盤数にする
  if (argc == 2) nRequestedNumDisks = atoi(argv[1]);

  //SIGINT, SIGQUIT, SIGTERMはSignalHandlerに処理させる
  //(中断した時に共有メモリ領域が開放されるように)
  signal(SIGINT , SignalHandler);
  signal(SIGQUIT, SignalHandler);
  signal(SIGTERM, SignalHandler);

  //共有メモリ確保と領域割り振り
  AllocateShareMemory(nRequestedNumDisks);

  //領域を割り振ったので変数を初期化
  *nMoves = 0;
  *nDisks = nRequestedNumDisks;
  Initialize();

  //Hanoiの塔を解く
  Solve(naA, naB, naC, *nDisks);

  //結果出力
  printf("Number of Moves: %d\n", *nMoves);
  printf("Number of Disks: %d\n", *nDisks);

  //共有メモリ領域開放
  shmctl(nShareMemID, IPC_RMID, 0);

  return 0;
}

void AllocateShareMemory(int nRequestedNumDisks) {

  //共有メモリ領域の大きさは (nDisksとnMoves + naAとnaBとnaC)
  int nShareMemSize = (5+3*nRequestedNumDisks)*sizeof(int);

  //共有メモリ領域要求, 領域のIDが返る
  nShareMemID =shmget(IPC_PRIVATE,nShareMemSize,SHM_R|SHM_W);
  printf("Share Memory ID = %d\n", nShareMemID);

  //メモリ領域割り振り
  nDisks = shmat(nShareMemID,0,SHM_R|SHM_W);
  if (nDisks == (void*)-1) {
    printf("shmat failed\n");
    exit(1);
  }

  //nMoves, naA, naB, naCにも割り振る
  nMoves=nDisks+1;
  naA=nMoves+2;
  naB=naA+nRequestedNumDisks;
  naC=naB+nRequestedNumDisks;
  naWantPossess=naC+nRequestedNumDisks;
  nPriority=naWantPossess+2;
}

void Initialize() {
  int i;

  for (i=0; i<*nDisks; ++i){
    naA[i] = *nDisks - i;
    naB[i] = 0;
    naC[i] = 0;
  }

}

void Solve(int *_naA, int *_naB, int *_naC, int n) {
//プロセス1が使用の意志を表示する
naWantPossess[PROCESS1] = TRUE; 

//プロセス2に優先権を与えて処理を促す
*nPriority = PROCESS2;    
 if (n) {

    Solve(_naA+1, _naC, _naB, n-1);

//プロセス2に使用意志があり かつ 優先権が有れば whileループで共有メモリへのアクセス保留
while (naWantPossess[PROCESS2] && *nPriority == PROCESS2) { }

/*** 共有メモリ資源へのアクセス ***/

    *_naB = *_naA;
    *_naA = 0;

  

//プロセス1が使用の意志を撤回する

naWantPossess[PROCESS1] = FALSE; 
   ++(*nMoves);

    Solve(_naC, _naB+1, _naA, n-1);

  }     
 

}

void SignalHandler(int h){

  //共有メモリ領域開放
  shmctl(nShareMemID, IPC_RMID, 0);

  exit(1);

}
