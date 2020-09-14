/**
* File:   GoodmanLoadBalancer.c
* Implements API for balancing server traffic load.
*
* Completion time: 8 hours (all files)
*
* @author Goodman, Khan, Acuna
* @version 2020.09.14
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "LoadBalancer.h"
#include "InstanceHost.h"

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
typedef struct list {
    struct job_node* head;
    int size;
} batch;

typedef struct balancer {
    int batch_size;
    batch* jobs;
    host* host;
    pthread_mutex_t lock;
}balancer;

////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATIONS FOR PRIVATE FUNCTIONS
batch* create_batch();
void destroy_batch(batch** batch);
void add_job(batch* batch, int user_id, int data, int* data_return);
void remove_job(batch* batch);

/**
 * Initializes the load balancer. Takes batch size as parameter.
 *
 * @param batch_size The max batch size for this load balancer.
 */
balancer* balancer_create(int batch_size){
    balancer* lb = (balancer*)malloc(sizeof(balancer));
    pthread_mutex_init(&lb->lock, NULL);
    lb->batch_size = batch_size;
    lb->jobs = create_batch();
    lb->host = host_create();
    return lb;
}

/**
 * Shuts down the load balancer. Ensures any outstanding batches have
 * completed.
 *
 * @param lb A load balancer struct to be shut down.
 */
void balancer_destroy(balancer** lb){
    while((*lb)->jobs->size != 0) {
        host_request_instance((*lb)->host, (*lb)->jobs->head);
        while ((*lb)->jobs->head != NULL) {
            remove_job((*lb)->jobs);
        }
    }
    host_destroy(&(*lb)->host);
    destroy_batch(&(*lb)->jobs);
    pthread_mutex_destroy(&(*lb)->lock);
    free(*lb);
    *lb = NULL;
}

/**
 * Adds a job to the load balancer. If enough jobs have been added to fill a
 * batch, will request a new instance from InstanceHost. When job is complete,
 * data_return will be updated with the result.
 *
 * @param lb A load balancer to which to send a job.
 * @param user_id The id of the user making the request.
 * @param data The data the user wants to process.
 * @param data_return A pointer to a location to store the result of processing.
 */
void balancer_add_job(balancer* lb, int user_id, int data, int* data_return){
    printf("LoadBalancer: Received new job from user #%d to process data=%d and store it at %p.\n",
           user_id, data, data_return);
    pthread_mutex_lock(&lb->lock);
    add_job(lb->jobs, user_id, data, data_return);
    if(lb->jobs->size == lb->batch_size) {
        host_request_instance(lb->host, lb->jobs->head);
        while(lb->jobs->head != NULL){
            remove_job(lb->jobs);
        }
    }
    pthread_mutex_unlock(&lb->lock);
}

/**
 * Creates a new batch (list of job nodes).
 */
batch* create_batch(){
    batch* new_batch = (batch*)malloc(sizeof(batch));
    new_batch->size = 0;
    return new_batch;
}

/**
 * Destroys a batch (list of job nodes).
 *
 * @param batch A batch struct to be destroyed.
 */
void destroy_batch(batch** batch){
    free(*batch);
    *batch = NULL;
}

/**
 * Adds a job node to a batch struct.
 *
 * @param batch A batch struct to be added to.
 * @param user_id The id of the user making the request.
 * @param data The data the user wants to process.
 * @param data_return A pointer to a location to store the result of processing.
 */
void add_job(batch* batch, int user_id, int data, int* data_return){
    struct job_node* new_job = (struct job_node*)malloc(sizeof(struct job_node));
    new_job->user_id = user_id;
    new_job->data = data;
    new_job->data_result = data_return;
    new_job->next = NULL;
    if(batch->size == 0)
        batch->head = new_job;
    else{
        struct job_node* iter = batch->head;
        while(iter->next != NULL)
            iter = iter->next;
        iter->next = new_job;
        iter = NULL;
    }
    batch->size++;
}

/**
 * Removes a job node from a batch list.
 *
 * @param batch A batch struct from which to remove a job node.
 */
void remove_job(batch* batch){
    struct job_node* temp = batch->head;
    batch->head = batch->head->next;
    free(temp);
    temp = NULL;
    batch->size--;
}