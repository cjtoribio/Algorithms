template<class T>
struct Node {
    int l, r;
    T val;
    Node() : l(-1), r(-1), val() {}
};

template<class T, class U>
struct PersistentSegmentTree {
    vector<Node<T>> nodes;
    int B, E;
    PersistentSegmentTree(int B, int E) : B(B), E(E) {}
    int setMaxMemory(int maxMemory) {
        nodes.reserve(max((int)(maxMemory / sizeof(Node<T>)), 20));
    }
    int newNode() {
        int id = nodes.size();
        nodes.push_back(Node<T>());
        return id;
    }
    int create() { return create(B, E); }
    int create(int b, int e) {
        int id = newNode();
        if (b == e) return id;
        int m = (b+e)/2;
        nodes[id].l = create(b, m);
        nodes[id].r = create(m+1, e);
        return id;
    }
    int update(int oid, int i, int j, const U &u) { return update(oid, i, j, u, B, E); }
    int update(int oid, int i, int j, const U &u, int b, int e) {
        int id = newNode();
        nodes[id] = nodes[oid];
        if (b == e) {
            u(nodes[id].val);
        } else {
            int m = (b + e) / 2;
            if (j <= m) nodes[id].l = update(nodes[oid].l, i, j, u, b, m);
            if (m < i) nodes[id].r = update(nodes[oid].r, i, j, u, m + 1, e);
            nodes[id].val = nodes[nodes[id].l].val + nodes[nodes[id].r].val;
        }
        return id;
    }
    T query(int id, int i, int j) { return query(id, i, j, B, E); }
    T query(int id, int i, int j, int b, int e) {
        if (i <= b && e <= j) {
            return nodes[id].val;
        } else {
            int m = (b + e) / 2;
            if (j <= m) return query(nodes[id].l, i, j, b, m);
            if (m < i) return query(nodes[id].r, i, j, m + 1, e);
            return query(nodes[id].l, i, j, b, m) + query(nodes[id].r, i, j, m+1, e);
        }
    }
};
