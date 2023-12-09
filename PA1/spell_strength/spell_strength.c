#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define STRSIZE 10

/**
 * Structure takes care of the whole string input on stdin
 * */
struct strArr{
    size_t max;
    char* str;
    char* strEnd;
};

/**
 * Structure used to store separated whole words
 */
struct dictionary{
    size_t max;
    size_t size;
    char** word;
};

/**
 * Function used for reallocating the array storing the one whole string
 * @return returns the size of newly allocated empty space (num of elements, not size in bytes)
 * @param inStr
 */
size_t reallocate( struct strArr* inStr ){
    size_t sizediff = inStr->max;
    inStr->max *= 2;
    sizediff = inStr->max - sizediff;
    inStr->str = (char*) realloc(inStr->str, inStr->max);
    return sizediff;
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
    maxIn = reallocate(inStr);
    inStr->strEnd = strrchr(inStr->str, '\0');

    while( true ){
        fgetsOut = fgets( inStr->strEnd, (int) maxIn, stdin);

        if( feof(stdin) ){
            inStr->strEnd = strrchr(inStr->str, '\0');
            break;
        }

        maxIn = reallocate(inStr);
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

        // add new word into the dictionary
        tmp = wordEnd[0];
        wordEnd[0] = '\0';
        length = strlen(wordStart);
        dict->word[dictIter] = (char*) malloc( (length+1) * sizeof(char) );
        strcpy(dict->word[dictIter], wordStart);
        wordEnd[0] = tmp;
        dictIter ++;
        // reallocate dictionary if needed
        if( dictIter == dict->max ){
            dict->max = dict->max * 2;
            dict->word = (char**) realloc(dict->word, dict->max);
        }

    }

    // check for no words
    if( dictIter == 0 )
        return 1;

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
        return 1;
    }

    struct dictionary dict;
    dict.max = 10;
    dict.size = 0;
    dict.word = (char**) malloc( dict.max * sizeof(char*) );

    // separates the input and checks for duplicates
    if( separate( &inStr, &dict ) ){
        printf("Nespravny vstup.\n");
        return 1;
    }
    free(inStr.str);

    printf("\n%s\n", inStr.str);

    return 0;
}