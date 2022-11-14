//-------------------------------------------------------------
/// @file: main.h
/// @author G. Downing
/// @date 10-10-2022
/// @copyright (c) 2022 G. Downing
/// @details Header file for program to find the fastest route to deliver up to 5 packages
/// @details documentation: https://georgedowning20.github.io/routePlanningProject/index.html
//-------------------------------------------------------------

//-------------------------------------------------------------
//                  Preprocessor directives
//-------------------------------------------------------------

#pragma once

#include <stdio.h> //for printf
#include <math.h>  //for sqrt

// Configuration
#define LIMIT_JOB_SIZE ///< limit the job size to 5

// define constants
#define MAX_POSTAL_CODE sizeof(postal_register) / sizeof(location_t) - 1 ///< max postal code is the size of the postal register divided by the size of a #location_t
#ifdef LIMIT_JOB_SIZE                                                    ///< if the job size is limited
#define MAX_JOB_SIZE 5                                                   ///< max job size is 5
#else                                                                    ///< if the job size is not limited
#define MAX_JOB_SIZE MAX_POSTAL_CODE                                     ///< if the job size is not limited max job size is the max postal code
#endif                                                                   // end if
#define MAX_JOURNEY_SIZE MAX_JOB_SIZE + 2                                ///< max journey size is the max job size plus 2 (start and end)
#define ever \
    ;        \
    ; ///< if the job size is not limited for(ever) ultimate readability

// function like macros
#define in_range(x, min, max) ((x) >= (min) && (x) <= (max)) ///< checks if x is in range of min and max

//-------------------------------------------------------------
//                  Data types and structures
//-------------------------------------------------------------

///< Status enum for the program
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

///< mode enum for the functions with multiple operating modes
enum mode_e
{
    CONFIGURE, // configure mode
    UPDATE     // update mode
};

///< location struct for the postal register
typedef struct location_s
{
    __int8_t x;    ///< x coordinate
    __int8_t y;    ///< y coordinate
    char name[20]; ///< name of the location
} location_t;

///< job struct for the job list
typedef struct job_s
{
    const location_t *postal_register; ///< pointer to the postal register
    size_t size;                       ///< size of the job
    int order[MAX_JOURNEY_SIZE];       ///< order in which the job should be completed
} job_t;

//-------------------------------------------------------------
//                  Function prototypes
//-------------------------------------------------------------

/// @brief calculates the distance between two locations
/// @details calculates the distance between two locations using the pythagorean theorem.
/// @param[in] a pointer to the first location
/// @param[in] b pointer to the second location
/// @return distance between the two locations
/// @author G. Downing
/// @date 10-10-2022
static inline double distance_Between_Locations(const location_t *const a, const location_t *const b);

/// @brief Swaps two elements in an array
/// @details Swaps two elements in an array using a temporary variable to store one of the elements while the other is being overwritten by the other element and then the temporary variable is used to overwrite the other element with the first element. This is done to avoid overwriting the first element before it can be used to overwrite the second element. This function is used to swap the order of the job list.
/// @param[in] a pointer to the first element
/// @param[in] b pointer to the second element
/// @param[out] a pointer to the second element
/// @param[out] b pointer to the first element
/// @return void
/// @author G. Downing
/// @date 10-10-2022
static inline void swap(int *const a, int *const b);

/// @brief calculates the total distance of a journey
/// @details calculates the total distance of a journey by adding the distance between each location in the journey using the #distance_Between_Locations function
/// @param[in] job pointer to the job
/// @return total distance of the journey
/// @author G. Downing
/// @date 10-10-2022
static float total_Distance(const job_t *const job);

/// @brief gets the job request from the user
/// @details gets the job request from the user by asking for the size of the job and then asking for the postal codes of the locations in the job. The postal codes are then checked to make sure they are valid and then the job is updated with the postal codes.
/// @param[in] job pointer to the job
/// @param[in] User-Input  String containing the user input
/// @return Status of the function
/// @author G. Downing
/// @date 10-10-2022
static enum status_e get_job_request(job_t *const job);

/// @brief calculates the total distance of a trial order and stores shortest order in cache
/// @details calculates the total distance of a trial order and stores shortest order in cache by calculating the total distance of the trial order using the #total_Distance function and then comparing it to the shortest distance in the cache. If the trial order is shorter than the shortest distance in the cache then the shortest distance in the cache is updated with the trial order distance and the shortest order in the cache is updated with the trial order.
/// @param[in] mode operation mode (polymorphically used to determine if the function is being used to configure the job or to update the job)
/// @return void
/// @author G. Downing
/// @date 10-10-2022
static void trial_order(job_t *const job, const enum mode_e mode);

/// @brief recursively generates all permutations of the job order and calculates the total distance of each permutation to find the shortest order
/// @details recursively generates all permutations of the job order and calculates the total distance of each permutation to find the shortest order by using a recursive function to generate all permutations of the job order. The recursive function is called with the start index of the job order and the end index of the job order. The recursive function then swaps the start index with each index from the start index to the end index and then calls itself with the start index incremented by 1 and the end index. This continues until the start index is equal to the end index. When the start index is equal to the end index the recursive function calls the #trial_order function to calculate the total distance of the trial order and store the shortest order in the cache.
/// @param[in,out] job pointer to the job. The job order is updated with the shortest order in the cache.
/// @param[in] l start index
/// @param[in] r end index
/// @return void
/// @author G. Downing
static void _permute(job_t *const job_buff, const __uint8_t l, const __uint8_t r);

/// @brief optimizes the route by generating all permutations of the job order
/// @details optimizes the route by generating all permutations of the job order by calling the #_permute function with the start index of the job order and the end index of the job order.
/// @param[in,out] job pointer to the job. The job order is updated with the shortest order in the cache.
/// @return void
/// @author G. Downing
/// @date 10-10-2022
static void optimize_route(job_t *const job);

/// @brief prints an error message to the user and determines if the program should exit
/// @details prints an error message to the user and determines if the program should exit by printing an error message to the user based on the error code and then returning the error code. If the error code is FATAL_ERROR then the program exits.
/// @param[in] error code
/// @param[out] 'printf' prints an error message to the user
/// @return status code
/// @author G. Downing
/// @date 10-10-2022
static enum status_e parse_errors(const enum status_e status);
