#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int a(void) 
{
  int i=0,g=0;
  while(i++<100000)
  {
     g+=i;
  }
  return g;
}

int b(void)
{
  int i=0,g=0;
  while(i++<400000)
  {
    g +=i;
  }
  return g;
}
int main(int argc, char** argv)
{
   int iterations;
   if(argc != 2)
   {
      printf("Usage %s \n", argv[0]);
      exit(-1);
   }
   else
      iterations = atoi(argv[1]);
   printf("No of iterations = %d\n", iterations);
   while(iterations--)
   {
      a();
      b();
   }
return 0;
}
