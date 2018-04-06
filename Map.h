#pragma once
#include <stdlib.h>
#include <stdio.h>

struct Database {
  void array[500];
  int size;
};
typedef struct Database Database;

struct Map {
  int array[500];
  int size;
};
typedef struct Map Map;

int hash_function(int key);
int size(Map map); 
