#include <stdio.h>
#include <stdlib.h>
#define NAMELEN 2

/*
 * Process: Each process has a name and has an execution time.
 *          We keep track of how long the process still needs
 *          to run on the processor. "remaining_time" is the
 *          remaining execution time of the process.
 */
typedef struct
{
    char *name;
    int starting_time;
    int remaining_time;
} Process;
/*
 *
 */
typedef struct Node
{
    Process data;
    struct Node* next;
} Node;
/*
 *
 */
typedef struct
{
    Node *first, *last;
} Queue;
/*
 *
 */
Queue* init()
{
    Queue *q = (Queue*) malloc(sizeof(Queue));
    q->first = (Node *) malloc(sizeof( Node ));
    q->last = (Node *) malloc(sizeof( Node ));
    q->first = NULL;
    q->last = NULL;
    return q;
}
/*
 *
 */
int isEmpty(Queue *q)
{
    if(q->first == 0)
        return 1;
    return 0;
}
/*
 *
 */
Process dequeue(Queue *q)
{
    if(!isEmpty(q))
    {
        Node* temp = q->first;
        q->first = q->first->next;
        return temp->data;
    }
    return;
}
/*
 *
 */
Node *newNode(Process val)
{
    Node *n = (Node*) malloc(sizeof(Node));
    n->data = val;
    n->next = NULL;
    return n;
}
/*
 *
 */
void enqueue(Queue *q, Process val)
{
    Node *n = newNode(val);
    if(isEmpty(q))
    {
        q->first = q->last = n;
    }
    else
    {
        q->last->next = n;
        q->last = n;
    }
}
/*
 *
 */
void destroy(Queue *q)
{
    Node* temp;
    while( q->first )
    {
        temp = q->first->next;
        free(q->first->next);
        q->first = temp;
    }
    q->first = NULL;
    q->last = NULL;
}
/*
 * RoundRobin Scheduling
 */
void RoundRobin(char* filename)
{
    int time = 0;
    int unitTime = 1;
    int watchingTime;
    int numberOfProcesses = 0;
    Queue *waitingQ = init();
    Process p;
    Process w[10];

    FILE *fp = fopen(filename,"r");
    fseek(fp, 22, SEEK_SET);
    fscanf(fp, "%d", &watchingTime);

    while(!feof(fp))
    {
        w[numberOfProcesses].name = malloc(NAMELEN);
        fscanf(fp, "\n%[^ ] %d %d\n", w[numberOfProcesses].name, &(w[numberOfProcesses].starting_time), &(w[numberOfProcesses].remaining_time));
        numberOfProcesses++;
    }
    fclose(fp);
    for(time = 0; time < watchingTime; time+=unitTime)
    {
        for(int i = 0; i < numberOfProcesses; i++ )
        {
            if(w[i].starting_time == time)
            {
                enqueue(waitingQ,w[i]);
            }

        }

        if(isEmpty(waitingQ))
            printf("\nidle (%d->%d)", time, time+unitTime);
        else
        {
            p = dequeue(waitingQ);
            printf("\n%s (%d->%d) ", p.name, time, time+unitTime);
            p.remaining_time--;
            if(p.remaining_time)
                enqueue(waitingQ,p);
            else
                printf("%s aborts.", p.name);
        }
    }
    printf("\nstop\n");
    destroy(waitingQ);


}
/*
 *
 */
int main()
{
    char filename[261];
    puts("Enter file name or Ctrl+Z to exit:");
    puts("----------------------------------");
    while(fgets(filename, 260, stdin) != NULL)
    {
        filename[strlen(filename)-1]='\0';
        if(fopen(filename,"r"))
            RoundRobin(filename);
        else
        {
            puts("File Not Found!");
            puts("----------------------------------");
        }
        puts("Enter file name or Ctrl+Z to exit:");
        puts("----------------------------------");
    }

    return 0;
}
