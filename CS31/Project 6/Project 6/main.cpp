//
//  main.cpp
//  Project 6
//
//  Created by Michelle Kim on 5/24/22.
//

#include <iostream>
using namespace std;

/*
int main() // Expected output: " 30 20 10"
    {
        int arr[3] = { 5, 10, 15 };
        int* ptr = arr;

        *ptr = 10;          // set arr[0] to 10
        *(ptr + 1) = 20;      // set arr[1] to 20
        ptr += 2;
        ptr[0] = 30;        // set arr[2] to 30

        while (ptr >= arr)
        {
            cout << ' ' << *ptr;    // print values
            ptr--;
        }
    cout << endl;
    }
 */

// 1c.

#include <iostream>
    #include <cmath>
    using namespace std;

    void hypotenuse(double leg1, double leg2, double* resultPtr)
    {
        *resultPtr = sqrt(leg1*leg1 + leg2*leg2);
    }

    int main()
    {
        double a;
        double* p = &a;
        
        hypotenuse(1.5, 2.0, p);
        cout << "The hypotenuse is " << *p << endl;
    }
