#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>
using namespace std;

template <typename T>
class MyLinkedQueue{
  private:
    struct Node{
      Node* next;
      T data;
    };
    int count;
    Node* head;
    Node* tail;
  public:
    MyLinkedQueue(){
      count = 0;
      head = nullptr;
      tail = nullptr;
    }
    int size(){
      return count;
    };
    bool empty(){
      return count == 0;
    };
    T front(){
      if(empty()){
        cout << "queue is full" << endl;
        exit(1);
      }
      return head->data;
    };
    void enqueue(T e){
      Node* temp = new Node();
      temp->data = e;
      temp->next = nullptr;
      if(empty()){
        head = temp;
        tail = temp;
        count++;
      } else {
        tail->next = temp;
        tail = temp;
        count++;
      }
    };
    T dequeue(){
      if(empty()){
        cout << "queue is full" << endl;
        exit(1);
      }
      Node* temp = head;
      T result = temp->data;
      head = head->next;
      delete temp;
      count--;
      if(head == nullptr){
        tail = nullptr;
      }
      return result;
    };
};

struct Job{
  int jobNum;
  int remainingUnits;
};

void runQueueSimulation(queue<Job>& q, const string& label) {
    cout << "\n" << label << "\n" << endl;

    int cycle = 1;
    while (!q.empty()) {
        Job current = q.front();
        q.pop();

        cout << cycle++ << " Processing job " << current.jobNum << endl;
        current.remainingUnits--;

        if (current.remainingUnits > 0) {
            q.push(current);
        } else {
            cout << "Done with job " << current.jobNum << endl;
        }
    }
    cout << "Done with all jobs\n";
}

void runCustomQueueSimulation(MyLinkedQueue<Job>& q, const string& label) {
    cout << "\n" << label << "\n" << endl;

    int cycle = 1;
    while (!q.empty()) {
        Job current = q.dequeue();

        cout << cycle++ << " Processing job " << current.jobNum << endl;
        current.remainingUnits--;

        if (current.remainingUnits > 0) {
            q.enqueue(current);
        } else {
            cout << "Done with job " << current.jobNum << endl;
        }
    }
    cout << "Done with all jobs\n";
}

int main() {
    // ---------- Built-in queue with hardcoded jobs ----------
    queue<Job> stdQueue;
    cout << "Using queue from the language." << endl;

    int testUnits[] = {2, 1, 2, 1, 1};
    for (int i = 0; i < 5; ++i) {
        Job j = {i + 1, testUnits[i]};
        stdQueue.push(j);
        cout << "Enqueue job " << j.jobNum << " with " << j.remainingUnits << " units" << endl;
    }

    runQueueSimulation(stdQueue, "Simulation using built-in queue:");

    // ---------- MyLinkedQueue with random jobs ----------
    MyLinkedQueue<Job> myQueue;
    srand(time(0)); // Seed random

    cout << "\nUsing MyLinkedQueue with random jobs." << endl;

    for (int i = 0; i < 5; ++i) {
        int units = rand() % 6 + 1; // between 1 and 6
        Job j = {i + 1, units};
        myQueue.enqueue(j);
        cout << "Enqueue job " << j.jobNum << " with " << j.remainingUnits << " units" << endl;
    }

    runCustomQueueSimulation(myQueue, "Simulation using MyLinkedQueue:");

    return 0;
}