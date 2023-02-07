//
//  jitter.cpp
//  Project 5
//
//  Created by Michelle Kim on 5/16/22.
//

#include <iostream>
#include <string>
#include <cctype>
#include <cassert>
using namespace std;

// Do not use c++ strings or global variables that can be altered during execution. Only use c strings.
const int MAX_WORD_LENGTH = 20;
int editStandards(int distance[],
                  char word1[][MAX_WORD_LENGTH+1],
                  char word2[][MAX_WORD_LENGTH+1],
                  int nStandards);
int determineMatchLevel(const int distance[],
                        const char word1[][MAX_WORD_LENGTH+1],
                        const char word2[][MAX_WORD_LENGTH+1],
                        int nStandards,
                        const char jeet[]);

int main()
        {
            const int TEST1_NSTANDARDS = 4;
            int test1dist[TEST1_NSTANDARDS] = {
                2,             4,         1,         13
            };
            char test1w1[TEST1_NSTANDARDS][MAX_WORD_LENGTH+1] = {
                "eccentric",   "space",  "electric", "were"
            };
            char test1w2[TEST1_NSTANDARDS][MAX_WORD_LENGTH+1] = {
                "billionaire", "capsule", "car", "eccentric"
            };
            assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
                "The eccentric outspoken billionaire launched a space station cargo capsule.") == 2);
            assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
                "The eccentric outspoken billionaire launched    a space capsule.") == 2);
            assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
                "**** 2022 ****") == 0);
            assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
                "space space capsule space capsule capsule") == 1);
            assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
                "Two eccentric billionaires were space-capsule riders.") == 0);
            assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
                "  It's an ELECTRIC car!") == 1);
            cout << "All tests succeeded" << endl;
        }

// Function 1: Put a collection of purported match standards into standard form.
int editStandards(int distance[],
                  char word1[][MAX_WORD_LENGTH+1],
                  char word2[][MAX_WORD_LENGTH+1],
                  int nStandards)
{
    if (nStandards <= 0)
        return 0;
    for (int i = 0; i < nStandards; i++)
    {
        for (int j = 0; ; j++)
        {
            tolower(word1[i][j]);
            tolower(word2[i][j]);
            // Remove erroneous standards from the array. isalpha additionally ensures each word does not contain no characters.
            if (distance[i] <= 0 || isalpha(word1[i][j]) == false || isalpha(word2[i][j]) == false)
            {
                if (j != 0 || word1[i][j] == '\0' || word2[i][j] == '\0')
                    break; // A null character at j = 0 is invalid.
                // Erase erroneous standards from the array.
                for (int k = 0; k < MAX_WORD_LENGTH+1; k++)
                {
                    word1[i][k] = '\0';
                    word2[i][k] = '\0';
                }
            }
        }
    }
    
    // Check for duplicate standards and erase them from the array.
    for (int i = 0; i < nStandards; i++)
    {
        int j = 0;
        for (int m = 0; m < nStandards; m++)
        {
            int q = 999;
            //  Do not evaluate if the first character is a '\0'. If word1[m][n] was set to '\0' in the previous step, word2[m][n] was as well. Do not evaluate a string against itself.
            if (m != i || word1[m][0] != '\0')
            {
                for (int n = 0; q == 999;)
                {
                    if (word1[m][n] == word1[i][j] && word1[m][n] != '\0')
                    {
                        n++;
                        j++;
                    }
                    else if (word1[m][n] == '\0' && word1[i][j] == '\0')
                    {
                        // Evaluate the second word.
                        for (int p = 0; ; p++)
                        {
                            if (word2[m][p] == word2[i][j] && word2[m][p] != '\0')
                                j++;
                            else if (word2[m][p] == '\0' && word2[i][j] == '\0')
                            {
                                // If 2+ standards have the same w1 and w2, only keep one with the highest distance and remove the rest.
                                if (distance[m] >= distance[i])
                                    distance[i] = distance[m];
                                for (int k = 0; k < MAX_WORD_LENGTH+1; k++)
                                {
                                    word1[m][k] = '\0';
                                    word2[m][k] = '\0';
                                }
                                n = 0;
                                j = 0;
                                q = 2000;
                                break;
                                // Check for other standards with the same w1 and w2 before moving on.
                            }
                            else
                            {
                                q = 1000;
                                break;
                            }
                        }
                    }
                    else break;
                }
            }
        }
    }

    // Move the last standard into the position of each empty standard.
    // The match standards are in positions 0 to nStandards-1.
    for (int i = 0; i < nStandards; i++)
    {
        if (word1[i][0] == '\0')
        {
            for (int r = 0; word1[i][r] != '\0'; r++)
            {
                word1[i][r] = word1[nStandards-1][r];
                word2[i][r] = word2[nStandards-1][r];
                distance[r] = distance[nStandards-1];
                nStandards--;
            }
        }
    }
    
    // Return the number of standards in the set.
    return nStandards--;
}

// Function 2: Compute the match level of a jeet WITHOUT MODIFYING THE ORIGINAL JEET. Assume match standards are in standard form. The length of every jeet (not counting the zero byte) is <= 280 characters.
int determineMatchLevel(const int distance[],
                        const char word1[][MAX_WORD_LENGTH+1],
                        const char word2[][MAX_WORD_LENGTH+1],
                        int nStandards,
                        const char jeet[])
{
    int numberMatches = 0;
    if (nStandards <= 0)
        return numberMatches;
    
    for (int i = 0; jeet[i] != '\0'; i++)
        tolower(jeet[i]);
        
    // Make a copy of the original jeet.
    int z;
    char jeetCopy[281];
    for (int a = 0; ; a++)
    {
        jeetCopy[a] = jeet[a];
        if (jeet[a] == '\0')
        {
            jeetCopy[a] = jeet[a];
            z = a;
            break;
        }
    }
    for (;z < 281; z++)
        jeetCopy[z] = '\0';
    
    // Words are separated by one or more space characters; all other non-alphabetic characters are not word separators.
    // Remove all non-alphabetic characters that are not spaces from jeetCopy.
    for (int a = 0; a < 281; a++)
    {
        if (isspace(jeetCopy[a]) || isalpha(jeetCopy[a])) {}
        else
        {
            for (int c = a; c < 281 ; c++)
            {
                if (c+1 < 281 && jeetCopy[c+1] != '\0')
                    jeetCopy[c] = jeetCopy[c+1];
                else // jeetCopy[c+1] == '\0'
                {
                    if (c+1 < 281)
                    {
                        jeetCopy[c] = jeetCopy[c+1];
                        break;
                    }
                }
            }
        }
    }
    
    // Loop through each match standard separately.
    for (int j = 0; j < nStandards; j++)
    {
        bool matchFound = false;
        // Find a match to the first char of w1.
        // Find the first alpha in jeetCopy, then iterate through its characters.
        for(int m = 0; jeetCopy[m] != '\0'; m++)
        {
            if (isalpha(jeetCopy[m]))
            {
                if (jeetCopy[m] == word1[j][0])
                {
                    // Check the rest of the letters for a w1 match.
                    int k = 0;
                    for (int n = m;;)
                    {
                        if (word1[j][k] == '\0' && (isspace(jeetCopy[n]) || jeetCopy[n] == '\0'))
                        // Check if w2 matches if w1 matches. If w1 doesn't match, move to the next word in jeetCopy.
                        {
                            for(int s = n; jeetCopy[s] != '\0'; s++)
                            {
                                if (jeetCopy[s] == word2[j][0]) // Check the rest of the letters for a w2 match.
                                {
                                    int t = 0;
                                    for (int u = s;;)
                                    {
                                        if (word2[j][t] == '\0' && (isspace(jeetCopy[u]) || jeetCopy[u] == '\0'))
                                        // Check if w2 matches. If w2 doesn't match, move to the next word in jeetCopy.
                                        // A jeet matches a standard if w2 appears at least 1 word and no more than 'distance' words after w1. If a jeet matches a standard, the number of ways it does so is irrelevant (it only counts once).
                                        {
                                            int v = s;
                                            int currentDifference = 0;
                                            for (;isspace(jeetCopy[v]) && isalpha(jeetCopy[v+1]) && v < u;)
                                            {
                                                currentDifference++;
                                                v++;
                                            }
                                            
                                            if (distance[j] - currentDifference >= 0)
                                            {
                                                numberMatches++;
                                                matchFound = true;
                                                break;
                                            }
                                        }
                                        
                                        if (word2[j][t] != jeetCopy[u])
                                        {
                                            for (; isalpha(jeetCopy[s]) && jeetCopy[s+1] != '\0'; s++) {}
                                            break;
                                        }
                                        
                                        t++; u++;
                                    }
                                }
                            if (matchFound == true)
                                break;
                            }
                        }

                        if (word1[j][k] != jeetCopy[n])
                        {
                            for (; isalpha(jeetCopy[m]) && jeetCopy[m+1] != '\0'; m++) {}
                            break;
                        }
                        
                        k++; n++;
                        if (matchFound == true)
                            break;
                    }
                }
                else    // Move to the next word in jeetCopy.
                {
                    for (; isalpha(jeetCopy[m]) && jeetCopy[m+1] != '\0'; m++) {}
                }
            }
        }
    }
    // The function returns the match level of the jeet (the number of match standards the jeet matches).
    return numberMatches;
}
