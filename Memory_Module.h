#ifndef INCLUDE_MEMORY_MODULE_H
#define INCLUDE_MEMORY_MODULE_H

typedef struct Temp_Node{

  Day day;
  struct Temp_Node *next;
  
}Temp_Node;


void Init_Memory(void);


void Add_Day_To_Memory(Day theDay);


#endif