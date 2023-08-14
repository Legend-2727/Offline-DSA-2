#include<bits/stdc++.h>
using namespace std;

class Node{
    public:
    int data;
    int degree;
    Node* parent;
    Node* child;
    Node* right_sibling;
    Node(int key)
    {
        data = key;
        degree = 0;
        child = parent = right_sibling = NULL;
    }
    int get_data(){
        return data;
    }
};

class Binomial_heap{
    vector<Node*> heap;
public:
    vector<Node*>& get_heap(){
        return heap;
    }
    Node* merge(Node *n1,Node* n2)
    {
        if(n1->data<n2->data)
        {
            n2->right_sibling=n1->child;
            n1->child=n2;
            n2->parent=n1;
            n1->degree++;
            return n1;
        }
        n1->right_sibling=n2->child;
        n2->child=n1;
        n1->parent=n2;
        n2->degree++;
        return n2;
    }
    vector<Node*>& Union(vector<Node*> &hp1,vector<Node*> &hp2)
    {
        if(hp1.size()==0){
            heap=hp2;
            return heap;
        } 
        if(hp2.size()==0)
        {
            heap=hp1;
            return heap;
        } 

        vector<Node*> temp1;
        int ind1=0,ind2=0;
        while(ind1<hp1.size()&&ind2<hp2.size())
        {
            if(hp1[ind1]->degree>hp2[ind2]->degree)
            {
                temp1.push_back(hp1[ind1]);
                ind1++;
            }
            else
            {
                temp1.push_back(hp2[ind2]);
                ind2++;
            }
        }
        while(ind1<hp1.size())
        {
            temp1.push_back(hp1[ind1]);
            ind1++;
        }
        while(ind2<hp2.size())
        {
            temp1.push_back(hp2[ind2]);
            ind2++;
        }
        shrink(temp1);
        return heap;
    }

    void shrink(vector<Node*> &temp1)
    {
        if(temp1.size()<=1)
        {
            heap=temp1;
            return ;
        }
        (temp1[1]->data,temp1[0]->data);
        vector<Node*> temp;
        int i=temp1.size()-1,j=i-1,k=i-2;
        (i,j,k);
        while(i>=0)
        {
            if(j>=0 && temp1[i]->degree==temp1[j]->degree)
            {
                if(k>=0 && temp1[i]->degree==temp1[k]->degree)
                {
                    temp.push_back(temp1[i]);
                    
                }
                else{
                    (temp1[j]->data,temp1[i]->data);
                    temp1[j]=merge(temp1[i],temp1[j]);
                }
            }
            else{
                temp.push_back(temp1[i]);
            }
            i--;
            j--;
            k--;
        }
        reverse(temp.begin(),temp.end());
        heap=temp;
    }
    void insert(int value)
    {
        heap.push_back(new Node(value));
        shrink(heap);
    }
    Node* find_min()
    {
        if(heap.size()==0) return NULL;
        Node *temp=heap[0];
        for(int i=0;i<heap.size();i++)
        {
            if(temp->data>heap[i]->data)
            {
                temp=heap[i];
            }
        }
        return temp;
    }

    Node* extract_min()
    {
        if(heap.size()==0) return NULL;
        Node *temp=heap[0];
        int temp_ind=0;
        for(int i=0;i<heap.size();i++)
        {
            if(temp->data>heap[i]->data)
            {
                temp=heap[i];
                temp_ind=i;
            }
        }
        vector<Node*> temp_vect;
        Node* temp2=temp->child;
        while(temp2!=NULL)
        {
            Node *newNode=temp2;
            temp2=temp2->right_sibling;
            newNode->parent=NULL;
            newNode->right_sibling=NULL;
            temp_vect.push_back(newNode);
        }
        (temp->data);
        vector<Node*> part1(heap.begin(),heap.begin()+temp_ind);
        vector<Node*> part2(heap.begin()+temp_ind+1,heap.end());
        Union(part1,temp_vect);
        (part1.size(),part2.size(),heap.size(),heap[0]->data);
        Union(heap,part2);
        (temp->data);
        (temp->data);
        return temp;
    }
    void print(ofstream &fout){
        fout<<"Printing Binomial Heap...\n";
        for(int i=heap.size()-1;i>=0;i--)
        {
            print_tree(fout,heap[i]);
        }
    }

    void print_tree(ofstream &fout,Node* node)
    {
        map<int,vector<int>> level;
        map<Node*,int> visited;
        mapping(level,node,0,visited);
        fout<<"Binomial Tree , B"<<node->degree<<'\n';
        for(auto it=level.begin();it!=level.end();it++)
        {
            fout<<"Level "<<it->first<<" : ";
            for(int i=0;i<it->second.size();i++)
            {
                fout<<it->second[i]<<" ";
            }
            fout<<"\n";
        }
    }
    void mapping(map<int,vector<int>> &level,Node* cur,int cur_level,
                    map<Node*,int> &visited)
    {
        if(cur==NULL) return;
        if(visited[cur]) return;
        visited[cur]=1;
        level[cur_level].push_back(cur->data);
        mapping(level,cur->child,cur_level+1,visited);
        mapping(level,cur->right_sibling,cur_level,visited);
    }
};

using namespace std;

int main()
{
    ifstream fin("in.txt");
    ofstream fout("out.txt");
    Binomial_heap main_heap;
    string line;
    while(getline(fin,line))
    {
        if(line[0]=='I')
        {
            int a=stoi(line.substr(2));
            main_heap.insert(a);
        }
        else if(line[0]=='F'){
            fout<<"Find-Min returned "<<main_heap.find_min()->get_data()<<'\n';
        }
        else if(line[0]=='E')
        {
            fout<<"Extract-Min returned "<<main_heap.extract_min()->get_data()<<'\n';
        }
        else if(line[0]=='P')
        {
            main_heap.print(fout);
        }
        else if(line[0]=='U')
        {
            stringstream values(line.substr(2));
            Binomial_heap new_heap;
            string temp;
            while(values>>temp)
            {
                new_heap.insert(stoi(temp));
            }
            main_heap.Union(main_heap.get_heap(),new_heap.get_heap());
        }
    }
    
    return 0;
}

