#include<bits/stdc++.h>
using namespace std;

int dijkstra(vector<int> adj[],vector<int> edges[],
                int n,int source,int dest,vector<int> &path)
{
    vector<int> dist(n,INT16_MAX);
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
    dist[source] = 0;
    pq.push({0,source});
    vector<int> parents(n,-1);
    while(!pq.empty()) {
        pair<int,int> p = pq.top();
        pq.pop();
        for(auto it: adj[p.second])
        {
            if(edges[p.second][it]+dist[p.second]<dist[it])
            {
                dist[it]=edges[p.second][it]+dist[p.second];
                parents[it]=p.second;
                pq.push({dist[it],it});
            }
        }
    }
    int d=dest;
    while(d!=source)
    {
        path.push_back(d);
        d = parents[d];
    }
    path.push_back(source);
    reverse(path.begin(),path.end());
    return dist[dest];
}