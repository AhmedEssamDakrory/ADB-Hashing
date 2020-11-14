//============================================================================
// Name        : hashskeleton.cpp
// Author      : 
// Version     :
// Copyright   : Code adapted From https://www.tutorialspoint.com/
// Description : Hashing using open addressing
//============================================================================

#include<iostream>
#include<fstream>

using namespace std;

#include "readfile.h"
#include "multiHashing.h"

void insert(int key,int data);
int deleteItem(int key);
struct DataItem * search(int key);


int filehandle;   //handler for the database file

int main(){

   printf("Multi-Hashing running ..... \n");
	// Create Database file or Open it if it already exists, check readfile.cpp
	filehandle = createFile(FILESIZE,"multihashing.db");
	
	printf("Enter the test case file name: ");
	string fileName; cin>>fileName;
	ifstream cin(fileName);
	int n; cin>>n;
   printf("Displaying file before operations.....\n");
	DisplayFile(filehandle);
	for(int i = 0 ; i < n; ++i){
		string op; cin>>op;
		int key, data;
		if(op == "insert"){
			cin>>key>>data;
			insert(key, data);
		}
		else if(op == "search") {
			cin>>key;
			search(key);
		}
		else{
			cin>>key;
			deleteItem(key);
		}
	}
   printf("\nDisplaying file after operations.....\n");
   DisplayFile(filehandle);
	
   close(filehandle);
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
	int Offset = searchItemMultiHashing(filehandle, item, &diff); //this function is implemented for you in openAddressing.cpp
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
	int Offset = searchItemMultiHashing(filehandle, item, &diff);
	printf("Delete: No of records searched is %d\n",diff);
	if(Offset >= 0){
		return deleteOffset(filehandle, Offset);
	}
	return -1;
}
