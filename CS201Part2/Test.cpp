#include <iostream>
using namespace std;

#include "RBTree.cpp"

int main()
{
    int keyRay[] = {0,1,2,3,4,5,6,7,8,9,10,11,12};
    int valueRay[] = {0,1,2,3,4,5,6,7,8,9,10,11,12};
    
    //RBTree<int, int> C(keyRay, valueRay, 13);

    //C.inorder();

    RBTree<int, int> C;
    C.insert(7,8);
    C.insert(9,10);
    C.insert(6,7);

    C.preorder();
    C.inorder();
    C.postorder();

    // cout << floor(log2(15)) << endl;
}