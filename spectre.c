
// by newlord
#include "cacheutils.h"

// by newlord
int newlord_buffer;
int bitstream[10]={0,1,0,};

int test(unsigned int val); 

inline uint64_t rdtsc() {
  uint64_t a, d;
  asm volatile("mfence");
#if USE_RDTSCP
  asm volatile("rdtscp" : "=a"(a), "=d"(d) :: "rcx");
#else
  asm volatile("rdtsc" : "=a"(a), "=d"(d));
#endif
  a = (d << 32) | a;
  asm volatile("mfence");
  return a;
}


int test(unsigned int val) {

	unsigned int x = val;
	uint64_t s,e;

	// by newlord
	flush (&newlord_buffer);
	maccess (&bitstream[1]);
	maccess (&bitstream[2]);

#if 1
	if ( x < 1 ) 
		if (bitstream[x])
			maccess (&newlord_buffer);
#else
	if ( x == 1)
		maccess (&newlord_buffer);

#endif


	s=rdtsc();
	maccess (&newlord_buffer);
	e=rdtsc() - s;

	return e;
}


#define LOOP	100000
#define COUNT	1000
int main () {

	uint64_t d;
	int i,j;
	unsigned int val;
	
	for (i=0; i < LOOP; i++) {
		for (j=0; j < COUNT ; j++)
			test (0);
		val = (unsigned int) (i%2 + 1);
		d=test(val);
		printf ("%d,%ld\n",val, d);
	}

}
