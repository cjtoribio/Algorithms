template<class T, class U>
struct Node {
    Node *l, *r;
    int b,e;
    T sum;
    Node(int b, int e):l(NULL),r(NULL),b(b),e(e), sum() { }
    Node* clone() {
        Node *n = new Node(*this);
        return n;
    }
    Node operator+(const Node &n) const {
        Node r(b, n.e);
        r.sum = sum + n.sum;
        return r;
    }
    static Node* create(int b, int e) {
        Node *r = new Node(b, e);
        if (b == e) return r;
        r->l = create(b, (b+e)/2);
        r->r = create((b+e)/2+1, e);
        return r;
    }
    Node* update(int i, int j, U val) {
        Node* n = clone();
        if(i <= b && e <= j){
            n->sum += val;
            return n;
        }else{
            int m = (b+e)/2;
            if(m >= i) n->l = l->update(i, j, val);
            if(m <  j) n->r = r->update(i, j, val);
            n->sum = n->l->sum + n->r->sum;
            return n;
        }
    }
    T query(int i, int j) {
        if(i <= b && e <= j){
            return sum;
        }else{
            int m = (b+e)/2;
            if(m >= j) return l->query(i, j);
            if(m <  i) return r->query(i, j);
            return l->query(i, j) + r->query(i, j);
        }
    }
};
