#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char *err_msg = 
"Invalid Usage.\n\n\
Usage: ./a.out <no_of_row> <no_of_col> <elements_of array_in_row_order>\n\
Example: ./a.out 2 3 1 2 3 3 4 5\n";

int main(int argc, char *argv[])
{
    int row1 =4, cols1=3,row2=3,cols2=6, sum=0; 
    int m[10][10], m2[10][10]; 
    int multi[10][10]; 
    int index =1, index2 = 13; 

    printf("input matrix 1\n");
    for(int i=0; i<row1; i++) {
        for(int j=0; j<cols1; j++){
            m[i][j] = atoi(argv[index]);
            index++;
        }
    }
    for (int c = 0; c < row1; c++) {
      for (int d = 0; d < cols1; d++)
        printf("%d\t", m[c][d]);
 
      printf("\n");
    }
    printf("input matrix 2\n");
     for(int i=0; i<row2; i++) {
        for(int j=0; j<cols2; j++){
            m2[i][j] = atoi(argv[index2]);
            index2++;
        }
     }
     for (int c = 0; c < row2; c++) {
      for (int d = 0; d < cols2; d++)
        printf("%d\t", m2[c][d]);

      printf("\n");
    }
 
    for (int c = 0; c < row1; c++) {
      for (int d = 0; d < cols2; d++) {
        for (int k = 0; k < row2; k++) {
          sum = sum + m[c][k]*m2[k][d];
        }
 
        multi[c][d] = sum;
        sum = 0;
      }
    }
 
    printf("Product:\n");
 
    for (int c = 0; c < row1; c++) {
      for (int d = 0; d < cols2; d++)
        printf("%d\t", multi[c][d]);
 
      printf("\n");
    }
    
    return 0;
}

    