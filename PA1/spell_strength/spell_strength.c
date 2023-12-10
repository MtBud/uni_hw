#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define STRSIZE 10
#define DICTSIZE 4
bool duplicates_flag = false;


/**
 * Structure takes care of the whole string input on stdin
 * */
struct strArr{
    char* str;
    char* strEnd;
    size_t max;
};

/**
 * Structure used to store separated whole words
 */
struct dictionary{
    char** word;
    size_t max;
    size_t size;
};


void printDictionary( struct dictionary* dict ){
    printf("\nDICTIONARY\n");
    for( uint i = 0; i < dict->size; i++ ){
        printf("%s\n", dict->word[i]);
    }
}

void strToLower( char* str ){
    int i = 0;
    while( str[i] != '\0' ){
        str[i] = (char) tolower(str[i]);
        i ++;
    }
}

/**
 * Function used for reallocating the array storing the one whole string
 * @return returns the size of newly allocated empty space (num of elements, not size in bytes)
 * @param inStr
 */
size_t reallocStr( struct strArr* inStr ){
    char* tmp;
    size_t sizediff = inStr->max;
    inStr->max *= 2;
    sizediff = inStr->max - sizediff;
    tmp = (char*) realloc(inStr->str, inStr->max * sizeof(char));
    if( tmp == NULL){
        printf("Reallocation failed\n");
        exit (EXIT_FAILURE);
    }
    inStr->str = tmp;
    return sizediff;
}

void reallocDict( struct dictionary* dict ){
    //printDictionary( dict );
    //printf("REALLOC DICT\n");
    char** tmp;
    dict->max *= 2;
    tmp = (char**) realloc(dict->word, dict->max * sizeof(char*));
    if( tmp == NULL){
        printf("Reallocation failed\n");
        exit (EXIT_FAILURE);
    }
    dict->word = tmp;
}

/**
 * Comparison function used in quicksort
 * @param a
 * @param b
 * @return
 */
int compareDict( const void* a, const void* b ){
    const char *arg1 = *(char**)a;
    char *arg1Lower = (char*) malloc(sizeof(char) * (strlen(arg1) + 1));
    const char *arg2 = *(char**)b;
    char *arg2Lower = (char*) malloc(sizeof(char) * (strlen(arg2) + 1));
    strcpy(arg1Lower, arg1);
    strcpy(arg2Lower, arg2);
    strToLower( arg1Lower );
    strToLower( arg2Lower );


    int comparison = strcmp(arg1Lower, arg2Lower);
    //printf("compare: %s, %s, outcome: %d\n", arg1, arg2, comparison );
    if( comparison == 0 ){
        duplicates_flag = true;
    }

    free(arg1Lower);
    free(arg2Lower);
    return comparison;
}

void freeDict( struct dictionary* dict ){
    for( uint i = 0; i < dict->size - 1 ; i ++ )
        free( dict->word[i] );
    free(dict->word);
}

/**
 * Function takes care of the large string on stdin
 * @param inStr
 * @return
 */
int input( struct strArr* inStr ){
    size_t maxIn = inStr->max;
    char* fgetsOut;

    // check if no words have been entered
    fgetsOut = fgets( inStr->str, (int) maxIn, stdin);
    if( fgetsOut == NULL )
        return 1;

    // check
    if( feof(stdin) ){
        inStr->strEnd = strrchr(inStr->str, '\0');
        return 0;
    }
    maxIn = reallocStr(inStr);
    inStr->strEnd = strrchr(inStr->str, '\0');

    while( true ){
        fgetsOut = fgets( inStr->strEnd, (int) maxIn, stdin);

        if( feof(stdin) ){
            inStr->strEnd = strrchr(inStr->str, '\0');
            break;
        }

        maxIn = reallocStr(inStr);
        inStr->strEnd = strrchr(inStr->str, '\0');

    }

    return 0;
}

/**
 * Separates the one big string into single words and stores them in an array
 * @return 0 on success, 1 on fail ( word is duplicate )
 */
int separate( struct strArr* inStr, struct dictionary* dict ){
    char* wordStart = inStr->str;
    char* wordEnd;
    size_t length;
    char tmp;
    uint dictIter = 0;

    while( true ){
        // find the start of a new word
        if( wordStart[0] == '\0')
            break;
        if(isspace(wordStart[0])){
            wordStart ++;
            continue;
        }

        // find the end of a new word
        wordEnd = wordStart + 1;
        while(true){
            if(isspace(wordEnd[0]) || wordEnd[0] == '\0')
                break;
            wordEnd ++;
        }

        // reallocate dictionary if needed
        if( dictIter == dict->max ){
            dict->size = dictIter;
            reallocDict( dict );
        }

        // add new word into the dictionary
        dict->size = dictIter;
        tmp = wordEnd[0];
        wordEnd[0] = '\0';
        length = strlen(wordStart);
        dict->word[dictIter] = (char*) malloc( (length+1) * sizeof(char) );
        strcpy(dict->word[dictIter], wordStart);
        //printf("OG: %s\n", wordStart);
        //printf("CP: %s\n", dict->word[dictIter]);
        //printDictionary(dict);
        wordEnd[0] = tmp;
        wordStart = wordEnd;
        dictIter ++;

    }

    // check for no words
    if( dictIter == 0 )
        return 1;

    dict->size = dictIter;

    return 0;
}

/**
 * Prints the final output
 * @param dict
 * @return
 */
int printFinal( struct dictionary* dict ){
    uint len = 0, currLen;
    for( int i = (int) dict->size - 1; i > 0; i -- ){
        currLen = strlen( dict->word[i] );

        if( len == 0 ){
            printf("%s", dict->word[i]);
            len = currLen;
            continue;
        }

        if( len + currLen >= 80 ){
            len = currLen;
            printf("\n%s", dict->word[i]);
            continue;
        }

        printf(" %s", dict->word[i]);
        len += currLen + 1;
    }
    printf("\n");
    return 0;
}

/*
 * TODO
 * - separate() function, that separates the string into words
 * - function to connect the words for highest lexicographical score - use strcmp()
 * */
int main(){
    struct strArr inStr;
    inStr.max = STRSIZE;
    inStr.str = (char*) malloc(inStr.max * sizeof(char) );
    inStr.strEnd = inStr.str;

    // load in the input and check for empty input
    if( input(&inStr) ){
        printf("Nespravny vstup.\n");
        free(inStr.str);
        return 1;
    }
    /*
    printf("STRING\n");
    printf("%s\n", inStr.str);
    */

    struct dictionary dict;
    dict.max = DICTSIZE;
    dict.size = 0;
    dict.word = (char**) malloc( dict.max * sizeof(char*) );

    // separates the input and fills words into the dictionary
    if( separate( &inStr, &dict ) ){
        printf("Nespravny vstup.\n");
        free(inStr.str);
        freeDict(&dict);
        return 1;
    }
    //printDictionary( &dict );

    // sort the dictionary lexicographically and check for duplicates
    qsort( dict.word, dict.size, sizeof(char*), compareDict );
    if( duplicates_flag ){
        printf("Nespravny vstup.\n");
        free(inStr.str);
        freeDict(&dict);
        return 1;
    }

    /*
    printf("\nSORTED\n");
    printDictionary( &dict );
    */

    // print the output
    printFinal( &dict );

    free(inStr.str);
    freeDict(&dict);
    return 0;
}