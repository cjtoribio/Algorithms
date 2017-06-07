#include <vector>
#include <iostream>
#include <cstdio>
using namespace std;
struct Matrix
{
#define MOD 1000000007
	vector<vector<int> > MAT;
	Matrix(int N,int M)
	{
		MAT = vector<vector<int> >(N, vector<int>(M,0));
	}
	static Matrix identity(int N)
	{
		Matrix A(N,N);
		for(int i = 0; i < N; ++i)
			A[i][i] = 1;
		return A;
	}
	Matrix operator+(const Matrix &M)const
	{
		Matrix A(this->rows() , M.cols());
		for(int i = 0; i < this->rows(); ++i)
		{
			for(int j = 0; j < this->cols(); ++j)
			{
				A[i][j] = (MAT[i][j] + M.MAT[i][j]) % MOD;
			}
		}
		return A;
	}
	Matrix operator*(const Matrix &M)const
	{
		Matrix A(this->rows() , M.cols());
		if(this->cols() != M.rows())return A;
		for(int i = 0; i < this->rows(); ++i)
		{
			for(int j = 0; j < M.cols(); ++j)
			{
				A[i][j] = 0;
				for(int k = 0; k < this->cols(); ++k)
				{
					Long m = 1LL*MAT[i][k] * M.MAT[k][j];
					m %= MOD;
					if(m < 0)m += MOD;
					A[i][j] = (A[i][j] + m) % MOD;
				}
			}
		}
		return A;
	}
	Matrix operator*(int &n)
	{
		Matrix A(this->rows() , this->cols());
		for(int i = 0; i < this->rows(); ++i)
		{
			for(int j = 0; j < this->cols(); ++j)
			{
				A[i][j] = (1LL * n * (*this)[i][j]) % MOD;
			}
		}
		return A;
	}
	Matrix transponse()const{
		Matrix r(cols(), rows());
		for(int i = 0; i < rows(); ++i)
			for(int j = 0; j < cols(); ++j)
				r[j][i] = MAT[i][j];
		return r;
	}
	Matrix pow(long long n)
	{
		Matrix A = identity((*this).rows());
		long long hb = 1;
		while(hb < n)hb <<= 1;
		for(long long b = hb; b >= 1; b>>=1)
		{
			A = A * A;
			if (b & n) A = A * (*this);
		}
		return A;
	}
	vector<int>& operator[](int i){return MAT[i];}
	int rows()const{return MAT.size();}
	int cols()const{return MAT[0].size();}
	string toString()
	{
		string ans = "{\n";
		for(int i = 0; i < this->rows(); ++i)
		{
			ans += "[";
			for(int j = 0; j < this->cols(); ++j)
			{
				char arr[30];
				string app = (j==0?"":" ");
				sprintf(arr,"%d",(*this)[i][j]);
				app += arr;
				ans += app;
			}
			ans += "]\n";
		}
		ans += "}";
		return ans;
	}
#undef MOD
};