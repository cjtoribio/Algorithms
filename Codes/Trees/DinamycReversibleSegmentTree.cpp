#include <cmath>
#include <vector>
#include <iostream>
#include <queue>
using namespace std;

struct SplayTree {
	struct Node {
		Node *p, *r, *l;
		int sz,gid, id,rev;
		int val , pending , ma , carr;
		Node() {
			static int IDX = 0;
			p = r = l = NULL;
			gid = IDX++;
			rev = 0;
			id = 0;
			sz = 1;
			val = pending = ma = carr = 0;
		}
		bool isRoot() {
			return p == NULL;
		}
		void pushDown() {
			if(rev){
				swap(r,l);
				if(r)r->rev ^= 1;
				if(l)l->rev ^= 1;
				rev = 0;
			}
			if(pending){
				if(r)r->applyChange(carr);
				if(l)l->applyChange(carr);
				pending = 0;
				carr = 0;
			}
		}
		void applyChange(int v){
			val += v;
			carr += v;
			ma += sz * v;
			pending = 1;
		}
		void update() {
			sz = 1 + (r?r->sz:0) + (l?l->sz:0);
			ma = val;
			if(r)ma += r->ma;
			if(l)ma += l->ma;
		}
	};
	Node* root;
	vector<Node> V;
	int N;
	SplayTree(int N) :
			N(N) {
		V = vector<Node>(N);
		root = &V[0];
		for (int i = 0; i < N; ++i){
			V[i].id = i;
			V[i].sz = N-i;
			if(i+1<N){
				V[i].r = &V[i+1];
				V[i+1].p = &V[i];
			}
		}
	}
	void moveUp(Node *v) {
		if (!v->p)
			return;
		Node *p = v->p;
		p->pushDown();
		v->pushDown();
		Node *g = p->p;
		p->p = v; v->p = g;
		if (p->l == v) { p->l = v->r; v->r = p; if (p->l) p->l->p = p; } 
		else           { p->r = v->l; v->l = p; if (p->r) p->r->p = p; }
		if (g && (g->l == p || g->r == p))
			(g->l == p ? g->l : g->r) = v;
		else root = v; 
		p->update();
		v->update();
	}
	void splay(Node *v) {
		v->pushDown();
		while (!v->isRoot())
			moveUp(v);
	}
	Node* operator[](int k){
		Node *v = root;
		v->pushDown();
		while(k != (v->l?v->l->sz:0)){
			if(k < (v->l?v->l->sz:0)){
				v = v->l;
			}else{
				k -= (v->l?v->l->sz:0)+1; 
				v = v->r;
			}
			v->pushDown();
		}
		splay(v);
		return v;
	}
	void applyChange(int i,int j,int v){
		Node* J = (*this)[j];
		if(J->r)(*this)[j+1];
		if(i != 0)(*this)[i-1];
		J->applyChange(v);
	}
	void reverseB(Node *B){
		splay(B);
		B->pushDown();
		if(B->r) {
			Node *r = B->r; r->pushDown();
			while(r->l){
				r = r->l; r->pushDown();
			}
			splay(r);
		}
		B->rev ^= 1;
	}
	void reverse(int i,int j){
		if(i == 0){
			Node* J = (*this)[j];
			reverseB(J);
		}else{
			Node* J = (*this)[j];
			reverseB(J);
			Node* I = (*this)[j-i];
			reverseB(I);
			J = (*this)[j];
			reverseB(J);
		}
	}
	void print(Node* v, int LVL = 0){
		v->pushDown();
		if(v->r)print(v->r,LVL+1);
		cout <<string(LVL,'\t')<< v->id << "|" << v->sz << endl;
		if(v->l)print(v->l,LVL+1);
	}
	void print(){ print(root); }
};

int main(){
	SplayTree V(80);
	V.applyChange(10,20,3);
	V.applyChange(15,30,7);
	V.reverse(0,79);
	V.applyChange(0,79,2);
	for(int i = 0; i < 80; ++i)
		cout << V[i]->val << " ";
	cout << endl;
}









