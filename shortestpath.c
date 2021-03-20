//
//  main.c
//  proj3
//
//  Created by Owen Graffis on 12/2/19.
//  Copyright © 2019 Owen Graffis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>


#define MAX 9999999



/* ---------------------------------
 Function + struct Declaration
--------------------------------- */

struct vertice {
    int x_coor;
    int y_coor;
    int * weights;
};

struct node {
    int val;
    struct node * next;
};

int ** getWeights(const char * mapName);
int findDistance(int x1, int y1, int x2, int y2);
int getCount(const char * mapName);

//void dijkstra(int ** graph, int source, int dest, int vertCount);
void dijkstra(int count, int dest, int graph[count][count], int src);
int printSolution(int dist[], int parent[], int src, int dest);

int minDistance(int dist[], bool sptSet[], int vertCount);
void printPath(int parent[], int j);

/* ---------------------------------
 Driver function
 --------------------------------- */

int main(int argc, const char * argv[]) {

    if (argc != 3)
    {
        printf("Enter all files");
        return 1;
    }

    const char * mapName = argv[1];
    const char * queryName = argv[2];
    
    const char * mapName = "map5x5.txt";
//    const char * queryName = "query5x5.txt";

    FILE * queryPtr = fopen(queryName, "r");
    
    
    int src, dest, queries;
    src = 0;
    dest = 0;


    
    int loop, loop2;
    int ** graph = getWeights(mapName);
    
    int count = getCount(mapName);
    int pass_graph[count][count];
    
    
    
    for (loop = 0; loop < count; loop ++)
    {
        for (loop2 = 0; loop2 < count; loop2 ++)
        {
            pass_graph[loop][loop2] = graph[loop][loop2];
        }
    }
    
    
    if (!fscanf(queryPtr, "%d", &queries)) return 1;
    for (loop = 0; loop < queries; loop++)
    {
        fscanf(queryPtr, "%d %d", &src, &dest);
        dijkstra(count, dest, pass_graph, src);
    }
    

    return 0;
}

/* ---------------------------------
 get weights + vertices function
 --------------------------------- */

int ** getWeights(const char * mapName)
{
    FILE * mapPtr = fopen(mapName, "r");
    
    int vertCount;
    int edgeCount;
    int x, y;
    int index;
    int loop, loop2;
    int edge;
    
    if (!fscanf(mapPtr, "%d %d", &vertCount, &edgeCount)) return NULL;
    
    // initialize all vertices
    
    struct vertice * vertArr = (struct vertice *) malloc(sizeof(struct vertice) * vertCount);
    if (vertArr == NULL) {
        return NULL;
    }
    
    
    //store coordinates in every vertex index
    for (loop = 0; loop < vertCount; loop++) {
        fscanf(mapPtr, "%d %d %d", &index, &x, &y);
        vertArr[index].x_coor = x;
        vertArr[index].y_coor = y;
    }
    
    for (loop = 0; loop < vertCount; loop++)
    {
        vertArr[loop].weights = (int*) malloc(sizeof(int) * vertCount);
    }
    
    //initialize all the weights
    
    for (loop = 0; loop < vertCount; loop++)
    {
        for (loop2 = 0; loop2 < vertCount; loop2++)
        {
            vertArr[loop].weights[loop2] = 0;
        }
    }
    
    while (fscanf(mapPtr, "%d %d", &index, &edge) == 2)
    {
        int dist = findDistance(vertArr[index].x_coor, vertArr[index].y_coor, vertArr[edge].x_coor, vertArr[edge].y_coor);
        vertArr[index].weights[edge] = dist;
        vertArr[edge].weights[index] = dist;
    }
    
    //square graph
    int ** graph = (int **) malloc(sizeof(int*) * vertCount);
    for (loop = 0; loop < vertCount; loop++)
    {
        graph[loop] = (int *)malloc(sizeof(int) * vertCount);
    }
    
    for (loop = 0; loop < vertCount; loop++)
    {
        for (loop2 = 0; loop2 < vertCount; loop2++)
        {
            graph[loop][loop2] = vertArr[loop].weights[loop2];
        }
    }

    
    for (loop = 0; loop < vertCount; loop++)
        free(vertArr[loop].weights);
    free(vertArr);
    fclose(mapPtr);
    
    return graph;
}

int getCount(const char * mapName)
{
    int vertCount;
    int edgeCount;
    FILE * mapPtr = fopen(mapName, "r");
    if (!fscanf(mapPtr, "%d %d", &vertCount, &edgeCount)) return 0;
    fclose(mapPtr);
    return vertCount;

}


int findDistance(int x1, int y1, int x2, int y2)
{
    int dist = sqrt(pow((y2 - y1), 2) + pow((x2 - x1), 2));
    return dist;
}

int minDistance(int dist[], bool sptSet[], int vertCount)
{
    int min = MAX;
    int min_index = 0;

    int v;
    for (v = 0; v < vertCount; v++)
        if (sptSet[v] == false && dist[v] <= min)
        {
            min = dist[v];
            min_index = v;
        }

    return min_index;
}


 ////----------------------------- - - - - - - - - -- - - - - - - - - - - - - - - - - --  -- - - - - - - - - -- - - - - - - - -- - - - - - - - - - - - - - -- -  - - - - - - - - - - - - - - - - - - - - -

void printPath(int parent[], int j)
{
      
    if (parent[j] == -1) return;

    printPath(parent, parent[j]);
    printf("%d ", j);
}


void dijkstra(int count, int dest, int graph[count][count], int src)
{

    int dist[count];
  
    bool sptSet[count];
  
    int parent[count];
  
    for (int i = 0; i < count; i++)
    {
        parent[src] = -1;  //right here
        dist[i] = MAX;
        sptSet[i] = false;
    }
  
    dist[src] = 0;
    
    int bonerAlert;
  
    for (bonerAlert = 0; bonerAlert < count - 1; bonerAlert++)
    {

        int u = minDistance(dist, sptSet, count);
  
        sptSet[u] = true;
  
        for (int v = 0; v < count; v++)

            if (!sptSet[v] && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v])
            {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }
    }
  

    printSolution(dist, parent, src, dest);
}

int printSolution(int dist[], int parent[], int src, int dest)
{
    printf("%d\n", dist[dest]);
    printf("%d ", src);
    printPath(parent, dest);
    printf("\n");
    return 0;
}
