template<class T>
class Treap {
	struct Node{
		int y,sz;
		T x;
		Node *l, *r;
		Node(T x):x(x){
			y = rand();
			sz = 1;
			l = r = NULL;
		}
		void update(){
			sz = 1 + (r?r->sz:0) + (l?l->sz:0);
		}
	};
	Node *root;
	void split (Node *t, T x, Node *&l, Node *&r) {
		if (!t)l = r = NULL;
		else if ( x < t->x )
			split (t->l, x, l, t->l), r = t;
		else
			split (t->r, x, t->r, r), l = t;
		if(t)t->update();
	}

	void insert (Node *&t, Node *it) {
		if(!t)t = it;
		else if(it->y > t->y){
			split(t, it->x, it->l, it->r), t = it;
		}else{
			insert(it->x < t->x ? t->l : t->r, it);
		}
		t->update();
	}
	void erase(Node *&t, T x){
		if(t->x == x){
			Node *old = t;
			merge(t, t->l, t->r);
			delete old;
		}else
			erase(x < t->x ? t->l : t->r, x);
		if(t)t->update();
	}
	void merge (Node *&t, Node *l, Node *r) {
		if (!l || ! r){
			t = l? l: r;
		}else if (l->y > r->y){
			merge (l->r, l->r, r);
			t = l;
		}else{
			merge (r->l, l, r->l);
			t = r;
		}
		if(t)t->update();
	}
public:
	Treap(){
		root = NULL;
	}
	void clear(){ clear(root);}
	void clear(Node *&t){
		if(!t)return;
		clear(t->l);
		clear(t->r);
		delete t;
		t = NULL;
	}
	void insert(T x){
		Node *nn = new Node(x);
		insert(root, nn);
	}
	void erase(T x){
		if(!root)return;
		erase(root, x);
	}
	int countLowerEqual(T x){
		if(!root)return 0;
		Node *l, *r;
		split(root, x, l, r);
		int cnt = l?l->sz:0;
		merge(root, l, r);
		return cnt;
	}
	void print(bool asArray = false){
		if(asArray)printArray(root);
		else       printTree(root);
		cout << endl;
	}
	void printArray(Node *p, int LVL = 0){
		if(!p)return;
		printArray(p->l, LVL+1);
		cout << (p->x) << " ";
		printArray(p->r, LVL+1);
	}
	void printTree(Node *p, int LVL = 0){
		if(!p)return;
		printTree(p->r, LVL+1);
		cout <<string(LVL*3,' ')<< (p->x) << endl;
		printTree(p->l, LVL+1);
	}
	
};