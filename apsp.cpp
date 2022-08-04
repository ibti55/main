#include <iostream>
#include<bits/stdc++.h>
#include <vector>
#include <queue>
using namespace std;

#define INF 100001
int l[15][15],w[15][15];

void extend(int n){
    int temp[n+1][n+1];
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++){
            temp[i][j]=INF;
            for(int k=1;k<=n;k++)
                temp[i][j]=min(temp[i][j], l[i][k]+w[k][j]);
        }

    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            l[i][j]=temp[i][j];
}

void matrix_multiplication(int n){
    for(int i=2;i<n;i++)
        extend(n);

    cout<<"Shortest distance matrix"<<endl;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++)
            cout<<l[i][j]<<" ";
        cout<<endl;
    }
}

void floyd_warshall(int l[][15], int n){
    for(int k=1;k<=n;k++)
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                l[i][j]=min(l[i][j], l[i][k]+l[k][j]);

    cout<<"Shortest distance matrix"<<endl;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++)
            cout<<l[i][j]<<" ";
        cout<<endl;
    }
}

int main(){
    int src,dst,weight,n,edgeNum;

    cin>>n>>edgeNum;

    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++){
            if(i==j){
                l[i][j]=0;
                w[i][j]=0;
            }
            else{
                l[i][j]=INF;
                w[i][j]=INF;
            }
        }

    for(int i=0;i<edgeNum;i++){
        cin>>src>>dst>>weight;
        w[src][dst]=weight;
        l[src][dst]=weight;
    }
    cout<<endl;

    //matrix_multiplication(n);
    floyd_warshall(l,n);

}

/*
4 6
1 2 8
1 4 1
2 3 1
3 1 4
4 2 2
4 3 9

4 4
1 2 5
2 3 3
3 4 1
1 4 10
*/
