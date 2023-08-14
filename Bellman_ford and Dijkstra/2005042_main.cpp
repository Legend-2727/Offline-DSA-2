#include "bellman_ford.cpp"
#include "dijkstra.cpp"

int main()
{
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    int m,n;
    cin>>n>>m;
    vector<int> adj[n];
    vector<pair<int,pair<int,int>>> edges_bf;
    vector<int> edges_dijk[n];
    for(int i=0;i<n;i++) edges_dijk[i].resize(n,INT16_MAX);
    for(int i=0;i<m;i++){
        int u,v,w;
        cin>>u>>v>>w;
        adj[u].push_back(v);
        edges_bf.push_back({w,{u,v}});
        edges_dijk[u][v]=min(abs(w),edges_dijk[u][v]);
    }
    int s,d;
    cin>>s>>d;
    vector<int> path_bf;
    int distance=bellman_ford(edges_bf,n,s,d,path_bf);
    cout<<"Bellman_Ford Algorithm :\n";
    if(path_bf.size()==0)
    {
        cout<<"Negative weight cycle present\n\n";
    }
    else{
        cout<<distance<<'\n';
        for (int i=0;i<path_bf.size();i++){
            cout<<path_bf[i];
            if((i)!=path_bf.size()-1) cout<<" -> ";
            else cout<<'\n';
        }
        cout<<'\n';
    }
    vector<int> path_dijkstra;
    int ans=dijkstra(adj,edges_dijk,n,s,d,path_dijkstra);
    cout<<"Dijkstra's algorithm: "<<'\n';
    cout<<ans<<'\n';
    for(int i=0;i<path_dijkstra.size();i++)
    {
        cout<<path_dijkstra[i];
        if(i!=path_dijkstra.size()-1) cout<<" -> ";
        else cout<<"\n";
    }
}