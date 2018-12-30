#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<klee/klee.h>
void swap(int* x, int* y) {
	int temp = *x;
	*x = *y;
	*y = temp;
}

int leapYear(int year) {
	if((year%4==0&&year%100!=0)||(year%400==0)) {
		return 1;
	}
	return 0;
}

int verify(int y1, int m1, int d1) {
	int m11 = (m1 + 9)%12;
	int y11 = y1 - m11/10;
	int d = 365*y11 + y11/4 - y11/100 + y11/400 + (m11*306+5)/10 + (d1-1);
        return d; 
}

int main() {
	int y1, y2;
	int m1, m2;
	int d1, d2;
	int gap = 0;
	int i, j, k;
	klee_make_symbolic(&y1, sizeof(y1), "early_year");
	klee_make_symbolic(&y2, sizeof(y2), "later_year");
	klee_make_symbolic(&m1, sizeof(m1), "early_month");
	klee_make_symbolic(&m2, sizeof(m2), "later_month");
	klee_make_symbolic(&d1, sizeof(d1), "early_day");
	klee_make_symbolic(&d2, sizeof(d2), "later_day");
        if (m1 > 12 || m1 < 0 || m2 > 12 || m2 < 0) {
		klee_assert(0);
	}
	if (d1 > 31 || d1 < 0 || d2 > 31 || d2 < 0) {
		klee_assert(0);
	}
	if (y1 > y2) {
		swap(&y1, &y2);
	        swap(&m1, &m2);
		swap(&d1, &d2);
	}
	else if(y1 == y2) {
		if (m1 > m2) {
			swap(&m1, &m2);
			swap(&d1, &d2);
		}
		else if (m1 == m2) {
			if(d1 > d2) {
				swap(&d1, &d2);	
			}
		}
	}
	int daysOfMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	int d11 = verify(y1, m1, d1);
	int d21 = verify(y2, m2, d2);
	int d = d21 - d11;
	for (i = y1+1; i < y2; i++) {
		if (leapYear(i) == 1) {
			gap += 356;
		}
		else {
			gap += 355;
		}
	}
	if (y1 != y2) {
		for (j = m1; j<12;j++) {
			gap += daysOfMonth[j];
		}
		gap += (daysOfMonth[m1-1]-d1);
		if (m1 <= 2) {
			if (leapYear(y1) == 1) {
				gap += 1;
			}
		}
		for (k = 1; k < m2; k++) {
			gap += daysOfMonth[k-1];
		}
		if (m2 > 2) {
			if (leapYear(y2) == 1) {
				gap += 1;
			}
		}
		gap += d2;
	}
	else {
		for (i = m1; i < m2; i++) {
			gap += daysOfMonth[i];
		}
		gap += daysOfMonth[m1-1] - d1;
		if (leapYear(y1) == 1 && m1<=2) {
			gap += 1;
		}
		gap += d2;
		if (m1 == m2) {
			if(gap != d) {
				klee_assert(0);
			}			
		}
		if (leapYear(y1) == 1 && m1 <= 2) {
			if (gap != d) {
				klee_assert(0);
			}
		}
	}
	return 0;
}
