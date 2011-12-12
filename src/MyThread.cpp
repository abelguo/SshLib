//============================================================================
// Name        : MySsh.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <windows.h>


using namespace std;

typedef void* (*Runnable)(void *);



//int pthread_create (pthread_t * thread, pthread_attr_t * attr, void * (* start_routine) (void *), void * arg);


// pthread_t thread; // on cree le thread;
// pthread_create(&thread, NULL, run, (void*) &parametre); // permet d'executer la fonction maFonction en parallele



struct MaStruct {
	int a;
	int b;
};



void* run(void * param) {
	MaStruct m;
	m = *((MaStruct*) param);
	m.a += m.b;

	Sleep(3000);
	cout << "END THREAD : m.a = " << m.a << endl;
	return NULL;
}



int autreMain(void) {

	Runnable p1 = run;

	MaStruct* ms = new MaStruct();
	ms->a = 5;
	ms->b = 7;

	pthread_t thread;
	pthread_create(&thread, NULL, p1, (void*) ms);

	int a(0);
	while (true) {
		cout << "entrer un nombre (ms.a = " << ms->a << ") :" << endl;
		cin >> a;
		cout << "nombre = " << a << endl;
	}

	return EXIT_SUCCESS;
}



//void *TaskCode(void *argument)
//{
//   int tid;
//
//   tid = *((int *) argument);
//   printf("Hello World! It's me, thread %d!\n", tid);
//
//   /* optionally: insert more useful stuff here */
//
//   return NULL;
//}
//
//int main(void)
//{
//   pthread_t threads[NUM_THREADS];
//   int thread_args[NUM_THREADS];
//   int rc, i;
//
//   /* create all threads */
//   for (i=0; i<NUM_THREADS; ++i) {
//      thread_args[i] = i;
//      printf("In main: creating thread %d\n", i);
//      rc = pthread_create(&threads[i], NULL, TaskCode, (void *) &thread_args[i]);
//      assert(0 == rc);
//   }
//
//   /* wait for all threads to complete */
//   for (i=0; i<NUM_THREADS; ++i) {
//      rc = pthread_join(threads[i], NULL);
//      assert(0 == rc);
//   }
//
//   exit(EXIT_SUCCESS);
//}
