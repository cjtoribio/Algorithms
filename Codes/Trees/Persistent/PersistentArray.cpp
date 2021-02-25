template<class T>
struct PersistentArray {
    static VI L, R;
    static vector<T> V;
    int root, b, e;
    static int createLeafNode(const T &v) {
        V.push_back(v);
        return V.size() - 1;
    }
    static int createInternalNode(int l, int r) {
        L.push_back(l);
        R.push_back(r);
        return L.size() - 1;
    }
    PersistentArray(int root, int b, int e) : root(root), b(b), e(e) { }
    PersistentArray(const vector<T> &vals, int b = 0, int e = -1) : b(b), e(e) {
        if (e == -1) e = vals.size() - 1;
        this->e = e;
        if (b == e) {
            root = createLeafNode(vals[0]);
        } else {
            int m = (b + e) / 2;
            root = createInternalNode(PersistentArray(vals, b, m).root, PersistentArray(vals, m+1, e).root);
        }
    }
    T get(int i) const {
        return get(root, b, e, i);
    }
    T get (int v, int b, int e, int i) const {
        if (b == e) return V[v];
        int m = (b+e)/2;
        return i <= m ? get(L[v], b, m, i) : get(R[v], m+1, e, i);
    }
    PersistentArray<T> set(int i, const T &nv) {
        return set(root, b, e, i, nv);
    }
    PersistentArray<T> set (int v, int b, int e, int i, const T &nv) {
        if (b == e) return PersistentArray<T>(createLeafNode(nv), b, e);
        int m = (b+e)/2;
        int l = i <= m ? set(L[v], b, m, i, nv).root : L[v];
        int r = i >  m ? set(R[v], m+1, e, i, nv).root : R[v];
        return PersistentArray<T>(createInternalNode(l, r), b, e);
    }
    int size() const {
        return e - b + 1;
    }
};
template <typename T> vector<T> PersistentArray<T>::V = vector<T>();
template <typename T> vector<int> PersistentArray<T>::L = vector<int>();
template <typename T> vector<int> PersistentArray<T>::R = vector<int>();
