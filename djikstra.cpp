#include <iostream>
#include<bits/stdc++.h>
#include <vector>
#include <queue>
#include<chrono>
using namespace std::chrono;
using namespace std;

#define INF 100001
#define NIL -1

vector<int> parent;


class node {
public:
  int key;
  int degree;
  node *parent;
  node *child;
  node *left;
  node *right;
  bool mark;
};

class fib_heap{
public:
    int Hn;
    node *Hmin;
    fib_heap(){
        Hmin=NULL;
        Hn=0;
    }
};

node * create_node(int n){
    node *new_node;
    cout<<"here12"<<endl;
    new_node->key=n;
    cout<<"here"<<endl;
    new_node->degree=0;
    cout<<"here"<<endl;
    new_node->parent=NULL;
    cout<<"here"<<endl;
    new_node->child=NULL;
    new_node->left=NULL;
    cout<<"here"<<endl;
    new_node->right=NULL;
    new_node->mark=false;
    cout<<"here"<<endl;
    return new_node;
}

void fh_insert(fib_heap f, node* Heap, node* x){
    x->degree=0;
    x->parent=nullptr;
    x->child=nullptr;
    x->left=x;
    x->right=x;
    x->mark=false;
    if(Heap==nullptr){
        Heap=x;
    }
    else{
        (Heap->left)->right=x;
        x->right=Heap;
        x->left=Heap->left;
        Heap->left=x;
        if(x->key < Heap->key)
            Heap=x;
    }
    f.Hn=f.Hn+1;
}

void fib_heap_link(node *Heap, node *y, node *x){
    (y->left)->right = y->right;
    (y->right)->left = y->left;
    node *temp;
    y->left = y;
    y->right = y;

    if(x->child==nullptr)
        x->child=y;
    else{
        temp=x->child;
        (temp->left)->right=y;
        y->right=temp;
        y->left=temp->left;
        temp->left=y;
    }
    y->parent=x;
    x->degree=x->degree+1;
    y->mark=false;
}

void Consolidate(fib_heap f, node* Heap){
    int d,D=(int)((log(f.Hn)) / (log(2)));
    node *A[D];
    for(int i=0;i<D;i++)
        A[i]=nullptr;
    node *temp;
    node *x=Heap;
    node *y;
    node *exchange;
    temp=Heap;
    x=nullptr;

    //adding each child to the root list
    while(temp!=x){
        x=temp;
        d=x->degree;
        temp=temp->right;
        while(A[d]!=nullptr){
            y=A[d];
            if(x->key > y->key){
                exchange=y;
                y=x;
                x=exchange;
            }
            fib_heap_link(Heap, y, x);
            A[d]=nullptr;
            d=d+1;
        }
        A[d]=x;
    }
    Heap=nullptr;
    for(int i=0;i<D;i++){
        if(A[i]!=nullptr){
            A[i]->left=A[i];
            A[i]->right=A[i];
            if(Heap!= nullptr){
               (Heap->left)->right=A[i];
                A[i]->right=Heap;
                A[i]->left=Heap->left;
                Heap->left=A[i];
                if (A[i]->key < Heap->key)
                  Heap=A[i];
            }
            else
                Heap = A[i];
            if (Heap==nullptr)
                Heap=A[i];
            else if (A[i]->key < Heap->key)
                Heap=A[i];
        }
    }

}

void extract_min(fib_heap f, node* Heap){
    node *z = Heap;
    node *x;
    node *temp;
    node *child_ptr;
    if(z!=nullptr){
        if(z->child!=nullptr)
            child_ptr=z->child;
        temp=nullptr;
        x=child_ptr;
        //adding each child to the root list
        while(temp!=x){
            temp=x->right;
            (Heap->left)->right=x;
            x->right=Heap;
            x->left=Heap->left;
            Heap->left=x;
            if(x->key < Heap->key)
                Heap=x;
            x->parent=nullptr;
            x=temp;
        }
        //removing z from root list
        (z->left)->right = z->right;
        (z->right)->left = z->left;
        Heap = z->right;

        if (z == z->right)
          Heap = NULL;

        else {
          Heap = z->right;
          Consolidate(f, Heap);
        }
        f.Hn = f.Hn - 1;
    }
}


void Cut(node *Heap, node *x, node *y){
    if(x==x->right)
        y->child=nullptr;
    else{
        (x->left)->right = x->right;
        (x->right)->left = x->left;
        if(x==y->child)
            y->child=x->right;
        y->degree=y->degree-1;
        (Heap->left)->right = x;
        x->right = Heap;
        x->left = Heap->left;
        Heap->left = x;
        x->parent = nullptr;
        x->mark= false;
    }
}

void Cascade_cut(node *Heap, node *y){
    node *z;
    z=y->parent;
    if(z!=nullptr){
        if(y->mark==false)
            y->mark=true;
        else{
            Cut(Heap, y, z);
            Cascade_cut(Heap,z);
        }
    }
}

void decrease_key(node *Heap, node *x, int k){
    node *y;
    if(k > x->key){
        cout << "Entered key greater than current key" << endl;
        return;
    }
    x->key=k;
    y=x->parent;
    if(y != nullptr && x->key < y->key) {
        Cut(Heap, x, y);
        Cascade_cut(Heap, y);
    }
    if(x->key < Heap->key)
        Heap=x;

}

void delete_node(fib_heap f, node *Heap, node *x){
    decrease_key(Heap, x, -100000);
    extract_min(f, Heap);
}

//priority_queue part
class Edge{
public:
    int source;
    int destination;
    int weight;
    Edge(int src, int dst, float wt){
        source=src;
        destination=dst;
        weight=wt;
    }
    void set_wt(int i){
        weight=i;
    }
};

vector<Edge> edges;

void djikstra_pq(vector<pair<int,int>> const &sorted_vertex, int vertexNum, int edgeNum, int v[], int s, int d){

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > pq;
    int u,cost=0;
    int path[vertexNum];
    vector<int> key;
    vector<int> parent;

    for(int i=0;i<vertexNum;i++){
        key.push_back(INF);
        parent.push_back(-1);
    }
    key[s]=0;

    for(int i=0;i<vertexNum;i++){
        pq.push(make_pair(key[i],i));
    }

    while(!pq.empty()){
        u=pq.top().second;
        pq.pop();
        for(int i=0;i<edgeNum;i++){
            int temp;
            if(u==sorted_vertex[i].first){
                temp=sorted_vertex[i].second;
                if(key[temp]>key[u]+v[i]){
                    parent[temp]=u;
                    key[temp]=key[u]+v[i];
                    pq.push(make_pair(key[temp],temp));
                }
            }
        }
    }

    cout<<"Shortest path cost: "<<key[d]<<endl;

    int dst=d,j=0;
    while(1){
        if(parent[dst]==s){
            path[j]=s;
            break;
        }
        path[j++]=parent[dst];
        dst=parent[dst];
    }
    for(int i=j;i>=0;i--)
        cout<<path[i]<<"->";
    cout<<d;
    cout<<endl;
}

void djikstra_fibheap(vector<pair<int,int>> const &sorted_vertex, int vertexNum, int edgeNum, int v[], int s, int d){
    fib_heap fib;
    int u,cost=0;
    int path[vertexNum];
    vector<int> key;
    vector<int> parent;

    for(int i=0;i<vertexNum;i++){
        key.push_back(INF);
        parent.push_back(-1);
    }
    key[s]=0;

    for(int i=0;i<vertexNum;i++){
//        fib.fh_insert(fib, make_pair(key[i],i));
    }

//    while(!pq.empty()){
//        u=pq.top().second;
//        pq.pop();
//        for(int i=0;i<edgeNum;i++){
//            int temp;
//            if(u==sorted_vertex[i].first){
//                temp=sorted_vertex[i].second;
//                if(key[temp]>key[u]+v[i]){
//                    parent[temp]=u;
//                    key[temp]=key[u]+v[i];
//                    pq.push(make_pair(key[temp],temp));
//                }
//            }
//        }
//    }

    cout<<"Shortest path cost: "<<key[d]<<endl;

    int dst=d,j=0;
    while(1){
        if(parent[dst]==s){
            path[j]=s;
            break;
        }
        path[j++]=parent[dst];
        dst=parent[dst];
    }
    for(int i=j;i>=0;i--)
        cout<<path[i]<<"->";
    cout<<d;
}


int main()
{
    vector<Edge> edges;
    vector<pair<int,int>> vertex;
    vector<pair<int,int>> sorted_vertex;

    int src,dst,weight,source,destin;
    int vertexNum,edgeNum;
    cin>>vertexNum>>edgeNum;
    int v[edgeNum];

    for(int i=0;i<edgeNum;i++){
        cin>>src>>dst>>weight;
        vertex.push_back( make_pair(src, dst));
        v[i]=weight;
        Edge *edge= new Edge(src, dst, weight);
        edges.push_back(*edge);
    }
    cin>>source>>destin;

    auto start = high_resolution_clock::now();
    djikstra_pq(vertex, vertexNum, edgeNum, v, source, destin);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end-start);
    cout<<endl;
    cout <<"time:" <<duration.count()<<endl;

    auto start1 = high_resolution_clock::now();
    djikstra_fibheap(vertex, vertexNum, edgeNum, v, source, destin);
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1-start);
    cout << duration1.count()<<endl;


    return 0;
}

/*
9 17
0 7 60
7 1 150
4 8 70
6 4 80
5 1 4000
8 0 100000
2 3 200
8 2 1000
0 3 300
3 8 50000
3 7 200
2 5 120
6 3 1500
4 0 90
5 7 50
1 6 100
4 1 90
0 5
*/

/*
9 17
0 7 60
7 1 -150
4 8 -70
6 4 80
5 1 4000
8 0 100000
2 3 -200
8 2 1000
0 3 300
3 8 50000
3 7 -200
2 5 120
6 3 1500
4 0 90
5 7 -50
1 6 100
4 1 -90
0 5

4 4
0 1 1
1 2 -1
2 3 -1
3 0 -1
0 3


*/
