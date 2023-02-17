#include <stdio.h>
#include <stdlib.h>

int infinity = 999; 
int nodes_num = 4; 
int table[4][4];

struct Node{ 
    int dist[4];
    int visit[4];
} nodes[4];

void print_min_cost(int id){
    printf("Min cost %d : %d %d %d %d\n", id, nodes[id].dist[0], nodes[id].dist[1], nodes[id].dist[2], nodes[id].dist[3]);
}

void dijkstra(int id){
    
    nodes[id].dist[id] = 0; //setting source
    for(int count=0 ; count<nodes_num-1 ; count++){  //algs outer loop
        int min = infinity;
        int minID;

        for (int i= 0; i<nodes_num; i++){  //algs inner loop
            if (nodes[id].visit[i] == 0 && nodes[id].dist[i] <= min){
                min = nodes[id].dist[i];
                minID = i;
            }
        }

        int u = minID;
        nodes[id].visit[u] = 1;

        for (int v=0; v<nodes_num; v++){ //algs inner loop
            if (!nodes[id].visit[v] && table[u][v] && nodes[id].dist[u] != infinity
                && nodes[id].dist[u] + table[u][v] < nodes[id].dist[v]){
                nodes[id].dist[v] = table[u][v] + nodes[id].dist[u];
            }
        }
    }
    
}

void init(){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            nodes[i].dist[j] = infinity;
            nodes[i].visit[j] = 0;
        }
    }

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            table[i][j] = infinity;
        }
    }
}


int main(void){
    init();

    FILE *file;
    char *line = NULL;
    size_t len = 0;
    file = fopen("di_config.txt", "r");

    for(int i = 0;i < 4; i++){
        getline(&line, &len, file);
        sscanf(line, "%d %d %d %d", &table[i][0], &table[i][1], &table[i][2], &table[i][3]);
    }
    

    dijkstra(0);
    dijkstra(1);
    dijkstra(2);
    dijkstra(3);

    print_min_cost(0);
    print_min_cost(1);
    print_min_cost(2);
    print_min_cost(3);

    fclose(file);

    return 0;
}
