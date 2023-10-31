#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef struct
{
    int m_TotalDays;
    int m_WorkDays;
} TResult;
#endif /* __PROGTEST__ */

bool isWorkDay ( int y, int m, int d )
{
    /* todo */
}

TResult countDays ( int y1, int m1, int d1,
                    int y2, int m2, int d2 )
{
    /* todo */
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