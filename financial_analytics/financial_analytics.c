#include <stdio.h>

#define SEQUENCE_MAX 250000

struct result{
    int start;
    int end;
};

// sanitizes input and returns number of elements in sequence array
int input( int sequence[] ){
    int i = 0, scanRes;

    printf("Hodnoty:\n");
    while(1){
        if( i == SEQUENCE_MAX )
            return -1;

        scanRes = scanf("%d", &sequence[i]);
        if( scanRes == 0 )
            return -1;
        if( scanRes == -1 )
            break;
        i++;
    }

    if( i < 2 )
        return -1;

    return i;
}

// finds all the longest intervals in the sequence
void findIntervals( const int sequence[], struct result results[], int sequenceLen, int* intervalLength, int* numOfResults ){
    for( int i = 0; i < sequenceLen - 1; i++ ){
        // end cycle if no longer interval can be found
        if( sequenceLen - i < *intervalLength )
            break;
        for( int j = sequenceLen - 1; j > i; j-- ){
            // end cycle if no longer interval can be found
            if( j - i + 1 < *intervalLength )
                break;
            if( sequence[i] <= sequence[j] ){
                // replace results if a longer sequence is found
                if( j - i + 1 > *intervalLength ){
                    *numOfResults = 1;
                    *intervalLength = j - i + 1;
                    results[0].start = i;
                    results[0].end = j;
                    continue;
                }
                // add new result if one is found with the same length
                if( j - i + 1 == *intervalLength ){
                    results[*numOfResults].start = i;
                    results[*numOfResults].end = j;
                    *numOfResults = *numOfResults + 1;
                }
            }
        }
    }
}

int main(){
    int sequence[SEQUENCE_MAX];
    struct result results[SEQUENCE_MAX / 2 + 1];
    int sequenceLen = input(sequence);


    if( sequenceLen == -1 ){
        printf("Nespravny vstup.\n");
        return 1;
    }

    int intervalLength = 0, numOfResults = 0;
    findIntervals( sequence, results, sequenceLen, &intervalLength, &numOfResults);

    // print special message when no interval was found
    if( numOfResults == 0 ){
        printf("Nelze najit.\n");
        return 0;
    }

    // print all results
    for( int i = 0; i < numOfResults; i++ ){
        printf("%d: %d - %d\n", intervalLength, results[i].start, results[i].end );
    }
    printf("Moznosti: %d\n", numOfResults);

    return 0;
}