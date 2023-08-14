// C++ program to implement different operations
// on Binomial Heap
#include<bits/stdc++.h>
using namespace std;

// A Binomial Tree node.
struct Node
{
int data, degree;
Node *child, *sibling, *parent;
};

Node* newNode(int key)
{
Node *temp = new Node;
temp->data = key;
temp->degree = 0;
temp->child = temp->parent = temp->sibling = NULL;
return temp;
}

// This function merge two Binomial Trees.
Node* mergeBinomialTrees(Node *b1, Node *b2)
{
// Make sure b1 is smaller
if (b1->data > b2->data)
	swap(b1, b2);

// We basically make larger valued tree
// a child of smaller valued tree
b2->parent = b1;
b2->sibling = b1->child;
b1->child = b2;
b1->degree++;

return b1;
}

// This function perform union operation on two
// binomial heap i.e. l1 & l2
list<Node*> unionBionomialHeap(list<Node*> l1,
			list<Node*> l2)
{
// nsss to another binomial heap which contain
// new heap after merging l1 & l2
list<Node*> nsss;
list<Node*>::iterator it = l1.begin();
list<Node*>::iterator ot = l2.begin();
while (it!=l1.end() && ot!=l2.end())
{
	// if D(l1) <= D(l2)
	if((*it)->degree <= (*ot)->degree)
	{
	nsss.push_back(*it);
	it++;
	}
	// if D(l1) > D(l2)
	else
	{
	nsss.push_back(*ot);
	ot++;
	}
}

// if there remains some elements in l1
// binomial heap
while (it != l1.end())
{
	nsss.push_back(*it);
	it++;
}

// if there remains some elements in l2
// binomial heap
while (ot!=l2.end())
{
	nsss.push_back(*ot);
	ot++;
}
return nsss;
}

// adjust function rearranges the heap so that
// heap is in increasing order of degree and
// no two binomial trees have same degree in this heap
list<Node*> adjust(list<Node*> heap)
{
if (heap.size() <= 1)
	return heap;
list<Node*> newheap;
list<Node*>::iterator it1,it2,it3;
it1 = it2 = it3 = heap.begin();

if (heap.size() == 2)
{
	it2 = it1;
	it2++;
	it3 = heap.end();
}
else
{
	it2++;
	it3=it2;
	it3++;
}
while (it1 != heap.end())
{
	// if only one element remains to be processed
	if (it2 == heap.end())
	it1++;

	// If D(it1) < D(it2) i.e. merging of Binomial
	// Tree pointed by it1 & it2 is not possible
	// then move next in heap
	else if ((*it1)->degree < (*it2)->degree)
	{
	it1++;
	it2++;
	if(it3!=heap.end())
		it3++;
	}

	// if D(it1),D(it2) & D(it3) are same i.e.
	// degree of three consecutive Binomial Tree are same
	// in heap
	else if (it3!=heap.end() &&
		(*it1)->degree == (*it2)->degree &&
		(*it1)->degree == (*it3)->degree)
	{
	it1++;
	it2++;
	it3++;
	}

	// if degree of two Binomial Tree are same in heap
	else if ((*it1)->degree == (*it2)->degree)
	{
	Node *temp;
	*it1 = mergeBinomialTrees(*it1,*it2);
	it2 = heap.erase(it2);
	if(it3 != heap.end())
		it3++;
	}
}
return heap;
}

// inserting a Binomial Tree into binomial heap
list<Node*> insertATreeInHeap(list<Node*> heap,
			Node *tree)
{
// creating a new heap i.e temp
list<Node*> temp;

// inserting Binomial Tree into heap
temp.push_back(tree);

// perform union operation to finally insert
// Binomial Tree in original heap
temp = unionBionomialHeap(heap,temp);

return adjust(temp);
}

// removing minimum key element from binomial heap
// this function take Binomial Tree as input and return
// binomial heap after
// removing head of that tree i.e. minimum element
list<Node*> removeMinFromTreeReturnBHeap(Node *tree)
{
list<Node*> heap;
Node *temp = tree->child;
Node *lo;

// making a binomial heap from Binomial Tree
while (temp)
{
	lo = temp;
	temp = temp->sibling;
	lo->sibling = NULL;
	heap.push_front(lo);
}
return heap;
}

// inserting a key into the binomial heap
list<Node*> insert(list<Node*> head, int key)
{
Node *temp = newNode(key);
return insertATreeInHeap(head,temp);
}

// return pointer of minimum value Node
// present in the binomial heap
Node* getMin(list<Node*> heap)
{
list<Node*>::iterator it = heap.begin();
Node *temp = *it;
while (it != heap.end())
{
	if ((*it)->data < temp->data)
	temp = *it;
	it++;
}
return temp;
}

list<Node*> extractMin(list<Node*> heap)
{
list<Node*> newheap,lo;
Node *temp;

// temp contains the pointer of minimum value
// element in heap
temp = getMin(heap);
list<Node*>::iterator it;
it = heap.begin();
while (it != heap.end())
{
	if (*it != temp)
	{
	// inserting all Binomial Tree into new
	// binomial heap except the Binomial Tree
	// contains minimum element
	newheap.push_back(*it);
	}
	it++;
}
lo = removeMinFromTreeReturnBHeap(temp);
newheap = unionBionomialHeap(newheap,lo);
newheap = adjust(newheap);
return newheap;
}

// print function for Binomial Tree
void printTree(Node *h)
{
while (h)
{
	cout << h->data << " ";
	printTree(h->child);
	h = h->sibling;
}
}

// print function for binomial heap
void printHeap(list<Node*> heap)
{
list<Node*> ::iterator it;
it = heap.begin();
while (it != heap.end())
{
	printTree(*it);
	it++;
}
}


// Driver program to test above functions
int main()
{
int ch,key;
list<Node*> heap;

// Insert data in the heap
heap = insert(heap,10);
heap = insert(heap,20);
heap = insert(heap,30);

cout << "Heap elements after insertion:\n";
printHeap(heap);

Node *temp = getMin(heap);
cout << "\nMinimum element of heap "
	<< temp->data << "\n";

// Delete minimum element of heap
heap = extractMin(heap);
cout << "Heap after deletion of minimum element\n";
printHeap(heap);

return 0;
}
