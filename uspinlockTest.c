/*A program to show how to use uspinlock*/

#include "types.h"
#include "stat.h"
#include "user.h"
#include "uspinlock.h"

int main(int argc, char* argv[]) 
{
    struct uspinlock lk;

    init_lock(&lk);
    printf(1, "lock initialized. lk.locked == %d\n", lk.locked);

    lock(&lk);
    printf(1, "locked the lk. lk.locked == %d\n", lk.locked);

    printf(1, "We are in critical section\n");

    unlock(&lk);
    printf(1, "unlocked the lk, lk.locked == %d\n", lk.locked);

    exit();
}