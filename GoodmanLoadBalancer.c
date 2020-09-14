/**
 * Header file definition of functions to simulate a load balancer.
 *
 * @author Khan, Acuna
 * @version 1.1
 */

#include "GoodmanLoadBalancer.h"
#include "GoodmanInstanceHost.h"

//forward declarations for (public) functions

/**
 * Initializes the load balancer. Takes batch size as parameter.
 */
balancer* balancer_create(int batch_size);

/**
 * Shuts down the load balancer. Ensures any outstanding batches have
 * completed.
 */
void balancer_destroy(balancer** lb);

/**
 * Adds a job to the load balancer. If enough jobs have been added to fill a
 * batch, will request a new instance from InstanceHost. When job is complete,
 * *data_return will be updated with the result.
 *
 * @param user_id the id of the user making the request.
 * @param data the data the user wants to process.
 * @param data_return a pointer to a location to store the result of processing.
 */
void balancer_add_job(balancer* lb, int user_id, int data, int* data_return);