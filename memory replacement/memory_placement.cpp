#include <iostream>
#include <vector>
#include <limits>
using namespace std;

// First Fit Algorithm
void first_fit(int input_blocks[], int input_processes[], int m, int n)
{
    vector<int> blocks(m);
    for (int i = 0; i < m; i++)
        blocks[i] = input_blocks[i];
    
    vector<int> processes(n);
    for (int i = 0; i < n; i++)
        processes[i] = input_processes[i];
    
    vector<int> allocating(n, -1);
    
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            if (processes[j] <= blocks[i]) {
                blocks[i] = blocks[i] - processes[j];
                allocating[j] = i;
                break;
            }
        }
    }
    
    cout << "\n\t\t\t Process No.\tProcess Size\tBlock no." << endl;
    for (int i = 0; i < n; i++)
    {
        cout << "\t\t\t " << (i+1) << "\t\t" << processes[i] << "\t\t";
        if (allocating[i] != -1)
            cout << allocating[i] + 1;
        else
            cout << "Not Allocated";
        cout << endl;
    }
}

// Next Fit Algorithm
void next_fit(int input_blocks[], int input_processes[], int m, int n)
{
    vector<int> blocks(m);
    for (int i = 0; i < m; i++)
        blocks[i] = input_blocks[i];
    
    vector<int> processes(n);
    for (int i = 0; i < n; i++)
        processes[i] = input_processes[i];
    
    vector<int> allocating(n, -1);
    
    int flag = 0;
    for (int j = 0; j < n; j++) {
        int i = 0;
        if (flag == m - 1)
            i = 0;
        
        while (i < m) {
            if (processes[j] <= blocks[i]) {
                blocks[i] = blocks[i] - processes[j];
                allocating[j] = i;
                flag = i;
                break;
            }
            i++;
        }
    }
    
    cout << "\n\t\t\t Process No.\tProcess Size\tBlock no." << endl;
    for (int i = 0; i < n; i++)
    {
        cout << "\t\t\t " << (i+1) << "\t\t" << processes[i] << "\t\t";
        if (allocating[i] != -1)
            cout << allocating[i] + 1;
        else
            cout << "Not Allocated";
        cout << endl;
    }
}

// Best Fit Algorithm
void best_fit(int input_blocks[], int input_processes[], int m, int n)
{
    vector<int> blocks(m);
    for (int i = 0; i < m; i++)
        blocks[i] = input_blocks[i];
    
    vector<int> processes(n);
    for (int i = 0; i < n; i++)
        processes[i] = input_processes[i];
    
    vector<int> allocating(n, -1);
    
    for (int j = 0; j < n; j++) {
        int Best_block = -1;
        
        for (int i = 0; i < m; i++) {
            if (blocks[i] >= processes[j]) {
                if (Best_block == -1)
                    Best_block = i;
                else if (blocks[i] < blocks[Best_block])
                    Best_block = i;
            }
        }
        
        if (Best_block != -1) {
            allocating[j] = Best_block;
            blocks[Best_block] = blocks[Best_block] - processes[j];
        }
    }
    
    cout << "\n\t\t\t Process No.\tProcess Size\tBlock no." << endl;
    for (int i = 0; i < n; i++)
    {
        cout << "\t\t\t " << (i+1) << "\t\t" << processes[i] << "\t\t";
        if (allocating[i] != -1)
            cout << allocating[i] + 1;
        else
            cout << "Not Allocated";
        cout << endl;
    }
}

// Worst Fit Algorithm
void worst_fit(int input_blocks[], int input_processes[], int m, int n)
{
    vector<int> blocks(m);
    for (int i = 0; i < m; i++)
        blocks[i] = input_blocks[i];
    
    vector<int> processes(n);
    for (int i = 0; i < n; i++)
        processes[i] = input_processes[i];
    
    vector<int> allocating(n, -1);
    
    for (int j = 0; j < n; j++) {
        int Max_block = -1;
        
        for (int i = 0; i < m; i++) {
            if (blocks[i] >= processes[j]) {
                if (Max_block == -1)
                    Max_block = i;
                else if (blocks[i] > blocks[Max_block])
                    Max_block = i;
            }
        }
        
        if (Max_block != -1) {
            allocating[j] = Max_block;
            blocks[Max_block] -= processes[j];
        }
    }
    
    cout << "\n\t\t\t Process No.\tProcess Size\tBlock no." << endl;
    for (int i = 0; i < n; i++)
    {
        cout << "\t\t\t " << (i+1) << "\t\t" << processes[i] << "\t\t";
        if (allocating[i] != -1)
            cout << allocating[i] + 1;
        else
            cout << "Not Allocated";
        cout << endl;
    }
}

// Main Function
int main()
{
    int nh, np;
    bool flag = true;
    
    cout << "\n\n\t\t\t === MEMORY PLACEMENT STRATEGIES ===" << endl;
    cout << "\n\n\t\t Enter the number of Holes : ";
    cin >> nh;
    cout << "\n\t\t Enter the number of Processes : ";
    cin >> np;
    
    int blocks[nh];
    int processes[np];
    
    cout << "\n\t\t === Enter the sizes of blocks ... " << endl;
    for(int i = 0; i < nh; i++)
    {
        cout << "\n\t\t\t Enter the size of block no. " << (i+1) << " = ";
        cin >> blocks[i];
    }
    
    cout << "\n\t\t === Enter the sizes of Processes ... " << endl;
    for(int i = 0; i < np; i++)
    {
        cout << "\n\t\t\t Enter the size of Process no. " << (i+1) << " = ";
        cin >> processes[i];
    }
    
    while(flag)
    {
        cout << "\n ==== Main-Menu ==== \n\t 1. First Fit \n\t 2. Next Fit"
             << "\n\t 3. Best Fit \n\t 4. Worst Fit \n\t 5. EXIT..." << endl;
        
        cout << "\n\t Enter choice : ";
        int choice;
        cin >> choice;
        
        switch(choice)
        {
            case 1:
                cout << "\n\t\t\t\t 1.] First Fit " << endl;
                first_fit(blocks, processes, nh, np);
                cout << "\n\t\t\t ==========================================" << endl;
                break;
            
            case 2:
                cout << "\n\t\t\t\t 2.] Next Fit " << endl;
                next_fit(blocks, processes, nh, np);
                cout << "\n\t\t\t ==========================================" << endl;
                break;
            
            case 3:
                cout << "\n\t\t\t\t 3.] Best Fit " << endl;
                best_fit(blocks, processes, nh, np);
                cout << "\n\t\t\t ==========================================" << endl;
                break;
            
            case 4:
                cout << "\n\t\t\t\t 4.] Worst Fit " << endl;
                worst_fit(blocks, processes, nh, np);
                cout << "\n\t\t\t ==========================================" << endl;
                break;
            
            case 5:
                flag = false;
                cout << "\n\t\t\t\t Thank You ...!!" << endl;
                cout << "\n\n\t\t\t\t *****=====*****" << endl;
                break;
            
            default:
                cout << "\n\t\t Invalid Choice ...!!" << endl;
        }
    }
    
    return 0;
}
