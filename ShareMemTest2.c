#include "ShareMemTest.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int nShareMemID;

int main(int argc, char *argv[]) {

  if (argc != 2) return 0;

  //共有メモリIDを引数から取る
  nShareMemID = atoi(argv[1]);
  printf("nShareMemID = %d\n", nShareMemID);

  //共有メモリIDから共有メモリ領域を得る
  nVar1 = shmat(nShareMemID, 0, SHM_R|SHM_W);
  nVar2 = nVar1 + 1;

  printf("process2 : nVar1 = %d, nVar2 = %d\n", *nVar1, *nVar2);

  //nVar1の中身を2に書き換えてみる
  *nVar1 = 2;
  printf("process2 : nVar1 = %d, nVar2 = %d\n", *nVar1, *nVar2);

  return 0;

}
