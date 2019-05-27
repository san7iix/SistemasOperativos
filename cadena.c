#include <stdio.h>
#include <string.h>

int main()
{
   char s1[13] = "Hola a todos";
   char s2[3] = "la";

   printf( "s1=%s\n", s1 );
   printf( "s2=%s\n", s2 );
   printf( "strstr(s1,s2) = %s\n", strstr( s1, s2 ) );

   return 0;
}
