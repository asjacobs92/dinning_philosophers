#include "../include/monitor.h"

int* forks;
char* states;
int num_philosophers = 0;

pthread_t* philosophers;
pthread_cond_t* cond;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc,char *argv[])
{
    if (argc > 1)
    {
        num_philosophers = atoi(argv[1]);
    }
    else
    {
        printf("To use this program, you should type the following command line:\n");
        printf("    monitor.exe <number_of_philosophers>\n");

        return -1;
    }

    forks = malloc(sizeof(int) * num_philosophers);
    states = malloc(sizeof(char) * num_philosophers);
    philosophers = malloc(sizeof(pthread_t) * num_philosophers);
    cond = malloc(sizeof(pthread_cond_t) * num_philosophers);

    int i;
    for (i = 0; i < num_philosophers; i++)
    {
        forks[i] = 0;
        pthread_cond_init(cond + i, NULL);
    }

    for (i = 0; i < num_philosophers; i++)
    {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&philosophers[i], NULL, philosopher, (void *)arg);
    }

    while (1)
    {
        sleep(3);
        for (i = 0; i < num_philosophers; i++)
        {
            printf("%c ", states[i]);
        }
        printf("\n");
    }
    return 0;
}

int is_even(int number)
{
    return number % 2 == 0;
}

void *philosopher(void *i)
{
    int philosopher_pos = *((int *) i);
    while(1)
    {
        // philosopher is thinking
        states[philosopher_pos] = STATE_THINKING;
        sleep(rand() % 10 + 1);

        // philosopher is hungry and trying to acquire forks
        states[philosopher_pos] = STATE_HUNGRY;
        acquire_forks(philosopher_pos);


        // philosopher is eating
        states[philosopher_pos] = STATE_EATING;
        sleep(rand() % 10 + 1);

        release_forks(philosopher_pos);
    }
}

void acquire_forks(int philosopher_pos)
{
    pthread_mutex_lock(&mutex);
    {
        if (is_even(philosopher_pos)) // to avoid deadlock
        {
            acquire_left_fork(philosopher_pos);
            acquire_right_fork(philosopher_pos);
        }
        else
        {
            acquire_right_fork(philosopher_pos);
            acquire_left_fork(philosopher_pos);
        }
    }
    pthread_mutex_unlock(&mutex);
}

void release_forks(int philosopher_pos)
{
    pthread_mutex_lock(&mutex);
    {
        int left_fork = philosopher_pos;
        int right_fork = (philosopher_pos + 1) % num_philosophers;

        (forks[left_fork])--;
        (forks[right_fork])--;
        pthread_cond_signal(cond + left_fork);
        pthread_cond_signal(cond + right_fork);
    }
    pthread_mutex_unlock(&mutex);
}

void acquire_left_fork(int philosopher_pos)
{
    int left_fork = philosopher_pos;
    if (forks[left_fork] == 1)
        pthread_cond_wait(cond + left_fork, &mutex);
    (forks[left_fork])++;
}

void acquire_right_fork(int philosopher_pos)
{
    int right_fork = (philosopher_pos + 1) % num_philosophers;
    if (forks[right_fork] == 1)
        pthread_cond_wait(cond + right_fork, &mutex);
    (forks[right_fork])++;
}
