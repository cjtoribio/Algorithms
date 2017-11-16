#include <bits/stdc++.h>

using namespace std;

template<class T, class U>
struct LinkCutTree
{
	struct Node
	{
		Node *p,*r,*l;
		int sz,gid,id,csz;
		T edge;
		T path;
		U carry;
		Node(){
			static int IDX = 0;
			p = r = l = NULL;
			sz = 1; gid = IDX++; id=0;
			csz = 0;
			edge = path = T();
		}
		bool isLocalRoot(){
			return p == NULL || (p->r != this && p->l != this);
		}
		void pushDown(){
			if(l)l->addSize(csz);
			if(r)r->addSize(csz);
			if(l)l->apply(carry);
			if(r)r->apply(carry);
			if(r)r->path = path + r->edge;
			csz = 0;
			carry = U();
		}
		void update(){
		}
		void apply(U update){
			carry += update;
			edge += update;
			path += update;
		}
		void addSize(int sz){
			csz += sz;
			this->sz += sz;
		}
	};
	vector<Node> V; int N;
	LinkCutTree(int N) : N(N){
		V = vector<Node>(N);
		for (int i = 0; i < N; ++i)
			V[i].id = i;
	}
	void moveUp(Node *v){
		if(!v->p)return;
		Node *p = v->p;
		p->pushDown(); v->pushDown();
		Node *g = p->p;
		p->p = v;
		v->p = g;
		if(p->l == v){ p->l = v->r; v->r = p; if(p->l)p->l->p = p; }
		else         { p->r = v->l; v->l = p; if(p->r)p->r->p = p; } 
		if(g && (g->l == p || g->r == p))(g->l == p ? g->l : g->r) = v;
		p->update();v->update();
	}
	void splay(Node *v){
		v->pushDown();
		while(!v->isLocalRoot())
			moveUp(v);
	}
	void expose(int v){ _expose(&V[v]); }
	Node* _expose(Node *v){
		Node* last = NULL;
		for(Node *x = v; x ; x = x->p){
			splay(x);
			x->pushDown();
			x->r = last;
			x->update();
			last = x;
		}
		splay(v);
		return last;
	}
	void link(int u,int v, T val){ if(findRoot(u) != findRoot(v))_link(&V[u],&V[v], val); }
	void _link(Node *u,Node *v, T val){
		_expose(v);
		if (!v->l) {
			v->p = u;
			v->edge = val;
			_expose(v);
			v->l->addSize(v->sz);
			v->update();
		}
	}
	void cut(int v){ _cut(&V[v]); }
	void _cut(Node *v) {
		_expose(v);
		if (v->l) {
			v->l->addSize(-v->sz);
			v->l->p = NULL;
			v->path = T();
			v->l = NULL;
			v->update();
		}
	}
	int findRoot(int v){ return _findRoot(&V[v])->id; }
	Node* _findRoot(Node* v) {
		_expose(v);
		while (v->l)
			v = v->l;
		splay(v);
		return v;
	}
	int findLCA(int u,int v) { Node* lc = _findLCA(&V[u],&V[v]); return lc==NULL ? -1 : lc->id; }
	Node* _findLCA(Node* u, Node* v)
	{
		if (_findRoot(u) != _findRoot(v))return NULL;
		_expose(u);
		return _expose(v);
	}
	Node* find(int u) { _expose(&V[u]); return &V[u]; }
	void applyToPath(int u, U val){ applyToPath(&V[u], val); }
	void applyToPath(Node *v, U val){
		_expose(v);
		if(v->l) v->l->apply(val);
		v->apply(val);
		v->carry = U();
	}
	void print(){
		for(int i = 0; i < N; ++i)
			if(V[i].isLocalRoot()){
				print(&V[i]);
				cout << "-----------------------" << endl;
			}
	}
	void print(Node *v, int LVL = 0){
		if(v->r)print(v->r,LVL+1);
		cout << string(LVL,'\t') << v->id << "| " << v->sz << " " << (v->p ? v->p->id : -1) << " " << v->path.val << endl;
		if(v->l)print(v->l,LVL+1);
	}
};

struct Int {
	int val;
	Int(int v = 0):val(v){}
	Int operator+(const Int m)const{
		return Int(min(m.val, val));
	}
	void operator+=(int v){
		val += v;
	}
};



int main() {

	int N = 10;
	LinkCutTree<Int, int> T(N);
	T.V[0].path = 1000000;
	for (int i = 1; i < N; ++i) {
		int n = rand() % i;
		int c = rand() % 31;
		T.link(n, i, c);
		cout << n << " " << i << " " << c << endl;
	}
	T.print();
	for (int i = 0; i < N; ++i) {
		cout << i << ": " << T.find(i)->path.val  << endl;
	}
	T.applyToPath(3, -1);
	for (int i = 0; i < N; ++i) {
		cout << i << ": " << T.find(i)->path.val  << endl;
	}
	cout << endl;
	
	
	
	
}
