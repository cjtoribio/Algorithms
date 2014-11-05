#include <cmath>
#include <vector>
#include <iostream>
#include <queue>
using namespace std;

struct SortedSplayTree {
	struct Node {
		Node *p, *r, *l;
		int sz,gid;
		int val;
		Node(int v) {
			static int IDX = 0;
			p = r = l = NULL;
			gid = IDX++;
			sz = 1;
			val = v;
		}
		bool isRoot() {
			return p == NULL;
		}
		void update() {
			sz = 1 + (r?r->sz:0) + (l?l->sz:0);
		}
		inline int leftSize(){ return l ? l->sz : 0; }
	};
	Node* root;
	SortedSplayTree(){ root = NULL; }
	int size(){ return root == NULL ? 0 : root->sz; }
	void moveUp(Node *v) {
		if (!v->p)
			return;
		Node *p = v->p;
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
		while (!v->isRoot())
			moveUp(v);
	}
	Node* operator[](int k){
		Node *v = root;
		while(k != v->leftSize()){
			if(k < v->leftSize()){
				v = v->l;
			}else{
				k -= v->leftSize() + 1; 
				v = v->r;
			}
		}
		splay(v);
		return v;
	}
	int countL(int v){
		Node *r = root , *p = NULL;
		int cnt = 0;
		while( r != NULL ){
			p = r;
			if(r->val >= v) r = r->l;
			else {
				cnt += r->leftSize() + 1;
				r = r->r;
			}
		}
		if(p)splay(p);
		return cnt;
	}
	void insert(int v){
		Node* nn = new Node(v);
		if(root == NULL){
			root = nn; return;
		}
		Node *r = root , *p = NULL;
		while( r != NULL ){
			p = r; p->sz++; 
			if(r->val >= v) r = r->l;
			else            r = r->r;
		}
		(p->val >= v  ? p->l : p->r) = nn;
		nn->p = p;
		splay(nn);
	}
	void print(Node* v, int LVL = 0){
		if(v->r)print(v->r,LVL+1);
		cout <<string(LVL,'\t')<< v->gid << "|" << v->sz << endl;
		if(v->l)print(v->l,LVL+1);
	}
	void print(){ print(root); }
};

int main(){
	SortedSplayTree SST;
	SST.insert(20);
	SST.insert(10);
	SST.insert(20);
	cout << SST.countL(10) << endl;
	SST.print();
}