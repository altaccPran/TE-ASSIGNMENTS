#include <bits/stdc++.h>
using namespace std;

#define ll long long

vector<ll> arr;

/* -------- Display -------- */
void display(){
    if (arr.empty()){
        cout << "Array is empty." << endl;
        return;
    }
    cout << "\nArray elements: ";
    for (ll x : arr) cout << x << " ";
    cout << endl;
}

/* --------------- Selection Sort --------------- */
void selectionSort(){
    ll n = arr.size();

    for (ll i = 0; i < n-1; i++){
        ll minIdx = i;

        for (ll j = i+1; j < n; j++){
            if (arr[j] < arr[minIdx]) minIdx = j;
        }

        if (minIdx != i) swap(arr[i], arr[minIdx]);
    }

    cout << "\nArray sorted using Selection Sort!" << endl;
    display();
}

/* --------------- Input Array --------------- */
void inputArr(){
    ll n;
    cout << "Enter number of elements: ";
    cin >> n;

    arr.clear();
    arr.resize(n);

    cout << "Enter elements: ";
    for (ll i = 0; i < n; i++) {
        cin >> arr[i];
    }

    cout << "Array input successful!" << endl;
}

/* --------------- Main Menu --------------- */
int main() {
    ll choice;

    while (true){
        cout << "\n====== MENU ======" << endl;
        cout << "1. Input Array" << endl;
        cout << "2. Display Array" << endl;
        cout << "3. Selection Sort" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) inputArr();
        else if (choice == 2) display();
        else if (choice == 3){
            if (arr.empty()) cout << "Array is empty. Input array first." << endl;
            else selectionSort();
        }
        else if (choice == 4){
            cout << "Exiting program..." << endl;
            break;
        }
        else cout << "Invalid choice. Try again." << endl;
    }
    return 0;
}