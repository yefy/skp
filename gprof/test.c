#include <sys/time.h>
#include <stdio.h>


void longa()   
 {   
   int i,j;   
   for(i = 0; i < 10000; i++)   
   j=i; //am I silly or crazy? I feel boring and desperate.   
 }   
  
 void foo2()   
 {   
   int i;   
   for(i=0 ; i < 10; i++)   
        longa();   
 }   
  
 void foo1()   
 {   
   int i;   
   for(i = 0; i< 100; i++)   
      longa();   
 }   

 void foo()
 {
  int i = 0;
  for(i; i < 10000000; i++)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
}
 }
  
 int main(void)   
 {   
   foo();
   foo1();   
   foo2();   
 }
