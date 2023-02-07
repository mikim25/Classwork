//
//  hero.cpp
//  CS 31 Project 3
//
//  Created by Michelle Kim on 4/23/22.
//

#include <iostream>
#include <cctype>
#include <cassert>
#include <string>
using namespace std;

// Each beat will convert into one instruction letter. Capital vs. lowercase matters in output, not in input. Only report the earliest error.

    bool hasProperSyntax(string tune);
    int convertTune(string tune, string& instructions, int& badBeat);

    int main()
    {
        for(;;)
        {
        string t;
        string instrs = "ayato";
        int bbeat = 999;
        
        cout << "artist's tune: ";
        getline(cin, t);
        if (t == "quit")
            break;
        
        if (hasProperSyntax(t))
            cout << "good syntax" << endl;
        else
            cout << "bad syntax" << endl;
    
        cout << "error code: " << convertTune(t, instrs, bbeat) << endl;
        convertTune(t, instrs, bbeat);
        cout << "bad beat is now: " << bbeat << endl;
        cout << "conversion is now: " << instrs << endl;
        }
    }

    bool hasProperSyntax(string tune)
    {
        // Separating special case of empty string
        if (tune != "")
        {
            for (int k = 0; k != tune.size(); k++)
            {
                // Check that each character in the tune is a color, digit, or a slash.
                switch (tune.at(k))
                {
                    case 'G': case 'g':
                    case 'R': case 'r':
                    case 'Y': case 'y':
                    case 'B': case 'b':
                    case 'O': case 'o':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    case '0':
                    case '/':
                        break;
                    default:
                        return false;
                }
            
                // Check that the tune is comprised of valid beats and starts with an alpha or /.
                // Check that the function never accesses an index out of bounds.
                if (isdigit(tune.at(0)))
                    return false;
                if (isdigit(tune.at(k)) && k-1 >= 0 && tune.at(k-1) == '/')
                    return false;
                if (isalpha(tune.at(k)))
                {
                    if (k+1 < tune.size() && tune.at(k+1) == '/') {}
                    else if (k+2 < tune.size() && isdigit(tune.at(k+1)) && tune.at(k+2) == '/') {}
                    else if (k+3 < tune.size() && isdigit(tune.at(k+1)) && isdigit(tune.at(k+2)) && tune.at(k+3) == '/') {}
                    else
                        return false;
                }
            }
        }
        // If no syntax errors are caught here...
        return true;
    }

    int convertTune(string tune, string& instructions, int& badBeat)
    {
        if (tune == "")
            return 0;
        
        int beatNumber = 0;

        for (int j = 0; j < tune.size(); j++)
        {
            bool possibleSustain = true;
            
            // Check if the input has incorrect syntax.
            if (hasProperSyntax(tune) == false)
                return 1;
            
            // Determine the current beat number and starting index.
            // Separate the case in which the first beat is a '/'.
            if (isalpha(tune.at(j)) && j+1 < tune.size() && tune.at(j+1) == '/')
            {
                beatNumber++;
                possibleSustain = false;
            }
            else if (isalpha(tune.at(j)) && j+2 < tune.size() && isdigit(tune.at(j+1)) && tune.at(j+2) == '/')
            {
                beatNumber++;
                possibleSustain = true;
            }
            else if (isalpha(tune.at(j)) && j+3 < tune.size() && isdigit(tune.at(j+1)) && isdigit(tune.at(j+2)) && tune.at(j+3) == '/')
            {
                beatNumber++;
                possibleSustain = true;
            }
            else if (j-1 >= 0 && tune.at(j) == '/' && tune.at(j-1) == '/')
            {
                beatNumber++;
            }
            else if (j == 0 && tune.at(j) == '/')
            {
                beatNumber++;
                possibleSustain = false;
            }
            
            // Check if any beat specifies a sustained note of length 0 or 1.
            if (isdigit(tune.at(j)))
            {
                // Consider a 1-digit sustain.
                if (j+1 < tune.size() && tune.at(j+1) == '/' && isalpha(tune.at(j-1)))
                {
                    if (tune.substr(j,1) == "0" || tune.substr(j,1) == "1")
                    {
                        badBeat = beatNumber;
                        return 3;
                    }
                }
                // Consider a 2-digit sustain.
                else if (j+2 < tune.size() && isdigit(tune.at(j+1)) && tune.at(j+2) == '/')
                {
                    if (tune.substr(j,2) == "00" || tune.substr(j,2) == "01")
                    {
                        badBeat = beatNumber;
                        return 3;
                    }
                }
            }
            
            // Check if a beat consisting of something other than a slash is present while a sustained note is in effect.
            // Separate the possibility that the tune ends prematurely.
            if (possibleSustain == true)
            {
                if (isdigit(tune.at(j)))
                {
                    // Consider a 1-digit sustain.
                    if (j+1 < tune.size() && tune.at(j+1) == '/' && isalpha(tune.at(j-1)))
                    {
                        for (int h = 1; h <= stoi(tune.substr(j,1)); h++)
                        {
                            if (j+h < tune.size() && tune.at(j+h) == '/') {}
                            else if (j+h < tune.size())
                            {
                                badBeat = beatNumber + h - 1;
                                return 2;
                            }
                            // Check if the tune ends prematurely.
                            else // (j+h >= tune.size())
                            {
                                badBeat = beatNumber + h - 1;
                                return 4;
                            }
                            
                        }
                            
                    }
                    // Consider a 2-digit sustain.
                    else if (j+2 < tune.size() && isdigit(tune.at(j+1)) && tune.at(j+2) == '/')
                    {
                        // Check if the integer begins with 0.
                        if (tune.at(j) == '0')
                        {
                            for (int f = 1; f <= stoi(tune.substr(j+1,1)); f++)
                            {
                                if (j+1+f < tune.size() && tune.at(j+1+f) == '/') {}
                                else if (j+1+f < tune.size())
                                {
                                    badBeat = beatNumber + f - 1;
                                    return 2;
                                }
                                // Check if the tune ends prematurely.
                                else // (j+1+f >= tune.size())
                                {
                                    badBeat = beatNumber + f - 1;
                                    return 4;
                                }
                            }
                        }
                        // Otherwise, the integer must span two digits.
                        else
                        {
                            for (int m = 1; m <= stoi(tune.substr(j,2)); m++)
                            {
                                if (j+1+m < tune.size() && tune.at(j+1+m) == '/') {}
                                else if (j+1+m < tune.size())
                                {
                                    badBeat = beatNumber + m - 1;
                                    return 2;
                                }
                                // Check if the tune ends prematurely.
                                else // (j+1+m >= tune.size())
                                {
                                    badBeat = beatNumber + m - 1;
                                    return 4;
                                }
                            }
                        }
                            
                    }
                }
            }
        }
        
        // The parameter 'tune' is convertible; set 'instructions' to the conversion of the tune.
        string output = "";
        if (tune == "")
        {
            instructions = "";
            return 0;
        }
        else
        {
            for (int p = 0; p < tune.size();)
            {
                if (isalpha(tune.at(p)) && tune.at(p+1) == '/')
                {
                    output += tolower(tune.at(p));
                    p += 2;
                }
                else if (isalpha(tune.at(p)) && isdigit(tune.at(p+1)) && tune.at(p+2) == '/')
                {
                    for (int q = 1; q <= tune.at(p+1) - '0'; q++)
                    {
                        output += toupper(tune.at(p));
                    }
                    p = p + 2 + (tune.at(p+1) - '0');
                }
                else if (isalpha(tune.at(p)) && isdigit(tune.at(p+1)) && isdigit(tune.at(p+2)) && tune.at(p+3) == '/')
                {
                    // Consider a sustain that starts with 0.
                    if (tune.at(p+1) == '0')
                    {
                        for (int z = 1; z <= stoi(tune.substr(p+2,1)); z++)
                        {
                            output += toupper(tune.at(p));
                        }
                        p = p + 3 + (tune.at(p+2) - '0');
                    }
                    //Consider a sustain that is a two-digit integer.
                    else
                    {
                        for (int y = 1; y <= stoi(tune.substr(p+1,2)); y++)
                        {
                            output += toupper(tune.at(p));
                        }
                        p = p + 3 + stoi(tune.substr(p+1,2));
                    }
                }
                else
                {
                    output.append("x");
                    p++;
                }
            }
            instructions = output;
            return 0;
        }
    }
