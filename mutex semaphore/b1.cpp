 #include<iostream>
 #include<mutex>
 #include<thread>
 #include<chrono>
 #include<condition_variable>
 using namespace std;

class Customer{
    string customer_name;
    int chairs_needed;
public:
    Customer(string name,int chairs=1){
        customer_name=name;
        chairs_needed=chairs;
    }

    string getName(){
        return customer_name;
    }

    int getChairs(){
        return chairs_needed;
    }
};
class Semaphore {
private:
    int count;
    mutex mtx;
    condition_variable cv;

public:
    Semaphore(int count_ = 1) : count(count_) {}

    void wait(int n) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]() { return count >= n; });
        count -= n;
    }

    void signal(int n) {
        unique_lock<mutex> lock(mtx);
        count += n;
        cv.notify_all();
    }
};

class Restaurant {
    int total_chairs;
    bool use_mutex;
    mutex mtx;
    Semaphore sem;

public:
    Restaurant(int chairs, bool useMutex) : total_chairs(chairs), use_mutex(useMutex), sem(chairs) {}

    void enter(Customer c) {
        if (use_mutex) {
            lock_guard<mutex> lock(mtx);

            cout << "Customer " << c.getName() << " got exclusive access (mutex)." << endl;
            cout << c.getName() << " is using the restaurant for 1 hour." << endl;
            this_thread::sleep_for(chrono::seconds(5));
            cout << c.getName() << " has left the restaurant (mutex)." << endl;

        } else {
            cout << "Customer " << c.getName() << " is requesting " << c.getChairs() << " chair(s)." << endl;

            sem.wait(c.getChairs());

            cout << "Customer " << c.getName() << " got " << c.getChairs() << " chair(s)." << endl;
            cout << c.getName() << " is using the chairs for 1 hour." << endl;
            this_thread::sleep_for(chrono::seconds(5));
            cout << c.getName() << " has left and released " << c.getChairs() << " chair(s)." << endl;

            sem.signal(c.getChairs());
        }
    }
};

    void Create_customers(Restaurant &r, string s, int chairs) {
    Customer c(s, chairs);
    r.enter(c);
}


 int main() {
    const int total_chairs = 10;

    while (true) {
        int choice;
        cout << "Customer no\tCustomer Name\tChairs required (for semaphore)\n";
        cout << "1.\t\tABC\t\t4\n";
        cout << "2.\t\tXYZ\t\t3\n";
        cout << "3.\t\tLMN\t\t5\n";
        cout << "4.\t\tPQR\t\t2\n";
        cout << "Choose synchronization method:\n";
        cout << "1. Mutex (Exclusive access)\n";
        cout << "2. Semaphore (Chair allocation(based on requirement of chair))\n";
        cout << "3. Exit\n";
        cout << "Enter choice (1 or 2 or 3): ";
        cin >> choice;

        if (choice == 3) {
            cout << "Exiting program. Goodbye!\n";
            break; 
        }

        bool use_mutex = (choice == 1);
        Restaurant r(total_chairs, use_mutex);

        thread t1(Create_customers, ref(r), "ABC", 4);
        this_thread::sleep_for(chrono::milliseconds(500));
        thread t2(Create_customers, ref(r), "XYZ", 3);
        this_thread::sleep_for(chrono::milliseconds(500));
        thread t3(Create_customers, ref(r), "LMN", 5);
        this_thread::sleep_for(chrono::milliseconds(500));
        thread t4(Create_customers, ref(r), "PQR", 2);

        t1.join();
        t2.join();
        t3.join();
        t4.join();

        cout << "\n"; 
    }

    return 0;
}