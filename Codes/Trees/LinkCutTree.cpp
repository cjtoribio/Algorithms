struct LinkCutTree
{
	struct Node
	{
		Node *p,*r,*l;
		int sz,gid,id;
		Node(){
			static int IDX = 0;
			p = r = l = NULL;
			sz = 1; gid = IDX++; id=0;
		}
		bool isLocalRoot(){
			return p == NULL || (p->r != this && p->l != this);
		}
		void pushDown(){
			
		}
		void update(){
			
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
		while(!v->isLocalRoot())
			moveUp(v);
	}
	void expose(int v){ _expose(&V[v]); }
	Node* _expose(Node *v){
		Node* last = NULL;
		for(Node *x = v; x ; x = x->p){
			splay(x);
			x->r = last;
			x->update();
			last = x;
		}
		splay(v);
		return last;
	}
	void link(int u,int v){ if(findRoot(u) != findRoot(v))_link(&V[u],&V[v]); }
	void _link(Node *u,Node *v){
		_expose(v);
		if (!v->l) {
			v->p = u;
			v->update();
		}
	}
	void cut(int v){ _cut(&V[v]); }
	void _cut(Node *v) {
		_expose(v);
		if (v->l) {
			v->l->p = NULL;
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