#include <iostream>
#include "Tree.h"
#include "HashSet.h"
#include <string>
using namespace std;
struct Student {
    string FIO;
    int number;
    int grades[5];
    int scholarship;
    string email;
    Student(string FIO, int number, int gradesArray[5], int scholarship, string email)
        : FIO(FIO.substr(0, 50)), number(number), scholarship(scholarship), email(email) {
        for (int i = 0; i < 5; i++) {
            this->grades[i] = gradesArray[i];
        }
    }
};
int main()
{
    cout << "Hello World!\n";
}