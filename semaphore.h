// Semaphore definition
struct semaphore {
    int value; // value of semaphore
    struct spinlock lk; // spinlock protecting this semaphore

    struct proc* waiting[NPROC]; // waiting processes
};