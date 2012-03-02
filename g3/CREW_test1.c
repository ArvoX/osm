#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#define NUM_THREADS 10
#define NUM_ITER 10

__inline__ uint64_t get_cycles()
{
	unsigned l,h;
	__asm__ __volatile__("rdtsc": "=a" (l), "=d" (h));
	return l + (((uint64_t)h) << 32);
}

// error function, checking if val is 0, expecting it to be errno otherwise.
void checkResults(char* msg, int val) {
	if (val == 0) 
		return;
	fprintf(stderr, "%s: %s\n", msg, strerror(val));
	exit(EXIT_FAILURE);
}

// the read-write lock
pthread_mutex_t rlock, wlock;
int readers = 0;

void *rdlockThread(void *arg)
{
	int rc;
	int me = (int)arg;
	int i = NUM_ITER;
	
	while(i-- > 0) {
//		printf("Reader %d getting read lock\n", me);		
//		printf("%d prepere\n", me);
		
		rc = pthread_mutex_lock(&rlock);
		if (++readers == 1){
			rc = pthread_mutex_lock(&wlock);
		}
		
//		printf("Readers increase -> %d\n", readers);
		rc = pthread_mutex_unlock(&rlock);
		
		printf("%llu\t2\n", get_cycles());
		// read for a while
		usleep(50);
				
		rc = pthread_mutex_lock(&rlock);
		if (--readers == 0){
			rc = pthread_mutex_unlock(&wlock);
		}
		
//		printf("%d end reading\n", me);
		
//		printf("Readers decrease -> %d\n", readers);
		rc = pthread_mutex_unlock(&rlock);
				
//		printf("Reader %d unlocked\n", me);
	}
	return NULL;
}

void *wrlockThread(void *arg)
{
	int rc;
	int me = (int)arg;
	int i = NUM_ITER;
	
	while (i-- > 0) {
//		printf("Writer %d getting write lock\n", me);
		rc = pthread_mutex_lock(&wlock);
		
		printf("%llu\t1\n", get_cycles());
		// write for a while
		usleep(100);
		
		rc = pthread_mutex_unlock(&wlock);
		//    printf("Writer %d unlocked\n", me);
	}
	return NULL;
}

int main(int argc, char **argv)
{
	int rc=0;
	int num= NUM_THREADS;
	int i;
	pthread_t *reader, *writer;
	
	if (argc > 1) {
		num = atoi(argv[1]);
	}
	
//	printf("Main initializing rwlock, will use %d readers and writers\n", 
//		   num);
	rc = pthread_mutex_init(&rlock, NULL);
	checkResults("pthread_rwlock_init()\n", rc);
	rc = pthread_mutex_init(&wlock, NULL);
	checkResults("pthread_rwlock_init()\n", rc);
	
	reader = malloc(2*num*sizeof(pthread_t));
	if (!reader) checkResults("malloc readers\n", errno);
	writer = reader + num;
	
	for (i=0; i < num; i++) {
		rc = pthread_create(&reader[i], NULL, rdlockThread, (void*) i);
		checkResults("pthread_create reader\n", rc);
		rc = pthread_create(&writer[i], NULL, wrlockThread, (void*) i+num);
		checkResults("pthread_create writer\n", rc);
	}
	
	for (i=0; i < num; i++) {
		rc = pthread_join(reader[i], NULL);
		checkResults("pthread_join\n", rc);
		rc = pthread_join(writer[i], NULL);
		checkResults("pthread_join\n", rc);
	}
	
	rc = pthread_mutex_destroy(&rlock);
	rc = pthread_mutex_destroy(&wlock);
	checkResults("pthread_rwlock_destroy()\n", rc);
	
//e	printf("Main completed\n");
	return 0;
}

