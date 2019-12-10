#include <stdio.h> 
#include <stdlib.h> 
#include <limits.h>
#include <semaphore.h>
#include <pthread.h>



sem_t wrmutex, mutex;
void* writer_func(void* id);
void* reader_func(void* id);
struct Queue* queue;
int lock;



struct Queue 
{ 
    int front, rear, size; 
    unsigned capacity; 
    int* array; 
}; 
   
struct Queue* createQueue(unsigned capacity) 
{ 
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue)); 
    queue->capacity = capacity; 
    queue->front = queue->size = 0;  
    queue->rear = capacity - 1;
    queue->array = (int*) malloc(queue->capacity * sizeof(int)); 
    return queue; 
}

void enqueue(struct Queue* queue, int item) 
{ 
    if (isFull(queue)) 
        return; 
    queue->rear = (queue->rear + 1)%queue->capacity; 
    queue->array[queue->rear] = item; 
    queue->size = queue->size + 1; 
}

/*void traverse(struct Queue* queue)
{
    for (int i=0; i<queue->size; i++)
    {
        printf("%d ", queue->array[i]);
    }
    printf("\n"); 
}*/

int isFull(struct Queue* queue) 
{  return (queue->size == queue->capacity);  }



void* reader_func(void* id)
{   
    int i = *(int*)id;

    sem_wait(&wrmutex);

    //read queue
    printf("Reader %d read ", i);
        for (int i=0; i<queue->size; i++)
    {
        printf("%d ", queue->array[i]);
    }
    printf("\n");

    sem_post(&wrmutex);
}

void* writer_func(void* id)
{   
    int i = *(int*)id;
    int rndm = rand()%10;

    sem_wait(&mutex);
    lock++;

    if (lock==1)
    {
            printf("Writer %d is writing %d\n", i, rndm);

            sem_wait(&wrmutex);
            enqueue(queue, rndm);
            printf("Writer %d wrote %d\n", i, rndm);
            lock--;
            sem_post(&wrmutex);
    }
    sem_post(&mutex);
}



int main()
{

    lock = 0;
    queue = createQueue(1000);
    printf("Input number of readers/writers: \n");
    int n;
    scanf("%d", &n);

    //number of threads
    pthread_t read[n], write[n];
    int id_n[n];


    sem_init(&mutex, 0, 1);
    sem_init(&wrmutex, 0, 1);


    for (int i=0; i<n; i++) 
    {
        id_n[i] = i;
        //reader threads    
        pthread_create(&write[i], NULL, &writer_func, &id_n[i]);
        pthread_create(&read[i], NULL, &reader_func, &id_n[i]);

    }



    for (int i=0; i<n; i++) 
    {
        pthread_join(write[i], NULL);
        pthread_join(read[i], NULL);
    }
    

    sem_destroy(&mutex);
    sem_destroy(&wrmutex);


    return 0;
}