#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define MAX_TRANS 50

/**
 * Represents one transmission pair
 */
struct s_transmission{
    long long num1;
    long long num2;
};

/**
 * Swaps the two values of the structure
 * @param tr
 */
void swapTransmission( struct s_transmission* tr ){
    long long x = tr->num1;
    tr->num1 = tr->num2;
    tr->num2 = x;
}

/**
 * Represents an array of s_transmission structures
 */
struct s_transArr{
    struct s_transmission array[MAX_TRANS];
    int size;
    struct s_transmission sum;
};

/**
 * Copies arr1 into arr2
 * @param arr1
 * @param arr2
 */
void copyArray( struct s_transArr* arr1, struct s_transArr* arr2 ){
    for( int k = 0; k < arr1->size; k ++ )
        arr2->array[k] = arr1->array[k];
    arr2->sum = arr1->sum;
    arr2->size = arr1->size;
}

/**
 * Prints the given array
 */
void printArr( struct s_transArr* transArr ){
    printf("PRINT ARRAY\n");
    for( int i = 0; i < transArr->size; i++ ){
        printf("%lld:%lld\n", transArr->array[i].num1, transArr->array[i].num2 );
    }
}

/**
 * Prints the array in format of a result
 */
void printResult( struct s_transArr* result ){
    printf("%lld:%lld", result->sum.num1, result->sum.num2 );
    if( result->sum.num1 == 1 && result->sum.num2 == 1 ){
        printf("\n");
        return;
    }
    printf(" = ");
    for( int i = 0; i < result->size; i ++ ){
        if( i != 0 )
            printf(" * ");
        printf("[%lld:%lld]", result->array[i].num1, result->array[i].num2 );
    }
    printf("\n");
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
        //printf("END EOF\n");
        return -1;
    }

    if( queryID != '+' && queryID != '?' ){
        //printf("ERROR: ID not recognized\n");
        return 1;
    }

    if( queryID == '+' && *type ){
        //printf("ERROR: Adding new data after first request\n");
        return 1;
    }
    if( queryID == '?' ){
        //printf("Changing ID\n");
        *type = true;
    }


    long long num1, num2;
    if( scanf( "%lld:%lld\n", &num1, &num2 ) != 2 ){
        //printf("ERROR: Scanf failed\n");
        return 1;
    }

    // add new entry to array
    if( queryID == '+' ){
        if( num1 < 10 || num1 > 1000 || num2 < 10 || num2 > 1000 ){
            //printf("ERROR: Numbers out of range\n");
            return 1;
        }
        transArr->array[transArr->size].num1 = num1;
        transArr->array[transArr->size].num2 = num2;
        transArr->size ++;
        return 0;
    }

    if( num1 <= 0 || num2 <= 0 ){
        //printf("ERROR: Numbers out of range\n");
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
long long gcd(long long a, long long b){
    if (a == 0)
        return b;
    return gcd(b % a, a);
}

/**
 * Simplifies a fraction
 * @param fr1
 */
void simplifyFraction( struct s_transmission* fr1 ){
    long long gcdFr = gcd( fr1->num1, fr1->num2 );
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
 * Function compares which of the two transmissions is more optimal
 * @param tr1
 * @param tr2
 * @return 0 if the transmission is optimal, > 0 if the tested transmission is worse,
 *          < 0 if the tested transmission is better
 */
int comparison( long double tested, long double smallest, long double wanted ){
    long double diffTest, diffSmall;
    if( tested == wanted )
        return 0;

    diffTest = tested / wanted;
    if( diffTest < 1 )
        diffTest = wanted / tested;

    diffSmall = smallest / wanted;
    if( diffSmall < 1 )
        diffSmall = wanted / smallest;

    if( diffTest < diffSmall )
        return -1;
    return 1;
}

/**
 * Finds the closest combination of transmissions from the given array
 * @return 0 if an exact match is found, 1 if the function exited by iterating over the loop
 */
int findTrans( struct s_transArr* database, struct s_transArr* result,
               struct s_transArr* progress, struct s_transmission* request,
               int offset, int level ){
    struct s_transmission ogSum = progress->sum;
    progress->size = level;


    for( int i = 0 + offset; i < database->size; i ++ ){
        // do it twice for both variants of the transmission
        for( int j = 0; j < 2; j ++ ){
            progress->array[level] = database->array[i];
            if( j == 1 )
                swapTransmission( &progress->array[level] );
            progress->sum.num1 = ogSum.num1 * progress->array[level].num1;
            progress->sum.num2 = ogSum.num2 * progress->array[level].num2;

            // compare current iteration with best so far
            int cmp = comparison( (long double) progress->sum.num1 / progress->sum.num2,
                                  (long double) result->sum.num1 / result->sum.num2,
                                  (long double) request->num1 / request->num2 );

            // overwrite result if better match has been found
            if( cmp <= 0 ){
                copyArray( progress, result );
                simplifyFraction( &result->sum );

                // end recursion if best possible match has been found
                if( cmp == 0 )
                    return 0;
            }

            // start recursion + end recursion if match is found
            if( findTrans( database, result, progress, request, i + 1, level + 1 ) == 0 )
                return 0;
        }

    }
    return 1;
}

int main(){
    bool type = false;
    struct s_transmission request;
    struct s_transArr database;
    struct s_transArr progress;
    struct s_transArr result;
    database.size = 0;

    printf("Prevody:\n");
    while( true ){
        int inputOut = input( &type, &database, &request );
        if( inputOut == -1 )
            break;
        if( inputOut == 1 ){
            printf("Nespravny vstup.\n");
            exit(EXIT_FAILURE);
        }

        if( inputOut == -2 ){
            struct s_transmission tmp = {1,1};
            result.size = 1;
            result.array[0] = tmp;
            result.sum = tmp;
            progress.sum = tmp;
            findTrans( &database, &result, &progress, &request, 0, 0 );
            printResult( &result );
        }


    }

    //printArr( &database );
    exit(EXIT_SUCCESS);
}