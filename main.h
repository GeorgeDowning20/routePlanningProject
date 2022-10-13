//-------------------------------------------------------------
// @path: main.h
// @author: G. Downing
// @created on: 10-10-2022
// @last modified: 10-10-2022
// @version: 1.0
// @language: c
// @Copyright (c) 2022 G. Downing
// @Description: header file for program to find the fastest route to deliver up to 5 packages
//-------------------------------------------------------------

//-------------------------------------------------------------
//                  Preprocessor directives
//-------------------------------------------------------------

#pragma once

#include <stdio.h> //for printf
#include <math.h>  //for sqrt

// Configuration
#define LIMIT_JOB_SIZE // limit the job size to 5

// define constants
#define MAX_POSTAL_CODE sizeof(postal_register) / sizeof(location_t) - 1 // max postal code is the size of the postal register divided by the size of a location_t
#ifdef LIMIT_JOB_SIZE                                                    // if the job size is limited
#define MAX_JOB_SIZE 5                                                   // max job size is 5
#else                                                                    // if the job size is not limited
#define MAX_JOB_SIZE MAX_POSTAL_CODE                                     // max job size is the max postal code
#endif                                                                   // end if
#define MAX_JOURNEY_SIZE MAX_JOB_SIZE + 2                                // max journey size is the max job size plus 2 (start and end)
#define ever \
    ;        \
    ; // for(ever) ultimate readability

// function like macros
#define in_range(x, min, max) ((x) >= (min) && (x) <= (max)) // checks if x is in range of min and max

//-------------------------------------------------------------
//                  Data types and structures
//-------------------------------------------------------------

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

//-------------------------------------------------------------
//                  Function prototypes
//-------------------------------------------------------------

// @function: distance_Between_Locations
// @description: calculates the distance between two locations
// @input: two locations
// @output: distance between the two locations
// @arguments: pointers to two locations
// @return: distance between the two locations
// @author: G. Downing
// @version: 1.0
// @language: c
// @required headers: math.h , main.h for sqrt and location_t
static double distance_Between_Locations(const location_t *a, const location_t *b);

// @function: total_Distance
// @description: calculates the total distance of a journey
// @input: pointer to the postal register, pointer to the job, size of the job
// @output: total distance of the journey
// @arguments: pointer to the job
// @return: total distance of the journey
// @author: G. Downing
// @date: 10-10-2022
// @version: 1.0
// @language: c
// @required headers: main.h for job_t
static float total_Distance(job_t *const job);

// @function: get_job_request
// @description: gets the job request from the user
// @input: pointer to the postal register, pointer to the job, size of the job, user input
// @output: job request, GUI, status
// @arguments: pointer to the job
// @return: status of the function
// @author: G. Downing
// @date: 10-10-2022
// @version: 1.0
// @language: c
// @required headers: stdio.h, main.h for printf, job_t, status_e
static enum status_e get_job_request(job_t *const job);

// @function: trial_order
// @description: calculates the total distance of a trial order and stores shortest order in cache
// @input: pointer to the postal register, pointer to the job, size of the job, pointer to the cache, pointer to the shortest order
// @output: writes the shortest order to the the job object
// @arguments: pointer to the job, operation mode
// @return: void
// @author: G. Downing
// @date: 10-10-2022
// @version: 1.0
// @language: c
// @required headers: main.h for job_t, mode_e
static void trial_order(job_t *const job, const enum mode_e mode);

// @function: swap
// @description: swaps two elements in an array
// @input: pointers to each integer to be swapped
// @output: swaps the two integers
// @return: void
// @author: G. Downing
// @date: 10-10-2022
// @version: 1.0
// @language: c
// @required headers: none
static void swap(int *const a, int *const b);

// @function: _permute
// @description: recursively generates all permutations of the job order and calculates the total distance of each permutation to find the shortest order
// @input: pointer to the postal register, pointer to the job, size of the job, pointer to the cache, pointer to the shortest order
// @output: writes the shortest order to the the job object
// @arguments: pointer to the job, start index, end index
// @return: void
// @author: G. Downing
// @date: 10-10-2022
// @version: 1.0
// @language: c
// @required headers: main.h for job_t
static void _permute(job_t *const job_buff, int l, int r);

// @function: optimize_route
// @description: optimizes the route by generating all permutations of the job order
// @input: pointer to the postal register, pointer to the job, size of the job, pointer to the cache, pointer to the shortest order
// @output: writes the shortest order to the the job object
// @arguments: pointer to the job
// @return: void
// @author: G. Downing
// @date: 10-10-2022
// @version: 1.0
// @language: c
// @required headers: main.h for job_t
static void optimize_route(job_t *const job);

// @function: parse_errors
// @description: prints an error message to the user and determines if the program should exit
// @input: error code
// @output: prints an error message to the user
// @arguments: error code
// @return: status code
// @author: G. Downing
// @date: 10-10-2022
// @version: 1.0
// @language: c
// @required headers: stdio.h, main.h for printf and status_e
static enum status_e parse_errors(const enum status_e status);
