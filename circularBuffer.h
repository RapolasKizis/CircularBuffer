// AI assistance: Claude (Anthropic) was used to help develop and debug
// portions of this implementation, including operator implementations,
// test cases, and the makefile.

#ifndef CIRCULAR_BUFFER_H_03302026
#define CIRCULAR_BUFFER_H_03302026

#include "util/all.h"

namespace circularBufferSpace {
    class ValueNotFoundException : public std::runtime_error {
    public:
        ValueNotFoundException()
            : std::runtime_error("Value not found") {}
    };

    class CircularBuffer {
    private:
        class Inner;
        Inner* impl;
    public:
        CircularBuffer();                                           //default constructor. Sets the capacity to 8
        CircularBuffer(int capacity);                               //regular constructor
        CircularBuffer(const CircularBuffer& other);                //copy constructor
        CircularBuffer& operator=(const CircularBuffer& other);     //overloaded assignment operator. Allows for object copying through assignment operator
        ~CircularBuffer();                                          //destructor

        int getCapacity() const;                                    //get current capacity of the circular buffer
        void setCapacity(int capacity);                             //set a new capacity of the circular buffer
        std::string toString() const;                               //returns a string of an object: nodeCount/capacity {element1, element2,...}

        void addValue(int value);                                   //add a value to the circular buffer
        void removeValue(int value);                                //remove selected value from the buffer. This will throw ValueNotFoundException() if the value cannot be found
        void editValue(int newValue, int position);                 //set a new value at a desired position in the buffer. Note that the first element is considered the current oldest one.
        int positionOf(int value) const;                            //returns the position of the first matching value in the buffer

        void clearBuffer();                                         //clears all buffer elements

        CircularBuffer& operator+=(int value);                      //addValue() through an operator
        CircularBuffer& operator-=(int value);                      //removeValue() through an operator
        CircularBuffer& operator*=(std::pair<int,int> edit);        //editValue() through an operator
        int operator[](int value) const;                            //positionOf() through an operator

        CircularBuffer& operator!();                                //clearBuffer() through an operator

        bool operator==(const CircularBuffer& other) const;         //returns true if the buffers are equal
        bool operator!=(const CircularBuffer& other) const;         //returns true if the buffers are not equal
        bool operator<(const CircularBuffer& other)  const;         //returns true if the first buffer has a lower total sum of elements than the other
        bool operator<=(const CircularBuffer& other) const;         //returns true if the first buffer has a lower total sum of elements than the other or is equal to it
        bool operator>(const CircularBuffer& other)  const;         //returns true if the first buffer has a bigger total sum of elements than the other
        bool operator>=(const CircularBuffer& other) const;         //returns true if the first buffer has a bigger total sum of elements than the other or is equal to it
    };
}

#endif // CIRCULAR_BUFFER_H_03302026
