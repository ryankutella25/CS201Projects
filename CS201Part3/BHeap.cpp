#include <cstdlib>
#include <iostream>
using namespace std;

template <typename keytype>
class Node{
    public:
    int rank;
    Node *sibling = nullptr;
    Node *firstChild = nullptr;
    keytype key;

    Node(keytype k, int r = 0){
        key = k;
        rank = r;
    }
};

template <typename keytype>
class BHeap{
    public:
    //will need to use CDA (circ. dynam. ray) for main keys
    //min index will be the index of minimum in the CDA line above
    keytype returnKey;
    Node<keytype> * topLeft = nullptr;
    Node<keytype> * minNode = nullptr;

    //default constructor (does nothing)
    BHeap(){};

    //Done, insert each key in k using basic insert
    BHeap(keytype k[], int s){
        for(int i = 0; i<s; i++){
            insert(k[i]);
        }
    };

    //Done, just uses insertHelper
    BHeap(BHeap &src){
        //copies each node into new node and if it's minNode it sets this minNode to the new node
        topLeft = insertHelper(src.topLeft, src.minNode);
    }

    //Done, Called from copy and = 
    Node<keytype> * insertHelper(Node<keytype> *nodeToCopy, Node<keytype> *srcMinNode = nullptr){
        if (nodeToCopy==nullptr) return nullptr;
        Node<keytype> *nodeToAdd = new Node<keytype>(nodeToCopy->key,nodeToCopy->rank);
        //if the node we are copying was the sources minNode that means nodeToAdd is our minNode
        if(nodeToCopy == srcMinNode) minNode = nodeToAdd;
        nodeToAdd->sibling = insertHelper(nodeToCopy->sibling, srcMinNode);
        //child will never be minNode so don't need to pass that in
        nodeToAdd->firstChild = insertHelper(nodeToCopy->firstChild);
        return nodeToAdd;
    }

    //Done, deletes child then sibling then current
    void deleteHelper(Node<keytype> *current){
        if(current == nullptr) return;   
        deleteHelper(current->firstChild);
        deleteHelper(current->sibling);
        delete current;
    };

    //Done, clears array with deleteHelper then calls insertHelper
    BHeap operator=(BHeap &src){
        deleteHelper(topLeft);
        //next 2 lines should happen automatically but doesn't hurt to have
        topLeft = nullptr;
        minNode = nullptr;
        //BHeap is now fresh new BHeap

        //copies each node into new node and if it's minNode it sets this minNode to the new node
        topLeft = insertHelper(src.topLeft, src.minNode);
        //returns like all operator= functions
        return (*this);
    }

    //Done, just calls deleteHelper with topLeft
    ~BHeap(){
        deleteHelper(topLeft);
    };

    //Done, just returns minNode->key if minNode != nullptr
    keytype peekKey(){
        if(minNode==nullptr) return returnKey;
        return minNode->key;
    };

    //Done
    void insertBig(Node<keytype> *currNode){
        //need combineNode because sibling changes when combining so cant use nextCheck
        Node<keytype> *combineNode = nullptr;
        //need previous for when you add back into list (won't always be topLeft like regular insert)
        Node<keytype> *previousNode = nullptr;
        //nextCheck is just basic iteration
        Node<keytype> *nextCheck = topLeft;

        int currRank = currNode->rank;
        //while runs when spot is less than or equal or taken (or reach end)
        while(nextCheck != nullptr && nextCheck->rank <= currRank){
            combineNode = nextCheck;
            nextCheck = nextCheck->sibling;
            //if same rank need to combine and keep going
            if(combineNode->rank == currRank){
                currNode = combine(currNode, combineNode);
                if(combineNode == minNode) minNode = nullptr;
                currRank++;
                //don't need to update previousNode cause combineNode gets taken out of list
            }else{
                previousNode = combineNode;
            }
        }
        //at this point spot is open before nextCheck (nextCheck rank>currRank) or nextCheck is end of list (so nullptr)
        currNode->sibling = nextCheck;
        if(previousNode == nullptr) topLeft = currNode;
        else previousNode->sibling = currNode;

        //did <= because if they're equal it's easier to remove node with smaller rank
        if(minNode==nullptr||currNode->key<minNode->key) minNode = currNode;
    };

    //Done, if null returns returnKey, else takes minNode out of sibling list
    //then adds back minNode's children to heap, then sets returnKey and deletes
    //lastly, finds new minNode if any nodes are left
    keytype extractMin(){
        //if no minNode just return anything
        if (minNode == nullptr) return returnKey;

        //need to get minNode out of top sibling list
        if (minNode == topLeft){
            // cout <<"ran"<<endl;
            topLeft = minNode->sibling;
        }else{
            // cout <<"here"<<endl;
            Node<keytype> *nodeBeforeMin = topLeft;
            //cout <<topLeft<<" "<<minNode<<" "<<minNode->sibling<<endl;
            while(nodeBeforeMin->sibling != minNode){
                // cout <<"here 15"<< nodeBeforeMin<<" "<<nodeBeforeMin->sibling <<endl;
                nodeBeforeMin = nodeBeforeMin->sibling;
            }
            // cout << nodeBeforeMin << " " << minNode<<endl;
            //nodeBeforeMin is right before minNode so skip minNode (it's okay if minNode->sibling is nullptr)
            nodeBeforeMin->sibling = minNode->sibling;
        }

        //now minNode is out of sibling list so insert each child before deletion
        Node<keytype> *currChild = minNode->firstChild;
        //need nextChild because after insertion currChild->sibling will change
        Node<keytype> *nextChild;
        // cout <<"here 2"<<endl;
        while(currChild != nullptr){
            // cout <<"here 3"<<endl;

            nextChild = currChild->sibling;
            //insertBig inserts entire heaps so it's okay if it has children;
            insertBig(currChild);
            currChild = nextChild;
        }
        //now children are back in regular array so minNode is all alone (can delete)
        returnKey = minNode->key;
        // cout<<"Breaks"<<returnKey<<endl;
        delete minNode;
        // cout <<" here"<<endl;

        //Now need to find new minNode if there are any nodes left (if none then will be nullptr)
        minNode = topLeft;
        // cout <<"topLeft "<<topLeft<<endl;
        Node<keytype> *currSib = topLeft;
        while(currSib != nullptr){
            if(currSib->key < minNode->key){
                minNode = currSib;
            }
            currSib = currSib->sibling;
        }
        
        return returnKey;
    };

    //Done, ONLY PASS IN SAME RANK HEAPS, siblings don't matter and will be lost
    Node<keytype> * combine(Node<keytype> *one, Node<keytype> *two){
        one->sibling = nullptr;
        two->sibling = nullptr;

        bool returnOne = true;
        if(one->key<two->key){
            two->sibling = one->firstChild;
            one->firstChild = two;
            one->rank++;
        }else{
            returnOne = false;
            one->sibling = two->firstChild;
            two->firstChild = one;
            two->rank++;
        }
        //can simplify below but keep for testing
        if(returnOne){
            // cout << "GOT" <<endl;
            // printRecursNoSib(one, true);
            return one;
        };
        // cout << "GOT" <<endl;
        // printRecursNoSib(two, true);
        return two;
    }

    //Done, makes new node with key k, combines down the sibling list until it finds spot or gets to end
    //then adds into sibling list at topLeft and checks if less than minNode and if it is then sets minNode to it
    void insert(keytype k){
        Node<keytype> *nodeToAdd = new Node<keytype>(k);
        //need combineNode because if you combine nodeToCheck it changes sibling
        Node<keytype> *combineNode;
        Node<keytype> *nodeToCheck = topLeft;

        int currRank = 0;
        //while runs when spot is taken (or reach end)
        while(nodeToCheck != nullptr && nodeToCheck->rank == currRank){
            combineNode = nodeToCheck;
            //need to get sibling before combine because it'll change sibling
            nodeToCheck = nodeToCheck->sibling;
            //combine automatically updates nodeToAdd's rank
            nodeToAdd = combine(nodeToAdd, combineNode);
            if(combineNode == minNode) minNode = nullptr;

            currRank++;
        }
        //at this point spot is open before nodeToCheck (nodeToCheck->rank>currRank) or nodeToCheck is end of list (so nullptr)
        nodeToAdd->sibling = nodeToCheck;
        //added node will always be topLeft because will collect any nodes of same rank and no node's rank can be <0
        topLeft = nodeToAdd;

        //just changes minNode to nodeToAdd if it's key is less than or equal
        //did or equal because if they're the same nodeToAdd's rank will be smaller and easier to extract smaller heaps
        if(minNode == nullptr||nodeToAdd->key < minNode->key) minNode = nodeToAdd;
    };

    //Done, consumes H2. Takes each sibling in main siblings list and adds them whole. Uses insertBig
    void merge(BHeap<keytype> &H2){
        Node<keytype> *currNode = H2.topLeft;
        //need nextNode because currNode sibling will change after insertBig
        Node<keytype> *nextNode;
        while(currNode != nullptr){
            nextNode = currNode->sibling;
            insertBig(currNode);
            currNode = nextNode;
        }
        //below just makes it so they don't point into this heap
        H2.topLeft = nullptr;
        H2.minNode = nullptr;
    };

    //Just for testing, basically prints a node and its children
    //Calls regular printRecurs (just no siblings for original node)
    void printRecursNoSib(Node<keytype> *currNode, bool top){
        if(currNode==nullptr) return;
        if(top){
            cout << "B" << currNode->rank << endl;
        } 
        cout << currNode->key;
        printRecurs(currNode->firstChild, false);
        if(top) cout << endl << endl;
    }

    //Done
    void printRecurs(Node<keytype> *currNode, bool top){
        if(currNode==nullptr) return;
        if(top){
            cout << "B" << currNode->rank << endl;
        } 
        cout << currNode->key << " ";
        //no child will be in top ever
        printRecurs(currNode->firstChild, false);
        //new line after last child and new line so line inbetween if top=true
        if(top) cout << endl << endl;
        printRecurs(currNode->sibling, top);
    }

    //Done, just calls printRecurs with topLeft
    void printKey(){
        printRecurs(topLeft,true);
    };

    void testMinNode(){
        cout <<"here"<<endl;
        if(topLeft==minNode){
            cout <<"good"<<endl;
            return;
        }
            Node<keytype> *nodeBeforeMin = topLeft;
            cout <<topLeft<<" "<<minNode<<" "<<minNode->sibling<<endl;
            while(nodeBeforeMin->sibling!=minNode){
                cout <<"here 15"<< nodeBeforeMin<<" "<<nodeBeforeMin->sibling <<endl;
                nodeBeforeMin = nodeBeforeMin->sibling;
            }
            cout << nodeBeforeMin << " " << minNode<<endl;
    }
};