#include<iostream>
#include<fstream>
#include<queue>
using namespace std;

class vertex{
    int pos, color, fparent, fcost;//4=white, 5=gray, 6=black
    int* adjList;
    int* parent;
    int* cost;
    bool ifvisited=false;
public:
    int getColor(){
        return color;
    }

    void setcolor(int Color){
        color=Color;
    }

    int getPosition(){
        return pos;
    }

    void setPosition(int position){
        pos=position;
    }

    int getFparent(){
        return fparent;
    }

    int getFcost(){
        return fcost;
    }

    int getCost(int index){
        return cost[index];
    }

    void setParentCost(int index, int Parent, int Cost){
        parent[index]= Parent;
        cost[index]= Cost;
    }

    vertex(){}


    void setSize(int boardsz, int side){
        adjList= new int[side+1];
        parent= new int[boardsz+1];
        cost= new int[boardsz+1];
        for(int i=0;i<side+1;i++)
            adjList[i]=0;
        for(int i=0;i<boardsz+1;i++){
            parent[i]=99999;
            cost[i]=99999;
        }
    }

    void setAdjList(int index, int element){
        adjList[index]=element;
    }

    void setBool(bool val){
        ifvisited=val;
    }

    int adjacentElement(int index){
        return adjList[index];
    }

    bool getBool(){
        return ifvisited;
    }

    void finalpath(int Size){
        int Min=cost[1],index=1;
        for(int i=2;i<=Size;i++){
            if(cost[i]<Min){
                Min=cost[i];
                index=i;
            }
        }
        fparent=parent[index];
        fcost=cost[index];
    }

    void copyvertex(vertex& v, int boardsz, int side){
        //cout<<"here3"<<endl;
        pos=v.getPosition();
        cout<<"in copy "<<v.getPosition()<<endl;
        cout<<"here4"<<endl;
        color=v.getColor();
        cout<<"here5"<<endl;
        fparent=v.getFparent();
        //cout<<"here6"<<endl;
        fcost=v.getFcost();
        cout<<"here6"<<endl;
        int bs=boardsz;
        int sid=side;
        cout<<bs<<"  "<<sid<<endl;
        adjList= new int[side+1];
        parent= new int[boardsz+1];
        cost= new int[boardsz+1];
        cout<<"here7"<<endl;
        for(int i=1;i<=boardsz;i++){
            adjList[i]=v.adjList[i];
        }
        for(int i=1;i<=boardsz;i++){
            parent[i]=v.parent[i];
            cost[i]=v.cost[i];
        }
        ifvisited=v.getBool();
    }
};

class Graph{
    int side,boardsize;
    int* notvisited;
    int Size=0;
    int finalcost,index=0;
    int* finalParent;
public:
    vertex* allvertex;

    Graph(int N, int X){
        side=N;
        boardsize=X;
        allvertex= new vertex[boardsize+1];
        for(int i=1;i<=boardsize;i++){
            allvertex[i].setSize(boardsize, side);
            allvertex[i].setPosition(i);
        }
    }

    int* vertex;
    int* color;
    int* cost;
    int* parent;
    int fcost, fparent;
    bool* ifvisit;
    int adjlist[boardsize+1][side+1];
    //vector<int> adjlist[boardsize];
    //initialize the array

    void bfs(){
        for(int i=2;i<=boardsize;i++){
            color[i]=4;//4==white
            parent[i]=0;
            cost[i]=99999;
            ifvisit[i]=false;
        }

        color[1]=5//gray=5
        parent[1]=0;
        cost[1]=0;
        ifvisit[1]=true;
        queue<int> q;
        q.push(1);
        while(!q.empty()){
            int u=q.front();
            q.pop();
            for(int i=1;i<=side;i++){
                int temp=adjlist[u][i];
                if(color[temp]==4){
                    color[temp]=5;//gray
                    cost[temp]=cost[u]+1;
                    parent[temp]=u;
                    ifvisit[temp]=true;
                    q.push(temp);
                }
            }
            color[u]=6;//black
        }
    }

    void BFS(){
        for(int i=2;i<=boardsize;i++){
            allvertex[i].setcolor(4);
            allvertex[i].setParentCost(i, 0, 99999);
        }
        allvertex[1].setcolor(5);
        allvertex[1].setParentCost(1, 0, 0);
        queue<vertex> q;
        q.push(allvertex[1]);
        //cout<<allvertex[1].getPosition()<<"in 1"<<endl;
        cout<<"here1"<<endl;
        while(!q.empty()){
            //cout<<q.empty()<<endl;
            vertex u;
            cout<<"here2"<<endl;
            cout<<q.front().getPosition()<<endl;
            u.copyvertex(q.front(), boardsize, side);//problem here
            //cout<<"here3"<<endl;
            q.pop();
            for(int i=1;i<=side;i++){
                vertex u1;
                u1.copyvertex(allvertex[u.adjacentElement(i)], boardsize, side);
                if(u1.getColor()==4){
                    u1.setcolor(5);
                    u1.setBool(true);
                    u1.setParentCost(u.adjacentElement(i), u.getPosition(), u.getCost(i)+1);
                    q.push(u1);
                }
            }
            u.setcolor(6);
        }
        //cout<<"here2"<<endl;
    }

    void shortestPath(){
        for(int i=1;i<=boardsize;i++)
            allvertex[i].finalpath(boardsize);

        finalParent= new int[boardsize+1];
        int c=allvertex[boardsize].getFcost();
        int p=allvertex[boardsize].getFparent();
        finalParent[index++]=p;
        while(p!=1){
            p=allvertex[p].getFparent();
            finalParent[index++];
        }
    }

    void visitcount(){
        notvisited= new int[boardsize+1];
        for(int i=1;i<=boardsize;i++){
            if(!allvertex[i].getBool())
                notvisited[Size++]=allvertex[i].getPosition();
        }
    }

    void printFunction(){
        shortestPath();
        visitcount();
        if(!allvertex[boardsize].getFcost()){
            cout<<-1<<endl;
            cout<<"no solution"<<endl;
        }
        else{
            cout<<allvertex[boardsize].getFcost()<<endl;
            for(int i=index-1;i>=0;i--)
                cout<<finalParent[i]<<" -> ";
            cout<<allvertex[boardsize].getPosition()<<endl;
        }
        if(!Size)
            for(int i=0;i<Size;i++)
                cout<<notvisited[i]<<" ";
        else
            cout<<"all reachable";
    }
};


int main(){
    ifstream readFile("input.txt");
    int t,n,x,l,s;
    readFile>>t;
    while(t--){
        int* ladderstart;
        int* ladderend;
        int* snakestart;
        int* snakeend;
        readFile>>n>>x;
        Graph graph(n,x);
        readFile>>l;
        ladderstart = new int[l];
        ladderend = new int[l];
        for(int i=0;i<l;i++){
            readFile>>ladderstart[i];
            readFile>>ladderend[i];
        }
        readFile>>s;
        snakestart = new int[s];
        snakeend = new int[s];
        for(int i=0;i<s;i++){
            readFile>>snakestart[i];
            readFile>>snakeend[i];
        }
        for(int i=1,flag=0;i<=x;i++){//boardSize
            for(int k=0;k<l || k<s;k++)
                if(ladderstart[k]==i||snakestart[k]==i){
                    flag=1;
                    break;
                }
            if(flag){
                flag=0;
                continue;
            }
            int inflag=1;
            for(int j=1;j<=n && (i+j<=x);j++){//side
                for(int k=0;k<l||k<s;k++){//ladder
                    if(ladderstart[k]==i+j){
                        //cout<<"hereer"<<endl;
                        graph.allvertex[i].setAdjList(j,ladderend[k]);
                        break;
                    }
                    else if(snakestart[k]==i+j){
                        graph.allvertex[i].setAdjList(j,snakeend[k]);
                        break;
                    }
                    else{
                        //cout<<ladderstart[k]<<endl;
                        //cout<<i+j<<endl;
                        graph.allvertex[i].setAdjList(j,i+j);
                    }
                }
            }
        }
        for(int temp=2;temp<=x;temp++){
            int inflag=0;
            for(int i=1;i<temp;i++){
                for(int j=1;j<=n && (i+j<=x);j++){
                    //cout<<graph.allvertex[i].adjacentElement(j)<<" "<<temp+j<<endl;
                    if(graph.allvertex[i].adjacentElement(j)==temp){
                        inflag=1;
                        //cout<<"here12312";
                        break;
                    }
                }
            }
            //cout<<inflag;
            if(!inflag)
                for(int j=1;j<=n && (temp+j<=x);j++)
                    graph.allvertex[temp].setAdjList(j,0);
        }
//        for(int i=1;i<=x;i++){
//            for(int j=1;j<=n && (i+j<=x);j++){
//                cout<<graph.allvertex[i].adjacentElement(j)<<" ";
//            }
//            cout<<endl;
//        }
        graph.BFS();
        cout<<"here"<<endl;
        //graph.printFunction();
    }
    readFile.close();
}

















