#include<iostream>
using namespace std;
#include "readfile.h"
#include "chaining.h"

int filehandle;   //handler for the database file

void insert(int key,int data){
     struct DataItem item ;
     item.data = data;
     item.key = key;
     item.valid = 1;
     int result= insertChaining(filehandle,item);  //TODO: implement this function in openAddressing.cpp
     printf("Insert: No. of searched records:%d\n",abs(result));
}

void search(int key){
	struct DataItem item ;
	item.key = key;
	item.valid = 1;
	int count = 0;
	int dummy = 0;
	int result = searchChaining(filehandle, &item, &count, dummy, dummy);
	if(result == -1){
		printf("Not found!\n");
	} else{
		if(result < MAIN_FILE_SIZE){
			printf("found in the main buckets at index %d\n", result/sizeof(MainBucket));
		} else{
			printf("found in the overflow buckets at record number %d\n", (result-MAIN_FILE_SIZE)/sizeof(o_DataItem));
		}
		printf("Search: No. of searched records:%d\n",count);
	}
}

void deleteItem_(int key){
	struct DataItem item ;
	item.key = key;
	item.valid = 1;
	int count = 0;
	int result = deleteChaining(filehandle, &item, &count);
	if(result == -1){
		printf("not found!");
	} else{
		printf("Deleted successfully");
		printf("Delete: No. of searched records:%d\n",count);
	}
}


int main(){
	filehandle = createFile(FILE_SIZE_OV,"chaining.txt");
	initializeAllPointers(filehandle);
	insert(1, 20);
	insert(2,30);
	insert(11, 50);
	insert(21, 70);
	insert(22,80);
	insert(12, 44);
	insert(31, 31);
	insert(41, 41);
	insert(32, 32);
	display(filehandle);
	search(1);
	search(2);
	search(11);
	search(21);
	search(22);
	search(12);
	search(31);
	search(41);
	search(32);
	search(15);
	deleteItem_(1);
	deleteItem_(31);
	deleteItem_(21);
	display(filehandle);
}

