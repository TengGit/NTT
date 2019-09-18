#include <stdio.h>

#define N    (1<<26)
#define P    ((27<<26)+1)
#define PROG 10000000
#define MIN  2
#define MAX  1000
#define MINI 2
#define MAXI (P-1)

typedef int (*fpow_t)(int, int, int);

int mulm(long long a, long long b, int m) {
	return (a * b) % m;
}

int fpow(int base, int expo, int mod) {
	int coeff = 1;
	while (expo > 1) {
		if (expo & 1) {
			coeff = mulm(coeff, base, mod);
		}
		base = mulm(base, base, mod);
		expo >>= 1;
	}
	return expo ? mulm(coeff, base, mod) : 1;
}

int main() {
	int i;
	for (i = MIN; i < MAX; i++) {
		if (fpow(i, N, P) == 1 && fpow(i, N >> 1, P) != 1) {
			printf("Solution: %d\n", i);
		} else if (i % PROG == 0) {
			fprintf(stderr, "Progress: %d\n", i);
		}
	}
	for (i = MINI; i < MAXI; i++) {
		if (mulm(i, N, P) == 1) {
			printf("Inverse: %d\n", i);
			break;
		} else if (i % PROG == 0) {
			fprintf(stderr, "Progress: %d\n", i);
		}
	}
	return 0;
}
