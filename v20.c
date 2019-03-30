#include <pthread.h> 
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

struct Targ{
	 int T_n_threads,T_id,T_N, T_Distance;	
};

// number of elements in array 
#define N1 1000000
int a[N1]; 
int flag=2;

// merge function for merging two parts 
void merge(int low, int mid, int high) 
{  int* left = (int*)malloc((mid-low+1)*sizeof(int));	
	int* right = (int*)malloc((high-mid)*sizeof(int));
	int n1 = mid - low + 1, n2 = high - mid, i, j; 
 /*    printf("low =%d /t mid=%d /t high=%d /t\n",low,mid,high);
	printf("n1 =%d /t n2=%d \n",n1,n2); */
	
	for (i = 0; i < n1; i++) 
	{	
         left[i] = a[i + low]; 
	} 
	for (i = 0; i < n2; i++) 
	{	
       right[i] = a[i + mid + 1];
   	}
	int k = low; 
	i = j = 0;  
	while (i < n1 && j < n2) { 
		if (left[i] <= right[j]) 
			a[k++] = left[i++]; 
		else
			a[k++] = right[j++];		
	} 
	while (i < n1) { 
		a[k++] = left[i++]; 
	}  
	while (j < n2) { 
		a[k++] = right[j++]; 
	}
free(left);
free(right);	
} 

// merge sort function 
void merge_sort(int low, int high) 
{ 
	// calculating mid point of array 
	int mid = (high + low) / 2; 
	if (low < high) { 

		// calling first half 
		merge_sort(low, mid); 

		// calling second half 
		merge_sort(mid + 1, high); 

		// merging the two halves 
		merge(low, mid, high); 
	} 
} 


void* T_2(void* arg) 
{   
    int id,n_threads, chunksize,N,low,high,mid,distance;
	struct Targ  *mydata;
	mydata=(struct Targ*) arg;
	id=mydata->T_id;
	n_threads=mydata->T_n_threads;
	N=mydata->T_N;
	distance=mydata->T_Distance;
	chunksize=N/n_threads;
	if(id%distance==0)
	{
		low = chunksize*id; 
	    high = ((low + (chunksize*distance)-1));
		  if(id==n_threads-distance)
		 {
			 mid = ((high + low) / 2);
			 high+=N%n_threads;
		 }		 
	   // printf("Thread number =%d /t thread low=%d /t thread low=%d /t\n",thread_part,low,high); 
	    else
		{
			mid = (high + low) / 2;
        }
		printf("Thread number =%d /t thread low=%d  thread med=%d /t thread high=%d chunksize=%d/t\n",id,low,mid,high,chunksize);
		merge(low, mid, high);
	}
} 

// thread function for multi-threading 
void* T_1(void* arg) 
{   
    int id,n_threads, chunksize,N;
	struct Targ  *mydata;
	mydata=(struct Targ*) arg;
	id=mydata->T_id;
	n_threads=mydata->T_n_threads;
	N=mydata->T_N;
	chunksize=N/n_threads;
	int low = chunksize*id; 
	int high = (low + (chunksize-1));
      if(id==n_threads-1)
     {
		 high+=N%n_threads;
	 }		 
   // printf("Thread number =%d /t thread low=%d /t thread low=%d /t\n",thread_part,low,high); 
	int mid = (high + low) / 2; 
	printf("Thread number =%d /t thread low=%d  thread med=%d /t thread high=%d /t\n",id,low,mid,high);
	if (low < high) { 
		merge_sort(low, mid); 
		merge_sort(mid + 1, high); 
		merge(low, mid, high); 
	} 
} 

int main(int argc, const char *argv[]) 
{    
    if (argc != 4)
	  {
		printf("Enter 3 input args\n");
		return -1;
	  }
    //N=atoi(argv[1]);
	int n_threads= atoi(argv[1]);
	int N=atoi(argv[2]);
	int Limit=atoi(argv[3]);
	int mid;
	// generating random values in array 
	struct Targ  thread_data_array[n_threads];
	for (int i = 0; i < N; i++) 
	{a[i] = rand() % Limit; }
       /* for (int i = 0; i < N; i++) 
	{
		printf("index number =%d \t  value=%d \n",i,a[i]); 
     }
      printf("\n\n\n\n");  
 */	// t1 and t2 for calculating time for 
	// merge sort 
	void *status;
	double t1 = get_wall_seconds(); 
	pthread_t threads[n_threads]; 
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  //  printf("creating threads 1\n");
	// creating  threads 
	for (int i = 0; i < n_threads; i++) 
	{ 
		thread_data_array[i].T_id=i;
		thread_data_array[i].T_N=N;
		thread_data_array[i].T_n_threads=n_threads;
		pthread_create(&threads[i], &attr, T_1, (void*)&thread_data_array[i]); 
//		while(n_threads/2>flag)
	}
     
	// joining all threads 
	for (int i = 0; i < n_threads; i++) 
		pthread_join(threads[i], &status); 
	
	/* for (int i = 0; i < N; i++) 
	{
		printf("index number =%d \t  value=%d \n",i,a[i]); 
     } */
	int flag =n_threads;
	int distance=2;
	while (flag>2)
	{     
        flag =flag/2;
		printf("Value of distance= %d  value of flag =%d\n",distance, flag);
		//printf( "1st thread merging \n");
		for (int i = 0; i < n_threads; i++) 
		{ 
			thread_data_array[i].T_id=i;
			thread_data_array[i].T_N=N;
			thread_data_array[i].T_n_threads=n_threads;
			thread_data_array[i].T_Distance=distance;
			pthread_create(&threads[i], &attr, T_2, (void*)&thread_data_array[i]); 
		}
		for (int i = 0; i < n_threads; i++) 
		{
			pthread_join(threads[i], &status);
	    }
		//printf("Value of distance= %d \n",distance);
        distance=distance*2; 		
    }
	//final merge 
	if(N%n_threads==0)
	{
		mid=(N-1)/2;
	}
	else
	{
		mid=N/n_threads;
		mid=((n_threads*mid)/2)-1;
		printf("mid value in final marge value=%d \n",mid);
		
	}
	merge(0, mid, N - 1);
	//final print
	for (int i = 0; i < N; i++) 
	{
		printf("index number =%d \t  value=%d \n",i,a[i]); 
    }

	/* while(n_threads>2){
		   n_threads=n_threads/2;
		   pthread_t threads[n_threads];
	       // creating  threads 
		for (int i = 0; i < n_threads; i++) 
		{
			thread_data_array[i].T_id=i;
			thread_data_array[i].T_n_threads=n_threads;
			pthread_create(&threads[i], &attr, T_2, (void*)&thread_data_array[i]); 
		}
        for (int i = 0; i < n_threads; i++) 
		{	
			pthread_join(threads[i], &status);
		} 			
	}
	 */ 
//	 merge(0, (N - 1)/2, N - 1);   //Final merge

	double t2 = get_wall_seconds();
	// displaying sorted array 
	 // printf( "Sorted array: "); 
	  /*   for (int i = 0; i < N; i++) 
	 {   
         printf("index number =%d \t  value=%d \n",i,a[i]);
	//	printf("%d\t",a[i]); 
	 } */  
     
	 // printf("\n\n");
	// time taken by merge sort in seconds 
	 printf("Total time taken=%f\n",t2-t1); ; 

	return 0; 
}