Long discreteLogarithm(Long base, Long x, Long mod) {
    assert(x > 0);
    if (x == 1) return 0;
    Long n = (int) sqrt (mod) + 1;
    Long an = 1;
    for (int i = 0; i<n; ++i)
        an = (an * base) % mod;
    unordered_map<Long, Long> value;
    for (Long i = 1, cur = an; i<= n; ++i) {
        if (value.count(cur) == 0)
            value[ cur ] = i;
        cur = (cur * an) % mod;
    }
    for (Long i = 0, cur = x; i<= n; ++i) {
        if (value.count(cur)) {
            Long ans = value[cur] * n - i;
            if (ans < mod)
                return ans;
        }
        cur = (cur * base) % mod;
    }
    return -1;
}
