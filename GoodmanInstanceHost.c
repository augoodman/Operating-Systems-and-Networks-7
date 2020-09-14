/**
* File:   GoodmanInstanceHost.c
* Implements API for simulating a cloud processing environment.
*
* Completion time: 8 hours (all files)
*
* @author Goodman, Acuna
* @version 2020.09.14
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "InstanceHost.h"

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
typedef struct host {
    int num_instance;
    pthread_mutex_t lock;
} host;

/**
* Initializes the host environment.
*/
host* host_create(){
    host* h = (host*)malloc(sizeof(host));
    pthread_mutex_init(&h->lock, NULL);
    h->num_instance = 0;
    return h;
}

/**
* Shuts down the host environment. Ensures any outstanding batches have
* completed.
*
* @param h A host struct to be destroyed.
*/
void host_destroy(host** h){
    while((*h)->num_instance != 0);
    pthread_mutex_destroy(&(*h)->lock);
    free(*h);
    *h = NULL;
}

/**
* Creates a new server instance (i.e., thread) to handle processing the items
* contained in a batch (i.e., a listed batch of job_node). InstanceHost will
* maintain a batch of active instances, and if the host is requested to
* shutdown, ensures that all jobs are completed.
*
* @param h A host from which to request a server instance.
* @param job_batch_list A batch containing the jobs in a batch to process.
*/
void host_request_instance(host* h, struct job_node* batch){
    printf("LoadBalancer: Received batch and spinning up new instance.\n");
    int result;
    pthread_mutex_lock(&h->lock);
    h->num_instance++;
    while(batch != NULL){
        result = batch->data * batch->data;
        *batch->data_result = result;
        batch = batch->next;
    }
    h->num_instance--;
    pthread_mutex_unlock(&h->lock);
}