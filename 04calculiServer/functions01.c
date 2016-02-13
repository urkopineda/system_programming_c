#include "functions01.h"

unsigned int gcd(unsigned int a, unsigned int b)
{
  unsigned int tmp;

  tmp=a%b;
  while(tmp!=0)
  {
    a=b;
    b=tmp;
    tmp=a%b;
  }
  return b;
}

unsigned int lcm(unsigned int a, unsigned int b)
{
  return a/gcd(a,b)*b;
}

