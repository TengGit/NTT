// Number theory transform

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define DIGIT 3
#define X     1000
#define N (1 << 13)
#define L 5002
int a[N], b[N], c[N], temp[N];
char buffer[L];

#define P      ((27 << 26) + 1) /* Mathematica Table[PrimeQ[a*2^26+1],{a,1,32}] */
#define PR     136              /* See report */
#define PR_POW 26               /* pow(136, 2 << PR_POW) % P == 1 */
#define N_REV  1811939302       /* (N_REV * pow(2, PR_POW)) % P == 1, see report */

int add(long long x, long long y, int mod) {
	return (x + y) % mod;
}

int multiply(long long x, long long y, int mod) {
	return (x * y) % mod;
}

int square(long long x, int mod) {
	return multiply(x, x, mod);
}

int fpow(long long base, size_t expo, int mod) {
	long long coeff = 1;
	if (expo == 0) return 1;
	while (expo > 1) {
		if (expo & 1) {
			coeff = multiply(coeff, base, mod);
		}
		base = square(base, mod);
		expo >>= 1;
	}
	return multiply(base, coeff, mod);
}

void reverse(int *first, int *last) {
	--last;
	while (first < last) {
		int temp =  *first;
		*first++ = *last;
		*last-- = temp;
	}
}

/* [1,2,3,4,...,n] -> [1,3,...,n-1,2,4,...,n] */
void cross(int *first, size_t len, int *assist) {
	int *end = first + len;
	int *even_first = first;
	int *odd_first = assist;
	int *temp = first;
	++first;
	while (first < end) {
		*assist++ = *first;
		first += 2;
	}
	while (even_first < end) {
		*temp++ = *even_first;
		even_first += 2;
	}
	
	while (temp < end) {
		*temp++ = *odd_first++;
	}
}

void print(int *num, size_t sz) {
	int *end = num + sz;
	int p = 0;
	while (num < end && *num == 0) num++;
	while (num < end) {
		if (!p) {
			printf("%d", *num++);
			p = 1;
		} else {
			printf("%0*d", DIGIT, *num++);
		}
	}
	if (!p) putchar('0');
	
}

void fft(int *first, int *last, int prim_root, int mod) {
	if (last - first > 1) {
		int *mid = first + (last - first) / 2;
		int cur = 1;
		cross(first, last - first, temp);
		fft(first, mid, square(prim_root, mod), mod);
		fft(mid, last, square(prim_root, mod), mod);
		while (mid < last) {
			int x1 = *first, x2 = *mid;
			*first++ = add(x1, multiply(cur, x2, mod), mod);
			*mid++ = add(x1, multiply(mod - cur, x2, mod), mod);
			cur = multiply(cur, prim_root, mod);
		}
	}
} 

void ifft(int *first, int *last, int factor, int prim_root, int mod) {
	fft(first, last, prim_root, mod);
	while (first < last) {
		*first = multiply(*first, factor, mod);
		first++;
	}
}

int get_bin_len(size_t sz) {
	int result = 0;
	while (sz) {
		sz >>= 1;
		result++;
	}
	return result > 0 ? result : 1;
}

size_t long_mul(int *result, int *num1, size_t sz1, int *num2, size_t sz2) {
	if (sz1 == 0 || sz2 == 0) {
		*result = 0;
		return 0;
	} else {
		int s = get_bin_len(sz1 + sz2 - 1);
		int pr = fpow(PR, 1 << (PR_POW - s), P);
		int ni = multiply(N_REV, 1 << (PR_POW - s), P);
		int pri = fpow(pr, (1 << s) - 1, P);
		int *end;
		s = 1 << s;
		end = num1 + s;
		fft(num1, num1 + s, pr, P);
		fft(num2, num2 + s, pr, P);
		while (num1 < end) {
			*result++ = multiply(*num1++, *num2++, P);
		}
		result -= s;
		end = result + s;
		ifft(result, end, ni, pri, P);
		while (result < end) {
			result[1] += result[0] / X;
			result[0] %= X;
			result++;
		}
		return s;
	}
}

size_t get_int(int *arr, char *buf) {
	size_t len = 0, result = 0;
	int ch;
	char *begin = buf;
	while (isspace(ch = getchar())) ;
	while (ch == '0') ch = getchar();
	while (isdigit(ch)) {
		*buf++ = ch - '0';
		ch = getchar();
		len++;
	}
	while (len >= DIGIT) {
		int d = 0, i = 1;
		buf -= DIGIT;
		d = *buf++;
		while (i < DIGIT) d = d * 10 + *buf++, i++;
		len -= DIGIT;
		buf -= DIGIT;
		*arr++ = d;
		result++;
	}
	if (begin < buf) {
		int d = 0;
		d = *begin++;
		while (begin < buf) {
			d = d * 10 + *begin++;
		}
		*arr++ = d;
		result++;
	}
	return result;
}

void solve() {
	size_t s1, s2, rs;
	memset(a, 0, N * sizeof(int));
	memset(b, 0, N * sizeof(int));
	memset(c, 0, N * sizeof(int));
	s1 = get_int(a, buffer);
	s2 = get_int(b, buffer);
	rs = long_mul(c, a, s1, b, s2);
	reverse(c, c + rs);
	print(c, rs);
	putchar('\n');
}

int main() {
	int n;
	scanf("%d", &n);
	while (n-- > 0) {
		solve();
	}
	return 0;
}
