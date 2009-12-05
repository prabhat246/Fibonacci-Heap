/***************************************************************************
 *   Copyright (C) 2007 by Prabhat Kumar Gupta                             *
 *   mcs072895@yaman.cse.iitd.ernet.in                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string.h>
#include<stdio.h>
#include<malloc.h>
#define MAXKEY 100
#define MAXHEAPS 11
using namespace std;

struct HeapNode
{
	HeapNode *lsibling;
	int tag;
	HeapNode *child;
	HeapNode *parent;
	int key;
	int degree;
	HeapNode *rsibling;
};

struct FreeListNode
{
	HeapNode *addr;
	FreeListNode *Link;
}*F;

struct Heap
{
	HeapNode *rootlist;
	int rootlistcount;
}H;

int nodecount,nodecreatedflag,n,hn,na;
void InputConsole();
void Display(int);
Heap *FibHeapArray[MAXHEAPS];
HeapNode *SearchArray[MAXKEY];

//*************************Memory Management Module (FreeList Implementation****************************
FreeListNode *MakeFreeList(unsigned int n)
{
	FreeListNode *FreeListHead, *Temp;
	HeapNode *Node;
	FreeListHead = NULL;
	nodecreatedflag=1;
	for(int i=1;i<=n;i++)
	{
	Temp=(FreeListNode *) malloc(sizeof(FreeListNode));
	Node =(HeapNode *) malloc(sizeof(HeapNode));
	
	Temp->Link=FreeListHead;
	Temp->addr=Node;
	FreeListHead=Temp;
	}
	nodecount=n;
	printf("\n%d Nodes Created.",n);
	return FreeListHead;
}
void CreateNodes(int n)
{	F= MakeFreeList(n);
}
HeapNode *GetNode(FreeListNode *FreeListHead)
{
	FreeListNode *temp;
	HeapNode *Node;
	Node = F->addr;
	temp = F;
	F=F->Link;
	free(temp);
	nodecount=nodecount--;
	return Node;
}
void PushNode(HeapNode *Node)
{
	FreeListNode *temp;
	temp = (FreeListNode *) malloc(sizeof(FreeListNode));
	temp->addr=Node;
	temp->Link=F;
	F=temp;
	nodecount=nodecount++;
}

//*****************************************Print Module***********************************************
void PrintRootlist(Heap *H)
{	if(H->rootlist != NULL)
	{
		HeapNode *temp = H->rootlist;
		cout<<temp->key;
		if(temp->rsibling != temp)
		{
			temp = temp->rsibling;
			while(temp != H->rootlist)
			{
				cout<<temp->key;
				temp = temp->rsibling;
			}
		}
	}
}
void PrintTree(HeapNode *HN,int n)
{	HeapNode *t = HN;
	int i;
	do
	{
		cout<<t->key;
		if(t->tag !=0)
			cout<<"*";
		else
			cout<<" ";
		
		if(t->child !=NULL)
		{
			cout<<"=>\t";
			PrintTree(t->child,n+1);
		}
		if(t->rsibling != HN)
		{
		cout<<endl;
		for(i=0;i<n;i++)
			cout<<"\t";
		cout<<"|";
		cout<<endl;

		for(i=0;i<n;i++)
		cout<<"\t";
		}
		t=t->rsibling;
		}
	while(t!= HN);
}
void PrintHeap(Heap *H)
{	cout<<endl;
	cout<<endl;
	if(H->rootlist !=NULL)
		PrintTree(H->rootlist,0);
	else
		cout<<"Heap is Empty.";
	cout<<endl;
}
void PrintNode(HeapNode *A)
{
cout<<endl<<"Node Key :"<<A->key;


if(A->lsibling != NULL)
cout<<" lsib L : "<<A->lsibling->key;
else cout<<" lsib l : NULL ";


if(A->rsibling != NULL)
cout<<" lsib R : "<<A->rsibling->key;
else cout<<" lsib R : NULL ";

cout<<" Degree :"<<A->degree;

if(A->child != NULL)
cout<<" Child :"<<A->child->key;
else cout<<" Child : NULL";

if(A->parent!= NULL)
cout<<" Parent :"<<A->parent->key;
else cout<<" Parent :NULL ";

cout<<" tag :"<<A->tag<<endl;
}
//****************************Fibonacci Heap Operations ************************************************************
Heap *MakeHeap()
{
Heap *H=(Heap*) malloc(sizeof(Heap));
H->rootlist=NULL;
H->rootlistcount= 0;
cout<<endl<<"Heap "<<hn<<" created.";
hn+=1;
return H;
}
void Insert(Heap *H, int x)
{
	if(x<MAXKEY && SearchArray[x]==NULL)
	{	HeapNode *NewNode=GetNode(F);
		SearchArray[x]=NewNode;
		NewNode->parent=NULL;
		NewNode->child=NULL;
		NewNode->degree=0;
		NewNode->key=x;
		NewNode->tag=0;
		NewNode->lsibling=NewNode;
		NewNode->rsibling=NewNode;
		
		if(H->rootlistcount==0)
		{
		H->rootlist = NewNode;
		H->rootlistcount=1;
		}
		else
		{
			NewNode->lsibling=H->rootlist;
			NewNode->rsibling=H->rootlist->rsibling;
			H->rootlist->rsibling=NewNode;
			NewNode->rsibling->lsibling=NewNode;
			if((H->rootlist->key) > (NewNode->key))
			H->rootlist = NewNode;
			H->rootlistcount+=1;
		}
	}
	else if(x<MAXKEY && SearchArray[x]!=NULL)
		cout<<endl<<"Key already exists in Heap. Duplicate Key Not Allowed. Enter a different key.";
	else
		cout<<endl<<"Key "<<x<<" can not be inserted. Maximum allowable key is "<<MAXKEY;
}

int FindMin(Heap *H)
{	
	return H->rootlist->key;
}

Heap *MergeHeaps(Heap *H1, Heap *H2)
{	Heap *H3;
	HeapNode *t1;
	if(H1->rootlist ==NULL)
	{	H3=H2;
		H2=NULL;
	}
	else if(H2->rootlist ==NULL)
	{	H3=H1;
		H1=NULL;
	}
	else if (H1->rootlist == NULL and H2->rootlist == NULL)
		H3=NULL;
	else
	{	H3=H1;	
		H1->rootlist->rsibling->lsibling = H2->rootlist->lsibling;
		H2->rootlist->lsibling->rsibling = H1->rootlist->rsibling;
		H1->rootlist->rsibling = H2->rootlist;
		H2->rootlist->lsibling=H1->rootlist;

		if((H2->rootlist->key)< (H1->rootlist->key))
			H1->rootlist = H2->rootlist;
		H3->rootlistcount=H1->rootlistcount + H2->rootlistcount;

		H2=NULL;
		H1=NULL;
	}
	return H3;
}
HeapNode *Link(HeapNode *t1, HeapNode *t2)
{
	if((t1->child == NULL) && (t1->degree==0))
	{	
		t1->child=t2;
		t1->degree+=1;
		t2->parent=t1;
		t2->lsibling=t2;
		t2->rsibling=t2;
		return t1;
	}
	else
	{	
		t2->parent=t1;
		t1->degree+=1;
		t2->rsibling=t1->child->rsibling;
		t2->lsibling=t1->child;
		t1->child->rsibling->lsibling=t2;
		t1->child->rsibling=t2;
		return t1;
	}
}
void Consolidate(Heap *H)
{
	HeapNode *array[100];
	for(int i=0;i<100;i++)
		array[i]=NULL;

	int x=H->rootlistcount;
	HeapNode *t1,*t2,*temp;
	t1=H->rootlist;
	temp = t1->rsibling;
	int d;

	for(int i=1; i <= x; i++)
	{	
		d=t1->degree;

		while(array[d] != 0)
		{
			t2=array[d];
			if(t1->key < t2->key)	
				t1=Link(t1,t2);
			else
				t1=Link(t2,t1);
			array[d]=0;
			d=d+1;
		};

		array[d]=t1;
		t1 = temp;
		temp = temp->rsibling;
	}
	H->rootlist=NULL;
	H->rootlistcount = 0;
	for(int i=0; i<100; i++)
	{
		
		if(array[i] != 0)
		{
			
			if(H->rootlist == NULL)
			{
				H->rootlist=array[i];
				H->rootlist->lsibling = H->rootlist->rsibling= H->rootlist;
				H->rootlistcount=1;
			}
			else
			{	array[i]->lsibling = H->rootlist;
				array[i]->rsibling = H->rootlist->rsibling;
				H->rootlist->rsibling->lsibling = array[i];
				H->rootlist->rsibling = array[i];
				H->rootlistcount +=1;
				if(array[i]->key < H->rootlist->key)
				H->rootlist=array[i] ;
			}
		}
	}
}

int DeleteMin(Heap *H)
{	
if(H->rootlist != NULL)
{
	int temp;
	HeapNode *tempptr,*t1,*t2;
	temp = H->rootlist->key;
	if((H->rootlist->child==NULL) && (H->rootlistcount==1))
	{
		PushNode(H->rootlist);
		H->rootlistcount=0;
		H->rootlist=NULL;
	}
	else if ((H->rootlist->child==NULL) && (H->rootlistcount > 1))
	{	
		H->rootlist->lsibling->rsibling = H->rootlist->rsibling;
		H->rootlist->rsibling->lsibling = H->rootlist->lsibling;
		tempptr=H->rootlist;
		H->rootlist=H->rootlist->lsibling;
		PushNode(tempptr);
		H->rootlistcount-=1;
		if(H->rootlist->key != 3)
			Consolidate(H);
	}
	else if(H->rootlist->child != NULL)
	{
		t1 = H->rootlist->child;
		while(t1 != t1->rsibling )
		{
			t2=t1->rsibling;
			t1->rsibling = t2->rsibling;
			t2->rsibling->lsibling = t1;
			
			t2->rsibling= H->rootlist->rsibling;
			t2->lsibling = H->rootlist;
			t2->parent=NULL;
			t2->tag = 0;

			H->rootlist->rsibling->lsibling = t2;
			H->rootlist->rsibling=t2;
			H->rootlistcount +=1;
		}
			t1->rsibling= H->rootlist->rsibling;
			t1->lsibling = H->rootlist;
			t1->parent=0;
			t1->tag=0;
			H->rootlist->rsibling->lsibling = t1;
			H->rootlist->rsibling=t1;
			H->rootlistcount +=1;
			H->rootlist->degree = 0;
			t2=H->rootlist;
			H->rootlist->lsibling->rsibling = H->rootlist->rsibling;
			H->rootlist->rsibling->lsibling = H->rootlist->lsibling;
			H->rootlist=H->rootlist->rsibling;
			PushNode(t2);
			H->rootlistcount -=1;
			Consolidate(H);
	}
return temp;
}
else
	{
		cout<<"Heap is Empty.";
		return -1;
	}
}
void Cut(Heap *H, HeapNode *t, HeapNode *p)
{
	t->rsibling->lsibling = t->lsibling;
	t->lsibling->rsibling = t->rsibling;
	if(t->parent->degree == 1)
		p->child = NULL;
	else
		p->child = t->rsibling;
	p->degree -=1;
	t->rsibling = H->rootlist->rsibling;
	t->lsibling = H->rootlist;
	H->rootlist->rsibling->lsibling = t;
	H->rootlist->rsibling = t;
	H->rootlistcount+=1;
	t->parent = NULL;
	t->tag = 0;

	if(t->key < H->rootlist->key)
		H->rootlist=t;
}
void CascadingCut(Heap *H, HeapNode *p)
{
	HeapNode *z=p->parent;
	if(z != NULL)
	{	if(p->tag == 0)
			p->tag=1;
		else
		{	Cut(H,p,z);
			CascadingCut(H,z);
		}
	}
}
void DecreaseKey(Heap *H, HeapNode *t, int k)
{
	HeapNode *p;
	if(t->key < k)
		cout<<endl<<"New key greater than current key...Key value unchanged";
	t->key = k;
	p=t->parent;
	if(p != NULL && t->key < p->key)
		{
		Cut(H, t, p);
		CascadingCut(H, p);
		}
	if(t->key < H->rootlist->key)
		H->rootlist = t;
}
void Delete(Heap *H,HeapNode *x)
{
	int temp = FindMin(H) - 1;
	DecreaseKey(H,x,temp);
	DeleteMin(H);
}
int PrintMenu()
{
cout<<endl<<"*******************************************************************";
cout<<endl<<"0. Exit			5. DeleteMin";
cout<<endl<<"1. MakeHeap		6. DecreaseKey";
cout<<endl<<"2. Insert Key		7. Delete(Atrbitrary)";
cout<<endl<<"3. FindMin		8. PrintHeap";
cout<<endl<<"4. MergeHeaps		9. Check Memory";
cout<<endl<<"*******************************************************************";
cout<<endl<<"For Testing :		T. Test Node.";
cout<<endl<<"*******************************************************************";
};
void Display(char opt)
{
int x,j,k,l;
switch(opt)
{	
	case '1' :
		if(hn<MAXHEAPS)
			FibHeapArray[hn]=MakeHeap();
		else if(hn==MAXHEAPS)
			cout<<endl<<"All "<<(MAXHEAPS-1)<<" Heap Exists. New Heaps can not be created";
		break;
	case '2' :
		if(hn ==1)
		{
			cout<<endl<<"No Heap Exists. Make a heap First.";
			break;
		}
		else
		{	cout<<endl<<"Following heaps exists. In which heap do you want insertion ? "<<endl;
			for(int i=1; i<MAXHEAPS; i++)
			{	if(FibHeapArray[i] !=NULL)
					cout<<i<<endl;
			}
			cin>>j;
		}
		if(hn<j or j <1 or FibHeapArray[j]==NULL)
		{	cout<<endl<<"Heap "<<j<<" does not exists.";
			break;
		}
		else
		{	do
			{
				cout<<endl<<"Enter the key to be inserted in Heap "<<j<<". Enter -1 to exit. ";
				cin>>x;
				if(x==(-1))
				{	PrintHeap(FibHeapArray[j]);
					break;
				}
				else
				if(FibHeapArray[j]!=NULL)
					Insert(FibHeapArray[j],x);
				else
					cout<<"Wrong Heap";
			}while(nodecount != 0);
		}
		if (nodecount==0)
		{	cout<<endl<<"All nodes consumed. No more insertion possible.";
			PrintHeap(FibHeapArray[j]);
		}
		break;
	case '3' :
		if(hn == 1)
		{
			cout<<endl<<"No Heap Exists. Make a heap First.";
			break;
		}
			cout<<endl<<"Following heaps exists. In which heap do you want FindMin ? "<<endl;
			for(int i=1; i<MAXHEAPS; i++)
				if(FibHeapArray[i] !=NULL)
					cout<<i<<endl;
			cin>>j;
		
		if(hn<j or j<1 or FibHeapArray[j]==NULL)
		{	cout<<endl<<"Heap "<<j<<" does not exists.";
			break;
		}
		else
			cout<<endl<<"Minimum Value in the Heap "<<j<<" is "<<FindMin(FibHeapArray[j]);
			break;
	case '4' :
		if(hn <= 2)
		{
			cout<<"Two Heaps does not exists. Make two heaps First.";
			break;
		}
			cout<<endl<<"Following heaps exists. Select two Heaps for Merge operation. "<<endl;
			for(int i=1; i<MAXHEAPS; i++)
				if(FibHeapArray[i] !=NULL)
					cout<<i<<endl;
			
		cout<<endl<<"Enter the First Heap : ";
		cin>>j;
		cout<<endl<<"Enter the Second Heap : ";
		cin>>k;
		if(hn<j or hn< k or j<1 or FibHeapArray[j]==NULL)
		{	cout<<endl<<"Either Heap "<<j<<" or Heap "<<k<<"or both does not exists.";
			break;
		}
		else if(hn==MAXHEAPS)
			cout<<endl<<"New Heap can not be created hence Merging is not possible";
		else
			FibHeapArray[hn] = MergeHeaps(FibHeapArray[j],FibHeapArray[k]);
			FibHeapArray[j]=NULL;
			FibHeapArray[k]=NULL;
			PrintHeap(FibHeapArray[hn]);
			hn=hn+1;
		break;
	case '5' :
		if(hn == 1)
		{
			cout<<"No Heap Exists..";
			break;
		}
		else
		{
			cout<<endl<<"Following heaps exists. In which heap do you want Deletemin ? "<<endl;
			for(int i=1; i<MAXHEAPS; i++)
				if(FibHeapArray[i] !=NULL)
			cout<<i<<endl;
		}		cin>>j;
		
		if(hn<j or j<1 or FibHeapArray[j]==NULL)
		{	cout<<endl<<"Heap "<<j<<" does not exists.";
			break;
		}
		else
			DeleteMin(FibHeapArray[j]);
			PrintHeap(FibHeapArray[j]);
		break;
	case '6' :
		if(hn == 1)
		{
			cout<<endl<<"No Heap Exists. Make a heap first.";
			break;
		}
		else
		{	cout<<endl<<"Following heaps exists. In which heap do you want DecreaseKey ? "<<endl;
			for(int i=1; i<MAXHEAPS; i++)
				if(FibHeapArray[i] !=NULL)
					cout<<i<<endl;
		}	
			cin>>j;
		
		if(hn<j or j<1 or FibHeapArray[j]==NULL)
		{	cout<<endl<<"Heap "<<j<<" does not exists.";
			break;
		}
		else
			PrintHeap(FibHeapArray[j]);
			cout<<endl<<"Which Key value do you want to decrease ? ";
			cin>>k;
			if(k>MAXKEY)
			{
				cout<<endl<<k<<" is not allowable key.";
				break;
			}
			else if (SearchArray[k] == NULL)
			{	cout<<endl<<"Key does not exists.";
				break;
			}
			cout<<endl<<"Enter New Value.";
			cin>>l;
			DecreaseKey(FibHeapArray[j],SearchArray[k],l);
			PrintHeap(FibHeapArray[j]);
		break;
		
	case '7' :
		if(hn == 1)
		{
			cout<<endl<<"No Heap Exists. Make a heap first.";
			break;
		}
		else
		{
			cout<<endl<<"Following heaps exists. In which heap do you want Delete[Arbitrary] ? "<<endl;
			for(int i=1; i<MAXHEAPS; i++)
				if(FibHeapArray[i] !=NULL)
			cout<<i<<endl;
		}
			cin>>j;
		
		if(hn<j or j<1 or FibHeapArray[j]==NULL)
		{	cout<<endl<<"Heap "<<j<<" does not exists.";
			break;
		}
		else
			PrintHeap(FibHeapArray[j]);
			cout<<endl<<"Which Key value do you want to Delete ? ";
			cin>>k;
			if(k>MAXKEY)
			{
				cout<<endl<<k<<" is not allowable key.";
				break;
			}
			else if (SearchArray[k] == NULL)
			{	cout<<endl<<"Key does not exists. ";
				break;
			}
			Delete(FibHeapArray[j],SearchArray[k]);
			PrintHeap(FibHeapArray[j]);
		break;
	
	case '8' :
		if(hn==1)
		{	cout<<endl<<"No Heap Exists";
			break;
		}
		else
		{	cout<<endl<<"Following heaps exists. Which Heap do you want to print ? "<<endl;
			for(int i=1; i<MAXHEAPS; i++)
				if(FibHeapArray[i] !=NULL)
				cout<<i<<endl;
		}
		cin>>j;
		if(hn<j or FibHeapArray[j]==NULL)
		{	cout<<" Heap "<<j<<" does not exists.";
			break;
		}
		else
			PrintHeap(FibHeapArray[j]);
		break;
	case 'T':
		if(hn==1)
		{	cout<<endl<<"No Heap Exists";
			break;
		}
		cout<<endl<<"Enter the key of the node you want to print";
		cin>>j;
		if(SearchArray[j] != NULL)
			PrintNode(SearchArray[j]);
		else
			cout<<"Key Does Not Exists";
		break;
	case '9':
		cout<<endl<<"****************************";
		cout<<endl<<"    Status of Memory";
		cout<<endl<<"****************************";
		cout<<endl<<"Maximum Nodes Alllowed : "<<na;
		cout<<endl<<"Nodes Available        : "<<nodecount;
		cout<<endl<<"Nodes Consumed         : "<<(na - nodecount);
		cout<<endl<<"Maximum Heaps Allowed  : "<<MAXHEAPS-1;
		cout<<endl<<"No. of Heaps Consumed  : "<<hn-1;
		cout<<endl<<"No. of Heaps Available : "<<MAXHEAPS-hn;
		cout<<endl<<"****************************";
		break;
	case '0' :
		exit(1);
		break;
	default :
		break;
}
}

void InputFile()
{
	int i = 0,j,k,l;
	char temp[15];
	fstream fin;
	fin.open("input.txt");
	while(fin.eof() != 1)
	{
	fin >> temp;
		if(strcmp(temp,"nodes")==0)
		{
		fin >> n;
			na=n;
			CreateNodes(n);
		}
		if(strcmp(temp,"makeheap")==0)
		{
			fin >> j;
			if(j!=hn)
			{	cout<<endl<<"Invalid Heap Sequence No. Please Check whether Heap No. are in sequence in Input File.";
				break;}
			else if(hn<MAXHEAPS)
				FibHeapArray[hn]=MakeHeap();
			else if(hn==MAXHEAPS)
				cout<<endl<<"All "<<(MAXHEAPS-1)<<" Heap Exists. New Heaps can not be created";
		
		}
		if(strcmp(temp,"insert")==0)
		{	
			fin >>j;
			fin>>k;
			if(hn ==1)
			{
				cout<<endl<<"No Heap Exists. Make a heap First.";
				break;
			}
			else if(hn<j or j <1 or FibHeapArray[j]==NULL)
			{	cout<<endl<<"Insertion Not Possible. Heap "<<j<<" does not exists.";
				break;
			}
			else if (nodecount==0)
				cout<<endl<<"All nodes consumed. No more insertion possible.";
			else 
			{	cout<<endl<<"Inseting Key "<<k<<" in heap "<<j<<"."; 
				Insert(FibHeapArray[j],k);
			}
		}
		if(strcmp(temp,"findmin")==0)
		{	fin >>j;
			if(hn == 1)
			{
				cout<<endl<<"No Heap Exists. Make a heap First.";
				break;
			}
					
			if(hn<j or j<1 or FibHeapArray[j]==NULL)
			{	cout<<endl<<"Heap "<<j<<" does not exists.";
				break;
			}
			else
			cout<<endl<<"Minimum Value in the Heap "<<j<<" is "<<FindMin(FibHeapArray[j]);
		}
		if(strcmp(temp,"deletemin")==0)
		{	fin >>j;
			if(hn == 1)
			{
				cout<<"No Heap Exists..";
				break;
			}
			else
			if(hn<j or j<1 or FibHeapArray[j]==NULL)
			{	cout<<endl<<"Heap "<<j<<" does not exists.";
				break;
			}
			else
				cout<<endl<<"Performing deletemin in Heap "<<j<<". ";
				DeleteMin(FibHeapArray[j]);
		}
		if(strcmp(temp,"mergeheaps")==0)
		{	fin >>j;
			fin >>k;
			if(hn <= 2)
			{
				cout<<"Two Heaps does not exists. Make two heaps First.";
				break;
			}
			if(hn<j or hn< k or j<1 or FibHeapArray[j]==NULL)
			{	cout<<endl<<"Either Heap "<<j<<" or Heap "<<k<<"or both does not exists.";
				break;
			}
			else if(hn==MAXHEAPS)
				cout<<endl<<"New Heap can not be created hence Merging is not possible";
			else
				FibHeapArray[hn] = MergeHeaps(FibHeapArray[j],FibHeapArray[k]);
				FibHeapArray[j]=NULL;
				FibHeapArray[k]=NULL;
				cout<<"Following Heap "<<hn<<" is created after merging heaps "<<j<<" and "<<k<<".";
				hn=hn+1;
		}
		if(strcmp(temp,"decreasekey")==0)
		{	fin >>j;
			fin >>k;
			fin >>l;

		if(hn == 1)
			{
			cout<<endl<<"No Heap Exists. Make a heap first.";
			break;
		}
				
		if(hn<j or j<1 or FibHeapArray[j]==NULL)
		{	cout<<endl<<"Heap "<<j<<" does not exists.";
			break;
		}
		else
			if(k>MAXKEY)
			{
				cout<<endl<<k<<" is not allowable key.";
				break;
			}
			else if (SearchArray[k] == NULL)
			{	cout<<endl<<"Key does not exists.";
				break;
			}
			cout<<endl<<"Decreasing Key value "<<k<<" of Heap "<<j<<" to Key value "<<l<<" .";
			DecreaseKey(FibHeapArray[j],SearchArray[k],l);
		}
		if(strcmp(temp,"delete")==0)
		{	fin >>j;
			fin >>k;
			if(hn == 1)
			{
				cout<<endl<<"No Heap Exists. Make a heap first.";
				break;
			}
			if(hn<j or j<1 or FibHeapArray[j]==NULL)
			{	cout<<endl<<"Heap "<<j<<" does not exists.";
				break;
			}
			if(k>MAXKEY)
			{
				cout<<endl<<k<<" is not allowable key.";
				break;
			}
			else if (SearchArray[k] == NULL)
			{	cout<<endl<<"Key does not exists. ";
				break;
			}
				cout<<"Deleteing key "<<k<<" from the heap "<<j<<".";
				Delete(FibHeapArray[j],SearchArray[k]);
		}
		if(strcmp(temp,"print")==0)
		{	fin >>j;
			if(hn==1)
			{	cout<<endl<<"No Heap Exists";
				break;
			}
			if(hn<j or FibHeapArray[j]==NULL)
			{	cout<<" Heap "<<j<<" does not exists.";
				break;
			}
			else
				PrintHeap(FibHeapArray[j]);
		}
		if(strcmp(temp,"test")==0)
		{	fin >>k;
			if(SearchArray[k]==NULL)
			{	cout<<endl<<"Key "<<k<<" does not exist in any of the heaps.";
				break;
			}
			else
				PrintNode(SearchArray[k]);
		}
		if(strcmp(temp,"memory")==0)
		{	cout<<endl<<"****************************";
			cout<<endl<<"    Status of Memory";
			cout<<endl<<"****************************";
			cout<<endl<<"Maximum Nodes Alllowed : "<<na;
			cout<<endl<<"Nodes Available        : "<<nodecount;
			cout<<endl<<"Nodes Consumed         : "<<(na - nodecount);
			cout<<endl<<"Maximum Heaps Allowed  : "<<MAXHEAPS-1;
			cout<<endl<<"No. of Heaps Consumed  : "<<hn-1;
			cout<<endl<<"No. of Heaps Available : "<<MAXHEAPS-hn;
			cout<<endl<<"****************************";
		}
	}
}

void InputConsole()
{
int n;
char opt;
if(nodecreatedflag==0)
	{
	cout<<"How Many Nodes Do You want To Create ?";
	cin>>na;
	CreateNodes(na);
	nodecreatedflag=1;
	}
PrintMenu();
cout<<endl<<"Enter Your Choice.";
cin>>opt;
Display(opt);
InputConsole();
}

void Input()
{
char a;
cout<<endl<<"*******************************************************************";
cout<<endl<<"                     Input Mode of Input ";
cout<<endl<<"*******************************************************************";
cout<<endl<<"1. Console";
cout<<endl<<"2. File";
cout<<endl<<"*******************************************************************";
cout<<endl;
cin>>a;
switch(a)
{	case '1' :
		InputConsole();
		break;
	case '2' :
		InputFile();
		break;		
	default :
		Input();
}
}
//********************************Input Interface********************************************

int main()
{	
	nodecreatedflag =0; hn=1;
	for(int i=0; i<MAXKEY; i++)
		SearchArray[MAXKEY]=NULL;
	
	Input();
	return EXIT_SUCCESS;
}

