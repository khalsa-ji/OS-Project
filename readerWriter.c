				// Header files
#include <stdio.h>			// For printf(), scanf().
#include <unistd.h>			// For usleep().
#include <stdlib.h>			// For srand(), rand();
#include <pthread.h>		// For pthread_t, pthread_mutex_t, pthread_create(), pthread_join(), pthread_mutex_init(), pthread_mutex_lock(), pthread_mutex_unlock(). 

				// Function prototypes.
void *reader(void *);		// For allowing controlled access(prioritised among writers) to perform _read().
void _read(void *, int);	// For displaying and performing basic read operation.

				// Global variables.
int x, count_readers = 0, sharedVariable = 100;
pthread_mutex_t lock_reader, lock_writer;

				// main() function.
int main()
{
	int n_readers, n_writers, i, res;
	void *ptr;

		// Get required inputs from the user.
	printf("Enter the number of reader threads: ");
	scanf("%d", &n_readers);
	printf("Enter the number of writer threads: ");
	scanf("%d", &n_writers);
	printf("Enter the number of times each reader/writer thread accesses the shared variable: ");
	scanf("%d", &x);
	
		// Implementing constraints
	n_readers = (n_readers > 0) ? n_readers : 5;
	n_writers = (n_writers > 0)	? n_writers : 5;	
	
	pid_t th_reader[n_readers], th_writer[n_writers];
	pthread_mutex_init(&lock_reader, NULL);	
	pthread_mutex_init(&lock_writer, NULL);
	
		// Creating reader threads.
	for(i=0; i<n_readers; i++)
	{
		ptr = (int *)i;
		res = pthread_create(&th_reader[i], NULL, reader, ptr);
		if(res != 0)	perror("Oops! reader thread was unable to be created successfully!\n");
		else	printf("Reader thread %d created successfully.\n", i);
	}
	printf("\n");
	
		// Creating writer threads.
	for(i=0; i<n_writers; i++)
	{
		ptr = (int *)i;
		res = pthread_create(&th_writer[i], NULL, writer, ptr);
		if(res != 0)	perror("Oops! writer thread was unable to be created successfully!\n");
		else	printf("Writer thread %d created successfully.\n", i);
	}
	printf("\n");

		// Joining threads.
	for(i=0; i<n_readers; i++)	pthread_join(th_reader[i], NULL);	// Joining reader threads.
	for(i=0; i<n_writers; i++)	pthread_join(th_writer[i], NULL);	// Joining writer threads.
	
	return 0;
}
		// ENDS here...

				// Functions begin from here...
	// 1. Function for allowing controlled access(prioritised among writers) to perform _read(), begins from here...
void *reader(void *id)
{
	int i, randTime;
	srand(time(0));
	randTime = rand() % 150;
	usleep(randTime);

	pthread_mutex_lock(&lock_reader);	
	count_readers++;	// Incrementing reader's count by 1.	
	if(count_readers > 0)	pthread_mutex_lock(&lock_writer);		// Preventing writers to access the sharedVariable.
	
		//	Reading the sharedVariable 'x' number of times.
	for(i=0; i<x; i++)
	{
		randTime = rand() % 100;
		usleep(randTime);
		_read(id, i+1);
	}

	count_readers--;	// Decrementing reader's count by 1.
	if(count_readers == 0)	pthread_mutex_unlock(&lock_writer);		// Allowing writers to access the sharedVariable, if they are willing to.
	pthread_mutex_unlock(&lock_reader);
	pthread_exit(0);
}
		// ENDS here...

