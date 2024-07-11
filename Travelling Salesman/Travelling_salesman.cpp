#include <iostream>    // Input/output stream
#include <iomanip>     // Input/output manipulators
#include <cstdio>      // C Standard Input/Output functions
#include <cstdlib>     // General utilities library
#include <cmath>       // Mathematical functions
#include <string>      // String handling
#include <vector>      // Dynamic arrays (vectors)
#include <deque>       // Double-ended queues
#include <queue>       // Queues
#include <stack>       // Stacks
#include <set>         // Sets
#include <map>         // Maps
#include <unordered_set>   // Unordered sets
#include <unordered_map>   // Unordered maps
#include <algorithm>   // Algorithms
#include <utility>     // Utility components
#include <bitset>      // Bitsets
#include <limits>      // Numeric limits
#include <functional>  // Function objects
//#include "..///dbg.h"
using namespace std;

vector<vector<int>> dp(21,vector<int> (1<<21));

class Graph{
    public:
        vector<vector<int>> edges;
        vector<vector<int>> adj;
        int size;
        Graph(vector<vector<int>> &edge,vector<vector<int>> &adjacent)
        {
            edges=edge;
            adj=adjacent;
            size=edge.size();
        }
        void print_graph()
        {
            for(int i=1;i<=size;i++){
                for(int j=1;j<=size;j++)
                {
                    cout<<edges[i][j]<<" \n"[j==size];
                }
            }
        }
};

void generate_graph(vector<vector<int>> &edges,int size){
    int min_wt=50;
    int max_wt=100;
    for(int i=1;i<=size;i++) edges[i][i]=0;
    for(int i=1;i<=size;i++){
        for(int j=1;j<=size;j++)
        {
            if(j==i) continue;
            int temp_min=min_wt;
            int temp_max=max_wt;
            for(int k=1;k<=size;k++){
                if(k==i || k==j) continue;
                if(edges[i][k]!=-1 && edges[k][j]!=-1){
                    temp_max=min(edges[i][k]+edges[k][j],temp_max);
                    temp_min=max(abs(edges[i][k]-edges[k][j]),temp_min);
                }
            }
            edges[i][j]=edges[j][i]=temp_min+rand()%(temp_max-temp_min+1);
        }
    }
}


long long Exact_Tsp(vector<vector<int>> &edges,int size,
                    int start,int mask){
    
    if(dp[start][mask]!=0) return dp[start][mask];
    if(mask==(1<<start|3)) return edges[start][1];
    
    long long temp_cost=INT_MAX;
    for(int i=2;i<=size;i++){
        if(i==start) continue;
        //dbg(start,i);
        if((mask&(1<<i))!=0)
        {
            temp_cost=min(temp_cost,
                Exact_Tsp(edges,size,i,(mask^(1<<start)))+1l*edges[start][i]);
            //dbg(temp_cost);
        }
    }
    return dp[start][mask]=temp_cost;
}


void prims_algo(int root,int size,
                    vector<vector<int>> &edges,
                    map<int,vector<int>> &path)
{
    priority_queue<pair<int,pair<int,int>>,vector<pair<int,
                    pair<int,int>>>,greater<pair<int,pair<int,int>>> > pp;
    // int first_min=INT_MAX,min_ind=0;
    // for(int i=2;i<=size;i++)
    // {
    //     if(edges[root][i]<first_min)
    //     {
    //         first_min=edges[root][i];
    //         min_ind=i;
    //     }
    // }
    pp.push({0,{root,root}});
    vector<int> visited(size+1,0);

    //int total_weight=0;
    while(!pp.empty())
    {
        pair<int,int> u=pp.top().second;
        //int wt=pp.top().first;
        pp.pop();
        if(visited[u.second]==true)
            continue;
        visited[u.second]=true;
        if(u.first!=u.second)
        {
            path[u.first].push_back(u.second);
        } 
        
        //dbg(u,pp);
        for(int i=1;i<=size;i++)
        {
            if(visited[i]==0 && i!=u.second)
            {
                pp.push({edges[u.second][i],{u.second,i}});
            }
        }
    }
    //return total_weight;
}

void dfs(map<int,vector<int>> &MST,vector<int> &visited,vector<int> &newpath,int root){
    newpath.push_back(root);
    visited[root]=1;
    for (auto x : MST[root])
        if (!visited[x]){
            dfs(MST,visited,newpath,x);
        }
}


int main()
{
    int size=20;
    vector<vector<int>> adj;
    vector<int> single_vect;
    for(int i=1;i<=size;i++){
        single_vect.push_back(i);
    }
    int t;
    cout<<"No. of testcases: ";
    cin>>t;
    cout<<endl;
    adj.resize(size+1,single_vect);
    int test=1;
    while(t--){
        for(int i=1;i<=20;i++)
        {
            for(int j=0;j<=(1<<size);j++)
            {
                dp[i][j]=0;
            }
        }
        vector<vector<int>> edges(size+1,vector<int> (size+1,-1));

        generate_graph(edges,size);
        // for(int i=1;i<=size;i++)
        // {
        //     for(int j=1;j<=size;j++)
        //     {
        //         cin>>edges[i][j];
        //     }
        // }
        Graph graph(edges,adj);

        
        long long exact_length=INT_MAX;
        for(int i=2;i<=size;i++){
            vector<map<int,long long>> dp(size+1);
            //dbg(dp);
            int mask=(1<<(size+1))-1;
            exact_length=min(exact_length,
                    Exact_Tsp(graph.edges,size,i,mask)+graph.edges[1][i]);
            //dbg(dp);
        }
        //cout<<exact_length<<endl;

        map<int,vector<int>> path_for_appx;
        prims_algo(1,size,graph.edges,path_for_appx);
        //dbg(path_for_appx);
        //cout<<'\n';
        vector<int> newpath;
        vector<int> visited(size+1,0);
        dfs(path_for_appx,visited,newpath,1);
        newpath.push_back(1);
        //dbg(newpath);

        long long appx_length=0;
        for(int i=0;i<newpath.size()-1;i++){
            appx_length+=graph.edges[newpath[i]][newpath[i+1]];
        }
        //cout<<appx_length<<'\n';
        cout<<test++<<". "<<(1.0*appx_length/exact_length)<<'\n';
    }

    return 0;
}

