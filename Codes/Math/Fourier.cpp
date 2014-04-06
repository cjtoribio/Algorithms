#include <vector>
#include <complex>

using namespace std;
struct Fourier
{
	typedef vector<complex<double> > VCD;
	typedef complex<double> CD;
	typedef long long Long;
	 
	#define MAXN 1000000
	static void four1(double *data, Long nn) {
		Long n, mmax, m, j, istep, i;
		double wtemp, wr, wpr, wpi, wi, theta;
		double tempr, tempi;

		// reverse-binary reindexing
		n = nn << 1;
		j = 1;
		for (i = 1; i < n; i += 2) {
			if (j > i) {
				swap(data[j - 1], data[i - 1]);
				swap(data[j], data[i]);
			}
			m = nn;
			while (m >= 2 && j > m) {
				j -= m;
				m >>= 1;
			}
			j += m;
		};
		// here begins the Danielson-Lanczos section
		mmax = 2;
		while (n > mmax) {
			istep = mmax << 1;
			theta = -(2 * M_PI / mmax);
			wtemp = sin(0.5 * theta);
			wpr = -2.0 * wtemp * wtemp;
			wpi = sin(theta);
			wr = 1.0;
			wi = 0.0;
			for (m = 1; m < mmax; m += 2) {
				for (i = m; i <= n; i += istep) {
					j = i + mmax;
					tempr = wr * data[j - 1] - wi * data[j];
					tempi = wr * data[j] + wi * data[j - 1];
					data[j - 1] = data[i - 1] - tempr;
					data[j] = data[i] - tempi;
					data[i - 1] += tempr;
					data[i] += tempi;
				}
				wtemp = wr;
				wr += wr * wpr - wi * wpi;
				wi += wi * wpr + wtemp * wpi;
			}
			mmax = istep;
		}
	}
	static void _FFT(CD *V, Long &N)
	{
		 while(N ^ (N & -N))
		 {
			 V[N].real() = 0;
			 V[N].imag() = 0;
			 N++;
		 }
		 double *A_T = new double[2*N + 10];
//			double A_T[6*MAXN];
		 for(int i = 0; i < N; ++i)
		 {
			 A_T[i+i] = V[i].real();
			 A_T[i+i+1]=V[i].imag();	 }
		 four1(A_T,N);
		 for(int i = 0; i < N; ++i)
		 {
			 V[i].real() = A_T[i+i];
			 V[i].imag() = A_T[i+i+1];	 	
		 }
		 delete A_T;
	}
	static void _IFFT(CD *V , Long &N)
	{
		 for(int i = 0; i < N; ++i)
			 V[i].imag() *= -1;
		 _FFT(V , N);
		 for(int i = 0; i < N; ++i)
		 {
			 V[i].imag() *= -1;
			 V[i] *= 1.0/double(N);
		 }
	}
	static void doNothing(CD *CV, const int SZ){}
	static vector<double> FFT(vector<double> V , void (*process)(CD *CV,const int SZ) = doNothing)
	{
		Long SZ = V.size();
		CD *CV = new CD[2 * V.size()];
		for(int i = 0; i < V.size(); ++i)
			CV[i].real() = V[i] , CV[i].imag() = 0;
		_FFT(CV , SZ);
		process(CV,SZ);
		_IFFT(CV , SZ);
		for(int i = 0; i < V.size(); ++i)
			V[i] = CV[i].real();
		return V;
	}
};
