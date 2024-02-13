#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define STRSIZE 100
#define DICTSIZE 40
bool duplicates_flag = false;

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
 * Function used to reallocate the words array in dictionary structure
 * @param dict
 */
void reallocDict( struct dictionary* dict ){
    struct wordStr* tmp;
    dict->max = (int) ((double) dict->max * 1.5);
    tmp = (struct wordStr*) realloc(dict->word, dict->max * sizeof(struct wordStr));
    if( tmp == NULL){
        printf("Reallocation failed, dict\n");
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
 * Function fills in the dictionaries straight from the input
 * @param dict
 * @param dictLwr
 * @return
 */
int input_straight( struct dictionary* dict, struct dictionary* dictLwr ){
    char c;
    uint iter = 0, dictIter = 0;
    uint buffMax = STRSIZE;
    char* buff = (char*) malloc( buffMax * sizeof(char) );

    while( true ){
        c = (char) getchar();
        if( c == EOF && iter == 0 )
            break;

        if( isspace(c) && iter == 0 )
            continue;

        if( isspace(c) || c == EOF ){
            buff[iter] = '\0';

            if( dictIter == dict->max ){
                dict->size = dictIter;
                dictLwr->size = dictIter;
                reallocDict( dict );
                reallocDict( dictLwr );
            }

            uint length = strlen(buff);
            dict->word[dictIter].word = (char*) malloc( (length+1) * sizeof(char) );
            dictLwr->word[dictIter].word = (char*) malloc( (length+1) * sizeof(char) );
            strcpy(dict->word[dictIter].word, buff);
            strcpy(dictLwr->word[dictIter].word, buff);
            strToLower(dictLwr->word[dictIter].word);
            dict->word[dictIter].idx = (int) dictIter;
            dictLwr->word[dictIter].idx = (int) dictIter;


            iter = 0;
            dictIter ++;
            continue;
        }

        if( iter == buffMax ){
            buffMax = (int) (buffMax * 1.5);
            char* tmp;
            tmp = (char*) realloc( buff, buffMax * sizeof(char) );
            if( tmp == NULL){
                printf("Reallocation failed, buff\n");
                exit (EXIT_FAILURE);
            }
            buff = tmp;
        }

        buff[iter] = c;
        iter ++;
    }

    free(buff);

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


    // load in the input and check for empty input
    printf("Slova:\n");
    if( input_straight( &dict, &dictLwr) ){
        printf("Nespravny vstup.\n");
        freeDict(&dict);
        freeDict(&dictLwr);
        return 1;
    }

    //printDictionary( &dictLwr );

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