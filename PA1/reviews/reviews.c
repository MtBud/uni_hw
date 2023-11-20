#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

struct review{
    int year;
    int score;
    int month;
    int day;
    char text[4097];
};

struct revArr{
    int size;
    int max;
    struct review* reviews;
};

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

void reallocate( struct revArr* revArr_i ){
    revArr_i->max = revArr_i->max * 1.5;
    revArr_i->reviews = realloc(&revArr_i->reviews, sizeof(struct review) * revArr_i->max );
}

int input( char* requestType, int* sum , struct revArr* revArr_i ){
    // get the type of request
    if( scanf(" %c", requestType) != 1 )
        return 1;

    // get sum if the request is a query
    if( *requestType == '#' || *requestType == '?' ){
        if( scanf("%d", sum) != 1 )
            return 1;
        if( sum <= 0 )
            return 1;
        return 0;
    }

    // get all other info if the request adds a new entry
    if( *requestType != '+' )
        return 1;
    struct review newEntry;


    if( scanf( "%d-%d-%d", &newEntry.year, &newEntry.month, &newEntry.day ) != 3 )
        return 1;
    if( scanf("%d", &newEntry.score) != 1 )
        return 1;
    if( scanf("%s", newEntry.text) != 1 )
        return 1;

    // check if the date is valid and more or equal than the previous one
    if( !checkDateValidity( newEntry.year, newEntry.month, newEntry.day) )
        return 1;
    if( revArr_i->reviews[revArr_i->size].year < newEntry.year )
        return 1;
    if( revArr_i->reviews[revArr_i->size].year == newEntry.year &&
        revArr_i->reviews[revArr_i->size].month < newEntry.month)
        return 1;
    if( revArr_i->reviews[revArr_i->size].year == newEntry.year &&
        revArr_i->reviews[revArr_i->size].month == newEntry.month &&
        revArr_i->reviews[revArr_i->size].day < newEntry.day )
        return 1;

    // if it passes all the tests, the new entry is added to the all encompasing array
    if( revArr_i->max == revArr_i->size )
        reallocate( revArr_i );
    revArr_i->size ++;
    revArr_i->reviews[revArr_i->size] = newEntry;

    return 0;
}

int main(){
    char requestType;
    int sum;
    struct revArr revArr_i = {0, 5, malloc( revArr_i.max*sizeof(struct review) )};

    if( input(&requestType, &sum, &revArr_i) == 1 ){
        printf("Nespravny vstup.\n");
        return 1;
    }
    return 0;
}