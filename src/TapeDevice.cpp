#include "ITapeDevice.h"
#include <stdexcept>

struct Node {
    int data;
    Node* prev;
    Node* next;

    Node(int value) : data(value), prev(nullptr), next(nullptr) {}
};

class TapeDevice : public ITapeDevice {
private:
    Node* head;
    Node* current;
public:
    TapeDevice(size_t length) : head(nullptr), current(nullptr) {
        if (length == 0) {
            throw std::invalid_argument("Length of the tape must be greater than 0");
        }

        head = new Node(0);
        current = head;
        Node* prevNode = head;

        for (size_t i = 1; i < length; ++i) {
            Node* newNode = new Node(0);
            prevNode->next = newNode;
            newNode->prev = prevNode;
            prevNode = newNode;
        }
    }

    ~TapeDevice() {
        Node* node = head;
        while (node != nullptr) {
            Node* nextNode = node->next;
            delete node;
            node = nextNode;
        }
    }

    int getCurrentCell() override {
        return current->data;
    }

    void changeCurrentCell(int value) override {
        current->data = value;
    }

    void moveToNextCell() override {
        if (current->next == nullptr){
            throw std::out_of_range("End of tape reached");
        }
        current = current->next;
    }

    void moveToPreviousCell() override {
        if (current->prev == nullptr){
            throw std::out_of_range("Beginning of tape reached");
        }
        current = current->prev;
    }
};