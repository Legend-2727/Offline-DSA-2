#include<bits/stdc++.h>
using namespace std;

vector<vector<int>> matrix_multiplication(vector<vector<int>> &ans,int n)
{
    vector<vector<int>> temp(n+1,vector<int> (n+1));
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        {
            temp[i][j]=1e7;
            for(int k=1;k<=n;k++)
            {
                temp[i][j]=min(temp[i][j],ans[i][k]+ans[k][j]);
            }
        }
    }
    return temp;
}


int main()
{
    freopen("in.txt","r",stdin);
    freopen("Matrix_multiplication.txt","w",stdout);
    int n,m;
    cin>>n>>m;
    vector<vector<int>> adj(n+1);
    for(int i=0;i<=n;i++) adj[i].resize(n+1,1e7);
    for(int i=0;i<m;i++)
    {
        int u,v,w;
        cin>>u>>v>>w;
        adj[u][v]=w;
    }
    for(int i=1;i<=n;i++) adj[i][i]=0;
    int times=1;
    while(times<=n){
        adj=matrix_multiplication(adj,n);
        times*=2;
    }
    cout<<"Shortest distance matrix :\n";    
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        {
            if(adj[i][j]<1e7) cout<<adj[i][j]<<" \n"[j==n];
            else cout<<"INF"<<" \n"[j==n];
        }
    }
}