/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

template <class T>
Deque<T>::Deque(){
    n1 = 0; // initialize n1 to 0
    n2 = 0; // initialize n2 to 0
    // The vector 'data' is initialized automatically.
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem) {
    if (n1 > 0 || n2 < data.size()) {
        if (n2 == data.size()) {
            data.resize(data.size() + 1);
        }
        data[n2] = newItem;
    } else {
        data.push_back(newItem);
    }
    ++n2; // Increment the 'right' index because we've added an element
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes.
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    T leftmostValue = data[n1]; // Save the value at the left of the deque.
    ++n1; // Increment n1 to effectively remove the element from the deque.

    int numElements = n2 - n1; // Determine the number of elements currently in the deque.

    if (n1>=0 && numElements <= n1) {
        std::vector<T> newData(numElements);
        std::copy(data.begin() + n1, data.begin() + n2, newData.begin());
        data.swap(newData); // Swap the vectors to avoid copying.
        n2 = numElements;
        n1 = 0;
    }

    return leftmostValue; // Return the saved value.
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR() {
    T rightmostValue = data[--n2]; // Save the value at the right of the deque.

    int numElements = n2 - n1; // Calculate the number of elements in the deque.
    // If there's more unused space at the beginning than elements, resize.
    if (n1>=0 && numElements <= n1) {
        std::vector<T> newData(numElements);
        std::copy(data.begin() + n1, data.begin() + n2, newData.begin());
        data.swap(newData); // Efficiently swap over to the new data vector.
        // After the swap, the indices must be updated accordingly.
        n2 = numElements;
        n1 = 0;
    }
    return rightmostValue; // Return the value that was at the right.
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    return data[n1]; // Return the value at the front of the deque.
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    return data[n2 - 1]; // Return the value at the back of the deque.
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    return n1 == n2; // Check if n1 and n2 are equal.
}
