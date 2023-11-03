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

const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int holidayMonths[] = {1,5,5,7,7,9,10,11,12,12,12};
const int holidayDays[] = {1,1,8,5,6,28,28,17,24,25,26};


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

int calcDayOfWeek( int y, int m, int d ){
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
    //printf("Day of Week: %d\n", dayOfWeek );

    return dayOfWeek;
}

// function checks if the day given is a work day(true) or not(false)
bool isWorkDay ( int y, int m, int d )
{
    //printf("Y M D: %d %d %d\n", y, m, d );
    if( !checkDateValidity( y, m, d ) ){
        printf("Date isn't valid: %d %d %d\n", y, m, d);
        return false;
    }

    // check if the date is holiday
    for( int i = 0; i < 11; i ++ ){
        if(holidayMonths[i] == m && holidayDays[i] ==d )
            return false;
    }

    int dayOfWeek = calcDayOfWeek( y, m, d );
    // 0 is sunday, 6 is saturday
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

// counts number of leap years before given date
int countLeapYears ( int y1, int m1 ){
    if ( m1 <= 2)
        y1--;
    return (y1 / 4) - (y1 / 100) + (y1 / 400);
}


// counts total number of days between two dates
// counts number of days since 0 0 0 for both dates and then subtracts the outcomes
int totalDays ( int y1, int m1, int d1,
                int y2, int m2, int d2 ){
    // sum of first date
    long long int sum1 = y1 * 365 + d1;
    sum1 += countLeapYears( y1, m1 );
    for( int i = 0; i < m1 - 1; i++ )
        sum1 += daysInMonth[i];

    // sum of second date
    long long int sum2 = y2 * 365 + d2;
    sum2 += countLeapYears( y2, m2 );
    for( int i = 0; i < m2 - 1; i++ )
        sum2 += daysInMonth[i];

    return sum2 - sum1 + 1;
}

int countWD_interval( int y1, int m1, int d1, int d2 ){
    int numOfDays = 0;
    for( int i = d1; i <= d2; i++ ){
        if(isWorkDay(y1,m1,i))
            numOfDays ++;
    }
    return numOfDays;
}

// counts number of work days for given month
int countWorkDaysMonth( int y1, int m1 ){
    int firstDay = calcDayOfWeek( y1, m1, 1 );
    int numFreeDays, numWorkDays = daysInMonth[m1-1];
    if( isLeap(y1) && numWorkDays == 28 )
        numWorkDays ++;
    switch (firstDay){
        // if month starts with sunday
        case 0:
            if( daysInMonth[m1 - 1] == 28 ){
                if( isLeap(y1) )
                    numFreeDays = 9;
                else
                    numFreeDays = 8;
                break;
            }
            numFreeDays = 9;
            break;

        // if month starts with saturday
        case 6:
            if( daysInMonth[m1 - 1] == 28 ){
                if( isLeap(y1) )
                    numFreeDays = 9;
                else
                    numFreeDays = 8;
                break;
            }
            numFreeDays = 10;
            break;

        // if month starts with friday
        case 5:
            switch( daysInMonth[m1 - 1] ){
                case 31:
                    numFreeDays = 10;
                    break;
                case 30:
                    numFreeDays = 9;
                    break;
                case 28:
                    numFreeDays = 8;
                    break;
            }
            break;

        // if month starts with thursday
        case 4:
            if( daysInMonth[m1 - 1] == 31 ){
                numFreeDays = 9;
                break;
            }
            numFreeDays = 8;
            break;

        // for all other days of week
        default:
            numFreeDays = 8;
            break;
    }

    // add more days if holiday isn't on weekend
    for( int i = 0; i < 11; i++ ){
        if( holidayMonths[i] == m1 ){
            int x = calcDayOfWeek( y1, holidayMonths[i], holidayDays[i] );
            if( x != 0 && x != 6 )
                numFreeDays ++;
        }
    }

    return numWorkDays - numFreeDays;
}

int countWorkDays( int y1, int m1, int d1,
                   int y2, int m2, int d2 ){
    if( y1 == y2 && m1 == m2 ){
        return countWD_interval(y1, m1, d1, d2);
    }

    int numOfDays = 0;
    for(int i = y1; i <= y2; i++){
        if(i == y1){
            if(isLeap(y1) && m1 == 2)
                numOfDays += countWD_interval( y1, m1, d1, daysInMonth[m1 - 1] + 1);
            else
                numOfDays += countWD_interval( y1, m1, d1, daysInMonth[m1 - 1]);
            for( int j = m1 + 1; j <= 12; j++ ){
                if( i == y2 && j == m2 ){
                    numOfDays += countWD_interval( y2, m2, 1, d2);
                    break;
                }
                numOfDays += countWorkDaysMonth( i, j );
            }
        }
        for( int j = 0; j <= 12; j++ ){
            if( i == y2 && j == m2 ){
                numOfDays += countWD_interval( y2, m2, 1, d2);
                break;
            }
            numOfDays += countWorkDaysMonth( i, j );
        }
    }

    return numOfDays;
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


    result.m_TotalDays = totalDays( y1, m1, d1, y2, m2, d2 );
    printf("Total Days: %d\n", result.m_TotalDays );
    result.m_WorkDays = countWorkDays( y1, m1, d1, y2, m2, d2 );
    printf("Work Days: %d\n\n", result.m_WorkDays );


    return result;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
    TResult r;

    assert ( isWorkDay ( 2023, 10, 10 ) );

    assert ( ! isWorkDay ( 2023, 11, 11 ) );

    assert ( ! isWorkDay ( 2023, 11, 17 ) );

    assert ( ! isWorkDay ( 2023, 11, 31 ) );

    assert ( ! isWorkDay ( 2023,  2, 29 ) );

    assert ( ! isWorkDay ( 2004,  2, 29 ) );

    assert ( isWorkDay ( 2008,  2, 29 ) );

    assert ( ! isWorkDay ( 2001,  2, 29 ) );

    assert ( ! isWorkDay ( 1996,  1,  2 ) );


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


    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */