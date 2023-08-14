#include<bits/stdc++.h>
using namespace std;

int bellman_ford(vector<pair<int,pair<int,int>>> &edges,
                    int n,int source,int dest,vector<int> &path)
{
    int e=edges.size();
    vector<int> dist(n,INT16_MAX);
    vector<int> parents(n,-1);
    dist[source]=0;
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<e;j++)
        {
            int u=edges[j].second.first;
            int v=edges[j].second.second;
            int w=edges[j].first;
            if(dist[u] !=INT16_MAX && (dist[u]+w)<dist[v])
            {
                dist[v]=dist[u]+w;
                parents[v]=u;
            }
        }
    }
    for(int j=0;j<e;j++)
    {
        int u=edges[j].second.first;
        int v=edges[j].second.second;
        int w=edges[j].first;
        if(dist[u] !=INT16_MAX && (dist[u]+w)<dist[v])
        {
            return -1;
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