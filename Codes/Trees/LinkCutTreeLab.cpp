struct LinkCutTree
{
	struct Node
	{
		Node *p,*r,*l;
		int sz,gid,id;
		int edgeCost , upCost;
		Node(){
			static int IDX = 0;
			p = r = l = NULL;
			sz = 1; gid = IDX++; id=0; edgeCost = 0 , upCost = 0;
		}
		bool isLocalRoot(){
			return p == NULL || (p->r != this && p->l != this);
		}
		void pushDown(){
			if(r)r->upCost = upCost + edgeCost;
		}
		void update(){
			
		}
		int getCost(){ return upCost + edgeCost; }
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
		while(!v->isLocalRoot())
			moveUp(v);
	}
	void expose(int v){ _expose(&V[v]); }
	void _expose(Node *v){
		for(Node *x = v, *y = NULL; x ; x = x->p){
			splay(x);
			x->r = y;
			x->update();
			y = x;
		}
		splay(v);
	}
	void link(int u,int v,int w){ if(findRoot(u) != findRoot(v))_link(&V[u],&V[v],w); }
	void _link(Node *u,Node *v, int w){
		_expose(v);
		if (!v->l) {
			v->p = u;
			v->edgeCost = w;
			v->update();
		}
	}
	void cut(int v){ _cut(&V[v]); }
	void _cut(Node *v) {
		_expose(v);
		if (v->l) {
			v->l->p = NULL;
			v->l = NULL;
			v->upCost = 0;
			v->edgeCost = 0;
			v->update();
		}
	}
	int findRoot(int v){ return _findRoot(&V[v])->id; }
	int findRootCost(int v) { return _findRoot(&V[v])->getCost(); }
	Node* _findRoot(Node* v) {
		_expose(v);
		while (v->l)
			v = v->l;
		splay(v);
		return v;
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
		cout << string(LVL,'\t') << v->id << "|" << (v->p ? v->p->id : -1) << endl;
		if(v->l)print(v->l,LVL+1);
	}
};
struct BruteForceTree
{
	vector<int> P;
	vector<int> C;
	int N;
	BruteForceTree(int N) : N(N) {
		P = vector<int>(N , -1);
		C = vector<int>(N , 0);
	}
	void link(int u,int v, int w){
		if(P[v] != -1 || findRoot(u) == findRoot(v))return;
		P[v] = u;
		C[v] = w;
	}
	void cut(int v){
		P[v] = -1;
	}
	int findRoot(int v){
		while(P[v] != -1)v = P[v];
		return v;
	}
	int findCostRoot(int v){
		int c = 0;
		while(P[v] != -1)c += C[v], v = P[v];
		return c;
	}
	void print(){
		for (int i = 0; i < N; ++i) {
			cout << P[i] << " ";
		}
		cout << endl;
	}
};