
#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

process_stat *generateProcessStat(process* proc);

int compareRunTime(void *data1, void *data2) {
    process_stat *prs1 = (process_stat *) data1;
    process_stat *prs2 = (process_stat *) data2;
    if(((process *)prs1->proc)->runTime < ((process *)prs2->proc)->runTime) {
        return -1;
    } else {
        return 1;
    }
}

average_stats shortestJobFirstNP(linked_list *processes) {
    int time = 0;

    //Create Process Queue
    queue *processQueue = (queue *)createQueue();
    node *procPtr = processes->head;
    if(processes->head == NULL) {
        fprintf(stderr,"No Process to schedule\n");
    }

    process_stat *scheduledProcess = NULL;

    linked_list *ll = createLinkedList();
    printf("\n\n\n==================================================================================================================================\n");
    printf("\nShortest Job First Algorithm:\n");
    printf("Order of Processes in Execution: ");
    
    //keep checking while the process queue is empty or the time quanta is less than 100.
    for (time = 0; time < 100 || scheduledProcess != NULL; time++) {
        // Check for incoming new processes and enqueue them
        while (procPtr != NULL && ((process *)(procPtr->data))->arrivalTime <= time) {
            process *newProcess = (process *)(procPtr->data);
            enqueue(processQueue, generateProcessStat(newProcess));
            sort(processQueue, compareRunTime);
            procPtr = procPtr->next;
        }

        // If there is no scheduled process now then check the process queue and schedule it.
        if(scheduledProcess == NULL && processQueue->size > 0) {
            scheduledProcess = (process_stat *) dequeue(processQueue);
        }

        if(scheduledProcess != NULL) {
            process * proc = scheduledProcess->proc;

            //add the current running process to the time chart
            printf("%c",scheduledProcess->proc->pid);

            //update the current processes stat
            if(scheduledProcess->startTime == -1) {
                scheduledProcess->startTime = time;
            }
            scheduledProcess->runTime++;

            if(scheduledProcess->runTime >= proc->runTime) {
                scheduledProcess->endTime = time;
                //adding the scheduled process
                addNode(ll,scheduledProcess);
                scheduledProcess = NULL;
            }
        } else {
            printf("_");
        }
        //Increasing the time
        time++;
    }
    printf("\n");

    //Printing the stat of the process
    return printPolicyStat(ll);
    
}
