#include<bits/stdc++.h>
using namespace std;

void floyd_warshall(vector<int> adj[],int n)
{
    for(int i=1;i<=n;i++) adj[i][i]=0;

    for(int k=1;k<=n;k++)
    {
        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=n;j++)
            {
                adj[i][j]=min(adj[i][j],adj[i][k]+adj[k][j]);
            }
        }
    }
}


int main()
{
    freopen("in.txt","r",stdin);
    freopen("floyd_warshall.txt","w",stdout);
    int n,m;
    cin>>n>>m;
    vector<int> adj[n+1];
    for(int i=0;i<=n;i++) adj[i].resize(n+1,1e7);
    for(int i=0;i<m;i++)
    {
        int u,v,w;
        cin>>u>>v>>w;
        adj[u][v]=w;
    }
    floyd_warshall(adj,n);
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