#include <stdio.h>
#include <string.h>
#include <math.h>

int main () {
   FILE *fp;
   FILE *fp2;
   unsigned char ch[54];
   unsigned char buffer;
   int i = 0; 

   printf("%d\n", sizeof(buffer));
   /* Open file for both reading and writing */
   fp = fopen("red_tele.bmp", "r");
   fp2 = fopen("out.bmp", "r+");

   while(i < 54){
      fread(&buffer, 1, 1, fp);
      fwrite(&buffer, 1, 1, fp2);
      // printf("%i ",buffer);
      i++;
   }
   // printf("\n");
   // i = 0;
   // fseek(fp2, 0, SEEK_SET);
   // while(i < 54){
   //    fread(&buffer, 1, 1, fp2);
   //    printf("%i ",buffer);
   //    i++;
   //  }
   printf("\n");

   // fseek(fp2, 54, SEEK_SET);
   // fseek(fp, 54, SEEK_SET);
   // i = 54;
   while(i < 60){
      fread(&buffer, 1, 1, fp);
      fwrite(&buffer, 1, 1, fp2);
      printf("%i ", buffer);
      // printf("%i ",buffer);
      i++;
   }
   
   printf("\n\n");
   i = 0;
   fseek(fp2, 0, SEEK_SET);
   while(i < 60){
      fread(&buffer, 1, 1, fp2);
      printf("%i ",buffer);
      i++;
    }





   /* Write data to the file */
   // fwrite(c, strlen(c), 1, fp);

   /* Seek to the beginning of the file */
   // fseek(fp, 0, SEEK_SET);

   /* Read and display data */
   // fread(buffer, 1, 2, fp);
   // printf("%s\n", buffer);
   // fclose(fp);
   
   return(0);
}