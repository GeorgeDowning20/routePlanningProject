//-------------------------------------------------------------
// @path: postal_register.inc
// @author: G. Downing
// @created on: 10-10-2022
// @last modified: 10-10-2022
// @version: 1.0
// @language: c
// @Copyright (c) 2022 G. Downing
// @Description: database containing postal register to be expanded as required
//-------------------------------------------------------------

//-------------------------------------------------------------
//                  Postal register
//-------------------------------------------------------------

// postal register containing all possible locations for parcels can be grown as needed
const location_t postal_register[] = {
    {0, 0, "Depot"},        // Depot
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