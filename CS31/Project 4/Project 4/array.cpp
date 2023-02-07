//
//  array.cpp
//  Project 4
//
//  Created by Michelle Kim on 5/4/22.
//

#include <iostream>
#include <string>
using namespace std;

int enumerate(const string a[], int n, string target);
int findMatch(const string a[], int n, string target);
bool findRun(const string a[], int n, string target, int& begin, int& end);
int findMin(const string a[], int n);
int moveToEnd(string a[], int n, int pos);
int moveToBeginning(string a[], int n, int pos);
int findDifference(const string a1[], int n1, const string a2[], int n2);
int removeDups(string a[], int n);
bool subsequence(const string a1[], int n1, const string a2[], int n2);
int makeMerger(const string a1[], int n1, const string a2[], int n2, string result[], int max);
int divide(string a[], int n, string divider);

int main ()
{
    string test[10] = {"ayato", "xiao", "yunjin", "ayato", "raiden", "chongyun", "yae miko", "yae miko", "zhongli", "yae miko"};
    cout << divide(test, 10, "kazuha") << endl;
}

/*
 All functions that return an int must return −1 if they are passed any bad arguments (e.g. a position that would require looking at the contents of an element past the last element we're interested in).
 
 Passing 0 to the function as the array size is not an error. A sequence of 0 elements is a subsequence of every sequence (even another one with no elements), at position 0.

 The one error your function implementations don't have to handle (and thus we won't test for) is when the caller of the function says to examine more items in the array than it actually has.
  
 Empty strings are valid.
 */

// Return the number of strings in the array that are equal to target.
int enumerate(const string a[], int n, string target)
{
    int numberEqual = 0;
    if (n < 0)
        return -1;
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (a[i] == target)
                numberEqual++;
        }
        return numberEqual;
    }
}

// Return the position of a string in the array that is equal to target; if there is more than one such string, return the smallest position number of such a matching string. Return −1 if there is no such string.
int findMatch(const string a[], int n, string target)
{
    int position = 0;
    bool stringExists = false;
    if (n < 0)
        return -1;
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (a[i] == target)
            {
                stringExists = true;
                break;
            }
            else
                position++;
        }
        if (stringExists == false)
            return -1;
        else
            return position;
    }
}

// Find the earliest occurrence of one or more consecutive strings that are equal to target; set begin to the position of the first occurrence of target, set end to the last occurrence of target in that earliest consecutive sequence, and return true. If no string in a is equal to target, leave begin and end unchanged and return false.
bool findRun(const string a[], int n, string target, int& begin, int& end)
{
    if (n < 0)
        return false;
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (a[i] == target)
            {
                begin = i;
                for (int j = i + 1; j < n; j++) // Check if consecutive strings match target
                {
                    if (a[j] != target)
                    {
                        end = j - 1;
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
}

// Return the position of a string in the array such that that string is <= every string in the array. If there is more than one such string, return the smallest position number of such a string.
int findMin(const string a[], int n)
{
    if (n <= 0)
        return -1;
    else
    {
        string smallestString = a[0];
        int smallestIndex = 0;
        for (int i = 0; i < n; i++)
        {
            if (a[i] < smallestString)
            {
                smallestIndex = i;
                smallestString = a[i];
            }
        }
        return smallestIndex;
    }
}

// Eliminate the value at position pos by copying all elements after it one place to the left. Put the value that was thus eliminated into the last position of the array that the function knows about.
int moveToEnd(string a[], int n, int pos)
{
    if (n < 0 || pos >= n)
        return -1;
    else
    {
        string valueMoved = a[pos];
        for (int i = pos; i + 1 < n; i++)
        {
            a[i] = a[i+1];
        }
        a[n-1] = valueMoved;
        return pos;
    }
}

// Eliminate the value at position pos by copying all elements before it one place to the right. Put the value that was thus eliminated into the first position of the array.
int moveToBeginning(string a[], int n, int pos)
{
    if (n < 0 || pos >= n)
        return -1;
    else
    {
        string valueMoved = a[pos];
        for (int i = pos; i - 1 >= 0; i--)
        {
            a[i] = a[i-1];
        }
        a[0] = valueMoved;
        return pos;
    }
}

// Return the position of the first corresponding elements of a1 and a2 that are not equal. n1 is the number of interesting elements in a1, and n2 is the number of interesting elements in a2. If the arrays are equal up to the point where one or both runs out, return whichever value of n1 and n2 is less than or equal to the other.
int findDifference(const string a1[], int n1, const string a2[], int n2)
{
    int n;
    if (n1 < 0 || n2 < 0)
        return -1;
    else if (n1 == 0 || n2 == 0)
        return 0;
    else
    {
        if (n1 > n2)
            n = n2; // n is the smaller value between n1 and n2.
        else
            n = n1;
        
        for (int i = 0; i < n; i++)
        {
            if (a1[i] != a2[i])
                return i;
        }
        return n;
    }
}

// For every sequence of consecutive identical items in a, retain only one item of that sequence. Suppose we call the number of all retained items r. Then when this functions returns, elements 0 through r-1 of a must contain the retained items (in the same relative order they were in originally), and the remaining elements may have whatever values you want.
int removeDups(string a[], int n)
{
    int placeIndex = 0;
    int nextIndex = 0;
    if (n < 0)
        return -1;
    else
    {
        for (int i = 0; i < n;)
        {
            for (int j = 1; j + i < n; j++) // j is the number of additional consecutive identical items.
            {
                if (i+j < n && a[i+j] == a[i])
                    nextIndex = i + j + 1;
                else
                {
                    nextIndex = i + j;
                    break;
                }
            }
            if (nextIndex - 1 >= 0)
                a[placeIndex] = a[nextIndex - 1];
            placeIndex++;
            i = nextIndex;
        }
        return placeIndex;
    }
}

// If all n2 elements of a2 appear in a1, in the same order (though not necessarily consecutively), then return true. Return false otherwise.
bool subsequence(const string a1[], int n1, const string a2[], int n2)
{
    int index1 = 0;
    int index2 = 0; // Count the index of each element in a2 confirmed to appear in a1 in the correct order.
    if (n1 < 0 || n2 < 0)
        return false;
    else if (n2 == 0)
        return true;
    else
    {
        for (int i = 0; i < n2; i++)
        {
            for (int j = index1; j < n1; j++)
            {
                if (a1[j] == a2[i])
                {
                    index2 = i;
                    break;
                }
                else
                    index1++;
            }
            index1++;
        }
    }
    
    if (index2 == n2 - 1)
        return true;
    else
        return false;
}

// Place in result all the elements of a1 and a2, arranged in nondecreasing order, and return the number of elements so placed. (Nondecreasing order means that no item is > the one that follows it.)
int makeMerger(const string a1[], int n1, const string a2[], int n2, string result[], int max)
{
    if (n1 < 0 || n2 < 0 || n1 + n2 > max)
        return -1;
    else
    {
        // Check that both a1 and a2 are in nondecreasing order.
        for (int i = 0; i < n1; i++)
        {
            if (i+1 < n1 || a1[i] <= a1[i+1]) {}
            else
                return -1;
        }
    
        for (int k = 0; k < n2; k++)
        {
            if (k+1 < n2 || a1[k] <= a1[k+1]) {}
            else
                return -1;
        }
        
        // Build the result[] array.
        int index1 = 0;
        int index2 = 0;
        int j = 0;
        while (index1 < n1 && index2 < n2)
        {
            if (a1[index1] <= a2[index2])
            {
                result[j] = a1[index1];
                index1++;
            }
            else
            {
                result[j] = a2[index2];
                index2++;
            }
            j++;
        }
        
        if (index1 == n1) // The rest of result is what remains of a2.
            for (; index2 < n2; index2++)
            {
                result[j] = a2[index2];
                j++;
            }
        else if (index2 == n2) // The rest of result is what remains of a1.
            for (; index1 < n1; index1++)
            {
                result[j] = a1[index1];
                j++;
            }
        
        return n1 + n2;
    }
}

// Rearrange the elements of the array so that all the elements whose value is < divider come before all the other elements, and all the elements whose value is > divider come after all the other elements. Return the position of the first element that, after the rearrangement, is not < divider, or n if there are no such elements.
// Divider string goes in the middle.
int divide(string a[], int n, string divider)
{
    string b[n];
    int indexUp = 0;
    int indexDown = n-1;
    if (n < 0)
        return -1;
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (a[i] < divider)
            {
                b[i] = a[i];
                indexUp++;
            }
            else if (a[i] > divider)
            {
                b[n-1-i] = a[i];
                indexDown--;
            }
        }
        for (int j = indexUp; j <= indexDown; j++)
            b[j] = divider;
        for (int k = 0; k < n; k++)
            a[k] = b[k];
        return indexUp;
    }
}
