//共有メモリ領域の確保先変数nVar1とnVar2
int *nVar1;
int *nVar2;

//共有メモリ領域のサイズはnVar1とnVar2の分で型はint
int nShareMemSize = 2*sizeof(int);
