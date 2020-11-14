#include "readfile.h"
#include "openAddressing.h"

/* Hash function to choose bucket
 * Input: key used to calculate the hash
 * Output: HashValue;
 */

int hashFn1(int key){
    return key % MBUCKETS;
}

int hashFn2(int key){
    return (key % 13) % MBUCKETS;
}



/* Functionality insert the data item into the correct position
 *          1. use the hash function to determine which bucket to insert into
 *          2. search for the first empty space within the bucket
 *          	2.1. if it has empty space
 *          	           insert the item
 *          3. else try second hashing
 *          4. search for the first empty space within the bucket
*               4.1 if it has empty space
*                          inset the item
*                   else
*          	            use OpenAddressing to insert the record
 *          5. return the number of records accessed (searched)
 *
 * Input:  fd: filehandler which contains the db
 *         item: the dataitem which should be inserted into the database
 *
 * Output: No. of record searched
 */
int insertItemMultiHashing(int fd, DataItem item){
    int count = 0;
    DataItem d;
    
    // trying first hashing
    int bucket = hashFn1(item.key);
    int offset = bucket*sizeof(Bucket);
    for(int i = offset; i<offset+sizeof(Bucket); i+=sizeof(DataItem)) {
        count++;
        ssize_t r = pread(fd, &d, sizeof(DataItem), i);
        if(r <= 0){
            perror("Error with pread");
            return -1;
        }
        if(d.valid == 0) {
            r = pwrite(fd, &item, sizeof(DataItem), i);
            if(r <= 0) {
                perror("Error with pwrite");
                return -1;
            }
            return count;
        }
    }

    // no space found in bucket, trying second hashing
    bucket = hashFn2(item.key);
    offset = bucket*sizeof(Bucket);
    for(int i = offset; i<offset+sizeof(Bucket); i+=sizeof(DataItem)) {
        count++;
        ssize_t r = pread(fd, &d, sizeof(DataItem), i);
        if(r <= 0){
            perror("Error with pread");
            return -1;
        }
        if(d.valid == 0) {
            r = pwrite(fd, &item, sizeof(DataItem), i);
            if(r <= 0) {
                perror("Error with pwrite");
                return -1;
            }
            return count;
        }
    }
    
    // no space found in bucket again... applying open addressing...
    count += insertItem(fd, item);
    
    return count;
}


/* Functionality: using a key, it searches for the data item
 *          1. use the hash function to determine which bucket to search into.
 *          2. use the second hash function to determine which bucket to search into.
 *          3. search for the element starting from this bucket and till it find it.
 *          4. return the number of records accessed (searched).
 *
 * Input:  fd: filehandler which contains the db
 *         item: the dataitem which contains the key you will search for
 *               the dataitem is modified with the data when found
 *         count: No. of record searched
 *
 * Output: the in the file where we found the item
 */

int searchItemMultiHashing(int fd, struct DataItem* item, int *count)
{

	//Definitions
	struct DataItem data;   //a variable to read in it the records from the db
	*count = 0;				//No of accessed records
	
    // using first hashing function
    int hashIndex = hashFn1(item->key);  				//calculate the Bucket index
    int startingOffset = hashIndex*sizeof(Bucket);		//calculate the starting address of the bucket
	for(int offset=startingOffset;
            offset<FILESIZE && offset<startingOffset+sizeof(Bucket); 
            offset+=sizeof(DataItem)){

        (*count)++;
        ssize_t r = pread(fd, &data, sizeof(DataItem), offset);
        if(r <= 0) {
            perror("Error in pread");
            return -1;
        }
        if(data.valid == 1 && data.key== item->key) {
            item->data = data.data;
            return offset;
        }
    }
    
    // using second hashing function
    hashIndex = hashFn2(item->key);  				//calculate the Bucket index
    startingOffset = hashIndex*sizeof(Bucket);		//calculate the starting address of the bucket
	for(int offset=startingOffset;
            offset<FILESIZE && offset<startingOffset+sizeof(Bucket); 
            offset+=sizeof(DataItem)){

        (*count)++;
        ssize_t r = pread(fd, &data, sizeof(DataItem), offset);
        if(r <= 0) {
            perror("Error in pread");
            return -1;
        }
        if(data.valid == 1 && data.key== item->key) {
            item->data = data.data;
            return offset;
        }
    }

    // using openAddressing
    return searchItem(fd, item, count);
}