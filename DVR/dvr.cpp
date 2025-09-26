#include <iostream>
using namespace std;

struct node
{
    unsigned dist[6];
    unsigned from[6];
} DVR[10];

int main()
{
    cout << "\n\n-------------------- Distance Vector Routing Algorithm----------- ";
    int costmat[6][6];
    int nodes, i, j, k, count;

    cout << "\n\n Enter the number of nodes : ";
    cin >> nodes; // Enter the number of nodes

    cout << "\n Enter the cost matrix : \n";
    for (i = 0; i < nodes; i++)
    {
        for (j = 0; j < nodes; j++)
        {
            cout << "Enter value at " << i << " -- " << j << " : ";
            cin >> costmat[i][j];
        }
        costmat[i][i] = 0; 
    }

    
    for (i = 0; i < nodes; i++)
    {
        for (j = 0; j < nodes; j++)
        {
            DVR[i].dist[j] = costmat[i][j];
            DVR[i].from[j] = j;
        }
    }

    
    for (count = 0; count < nodes - 1; count++) 
    {
        for (i = 0; i < nodes; i++)
        {
            for (j = 0; j < nodes; j++)
            {
                for (k = 0; k < nodes; k++)
                {
                    if (DVR[i].dist[j] > costmat[i][k] + DVR[k].dist[j])
                    {
                        DVR[i].dist[j] = costmat[i][k] + DVR[k].dist[j];
                        DVR[i].from[j] = k;
                    }
                }
            }
        }
    }

   
    for (i = 0; i < nodes; i++)
    {
        cout << "\n\n For router: " << i + 1;
        for (j = 0; j < nodes; j++)
        {
            cout << "\n Node " << j + 1 << " via " << DVR[i].from[j] + 1 << " Distance " << DVR[i].dist[j];
        }
    }

    cout << "\n\n";
    return 0;
}
