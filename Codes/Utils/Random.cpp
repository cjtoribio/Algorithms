struct Rand {
	mt19937 gen;
	Rand(int SEED = 1234567):gen(SEED){}
	Rand(mt19937 gen):gen(gen){}
	
	int getInt(int mn, int mx){
		uniform_int_distribution<int> uDist(mn, mx);
		return uDist(gen);
	}
	double getNormal(double mean, double deviation){
		normal_distribution<double> nDist(mean, deviation);
		return nDist(gen);
	}
};