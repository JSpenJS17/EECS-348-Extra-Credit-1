#include <stdio.h>
#define NEWLINE 10

void print_matrix(int arr[5][5], char *row_label){
    /* prints out a 5x5 matrix of ints */
    int row, col;
    for (row = 0; row < 5; row++){
        printf("%s %d:   ", row_label, row+1);
        for (col = 0; col < 5; col++){
            printf("%d ", arr[row][col]);
        }
        printf("\n");
    }

}

int main(){
    int departments[5][5];
    int programmers[5][5];

    /* get input file name */
    char file_name[50];
    FILE *file;
    do {
        printf("Input file name: ");
        scanf("%s", file_name);
        file = fopen(file_name, "r");
    } while (file == NULL);
    

    /* load the values into arrays */

    char cur_char;
    char line[50];
    int row = 0, col = 0;
    //loop through all lines of the input file
    while (fgets(line, sizeof(line), file) != NULL){

        //loop through the characters of the input file
        while (*(line + col)){
            cur_char = *(line + col);
            if (cur_char != ' ' && cur_char != NEWLINE){
                if (row < 5)
                    //col/2 is the index of the array absent the spaces in the
                    //input file. assign departments[col][row] to rotate the
                    //input 90 degrees to get a more usable value. this makes
                    //departments[0] equal to just the first departments values
                    departments[col/2][row] = cur_char - '0';
                else 
                    //same same for programmers. row % 5 gets the relative index
                    //of the row. this accounts for the departments matrix being
                    //above the programmers matrix in the input file.
                    programmers[col/2][row % 5] = cur_char - '0';
            }
            col++;
        }

        col = 0;
        row++;
    }

    print_matrix(departments, "Dept.");

    printf("--------------------\n");

    print_matrix(programmers, "Prog.");
    return 0;
}
