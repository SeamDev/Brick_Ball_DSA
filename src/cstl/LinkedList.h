#pragma once

template <typename T>
struct Node {
    T data;
    Node* next;
    Node(const T& d, Node* next = nullptr) : data(d), next(next) {}
};

template <typename T>
class LinkedList {
    Node<T>* head = nullptr;
    int count = 0;

public:
    LinkedList() = default;
    ~LinkedList() { clear(); }

    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    // insert at head O(1)
    void insert_front(const T& val) {
        head = new Node<T>(val, head);
        ++count;
    }

    // delete by id , O(n) search + O(1) unlink , handles empty & not-found
    bool delete_by_id(int id) {
        if (!head) return false;

        if (head->data.id == id) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            --count;
            return true;
        }

        Node<T>* curr = head;
        while (curr->next && curr->next->data.id != id) {
            curr = curr->next;
        }
        if (!curr->next) return false;

        Node<T>* temp = curr->next;
        curr->next = temp->next;
        delete temp;
        --count;
        return true;
    }

    // search by id O(n)
    Node<T>* search(int id) const {
        Node<T>* curr = head;
        while (curr) {
            if (curr->data.id == id) return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    // update by id O(n)
    bool update(int id, const T& newData) {
        Node<T>* node = search(id);
        if (!node) return false;
        node->data = newData;
        return true;
    }

    // get head ptr O(1)
    Node<T>* get_head() const { return head; }
    int size() const { return count; }
    bool empty() const { return head == nullptr; }

    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        count = 0;
    }
};