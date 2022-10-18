#include <stdio.h>

void swap_pointers(int **p, int **q)
{
  int *f = *p;
  *p = *q;
  *q = f;
}

int main()
{
  int a = 1;
  int b = 2;
  int *p = &a;
  int *q = &b;
  printf("address of p = %p and q = %p\n", p, q);

  swap_pointers(&p, &q);

  printf("address of p = %p and q = %p\n", p, q);
}
