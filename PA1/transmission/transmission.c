#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * Represents one transmission pair
 */
struct s_transmission{
    int num1;
    int num2;
};

/**
 * Represents an array of s_transmission structures
 */
struct s_transArr{
    struct s_transmission* array;
    int size;
    int max;
};

/**
 * Function takes care of input control and adding data to the array
 * @param type
 * @param transArr
 * @param request
 * @return
 */
int input( bool* type, struct s_transArr* transArr, struct s_transmission* request ){
    int queryID = getchar();

    // input checking of the query identifier
    if( queryID == EOF ){
        printf("END EOF\n");
        return -1;
    }

    if( queryID != '+' && queryID != '?' ){
        printf("ERROR: ID not recognized\n");
        return 1;
    }

    if( queryID == '+' && *type ){
        printf("ERROR: Adding new data after first request\n");
        return 1;
    }
    if( queryID == '?' ){
        //printf("Changing ID\n");
        *type = true;
    }


    int num1, num2;
    if( scanf( "%d:%d\n", &num1, &num2 ) != 2 ){
        printf("ERROR: Scanf failed\n");
        return 1;
    }

    // add new entry to array
    if( queryID == '+' ){
        transArr->size ++;
        // reallocate if needed
        if( transArr->size == transArr->max ){
            transArr->max = transArr->max * 2;
            struct s_transmission* tmp = (struct s_transmission*) realloc( transArr->array ,transArr->max * sizeof(struct s_transmission) );
            if( tmp == NULL ){
                printf("Out of memory\n");
                exit(EXIT_FAILURE);
            }
            transArr->array = tmp;
        }
        struct s_transmission tmp_str;
        tmp_str.num1 = num1;
        tmp_str.num2 = num2;
        transArr->array[transArr->size - 1] = tmp_str;
        return 0;
    }

    request->num1 = num1;
    request->num2 = num2;
    return 0;
}

/**
 * Prints the given array
 */
void printArr( struct s_transArr* transArr ){
    printf("PRINT ARRAY\n");
    for( int i = 0; i < transArr->size; i++ ){
        printf("%d:%d\n", transArr->array[i].num1, transArr->array[i].num2 );
    }
}

int main(){
    bool type = false;
    struct s_transmission request;
    struct s_transArr transArr;
    transArr.max = 2;
    transArr.size = 0;
    transArr.array = (struct s_transmission*) malloc( transArr.max * sizeof(struct s_transmission) );

    printf("Prevody:\n");
    while( true ){
        int inputOut = input( &type, &transArr, &request );
        if( inputOut == -1 )
            break;
        if( inputOut == 1 ){
            printf("Nespravny vstup.\n");
            free( transArr.array );
            exit(EXIT_FAILURE);
        }



    }

    printArr( &transArr );
    free( transArr.array );
    exit(EXIT_SUCCESS);
}