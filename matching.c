#include <stdio.h>
#define NEWLINE 10

void print_matrix(short arr[5][5], char *row_label){
    /* prints out a 5x5 matrix of ints */
    short row, col;
    for (row = 0; row < 5; row++){
        printf("%s %d:   ", row_label, row+1);
        for (col = 0; col < 5; col++){
            printf("%d ", arr[row][col]);
        }
        printf("\n");
    }
}

void print_array(short arr[5]){
    printf("{");
    for (short i = 0; i < 5; i++){
        if (i < 4 && i > 0)
            printf("%3d,", arr[i]);
        else if (i == 0)
            printf("%d,", arr[i]);
        else
            printf("%3d", arr[i]);
    }
    printf("}\n");
}

short in(short value, short arr[5]){
    for (short i = 0; i < 5; i++){
        if (arr[i] == value)
            return 1;
    }
    return 0;
}

short find_index(short value, short arr[5]){
    short index;
    for (short i = 0; i < 5; i++){
        if (arr[i] == value){
            index = i;
            break;
        }
    }
    return index;
}

short find_min_index(short arr[5]){
    short min_index = 0, min = arr[0];
    for (short i = 0; i < 5; i++){
        if (arr[i] < min){
            min = arr[i];
            min_index = i;
        }
    }
    return min_index;
}

void print_results(short depts[5][5], short progs[5][5]){
    /* find and print where each programmer should go */
    short final[5] = {-1, -1, -1, -1, -1};
    short i;


    short depts_list[5] = {0, 1, 2, 3, 4};
    short dept_indecies[5] = {0, 0, 0, 0, 0};
    short prog;
    short dept;

    short picks[5];

    //main loop
    for (i = 0; i < 5; i++){
        //update the current top picks first
        //loop through all the depts
        for (short dept = 0; dept < 5; dept++)
            //if the department hasn't been assigned a programmer yet
            if (in(dept, depts_list))
                //set their top available pick to their spot in picks[]
                picks[dept] = depts[dept][dept_indecies[dept]];

            else
                //if the dept has been assigned a prog, we don't care about it
                picks[dept] = -1;

        //find the programmer we're looking at
        //loop through depts again
        for (short dept = 0; dept < 5; dept++){
            //the first time we find a department that needs a programmer
            if (picks[dept] != -1){
                //set the working prog to that programmer
                prog = picks[dept];
                //and break out
                break;
            }
        }

        //now we need a list of all the depts that want that prog
        short clash_list[5] = {-1, -1, -1, -1, -1};
        short wanted_count = 0;
        for (short dept = 0; dept < 5; dept++){
            //if a department wants that prog
            if (picks[dept] == prog){
                //add it to the list of depts that want that prog
                clash_list[wanted_count] = dept;
                //and increment the counter of how many depts wanted that prog
                wanted_count++;
            }
        }

        //now the hard part. check for a clash. if there was one, sort it out.
        //if not, just give the dept the prog.
        
        //pref_dept is going to have the final department that the programmer
        //will end up in
        short pref_dept;
        if (wanted_count > 1){
            //there was a clash, ask the programmer who they want
            
            short j = 0;
            short dept_ranks[5] = {6, 6, 6, 6, 6};
            dept = clash_list[j];
            //loop through the deptartments in clash_list
            while (dept != -1){
                //find the index of that dept in the programmer's list of
                //preferred departments and add it to a list
                dept_ranks[dept] = find_index(dept, progs[prog]);

                j++;
                dept = clash_list[j];
            }
            //the primo deptartment is the one with the lowest index in the
            //programmer's preferences
            pref_dept = find_min_index(dept_ranks);
            
            j = 0;
            dept = clash_list[j];
            //we have to increment what the next best preference for each
            //department is now that this programmer has been taken
            //so loop through clash_list again
            while (dept != -1){
                //if the department we're looking at isn't the lucky one that
                //got the programmer
                if (dept != pref_dept){
                    //go to the next available preferred prog of that department
                    dept_indecies[dept]++;
                    //if that programmer is already being used, increment again
                    while (in(depts[dept][dept_indecies[dept]], final))
                        dept_indecies[dept]++;
                }

                j++;
                dept = clash_list[j];
            }


        } else {
            //there was no clash, the dept gets the programmer
            pref_dept = clash_list[0];
        }
        
        //update the final programmer in the preffered department to the current
        //programmer
        final[pref_dept] = prog;
        //remove the department from the list of available departments
        depts_list[pref_dept] = -1;
    }

    //print out the final picks
    for (i = 0; i < 5; i++)
        printf("Department #%d will get Programmer #%d\n", i+1, final[i]+1);
    

}

int main(){
    short departments[5][5];
    short programmers[5][5];

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
    short row = 0, col = 0;
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
                    departments[col/2][row] = cur_char - '0' - 1;
                else 
                    //same same for programmers. row % 5 gets the relative index
                    //of the row. this accounts for the departments matrix being
                    //above the programmers matrix in the input file.
                    programmers[col/2][row % 5] = cur_char - '0' - 1;
            }
            col++;
        }

        col = 0;
        row++;
    }

    print_results(departments, programmers);

    return 0;
}
