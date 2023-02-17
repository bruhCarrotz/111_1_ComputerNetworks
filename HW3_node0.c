#include <stdio.h>

struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;
extern void tolayer2(struct rtpkt packet);

struct distance_table {
  int costs[4][4] ;
} dt0;

extern void read0(FILE *file){
  //  Initialize distance_table to 999
  for (int i = 0; i < 4; i++){
    for (int j = 0; j < 4; j++){
      dt0.costs[i][j] = 999;
    }
  }

  char *line = NULL;
  size_t len = 0;

  // Read file
  getline(&line, &len, file);
  sscanf(line, "%d %d %d %d", &dt0.costs[0][0], &dt0.costs[1][1], &dt0.costs[2][2], &dt0.costs[3][3]);
}

int findMin(int x, int y, int z){
  int minimum;
  if((x<=y) && (x<=z)) minimum = x;
  else if (y<=x && y<=z) minimum = y;
  else minimum = z;

  return minimum;
} 

extern void rtinit0() {
  //setting network topology
	dt0.costs[0][0] = 0;
	dt0.costs[1][1] = dt0.costs[1][0] = 1;
	dt0.costs[2][2] = dt0.costs[2][0] = 3;
	dt0.costs[3][3] = dt0.costs[3][0] = 7;

  //make three packets to send
	struct rtpkt pack1, pack2, pack3;
	pack1.sourceid = 0; pack1.destid = 1;
	pack2.sourceid = 0; pack2.destid = 2;
	pack3.sourceid = 0; pack3.destid = 3;

	for (int i=0; i<4; i++){
		pack1.mincost[i] = dt0.costs[i][0];
	}
	tolayer2(pack1); //sending packets

	for (int i=0; i<4; i++){
		pack2.mincost[i] = dt0.costs[i][0];
	}
	tolayer2(pack2); //sending packets

	for (int i=0; i<4; i++){
		pack3.mincost[i] = dt0.costs[i][0];
	}
	tolayer2(pack3); //sending packets


}

extern void rtupdate0(struct rtpkt *rcvdpkt){
	int nodeFlag = 0;
	for (int i=0; i<4; i++){
		if (dt0.costs[i][rcvdpkt->sourceid] > dt0.costs[rcvdpkt->sourceid][0] + rcvdpkt->mincost[i]){
			if(i != 0) nodeFlag = 1; 
			dt0.costs[i][rcvdpkt->sourceid] = dt0.costs[rcvdpkt->sourceid][0] + rcvdpkt->mincost[i];
		}
	}

	if (nodeFlag == 1){		
		struct rtpkt pack1, pack2, pack3;
		pack1.sourceid = 0; pack1.destid = 1; pack1.mincost[0] = 0;
		pack2.sourceid = 0; pack2.destid = 2; pack2.mincost[0] = 0;
		pack3.sourceid = 0; pack3.destid = 3; pack3.mincost[0] = 0;

		int i=1;
		while(i<4){ //finding min cost of of the three packets
			pack1.mincost[i] = findMin(dt0.costs[i][1], dt0.costs[i][2], dt0.costs[i][3]);
			pack2.mincost[i] = findMin(dt0.costs[i][1], dt0.costs[i][2], dt0.costs[i][3]);
			pack3.mincost[i] = findMin(dt0.costs[i][1], dt0.costs[i][2], dt0.costs[i][3]);
			i++;
		}

    //sending packets
		tolayer2(pack1);
		tolayer2(pack2);
		tolayer2(pack3);

	}
}


void printdt0(void) {
  struct distance_table *dtptr = &dt0;
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n", dtptr->costs[1][1],
           dtptr->costs[1][2], dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n", dtptr->costs[2][1],
           dtptr->costs[2][2], dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n", dtptr->costs[3][1],
           dtptr->costs[3][2], dtptr->costs[3][3]);
}

extern void linkhandler0(int linkid, int newcost){
    if (dt0.costs[1][1] == findMin(dt0.costs[1][1], dt0.costs[1][2], dt0.costs[1][3])|| 
    newcost <= findMin(dt0.costs[1][1], dt0.costs[1][2], dt0.costs[1][3])){

        dt0.costs[0][linkid] = newcost;

        struct rtpkt packToNode1, packToNode2, packToNode3;
        creatertpkt(&packToNode1, 0, 1, dt0.costs[0]);
        creatertpkt(&packToNode2, 0, 2, dt0.costs[0]);
        creatertpkt(&packToNode3, 0, 3, dt0.costs[0]);

        tolayer2(packToNode1);
        tolayer2(packToNode2);
        tolayer2(packToNode3);
    }

}

extern void print_min_cost0(){
  int min_cost[4];

  for(int i = 0;i < 4;i++)
  {
    int min = dt0.costs[i][i];
    for(int j = 0;j < 4; j++)
    {
      min = min > dt0.costs[i][j] ? dt0.costs[i][j] : min;
    }

    min_cost[i] = min;
  }

  printf("Min cost %d : %d %d %d %d\n", 0, min_cost[0], min_cost[1], min_cost[2], min_cost[3]);
}
