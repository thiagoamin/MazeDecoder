#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>
#include "stack.h"

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // Seed for random number generator

    Stack<int> myStack;

    myStack.push(1);
    myStack.pop();
    myStack.push(2);
    myStack.push(3);
    myStack.push(4);
    myStack.push(5);
    myStack.pop();



}
