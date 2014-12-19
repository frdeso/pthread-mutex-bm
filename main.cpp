#include <pthread.h>
#include <time.h>

#include <iostream>
#include <chrono>
#include <ctime>
long long int num_iter = 5000000;
pthread_mutex_t global_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_mutex_t dummy_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
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
	pthread_mutex_t mutex1 = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
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
	pthread_mutex_t mutex1 = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

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
	pthread_mutex_t mutex1 = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
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

void *second_thread(void *arg)
{
	long int percent = (long int) arg;
	for(int i = 0; i < num_iter; i++)
	{
		if(rand()%100 < percent)
		{
			pthread_mutex_lock(&global_mutex);
			pthread_mutex_unlock(&global_mutex);
		}
		else
		{
			pthread_mutex_lock(&dummy_mutex);
			pthread_mutex_unlock(&dummy_mutex);
		}
	}
}

void contentedLock()
{
	for(int i = 0; i < num_iter; i++)
	{
		pthread_mutex_lock(&global_mutex);
		pthread_mutex_unlock(&global_mutex);
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
void multipleContentionRatio()
{
		pthread_t thr;
		void* ret;
		pthread_create(&thr, NULL, &second_thread, (void*) 0);
		time_execution(&contentedLock,NULL);
		pthread_join(thr, &ret);

		pthread_create(&thr, NULL, &second_thread, (void*) 1);
		time_execution(&contentedLock,NULL);
		pthread_join(thr, &ret);

		pthread_create(&thr, NULL, &second_thread, (void*) 2);
		time_execution(&contentedLock,NULL);
		pthread_join(thr, &ret);

		pthread_create(&thr, NULL, &second_thread, (void*) 3);
		time_execution(&contentedLock,NULL);
		pthread_join(thr, &ret);

		pthread_create(&thr, NULL, &second_thread, (void*) 4);
		time_execution(&contentedLock,NULL);
		pthread_join(thr, &ret);

		pthread_create(&thr, NULL, &second_thread, (void*) 5);
		time_execution(&contentedLock,NULL);
		pthread_join(thr, &ret);

		pthread_create(&thr, NULL, &second_thread, (void*) 6);
		time_execution(&contentedLock,NULL);
		pthread_join(thr, &ret);

		pthread_create(&thr, NULL, &second_thread, (void*) 7);
		time_execution(&contentedLock,NULL);
		pthread_join(thr, &ret);

		pthread_create(&thr, NULL, &second_thread, (void*) 8);
		time_execution(&contentedLock,NULL);
		pthread_join(thr, &ret);

		pthread_create(&thr, NULL, &second_thread, (void*) 9);
		time_execution(&contentedLock,NULL);
		pthread_join(thr, &ret);

		pthread_create(&thr, NULL, &second_thread, (void*) 10);
		time_execution(&contentedLock,NULL);
		pthread_join(thr, &ret);

}
int main()
{
	std::cout<<"Single Lock"<<","<<"Double Lock"<<","
				<<"Recursive"<<","<<"Random"<<","
				<<"Content 0%"<<","	<<"Content 10%"
				<<","<<"Content 20%"<<","<<"Content 30%"
				<<","<<"Content 40%"<<","<<"Content 50%"
				<<","<<"Content 60%"<<","<<"Content 70%"
				<<","<<"Content 80%"<<","<<"Content 90%"
				<<","<<"Content 100%"<<std::endl;

	for(int i = 0; i < 10; ++i){
		std::cout<<i<<",";
		time_execution(&singleLock, NULL);
		time_execution(&doubleLock,NULL);
		time_execution(&recursiveLock,NULL);
		time_execution(&randLock,&generateRandomArray);
		multipleContentionRatio();
		std::cout<<std::endl;

	}
	return 0;
}
