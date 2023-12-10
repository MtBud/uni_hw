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
 * Structure representing one word in a dictionary
 */
struct wordStr{
    char* word;
    int idx;
};

/**
 * Structure used to store separated whole words
 */
struct dictionary{
    size_t max;
    size_t size;
    struct wordStr* word;
};

/**
 * Function prints the words in a dictionary
 * @param dict
 */
void printDictionary( struct dictionary* dict ){
    printf("\nDICTIONARY\n");
    for( uint i = 0; i < dict->size; i++ ){
        printf("%s\n", dict->word[i].word);
    }
}

/**
 * Function transforms a string to lowercase
 * @param str
 */
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

/**
 * Function used to reallocate the words array in dictionary structure
 * @param dict
 */
void reallocDict( struct dictionary* dict ){
    struct wordStr* tmp;
    dict->max *= 2;
    tmp = (struct wordStr*) realloc(dict->word, dict->max * sizeof(struct wordStr));
    if( tmp == NULL){
        printf("Reallocation failed\n");
        exit (EXIT_FAILURE);
    }
    dict->word = tmp;
}

/**
 * Function used for freeing the dictionary structure
 * @param dict
 */
void freeDict( struct dictionary* dict ){
    for( uint i = 0; i < dict->size ; i ++ )
        free( dict->word[i].word );
    free(dict->word);
}

/**
 * Comparison function used in quicksort
 * @param a
 * @param b
 * @return
 */
int compareDict( const void* a, const void* b ){
    struct wordStr *arg1 = (struct wordStr*) a;
    struct wordStr *arg2 = (struct wordStr*) b;
    char *arg1First = (char*) malloc(sizeof(char) * (strlen(arg1->word) + strlen(arg2->word) + 1));
    char *arg2First = (char*) malloc(sizeof(char) * (strlen(arg1->word) + strlen(arg2->word) + 1));
    strcpy(arg1First, arg1->word);
    strcpy(arg2First, arg2->word);

    // check for duplicates
    if( strcmp(arg1First, arg2First) == 0 ){
        duplicates_flag = true;
    }

    strcat(arg1First, arg2->word);
    strcat(arg2First, arg1->word);

    int comparison = strcmp(arg1First, arg2First);
    if( comparison == 0 ){
        if( strlen(arg1->word) < strlen(arg2->word)){
            comparison = -1;
        }
        else
            comparison = 1;
    }

    free(arg1First);
    free(arg2First);
    return comparison;
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
int separate( struct strArr* inStr, struct dictionary* dict, struct dictionary* dictLwr ){
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

        // reallocate dictionaries if needed
        if( dictIter == dict->max ){
            dict->size = dictIter;
            dictLwr->size = dictIter;
            reallocDict( dict );
            reallocDict( dictLwr );
        }

        // add new word into the dictionaries
        dict->size = dictIter;
        tmp = wordEnd[0];
        wordEnd[0] = '\0';
        length = strlen(wordStart);
        dict->word[dictIter].word = (char*) malloc( (length+1) * sizeof(char) );
        dictLwr->word[dictIter].word = (char*) malloc( (length+1) * sizeof(char) );
        strcpy(dict->word[dictIter].word, wordStart);
        strcpy(dictLwr->word[dictIter].word, wordStart);
        strToLower(dictLwr->word[dictIter].word);
        dict->word[dictIter].idx = (int) dictIter;
        dictLwr->word[dictIter].idx = (int) dictIter;


        wordEnd[0] = tmp;
        wordStart = wordEnd;
        dictIter ++;
    }

    // check for no words
    if( dictIter == 0 )
        return 1;

    dict->size = dictIter;
    dictLwr->size = dictIter;

    return 0;
}

/**
 * Prints the final output
 * @param dict
 * @return
 */
int printFinal( struct dictionary* dict, struct dictionary* dictLwr ){
    uint len = 0, currLen;

    for( int i = (int) dictLwr->size - 1; i >= 0; i -- ){
        currLen = strlen( dictLwr->word[i].word );

        if( len == 0 ){
            printf("%s", dict->word[dictLwr->word[i].idx].word);
            len = currLen;
            continue;
        }

        if( len + currLen >= 80 ){
            len = currLen;
            printf("\n%s", dict->word[dictLwr->word[i].idx].word);
            continue;
        }

        printf(" %s", dict->word[dictLwr->word[i].idx].word);
        len += currLen + 1;
    }
    printf("\n");
    return 0;
}

/**
 * Main function
 * @return
 */
int main(){
    struct strArr inStr;
    inStr.max = STRSIZE;
    inStr.str = (char*) malloc(inStr.max * sizeof(char) );
    inStr.strEnd = inStr.str;

    // load in the input and check for empty input
    printf("Slova:\n");
    if( input(&inStr) ){
        printf("Nespravny vstup.\n");
        free(inStr.str);
        return 1;
    }

    // make two dictionaries
    // dict contains all the input words in original format
    // dictLwr contains all words in lowercase
    struct dictionary dict;
    dict.max = DICTSIZE;
    dict.size = 0;
    dict.word = (struct wordStr*) malloc( dict.max * sizeof(struct wordStr) );
    struct dictionary dictLwr;
    dictLwr.max = DICTSIZE;
    dictLwr.size = 0;
    dictLwr.word = (struct wordStr*) malloc( dict.max * sizeof(struct wordStr) );

    // separates the input and fills words into the dictionary
    if( separate( &inStr, &dict, &dictLwr) ){
        printf("Nespravny vstup.\n");
        free(inStr.str);
        freeDict(&dict);
        freeDict(&dictLwr);
        return 1;
    }

    // free the long string because it's no longer needed
    free(inStr.str);

    // sort the dictionary lexicographically and check for duplicates
    qsort( dictLwr.word, dict.size, sizeof(struct wordStr), compareDict );
    if( duplicates_flag ){
        printf("Nespravny vstup.\n");
        freeDict(&dict);
        freeDict(&dictLwr);
        return 1;
    }

    // print the output
    printFinal( &dict, &dictLwr );

    freeDict(&dictLwr);
    freeDict(&dict);
    return 0;
}