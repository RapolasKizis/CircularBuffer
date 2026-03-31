#include "util/all.h"
#include "circularBuffer.h"

namespace circularBufferSpace {
    class CircularBuffer::Inner {
    public:
        struct Node {
            int value;
            Node* next;
            Node(int value) {
                this->value = value;
                next = nullptr;
            }
            ~Node() {
            }
        };

        Node* oldest;
        Node* newest;
        int nodeCount;
        int capacity;

        Inner(int capacity) {
            Validator::validateRange(capacity, 1, INT_MAX);
            this->capacity = capacity;
            nodeCount = 0;
            newest = nullptr;
            oldest = nullptr;
        }
        Inner(const Inner& other) {
            capacity = other.capacity;
            nodeCount = other.nodeCount;
            newest = nullptr;
            oldest = nullptr;

            if (other.newest == nullptr)
                return;

            Node* temp = other.oldest;
            for(int i = 0; i < other.nodeCount; ++i) {
                Node* newNode = new Node(temp->value);

                if(newest == nullptr) {
                    newest = newNode;
                    oldest = newNode;
                    newNode->next = newNode;
                } else {
                    newNode->next = oldest;
                    newest->next = newNode;
                    newest = newNode;
                }

                temp = temp->next;
            }
        }
    };

    CircularBuffer::CircularBuffer() {
        impl = new Inner(8);
    }

    CircularBuffer::CircularBuffer(int capacity) {
        impl = new Inner(capacity);
    }

    CircularBuffer::CircularBuffer(const CircularBuffer& other) {
        impl = new Inner(*other.impl);
    }

    CircularBuffer& CircularBuffer::operator=(const CircularBuffer& other) {
        if(this == &other)
            return *this;

        !(*this);
        delete impl;

        impl = new Inner(*other.impl);
        return *this;
    }

    CircularBuffer::~CircularBuffer() {
        !(*this);
        delete impl;
    }

    int CircularBuffer::getCapacity() const {
        return impl->capacity;
    }
    void CircularBuffer::setCapacity(int capacity) {
        Validator::validateRange(capacity, impl->nodeCount, INT_MAX);
        impl->capacity = capacity;
    }

    std::string CircularBuffer::toString() const {
        std::stringstream ss;
        ss << impl->nodeCount << "/" << impl->capacity << " {";

        Inner::Node* temp = impl->oldest;
        for(int i = 0; i < impl->nodeCount; ++i) {
            if(i > 0)
                ss << ", ";
            ss << temp->value;

            temp = temp->next;
        }

        ss << "}";
        return ss.str();
    }

    void CircularBuffer::addValue(int value) {
        Inner::Node* newNode = new Inner::Node(value);

        if(impl->newest == nullptr) {
            impl->newest = newNode;
            impl->oldest = newNode;
            newNode->next = newNode;
            impl->nodeCount++;
        } else if(impl->nodeCount < impl->capacity) {
            newNode->next = impl->oldest;
            impl->newest->next = newNode;
            impl->newest = newNode;
            impl->nodeCount++;
        } else {
            newNode->next = impl->oldest->next;

            impl->newest->next = newNode;
            impl->newest = newNode;

            Inner::Node* temp = impl->oldest->next;
            delete impl->oldest;
            impl->oldest = temp;
        }
    }

    void CircularBuffer::removeValue(int value) {
        if(impl->nodeCount == 0)
            throw ValueNotFoundException();

        if(impl->nodeCount == 1) {
            if(impl->newest->value == value) {
                delete impl->newest;
                impl->nodeCount--;

                impl->newest = nullptr;
                impl->oldest = nullptr;
                return;
            } else
                throw ValueNotFoundException();
        }

        if(impl->nodeCount > 1) {
            Inner::Node* previous = impl->newest;
            Inner::Node* temp = impl->oldest;
            for(int i = 0; i < impl->nodeCount; ++i) {
                if(temp->value == value) {
                    previous->next = temp->next;

                    if(temp == impl->oldest)
                        impl->oldest = temp->next;

                    if(temp == impl->newest)
                        impl->newest = previous;

                    delete temp;
                    impl->nodeCount--;

                    return;
                }

                previous = temp;
                temp = temp->next;
            }

            throw ValueNotFoundException();
        }
    }

    void CircularBuffer::editValue(int value, int position) {
        Validator::validateRange(position, 0, INT_MAX);
        Inner::Node* temp = impl->oldest;
        if(position > 0) {
            for(int i = 0; i < (position%(impl->nodeCount)); ++i) {
                temp = temp->next;
            }
        }
        temp->value = value;
    }

    int CircularBuffer::positionOf(int value) const {
        Inner::Node* temp = impl->oldest;
        for(int i = 0; i < impl->nodeCount; ++i) {
            if(temp->value == value)
                return i;
            else
                temp = temp->next;
        }
        throw ValueNotFoundException();
    }

    void CircularBuffer::clearBuffer() {
        if(impl->newest != nullptr) {
            impl->newest->next = nullptr;

            while(impl->oldest != nullptr) {
                Inner::Node* temp = impl->oldest;
                impl->oldest = impl->oldest->next;
                delete temp;
            }

            impl->newest = nullptr;
            impl->oldest = nullptr;
            impl->nodeCount = 0;
        }
    }

    CircularBuffer& CircularBuffer::operator+=(int value) {
        addValue(value);
        return *this;
    }
    CircularBuffer& CircularBuffer::operator-=(int value) {
        removeValue(value);
        return *this;
    }
    CircularBuffer& CircularBuffer::operator*=(std::pair<int,int> edit) {
        editValue(edit.first, edit.second);
        return *this;
    }
    int CircularBuffer::operator[](int value) const {// returns position
        return positionOf(value);
    }

    CircularBuffer& CircularBuffer::operator!() {
        clearBuffer();
        return *this;
    }

    bool CircularBuffer::operator==(const CircularBuffer& other) const {
        return !operator!=(other);
    }
    bool CircularBuffer::operator!=(const CircularBuffer& other) const {
        if(impl->nodeCount != other.impl->nodeCount)
            return true;
        Inner::Node* temp1 = impl->oldest;
        Inner::Node* temp2 = other.impl->oldest;
        for(int i = 0; i < impl->nodeCount; ++i) {
            if(temp1->value != temp2->value)
                return true;
            temp1 = temp1->next;
            temp2 = temp2->next;
        }
        return false;
    }
    bool CircularBuffer::operator<(const CircularBuffer& other)  const {
        Inner::Node* temp1 = impl->oldest;
        Inner::Node* temp2 = other.impl->oldest;
        int sum1 = 0, sum2 = 0;
        for(int i = 0; i < impl->nodeCount; ++i) {
            sum1 += temp1->value;
            temp1 = temp1->next;
        }
        for(int i = 0; i < other.impl->nodeCount; ++i) {
            sum2 += temp2->value;
            temp2 = temp2->next;
        }
        if(sum1 < sum2)
            return true;
        return false;
    }
    bool CircularBuffer::operator<=(const CircularBuffer& other) const {
        return (operator<(other) || operator==(other));
    }
    bool CircularBuffer::operator>(const CircularBuffer& other)  const {
        Inner::Node* temp1 = impl->oldest;
        Inner::Node* temp2 = other.impl->oldest;
        int sum1 = 0, sum2 = 0;
        for(int i = 0; i < impl->nodeCount; ++i) {
            sum1 += temp1->value;
            temp1 = temp1->next;
        }
        for(int i = 0; i < other.impl->nodeCount; ++i) {
            sum2 += temp2->value;
            temp2 = temp2->next;
        }
        if(sum1 > sum2)
            return true;
        return false;
    }
    bool CircularBuffer::operator>=(const CircularBuffer& other) const {
        return (operator>(other) || operator==(other));
    }
}
