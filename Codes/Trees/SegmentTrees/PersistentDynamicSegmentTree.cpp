template <class T, class U>
struct Treap {
    struct Node{
        int y, sz;
        T val, tot;
        U carr;
        bool pending, rev, vL, vR;
        Node *l, *r;
        ~Node() {
            if (l) delete l;
            if (r) delete r;
        }
        Node(T val = T()) : val(val), tot(val), carr() {
            y = rand() % 1000000000;
            sz = 1;
            l = r = NULL;
            pending = rev = vL = vR = false;
        }
        Node* clone() {
            Node* ret = new Node(*this);
            ret->vL = !!l;
            ret->vR = !!r;
            return ret;
        }
        int x(){
            return (l?l->sz:0);
        }
        void push(){
            if (vR && r) r = r->clone(), vR = false;
            if (vL && l) l = l->clone(), vL = false;
            if (rev) {
                swap(vL, vR);
                swap(r,l);
                if (r) r->rev ^= 1;
                if (l) l->rev ^= 1;
                rev = 0;
            }
            if (pending) {
                if (r) r->applyChange(carr);
                if (l) l->applyChange(carr);
                pending = false;
                carr = U();
            }
        }
        void update(){
            sz = 1 + (r?r->sz:0) + (l?l->sz:0);
            tot = val;
            if (l) tot = l->tot + tot;
            if (r) tot = tot + r->tot;
        }
        void applyChange(const U &v){
            v(tot);
            v(val);
            carr += v;
            pending = true;
        }
    };
    Node *root;
    Treap(int N){
        root = NULL;
        build(root, 0, N-1);
    }
    void build(Node *&t, int i, int j) {
        if (j<i) {
            t = NULL;
            return;
        } else if(i==j) {
            t = new Node();
        } else {
            int m = (i+j)/2;
            t = new Node();
            build(t->l, i,m-1);
            build(t->r, m+1,j);
        }
        t->update();
    }
    // splits the tree t with elements with x() less than or equal x
    void split (Node *t, int x, Node *&l, Node *&r, bool mutate = true) {
        if (t) t->push();
        if (t && mutate) t = t->clone();
        if (!t) l = r = NULL;
        else if ( x < t->x() )
            split (t->l, x, l, t->l, mutate), r = t;
        else
            split (t->r, x - t->x() - 1, t->r, r, mutate), l = t;
        if (t) t->update();
    }
    void insert (Node *&t, int x, Node *it) {
        Node *t1, *t2;
        split(t, x, t1, t2);
        merge(t1, t1, it);
        merge(t, t1, t2);
    }
    void merge (Node *&t, Node *l, Node *r) {
        if (!l || ! r) {
            t = l? l: r;
        } else if (l->y > r->y) {
            l->push();
            merge (l->r, l->r, r), t = l;
        } else {
            r->push();
            merge (r->l, l, r->l), t = r;
        }
        if (t) t->update();
    }
    void reverse(int i, int j){
        Node *l, *m, *r;
        split(root, j, l, r);
        split(l, i-1, l, m);
        m->rev ^= 1;
        merge(l, l, m);
        merge(root, l, r);
    }
    void insert(int i, T val) {
        Node *l, *r, *n = new Node(val);
        split(root, i-1, l, r);
        merge(l, l, n);
        merge(root, l, r);
    }
    void remove(int i, int j) {
        Node *l, *r, *d;
        split(root, j, l, r);
        split(l, i-1, l, d);
//        delete d;
        merge(root, l, r);
    }
    void applyChange(int i, int j, U v){
        Node *l, *m, *r;
        split(root, j, l, r);
        split(l, i-1, l, m);
        m->applyChange(v);
        merge(l, l, m);
        merge(root, l, r);
    }
    T query(int i, int j){
        Node *l, *m, *r;
        split(root, j, l, r, false);
        split(l, i-1, l, m, false);
        T ret = m->tot;
        merge(l, l, m);
        merge(root, l, r);
        return ret;
    }
    int size() { return root ? root->sz : 0; }
    void print(){
        print(root);
        cout << endl;
    }
    void print(Node *p, int LVL = 0){
        if(!p)return;
        p->push();
        print(p->l, LVL+1);
        cout << (p->val.print()) << "";
        print(p->r, LVL+1);
    }
};
