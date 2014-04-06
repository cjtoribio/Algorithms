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
	Matrix operator+(Matrix &M)
	{
		Matrix A(this->rows() , M.cols());
		for(int i = 0; i < this->rows(); ++i)
		{
			for(int j = 0; j < this->cols(); ++j)
			{
				A[i][j] = (MAT[i][j] + M[i][j]) % MOD;
			}
		}
		return A;
	}
	Matrix operator*(Matrix &M)
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
					A[i][j] = (A[i][j] + 1LL*(*this)[i][k] * M[k][j]) % MOD;
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
	Matrix pow(long long n)
	{
		Matrix A = identity((*this).rows());
		for(long long b = (1LL<<62); b >= 1; b>>=1)
		{
			A = A * A;
			if(b & n)A = A * (*this);
		}
		return A;
	}
	vector<int>& operator[](int i){return MAT[i];}
	int rows(){return MAT.size();}
	int cols(){return MAT[0].size();}
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
