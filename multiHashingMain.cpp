//============================================================================
// Name        : hashskeleton.cpp
// Author      : 
// Version     :
// Copyright   : Code adapted From https://www.tutorialspoint.com/
// Description : Hashing using open addressing
//============================================================================

#include "readfile.h"
#include "multiHashing.h"
#include "openAddressing.h"


void insert(int key,int data);
int deleteItem(int key);
struct DataItem * search(int key);


int filehandle;   //handler for the database file

int main(){

//here we create a sample test to read and write to our database file

  //1. Create Database file or Open it if it already exists, check readfile.cpp
   filehandle = createFile(FILESIZE,"multiHashing");
  //2. Display the database file, check openAddressing.cpp
   DisplayFile(filehandle);

  
  //3. Add some data in the table
   insert(1, 20);
   insert(2, 70);
   insert(42, 80);
   insert(4, 25);
   insert(12, 44);
   insert(14, 32);
   insert(17, 11);
   insert(13, 78);
   insert(37, 97);
   insert(11, 34);
   insert(22, 730);
   insert(46, 840);
   insert(9, 83);
   insert(21, 424);
   insert(41, 115);
   insert(71, 47);
   insert(31, 92);
   insert(73, 45);

   //4. Display the database file again
   DisplayFile(filehandle);

   //5. Search the database
   search(13);

   //6. delete an item from the database
   deleteItem(31);

   //7. Display the final data base
   DisplayFile(filehandle);
   // And Finally don't forget to close the file.
   close(filehandle);
   return 0;



}

/* functionality: insert the (key,data) pair into the database table
                  and print the number of comparisons it needed to find
    Input: key, data
    Output: print statement with the no. of comparisons
*/
void insert(int key,int data){
	struct DataItem item;
	item.data = data;
	item.key = key;
	item.valid = 1;
	int result = insertItemMultiHashing(filehandle,item);  
	printf("Insert: No. of searched records:%d\n", abs(result));
}

/* Functionality: search for a data in the table using the key

   Input:  key
   Output: the return data Item
*/
struct DataItem * search(int key){
	struct DataItem* item = (struct DataItem *) malloc(sizeof(struct DataItem));
	item->key = key;
	int diff = 0;
	int Offset = searchItemMutliHashing(filehandle, item, &diff); //this function is implemented for you in openAddressing.cpp
	printf("Search: No of records searched is %d\n",diff);
	if(Offset < 0)  //If offset is negative then the key doesn't exists in the table
		printf("Item not found\n");
	else
		printf("Item found at Offset: %d,  Data: %d and key: %d\n",Offset,item->data,item->key);
	return item;
}

/* Functionality: delete a record with a certain key

   Input:  key
   Output: return 1 on success and -1 on failure
*/
int deleteItem(int key){
	struct DataItem* item = (struct DataItem *) malloc(sizeof(struct DataItem));
	item->key = key;
	int diff = 0;
	int Offset = searchItem(filehandle, item, &diff);
	printf("Delete: No of records searched is %d\n",diff);
	if(Offset >= 0){
		return deleteOffset(filehandle, Offset);
	}
	return -1;
}
