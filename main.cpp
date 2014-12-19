#include <pthread.h>
#include <time.h>

#include <iostream>
#include <chrono>
#include <ctime>
long long int num_iter = 10000000;
bool *randArray;
void singleLock()
{
	pthread_mutex_t mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

	for(int i = 0; i < num_iter; i++)
	{
		pthread_mutex_lock(&mutex);	
		pthread_mutex_unlock(&mutex);	
	}
}

void doubleLock()
{
	pthread_mutex_t mutex1= PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
	pthread_mutex_t mutex2 = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

	for(int i = 0; i < num_iter; i++)
	{
		pthread_mutex_lock(&mutex1);	
		pthread_mutex_lock(&mutex2);	
		pthread_mutex_unlock(&mutex2);	
		pthread_mutex_unlock(&mutex1);	
	}
}
void recursiveLock()
{
	pthread_mutex_t mutex1= PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

	for(int i = 0; i < num_iter; i++)
	{
		pthread_mutex_lock(&mutex1);	
		pthread_mutex_lock(&mutex1);	
		pthread_mutex_unlock(&mutex1);	
		pthread_mutex_unlock(&mutex1);	
	}
}
void generateRandomArray()
{	
	srand(time(NULL));
	randArray = (bool*) malloc(sizeof(bool)*num_iter);
	
	for(int i = 0; i < num_iter; i++)
	{
		randArray[i] = rand()%2;
	}
}
void randLock()
{
	pthread_mutex_t mutex1= PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
	pthread_mutex_t mutex2 = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

	for(int i = 0; i < num_iter; i++)
	{
		pthread_mutex_lock(&mutex1);	
		if(randArray[i] == 0)
		{
			pthread_mutex_lock(&mutex2);	
			pthread_mutex_unlock(&mutex2);	
		}
		pthread_mutex_unlock(&mutex1);	
	}
}

void time_execution(void (*fct) (void), void (*pre_fct) (void))
{

	if(pre_fct != NULL)
	{
		pre_fct();			
	}
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	fct();
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	std::cout<<elapsed_seconds.count()<<",";
}

int main()
{ 
	for(int i = 0; i < 20; ++i){
		std::cout<<i<<",";
		time_execution(&singleLock, NULL);
		time_execution(&doubleLock,NULL);
		time_execution(&recursiveLock,NULL);
		time_execution(&randLock,&generateRandomArray);
		std::cout<<std::endl;
	}
	return 0;
}
