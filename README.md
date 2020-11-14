# ADB-Hashing
Implementing different static hashing techniques.
## Usage on linux
### To compile them in one go:
1- use the command "make" in the main directory \
2- run the wanted technique using the command "./run<technique-name>.out"

example: \
`make` \
`./runMultiHashing.out`

### To compile each technique individually:
#### 1) To run the chaining technique:
- First compile using the command " g++ -o run chainingMain.cpp readfile.cpp chaining.h readfile.h
"
- Then run using the command "./run".
- You will be asked to enter the filename of the testcase you want to run (you can find an example in the file "test1.txt") 
#### 2) To run the openAddressing technique:
- First compile using the command " g++ -o run openAddressingMain.cpp readfile.cpp openAddressing.h readfile.h
"
- Then run using the command "./run".
- You will be asked to enter the filename of the testcase you want to run (you can find an example in the file "test1.txt") 

#### 3) To run the multiHashing technique:
- First compile using the command " g++ -o run multiHashingMain.cpp readfile.cpp
"
- Then run using the command "./run".
- You will be asked to enter the filename of the testcase you want to run (you can find an example in the file "test1.txt") 
