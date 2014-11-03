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
		return Node(val + N.val);
	}
};
struct Group {
	vector<Node> V;
	int sum, carry;
	int size(){ return V.size(); }
	void create(vector<int> V){
		this->V.reserve(V.size());
		for(int i = 0; i < V.size(); ++i)
			this->V.push_back(Node(V[i]));
	}
	void pushDown(){
		for(int i = 0; i < V.size(); ++i)
			V[i].update(carry);
		carry = 0;
	}
	void rebuild(){
		sum = 0;
		for(int i = 0; i < V.size(); ++i)
			sum += V[i].val;
	}
	void update(int v){
		sum += v;
		carry += v;
	}
	void update(int i, int v){
		V[i].update(v);
	}
	Node query(){
		return sum;
	}
	Node query(int i){
		return V[i];
	}
};
struct SqrtOptimization
{
#define GSIZE 1000
	vector<Group> GS;
	int N;
	SqrtOptimization(int N){
		this->N = N;
		int GCNT = N / GSIZE + (N % GSIZE != 0);
		GS = vector<Group>(GCNT);
		for (int i = 0; i < GCNT; ++i) {
			int sz = i != GCNT-1 ? GSIZE : N - GCNT*(i);
			GS[i].create(vector<int>(sz,0));
		}
	}
	void create(const vector<int> &V){
		int GCNT = N / GSIZE + (N % GSIZE != 0);
		for (int i = 0; i < GCNT; ++i) {
			vector<int> SUB_V(V.begin()+i*GSIZE, V.begin()+min((i+1)*GSIZE,N));
			GS[i].create(SUB_V);
		}
	}
	void update(int i,int j,int v){
		int gi = i/GSIZE;
		int gj = j/GSIZE;
		if(i % GSIZE && gi != gj){
			GS[gi].pushDown();
			for(int g = i%GSIZE; g < GS[gi].size(); ++g){
				GS[gi].update(g,v);
			}
			GS[gi].rebuild();
			i = (gi+1)*GSIZE;
		}
		while(gi != gj){
			GS[gi].update(v);
			gi++;
			i += GSIZE;
		}
		GS[gi].pushDown();
		for(int g = i; g <= j; ++g){
			GS[gi].update(g%GSIZE , v);
		}
		GS[gi].rebuild();
	}
	Node query(int i,int j){
		int gi = i/GSIZE;
		int gj = j/GSIZE;
		vector<Node> TV;
		TV.reserve(GSIZE);
		int cnt = 0;
		if(i % GSIZE && gi != gj){
			GS[gi].pushDown();
			for(int g = i%GSIZE; g < GS[gi].size(); ++g){
				TV.push_back(GS[gi].query(g));
			}
		}
		while(gi != gj){
			TV.push_back(GS[gi].query());
			gi++;
			i += GSIZE;
		}
		GS[gi].pushDown();
		for(int g = i%GSIZE; g <= j%GSIZE; ++g){
			TV.push_back(GS[gi].query(g));
		}
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
		cout << ": " << S.query(0,100).val << endl;
	}
	for(int i = 9; i >= 0; --i){
		S.update(0,i*5,-1);
		for(int j = 0; j < 50; ++j){
			cout << S.query(j,j).val << " ";
		}
		cout << ": " << S.query(0,100).val << endl;
	}
}
