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
    struct s_transmission array[50];
    int size;
};

/**
 * Prints the given array
 */
void printArr( struct s_transArr* transArr ){
    printf("PRINT ARRAY\n");
    for( int i = 0; i < transArr->size; i++ ){
        printf("%d:%d\n", transArr->array[i].num1, transArr->array[i].num2 );
    }
}

/**
 * Function takes care of input control and adding data to the array
 * @param type
 * @param transArr
 * @param request
 * @return -1 if EOF occurred, 1 if wrong input has been detected, 0 if addition has been executed, -2 if request
 * has been detected
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
        if( num1 < 10 || num1 > 1000 || num2 < 10 || num2 > 1000 ){
            printf("ERROR: Numbers out of range\n");
            return 1;
        }
        transArr->array[transArr->size].num1 = num1;
        transArr->array[transArr->size].num2 = num2;
        transArr->size ++;
        return 0;
    }

    if( num1 <= 0 || num2 <= 0 ){
        printf("ERROR: Numbers out of range\n");
        return 1;
    }
    request->num1 = num1;
    request->num2 = num2;
    return -2;
}

/**
 * Get gcd of two numbers
 * @param a
 * @param b
 * @return
 */
int gcd(int a, int b){
    if (a == 0)
        return b;
    return gcd(b % a, a);
}

/**
 * Simplifies a fraction
 * @param fr1
 */
void simplifyFraction( struct s_transmission* fr1 ){
    int gcdFr = gcd( fr1->num1, fr1->num2 );
    fr1->num1 = fr1->num1 / gcdFr;
    fr1->num2 = fr1->num2 / gcdFr;
}

/**
 * Multiplies two fractions
 * @param fr1
 * @param fr2
 * @return
 */
struct s_transmission multiplyFractions( struct s_transmission fr1, struct s_transmission fr2 ){
    struct s_transmission frOut;
    frOut.num1 = fr1.num1 * fr2.num1;
    frOut.num2 = fr1.num2 * fr2.num2;
    return frOut;
}

/**
 * Finds the closest combination of transmissions from the given array
 * @return
 */
int findTrans( struct s_transArr* transArr, struct s_transmission* request ){
    simplifyFraction( request );
    if( request->num1 == 1 && request->num2 == 1 )
        return 0;
    return 0;
}

int main(){
    bool type = false;
    struct s_transmission request;
    struct s_transArr transArr;
    transArr.size = 0;

    printf("Prevody:\n");
    while( true ){
        int inputOut = input( &type, &transArr, &request );
        if( inputOut == -1 )
            break;
        if( inputOut == 1 ){
            printf("Nespravny vstup.\n");
            exit(EXIT_FAILURE);
        }

        if( inputOut == -2 ){
            findTrans( &transArr, &request );
        }


    }

    printArr( &transArr );
    exit(EXIT_SUCCESS);
}