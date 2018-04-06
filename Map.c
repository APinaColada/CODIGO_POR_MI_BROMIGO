#include "Map.h"

Database *createDatabase() {
  Database *DB = malloc(sizeof(Database));
  DB->size     = 0;
  DB->array    = malloc(sizeof(void*)*500);
}

void printDB(Database DB) {
  int size = DB->size;
  for (int i=0; i<size; i++) {
    printf("1) ");
  }
}
