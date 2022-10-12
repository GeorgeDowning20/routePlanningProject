//-------------------------------------------------------------
// path: main.c
// author: G. Downing
// created on: 10-10-2022
// last modified: 10-10-2022
// version: 1.0
// language: c
// Copyright (c) 2022 G. Downing
// Description: program to find the fastest route to deliver up to 5 packages
//-------------------------------------------------------------

#include <stdio.h> //for printf
#include <math.h>  //for sqrt

#define MAX_POSTAL_CODE sizeof(postal_register) / sizeof(location_t) // max postal code is the size of the postal register divided by the size of a location_t
#define MAX_JOB_SIZE 5                                               // max job size is 5
#define MAX_JOURNEY_SIZE MAX_JOB_SIZE + 2                            // max journey size is the max job size plus 2 (start and end)
#define ever \
    ;        \
    ; // for(ever) ultimate readability

#define in_range(x, min, max) ((x) >= (min) && (x) <= (max)) // checks if x is in range of min and max

// status enum for the program
enum status_e
{
    OK,                  // program ran successfully
    ERROR,               // program ran unsuccessfully
    INVALID_POSTAL_CODE, // postal code is invalid
    INVALID_JOB_SIZE,    // job size is invalid
    NOT_FOUND,           // postal code not found
    FATAL_ERROR,         // fatal error
    ILLEGAL_INPUT        // illegal input
};

// mode enum for the functions with multiple operating modes
enum mode_e
{
    CONFIGURE, // configure mode
    UPDATE     // update mode
};

// location struct for the postal register
typedef struct location_s
{
    int x;         // x coordinate
    int y;         // y coordinate
    char name[20]; // name of the location
} location_t;

// postal register containing all possible locations for parcels can be grown as needed
const location_t postal_register[] = {
    {0, 0, "Depot"},        // depot
    {9, 8, "location 1"},   // location 1
    {6, 8, "location 2"},   // location 2
    {7, 8, "location 3"},   // location 3
    {1, 1, "location 4"},   // location 4
    {21, 11, "location 5"}, // location 5
    {7, 11, "location 6"},  // location 6
    {11, 11, "location 7"}, // location 7
    {5, 5, "location 8"},   // location 8
    {9, 9, "location 9"},   // location 9
    {8, 1, "location 10"}   // location 10
};

// job struct for the job list
typedef struct job_s
{
    const location_t *postal_register; // pointer to the postal register
    size_t size;                       // size of the job
    int order[MAX_JOURNEY_SIZE];       // order in which the job should be completed
} job_t;

// function: distance_Between_Locations
// description: calculates the distance between two locations
// input: two locations
// output: distance between the two locations
// arguments: pointers to two locations
// return: distance between the two locations
// author: G. Downing
// version: 1.0
// language: c
// required headers: math.h
static double distance_Between_Locations(const location_t *a, const location_t *b)
{
    int dx = a->x - b->x;           // x distance between a and b
    int dy = a->y - b->y;           // y distance between a and b
    return sqrt(dx * dx + dy * dy); // distance between a and b is the square root of the sum of the squares of the x and y distances (a^2 + b^2 = c^2)
}

// function: total_distance
// description: calculates the total distance of a journey
// input: pointer to the postal register, pointer to the job, size of the job
// output: total distance of the journey
// arguments: pointer to the job
// return: total distance of the journey
// author: G. Downing
// date: 10-10-2022
// version: 1.0
// language: c
// required headers: none
static float total_Distance(job_t *const job)
{
    // declare local variables
    float total = 0; // total distance

    // iterate through the job order
    for (int i = 0; i < job->size + 1; i++)
    {
        total += distance_Between_Locations(&job->postal_register[job->order[i]], &job->postal_register[job->order[i + 1]]); // add the distance between the current location and the next location to the total
    }
    return total; // return the total distance
}

// function: get_job_request
// description: gets the job request from the user
// input: pointer to the postal register, pointer to the job, size of the job, user input
// output: job request, GUI, status
// arguments: pointer to the job
// return: status of the function
// author: G. Downing
// date: 10-10-2022
// version: 1.0
// language: c
// required headers: stdio.h
static enum status_e get_job_request(job_t *const job)
{
    if (job == NULL)
        return ERROR; // if the job is null return error

    // get job size
    printf("Enter job size (number from 1-5): ");  // prompt the user for the job size
    if (scanf("%zu", &job->size) == ILLEGAL_INPUT) // if the user input is illegal
        return ILLEGAL_INPUT;                      // return illegal input to be processed by the main function
    if (!in_range(job->size, 1, MAX_JOB_SIZE))     // if the job size is not in range
        return INVALID_JOB_SIZE;                   // return invalid job size to be processed by the main function

    // get list of locations
    printf("\nEnter job order (number from 1-10):\n"); // prompt the user for the job order
    for (int i = 1; i < job->size + 1; i++)
    {
        printf("Enter postal code %d:", i);               // prompt the user for the postal code
        if (scanf("%d", &job->order[i]) == ILLEGAL_INPUT) // if the user input is illegal
            return ILLEGAL_INPUT;                         // return illegal input to be processed by the main function
        if (!in_range(job->order[i], 1, MAX_POSTAL_CODE)) // if the postal code is not in range
            return INVALID_POSTAL_CODE;                   // return invalid postal code to be processed by the main function
    }

    // set start and end locations
    job->order[0] = 0;             // set the first location to the depot
    job->order[job->size + 1] = 0; // set the last location to the depot

    return OK; // return OK to be processed by the main function
}

// function: trial_order
// description: calculates the total distance of a trial order and stores shortest order in cache
// input: pointer to the postal register, pointer to the job, size of the job, pointer to the cache, pointer to the shortest order
// output: writes the shortest order to the the job object
// arguments: pointer to the job, operation mode
// return: void
// author: G. Downing
// date: 10-10-2022
// version: 1.0
// language: c
// required headers: none
static void trial_order(job_t *const job, const enum mode_e mode)
{
    // declare local variables
    static job_t *best_job;     // pointer to the primary job object so the order can be rewritten to produce the shortest order
    static float best_distance; // best distance saved as cache
    float distance;             // distance

    // code to be executed in dependant on the mode
    switch (mode)
    {
    case CONFIGURE:                          // configure mode
        best_job = job;                      // set the best job pointer to point at the job object
        best_distance = total_Distance(job); // set the best distance to the total distance of the job
        break;

    case UPDATE:                        // update mode
        distance = total_Distance(job); // calculate the total distance of the job

        if (distance < best_distance)
        {
            best_distance = distance; // set the best distance to the distance
            *best_job = *job;         // set the best job to the job
        }
        break;

    default: // default mode (should never be reached)
        break;
    }
}

// function: swap
// description: swaps two elements in an array
// input: pointers to each integer to be swapped
// output: swaps the two integers
// return: void
// author: G. Downing
// date: 10-10-2022
// version: 1.0
// language: c
// required headers: none
static void swap(int *const a, int *const b)
{
    int temp = *a; // store a in temporary variable
    *a = *b;       // set a to b
    *b = temp;     // set b to temporary variable
}

// function: permute
// description: recursively generates all permutations of the job order and calculates the total distance of each permutation to find the shortest order
// input: pointer to the postal register, pointer to the job, size of the job, pointer to the cache, pointer to the shortest order
// output: writes the shortest order to the the job object
// arguments: pointer to the job, start index, end index
// return: void
// author: G. Downing
// date: 10-10-2022
// version: 1.0
// language: c
// required headers: none
static void _permute(job_t *const job_buff, int l, int r)
{
    if (l == r) // final recursion
    {
        trial_order(job_buff, UPDATE); // trials the order of the newly proposed job and compares it to the current best order
    }
    else
    {
        for (int i = l; i <= r; i++) // iterate through the job order
        {
            swap(&job_buff->order[l], &job_buff->order[i]); // swap the current location with the next location
            _permute(job_buff, l + 1, r);                   // recursively call the function
            swap(&job_buff->order[l], &job_buff->order[i]); // swap the current location with the next location
        }
    }
}

// function: optimize_route
// description: optimizes the route by generating all permutations of the job order
// input: pointer to the postal register, pointer to the job, size of the job, pointer to the cache, pointer to the shortest order
// output: writes the shortest order to the the job object
// arguments: pointer to the job
// return: void
// author: G. Downing
// date: 10-10-2022
// version: 1.0
// language: c
// required headers: none
static void optimize_route(job_t *const job)
{
    job_t job_buff = *job; // create a copy of the job object

    trial_order(job, CONFIGURE);           // configure trial_order function to store a pointer to the job object and reset cache
    _permute(&job_buff, 1, job_buff.size); // generate all permutations of the job order
}

// function: parse_error
// description: prints an error message to the user and determines if the program should exit
// input: error code
// output: prints an error message to the user
// arguments: error code
// return: status code
// author: G. Downing
// date: 10-10-2022
// version: 1.0
// language: c
// required headers: stdio.h
static enum status_e parse_errors(const enum status_e status)
{
    switch (status) // switch statement to determine which error message to print
    {
    case OK:   // no error
                break; // break out of the switch statement

    case INVALID_POSTAL_CODE:              // invalid postal code
        printf("\nInvalid postal code\n"); // print error message
        break;

    case INVALID_JOB_SIZE:              // invalid job size
        printf("\nInvalid job size\n"); // print error message
        break;

    case ERROR:              // error
        printf("\nError\n"); // print error message
        break;

    case NOT_FOUND:              // not found
        printf("\nNot found\n"); // print error message
        break;

    case FATAL_ERROR:              // fatal error
        printf("\nFatal error\n"); //  print error message
        break;

    case ILLEGAL_INPUT:              // illegal input
        printf("\nIllegal input\n"); // print error message
        break;

    default: // default case (should never be reached)
        break;
    }

    if (status != OK) // if the status is not OK
    {
        printf("exiting...\n\n\n\n\n\n\n\n"); // print error message
        return FATAL_ERROR;                   // return FATAL_ERROR to exit the program from the main function
    }

    return status; // return the status
}

// function: main
// description: main function to run the program. first the user is prompted to enter enter a job. next the route is optimized and displayed
// input: job specification
// output: fastest route to complete job
// arguments: none
// return: void
// author: G. Downing
// date: 10-10-2022
// version: 1.0
// language: c
// required headers: stdio.h, math.h
int main()
{
    for (ever)
    {
        job_t job = {postal_register, 0, {0}}; // create a job object

        printf("Welcome to the delivery service!\nPlease enter your job request:\n\n"); // print welcome message
        if (parse_errors(get_job_request(&job)) == FATAL_ERROR)                         // get the job request from the user if there is an error parse the error and restart the program
            continue;                                                                   // continue to the next iteration of the loop
        //
        optimize_route(&job);                                                           // optimize the route

        printf("\n\nThe shortest possible route to travel is %f:\n", total_Distance(&job)); // print the total distance of the job
        for (int i = 0; i < job.size + 2; i++)                                              // iterate through the job order
        {   //
            printf("%s", job.postal_register[job.order[i]].name);                           // print the name of the postal code
            if (i < job.size + 1)                                                           // if the index is less than the size of the job
                printf(" -> ");                                                             // print the arrow
        }

        printf("\n\n\n\n\n\n\n\n"); // print new lines

        printf("Press any key to continue...\n"); // prompt the user to press any key to continue
        while (getchar() == '\n')
            ; // while the user has not pressed a key
    }
}
