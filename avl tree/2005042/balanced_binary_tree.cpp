#include<bits/stdc++.h>
using namespace std;

class Node{
    public:
        int data;
        int height;
        Node *left;
        Node *right;
        Node(int n)
        {
            data=n;
            left=right=NULL;
            height=1;
        }
};

class Avl_tree{
    Node *root;
    public:
        Avl_tree(){root = NULL;}
        Node* getRoot()
        {
            return root;
        }
        int getHeight(Node *node)
        {
            if(node==NULL) return 0;
            return node->height;
        }
        Node* left_rotate(Node* node)
        {
            Node *child=node->right;
            node->right=child->left;
            child->left=node;
            node->height=1+max(getHeight(node->left),getHeight(node->right));
            child->height=1+max(getHeight(child->left),getHeight(child->right));
            return child;
        }
        Node* right_rotate(Node* node)
        {
            Node *child=node->left;
            node->left=child->right;
            child->right=node;
            node->height=1+max(getHeight(node->left),getHeight(node->right));
            child->height=1+max(getHeight(child->left),getHeight(child->right));
            return child;
        }
        void insert(int n)
        {
            root=utilInsert(root,n);
        }
        int getBalance(Node * node){
            if(node==NULL) return 0;
            return getHeight(node->left)-getHeight(node->right);
        }
        Node* utilInsert(Node *node,int n)
        {
            
            if(node==NULL) return new Node(n);
            if(node->data==n) return node;
            else if(n<node->data)
                node->left=utilInsert(node->left,n);
            else    
                node->right=utilInsert(node->right,n);
            node->height=1+max(getHeight(node->left),getHeight(node->right));
            int balance_fact;
            
            balance_fact=getBalance(node);
            if(balance_fact>1)
            {
                if(n<node->left->data)
                    node=right_rotate(node);
                else{
                    node->left=left_rotate(node->left);
                    node=right_rotate(node);
                }
            }
            else if(balance_fact< -1)
            {
                if(n>node->right->data)
                    node=left_rotate(node);
                else{
                    node->right=right_rotate(node->right);
                    node=left_rotate(node);
                }
            }
            return node;
        }
        void Delete(int n)
        {
            root=utilDelete(root,n);
        }
        Node* utilDelete(Node* node,int n)
        {
            if(node==NULL) return node;
            if(n>node->data)
                node->right=utilDelete(node->right,n);
            else if(n<node->data)
                node->left=utilDelete(node->left,n);
            
            else{
                if(node->left==NULL)
                {
                    if(node->right==NULL){
                        delete node;
                        return NULL;
                    }
                    else{
                        Node* temp=node->right;
                        free(node);
                        node=temp;
                    }
                }
                else if(node->right==NULL)
                {
                    Node* temp=node->left;
                    free(node);
                    node=temp;
                }
                else{
                    Node * temp=node->right;
                    while(temp->left!=NULL) temp=temp->left;
                    node->data=temp->data;
                    node->right=utilDelete(node->right,node->data);
                }
            }
            if(node==NULL) return node;
            node->height=1+max(getHeight(node->left),getHeight(node->right));
            int balance_fact;
            balance_fact=getBalance(node);
            if(balance_fact>1)
            {
                if(getBalance(node->left)>=0)
                    node=right_rotate(node);
                else{
                    node->left=left_rotate(node->left);
                    node=right_rotate(node);
                }
            }
            else if(balance_fact< -1)
            {
                if(getBalance(node->right)<=0)
                    node=left_rotate(node);
                else{
                    node->right=right_rotate(node->right);
                    node=left_rotate(node);
                }
            }
            return node;
        }
        bool find(int n)
        {
            Node * temp=root;
            while (temp!= NULL){
                if(temp->data==n) return true;
                else if(n < temp->data ) temp=temp->left;
                else if(n > temp->data ) temp=temp->right;
            }
            return false;
        }
        void pre_traverse(Node *node,ofstream &fout)
        {
            if(node==NULL) return;
            fout << node->data<<" ";
            if(node->left!=NULL || node->right!=NULL){
                fout<<"(";
                pre_traverse(node->left,fout);
                fout<< ",";
                pre_traverse(node->right,fout);
                fout<<")";
            }
        }
        void traverse(Node* node,ofstream &fout)
        {
            if(node==NULL) return;
            
            traverse(node->left,fout);   
            fout << node->data<< " ";
            traverse(node->right,fout);
        }
};

int main()
{
    ifstream fin;
    ofstream fout,f2out;
    fin.open("in.txt");
    fout.open("out_avl.txt");
    f2out.open("report_avl.txt");
    Avl_tree tree;
    long double insrt_time=0,find_time=0,dlt_time=0,trav_time=0;
    while(!fin.eof())
    {
        string s;
        int n;
        fin >> s;
        if(s=="T")
        {
            auto start_time = chrono::high_resolution_clock::now();
            tree.traverse(tree.getRoot(),fout);
            auto end_time = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();

            trav_time+=((1.0*duration)/1000000);
        } 
        else{
            fin>>n;
            if(s=="F")
            {
                auto start_time = chrono::high_resolution_clock::now();
                if(tree.find(n)) fout<<"Found";
                else fout<<"Not Found";
                auto end_time = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
                find_time+=((1.0*duration)/1000000);
            }
            else if(s=="I")
            {
                auto start_time = chrono::high_resolution_clock::now();
                tree.insert(n);
                tree.pre_traverse(tree.getRoot(),fout);
                auto end_time = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
                insrt_time+=((1.0*duration)/1000000);
                
            }
            else if(s=="D")
            {
                auto start_time = chrono::high_resolution_clock::now();
                tree.Delete(n);
                tree.pre_traverse(tree.getRoot(),fout);
                auto end_time = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
                dlt_time+=((1.0*duration)/1000000);
                
            }
        }
        fout<<"\n";
    }
    f2out<<"operation time(ms)\n";
    f2out<<"insert "<<fixed<<setprecision(8)<<insrt_time<<"\n";
    f2out<<"delete "<<fixed<<setprecision(8)<<dlt_time<<"\n";
    f2out<<"search "<<fixed<<setprecision(8)<<find_time<<"\n";
    f2out<<"trav "<<fixed<<setprecision(8)<<trav_time<<"\n";
    auto total_time=insrt_time+dlt_time+find_time+trav_time;
    f2out<<"total "<<fixed<<setprecision(8)<<total_time<<"\n";
    f2out.close();
    fout.close();
    fin.close();
}