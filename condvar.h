// condvar definition
struct condvar {
    struct uspinlock lk; // spinlock protecting this semaphore

    // struct proc* waiting[NPROC]; // waiting processes
};