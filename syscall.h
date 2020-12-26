// System call numbers
#define SYS_fork    1
#define SYS_exit    2
#define SYS_wait    3
#define SYS_pipe    4
#define SYS_read    5
#define SYS_kill    6
#define SYS_exec    7
#define SYS_fstat   8
#define SYS_chdir   9
#define SYS_dup    10
#define SYS_getpid 11
#define SYS_sbrk   12
#define SYS_sleep  13
#define SYS_uptime 14
#define SYS_open   15
#define SYS_write  16
#define SYS_mknod  17
#define SYS_unlink 18
#define SYS_link   19
#define SYS_mkdir  20
#define SYS_close  21

/*Semaphore System Calls */
#define SYS_semaphore_initialize    22
#define SYS_semaphore_aquire        23
#define SYS_semaphore_release       24

/*Producers Consumers Helper System Calls */
#define SYS_set_buffer              25
#define SYS_get_buffer              26

/*Condition Variables System Calls */
#define SYS_cv_wait                 27
#define SYS_cv_signal               28

/*Helper System Call for CondVarTest*/
#define SYS_get_condvartest_cv      29

/*Readers Writers System Calls */
#define SYS_init_readers_writers    30
#define SYS_start_reading           31
#define SYS_start_writing           32