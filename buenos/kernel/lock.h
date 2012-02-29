typedef struct {
} lock_t;

int lock_reset(lock_t *lock);
void lock_acquire(lock_t *lock);
void lock_release(lock_t *lock);
