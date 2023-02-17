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

struct distance_table 
{
  int costs[4][4];
} dt1;

extern void read1(FILE *file)
{
  //  Initialize distance_table to 999
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      dt1.costs[i][j] = 999;
    }
  }

  char *line = NULL;
  size_t len = 0;

  // Read file
  getline(&line, &len, file);
  sscanf(line, "%d %d %d %d", &dt1.costs[0][0], &dt1.costs[1][1], &dt1.costs[2][2], &dt1.costs[3][3]);
}

extern void rtinit1() {
    //setting network topology
    dt1.costs[0][0] = dt1.costs[0][1] = 1;
    dt1.costs[1][1] = 0;
    dt1.costs[2][2] = dt1.costs[2][1] = 1; 

    //make two packets to send
    struct rtpkt pack0, pack2;
    pack0.sourceid = 1; pack0.destid = 0;
    pack2.sourceid = 1; pack2.destid = 2;
    
    for (int i=0; i<4; i++){
        pack0.mincost[i] = dt1.costs[i][1];
        pack2.mincost[i] = dt1.costs[i][1];
    }
    tolayer2(pack0); //sending packet

    for (int i=0; i<4; i++){
        pack0.mincost[i] = dt1.costs[i][1];
        pack2.mincost[i] = dt1.costs[i][1];
    }
    tolayer2(pack2); //sending packet
}

extern void rtupdate1(struct rtpkt *rcvdpkt){
    int nodeFlag = 0;
    for (int i=0; i<4; i++){
        if (dt1.costs[i][rcvdpkt->sourceid] > dt1.costs[rcvdpkt->sourceid][1] + rcvdpkt->mincost[i]){
            if(i != 1) nodeFlag = 1;
            dt1.costs[i][rcvdpkt->sourceid] = dt1.costs[rcvdpkt->sourceid][1] + rcvdpkt->mincost[i];
        }
    }

    if (nodeFlag == 1){		
        struct rtpkt pack0, pack2;
        pack0.sourceid = 1; pack0.destid = 0; pack0.mincost[1] = 0;
        pack2.sourceid = 1; pack2.destid = 2; pack2.mincost[1] = 0;

        int i=0;
        while(i<4){ //finding min cost of of the two packets
            pack0.mincost[i] = findMin(dt1.costs[i][0], dt1.costs[i][2], dt1.costs[i][3]);
            pack2.mincost[i] = findMin(dt1.costs[i][0], dt1.costs[i][2], dt1.costs[i][3]);
            i++;
        }
        //sending packets
        tolayer2(pack0);
        tolayer2(pack2);
    }
}

void printdt1(void){
  struct distance_table *dtptr = &dt1;
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}

extern void linkhandler1(int linkid, int newcost){
	if (dt1.costs[0][0] == findMin(dt1.costs[0][0], dt1.costs[0][2], dt1.costs[0][3]) 
      || newcost <= findMin(dt1.costs[0][0], dt1.costs[0][2], dt1.costs[0][3])){

    dt1.costs[1][linkid] = newcost;

		struct rtpkt packToNode0, packToNode2;
		creatertpkt(&packToNode0, 1, 0, dt1.costs[1]);
		creatertpkt(&packToNode2, 1, 2, dt1.costs[1]);
    
		tolayer2(packToNode0);
		tolayer2(packToNode2); 
	}
}

extern void print_min_cost1(){
  int min_cost[4];

  for (int i = 0; i < 4; i++)
  {
    int min = dt1.costs[i][i];
    for (int j = 0; j < 4; j++)
    {
      min = min > dt1.costs[i][j] ? dt1.costs[i][j] : min;
    }

    min_cost[i] = min;
  }

  printf("Min cost %d : %d %d %d %d\n", 1, min_cost[0], min_cost[1], min_cost[2], min_cost[3]);
}
