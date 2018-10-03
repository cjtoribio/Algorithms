struct LinearSieve {
  VI primes, primeFactor;
  LinearSieve(int L): primeFactor(L+1) { 
    for (int i = 2; i < primeFactor.size(); ++i) {
      if (primeFactor[i] == 0) {
          primeFactor[i] = i;
          primes.push_back (i);
      }
      for (int j=0; j < primes.size() && primes[j]<=primeFactor[i] && i*primes[j]<=L; ++j)
          primeFactor[i * primes[j]] = primes[j];
    }
  }
  VI factors(int n) {
    VI F;
    while (n > 1) F.push_back(primeFactor[n]), n /= primeFactor[n];
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
