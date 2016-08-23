#include "../include/semaphores.h"

char* states;
int num_philosophers = 0;

sem_t mutex;
sem_t* forks;
pthread_t* philosophers;

int main(int argc,char *argv[])
{
    if (argc > 1)
    {
        num_philosophers = atoi(argv[1]);
    }
    else
    {
        printf("To use this program, you should type the following command line:\n");
        printf("    semaphores.exe <number_of_philosophers>\n");

        return -1;
    }

    forks = malloc(sizeof(sem_t) * num_philosophers);
    states = malloc(sizeof(char) * num_philosophers);
    philosophers = malloc(sizeof(pthread_t) * num_philosophers);

    sem_init(&mutex, 0, 1);

    int i;
    for (i = 0; i < num_philosophers; i++)
    {
        sem_init(forks + i, 0, 1);
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
    int philosopher_pos = *((int *)i);
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
    int left_fork = philosopher_pos;
    int right_fork = (philosopher_pos + 1) % num_philosophers;

    if(is_even(philosopher_pos))
    {
        sem_wait(forks + left_fork);
        sem_wait(forks + right_fork);
    }
    else
    {
        sem_wait(forks + right_fork);
        sem_wait(forks + left_fork);
    }
}

void release_forks(int philosopher_pos)
{
    int left_fork = philosopher_pos;
    int right_fork = (philosopher_pos + 1) % num_philosophers;

    sem_post(forks + left_fork);
    sem_post(forks + right_fork);
}
