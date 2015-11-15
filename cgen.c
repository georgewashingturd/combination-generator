// combination generator with duplicate(s) elimination
// full word dictionary checking added
// partial word combination and partial word dictionary checking added
// partial word phrase dictionary checking added, inspired by 
// the movie 'National Treasure' password -> "Valley Forge"
// Now you can launch dictionary attack :) but it's never meant that way
// copyright (c) 2004-2005 GreyGrolics, all rights reserved

#include<stdio.h>
#include<string.h>

#include "cgendict.h"

#define FREE 0
#define TAKEN 1

// limited to 8, because 9 elements will generate 362880 combinations......
// change this value at your own risk
#define MAX_ROOM_NON_LEXICAL 8

#if (DIC_MAX > MAX_ROOM_NON_LEXICAL)
#define MAX_ROOM DIC_MAX
#else
#define MAX_ROOM MAX_ROOM_NON_LEXICAL
#endif

#define THRESHOLD 5000000

#define WORD_SEARCH		3276
#define PHRASE_SEARCH	1947

// declare a BOOL type
typedef unsigned int BOOL;

#define true 1
#define false 0

int room[MAX_ROOM];
int skiplist[MAX_ROOM];
unsigned char result[MAX_ROOM];
unsigned char data[MAX_ROOM + 1];

int max;
int x = 0;

int screen = 1;
int saved = 1;
int lexical = 0;
char suppress = 0;

FILE *pFile;
FILE *fdict = NULL;

unsigned int total = 0;

void Usage (void)
{
    printf("Usage : cgen [s / f / a / sl / fl / al / slp / flp / alp] {FILENAME}\n");
    printf("        [s] - Print Result to Screen\n");
    printf("        [f] - Save Result to File in {FILENAME}\n");
    printf("        [a] - Print Result to Screen and Save to File in {FILENAME}\n");
    printf("       [.l] - Turn on Lexical Word Analyzer\n");
	printf("      [.lp] - Turn on Lexical Phrase Analyzer\n");
}

void InitRooms (void)
{
    int i;
    
    for (i = 0;i<max;i++)
    {
        room[i] = FREE;
        skiplist[i] = FREE;
    }
}

// It can print the result to screen or to a file or to both screen and file
void PrintResult (int charmax)
{
    int i;

    for (i = 0;i<charmax;i++)
    {
        if (screen == 1)
            printf("%c",result[i]);
        if (saved == 1)
            fprintf(pFile,"%c",result[i]);
    }

    x += (charmax + 1);

    if ((80 - x) < charmax)
    {
        if (x > 80)
        {
            if (saved == 1)
                fprintf(pFile,"\n");
        }
        else
        {
            if (screen == 1)
                printf("\n");
            if (saved == 1)
                fprintf(pFile,"\n");
        }

        x = 0;
    }
    else
    {
        if (screen == 1)
            printf(" ");
        if (saved == 1)
            fprintf(pFile," ");
    }

    total++;

}

// With the addition of the Phrase search capability steps 2 to 5 are put
// inside 1 function because phrase search requires recursive calls
// 2. step through each alphabet as the start of the word
// 3. fseek to the section of the dictionary where the alphabet resides
//    and the word length fits
// 4. check each word in that section, character by character, once a character
//    in the word is not inside the alpha list, break from the loop
// 5. only alphabets are allowed in the dictionary checking
void LexicalWork(char *prefix, int lexmax, int pmax, int *alphalist, int mode)
{
    int i, j, k;
    int wordcounter;
    int pointer;
    int alpha[26];
    int list[26];
    char words[DIC_MAX + 1];
	char affix[(DIC_MAX * 2) + 2 + 1];

	if (lexmax > max)
		return;
	
	j = 0;
	for (i = 0 ; i < 26 ; i++)
	{
		j += alphalist[i];
	}

	//if the remaining number of alphabets in the alphalist is less than lexmax
	//(lexmax is the min length of subword in the phrase) just display the prefix
	//(prefix is the phrases found till now), in the case j is NOT 0 if the suppress
	//is ON then these incomplete phrases won't be displayed
	if (j < lexmax)
	{
		if (mode == PHRASE_SEARCH)
		{
			if (screen == 1)
				if (j == 0)
					printf("%s\n",prefix);
				else if (suppress != 'y')
					printf("#%s\n",prefix);
			if (saved == 1)
				if (j == 0)
					fprintf(pFile,"%s\n",prefix);
				else if (suppress != 'y')
					fprintf(pFile,"#%s\n",prefix);
			total++;
		}
		return;
	}

	memcpy(alpha,alphalist,sizeof(alpha));
	memcpy(list,alpha,sizeof(alpha));

    for(k = lexmax; k <= pmax; k++)
    {
        for (i = 0; i < 26; i++)
        {
            if (alpha[i] != 0)
            {
                if (hashcount[i][k - DIC_MIN] == 0)
                    continue;

                fseek(fdict,hashoff[i][k - DIC_MIN],SEEK_SET);

				wordcounter = 0;
                while (!feof(fdict) && wordcounter < hashcount[i][k - DIC_MIN])
                {
                    wordcounter++;

                    fscanf(fdict,"%s",words);
                    memcpy(list,alpha,sizeof(alpha));
                    
                    for (j = 0; j < k; j++)
                    {
                        if (words[j] == '\0')
                            break;
                        pointer = words[j] - 65;
                        if (pointer >= 32)
                            pointer -= 32;
                        if (pointer < 0 || pointer > 25)
                            break;
                        if (list[pointer] == 0)
                            break;
                        list[pointer]--;
                    }
                    if (j == k && words[k]=='\0')
                    {
                        memcpy(result,words,k * sizeof(char));
						if (mode == WORD_SEARCH)
							PrintResult(k);
						else
						if (mode == PHRASE_SEARCH)
						{
							strcpy(affix, prefix);
							strcat(affix, words);
							strcat(affix, " ");
							LexicalWork(affix, lexmax, pmax, list, mode);
							//do NOT forget to call fseek because prev recursive calls messes
							//up the fdict pointer
							fseek(fdict,hashoff[i][k - DIC_MIN] + ((k + CR_SIZE) * wordcounter),SEEK_SET);
						}
                    }
                }
            }
        }
    }
}

// this is the function to check the dictionary
// when checking the dictionary, there is no need to turn on the combination
// generator, straight away check against the dictionary
// the steps are
// 1. list what alphabets are inside the word, store this in the alpha list
//    alpha list is an array of 26 integers, each holds the number of times
//    the character appears in the word, for example: "aabc" then the alpha
//    list will look like this : alpha [0] [1] [2] [3] ...
//                                      2   1   1   0  0 ...
// The rest of the steps are moved to LexicalWork()
void CheckLexical(int lexmax, int pmax, int mode)
{
    int i, j, k;
    int wordcounter;
    int pointer;
    int alpha[26];
    int list[26];
    char words[(DIC_MAX * 2) + 1];

    memset(alpha,0,sizeof(alpha));

    for (i = 0; i < max; i++)
    {
        if (data[i] >= 65 && data[i] <= 90)
            alpha[data[i] - 65]++;
        else if (data[i] >= 97 && data[i] <= 122)
            alpha[data[i] - 97]++;
        else
        {
            printf("Non Alpha Character Found, Aborting Lexical Analyzer.\n");
            return;
        }

    }

	words[0] = '\0';
	LexicalWork(words, lexmax, pmax, alpha, mode);
}

// this function is very short but very confusing as well
// so here is the story behind ..........
// to eliminate duplicate(s) we must do certain things
// 1. determine whether there is any duplicate(s) by using CheckSkip function
// 2. mark the duplicate(s) using different index, first duplicate is 1, second is 2 ...
//    e.g. 121223 -> skiplist [0][0][1][1][2][0]
//                   data     [1][2][1][2][2][3]
// 3. when processing rooms in the mainloop of GoToNextNumber check whether we need to
//    process the data or not
//    using the above example:
//    we need to process the second '1' if we're at level (n) == 3, but not at level 1
//    we need to process the second '2' if we're at level (n) == 4, but not at level 2
// 4. the way to do that is to count if there is enough duplicate(s) in the result[]
//    using the above example:
//    for the third '2', it is only processed is there are 2 '2's already in the result[]
//    else just ignore it
// this algorithm works because the data is processed sequentially.......

void GoToNextNumber(int n, int usermax)
{
    int i;
    int j;
    int taken;
    BOOL process;

    for (i = 0;i < max;i++)
    {
        process = true;
        if (room[i] == FREE)
        {
            if (skiplist[i] != FREE)
            {
                process = false;

                taken = 0;
                for (j = 0;j < n - 1;j++)
                {
                    if (data[i] == result[j])
                    {
                        taken++;
                    }
                }


                if (taken == skiplist[i])
                    process = true;
            }

            if (process == true)
            {
                room[i] = TAKEN;
                result[n - 1] = data[i];
                if (n < usermax)
                    GoToNextNumber(n + 1, usermax);
                else
                {
                    PrintResult(usermax);
                }
                room[i] = FREE;
            }
        }
    }
}

void CheckSkip(void)
{
    int i;
    int j;
    int taken;

    for (i = 0;i < max;i++)
    {
        taken = TAKEN;
        if (skiplist[i] == FREE)
        {
            for (j = i + 1;j < max;j++)
            {
                if (data[i] == data[j])
                {
                    skiplist[j] = taken;
                    taken++;
                }
            }
        }
    }
}

int CheckArguments (int argc, char ** argv)
{
    if (argc < 2)
    {
        Usage();
        return 0;
    }

    if (argv[1][0] == 'f')
    {
        if (argc < 3)
        {
            Usage();
            return 0;
        }

        screen = 0;

        pFile = fopen(argv[2],"w");

        if (pFile == NULL)
        {
            printf("Error Opening File ...\n");
            return 0;
        }

        if (argv[1][1] == 'l')
		{
			if (argv[1][2] == 'p')
				lexical = PHRASE_SEARCH;
			else
				lexical = WORD_SEARCH;
		}
    }
    else
	if (argv[1][0] == 'a')
    {
        if (argc < 3)
        {
            Usage();
            return 0;
        }

        pFile = fopen(argv[2],"w");

        if (pFile == NULL)
        {
            saved = 0;
        }

        if (argv[1][1] == 'l')
		{
			if (argv[1][2] == 'p')
				lexical = PHRASE_SEARCH;
			else
				lexical = WORD_SEARCH;
		}
    }
    else
	if (argv[1][0] == 's')
    {
        saved = 0;
        if (argv[1][1] == 'l')
		{
			if (argv[1][2] == 'p')
				lexical = PHRASE_SEARCH;
			else
				lexical = WORD_SEARCH;
		}
    }

    return 1;
}

// This function is to limit the number of combinations generated
// The formula of the total number of combinations is 
//    p!    *    q!
// --------
// (p-q)!q!
//     p!
// = ------
//   (p-q)!
// p is the length of the string
// q is the number of characters chosen
// a little bit of explanation here:
// the total number of q-long strings formed from p-long string multiplied by 
// the total number of combinations formed from q-long string

int NuclearMeltDown (int *lmax, unsigned int max)
{
	unsigned int CriticalMass = 1;
	unsigned int RestMass = 1;
	unsigned int Accelerator = max;
	int i;
	char ch;
	
	for (i = 0 ; ((4294967295 / RestMass) >= Accelerator) && (i < *lmax) ; i++, Accelerator--)
	{
		RestMass *= Accelerator;
	}
	
	if (RestMass < THRESHOLD)
		return 0;

	Accelerator = max;
	
	for (i = 0 ; ((4294967295 / CriticalMass) >= Accelerator) && Accelerator > 0 ; i++, Accelerator--)
	{
		CriticalMass *= Accelerator;
	}
		
	if (*lmax > i)
	{
		*lmax = i;
		RestMass = CriticalMass;
	}
	
	printf("\nWarning: Chars %d Total %u Combinations. Continue ? ",*lmax ,RestMass);
	scanf("\n%c",&ch);
	
	if (ch == 'y' || ch == 'Y')
		return 0;
	
	return -1;
}

int main (int argc, char ** argv)
{
    int lmax;
	int pmax;

    if (CheckArguments(argc, argv) == 0)
        return 0;

	// Open dictionary for lexical analyzer
    if (lexical != 0)
    {
        fdict = fopen(DICTIONARY_NAME,"r");
        if (fdict == NULL)
        {
            lexical = 0;
            printf("Error Opening Dictionary\n\n");
        }
    }

	// Input string from user
    printf("Enter String       : ");
    scanf("%s",data);
    max = strlen(data);
	pmax = max;
   	
	// Check length of string
    if (max > MAX_ROOM || max <= 0)
        return 0;

	// Input min number of combination
    printf("No. of Chars (Min) : ");
    scanf("%d",&lmax);
    if (lmax > max)
        lmax = max;
    if (lmax < DIC_MIN)
        lmax = DIC_MIN;

	// In case of phrase search input max number as well
	// and input suppress option
	if (lexical == PHRASE_SEARCH)
	{
		printf("No. of Chars (Max) : ");
		scanf("%d",&pmax);
		if (pmax > max)
			pmax = max;
		if (pmax < lmax)
			pmax = max;

		// clear stdin
		fflush(stdin);
		printf("Suppress < Min y/n : ");
		scanf("%c",&suppress);

		if (suppress == 'Y')
			suppress = 'y';
	}

	// Check max number of combination
    if (lexical == 0 && max > MAX_ROOM_NON_LEXICAL)
    {
	    if (NuclearMeltDown(&lmax, max) != 0)
	    	return 0;
	}
	
    printf("\n");

	// Rock n' Roll
    if (lexical != 0)
    {
        CheckLexical(lmax, pmax, lexical);
    }
    else
    {
        InitRooms();
        CheckSkip();
        GoToNextNumber(1,lmax);
    }

	printf("\n\nTotal Combination(s) : %d\n",total);

	// Wrap up
    if (fdict != NULL)
        fclose(fdict);

    return 0;
}
