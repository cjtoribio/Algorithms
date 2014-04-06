#include <vector>
#include <iostream>
#include <cstdio>
#include <cmath>
using namespace std;

typedef long long Long;
struct BigInteger
{
	static const int FACTOR = 1000000000;
	static const int DSZ = 9;
	vector<Long> V;
	BigInteger(Long n = 0){
		do
		{
			V.push_back(n % FACTOR);
			n /= FACTOR;
		}while(n > 0);
	}
	BigInteger operator<<(const int n)const{
		BigInteger ret;
		int m = n % DSZ , d = n/ DSZ;
		vector<Long> newV( V.size() + d + 2 , 0 );
		for(int i = 0; i < V.size(); ++i){
			newV[i+d] = V[i];
			for(int j = 0; j < m; ++j)
				newV[i] *= 10;
		}
		ret.V = newV;
		ret.adjust();
		return ret;
	}
	BigInteger sMult(const BigInteger &B)const{
		BigInteger ret;
		ret.V = vector<Long>(V.size() + B.V.size() , 0);
		for(int i = 0; i < V.size(); ++i){
			for(int j = 0; j < B.V.size(); ++j){
				ret.V[i+j] += B.V[j] * V[i];
			}
			for(int j = i; j < ret.V.size(); ++j)
			{
				Long d = ret.V[j] / FACTOR;
				if(j+1 < ret.V.size()) ret.V[j+1] += d;
				else if(d)
				{
					ret.V.push_back(d);
				}
				ret.V[j] %= FACTOR;
			}
		}
		while(ret.V.back() == 0)
			ret.V.pop_back();
		return ret;
	}
	BigInteger operator*(const BigInteger &B)const{
		if(B.V.size() <= 200 || V.size() <= 200)
			return (*this).sMult(B);
		int SZ = max(V.size()/2 , B.V.size()/2);
		SZ = min(SZ , (int)B.V.size() - 1);
		SZ = min(SZ , (int)V.size() - 1);
		vector<Long> vA1(V.begin(),V.begin()+SZ);
		vector<Long> vA2(V.begin()+SZ,V.end());
		vector<Long> vB1(B.V.begin(),B.V.begin()+SZ);
		vector<Long> vB2(B.V.begin()+SZ,B.V.end());
		BigInteger A1,A2,B1,B2;
		A1.V = vA1;
		A2.V = vA2;
		B1.V = vB1;
		B2.V = vB2;
		BigInteger Z2 = (A2*B2);
		BigInteger Z0 = (A1*B1);
		BigInteger Z1 = ((A1+A2) * (B1+B2)) - Z2 - Z0;
		return (((Z2<<(SZ*2*DSZ)) + (Z1<<(SZ*DSZ))) + Z0);
	}
	BigInteger operator+(const BigInteger &B)const{
		BigInteger ret;
		ret.V = vector<Long>(max(V.size() , B.V.size()) + 1,0);
		for(int i = 0; i < max(V.size(),B.V.size()); ++i){
			if(i < V.size())ret.V[i] += V[i];
			if(i<B.V.size())ret.V[i] += B.V[i];
		}
		ret.adjust();
		return ret;
	}
	BigInteger operator-(const BigInteger &B)const{
		BigInteger ret;
		ret.V = vector<Long>(max(V.size() , B.V.size()) + 1,0);
		int c = 0;
		for(int i = 0; i < max(V.size(),B.V.size()); ++i){
			Long d = V[i] - c;
			if(i < B.V.size())d -= B.V[i];
			if(d < 0)d += FACTOR , c = 1;
			else c = 0;
			ret.V[i] = d;
		}
		ret.adjust();
		return ret;
	}
	Long rawDivide(const vector<Long> &A,const vector<Long> &B)const{
		Long _A = 0 , _B = 0;
		int eq = 0;
		if(A.size() == B.size())
			_A = A.back() , _B = B.back() , eq = 1;
		else if(A.size() > B.size())
			_A = A.back() * FACTOR + A[A.size() - 2] , _B = B.back();
		else
			return 0;
		
		if(B.size() > 1){
			char AA[12],BB[12];
			sprintf(AA,"%0*lld",DSZ,A[A.size()-3+eq]);
			sprintf(BB,"%0*lld",DSZ,B[B.size()-2]);
			int idx = 0;
			while(_B < FACTOR / 10){
				_A = 10 * _A + (AA[idx]-'0');
				_B = 10 * _B + (BB[idx]-'0');
				idx++;
			}
		}
		return max( 0LL , _A / _B - 1 );
	}
	pair<BigInteger,BigInteger> divide(const BigInteger &B)const{
		BigInteger Q;
		BigInteger R;
		for(int i = V.size()-1; i>=0 ; --i)
		{
			R = R << DSZ;
			Q = Q << DSZ;
			R = R + V[i];
			Long rawFactor = rawDivide(R.V,B.V);
			Q.V[0] += rawFactor;
			R = R - (B * rawFactor);
			int cnt = 0;
			while( !(R < B) ){
				R = R - B;
				cnt++;
				Q.V[0] += 1;
			}
		}
		return pair<BigInteger,BigInteger>(Q,R);
	}
	BigInteger operator/(const BigInteger &B)const{
		return divide(B).first;
	}
	BigInteger operator%(const BigInteger &B)const{
		return divide(B).second;
	}
	int operator<(const BigInteger &B)const{
		return (V.size() != B.V.size() ? V.size() < B.V.size() : V.back() < B.V.back());
	}
	int operator>(const BigInteger &B)const{
		return B < (*this);
	}
	int digits()const{
		if(V.size() == 0 && V.back() == 0)return 1;
		return (V.size()-1) * DSZ + log10(V.back()) + 1;	
	}
	void adjust(){
		for(int i = 0; i < V.size(); ++i)
		{
			Long d = V[i] / FACTOR;
			if(i+1 < V.size()) V[i+1] += d;
			else if(d) V.push_back(d);
			V[i] %= FACTOR;
		}
		while(V.size() > 1 && V.back() == 0)V.pop_back();
	}
	string str()const{
		string ret = "";
		for(int i = (int)V.size()-1 ; i >= 0 ; --i)
		{
			char num[12];
			sprintf(num,"%0*lld",(i+1==V.size()?0:DSZ),V[i]);
			int n = strlen(num);
			ret += num;
		}
		return ret;
	}
	
};
ostream& operator<<(ostream &o,const BigInteger &B){
	return o << B.str();
}
