#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/**
 * @var daysInMonth Global constant holding the numbers of days in individual months.
 */
const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/** @struct review
 *  @brief  Structure representing an individual user review.
 *  @var    date    Stores the date of the review. The date is formatted as one number: YYYYMMDD.
 *  @var    score   Stores the score of the review
 *  @var    text    Stores the commentary of the review
 */
struct review{
    int date;
    int score;
    char text[4097];
};

/** @struct revArr
 *  @brief  Structure holding values for maintaining and taking care of the dynamically allocated array of @see review.
 *  @var    reviews pointer to an array of @see review
 *  @var    size    stores the number of elements in the array
 *  @var    max     stores the number of allocated places in the array
 */
struct revArr{
    struct review* reviews;
    int size;
    int max;
};


/** @struct answer
 *  @brief Structure is used in @see search and @see printAnswer for passing information about the latest search
 *         between the two functions.
 *  @var    start   holds the pointer to the beginning of the found interval
 *  @var    end     holds the pointer to the end of the found interval
 *  @var    sum     holds the closest sum that has been found
 *  @var    diff    holds the difference between the sum that is requested and sum that has been fuond
 */
struct answer{
    struct review* start;
    struct review* end;
    int sum;
    int diff;
};

//----------------------------------------------------------------------------------------------------------------------

/** @fn isLeap
 * Function decides if a given year y is leap or not
 * @param   y   year to be checked
 * @return return true if year is leap, false if not
 */
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

/** @fn checkDateValidity
 * Function checks if a given date is a valid existing date
 * @param   y   year
 * @param   m   month
 * @param   d   day
 * @return return true if date is valid, false if not
 */
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

/** @fn reallocate
 * Function reallocates the dynamically allocated array of reviews
 * @param revArr_i pointer to existing @see revArr that needs to be reallocated
 */
void reallocate( struct revArr* revArr_i ){
    revArr_i->max = (int) revArr_i->max * 1.5;
    revArr_i->reviews = (struct review*) realloc(revArr_i->reviews, sizeof(struct review) * revArr_i->max );
}

//----------------------------------------------------------------------------------------------------------------------

/** @fn input
 * Function takes care of input sanitization. It also adds new entries into the array of user reviews.
 *
 * @param   requestType pointer used as output. Stores the data about what type of request is being processed
 * @param   sum pointer used as output. If the request is a "search", it stores the number to be approximated.
 *              If the request is an "add", contents are undefined.
 * @param   revArr_i pointer to an instance of @see revArr
 *
 * @return  Function returns 0 for success, 1 for wrong input and -1 for end of input
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

/** @fn search
 * Functions searches the array for the best fitting interval
 * @param reqSum    sum required by the search request
 * @param revArr_i  pointer to @see revArr
 * @return @see answer
 */
struct answer search( int reqSum, struct revArr* revArr_i ){
    struct answer answer_i = {revArr_i->reviews, NULL, 0, 0};
    int iteratorStart = 0, iteratorEnd = 0, currSum = 0;
    bool reachEndFlag = false;


    while( true ){

        // iterate over the array from iteratorStart to iteratorEnd
        while( true ){
            currSum += revArr_i->reviews[iteratorEnd].score;
            if( iteratorEnd+1 == revArr_i->size )
                reachEndFlag = true;

            // add whole days
            if( revArr_i->reviews[iteratorEnd].date == revArr_i->reviews[iteratorEnd+1].date &&
                !reachEndFlag ){
                iteratorEnd ++;
                continue;
            }

            // fill in a new answer_i if the specified criteria is met
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

        // move iteratorStart one day forward
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

        // reset all other variables
        iteratorEnd = iteratorStart;
        currSum = 0;
        reachEndFlag = false;
    }

    return answer_i;
}

//----------------------------------------------------------------------------------------------------------------------

/** @fn printAnswer
 * Function used for printing out the output
 * @param requestType   type of request
 * @param answer_i      @see answer
 */
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
        if(requestType == '+')
            continue;


        answer_i = search( sum, &revArr_i );
        printAnswer( requestType, answer_i );
    }

    free(revArr_i.reviews);
    return 0;
}