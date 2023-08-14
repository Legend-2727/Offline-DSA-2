#include<bits/stdc++.h>
using namespace std;

int find_parent(vector<int> &parent,int u)
{
    if(parent[u]==u) return u;
    else{
        return parent[u]=find_parent(parent,parent[u]);
    }
}

int kruskal_algo(vector<pair<int,pair<int,int>>> &tree,
                    vector<pair<int,pair<int,int>>> &edges,int n)
{
    sort(edges.begin(),edges.end());
    int total_wt=0;
    vector<int> parent(n);
    vector<int> rank(n,0);
    for(int i=0;i<n;i++)
        parent[i]=i;
    for(int i=0;i<edges.size() && tree.size()!=(n-1);i++)
    {
        int u=edges[i].second.first;
        int v=edges[i].second.second;
        int parent_u=find_parent(parent,u);
        int parent_v=find_parent(parent,v);
        
        if(parent_u!=parent_v)
        {
            if(rank[parent_u]<rank[parent_v])
            {
                parent[parent_u]=parent_v;
            }
            
            else if(rank[parent_u]>rank[parent_v]){
                parent[parent_v]=parent_u;
            }
            else
            {
                parent[parent_v]=parent_u;
                rank[parent_u]++;
            }
            tree.push_back(edges[i]);  
            total_wt+=edges[i].first;
        }
    }
    return total_wt;
}


int main()
{
    freopen("in.txt","r",stdin);
    freopen("kruskal.txt","w",stdout);
    int n,m;
    cin>>n>>m;
    vector<pair<int,pair<int,int>>> edges;
    for(int i=0;i<m;i++)
    {
        int u,v,w;
        cin>>u>>v>>w;
        edges.push_back({w,{u,v}});
    }
    vector<pair<int,pair<int,int>>> tree;
    
    cout<<"Kruskal's algorithm:\n";
    int total_weight=kruskal_algo(tree,edges,n);
    cout<<"Total weight = "<<total_weight<<"\n";
    for(int i=0;i<tree.size();i++)
    {
        cout<<tree[i].second.first<<" "<<tree[i].second.second<<"\n";
    }
}