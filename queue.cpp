/**
 * @file queue.cpp
 * Implementation of the Queue class.
 *
 */

/**
 * Adds the parameter object to the back of the Queue.
 *
 * @param newItem object to be added to the Queue.
 */
template <class T>
void Queue<T>::enqueue(T newItem)
{
    myQueue.pushR(newItem); //push to the right of Deque (insert at the back of Queue)
}

/**
 * Removes the object at the front of the Queue, and returns it to the
 * caller.
 *
 * @return The item that used to be at the front of the Queue.
 */
template <class T>
T Queue<T>::dequeue()
{
    return myQueue.popL(); //pop from the left of Deque (remove from the front of Queue)
}


/**
 * Finds the object at the front of the Queue, and returns it to the
 * caller. Unlike dequeue(), this operation does not alter the queue.
 *
 * @return The item at the front of the queue.
 */
template <class T>
T Queue<T>::peek()
{
    return myQueue.peekL(); //peek from the left of Deque (look at the front of Queue)
}

/**
 * Determines if the Queue is empty.
 *
 * @return bool which is true if the Queue is empty, false otherwise.
 */
template <class T>
bool Queue<T>::isEmpty() const
{
    return myQueue.isEmpty(); //check if Deque is empty
}
