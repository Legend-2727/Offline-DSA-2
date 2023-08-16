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
    int lower = num - 1;
    int upper = num + 1;

    while (true)
    {
        if (is_prime(upper))
            return upper;
        if (is_prime(lower))
            return lower;

        lower--;
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
class Node
{
public:
    string key;
    int value;
    Node *next;
    Node(string k, int v)
    {
        key = k;
        value = v;
        next = NULL;
    }
};

class Seperate_chaining_Hash_table
{
    vector<Node *> nodes;
    int N;
    function<int(string, int)> key_generator;

public:
    Seperate_chaining_Hash_table(int n, function<int(string, int)> hash)
    {
        N = nearest_prime(n);
        nodes.resize(N, NULL);
        key_generator = hash;
    }
    void insert(string key, int value)
    {
        int index = key_generator(key, N);
        Node *temp = nodes[index];
        if (temp == NULL)
        {
            nodes[index] = new Node(key, value);
            return;
        }
        while (temp->next != NULL)
        {
            if (temp->next->key == key)
            {
                temp->next->value = value;
                return;
            }
            temp = temp->next;
        }
        temp->next = new Node(key, value);
        return;
    }
    int find_data(string key)
    {
        int index = key_generator(key, N);
        Node *temp = nodes[index];
        while (temp != NULL)
        {
            if (temp->key == key)
                return temp->value;
            temp = temp->next;
        }
        return 0;
    }
    void Delete(string key)
    {
        int index = key_generator(key, N);
        Node *temp = nodes[index];
        if (temp == NULL)
            return;
        if (temp->key == key)
        {
            nodes[index] = temp->next;
        }
        Node *prev = temp;
        temp = temp->next;
        while (temp != NULL)
        {
            if (temp->key == key)
            {
                prev->next = temp->next;
                delete[] temp;
                return;
            }
            prev = temp;
            temp = temp->next;
        }
    }
    void print()
    {
        for (int i = 0; i < nodes.size(); i++)
        {
            Node *temp = nodes[i];
            while (temp != NULL)
            {
                cout << i << ". " << temp->key << ' ' << temp->value << '\n';
                temp = temp->next;
            }
        }
    }
};

class Double_hashing_Hash_table
{
    vector<pair<string, int>> values;
    int N;
    function<int(string, int)> key_generator;

public:
    Double_hashing_Hash_table(int n, function<int(string, int)> hash)
    {
        N = nearest_prime(n);
        key_generator = hash;
        values.resize(N, {"-1", 0});
    }
    int auxHash(string k)
    {
        int small = N - 1;
        while (!is_prime(small))
        {
            small--;
        }
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
        while (values[index].first != "-1")
        {
            index = (func1 + j * func2) % N;
            j++;
            // dbg(index,values);
        }
        values[index].first = key;
        values[index].second = value;
    }
    int find_data(string key)
    {
        int index = key_generator(key, N);
        int j = 1;
        while (values[index].first != key)
        {
            index = (key_generator(key, N) + j * auxHash(key)) % N;
            j++;
        }
        return values[index].second;
    }
    void Delete(string key)
    {
        int index = key_generator(key, N);
        int j = 1;
        while (values[index].first != key)
        {
            index = (key_generator(key, N) + j * auxHash(key)) % N;
            j++;
        }
        values[index].first = "N";
        values[index].second = 0;
    }
    void print()
    {
        for (int i = 0; i < N; i++)
        {
            cout << i << ". " << values[i].first << " " << values[i].second << '\n';
        }
    }
};

class Custom_probing_Hash_table
{
    vector<pair<string, int>> values;
    int N;
    function<int(string, int)> key_generator;

public:
    Custom_probing_Hash_table(int n, function<int(string, int)> hash)
    {
        N = nearest_prime(n);
        key_generator = hash;
        values.resize(N, {"-1", 0});
    }
    int auxHash(string k)
    {
        int small = N - 1;
        while (!is_prime(small))
        {
            small--;
        }
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
        int c1=2,c2=7;
        // dbg(func1,func2,N);
        while (values[index].first != "-1")
        {
            index = (func1 + c1*j * func2 +c2*j*j) % N;
            j++;
            // dbg(index,values);
        }
        values[index].first = key;
        values[index].second = value;
    }
    int find_data(string key)
    {
        int index = key_generator(key, N);
        int j = 1;
        int c1=2,c2=7;
        int func1 = key_generator(key, N), func2 = auxHash(key);
        while (values[index].first != key)
        {
            index = (func1 + c1*j * func2 +c2*j*j) % N;
            j++;
        }
        return values[index].second;
    }
    void Delete(string key)
    {
        int index = key_generator(key, N);
        int j = 1;
        int c1=2,c2=7;
        int func1 = key_generator(key, N), func2 = auxHash(key);
        while (values[index].first != key)
        {
            index = (func1 + c1*j * func2 +c2*j*j) % N;
            j++;
        }
        values[index].first = "N";
        values[index].second = 0;
    }
    void print()
    {
        for (int i = 0; i < N; i++)
        {
            cout << i << ". " << values[i].first << " " << values[i].second << '\n';
        }
    }
};

int main()
{
    int n, m;
    cin >> n >> m;
    string words[m];
    for (int i = 0; i < m; i++)
    {
        int len = 5 + rand() % 5;
        words[i] = string_generator(len);
    }
    Seperate_chaining_Hash_table SH1(n, hash1), SH2(n, hash2);
    for (int i = 0; i < m; i++)
    {
        SH1.insert(words[i], i + 1);
        SH2.insert(words[i], i + 1);
    }
    SH1.print();
    SH2.print();
    Double_hashing_Hash_table DH1(n, hash1), DH2(n, hash2);
    for (int i = 0; i < m; i++)
    {
        DH1.insert(words[i], i + 1);
        DH2.insert(words[i], i + 1);
    }
    DH1.print();
    DH2.print();
    Custom_probing_Hash_table CH1(n, hash1), CH2(n, hash2);
    for (int i = 0; i < m; i++)
    {
        CH1.insert(words[i], i + 1);
        CH2.insert(words[i], i + 1);
    }
    CH1.print();
    CH2.print();
    // for(int i=0;i<n;i++)
    // {
    //     cout<<words[i]<<" "<<hash1(words[i],10)<<'\n';
    // }
    // for(int i=0;i<n;i++)
    // {
    //     cout<<hash2(words[i],10)<<'\n';
    // }
    return 0;
}

