#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
  printf("Using _exit...\n");
  printf("This is the end");
  _exit(0);
}
