#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define NUM_THREADS 10
#define NUM_ITER 20

// error function, checking if val is 0, expecting it to be errno otherwise.
void checkResults(char* msg, int val) {
  if (val == 0) 
    return;
  fprintf(stderr, "%s: %s\n", msg, strerror(val));
  exit(EXIT_FAILURE);
}

// the read-write lock
pthread_rwlock_t rwlock;

void *rdlockThread(void *arg)
{
  int rc;
  int me = (int)arg;
  int i = NUM_ITER;

  while(i-- > 0) {
    printf("Reader %d getting read lock\n", me);
    rc = pthread_rwlock_rdlock(&rwlock);
    checkResults("pthread_rwlock_rdlock()\n", rc);
    printf("%d reading\n", me);
    
    // read for a while
    usleep(50);
    
    rc = pthread_rwlock_unlock(&rwlock);
    checkResults("pthread_rwlock_unlock()\n", rc);
    printf("Reader %d unlocked\n", me);
  }
  return NULL;
}

void *wrlockThread(void *arg)
{
  int rc;
  int me = (int)arg;
  int i = NUM_ITER;

  while (i-- > 0) {
    printf("Writer %d getting write lock\n", me);
    rc = pthread_rwlock_wrlock(&rwlock);
    checkResults("pthread_rwlock_wrlock()\n", rc);
    
    printf("%d writing\n", me);
    // write for a while
    usleep(100);

    rc = pthread_rwlock_unlock(&rwlock);
    checkResults("pthread_rwlock_unlock()\n", rc);
    printf("Writer %d unlocked\n", me);
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

  printf("Main initializing rwlock, will use %d readers and writers\n", 
         num);
  rc = pthread_rwlock_init(&rwlock, NULL);
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

  rc = pthread_rwlock_destroy(&rwlock);
  checkResults("pthread_rwlock_destroy()\n", rc);

  printf("Main completed\n");
  return 0;
}

