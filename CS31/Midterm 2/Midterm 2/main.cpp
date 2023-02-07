//
//  main.cpp
//  Midterm 2
//
//  Created by Michelle Kim on 5/17/22.
//

// Submit ONLY the obscure function. Do not submit #include lines or a main routine.

/* FIRST PROBLEM ON EXAM
 
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>
using namespace std;

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE COMMENT AT THE END OF THE obscure FUNCTION

// Replace every character in s that is neither a lower case letter nor a digit character with the character ch.
int obscure(char s[], char ch)
{
    int charsReplaced = 0;
    
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (islower(s[i]) || isdigit(s[i])) {}
        // If a character in s that is neither a lower case letter nor a digit is the same character as ch, it still counts as a replaced character.
        else
        {
            s[i] = ch;
            charsReplaced++;
        }
    }
    
    return charsReplaced;
}

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE COMMENT AT THE START OF THE obscure FUNCTION

int main()
{
    char text[100] = "I'm UPSET that on Apr. 29th, 2022, my 2 brand-new BMW i7s were stolen!!";
    assert(obscure(text, '*') == 30);
    assert(strcmp(text, "**m*******that*on**pr**29th**2022**my*2*brand*new*****i7s*were*stolen**") == 0);
    
    strcpy(text, "#%@!");
    assert(obscure(text, 'x') == 4);
    assert(strcmp(text, "xxxx") == 0);
    
    strcpy(text, "73kg");
    assert(obscure(text, '?') == 0);
    assert(strcmp(text, "73kg") == 0);
    
    strcpy(text, "");
    assert(obscure(text, '-') == 0);
    assert(strcmp(text, "") == 0);
    
    cout << "All tests succeeded" << endl;
}

*/

/* ---------------------------------------------------------------------------- */

// SECOND PROBLEM ON EXAM

// Interpret an integer array representing for each second of the chase, how many feet Angie Anchovy's lead over Celia Seal increased or decreased by.

// The following line avoids a VC++ issue; it's harmless for Xcode and g31
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cassert>
using namespace std;

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE COMMENT AT THE END OF THE interpret FUNCTION

int interpret(const int changes[], int n, int& maxLead)
{
    /* changes: int array representing a sequence of changes in Angies's lead
     n: how many elements of changes to examine */
        
    // n is not positive
    if (n <= 0)
        return -3;
    
    // changes contains a bad change value (less than -15 or greater than 15)
    for (int i = 0; i < n; i++)
    {
        if (changes[i] < -15 || changes[i] > 15)
            return -2;
    }
    
    // At some point in the sequence, a change value causes Angie's lead over Celia to be negative
    int currentLead = 30;
    for (int j = 0; j < n; j++)
    {
        currentLead += changes[j];
        if (currentLead < 0)
            return -1;
    }
    
    // Otherwise, return the distance between Celia and Angie after processing all the changes in the array.
    currentLead = 30;
    maxLead = 30;
    for (int k = 0; k < n; k++)
    {
        currentLead += changes[k];
        if (currentLead >= maxLead)
            maxLead = currentLead;
    }
    
    return currentLead;
}

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE COMMENT AT THE START OF THE interpret FUNCTION

int main()
{
    
    int chg1[13] = { 5, 2, 0, -3, 6, 2, -4, 0, 0, -5, -5, 1, -8 };
    int m;
    assert(interpret(chg1, 13, m) == 21);
    assert(m == 42);

    int chg2[7] = { 0, 2, -12, -9, -13, 10, 10 };
    m = 999;
    assert(interpret(chg2, 7, m) == -1);
    assert(m == 999);
 
    int chg3[3] = { 10, 20, 10 };
    m = 999;
    assert(interpret(chg3, 3, m) == -2);
    assert(m == 999);

    cout << "All tests succeeded" << endl;
}
