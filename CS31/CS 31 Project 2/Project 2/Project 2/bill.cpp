//
//  bill.cpp
//  Project 2
//
//  Created by Michelle Kim on 4/10/22.
//

#include <iostream>
#include <string>
using namespace std;

int main()
{
    // Gathering input
    
    cout << "Initial meter reading: ";
    int initialReading;
    cin >> initialReading;
    
    cout << "Final meter reading: ";
    int finalReading;
    cin >> finalReading;
    
    cin.ignore(10000, '\n');
    
    cout << "Customer name: ";
    string customerName;
    getline(cin, customerName);
        
    cout << "Month number (1=Jan, 2=Feb, etc.): ";
    int month;
    cin >> month;
    
    cout << "---" << endl;
    
    int water = finalReading - initialReading;
    double total;
  
    // High season
    
    if (month == 4 || month == 5 || month == 6 || month == 7 || month == 8 || month == 9 || month == 10)
    {
        if (water <= 23)
        {
            total = water * 5.41;
        }
        else
        {
            total = 23 * 5.41 + (water - 23) * 9.79;
        }
    }
    
    // Low season
    
    if (month == 11 || month == 12 || month == 1 || month == 2 || month == 3)
    {
        if (water <= 15)
        {
            total = water * 5.41;
        }
        else
            total = 15 * 5.41 + (water - 15) * 7.77;
    }
    

    // Write only the error message for the earliest erroneous input item.

    if (initialReading < 0 || finalReading < initialReading || customerName == "" || month < 1 || month > 12)
    {
        if (initialReading < 0)
        {
        cout << "The initial meter reading must not be negative." <<endl;
        }
        else
        {
            if (finalReading < initialReading)
            {
                cout << "The final meter reading must be at least as large as the initial reading." << endl;
            }
            else
            {
                if (customerName == "")
                {
                    cout << "You must enter a customer name." << endl;
                }
                else
                {
                    if (month < 1 || month > 12)
                    {
                        cout << "The month number must be in the range 1 through 12." <<endl;
                    }
                }
            }
        }
    }

    // If there are no other errors, output the bill value and ensure that it contains two decimal points.
    else
    {
        cout.setf(ios::fixed);
        cout.precision(2);
        cout << "The bill for " << customerName << " is $" << total << endl;
    }
}
