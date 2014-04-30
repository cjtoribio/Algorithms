//============================================================================
// Name        : HLD.cpp
// Author      : cjtoribio
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

struct Node
{
	int val, carry , sz;
	bool HasCarry;
	Node(int val = 0)
	{
		this->val = val;
		this->carry = 0;
		this->sz = 1;
		HasCarry = 0;
	}
	Node operator+(const Node &N)const {
		Node ret(val);
		ret.val = max(val , N.val);
		ret.carry = 0;
		ret.HasCarry = 0;
		ret.sz = this->sz + N.sz;
		return ret;
	}
	void update(int val)
	{
		this->carry += val;
		this->val += val;
	}
};
struct SegmentTree
{
	Node *V;
	int N;
	SegmentTree(int N)
	{
		this->V = new Node[4 * N];
		this->N = N;
	}
	~SegmentTree(){ delete [] this->V; }
	void create(vector<Node> &VEC,int n = 1,int b = 0,int e = -1)
	{
		if(e == -1)e = N - 1;
		if(b == e){
			V[n] = Node(VEC[b]);
		}
		else
		{
			create(VEC,2*n,b,(e+b)/2);
			create(VEC,2*n+1,(e+b)/2+1,e);
			V[n] = V[2*n] + V[2*n+1];
		}
	}
	Node query(int i,int j, int n = 1,int b = 0,int e = -1)
	{
		if(e == -1)e = N - 1;
		if(i <= b && e <= j)return V[n];
		else
		{
			if(V[n].HasCarry)
			{
				V[2*n  ].update(V[n].carry);
				V[2*n+1].update(V[n].carry);
				V[n].HasCarry = 0;
			}
			int mid = (b+e)/2;
			if(i > mid)return query(i,j,2*n+1,mid+1,e);
			if(j <=mid)return query(i,j,2*n,b,mid);
			return query(i,j,2*n,b,mid) + query(i,j,2*n+1,mid+1,e);
		}
	}
	void update(int i,int j,int v,int n = 1,int b=0,int e=-1)
	{
		if(e == -1)e = N - 1;
		if(i <= b && e <= j)
		{
			V[n].HasCarry = 1;
			V[n].update(v);
		}
		else if(i > e || j < b)
			return;
		else
		{
			if(V[n].HasCarry)
			{
				V[2*n  ].update(V[n].carry);
				V[2*n+1].update(V[n].carry);
				V[n].HasCarry = 0;
			}
			int mid = (b+e)/2;
			update(i,j,v,2*n,b,mid);
			update(i,j,v,2*n+1,mid+1,e);
			V[n] = V[2*n] + V[2*n+1];
		}

	}
};

//int main() {
//	int SZ = 10;
//	SegmentTree ST(SZ);
//	for(int i = 0; i < 10; ++i)
//	{
//		ST.update(i,i,(92384*i)%10);
//		cout << ST.query(5,8).val << endl;
//		for(int j = 0; j < SZ; ++j)
//			cout << ST.query(j,j).val << " ";
//		cout << endl;
//	}
//	return 0;
//}









