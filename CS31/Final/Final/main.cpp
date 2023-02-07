//
//  main.cpp
//  Final
//
//  Created by Michelle Kim on 6/4/22.
//

/* BIGGIE PROBLEM
 
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AT THE END OF THE biggie FUNCTION

// The function returns a pointer to the string in the array that has the longest length of all the strings in the portion of the array from first to last INCLUSIVE.

// If more than one string in that portion has the same longest length, the function returns a pointer to the one that appears LATER in that portion (i.e., is at a position farther from first and closer to last)

string* biggie(string* first, string* last, int* numEmpty)
{
    string* longest = first;
    int a = 0;
    if (*first == "")
        a++;
    first++;
    for (; first != (last+1) ;first++)
    {
        if ((*first).size() >= (*(first - 1)).size())
            longest = first;
        if (*first == "")
            a++;
    }
    
    if (numEmpty != nullptr)
    {
        *numEmpty = a;
    }
    
    return longest;
}

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AT THE START OF THE biggie FUNCTION

int main()
{
    string a[9] = { "cat", "", "mouse", "", "", "fish", "dog", "bunny", "bird" };
    int empties = 999;
    assert(biggie(a, a+3, &empties) == a+2  &&  empties == 2);
    assert(biggie(a+5, a+8, &empties) == a+7  &&  empties == 0);
    empties = 888;
    assert(biggie(a, a+3, nullptr) == a+2  &&  empties == 888);
    assert(biggie(a, a+8, &empties) == a+7  &&  empties == 3);
    assert(biggie(a+1, a+1, &empties) == a+1  &&  empties == 1);
    assert(biggie(a, a, &empties) == a  &&  empties == 0);

    cout << "All tests succeeded" << endl;
}
 
 */

/* VENDING PROBLEM
 
#include <iostream>
#include <cassert>
using namespace std;

class Dispenser
{
  public:
    Dispenser();
    int addCans(int num);
    bool dispenseCan();
  private:
    int m_numCans;
    int m_capacity;
};
    
Dispenser::Dispenser()
{
    m_numCans = 0;
    m_capacity = 150;
}

int Dispenser::addCans(int num)
{
    if (num <= 0)
        return 0;
    if (m_numCans + num > m_capacity)
        num = m_capacity - m_numCans;
    m_numCans += num;
    return num;
}
    
bool Dispenser::dispenseCan()
{
    if (m_numCans == 0)
        return false;
    m_numCans--;
    return true;
}

class VendingMachine
{
  public:
    VendingMachine(int price, int startAmount);
    int addCans(int num);
    int purchase(int amt);
    int moneyRetained() const;
  private:
    Dispenser m_dispenser;
    int       m_price;
    int       m_totalMoneyKept;
};

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AT THE END OF THE VendingMachine MEMBER FUNCTIONS

// Write the implementations of the four functions. Use the m_dispenser member appropriately in your implementations.

VendingMachine::VendingMachine(int price, int startAmount)
{
    m_price = price;
    m_dispenser.addCans(startAmount);
    m_totalMoneyKept = 0;
}

int VendingMachine::addCans(int num)
{
    return m_dispenser.addCans(num);
}

int VendingMachine::purchase(int amt)
{
    if (amt > (150 * m_price))
    {
        m_totalMoneyKept = 150 * m_price;
        return amt - 150 * m_price;
    }
    for (;amt >= m_price;)
    {
        m_dispenser.dispenseCan();
        amt -= m_price;
        m_totalMoneyKept += m_price;
    }
    return amt;
}

int VendingMachine::moneyRetained() const
{
    return m_totalMoneyKept;
}

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AT THE START OF THE VendingMachine MEMBER FUNCTIONS

int main()
{
    VendingMachine vm(5, 25);      // 25 cans to start, each will cost 5 blorks
    assert(vm.addCans(35) == 35);  // So now vm holds 60 cans
    assert(vm.purchase(47) == 2);  // 9 cans dispensed, 2 blorks change
    assert(vm.moneyRetained() == 45);  // vm retains 45 blorks so far

    cout << "All tests succeeded" << endl;
}

*/

#include <iostream>
#include <cassert>
using namespace std;

class Dispenser
{
  public:
    Dispenser();
    int addCans(int num);
    bool dispenseCan();
  private:
    int m_numCans;
    int m_capacity;
};
    
Dispenser::Dispenser()
{
    m_numCans = 0;
    m_capacity = 150;
}

int Dispenser::addCans(int num)
{
    if (num <= 0)
        return 0;
    if (m_numCans + num > m_capacity)
        num = m_capacity - m_numCans;
    m_numCans += num;
    return num;
}
    
bool Dispenser::dispenseCan()
{
    if (m_numCans == 0)
        return false;
    m_numCans--;
    return true;
}

class VendingMachine
{
  public:
    VendingMachine(int price, int startAmount);
    int addCans(int num);
    int purchase(int amt);
    int moneyRetained() const;
  private:
    Dispenser m_dispenser;
    int       m_price;
    int       m_totalMoneyKept;
};

// Write the implementations of the four functions. Use the m_dispenser member appropriately in your implementations.

VendingMachine::VendingMachine(int price, int startAmount)
{
    m_price = price;
    m_dispenser.addCans(startAmount);
    m_totalMoneyKept = 0;
}

int VendingMachine::addCans(int num)
{
    return m_dispenser.addCans(num);
}

int VendingMachine::purchase(int amt)
{
    if (amt > (150 * m_price))
    {
        m_totalMoneyKept = 150 * m_price;
        return amt - 150 * m_price;
    }
    for (;amt >= m_price;)
    {
        m_dispenser.dispenseCan();
        amt -= m_price;
        m_totalMoneyKept += m_price;
    }
    return amt;
}

int VendingMachine::moneyRetained() const
{
    return m_totalMoneyKept;
}

// SUBMIT START HERE

VendingMachine* createVM()
{
    VendingMachine* machine[100];
    int nMachines = 0;
    machine[nMachines] = new VendingMachine(6, 150);
    machine[nMachines]->purchase(90);
    // nMachines++;
    return machine[nMachines];
}

// SUBMIT END HERE

int main()
{
    /*
    VendingMachine vm(5, 25);      // 25 cans to start, each will cost 5 blorks
    assert(vm.addCans(35) == 35);  // So now vm holds 60 cans
    assert(vm.purchase(47) == 2);  // 9 cans dispensed, 2 blorks change
    assert(vm.moneyRetained() == 45);  // vm retains 45 blorks so far

    cout << "All tests succeeded" << endl;
    */

    assert(createVM()->moneyRetained() == 90);
    assert(createVM()->addCans(300) == 15);
    
    cout << "All tests succeeded" << endl;
}

