#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main ( void ) {
   FILE *fp;
   char str[] = "This is tutorialspoint.com";
    int a = 2;
   fp = fopen( "file.txt" , "wb" );
   fwrite( &a , sizeof(a) , 1    , fp );

   fclose(fp);
  
   return(0);
}