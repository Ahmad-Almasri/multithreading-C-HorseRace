/*

--> Name : Ahmad Almasri -- University of Calgary -- 30114233

--> Multithreading programming in c [Pthreads]

--> This program simulates a horse race
  using N+1 threads < one Main_thread, and N number of threads>


*/


#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "stdbool.h"
#include "pthread.h"
#include "unistd.h"
//-----------------------------
#define finishLine 25
//-----------------------------
struct raceStruct{
        int *progress, *rank;
        time_t *finishTime;
        bool raceFinished;
}obj;
//-----------------------------
// == Declare Functions ==
void * mainThread(void *);
void arrayInitializer(int);
void * raceFunction(void *);
void display(int, time_t);
bool isRaceFinished(int);
void rankHorses(int, time_t);
void displayRanks(int, int *);
//-----------------------------


int main(int argc, char const *argv[]){

        pthread_t main_t;
        
        pthread_create(&main_t, NULL, mainThread, NULL);

        pthread_join(main_t, NULL);
        
        return 0;
}

//-----------------------------

void * mainThread(void * var){

        srand(time(NULL));

        // scan :: numberOfHorses
        int numberOfHorses;
        printf("Enter the number of <Horses> ?\n");
        scanf("%d", &numberOfHorses);

        // initialize :: arrays
        arrayInitializer(numberOfHorses);

        // declare :: threads[] , ids[]
        pthread_t horses[numberOfHorses];
        int ids[numberOfHorses];

        // time :: start <timer> for all horses
        time_t startTime = time(0);

        // create :: threads
        // initialize :: ids
        for(int i=0; i<numberOfHorses; i++){
                ids[i] = i;
                pthread_create(&horses[i], NULL, raceFunction, &ids[i]);
        }

        // while loop :: game logic <main thread>
        while(!obj.raceFinished){
                // Display :: raceProgress
                display(numberOfHorses, startTime);
                rankHorses(numberOfHorses, startTime);
                // rank :: finished horses
                // if(Onefinished()) rank();
                obj.raceFinished = isRaceFinished(numberOfHorses);
                sleep(1);
        }

        // :: join all threads
        for(int i=0; i<numberOfHorses; i++){
                pthread_join(horses[i],NULL);
        }

	display(numberOfHorses, startTime);
        // display :: the final ranking
        displayRanks(numberOfHorses, ids);
}

//-----------------------------

// allocates space for all arrays in the structure
void arrayInitializer(int numberOfHorses){
        obj.progress = (int *) malloc(numberOfHorses*sizeof(int));
        obj.rank = (int *) malloc(numberOfHorses*sizeof(int));
        obj.finishTime = (time_t *) malloc(numberOfHorses*sizeof(time_t));

        for(int i=0; i<numberOfHorses; i++)
        {
                obj.progress[i] = 0;
                obj.rank[i] = 0;
                obj.finishTime[i] = -1;
        }
}
//-----------------------------

// logic for All N threads (horses) 
void * raceFunction(void * horseId){
        int id = *(int*) horseId;
        for(int i = 0; i<finishLine; i++)
        {
                // sleep :: [0,5] seconds
                sleep(rand()%3);
                // increment progress by one
                obj.progress[id]++;
        }
        // get :: currentTime
        obj.finishTime[id] = time(0);
        // add variable to trace number of finshed
}
//-----------------------------

// this function is to display the status of the race
void display(int numberOfHorses, time_t startTime)
{
  int val; char symbol;
  system("clear");
  for(int i=0; i<numberOfHorses; i++){
        val = finishLine; symbol = '*';
        if(obj.progress[i] != finishLine)
        {
                val = obj.progress[i];
                symbol = '>';
        }
        printf("Horse %3d :: ", i);
        for(int j=0; j<val; j++)
        {
                printf("-");
        }
        if(symbol == '*')
        {
                printf("%c", symbol);
                printf("  Time[%3d]\n", obj.finishTime[i] - startTime );
        }
        else
        {
                printf("%c\n", symbol );
        }
  }
}
//-----------------------------

// if race is finished .. return true <Stop the game>
bool isRaceFinished(int numberOfHorses){
        for(int i=0; i<numberOfHorses; i++)
        {
                if(obj.progress[i]!=finishLine)
                        return false;
        }
        return true;
}
//----------------------------

// this function ranks all finished horses by giving them a unique index

void rankHorses(int numberOfHorses, time_t startTime){
		// static :: index :: keep track of horse ranking
        static int index = 1;
        for(int i=0; i<numberOfHorses; i++){
                // IF :: progress = finishLine && rank[i] = 0
                if(obj.progress[i] == finishLine && obj.rank[i] == 0){
                        obj.rank[i] = index++;
                }
        }


        // IF :: two horse finished at the same time --> {BREAK THE TIE IF EXIST}
        // i && j represents their Ids 
        // is are lower ids && js are higher ids
	int temp = 0;
        for(int i=0; i<numberOfHorses; i++)
            for(int j=i+1; j<numberOfHorses && obj.rank[i]!=0; j++)
                if((obj.finishTime[i]-startTime)==(obj.finishTime[j]-startTime))
                    if(obj.rank[i]>obj.rank[j]){
                        temp = obj.rank[i];
                        obj.rank[i] = obj.rank[j];
                        obj.rank[j] = temp; 
                    }
}
//-----------------------------

// finally after all horses are finished display the rank of them
void displayRanks(int numberOfHorses, int *id){
        int temp = 0;

	// sort all hoses by their ranks
    for(int i=0; i<numberOfHorses; i++)
        for(int j=i+1; j<numberOfHorses; j++)
           if(obj.rank[i] > obj.rank[j]){         
            temp = obj.rank[i];
            obj.rank[i] = obj.rank[j];
            obj.rank[j] = temp;

            temp = id[i];
            id[i] = id[j];
            id[j] = temp;
        }
        
    // display all sorted ranks 
    for(int i=0; i<numberOfHorses; i++){
        printf("Horse %3d -- Rank %3d\n", id[i], obj.rank[i] );
    }
}
//-----------------------------
