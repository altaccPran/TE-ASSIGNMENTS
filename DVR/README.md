# Distance Vector Routing (DVR) C++ Program
This program simulates the Distance Vector Routing (DVR) algorithm, which is used to determine the shortest path between nodes in a network. The program takes a cost matrix representing the distances between each pair of nodes and calculates the routing table for each node based on the DVR algorithm. It then displays the routing tables for each node.
## Features
- Allows the user to input a network's number of nodes.
- Accepts a cost matrix where the user specifies the cost (or distance) between each pair of nodes.
- Implements the Distance Vector Routing algorithm to compute the shortest paths.
- Displays the routing table for each node, showing the next hop and the distance to each destination node.

## Output
```bash
pict@pict-OptiPlex-5000:~/31226_CNSL$ cd "/home/pict/31226_CNSL/" && g++ dvrpran.cpp -o dvrpran && "/home/pict/31226_CNSL/"dvrpran


-------------------- Distance Vector Routing Algorithm----------- 

 Enter the number of nodes : 6

 Enter the cost matrix : 
Enter value at 0 -- 0 : 0
Enter value at 0 -- 1 : 4
Enter value at 0 -- 2 : 999
Enter value at 0 -- 3 : 999
Enter value at 0 -- 4 : 2
Enter value at 0 -- 5 : 6
Enter value at 1 -- 0 : 4
Enter value at 1 -- 1 : 0
Enter value at 1 -- 2 : 999
Enter value at 1 -- 3 : 3
Enter value at 1 -- 4 : 999
Enter value at 1 -- 5 : 1
Enter value at 2 -- 0 : 999
Enter value at 2 -- 1 : 999
Enter value at 2 -- 2 : 0
Enter value at 2 -- 3 : 1
Enter value at 2 -- 4 : 999
Enter value at 2 -- 5 : 1
Enter value at 3 -- 0 : 999
Enter value at 3 -- 1 : 3
Enter value at 3 -- 2 : 1
Enter value at 3 -- 3 : 0
Enter value at 3 -- 4 : 999
Enter value at 3 -- 5 : 999
Enter value at 4 -- 0 : 2
Enter value at 4 -- 1 : 999
Enter value at 4 -- 2 : 999
Enter value at 4 -- 3 : 999
Enter value at 4 -- 4 : 0
Enter value at 4 -- 5 : 3
Enter value at 5 -- 0 : 6
Enter value at 5 -- 1 : 1
Enter value at 5 -- 2 : 1
Enter value at 5 -- 3 : 999
Enter value at 5 -- 4 : 3
Enter value at 5 -- 5 : 0


 For router: 1
 Node 1 via 1 Distance 0
 Node 2 via 2 Distance 4
 Node 3 via 2 Distance 6
 Node 4 via 2 Distance 7
 Node 5 via 5 Distance 2
 Node 6 via 2 Distance 5

 For router: 2
 Node 1 via 1 Distance 4
 Node 2 via 2 Distance 0
 Node 3 via 6 Distance 2
 Node 4 via 4 Distance 3
 Node 5 via 6 Distance 4
 Node 6 via 6 Distance 1

 For router: 3
 Node 1 via 6 Distance 6
 Node 2 via 6 Distance 2
 Node 3 via 3 Distance 0
 Node 4 via 4 Distance 1
 Node 5 via 6 Distance 4
 Node 6 via 6 Distance 1

 For router: 4
 Node 1 via 2 Distance 7
 Node 2 via 2 Distance 3
 Node 3 via 3 Distance 1
 Node 4 via 4 Distance 0
 Node 5 via 3 Distance 5
 Node 6 via 3 Distance 2

 For router: 5
 Node 1 via 1 Distance 2
 Node 2 via 6 Distance 4
 Node 3 via 6 Distance 4
 Node 4 via 6 Distance 5
 Node 5 via 5 Distance 0
 Node 6 via 6 Distance 3

 For router: 6
 Node 1 via 2 Distance 5
 Node 2 via 2 Distance 1
 Node 3 via 3 Distance 1
 Node 4 via 3 Distance 2
 Node 5 via 5 Distance 3
 Node 6 via 6 Distance 0
```
