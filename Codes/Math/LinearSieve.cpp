VI sieve(int n, VI &lowestPrime){
    VI comp(n+1);
    lowestPrime.resize(n+1);
    VI prime;
    int q = 0;
    for (int i=2;i<=n;i++){
        if (!comp[i]){
            prime.push_back(i);
            lowestPrime[i]=i;
        }
        for (int j=0;j<prime.size() && prime[j]*i<=n;j++){
            comp[i*prime[j]]=1;
            lowestPrime[i*prime[j]]=prime[j];
            if(i%prime[j]==0)break;
        }
    }
    return prime;
}
