/* BIG BUG: global variables are not shared between children after fork.*/
/* must find another way to share buffer*/

#include "types.h"
#include "stat.h"
#include "user.h"

#define BUFFER_SIZE 5
#define REPEAT_NUM 20

#define EMPTY_SPACE_SEMAPHORE 0
#define AVAILABLE_DATA_SEMAPHORE 1
#define MUTEX_SEMAPHORE 2
#define STDOUT_SEMAPHORE 3

char buffer[BUFFER_SIZE];
int counter = 0;


void producer(void) {
    semaphore_aquire(STDOUT_SEMAPHORE);
    printf(1, "Producer started..\n");
    semaphore_release(STDOUT_SEMAPHORE);
    char data;
    
    for (int i = 0; i < REPEAT_NUM; i++) {
        // producing some alphabets
        data = (i % 20) + 0x41; 
        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "Producer wants to put [%c] in buffer[%d]\n", data, counter);
        semaphore_release(STDOUT_SEMAPHORE);

        semaphore_aquire(EMPTY_SPACE_SEMAPHORE);
        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "Producer passed empty_space_semaphore\n");
        semaphore_release(STDOUT_SEMAPHORE);

        semaphore_aquire(MUTEX_SEMAPHORE);
        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "Producer passed mutex_semaphore\n");
        semaphore_release(STDOUT_SEMAPHORE);

        if (counter < BUFFER_SIZE) {
            buffer[counter] = data;
            counter = (counter + 1) % BUFFER_SIZE;
            semaphore_aquire(STDOUT_SEMAPHORE);
            printf(1, "Producer wrote [%c] in buffer[%d]\n", data, counter);
            semaphore_release(STDOUT_SEMAPHORE);
        }

        semaphore_release(MUTEX_SEMAPHORE);
        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "Producer released mutex_semaphore\n");
        semaphore_release(STDOUT_SEMAPHORE);

        semaphore_release(AVAILABLE_DATA_SEMAPHORE);
        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "Producer released available_data_semaphore\n");
        semaphore_release(STDOUT_SEMAPHORE);
    }

    semaphore_aquire(STDOUT_SEMAPHORE);
    printf(1, "Producer died\n");
    semaphore_release(STDOUT_SEMAPHORE);
    exit();
}

void consumer(void) {
    semaphore_aquire(STDOUT_SEMAPHORE);
    printf(1, "Consumer started..\n");
    semaphore_release(STDOUT_SEMAPHORE);
    char data = 0;
    
    for (int i = 0; i < REPEAT_NUM; i++) {
        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "Consumer is starting to read new data\n");
        semaphore_release(STDOUT_SEMAPHORE);

        semaphore_aquire(AVAILABLE_DATA_SEMAPHORE);
        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "Consumer passed available_data_semaphore\n");
        semaphore_release(STDOUT_SEMAPHORE);

        semaphore_aquire(MUTEX_SEMAPHORE);
        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "Consumer passed mutex_semaphore\n");
        semaphore_release(STDOUT_SEMAPHORE);

        if (counter > 0) {
            data = buffer[counter];
            counter = counter - 1;
            semaphore_aquire(STDOUT_SEMAPHORE);
            printf(1, "Consumer read data mutex_semaphore\n");
            semaphore_release(STDOUT_SEMAPHORE);
        }

        semaphore_release(MUTEX_SEMAPHORE);
        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "Cosnumer released mutex_semaphore\n");
        semaphore_release(STDOUT_SEMAPHORE);

        semaphore_release(EMPTY_SPACE_SEMAPHORE);
        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "Cosnumer released empty_space_semaphore\n");
        semaphore_release(STDOUT_SEMAPHORE);

        // consuming the alphabet
        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "Cosnumer consumed data [%c]\n", data);
        semaphore_release(STDOUT_SEMAPHORE);
    }

    semaphore_aquire(STDOUT_SEMAPHORE);
    printf(1, "Consumer died\n");
    semaphore_release(STDOUT_SEMAPHORE);
    exit();
}

int main(int argc, char* argv[]) {

    semaphore_initialize(AVAILABLE_DATA_SEMAPHORE, 0, 0);
    semaphore_initialize(EMPTY_SPACE_SEMAPHORE, BUFFER_SIZE, 0);
    semaphore_initialize(MUTEX_SEMAPHORE, 1, 0);
    semaphore_initialize(STDOUT_SEMAPHORE, 1, 0);

    printf(1, "All semaphores are intialized\n");

    if (fork() == 0) { // producer child
        producer();
    }
    else {
        if (fork() == 0) { // consumer child
            consumer();
        }
        else { // tester parent
            wait();
            wait();
        }
    }
    exit();
}