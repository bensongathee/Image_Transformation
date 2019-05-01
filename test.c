#include <stdio.h>
#include <string.h>

double nfmod(double a,double b)
{
    return a - b * floor(a / b);
}
int main () {
   printf("%lf", nfmod(.5,6));
   return(0);
}