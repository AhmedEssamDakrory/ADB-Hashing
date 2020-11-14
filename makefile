all:
	g++ -w -o runChaining.out chainingMain.cpp readfile.cpp chaining.h readfile.h 
	g++ -w -o runOpenAddressing.out openAddressingMain.cpp readfile.cpp openAddressing.h readfile.h
	g++ -w -o runMultiHashing.out multiHashingMain.cpp readfile.cpp
