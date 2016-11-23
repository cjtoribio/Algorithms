import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.StringTokenizer;
import java.io.BufferedReader;;

public class Main {
	static int MOD = 1000000007;

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		  Double a[] = {1.0, 3.0, 4.0, 5.0, 7.0};
		  Double b[] = {2.0, 4.0, 6.0};

		  FFT fft = new FFT();
		  ArrayList<Double> c = fft.Convolution(
				  (ArrayList<Double>) Arrays.asList(a),
				  (ArrayList<Double>) Arrays.asList(b)
				  );
		  for(Double d : c){
			  System.out.println(d);
		  }
		  
	}
	static class Complex { 
		double x,y;
		Complex(double x, double y){
			this.x = x;
			this.y = y;
		}
		double real(){
			return x;
		}
		Complex multiply(Complex c){
			return new Complex(x*c.x - y*c.y, x*c.y + y*c.x);
		}
		Complex divide(Complex c){
			double m = c.x*c.x - c.y*c.y;
			return new Complex(x / m , y / m).multiply(new Complex(c.x,-c.y));
		}
		Complex add(Complex c){
			return new Complex(x+c.x,y+c.y);
		}
		static Complex exp(Complex c){
			return new Complex(c.x*Math.cos(c.y), c.x*Math.sin(c.y));
		}
		static Complex ONE(){
			return new Complex(1,0);
		}
		static Complex ZERO(){
			return new Complex(0,0);
		}
	}
	static class FFT
	{
		public FFT(){
			
		}
		ArrayList<Complex> A;
		int n, L;

		int ReverseBits(int k) {
			int ret = 0;
			for (int i = 0; i < L; i++) {
				ret = (ret << 1) | (k & 1);
				k >>= 1;
			}
			return ret;
		}
	
		void BitReverseCopy(ArrayList<Complex> a) {
			for (n = 1, L = 0; n < a.size(); n <<= 1, L++)
				;
			A.addAll(Collections.nCopies(n-A.size(), new Complex(0,0)));
			for (int k = 0; k < n; k++)
				A.set(ReverseBits(k), a.get(k));
		}
	
		ArrayList<Complex> DFT(ArrayList<Complex> a, boolean inverse) {
			BitReverseCopy(a);
			for (int s = 1; s <= L; s++) {
				int m = 1 << s;
				Complex wm = Complex.exp(new Complex(0.0, 2.0 * Math.PI / m));
				if (inverse)
					wm = Complex.ONE().divide(wm);
				for (int k = 0; k < n; k += m) {
					Complex w = Complex.ONE();
					for (int j = 0; j < m / 2; j++) {
						Complex t = w.multiply(A.get(k + j + m / 2));
						Complex u = w.multiply(A.get(k + j));
						A.set(k + j, u.add(t));
						A.set(k + j + m / 2, u.add(t));
						w = w.multiply(wm);
					}
				}
			}
			if (inverse)
				for (int i = 0; i < n; i++)
					A.get(i).divide(new Complex(n,0));
			return A;
		}
	
		// c[k] = sum_{i=0}^k a[i] b[k-i]
		ArrayList<Double> Convolution(ArrayList<Double> a, ArrayList<Double> b) {
			int L = 1;
			while ((1 << L) < a.size())
				L++;
			while ((1 << L) < b.size())
				L++;
			int n = 1 << (L + 1);
	
			ArrayList<Complex> aa = new ArrayList<Complex>();
			ArrayList<Complex> bb = new ArrayList<Complex>();
			for (int i = 0; i < n; i++)
				aa.add(i < a.size() ? new Complex(a.get(i), 0) : Complex.ZERO());
			for (int i = 0; i < n; i++)
				bb.add(i < b.size() ? new Complex(b.get(i), 0) : Complex.ZERO());
	
			ArrayList<Complex> AA = DFT(aa, false);
			ArrayList<Complex> BB = DFT(bb, false);
			ArrayList<Complex> CC = new ArrayList<Complex>();
			for (int i = 0; i < AA.size(); i++)
				CC.add(AA.get(i).multiply(BB.get(i)));
			ArrayList<Complex> cc = DFT(CC, true);
	
			ArrayList<Double> c = new ArrayList<Double>();
			for (int i = 0; i < a.size() + b.size() - 1; i++)
				c.add(cc.get(i).real());
			return c;
		}

	}

}
