
// by newlord
#include "cacheutils.h"

// by newlord
int __attribute__ ((aligned(4096)))  newlord_buffer;
int __attribute__ ((aligned(4096)))  bitstream[10]={0,1,0,};

int __attribute__ ((aligned(4096))) x_size=1;

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


__attribute__ ((noinline)) int test(unsigned int x) {
//int test(unsigned int x) {

	uint64_t s,e;

	//flush (&x_size);
	//maccess (&bitstream[0]);
	//maccess (&bitstream[1]);
	//maccess (&bitstream[2]);
  asm volatile("mfence");
	

#if 1
	if ( x < x_size ) 
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


#define LOOP	1000000
//#define COUNT	40000
#define COUNT	30000
int main () {

	uint64_t d;
	int i;
	unsigned int val=1;
	unsigned int x;

	//printf ("newlord_buffer:%p\n", &newlord_buffer);
	//printf ("bitstream:%p\n",bitstream);
	
	for (i=0; i < LOOP; i++) {
		flush (&newlord_buffer);
		flush (&x_size);
		maccess (&bitstream[1]);
		maccess (&bitstream[2]);

		if (i%COUNT != 0) {
			x=0;
		} else {
			val= (val==1)?2:1;
			x=val;
		}
		d=test(x);
		printf ("%d,%ld\n",x, d);
	}

}
