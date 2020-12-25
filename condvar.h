// condvar definition
struct condvar {
    struct uspinlock lk; // spinlock protecting this semaphore
};