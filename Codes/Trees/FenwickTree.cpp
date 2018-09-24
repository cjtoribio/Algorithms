struct Fenwick {
    vector<Long> V;
    int hb;
    Fenwick(int N) : V(N+10) {
      hb = 1;
      while (hb*2 < V.size()) {
        hb <<= 1;
      }
    }

    void add(int x, Long v) {
        for (int i = x + 1; i < V.size(); i += (i & -i)) {
            V[i] += v;
        }
    }
    Long get(int x) {
        Long r = 0;
        for (int i = x + 1; i > 0; i -= (i & -i)) {
            r += V[i];
        }
        return r;
    }
    Long lowerBound(Long val) {
      int ans = 0;
      for (int i = hb; i > 0; i >>= 1) {
        int nans = ans | i;
        if (nans < V.size() && V[nans] < val) {
          ans = nans;
          val -= V[ans];
        }
      }
      return ans;
    }
    Long segSum(int i, int j) {
        return get(j) - get(i-1);
    }
};
