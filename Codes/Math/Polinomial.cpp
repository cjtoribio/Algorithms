struct Polinomial
{
	vector<double> V;
	Polinomial(int deg)
	{
		V = vector<double>(deg+1,0);
	}
	Polinomial(vector<double> V){
		this->V = V;
	}

	int degree() const {
		return int(V.size()) - 1;
	}
	double getC(int p) const {
		return p >= int(V.size()) ? 0 : V[p];
	}

	Polinomial operator+(const Polinomial &P)const{
		Polinomial ret(max(degree() , P.degree()));
		for(int i = 0; i <= ret.degree(); ++i)
			ret.V[i] = getC(i) + P.getC(i);
		return ret;
	}
	Polinomial operator-(const Polinomial &P)const{
		Polinomial ret(max(degree() , P.degree()));
		for(int i = 0; i <= ret.degree(); ++i)
			ret.V[i] = getC(i) - P.getC(i);
		return ret;
	}

	Polinomial derivate()const{
		Polinomial ret(degree()-1);
		for(int i = 0; i <= ret.degree(); ++i)
			ret.V[i] = (i+1) * V[i+1];
		return ret;
	}
	Polinomial integrate()const{
		Polinomial ret(degree()+1);
		for(int i = 1; i <= ret.degree(); ++i)
			ret.V[i] = V[i-1] / i;
		return ret;
	}

	double eval(double x)
	{
		double ret = 0;
		for(int i = degree(); i >= 0; i--)
			ret = ret * x + V[i];
		return ret;
	}

	vector<double> getZeros()
	{
		if(degree() == 0)return vector<double>();
		Polinomial D = this->derivate();
		vector<double> Z = D.getZeros();
		vector<double> R;
		for(int i = 0; i <= Z.size(); ++i)
		{
			double l = i == 0 ? -1e9 : Z[i-1];
			double r = i == Z.size() ? 1e9 : Z[i];
			double x = (l + r) / 2;
			for(int k = 0; k < 60; ++k){
				x = x - eval(x) / D.eval(x);
			}
			double fe = eval(x);
			if( -1e-6 <= fe && fe <= 1e-6 )
				R.push_back(x);
		}
		sort(R.begin(),R.end());
		vector<double> RET;
		for(int i = 0; i < R.size(); ++i)
		{
			if(i == 0 || R[i]-R[i-1] >= 1e-6 )
				RET.push_back(R[i]);
		}
		return RET;
	}
};