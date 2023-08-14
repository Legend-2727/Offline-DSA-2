#include<bits/stdc++.h>
using namespace std;

    int bfs(int sink,vector<vector<int>> &adj,vector<int> &parent,
            vector<vector<int>> &capacity) 
    {
        parent[0] = -2;
        queue<pair<int, int>> q;
        q.push({0,1e7});

        while (!q.empty()) {
            int cur = q.front().first;
            int flow = q.front().second;
            q.pop();
            for (auto next : adj[cur]) {
                if (parent[next] == -1 && capacity[cur][next]) {
                    parent[next] = cur;
                    int new_flow = min(flow, capacity[cur][next]);
                    if (next == sink)
                        return new_flow;
                    q.push({next, new_flow});
                }
            }
        }
        return 0;
    }

    void dfs(int source,vector<int>& visited,vector<vector<int>> &adj,
                vector<vector<int>> &capacity)
    {
        visited[source]=1;
        for(auto it:adj[source])
        {
            if(!visited[it] && capacity[source][it]) dfs(it,visited,adj,capacity);
        }
    }

    bool edmond_karp(int sink,vector<vector<int>> &adj,
                    vector<vector<int>> &capacity,int len) 
    {
        int flow = 0;
        vector<int> parent(len,-1);
        int new_flow=bfs(sink,adj,parent,capacity);
        while (new_flow!=0) {
            flow += new_flow;
            int cur = sink;
            while (cur != 0) {
                int prev = parent[cur];
                capacity[prev][cur] -= new_flow;
                capacity[cur][prev] += new_flow;
                cur = prev;
            }
            fill(parent.begin(),parent.end(),-1);
            new_flow=bfs(sink,adj,parent,capacity);
        }
        for(auto it:capacity[0])
        {
            if(it>0) return true;
        }
        return false;
    }

    void build_nodes(map<pair<int,int>,int> &node_id,int team_ind,int n)
    {
        node_id[{-1,-1}]=0;
        int index=1;
        vector<int> done(n,0);
        done[team_ind]=1;
        for(int i=0;i<n;i++)
        {
            if(done[i]!=1)
            {
                done[i]=1;
                for(int j=0;j<n;j++)
                {
                    if(done[j]!=1)
                    {
                        node_id[{i,j}]=index++;
                    }
                }
            }       
        }
        for(int i=0;i<n;i++)
        {
            if(i!=team_ind) node_id[{i,i}]=index++;
        }
        node_id[{-2,-2}]=index;
    }

    void build_graph(vector<vector<int>> &adj,vector<vector<int>> &capacity,
                    map<pair<int,int>,int> &node_id,vector<vector<int>> &left_matches,
                    vector<int> &win,int max)
    {
        for(auto it:node_id)
        {
            int u=it.first.first;
            int v=it.first.second;
            if(it.second!=0 && it.first.first!=-2)
            {
                
                
                if(u!=v)
                {
                    adj[0].push_back(it.second);
                    adj[it.second].push_back(0);
                    capacity[0][it.second]=left_matches[u][v];
                    capacity[it.second][0]=0;
                    adj[node_id[{u,u}]].push_back(it.second);
                    adj[it.second].push_back(node_id[{u,u}]);
                    capacity[node_id[{u,u}]][it.second]=0;
                    capacity[it.second][node_id[{u,u}]]=1e7;
                    adj[node_id[{v,v}]].push_back(it.second);
                    adj[it.second].push_back(node_id[{v,v}]);
                    capacity[node_id[{v,v}]][it.second]=0;
                    capacity[it.second][node_id[{v,v}]]=1e7;
                }
                else if(u==v)
                {
                    adj[node_id[{-2,-2}]].push_back(it.second);
                    adj[it.second].push_back(node_id[{-2,-2}]);
                    capacity[node_id[{-2,-2}]][it.second]=0;
                    capacity[it.second][node_id[{-2,-2}]]=max-win[u];
                }
            }
        }
        
    }

    void isEliminated(int team_ind,vector<int> &win,vector<vector<int>> &left_matches,
                        vector<string> &teams,int max,vector<int> &left)
    {
        int n=win.size();
        
        map<pair<int,int>,int> node_id;
        build_nodes(node_id,team_ind,n);
        int graph_len=node_id.size();
        vector<vector<int>> capacity(graph_len,vector<int> (graph_len));
        vector<vector<int>> adj(graph_len);
        build_graph(adj,capacity,node_id,left_matches,win,max);

        if(edmond_karp(graph_len-1,adj,capacity,graph_len))
        {
            cout<<teams[team_ind]<<" is eliminated\n";
            cout<<"They can win at most ";
            cout<<win[team_ind]<<"+"<<left[team_ind]<<"="<<max<<" games.\n";
            vector<int> visited(graph_len,0);
            
            
            vector<int> winning_teams;
            int current_won=0,current_winner=-1;
            for(int i=0;i<teams.size();i++)
            {
                if(win[i]>current_won)
                {
                    current_won=win[i];
                    current_winner=i;
                }
            }
            if(max<current_won)
            {
                winning_teams.push_back(current_winner);
            }
            else{
                dfs(0,visited,adj,capacity);
                for(int i=0;i<teams.size();i++)
                {
                    if(i!=team_ind && visited[node_id[{i,i}]])
                    {
                        winning_teams.push_back(i);
                    }
                }
            }
            int winners=winning_teams.size();
            for(int i=0;i<winners;i++)
            {
                cout<<teams[winning_teams[i]];

                if(i<(winners)-2) cout<<",";

                else if(i==winners-2) cout<<" and";

                cout<<" ";
            }
            winners==1? cout<<"has ":cout<<"have ";
            int total_win=0;
            for(int i=0;i<winners;i++)
            {
                total_win+=win[winning_teams[i]];
            }
            int left_match_count=0;
            for(int i=0;i<winners;i++)
            {
                for(int j=0;j<winners;j++)
                {
                    left_match_count+=left_matches[winning_teams[i]][winning_teams[j]];
                }
            }
            left_match_count/=2;
            cout<<"won a total of "<<total_win<<" games\n";
            cout<<"They play each other "<<left_match_count<<" times\n";
            cout<<"So on average, each of the teams in this group wins ";
            int possible_win=total_win+left_match_count;
            cout<<possible_win<<"/"<<winners<<"=";
            cout<<((1.0*possible_win)/winners)<<"\n\n\n";
        }
    }

int main()
{
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    int n;
    cin>>n;
    vector<string> teams(n);
    vector<int> win(n),lose(n),left(n);
    vector<vector<int>> left_matches(n, vector<int> (n));
    for(int i=0;i<n;i++)
    {
        cin>>teams[i]>>win[i]>>lose[i]>>left[i];
        for(int j=0;j<n;j++)
        {
            cin>>left_matches[i][j];
        }
    }
    for(int i=0;i<n;i++)
    {
        int max_win=win[i]+left[i];
        isEliminated(i,win,left_matches,teams,max_win,left);
    }
}