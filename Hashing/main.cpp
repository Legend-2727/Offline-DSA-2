#include <bits/stdc++.h>
#include "../dbg.h"
using namespace std;

const long long prime = 16777619;
const long long offset = 2166136261;

bool is_prime(int num)
{
    if (num <= 1)
        return false;
    if (num <= 3)
        return true;
    if (num % 2 == 0 || num % 3 == 0)
        return false;

    for (int i = 5; i * i <= num; i += 6)
    {
        if (num % i == 0 || num % (i + 2) == 0)
            return false;
    }

    return true;
}

int nearest_prime(int num)
{
    if (num <= 1)
        return 2;
    if (is_prime(num))
        return num;
    int upper = num;

    while (true)
    {
        if (is_prime(upper)) return upper;
        upper++;
    }
}

string string_generator(int len)
{
    string s = "";
    for (int i = 0; i < len; i++)
    {
        s.push_back('a' + rand() % 26);
    }
    return s;
}

int hash1(string s, int n)
{

    unsigned long long hash = offset;

    for (int i = 0; i < s.size(); i++)
    {
        hash ^= (1ull * (s[i] - 'a'));
        hash *= prime;
    }
    int temp = hash % n;
    return temp;
}

int hash2(string s, int n)
{
    unsigned long hash = 0;

    for (int i = 0; i < s.size(); i++)
    {
        hash += (1ul * (s[i] - 'a'));
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    int temp = (hash % n);
    return temp;
}

class Seperate_chaining_Hash_table
{
    vector<list<pair<string,int>>> nodes;
    int N;
    function<int(string, int)> key_generator;
    int collision;
    int probe;

public:
    Seperate_chaining_Hash_table(int n, function<int(string, int)> hash)
    {
        N = nearest_prime(n);
        nodes.resize(N);
        key_generator = hash;
        collision=0;
        probe=0;
    }
    void insert(string key, int value)
    {
        int index = key_generator(key, N);
        if(nodes[index].size()!=0) collision++;
        nodes[index].push_back({key,value});

    }
    int find_data(string key)
    {
        int index = key_generator(key, N);
        for(auto it:nodes[index]){
            probe++;
            if((it).first == key){
                return it.second;
            }
        }
        return -1;
    }
    void Delete(string key)
    {
        dbg(key);
        int index = key_generator(key, N);
        dbg(index);
        for(auto &it:nodes[index])
        {
            if(it.first==key)
            {
                nodes[index].remove(it);
                return;
            }
        }
    }
    void print()
    {
        for (int i = 0; i < nodes.size(); i++)
        {
            for(auto it:nodes[i])
            {
                cout << i << ". " << it.first << ' ' << it.second << '\n';
            }
        }
    }
    int get_collisions(){return collision;}
    int get_probes(){return probe;}
};

class Probing_Hash_table
{
    vector<pair<string, int>> values;
    int N;
    function<int(string, int)> key_generator;
    vector<int> status;
    int type;
    int collision,probes;

public:
    Probing_Hash_table(int n, function<int(string, int)> hash,int t)
    {
        N = nearest_prime(n);
        key_generator = hash;
        values.resize(N, {"-1", 0});
        status.resize(N,-1);
        type=t;
        collision=0;
        probes=0;
    }
    int double_hashing(string key,int j)
    {
        return (key_generator(key,N)+j*auxHash(key))%N;
    }
    int custom_probing(string key,int j)
    {
        return (key_generator(key,N) + 2*j * auxHash(key) +7*j*j) % N;
    }
    int auxHash(string k)
    {
        int small = N - 1;
        while (!is_prime(small) && small>2)
        {
            small--;
        }
        small=max(2,small);
        int hash = 0;
        for (int i = 0; i < k.size(); i++)
        {
            hash += (k[i] - 'a');
        }
        return small - hash % small;
    }
    void insert(string key, int value)
    {
        int index = key_generator(key, N);
        int j = 1;
        int func1 = key_generator(key, N), func2 = auxHash(key);
        // dbg(func1,func2,N);
        while (status[index]==1)
        {
            collision++;
            if(values[index].first==key)
            {
                values[index].second=value;
                return;
            }
            if(type==1){
                index=(double_hashing(key,j));
            }
            else{
                index =(custom_probing(key,j));
            }
            j++;
        }
        values[index].first = key;
        values[index].second = value;
        status[index]=1;
    }
    int find_ind(string key)
    {
        int index = key_generator(key, N);
        int j = 1;
        while (j<N)
        {
            probes++;
            if(values[index].first==key) return index;
            if(status[index] == -1) return -1;
            if(type==1){
                index=(double_hashing(key,j));
            }
            else{
                index =(custom_probing(key,j));
            }
            j++;
        }
        probes-=N;
        return -1;
    }
    int find_data(string key){
        int ind=find_ind(key);
        if(ind==-1) return -1;
        else return values[ind].second;
    }
    void Delete(string key)
    {
        int index=find_ind(key);
        if(index<0) return;
        values[index].first = "-1";
        values[index].second = 0;
        status[index]=0;
    }
    void print()
    {
        for (int i = 0; i < N; i++)
        {
            cout << i << ". " << values[i].first << " " << values[i].second << '\n';
        }
    }
    int get_collisions(){return collision;}
    int get_probes(){return probes;}
};

int main()
{
    freopen("out.txt","w",stdout);
    srand(42);
    int m=10000;
    
    string words[m];
    int v=1;
    for (int i = 0; i < m; i++)
    {
        int len = 5 + rand() % 5;
        words[i] = string_generator(len);
    }
    int arr[3]={5000,10000,20000};
    //dbg(arr[2]);
    for(int i=0;i<3;i++)
    {
        int v=1;
        Seperate_chaining_Hash_table sh1(arr[i],hash1),sh2(arr[i],hash2);
        Probing_Hash_table dh1(arr[i],hash1,1) ,dh2(arr[i],hash2,1);
        Probing_Hash_table ch1(arr[i],hash1,2) ,ch2(arr[i],hash2,2);
        for(int j=0;j<min(10000,arr[i]);j++)
        {
            if(sh1.find_data(words[j])!=-1) continue;
            sh1.insert(words[j],v);
            sh2.insert(words[j],v);
            dh1.insert(words[j],v);
            dh2.insert(words[j],v);
            ch1.insert(words[j],v);
            ch2.insert(words[j],v);
            v++;
        }
        for(int j=0;j<1000;j++)
        {
            int rnd=rand()%(min(10000,arr[i]));
            //dbg(rnd);
            sh1.find_data(words[rnd]);
            sh2.find_data(words[rnd]);
            dh1.find_data(words[rnd]);
            dh2.find_data(words[rnd]);
            ch1.find_data(words[rnd]);
            ch2.find_data(words[rnd]);
        }
        cout<<"For n = "<<arr[i]<<"\n";
        cout<<"Hash        Collision               Hash1              Hash2     \n";
        cout<<"Table Size  resolution          #of     Average    #of     Average\n";
        cout<<"            Method            Collison  Probes     Method  Probe  \n\n";
        cout<<"            Chaining              "<<sh1.get_collisions()<<"       "<<sh1.get_probes();
        cout<<"   "<<sh2.get_collisions()<<"        "<<sh2.get_probes()<<"\n\n";
        cout<<"            Double Hashing        "<<dh1.get_collisions()<<"       "<<dh1.get_probes();
        cout<<"   "<<dh2.get_collisions()<<"        "<<dh2.get_probes()<<"\n\n";
        cout<<"            Custom Probing        "<<ch1.get_collisions()<<"       "<<ch1.get_probes();  
        cout<<"   "<<ch2.get_collisions()<<"        "<<ch2.get_probes()<<"\n\n";
    }
    
    return 0;
}

