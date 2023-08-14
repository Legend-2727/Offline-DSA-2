#include<bits/stdc++.h>
using namespace std;



int prims_algo(vector<vector<int>> &adj,vector<bool> &visited,
                    vector<int> &weight,
                    vector<vector<int>> &edge_wt,int root,
                    vector<pair<int,int>> &edges)
{


    priority_queue<pair<int,pair<int,int>>,vector<pair<int,
                    pair<int,int>>>,greater<pair<int,pair<int,int>>> > pp;
    pp.push({weight[root],{root,root}});
    int total_weight=0;
    while(!pp.empty())
    {
        pair<int,int> u=pp.top().second;
        int wt=pp.top().first;
        pp.pop();
        if(visited[u.second]==true)
            continue;
        visited[u.second]=true;
        if(u.first!=u.second)
        {
            edges.push_back(u);
            total_weight+=wt;
        } 
        for(int i=0;i<adj[u.second].size();i++)
        {
            if(visited[adj[u.second][i]]==false)
            {
                if(edge_wt[u.second][adj[u.second][i]]<=weight[adj[u.second][i]])
                {
                    weight[adj[u.second][i]]=edge_wt[u.second][adj[u.second][i]];
                    pp.push({weight[adj[u.second][i]],{u.second,adj[u.second][i]}});
                }
            }
        }
    }
    return total_weight;
}
                    

int main(){
    freopen("in.txt","r",stdin);
    freopen("prim.txt","w",stdout);
    int n,m;
    cin>>n>>m;
    vector<vector<int>> adj(n);
    vector<vector<int>> edge_wt(n);
    for(int i=0;i<n;i++)
    {
        edge_wt[i].resize(n,INT_MAX);
    }

    for(int i=0;i<m;i++){
        int u,v,w;
        cin>>u>>v>>w;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edge_wt[u][v]=edge_wt[v][u]=min(edge_wt[v][u],w);
    }
    vector<bool> visited(n,false);
    vector<int> weight(n,INT_MAX);
    vector<pair<int,int>> edges;
    int root=weight[0]=0;
    
    int total_wt=prims_algo(adj,visited,weight,edge_wt,root,edges);
    cout<<"Total weight = "<<total_wt<<'\n';
    cout<<"Root node = "<<root<<'\n';
    for(int i=1;i<n;i++)
    {
        cout<<edges[i-1].first<<" "<<edges[i-1].second<<'\n';
    }
}