#include<iostream>
using namespace std;
#include "openAddressing.h"
#include<fstream>

int filehandle;   //handler for the database file

void insert(int key,int data){
     struct DataItem item ;
     item.data = data;
     item.key = key;
     item.valid = 1;
     int result= insertItem(filehandle,item);  //TODO: implement this function in openAddressing.cpp
     printf("Insert: No. of searched records:%d\n",abs(result));
}

void search(int key){
	struct DataItem item ;
	item.key = key;
	item.valid = 1;
	int count = 0;
	int dummy = 0;
	int Offset = searchItem(filehandle, &item, &count);
	if(Offset == -1){
		printf("Not found!\n");
	} else{
		printf("SearchItem: Item found at Offset: %d,  Data: %d and key: %d\n",Offset,item.data,item.key);
		printf("SearchItem: No. of searched records:%d\n",count);
	}
}

void deleteItem_(int key){



	struct DataItem item ;
	item.key = key;
	int count = 0;
	int Offset= searchItem(filehandle,&item,&count);

	if(Offset == -1){
		printf("not found!");
	} else{
		deleteOffset(filehandle,Offset);
		printf("DataItem: key %d, data %d Deleted successfully\n", key, item.data);
		printf("DataItem: No. of searched records:%d\n",count);
	}
}


int main(){

	printf("OpenAddressingMain runing ..... \n");
	//1. Create Database file or Open it if it already exists, check readfile.cpp
	filehandle = createFile(FILESIZE,"openaddressing");
	//2. Display the database file, check openAddressing.cpp

	printf("Enter the test case file name....\n");
	string fileName; cin>>fileName;
	ifstream cin(fileName);
	int n; cin>>n;
	DisplayFile(filehandle);
	for(int i = 0 ; i < n; ++i){
		string op; cin>>op;
		int key, data;
		if(op == "insert"){
			cin>>key>>data;
			insert(key, data);
			DisplayFile(filehandle);
		}
		else if(op == "search") {
			cin>>key;
			search(key);
			DisplayFile(filehandle);
		}
		else{
			cin>>key;
			deleteItem_(key);
			DisplayFile(filehandle);
		}
	}
	close(filehandle);
}

