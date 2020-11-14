#include<iostream>
using namespace std;
#include "chaining.h"
#include<fstream>

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
			printf("DataItem: key %d, data %d found in the main buckets at index %d\n",key, item.data, result/sizeof(MainBucket));
		} else{
			printf("DataItem: key %d, data %d found in the overflow buckets at record number %d\n",key, item.data, (result-MAIN_FILE_SIZE)/sizeof(o_DataItem));
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
		printf("DataItem: key %d, data %d Deleted successfully\n", key, item.data);
		printf("Delete: No. of searched records:%d\n",count);
	}
}


int main(){
	filehandle = createFile(FILE_SIZE_OV,"chaining.txt");
	initializeAllPointers(filehandle);
	printf("Enter the test case file name....\n");
	string fileName; cin>>fileName;
	ifstream cin(fileName);
	int n; cin>>n;
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
			deleteItem_(key);
			display(filehandle);			
		} 
	}
	close(filehandle);
}

