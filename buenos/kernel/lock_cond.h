typedef struct {
} cond_t;

void condition_init(cond_t *cond);
void condition_wait(cond_t *cond, lock_t *lock);
void condition_signal(cond_t *cond);
void condition_broadcast(cond_t *cond);
