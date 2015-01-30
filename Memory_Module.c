#include <stdlib.h>
#include <stdio.h>
#include "Temp_Module.h"
#include "Memory_Module.h"
#include "at91sam3x8.h"

int Temp_Node_Size = sizeof(Temp_Node);
Temp_Node *head;
Temp_Node *tail;
Temp_Node *first_node;
Temp_Node *last_node;

void Init_Memory(void){
  
  head = (Temp_Node*)malloc(sizeof(Temp_Node_Size));
  Temp_Node *temp_node;
  Temp_Node *New_Node;
  temp_node = head;
  New_Node = (Temp_Node *)malloc(Temp_Node_Size);
  do{
  temp_node->next = New_Node;
  temp_node = temp_node->next;
  last_node = temp_node;
  New_Node = (Temp_Node *)malloc(Temp_Node_Size);
  }while(New_Node!= NULL);
 tail = head;
 first_node = head;
 last_node->next = head;
}




void Add_Day_To_Memory(Day theDay){

  tail->day = theDay;
  
  if(tail->next == head){
    head = head->next;
  }
  tail = tail->next;    
   
  }

