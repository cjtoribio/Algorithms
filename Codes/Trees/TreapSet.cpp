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
    void split (Node *t, const function<bool(const Node*)> &goLeft, Node *&l, Node *&r) {
        if (!t)l = r = NULL;
        else if ( goLeft(t) )
            split (t->l, goLeft, l, t->l), r = t;
        else
            split (t->r, goLeft, t->r, r), l = t;
        if(t)t->update();
    }
    void split (Node *t, const T &x, Node *&l, Node *&r) {
        return split(t, [x](const Node *t) { return x < t->x; }, l, r);
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
    T end(Node *t) {
        while (t->r) t = t->r; return t->x;
    }
public:
    Treap(){
        root = NULL;
    }
    void clear(){ clear(root);}
    int size() {
        return root == NULL ? 0 : root->sz;
    }
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
    T operator[](int i) {
        Node *l, *r;
        auto splitLeft = [&i](const Node *t) {
            int lsz = t->l ? t->l->sz : 0;
            if (lsz > i) return true;
            i -= lsz + 1;
            return false;
        };
        split(root, splitLeft, l, r);
        T ret = end(l);
        merge(root, l, r);
        return ret;
    }
    int lowerBound(const function<bool(const T &t)> gte) {
        Node *l, *r;
        auto splitLeft = [&gte](const Node *t) {
            if (gte(t->x)) return true;
            return false;
        };
        split(root, splitLeft, l, r);
        int c = (l ? l->sz : 0);
        merge(root, l, r);
        return c;
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
