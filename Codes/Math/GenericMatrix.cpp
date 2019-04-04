template<class T>
struct Matrix
{
	struct VirtualVector{
		vector<T> &R;
		int off;
		VirtualVector(vector<T> &R, const int off):R(R),off(off){}
		T& operator[](int k){
			return R[off+k];
		}
	};
	vector<T> MAT;
	int N,M;
	Matrix(int N,int M)
	{
		this->N = N;
		this->M = M;
		MAT = vector<T>(N*M);
	}
	static Matrix<T> identity(int N)
	{
		Matrix<T> A(N,N);
		for(int i = 0; i < N; ++i)
			A[i][i] = 1;
		return A;
	}
	Matrix<T> operator+(Matrix<T> &M)
	{
		Matrix<T> A(this->rows() , M.cols());
		for(int i = 0; i < this->rows(); ++i)
		{
			for(int j = 0; j < this->cols(); ++j)
			{
				A[i][j] = ((*this)[i][j] + M[i][j]);
			}
		}
		return A;
	}
	Matrix<T> operator*(Matrix<T> &MA)
	{
		Matrix<T> A(this->rows() , MA.cols());
		if(this->cols() != MA.rows())return A;
		for(int i = 0; i < this->rows(); ++i)
		{
			for(int j = 0; j < MA.cols(); ++j)
			{
				A[i][j] = 0;
				for(int k = 0; k < this->cols(); ++k)
				{
					A[i][j] = (A[i][j] + MAT[i*M + k] * MA[k][j]);
				}
			}
		}
		return A;
	}
	Matrix<T> operator*(int &n)
	{
		Matrix<T> A(this->rows() , this->cols());
		for(int i = 0; i < this->rows(); ++i)
		{
			for(int j = 0; j < this->cols(); ++j)
			{
				A[i][j] = (1LL * n * (*this)[i][j]);
			}
		}
		return A;
	}
	Matrix<T> pow(int n)
	{
		Matrix<T> A = identity((*this).rows());
		for(int b = (1<<30); b >= 1; b>>=1)
		{
			A = A * A;
			if(b & n)A = A * (*this);
		}
		return A;
	}
	VirtualVector operator[](int i){
		return VirtualVector(MAT, i*M);
	}
	int rows(){return N;}
	int cols(){return M;}
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
};
