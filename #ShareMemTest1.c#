#include "ShareMemTest.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int nShareMemID;

int main() {

  //共有メモリ領域確保を要求し IDを覚えておく
  nShareMemID = shmget(IPC_PRIVATE, nShareMemSize, SHM_R|SHM_W);
  printf("nShareMemID = %d\n", nShareMemID);

  //ポインタ変数nVar1にshmatで共有メモリ領域を割り当てる
  nVar1 = shmat(nShareMemID, 0, SHM_R|SHM_W);

  //ポインタ変数nVar2のための領域は, nVar1のアドレスから1つ先にあるのでそこを指す
  nVar2 = nVar1 + 1;

  //nVar1とnVar2に共有メモリ領域を割り当てたので 使えるようになる
  *nVar1 = 1;
  *nVar2 = 2;
  printf("process1 : nVar1 = %d\nnVar2 = %d\n", *nVar1, *nVar2);

  //別ターミナルからipcsを実行し共有メモリ領域が確保されたのを確認するために 終わるのを遅らせる
  sleep(20);

  printf("process1 : nVar1 = %d\nnVar2 = %d\n", *nVar1, *nVar2);

  //共有メモリ領域開放
  shmctl(nShareMemID, IPC_RMID, 0);

  return 0;

}
