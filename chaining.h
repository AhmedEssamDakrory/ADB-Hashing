#include "readfile.h"
#define OVERFLOW_BUCKETS 10
#define FILE_SIZE_OV OVERFLOW_BUCKETS*sizeof(OverflowBucket)+MBUCKETS*sizeof(MainBucket)
#define MAIN_FILE_SIZE MBUCKETS*sizeof(MainBucket)
struct o_DataItem {
   int valid;    //) means invalid record, 1 = valid record
   int data;
   int key;
   int pointer;
};

struct MainBucket{
    struct DataItem  dataItem[RECORDSPERBUCKET];
    int pointer;
};

struct OverflowBucket{
    struct o_DataItem  dataItem[OVERFLOW_BUCKETS];
};

int hashCode_(int key){
   return key % MBUCKETS;
}

int getFirstUnsedLocation(int fd, int& count){
    int valid;
    int offset = MBUCKETS*sizeof(MainBucket);
    int idx = 0;
    for(; offset < FILE_SIZE_OV; offset += sizeof(o_DataItem), ++idx){
        ++count;
         ssize_t result = pread(fd, &valid, sizeof(int), offset);
         if(result <= 0){
             perror("some error occurred in pread");
             return result;
         } else if(valid == 0){
             return idx;
         }
    }
    printf("overflow locations full!");
    return -1;
}

int getEndOfTheChain(int fd, int& count, int offset){
    int pointer = offset;
    do{
        ++count;
        offset = pointer*sizeof(o_DataItem)+MAIN_FILE_SIZE;
        ssize_t result = pread(fd, &pointer, sizeof(int), offset+sizeof(int)*3);
        if(result <= 0){
            perror("some error occurred in pread");
            return result;
        }
    }while(pointer != -1);
    return offset;
}

int insertChaining(int fd, DataItem item){
    int valid, pointer;
    o_DataItem data;
    int count = 0; // number of accessed records
    int hashIndex = hashCode_(item.key);
    int offset = hashIndex*sizeof(MainBucket);
    ssize_t result;
    for(int i = 0; i < RECORDSPERBUCKET; ++i){
        ++count;
        result = pread(fd, &valid, sizeof(int), offset);
        if(result <= 0){
            perror("some error occurred in pread");
			return -1;
        }
        else if(valid == 0 ){
            result = pwrite(fd, &item, sizeof(DataItem), offset);
            if(result <= 0){
                 perror("some error occurred in pwrite");
			     return -1;
            }
            return count;
        }
        offset += sizeof(DataItem);
    }
    ++count;
    int next = getFirstUnsedLocation(fd, count);
    if(next < 0){
        return -1;
    }
    result = pread(fd, &pointer, sizeof(int), offset);
    if(result <= 0){
         perror("some error occurred in pread");
		 return -1;
    } else if(pointer == -1){
        result = pwrite(fd, &next, sizeof(int), offset);
        if(result <= 0){
            perror("some error occurred in pwrite");
            return -1;
        }
    } else{
        offset = getEndOfTheChain(fd, count, pointer);
        if(offset < 0) return -1;
        ssize_t result = pwrite(fd, &next, sizeof(int), offset+3*sizeof(int));
        if(result <= 0){
            perror("some error occurred in pwrite");
            return -1;
        }
    }
    data.data = item.data;
    data.key = item.key;
    data.pointer = -1;
    data.valid = 1;
    result = pwrite(fd, &data, sizeof(o_DataItem), next*sizeof(o_DataItem)+MAIN_FILE_SIZE);
    if(result <= 0){
        perror("some error occurred in pwrite");
        return -1;
    }
    return count;
}

int searchTheOverflowList(int fd, struct DataItem* item, int *count, int offset){
	o_DataItem data;
	ssize_t result;
	while(offset != -1){
		result = pread(fd, &data, sizeof(o_DataItem), offset*sizeof(o_DataItem)+MAIN_FILE_SIZE);
		if(result <= 0){
			perror("error reading file");
			return -1;
		} else if(data.valid == 1 && data.key == item->key){
			return offset*sizeof(o_DataItem)+MAIN_FILE_SIZE;
		}
		offset = data.pointer;
	}
	return -1;
}

int searchChaining(int fd, struct DataItem* item, int *count){
	int hashIndex = hashCode_(item->key);
	int offset = hashIndex*sizeof(MainBucket);
	DataItem data;
	ssize_t result;
	int pointer;
	for(int i = 0 ; i < RECORDSPERBUCKET; ++i){
		++(*count);
		result = pread(fd, &data, sizeof(DataItem), offset);
		if(result <= 0){
			perror("error reading file");
			return -1;
		} else if(data.valid == 1 && data.key == item->key){
			return offset;
		}
		offset += sizeof(DataItem);
	}
	++(*count);
	result = pread(fd, &pointer, sizeof(int), offset);
	if(result <= 0){
		perror("error reading file");
		return -1;
	} else if(pointer == -1){
		return -1; // not found;
	} else{
		offset = searchTheOverflowList(fd, item, count, pointer);
		if(offset == -1) return -1;
		return offset;
	}
}

void display(int fd){
    printf("Main Buckets....\n");
    int offset = 0;
    DataItem data;
    o_DataItem data2;
    int pointer;
    ssize_t result;
    for(int i = 0 ; i < MBUCKETS; ++i){
        for(int j = 0; j < RECORDSPERBUCKET; ++j){
            result = pread(fd, &data, sizeof(DataItem), offset);
            if(result <= 0){
                perror("error!");
                return ;
            } else {
                printf("dataItem: %d %d\n", data.key, data.data);
            }
            offset += sizeof(DataItem);
        }
       result = pread(fd, &pointer, sizeof(int), offset);
       if(result <= 0){
           perror("error!");
           return ;
       } else{
           printf("EndOfBucket: %d\n", pointer);
       }
       offset += sizeof(int);
    }
    printf("Overflow Buckets...");
    for( ;offset < FILE_SIZE_OV; offset += sizeof(o_DataItem)){
    	 result = pread(fd, &data2, sizeof(o_DataItem), offset);
    	 printf("Data Item : %d %d %d\n", data2.key, data2.data, data2.pointer);
    }
}

void initializeAllPointers(int fd){
    int offset = sizeof(MainBucket);
    int p = -1;
    for(int i = 0 ; i < MBUCKETS; ++i){
        ssize_t result = pwrite(fd, &p, sizeof(int), offset-sizeof(int));
        offset += sizeof(MainBucket);
    }
    offset = MAIN_FILE_SIZE+sizeof(o_DataItem);
    for(; offset < FILE_SIZE_OV; offset += sizeof(o_DataItem)){
    	ssize_t result = pwrite(fd, &p, sizeof(int), offset-sizeof(int));
    }
}



