#include <cstdlib>
#include <iostream>
using namespace std;

template <typename keytype>
class DynamicArray{
    private:
    int size;
    int capacityNum;
    keytype placeholder;
    keytype blankReturn;
    keytype *ray;

    public:
    DynamicArray(){
        size = 0;
        capacityNum = 2;
        ray = new keytype[2];
    };

    DynamicArray(int s){
        size = s;
        capacityNum = s*2;
        ray = new keytype[s*2];
    };

    ~DynamicArray(){
        delete[] ray;
    };

    DynamicArray(const DynamicArray &src){
        size = src.size;
        capacityNum = src.capacityNum;
        ray = new keytype[src.capacityNum];
        for(int i = 0; i < size; i++){
            ray[i] = src.ray[i];
        }
    }

    DynamicArray operator=(const DynamicArray &src){
        delete[] ray;
        size = src.size;
        capacityNum = src.capacityNum;
        ray = new keytype[src.capacityNum];
        for(int i = 0; i < src.size; i++){
            ray[i] = src.ray[i];
        }
        return (*this);
    }

    keytype& operator[](int i){
        if(i < size && i >= 0){
            return ray[i];
        }else{
            return blankReturn;
        }
    };

    void addEnd(keytype v){
        if(size < capacityNum){
            ray[size] = v;
            size++;
        }else{
            resizeArray(capacityNum*2);
            addEnd(v);
        }
    };

    void delEnd(){
        if(size==0) return;
        size--;
        if(size*4 <= capacityNum){
            resizeArray(capacityNum/2);
        }
    };

    void resizeArray(int i){
        keytype *tempRay = new keytype[i];
        for(int i = 0; i < size; i++){
            tempRay[i] = ray[i];
        }
        delete[] ray;
        ray = tempRay;
        capacityNum = i;
    };

    int length(){
        return size;
    };
};

template <typename keytype>
class Heap{
    public:
    //will need to use CDA (circ. dynam. ray) for keys
    //min index will be the first item in array
    DynamicArray<keytype> ray;
    keytype blankReturn;

    //default constructor //Blank but done
    Heap(){
        ray = DynamicArray<keytype>();
    };

    //Done, just sets ray = src.ray
    Heap(Heap &src){
        ray = DynamicArray<keytype>(src.ray);
    }

    //Done, Heapify, Calls followUp with needFollowDown true
    Heap(keytype k[], int s){
        ray = DynamicArray<keytype>();

        //get all elements in array O(s)
        for(int i = 0; i < s; i++){
            ray.addEnd(k[i]);
        }

        followUp(s-1,true);
    };

    //Done, just sets ray = src.ray
    Heap operator=(Heap &src){
        ray = src.ray;
        return (*this);
    }

    //Done but blank
    //Ray gets deleted automatically when out of scope;
    ~Heap(){
        
    };

    //Done, called from insert and heap with array, bool fromHeapify is only true from heap(ray, size)(aka heapify) 
    void followUp(int currIndex, bool fromHeapify = false){
        keytype lowest = ray[currIndex];
        int lowestIndex = currIndex;
        //this parent and nextIndex is for a left child; if right child will change in if(currIndex%2==0)
        int parentIndex = (currIndex-1)/2;
        int nextIndex = currIndex-1;
        //if root just end
        if(currIndex==0) return;

        //if right child then have to check left and parent
        if(currIndex%2==0){
            parentIndex = (currIndex-2)/2;
            nextIndex -= 1;
            //ray[currIndex-1] is sibling to left
            if(ray[currIndex-1]<lowest){
                lowest = ray[currIndex-1];
                lowestIndex = currIndex-1;
            }
        }

        //at this point lowest is lower of both siblings values (or just left value if only left child)
        //need to check if parent is great cause then we will swap
        if(ray[parentIndex]>lowest){
            ray[lowestIndex] = ray[parentIndex];
            ray[parentIndex] = lowest;
            if(fromHeapify){
                followDown(lowestIndex);
            }
            else followUp(parentIndex, fromHeapify);
        }

        if(fromHeapify) followUp(nextIndex, fromHeapify);
        
    }

    //Done, used in followUp when needFollowDown is true (when ran from heap(ray,size))
    void followDown(int currIndex){
        //insert helper should start at root and follow wherever you move root
        keytype lowest = ray[currIndex];
        int lowestIndex = currIndex;
        //check if has left
        if(currIndex*2+1<ray.length()){
            lowestIndex = currIndex*2+1;
            lowest = ray[lowestIndex];
            if(currIndex*2+2<ray.length()){
                if(ray[currIndex*2+2]<lowest){
                    lowestIndex = currIndex*2+2;
                    lowest = ray[lowestIndex];
                }
            }
        }
        //lowestIndex is currIndex if no children
        if(lowestIndex==currIndex) return;
        //need to swap current and lowest index
        if(ray[currIndex]>lowest){
            ray[lowestIndex] = ray[currIndex];
            ray[currIndex] = lowest;
            followDown(lowestIndex);
        }
        
    }

    //Done
    keytype peekKey(){
        return ray[0];
    };

    //Done
    keytype extractMin(){
        if(ray.length() == 0) return blankReturn;
        keytype returnVal = ray[0];
        ray[0] = ray[ray.length()-1];
        ray.delEnd();
        followDown(0);
        return returnVal;
    };

    //Done
    void insert(keytype k){
        ray.addEnd(k);
        followUp(ray.length()-1);
    };

    //Done
    void printKey(){
        for(int i = 0; i < ray.length(); i++){
            cout << ray[i] << " ";
        }
        cout << endl;
    };

};