#include <iostream>
using namespace std;
#include "CircularDynamicArray.cpp"


int main(){
	CircularDynamicArray<float> C(10);
C.stableSort();
C.reverse();
for (int i=0; i< C.length();i++) cout << C[i] << " ";cout << endl;
cout << C.binSearch(75) << endl;
};