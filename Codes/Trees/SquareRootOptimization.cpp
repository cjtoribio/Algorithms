#include <cmath>
#include <vector>
#include <iostream>
#include <queue>
using namespace std;

struct Node{
	int val;
	Node(int val = 0){
		this->val = val;
	}
	void update(int v){
		val += v;
	}
	Node operator+(const Node &N){
		return Node(max(val,N.val));
	}
};
struct Group {
	Node *V;
	int SIZE;
	int ma, carry;
	Group(){ carry = -1; }
	~Group(){ delete[] V; }
	int size(){ return SIZE; }
	void create(vector<int> V){
		this->V = new Node[SIZE = V.size()];
		for(int i = 0; i < SIZE; ++i)
			this->V[i] = Node(V[i]);
	}
	void pushDown(){
		if(carry == -1)return;
		for(int i = 0; i < SIZE; ++i)
			V[i].update(carry);
		carry = -1;
	}
	void rebuild(){
		ma = 0;
		for(int i = 0; i < SIZE; ++i)
			ma = max(ma , V[i].val);
	}
	void update(int v){
		ma = max(v , ma);
		carry = v;
	}
	void update(int i, int v){
		V[i].update(v);
	}
	Node query(){
		return Node(ma);
	}
	Node query(int i){
		return V[i];
	}
};
struct SqrtOptimization
{
#define GSIZE 1000
	Group *GS;
	int N;
	SqrtOptimization(int N){
		this->N = N;
		int GCNT = N / GSIZE + (N % GSIZE != 0);
		GS = new Group[GCNT];
		this->create(vector<int>(N,0));
	}
	~SqrtOptimization(){ delete[] GS; }
	void create(const vector<int> &V){
		int GCNT = N / GSIZE + (N % GSIZE != 0);
		for (int i = 0; i < GCNT; ++i) {
			vector<int> SUB_V(V.begin()+i*GSIZE, V.begin()+min((i+1)*GSIZE,N));
			GS[i].create(SUB_V);
		}
	}
	void update(int i,int j,int v){
		int gi = i/GSIZE , gj = j/GSIZE, pi = i % GSIZE, pj = j % GSIZE;
		GS[gi].pushDown(); GS[gj].pushDown();
		while(pi && pi < GSIZE && gi < gj)
			GS[gi].update(pi++,v) , i++;
		GS[gi].rebuild();
		gi = i/GSIZE;
		while(gi < gj) 
			GS[gi++].update(v) , i += GSIZE;
		pi = i % GSIZE;
		while(pi<= pj) 
			GS[gi].update(pi++,v) , i++;
		GS[gj].rebuild();
	}
	Node query(int i,int j){
		int gi = i/GSIZE , gj = j/GSIZE, pi = i % GSIZE, pj = j % GSIZE;
		GS[gi].pushDown(); GS[gj].pushDown();
		vector<Node> TV; TV.reserve(2*GSIZE);
		while(pi && pi < GSIZE && gi < gj)
			TV.push_back(GS[gi].query(pi++)) , i++;
		gi = i/GSIZE;
		while(gi < gj) 
			TV.push_back(GS[gi++].query()) , i += GSIZE; 
		pi = i % GSIZE;
		while(pi<= pj)  
			TV.push_back(GS[gi].query(pi++)) , i++;
		Node ret = TV[0];
		for(int g = 1; g < TV.size(); ++g) ret = ret + TV[g];
		return ret;
	}
};

int main(){
	SqrtOptimization S(100);
	for(int i = 0; i < 10; ++i){
		S.update(0,i*5,1);
		for(int j = 0; j < 50; ++j){
			cout << S.query(j,j).val << " ";
		}
		cout << ": " << S.query(10,99).val << endl;
	}
	for(int i = 9; i >= 0; --i){
		S.update(0,i*5,-1);
		for(int j = 0; j < 50; ++j){
			cout << S.query(j,j).val << " ";
		}
		cout << ": " << S.query(10,99).val << endl;
	}
}
