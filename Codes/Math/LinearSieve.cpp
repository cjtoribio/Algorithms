struct LinearSieve {
  VI P, C;
  LinearSieve(int L): C(L+1) { 
    for (int i = 2; i < C.size(); ++i) {
      if (C[i] == 0) {
          C[i] = i;
          P.push_back (i);
      }
      for (int j=0; j < P.size() && P[j]<=C[i] && i*P[j]<=L; ++j)
          C[i * P[j]] = P[j];
    }
  }
  VI factors(int n) {
    VI F;
    while (n > 1) F.push_back(C[n]), n /= C[n];
    return F;
  }
  void rec(vector<int> &V, int tp, int id, int c, VI &A) {
    if (id == V.size()) {
      A.push_back(c);
    } else {
      rec(V, 0, id+1, c, A);
      if (tp == 1 || V[id] != V[id-1])
        rec(V, 1, id+1, c * V[id], A);
    }
  }
  VI divisors(int n) {
    VI f = factors(n), R;
    rec(f, 1, 0, 1, R);
    sort(R.begin(), R.end());
    return R;
  }
};
