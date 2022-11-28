#ifndef _CHILD_FUNCTIONS_H
#define _CHILD_FUNCTIONS_H

#include "db.h"

/**
 * Takes care of the "select" type of queries
 **/ 
bool select(int* fd_s,database_t* db);
/**
 * Takes care of the "insert" type of queries
 **/ 
bool insert(int* fd_i,database_t* db);
/**
 * Takes care of the "delete" type of queries
 **/ 
bool del(int* fd_d,database_t* db);
/**
 * Takes care of the "update" type of queries
 **/ 
bool update(int* fd_u,database_t* db);
#endif  // _CHILD_FUNCTIONS_H
