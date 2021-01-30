template<class Holder, class Q, class O, class U>
struct Mo {
    struct Query { int l, r, t, id; Q q; };
    struct Update { int l, r; U u; };
    int N;
    vector<Query> queries;
    vector<Update> updates;
    int GRP_SIZE;
    Moh(int N): N(N) {
        GRP_SIZE = 2000;
    }
    void addQuery(int l, int r, Q q) {
        queries.push_back((Query){l, r, (int)updates.size(), (int)queries.size(), q});
    }
    void addUpdate(int l, int r, U u) {
        updates.push_back((Update){l, r, u});
    }
    vector<O> processAll(Holder &h) {
        sort(queries.begin(), queries.end(), [&](const Query &a, const Query &b){
            if (updates.size() == 0)
                return make_pair(a.l / GRP_SIZE, a.r) < make_pair(b.l / GRP_SIZE, b.r);
            else
                return make_tuple(a.l / GRP_SIZE, a.r / GRP_SIZE, a.t) < make_tuple(b.l / GRP_SIZE, b.r / GRP_SIZE, b.t);
        });
        int s = 0, e = -1, t = 0;
        vector<O> R(queries.size());
        for (auto &q : queries) {
            while (t < q.t) h.process(updates[t].l, updates[t].r, updates[t].u), t++;
            while (t > q.t) t--, h.undo(updates[t].l, updates[t].r);
            while (e < q.r) h.addPos(++e);
            while (s > q.l) h.addPos(--s);
            while (e > q.r) h.removePos(e--);
            while (s < q.l) h.removePos(s++);
            R[q.id] = h.get(q.q);
        }
        return R;
    }
};
 
#define MAXVAL 100010
#define MAXFREQ 100010 // 100010
struct FreqTracker {
    VI NEXT = VI(MAXFREQ, -1), PREV = VI(MAXFREQ, -1);
    VI FREQ = VI(MAXVAL, 0);
    VI VAL = VI(MAXFREQ, 0);
    FreqTracker() {
        VAL[0] = MAXFREQ;
        NEXT[0] = NEXT.size() - 1;
        PREV[NEXT.size()-1] = 0;
    }
    void updateVal(int v, int delta) {
        modifyFreq(FREQ[v], delta);
        FREQ[v] += delta;
    }
    void modifyFreq(int f, int delta) {
        VAL[f]--;
        VAL[f+delta]++;
        if (delta == +1) {
            if (NEXT[f] != f+1) {
                int oldNext = NEXT[f];
                NEXT[f] = f+1;
                NEXT[f+1] = oldNext;
                PREV[f+1] = f;
                PREV[oldNext] = f+1;
            }
        } else if (delta == -1) {
            if (PREV[f] != f-1) {
                int oldPrev = PREV[f];
                PREV[f] = f-1;
                PREV[f-1] = oldPrev;
                NEXT[f-1] = f;
                NEXT[oldPrev] = f-1;
            }
        }
        if (VAL[f] == 0) {
            NEXT[PREV[f]] = NEXT[f];
            PREV[NEXT[f]] = PREV[f];
        }
    }
    vector<PII> getAll() {
        vector<PII> VF;
        for (int i = NEXT[0]; i < NEXT.size()-1; i = NEXT[i]) {
            VF.push_back({i, VAL[i]});
        }
        return VF;
    }
};
struct Freqs {
    VI V;
    int cl = 0, cr = -1;
    FreqTracker FT;
    Freqs(VI V): V(V) {}
    vector<int> updates;
    void process(int l, int r, int u) {
        updates.push_back(V[l]);
        if (cl <= l && r <= cr) FT.updateVal(V[l], -1);
        V[l] = u;
        if (cl <= l && r <= cr) FT.updateVal(V[l], +1);
    }
    void undo(int l, int r) {
        if (cl <= l && r <= cr) FT.updateVal(V[l], -1);
        V[l] = updates.back(); updates.pop_back();
        if (cl <= l && r <= cr) FT.updateVal(V[l], +1);
    }
    void addPos(int p) {
        if (p < cl) cl--;
        if (p > cr) cr++;
        FT.updateVal(V[p], +1);
    }
    void removePos(int p) {
        if (p == cl) cl++;
        if (p == cr) cr--;
        FT.updateVal(V[p], -1);
    }
    int get(int k) {
        auto T = FT.getAll();
        int best = -1;
        for (int i = 0, j = -1, s = 0; i < T.size(); ++i) {
            while (j+1 < T.size() && s < k) {
                s += T[j+1].second;
                j++;
            }
            if (s >= k) {
                best = (best == -1) ? T[j].first - T[i].first : min(T[j].first - T[i].first, best);
            }
            s -= T[i].second;
        }
        return best;
    }
};
