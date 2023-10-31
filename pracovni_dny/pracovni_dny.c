#ifndef __PROGTEST__
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

typedef struct
{
    int m_TotalDays;
    int m_WorkDays;
} TResult;

#endif /* __PROGTEST__ */

//----------------------------------------------------------------------------------------------------------------------

// function checks if a year is a leap year
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
    const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if( y < 2000 )
        return false;
    if( m < 1 || m > 12 )
        return false;

    if( m == 2 && d == 29 ){
        if( !isLeap(y) )
            return false;
        return true;
    }

    if( d < 0 || d > daysInMonth[m - 1] )
        return false;
    return true;
}

// function checks if the day given is a work day(true) or not(false)
bool isWorkDay ( int y, int m, int d )
{
    printf("Y M D: %d %d %d\n", y, m, d );
    if( !checkDateValidity( y, m, d ) ){
        printf("Date isn't valid\n");
        return false;
    }

    // check if the date is holiday
    const int holidayMonths[] = {1,5,5,7,7,9,10,11,12,12,12};
    const int holidayDays[] = {1,1,8,5,6,28,28,17,24,25,26};
    for( int i = 0; i < 12; i ++ ){
        if(holidayMonths[i] == m && holidayDays[i] ==d )
            return false;
    }

    // calculate the different codes needed for the formula
    int YY = y % 100;
    int yearCode = (YY + (YY / 4)) % 7;
    const int monthCode[] = {0,3,3,6,1,4,6,2,5,0,3,5};
    int centuryCode = 6;
    if( y > 2100 )
        centuryCode = 4;
    int leapYCode = 0;
    if( isLeap(y) && ( m == 1 || m == 2) )
        leapYCode = 1;

    // formula for the calculation
    int dayOfWeek = (yearCode + monthCode[m-1] + centuryCode + d - leapYCode) % 7;
    printf("Day of Week: %d\n", dayOfWeek );
    if( dayOfWeek == 0 || dayOfWeek == 6 )
        return false;

    return true;
}

//----------------------------------------------------------------------------------------------------------------------

// checks if interval 1 is smaller than 2
bool compareInterval( int y1, int m1, int d1,
                     int y2, int m2, int d2 ){
    if( y2 < y1 )
        return false;
    if( y2 == y1 && m2 < m1 )
        return false;
    if( y2 == y1 && m2 == m1 && d2 < d1 )
        return false;
    return true;
}

TResult countDays ( int y1, int m1, int d1,
                    int y2, int m2, int d2 )
{
    TResult result;
    result.m_TotalDays = -1;
    result.m_WorkDays = -1;
    if( !checkDateValidity(y1, m1, d1) || !checkDateValidity(y2, m2, d2) )
        return result;
    if( !compareInterval(y1, m1, d1, y2, m2, d2) )
        return result;

    for( int i = y1; i <= y2; i ++ ){
        for( int j = 0;)
    }
    result.m_TotalDays = 0;
    result.m_WorkDays = 0;


    return result;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
    //TResult r;

    assert ( isWorkDay ( 2023, 10, 10 ) );

    assert ( ! isWorkDay ( 2023, 11, 11 ) );

    assert ( ! isWorkDay ( 2023, 11, 17 ) );

    assert ( ! isWorkDay ( 2023, 11, 31 ) );

    assert ( ! isWorkDay ( 2023,  2, 29 ) );

    assert ( ! isWorkDay ( 2004,  2, 29 ) );

    assert ( isWorkDay ( 2008,  2, 29 ) );

    assert ( ! isWorkDay ( 2001,  2, 29 ) );

    assert ( ! isWorkDay ( 1996,  1,  2 ) );

    /*
    r = countDays ( 2023, 11,  1,
                    2023, 11, 30 );
    assert ( r . m_TotalDays == 30 );
    assert ( r . m_WorkDays == 21 );

    r = countDays ( 2023, 11,  1,
                    2023, 11, 17 );
    assert ( r . m_TotalDays == 17 );
    assert ( r . m_WorkDays == 12 );

    r = countDays ( 2023, 11,  1,
                    2023, 11,  1 );
    assert ( r . m_TotalDays == 1 );
    assert ( r . m_WorkDays == 1 );

    r = countDays ( 2023, 11, 17,
                    2023, 11, 17 );
    assert ( r . m_TotalDays == 1 );
    assert ( r . m_WorkDays == 0 );

    r = countDays ( 2023,  1,  1,
                    2023, 12, 31 );
    assert ( r . m_TotalDays == 365 );
    assert ( r . m_WorkDays == 252 );

    r = countDays ( 2024,  1,  1,
                    2024, 12, 31 );
    assert ( r . m_TotalDays == 366 );
    assert ( r . m_WorkDays == 254 );

    r = countDays ( 2000,  1,  1,
                    2023, 12, 31 );
    assert ( r . m_TotalDays == 8766 );
    assert ( r . m_WorkDays == 6072 );

    r = countDays ( 2001,  2,  3,
                    2023,  7, 18 );
    assert ( r . m_TotalDays == 8201 );
    assert ( r . m_WorkDays == 5682 );

    r = countDays ( 2021,  3, 31,
                    2023, 11, 12 );
    assert ( r . m_TotalDays == 957 );
    assert ( r . m_WorkDays == 666 );

    r = countDays ( 2001,  1,  1,
                    2000,  1,  1 );
    assert ( r . m_TotalDays == -1 );
    assert ( r . m_WorkDays == -1 );

    r = countDays ( 2001,  1,  1,
                    2023,  2, 29 );
    assert ( r . m_TotalDays == -1 );
    assert ( r . m_WorkDays == -1 );
     */

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */