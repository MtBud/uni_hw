#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// symbolizes one review
struct review{
    int date;
    int score;
    char text[4097];
};

// takes care of the whole array of reviews
struct revArr{
    struct review* reviews;
    int size;
    int max;
};

// is used as output of the searching algorithm
// start and end are the pointers to the start and end of the interval, sum is the total
struct answer{
    struct review* start;
    struct review* end;
    int sum;
    int diff;
};

//----------------------------------------------------------------------------------------------------------------------

bool isLeap( int y ){
    if( y % 4000 == 0 )
        return false;
    if( y % 400 == 0 )
        return true;
    if( y % 100 == 0 )
        return false;
    if( y % 4 == 0 )
        return true;
    return false;
}

// check if date is valid(true) or not(false)
bool checkDateValidity( int y, int m, int d ){
    if( m < 1 || m > 12 )
        return false;

    if( m == 2 && d == 29 ){
        if( !isLeap(y) )
            return false;
        return true;
    }

    if( d <= 0 || d > daysInMonth[m - 1] )
        return false;
    return true;
}

// reallocates the reviews array
void reallocate( struct revArr* revArr_i ){
    revArr_i->max = (int) revArr_i->max * 1.5;
    revArr_i->reviews = (struct review*) realloc(revArr_i->reviews, sizeof(struct review) * revArr_i->max );
}

void printEntry( char requestType, struct review review_i ){
    printf("%c %d-%d-%d %d %s\n", requestType,
           review_i.date / 10000,
           review_i.date % 10000 / 100,
           review_i.date % 100,
           review_i.score,
           review_i.text);
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Function takes care of input sanitization. It also fills in the information into the reviews array.
 * */
int input( char* requestType, int* sum , struct revArr* revArr_i ){
    // get the type of request, end program if end of input is detected
    if( scanf(" %c", requestType) == EOF )
        return -1;

    // get sum if the request is a query
    if( *requestType == '#' || *requestType == '?' ){
        if( revArr_i->size == 0 )
            return 1;
        if( scanf("%d", sum) != 1 )
            return 1;
        if(getc(stdin) != '\n')
            return 1;
        if( *sum <= 0 )
            return 1;
        return 0;
    }

    // get all other info if the request adds a new entry
    if( *requestType != '+' )
        return 1;
    struct review newEntry;

    int year, month, day;
    if( scanf( "%d-%d-%d", &year, &month, &day ) != 3 )
        return 1;
    if( scanf("%d", &newEntry.score) != 1 )
        return 1;
    if( scanf("%s", newEntry.text) != 1 )
        return 1;
    if(getc(stdin) != '\n')
        return 1;
    if( newEntry.score <= 0 )
        return 1;

    // check if the date is valid and more or equal than the previous one
    if( !checkDateValidity( year, month, day) )
        return 1;
    newEntry.date = year*10000 + month * 100 + day;
    if( revArr_i->size > 0 && revArr_i->reviews[revArr_i->size-1].date > newEntry.date )
        return 1;

    // if it passes all the tests, the new entry is added to the all encompassing array
    if( revArr_i->max == revArr_i->size )
        reallocate( revArr_i );
    revArr_i->reviews[revArr_i->size] = newEntry;
    revArr_i->size ++;

    //printEntry( *requestType, newEntry );
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

struct answer search_simple( int reqSum, struct revArr* revArr_i ){
    struct answer answer_i = {revArr_i->reviews, NULL, 0, 0};
    int iteratorStart = 0, iteratorEnd = 0, currSum = 0;
    bool reachEndFlag = false;


    while( true ){

        while( true ){
            currSum += revArr_i->reviews[iteratorEnd].score;
            if( iteratorEnd+1 == revArr_i->size )
                reachEndFlag = true;

            if( revArr_i->reviews[iteratorEnd].date == revArr_i->reviews[iteratorEnd+1].date &&
                !reachEndFlag ){
                iteratorEnd ++;
                continue;
            }

            if( answer_i.end == NULL ||
                abs(reqSum - currSum) < answer_i.diff ||
                ( abs(reqSum - currSum) == answer_i.diff && answer_i.end < &revArr_i->reviews[iteratorEnd] ) ||
                ( abs(reqSum - currSum) == answer_i.diff && answer_i.end == &revArr_i->reviews[iteratorEnd] && currSum > answer_i.sum )){

                answer_i.start = &revArr_i->reviews[iteratorStart];
                answer_i.end = &revArr_i->reviews[iteratorEnd];
                answer_i.sum = currSum;
                answer_i.diff = abs(reqSum - answer_i.sum);

            }

            if(reachEndFlag)
                break;

            iteratorEnd ++;
        }

        iteratorStart ++;
        if( iteratorStart == revArr_i->size )
            break;
        while( true ){
            if(iteratorStart+1 == revArr_i->size)
                break;
            if(revArr_i->reviews[iteratorStart].date != revArr_i->reviews[iteratorStart-1].date)
                break;
            iteratorStart ++;
        }
        if( iteratorStart+1 == revArr_i->size && revArr_i->reviews[iteratorStart].date == revArr_i->reviews[iteratorStart-1].date )
            break;

        iteratorEnd = iteratorStart;
        currSum = 0;
        reachEndFlag = false;
    }

    return answer_i;
}


/**
 * Add days until the difference between the desired sum and the current sum starts getting bigger.
 * Subtract days until the difference starts getting bigger.
 * Repeat until the best fit is found.
 * */
struct answer search_complex( int reqSum, struct revArr* revArr_i ){
    struct answer answer_i = {revArr_i->reviews, NULL, 0, 0};
    int iteratorEnd = 0, iteratorStart = 0, currSum = 0;
    bool reachEndFlag = false;

    while( true ){

        // adding new days
        while( true ){
            if( reachEndFlag )
                break;
            // adding all the entries of the same day
            while( true ){
                currSum += revArr_i->reviews[iteratorEnd].score;
                if( iteratorEnd+1 == revArr_i->size ){
                    reachEndFlag = true;
                    break;
                }

                if( revArr_i->reviews[iteratorEnd].date != revArr_i->reviews[iteratorEnd+1].date )
                    break;
                iteratorEnd ++;
            }

            // initialize the structure if it's the first loop
            // or update the structure if better entry is found
            if( answer_i.end == NULL || abs(reqSum - currSum) <= answer_i.diff ){
                answer_i.start = &revArr_i->reviews[iteratorStart];
                answer_i.end = &revArr_i->reviews[iteratorEnd];
                answer_i.sum = currSum;
                answer_i.diff = abs(reqSum - answer_i.sum);
                //printf("sumWriteAdd: %d\n", currSum);

                if( iteratorEnd+1 == revArr_i->size )
                    break;
                iteratorEnd ++;
                continue;
            }
            else
                break;

        }
        //printf("currSumAdd: %d\n", currSum);


        while( true ){
            if(iteratorStart == iteratorEnd)
                break;
            // subtracting all entries of the same day
            while( true ){
                if( iteratorStart+1 == revArr_i->size )
                    break;
                currSum -= revArr_i->reviews[iteratorStart].score;
                //printf("currSum: %d\n", currSum);
                //printf("iteratorStart: %d\n", iteratorStart);
                //printf("iteratorEnd: %d\n", iteratorEnd);

                iteratorStart ++;
                if( revArr_i->reviews[iteratorStart].date != revArr_i->reviews[iteratorStart-1].date )
                    break;

            }

            // update the structure if better entry is found
            if( abs(reqSum - currSum) < answer_i.diff ||
                (abs(reqSum - currSum) == answer_i.diff && answer_i.end < &revArr_i->reviews[iteratorEnd])){
                if( iteratorStart+1 == revArr_i->size &&
                    revArr_i->reviews[iteratorStart].date == revArr_i->reviews[iteratorStart-1].date )
                    break;
                answer_i.start = &revArr_i->reviews[iteratorStart];
                answer_i.end = &revArr_i->reviews[iteratorEnd];
                answer_i.sum = currSum;
                answer_i.diff = abs(reqSum - answer_i.sum);
                //printf("sumWriteSub: %d\n", currSum);
                if( iteratorStart+1 == revArr_i->size )
                    break;
                continue;
            }
            else
                break;
        }
        //printf("currSumSub: %d\n", currSum);


        if( iteratorStart+1 == revArr_i->size)
            break;

        if( iteratorEnd+1 != revArr_i->size )
            iteratorEnd++;
    }


    return answer_i;
}

//----------------------------------------------------------------------------------------------------------------------

void printAnswer( char requestType, struct answer answer_i ){
    printf("%d-%.2d-%.2d - %d-%.2d-%.2d: %d\n",
           answer_i.start->date / 10000,
           answer_i.start->date % 10000 / 100,
           answer_i.start->date % 100,
           answer_i.end->date / 10000,
           answer_i.end->date % 10000 / 100,
           answer_i.end->date % 100,
           answer_i.sum);
    if( requestType == '?'){
        while( answer_i.start != answer_i.end+1 ){
            printf("  %d: %s\n", answer_i.start->score, answer_i.start->text);
            answer_i.start ++;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

int main(){
    char requestType;
    int sum;
    struct revArr revArr_i = {(struct review*) malloc( revArr_i.max*sizeof(struct review) ), 0, 5};
    struct answer answer_i;

    printf("Recenze:\n");
    while( true ){
        int inputOut = input(&requestType, &sum, &revArr_i);
        if( inputOut == 1 ){
            printf("Nespravny vstup.\n");
            free(revArr_i.reviews);
            return 1;
        }
        if( inputOut == -1 )
            break;
        if(requestType == '+'){/*
            answer_i.start = &revArr_i.reviews[0];
            answer_i.end = &revArr_i.reviews[revArr_i.size-1];
            answer_i.sum = 0;
            answer_i.diff = 0;
            printAnswer( '?', answer_i );
            printf("size: %d\n", revArr_i.size);*/
            continue;
        }

        answer_i = search_simple( sum, &revArr_i );
        printAnswer( requestType, answer_i );
    }

    free(revArr_i.reviews);
    return 0;
}