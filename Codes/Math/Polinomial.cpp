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
	Polinomial& set(int pos,int c)
	{
		V[pos] = c;
		return *this;
	}

	int degree() const {
		return int(V.size()) - 1;
	}

	Polinomial operator+(const Polinomial &P)const{
		Polinomial ret(max(degree() , P.degree()));
		for(int i = 0; i <= ret.degree(); ++i)
		{
			if(i < V.size())ret.V[i] += V[i];
			if(i < P.V.size())ret.V[i] += P.V[i];
		}
		return ret;
	}
	Polinomial operator-(const Polinomial &P)const{
		Polinomial ret(max(degree() , P.degree()));
		for(int i = 0; i <= ret.degree(); ++i)
		{
			if(i < V.size())ret.V[i] += V[i];
			if(i < P.V.size())ret.V[i] -= P.V[i];
		}
		return ret;
	}

	Polinomial derivate()const{
		Polinomial ret(degree()-1);
		for(int i = 0; i <= ret.degree(); ++i)
		{
			ret.V[i] = (i+1) * V[i+1];
		}
		return ret;
	}

	double eval(double x)
	{
		double ret = 0;
		for(int i = degree(); i >= 0; i--)
		{
			ret *= x;
			ret += V[i];
		}
		return ret;
	}

	vector<double> getZeros()
	{
		if(degree() == 0)return vector<double>();
		vector<double> Z = this->derivate().getZeros();
		Printer::print(V);
		Printer::print(Z);
		cout << endl;
		vector<double> R;
		for(int i = 0; i <= Z.size(); ++i)
		{
			double l = i == 0 ? -1e9 : Z[i-1];
			double r = i == Z.size() ? 1e9 : Z[i];
			l -= 1e-7;
			r += 1e-7;
			int positive = eval(l) < eval(r);
			while( r-l > 1e-6 ){
				double m = (l+r)/2;
				double e = eval(m);
				if((positive && e > 0) || (!positive && e < 0))
					r = m;
				else
					l = m;

			}
			double fe = eval((l+r)/2);
			if( -1e-5 <= fe && fe <= 1e-5 )
				R.push_back((l+r)/2);
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