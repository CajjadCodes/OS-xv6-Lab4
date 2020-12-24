// Semaphore definition
struct semaphore {
    int value; // value of semaphore
    int max_value;
    struct spinlock lk; // spinlock protecting this semaphore

    struct proc* waiting[NPROC]; // waiting processes
};