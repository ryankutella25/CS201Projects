#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;

template <typename keytype, typename valuetype>
class Node{
    
    public:
        keytype key;
        valuetype value;
        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;
        bool isLeft = false;
        bool isBlack = false;
        int sizeOfLeftTree = 0;

        Node(){
        };

        Node(keytype k, valuetype v){
            key = k;
            value = v;
        }
};

template <typename keytype, typename valuetype>
class RBTree{
    //public
    public:
        int sizeNum;
        Node<keytype,valuetype> *rootNode;
        Node<keytype,valuetype> *nodeToAdd;
    
    //Done, basic
    RBTree(){
        sizeNum = 0;
        rootNode = nullptr;
    };

    //DONE!
    Node<keytype,valuetype> *makeTree(Node<keytype,valuetype> *pNode, keytype k[], valuetype v[], int left,  int mid, int right, int blackLevelsLeft){
        Node<keytype,valuetype> *cNode = new Node<keytype,valuetype>(k[mid],v[mid]);
        //# of elements left
        if (mid - left > 0){
            //finds midLeft not including current mid
            int midLeft = ((mid - 1 - left) / 2) + left;
            cNode->sizeOfLeftTree = mid - left;
            Node<keytype,valuetype> *temp = makeTree(cNode, k, v, left, midLeft, mid - 1, blackLevelsLeft-1);
            temp->isLeft = true;
            cNode->left = temp;
        }
        if (right - mid > 0){
            int midRight = ((right - mid + 1) / 2) + mid;
            Node<keytype,valuetype> *temp = makeTree(cNode, k, v, mid + 1, midRight, right, blackLevelsLeft-1);
            temp->isLeft = false;
            cNode->right = temp;
        }
        cNode->parent = pNode;
        if(blackLevelsLeft > 0){
            cNode->isBlack = true;
        }
        //cout << cNode->key << " " <<cNode->isBlack << endl;

        return cNode;
    }

    //DONE!
    RBTree(keytype k[], valuetype v[], int s){
        sizeNum = 0;
        rootNode = nullptr;
        bool inOrder = true;
        for (int i = 1; i < s; i++){
            if(k[i] < k[i-1]){
                inOrder = false;
                break;
            }
        }
        if(!inOrder){
            //cout << "ray is not in order" << endl;
            for (int i = 0; i < s; i++){
                //cout << i << endl;
                //cout << s << endl;
                insert(k[i], v[i]);
            }
        }
        else{
            //cout << "ray is in order" << endl;
            int legs = floor(log2(s+1));
            int middleNum = (s-1)/2;
            rootNode = makeTree(nullptr, k, v, 0, middleNum, s-1, legs);
        }
    };

    //Done, works with deletion (CHECK)
    void deleteSubtree(Node<keytype,valuetype> *root){
        if(root->left != nullptr){
            deleteSubtree(root->left);
        }
        if(root->right != nullptr){
            deleteSubtree(root->right);
        }
        delete root;
    }

    //Deletion is done
    ~RBTree(){
        if(sizeNum != 0){
            deleteSubtree(rootNode);
        }
        rootNode = nullptr;
        sizeNum = 0;
    };

    RBTree(RBTree &src){
        rootNode = copySubtree(src.rootNode, nullptr);
        sizeNum = src.sizeNum;
    }

    //Done, works with copy
    Node<keytype,valuetype> *copySubtree(Node<keytype,valuetype> *srcRoot, Node<keytype,valuetype> *parent){
        if(srcRoot==nullptr) return nullptr;
        Node<keytype,valuetype> *root = new Node<keytype,valuetype>();
        root->key = srcRoot->key;
        root->value = srcRoot->value;
        root->isLeft = srcRoot->isLeft;
        root->isBlack = srcRoot->isBlack;
        root->sizeOfLeftTree = srcRoot->sizeOfLeftTree;

        root->parent = parent;

        if(srcRoot->left != nullptr){
            root->left = copySubtree(srcRoot->left, root);
        }
        if(srcRoot->right != nullptr){
            root->right = copySubtree(srcRoot->right, root);
        }
        return root;
    }

    RBTree operator=(RBTree &src){
        // cout <<"started"<<endl;
        if(sizeNum != 0){
            deleteSubtree(rootNode);
        }
        rootNode = nullptr;
        rootNode = copySubtree(src.rootNode, nullptr);
        sizeNum = src.sizeNum;
                // cout <<"ended"<<endl;

        return (*this);
        // delete[] ray;
        // size = src.size;
        // capacityNum = src.capacityNum;
        // front = src.front;
        // back = src.back;
        // reverseBool = src.reverseBool;
        // ray = new T[src.capacityNum];
        // for(int count = 0; count < size; count++){
        //     int rayPos = rayPosFromUserPos(count);
        //     ray[rayPos] = src.ray[rayPos];
        // }
        // return (*this);
    }

    //Search is done, might need to change return NULL
    valuetype *search(keytype k){
        Node<keytype,valuetype> *currentNode = rootNode;
        while(currentNode != nullptr){
            if(k > currentNode->key){
                currentNode = currentNode->right;
            }
            else if(k == currentNode->key){
                return &currentNode->value;
            }
            else{
                currentNode = currentNode->left;
            }
        }
        return NULL;
    };

    //Rotate is done, used in fixup
    void rotate(Node<keytype,valuetype> *currentNode, bool right){
        Node<keytype,valuetype> *parentNode = currentNode->parent;
        Node<keytype,valuetype> *grandpaNode = parentNode->parent;
        if(right){
            // cout << "parent before; left:" << parentNode->left << " right:" << parentNode->right << " parent:" << parentNode->parent<<endl;
            // cout << "current before; left:" << currentNode->left << " right:" << currentNode->right << " parent:" << currentNode->parent<<endl;
            //rotate right
            parentNode->left = currentNode->right;
            if(currentNode->right!=nullptr){
                currentNode->right->parent = parentNode;
                currentNode->right->isLeft = true;
            }
            currentNode->right = parentNode;
            currentNode->isLeft = parentNode->isLeft;
            parentNode->isLeft = false;
            currentNode->parent = grandpaNode;
            parentNode->parent = currentNode;
            if(grandpaNode!=nullptr){
                if(currentNode->isLeft){
                    grandpaNode->left = currentNode;
                }else{
                    grandpaNode->right = currentNode;
                }
            }else{
                rootNode = currentNode;
                currentNode->isBlack = true;
            }
            //sizing stuff
            parentNode->sizeOfLeftTree -= (currentNode->sizeOfLeftTree + 1);
            // cout << "parent after; left:" << parentNode->left << " right:" << parentNode->right << " parent:" << parentNode->parent<<endl;
            // cout << "current after; left:" << currentNode->left << " right:" << currentNode->right << " parent:" << currentNode->parent<<endl;
        }
        else{
            // cout << "parent before; left:" << parentNode->left << " right:" << parentNode->right << " parent:" << parentNode->parent<<endl;
            // cout << "current before; left:" << currentNode->left << " right:" << currentNode->right << " parent:" << currentNode->parent<<endl;
            //rotate left
            parentNode->right = currentNode->left;
            if(currentNode->left!=nullptr){
                currentNode->left->parent = parentNode;
                currentNode->left->isLeft = false;
            }
            currentNode->left = parentNode;
            currentNode->isLeft = parentNode->isLeft;
            parentNode->isLeft = true;
            currentNode->parent = grandpaNode;
            parentNode->parent = currentNode;
            if(grandpaNode!=nullptr){
                if(currentNode->isLeft){
                    grandpaNode->left = currentNode;
                }else{
                    grandpaNode->right = currentNode;
                }
            }else{
                rootNode = currentNode;
                currentNode->isBlack = true;
            }
            //sizing stuff
            currentNode->sizeOfLeftTree += parentNode->sizeOfLeftTree + 1;
            // cout << "parent after; left:" << parentNode->left << " right:" << parentNode->right << " parent:" << parentNode->parent<<endl;
            // cout << "current after; left:" << currentNode->left << " right:" << currentNode->right << " parent:" << currentNode->parent<<endl;
        }
    }

    //Fixup is done, used in insert
    void fixup(Node<keytype,valuetype> *nodeToAdd){
        Node<keytype,valuetype> *parentNode = nodeToAdd->parent;
        //if parent is black do nothing
        if(!parentNode->isBlack){
            //Get uncle
            Node<keytype,valuetype> *uncleNode;
            //if no granpda must be case 2 or case 3 so don't find uncle
            if(parentNode->parent!=nullptr){
                if(parentNode->isLeft){
                    uncleNode = parentNode->parent->right;
                }
                else{
                    uncleNode = parentNode->parent->left;
                }
            }else{
                uncleNode = nullptr;
            }

            //Case 2 and 3: uncle is black
            if(uncleNode==nullptr || uncleNode->isBlack){
                //Case 3 both left
                if(nodeToAdd->isLeft && parentNode->isLeft){
                    //cout << "Case 3l " << nodeToAdd << endl;
                    parentNode->isBlack = true;
                    parentNode->parent->isBlack = false;
                    rotate(parentNode,true);
                    //cout << "Finsihed" << endl;
                }
                //Case 3 both right
                else if(!(nodeToAdd->isLeft) && !(parentNode->isLeft)){
                    //cout << "Case 3r " << nodeToAdd <<"   " <<sizeNum << endl;
                    //cout << "Case 3 Right" << endl;
                    parentNode->isBlack = true;
                    parentNode->parent->isBlack = false;
                    rotate(parentNode,false);
                    //cout << nodeToAdd->isLeft << endl;
                }
                //Case 2
                else{
                    //cout << "Case 2 " << nodeToAdd << endl;
                    if(nodeToAdd->isLeft){
                        rotate(nodeToAdd, true);
                        fixup(nodeToAdd->right);
                    }else{
                        rotate(nodeToAdd, false);
                        fixup(nodeToAdd->left);
                    }
                    //cout << "finished" << endl;
                }
            }
            //Case 1 uncle is red (has to go second in case uncle is nullptr)
            else if(!uncleNode->isBlack){
                //cout << "Case 1 " << nodeToAdd << endl;
                parentNode->isBlack = true;
                uncleNode->isBlack = true;
                if(parentNode->parent->parent!=nullptr){
                    parentNode->parent->isBlack = false;
                    fixup(parentNode->parent);
                }
            }
        }   
    }

    //Done
    void insert(keytype k, valuetype v){
        //cout << " A " << endl;
        Node<keytype,valuetype> *nodeToAdd = new Node<keytype,valuetype>(k,v);
        if(rootNode == nullptr){
            rootNode = nodeToAdd;
            rootNode->isBlack = true;
        }
        else{
            Node<keytype,valuetype> *parentNode = rootNode;
            Node<keytype,valuetype> *nextNode = rootNode;
            while(nextNode!=nullptr){
                parentNode = nextNode;
                if(nodeToAdd->key > parentNode->key){
                    nextNode = parentNode->right;
                }
                else{
                    parentNode->sizeOfLeftTree += 1;
                    nextNode = parentNode->left;
                }
            }
            //Now currentNode is parent of where nodeToAdd should be added
            nodeToAdd->parent = parentNode;
            //cout << "nodeToAdd Key" << nodeToAdd->key << " parent key " << parentNode->key<< endl;
            if(nodeToAdd->key > parentNode->key){
                parentNode->right = nodeToAdd;
                nodeToAdd->isLeft = false;
            }
            else{
                parentNode->left = nodeToAdd;
                nodeToAdd->isLeft = true;
            }
            nodeToAdd->isBlack = false;
            //Done with regular insert.

            fixup(nodeToAdd);
            //cout <<" NODE ADDED: " << nodeToAdd->key << endl;
            //preorder();
        }
        sizeNum++;
    };

    //at this point node is parent of deleted node
    void fixupRemove(Node<keytype,valuetype> *currentNode){
        // cout << "started fixup" << endl;
        if(!currentNode->isBlack){
            // cout <<"BROOO" <<endl;
            currentNode->isBlack = true;
        }else{
            Node<keytype,valuetype> *pNode = currentNode->parent;
            //always check if no parent
            if(pNode!=nullptr){
                Node<keytype,valuetype> *sNode;
                if(currentNode->isLeft){
                    sNode = pNode->right;
                    // if(sNode==nullptr) cout <<"BROOO" <<endl;
                    //sNode is set to sibling (can't be nullptr)
                    //sNode isLeft = false
                    //must be case 1
                    if(!sNode->isBlack){
                        // cout << "started 1 r" << endl;
                        rotate(sNode,false);
                        sNode->isBlack=true;
                        pNode->isBlack=false;
                        //cout << "end 1" << currentNode << endl;

                        fixupRemove(currentNode);
                    }
                    //must be cases 2-4
                    else{
                        //case 4
                        if(sNode->right!=nullptr&&!sNode->right->isBlack){
                            // cout << "start 4" << endl;
                            rotate(sNode,false);
                            sNode->isBlack=pNode->isBlack;
                            sNode->right->isBlack=true;
                            pNode->isBlack=true;
                            //cout << "end 4" << endl;
                        }
                        //case 3
                        else if(sNode->left!=nullptr&&!sNode->left->isBlack){
                            // cout << "start 3" << endl;
                            Node<keytype,valuetype> *nieceNode = sNode->left;
                            rotate(nieceNode,true);
                            nieceNode->isBlack=true;
                            sNode->isBlack=false;
                            //cout << "end 3" << endl;
                            fixupRemove(currentNode);
                            
                        }
                        //case 2
                        else{
                            //cout << "2" << endl;
                            sNode->isBlack=false;
                            fixupRemove(pNode);
                        }
                    }
                }
                else{
                    sNode = pNode->left;
                    // if(sNode==nullptr) cout <<"BROOO" <<endl;
                    //sNode is set to sibling (can't be nullptr)
                    //sNode isLeft = true
                    //must be case 1
                    if(!sNode->isBlack){
                        // cout << "start 1 l" << endl;
                        rotate(sNode,true);
                        sNode->isBlack=true;
                        pNode->isBlack=false;
                        //cout << "end 1" << currentNode << endl;
                        fixupRemove(currentNode);
                        
                    }
                    //must be cases 2-4
                    else{
                        //case 4
                        if(sNode->left!=nullptr&&!sNode->left->isBlack){
                            // cout << "start 4" << endl;
                            rotate(sNode,true);
                            sNode->isBlack=pNode->isBlack;
                            sNode->left->isBlack=true;
                            pNode->isBlack=true;
                            //cout << "end 4" << endl;
                        }
                        //case 3
                        else if(sNode->right!=nullptr&&!sNode->right->isBlack){
                            // cout << "start 3" << endl;
                            Node<keytype,valuetype> *nieceNode = sNode->right;
                            rotate(nieceNode,false);
                            nieceNode->isBlack=true;
                            sNode->isBlack=false;
                            //cout << "end 3" << endl;
                            fixupRemove(currentNode);
                        }
                        //case 2
                        else{
                            // cout << "start 2" << endl;
                            sNode->isBlack=false;
                            fixupRemove(pNode);
                        }
                    }
                }
            }else{
                //cout << "thus shouldnt break" << endl;
                currentNode->isBlack=true;
                //cout << "or does it" << endl;
            }
        }
    }

    //DONE
    void fixSizeRemove(Node<keytype,valuetype> *currentNode){
        Node<keytype,valuetype> *pNode = currentNode->parent;

        while(pNode!=nullptr){
            if(currentNode->isLeft) pNode->sizeOfLeftTree -= 1;
            currentNode = pNode;
            pNode = currentNode->parent;
        }
        
    }

    //DONE
    int remove(keytype k){
        Node<keytype,valuetype> *currentNode = rootNode;
        while(currentNode != nullptr){
            if(k > currentNode->key){
                currentNode = currentNode->right;
            }
            else if(k == currentNode->key){
                break;
            }
            else{
                currentNode = currentNode->left;
            }
        }
        
        if(currentNode==nullptr){
            return 0;
        }

        //now change to predecessor if necessary
        if(currentNode->left!=nullptr && currentNode->right!=nullptr){
            Node<keytype,valuetype> *predecessor = currentNode->left;
            while(predecessor->right != nullptr){
                predecessor = predecessor->right;
            }
            currentNode->key = predecessor->key;
            currentNode->value = predecessor->value;
            currentNode = predecessor;
        }

        //currentNode is now the node I want to delete
        Node<keytype,valuetype> *pNode = currentNode->parent;
        //if no parent
        if(pNode==nullptr){
            if(currentNode->left!=nullptr){
                rootNode = currentNode->left;
                rootNode->isBlack=true;
                rootNode->parent = nullptr;
            }
            else if(currentNode->right!=nullptr){
                rootNode = currentNode->right;
                rootNode->isBlack=true;
                rootNode->parent = nullptr;
            }
            else{
                rootNode = nullptr;
            }
            sizeNum -= 1;
            delete currentNode;
            return 1;
        }
        //if node is red easy delete no fixup
        else if(!currentNode->isBlack){
            //cout<<"MAY"<<endl;
            if(currentNode->left!=nullptr){
                if(currentNode->isLeft){
                    pNode->left = currentNode->left;
                    currentNode->left->parent = pNode;
                }else{
                    pNode->right = currentNode->left;
                    currentNode->left->parent = pNode;
                    currentNode->left->isLeft = false;
                }
            }
            else if (currentNode->right!=nullptr){
                if(currentNode->isLeft){
                    pNode->left = currentNode->right;
                    currentNode->right->parent = pNode;
                    currentNode->right->isLeft = true;
                }else{
                    pNode->right = currentNode->right;
                    currentNode->right->parent = pNode;
                }
            }else{
               if(currentNode->isLeft){
                    pNode->left = nullptr;
                }else{
                    pNode->right = nullptr;
                }
            }
            // cout<<"got to fixup size" << endl;
            fixSizeRemove(currentNode);
            // cout<<"got out of fixup size" << endl;
            sizeNum -= 1;
            delete currentNode;
            return 1;
        }
        //now to see if it has a child
        else if(currentNode->right!=nullptr){

            //move child up
            if(currentNode->isLeft){
                pNode->left = currentNode->right;
                currentNode->right->parent = pNode;
                currentNode->right->isLeft = true;
                
            }else{
                pNode->right = currentNode->right;
                currentNode->right->parent = pNode;
            }
            //if red super easy
            if(!currentNode->right->isBlack){
                //cout <<"RAN" <<endl;
                currentNode->right->isBlack = true;
                sizeNum -= 1;
                            // cout<<"got to fixup size" << endl;

                fixSizeRemove(currentNode);
                            // cout<<"got out of fixup size" << endl;

                delete currentNode;
                return 1;
            }
            //not red so need to use in fixup
            else{
                pNode = currentNode->right;
            }
            
        }
        else if(currentNode->left!=nullptr){
                            //   cout << "HUH4" << endl;
            //move child up
            if(currentNode->isLeft){
                pNode->left = currentNode->left;
                currentNode->left->parent = pNode;
            }else{
                // cout << "HUH44" << endl;

                pNode->right = currentNode->left;
                currentNode->left->parent = pNode;
                currentNode->left->isLeft = false;
            }
            //if red super easy
            if(!currentNode->left->isBlack){
                currentNode->left->isBlack = true;
                sizeNum -= 1;
                            // cout<<"got to fixup size" << endl;

                fixSizeRemove(currentNode);
                            // cout<<"got out of fixup size" << endl;

                delete currentNode;
                return 1;
            }
            //not red so need to use in fixup
            else{
                //cout <<"Got here" <<endl;
                pNode = currentNode->left;
            }
        }
        //no children so make parent point to nullptr
        else{
            //cout <<"HERE "<< pNode->key << currentNode->key <<endl;
            //inorder();
            if(currentNode->isLeft){
                //cout <<"SHOULD MATHC: Parent's Left: "<< pNode->left->key<< "; Current:" << currentNode->key <<endl;
                pNode->left = nullptr;
            }else{
                //cout <<"SHOULD MATHC: Parent's Right: "<< pNode->right->key<< "; Current:" << currentNode->key <<endl;
                pNode->right = nullptr;
            }
            pNode=currentNode;
            //cout <<"HERE SECOND"<<endl;
            //inorder();
        }
        //if this point reached I am missing a black on one side
        //pNode is either the child that got moved up or the original parent if no children (not nullptr)
        sizeNum -= 1;
        // cout << "got to fixupSize" << endl;
        fixSizeRemove(currentNode);
        // cout << "got to fixup" << endl;
        delete currentNode;
        fixupRemove(pNode);
        return 1;
    };

    //done
    int rank(keytype k){
        int num = 1;
        Node<keytype,valuetype> *cNode = rootNode;
        Node<keytype,valuetype> *nextNode = rootNode;
        while(nextNode!=nullptr){
            cNode = nextNode;
            if(k > cNode->key){
                //cout << cNode->sizeOfLeftTree;
                num += cNode->sizeOfLeftTree + 1;
                nextNode = cNode->right;
            }
            else if(k == cNode->key){
                //cout << cNode->sizeOfLeftTree;
                num += cNode->sizeOfLeftTree;
                return num;
            }
            else{
                nextNode = cNode->left;
            }
        }
        return 0;
    };    

    //DONE
    keytype select(int pos){
        Node<keytype,valuetype> *cNode = rootNode;
        while(cNode!=nullptr){
            //cout<<pos<<endl;
            //cout << cNode->sizeOfLeftTree+1 << endl;
            if(pos < cNode->sizeOfLeftTree+1){
                cNode = cNode->left;
            }
            else if (pos == cNode->sizeOfLeftTree+1){
                return cNode->key;
            }
            else{
                pos = pos - (cNode->sizeOfLeftTree+1);
                cNode = cNode->right;
            }
        }
        return 0;
    };

    //DONE
    keytype *successor(keytype k){
        Node<keytype,valuetype> *cNode = rootNode;
        Node<keytype,valuetype> *nextNode = rootNode;
        while(nextNode!=nullptr){
            cNode = nextNode;
            if(k > cNode->key){
                nextNode = cNode->right;
            }
            else if(k == cNode->key){
                break;
            }
            else{
                nextNode = cNode->left;
            }
        }
        if(cNode==nullptr){
            cout << "Key does not exist" << endl;
            return NULL;
        }
        //if right exists, go right one then left as far as possible
        if(cNode->right!=nullptr){
            cNode = cNode->right;
            while (cNode->left!=nullptr){
                cNode = cNode->left;
            }
            return &cNode->key;
        }
        while(cNode->parent!=nullptr && !cNode->isLeft){
            cNode = cNode->parent;
        }
        if(cNode->parent!=nullptr){
            return &cNode->parent->key;
        }else{
            return NULL;
        }
    };

    //DONE
    keytype *predecessor(keytype k){
        Node<keytype,valuetype> *cNode = rootNode;
        Node<keytype,valuetype> *nextNode = rootNode;
        while(nextNode!=nullptr){
            cNode = nextNode;
            if(k > cNode->key){
                nextNode = cNode->right;
            }
            else if(k == cNode->key){
                break;
            }
            else{
                nextNode = cNode->left;
            }
        }
        if(cNode==nullptr){
            cout << "Key does not exist" << endl;
            return NULL;
        }
        //if left exists, go left one then right as far as possible
        if(cNode->left!=nullptr){
            cNode = cNode->left;
            while (cNode->right!=nullptr){
                cNode = cNode->right;
            }
            return &cNode->key;
        }
        while(cNode->parent!=nullptr && cNode->isLeft){
            cNode = cNode->parent;
        }
        if(cNode->parent!=nullptr){
            return &cNode->parent->key;
        }else{
            return NULL;
        }
    };
    
    //Done with below
    int size(){
        return sizeNum;
    };

    bool firstPrint = false;
    void orderRecursive(Node<keytype,valuetype> *root, int x, bool isFirst){
        if(root != nullptr){
            if(x == 0){
                if(firstPrint){
                    // if(root->parent!=nullptr){
                    //     if(root->isLeft){
                    //         cout <<"added" << root->key <<" should match "<<root->parent->left->key << endl;
                    //     }
                    //     else{
                    //         cout <<"added" << root <<" should match "<<root->parent->right->key << endl;
                    //     }
                    // }else{
                    //     cout <<"START" <<endl;
                    //     cout << root << " is root" << endl;
                    // }
                    cout << root->key;
                    //cout << root->sizeOfLeftTree;
                    firstPrint = false;
                }else{
                    // if(root->parent!=nullptr){
                    //     if(root->isLeft){
                    //         cout <<"added" << root->key <<" should match "<<root->parent->left->key << endl;
                    //     }
                    //     else{
                    //         cout <<"added" << root <<" should match "<<root->parent->right->key << endl;
                    //     }
                    // }else{
                    //     cout <<"START" <<endl;
                    //     cout << root << " is root" << endl;
                    // }
                    cout << " " << root->key;
                    //cout << root->sizeOfLeftTree;
                }
                orderRecursive(root->left, 0, false);
                orderRecursive(root->right, 0, false);
            }
            else if(x == 1){
                orderRecursive(root->left, 1, false);
                if(firstPrint){
                    cout << root->key;
                    firstPrint = false;
                }else{
                    cout << " " << root->key;
                }
                orderRecursive(root->right, 1, false);
            }
            else if(x == 2){
                orderRecursive(root->left, 2, false);
                orderRecursive(root->right, 2, false);
                if(firstPrint){
                    firstPrint = false;
                    cout << root->key;
                }else{
                    cout << " " << root->key;
                }
            } 
        }
    };

    void preorder(){
        firstPrint = true;
        if(sizeNum == 0){
            cout << endl;
        }
        orderRecursive(rootNode, 0, true);
        cout << endl;
    };

    void inorder(){
        firstPrint = true;
        if(sizeNum == 0){
            cout << endl;
        }
        orderRecursive(rootNode, 1, true);
        cout << endl;
    };

    void postorder(){
        firstPrint = true;
        if(sizeNum == 0){
            cout << endl;
        }
        orderRecursive(rootNode, 2, true);
        cout << endl;
    };

    // void orderRecursiveColor(Node<keytype,valuetype> *root, int x, bool isFirst){
    //     if(root != nullptr){
    //         if(x == 0){
    //             cout << " " << root->isBlack;
    //             orderRecursiveColor(root->left, 0, false);
    //             orderRecursiveColor(root->right, 0, false);
    //         }
    //         else if(x == 1){
    //             orderRecursiveColor(root->left, 1, false);
    //             cout << " " << root->isBlack;
    //             orderRecursiveColor(root->right, 1, false);
    //         }
    //         else if(x == 2){
    //             orderRecursiveColor(root->left, 2, false);
    //             orderRecursiveColor(root->right, 2, false);
    //             cout << " " << root->isBlack;
    //         } 
    //     }
    // };

    // void inorderColor(){
    //     if(sizeNum == 0){
    //         cout << endl;
    //     }
    //     orderRecursiveColor(rootNode, 1, true);
    //     cout << endl;
    // };

};