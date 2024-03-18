#include <cstdlib>
using namespace std;
template <typename T>

class CircularDynamicArray{
    private:
    int size;
    int capacityNum;
    int front;
    int back;
    bool reverseBool;
    T placeholder;
    T blankReturn;
    T *ray;

    public:
    CircularDynamicArray(){
        size = 0;
        capacityNum = 2;
        ray = new T[2];
        front = 0;
        back = 0;
        reverseBool = false;
    }

    CircularDynamicArray(int s){
        size = s;
        capacityNum = s;
        ray = new T[s];
        front = 0;
        back = size-1;
        reverseBool = false;
    }

    ~CircularDynamicArray(){
        delete[] ray;
    };

    CircularDynamicArray(const CircularDynamicArray &src){
        size = src.size;
        capacityNum = src.capacityNum;
        front = src.front;
        back = src.back;
        reverseBool = src.reverseBool;
        ray = new T[src.capacityNum];
        for(int count = 0; count < size; count++){
            int rayPos = rayPosFromUserPos(count);
            ray[rayPos] = src.ray[rayPos];
        }
    }

    CircularDynamicArray operator=(const CircularDynamicArray &src){
        delete[] ray;
        size = src.size;
        capacityNum = src.capacityNum;
        front = src.front;
        back = src.back;
        reverseBool = src.reverseBool;
        ray = new T[src.capacityNum];
        for(int count = 0; count < size; count++){
            int rayPos = rayPosFromUserPos(count);
            ray[rayPos] = src.ray[rayPos];
        }
        return (*this);
    }

    T& operator[](int i){
        if(i < size && i >= 0){
            return ray[rayPosFromUserPos(i)];
        }else{
            cout << "Index: " << i << " is out of bound." << endl;
            return blankReturn;
        }
    };

    void addEnd(T v){
        if(size < capacityNum){
            int actualRayPos = rayPosFromUserPos(size);
            ray[actualRayPos] = v;
            size++;
            back = actualRayPos;
        }else{
            resizeArray(capacityNum*2);
            addEnd(v);
        }
    };

    void addFront(T v){
        if(size < capacityNum){
            int actualRayPos = rayPosFromUserPos(-1);
            ray[actualRayPos] = v;
            size++;
            front = actualRayPos;
        }else{
            resizeArray(capacityNum*2);
            addFront(v);
        }
    };

    void delEnd(){
        if(size>1){
            back = rayPosFromUserPos(size-2);
        }else if(size == 1){
            //Keep back at 0
        }else{
            //CANT DELETE
        }
        size--;
        if(size*4 <= capacityNum){
            resizeArray(capacityNum/2);
        }
    };

    void delFront(){
        if(size>1){
            front = rayPosFromUserPos(1);
        }else if(size == 1){
            //Keep front at 0
        }else{
            //CANT DELETE
        }
        size--;
        if(size*4 <= capacityNum){
            resizeArray(capacityNum/2);
        }
    };

    int length(){
        return size;
    };

    int capacity(){
        return capacityNum;
    };

    void clear(){
        delete[] ray;
        ray = new T[2];
        size = 0;
        capacityNum = 2;
        front = 0;
        back = 0;
        reverseBool = false;
    };

    int partition(T* arr, int l, int r){
        int x = arr[r];
        int i = l;
        for (int j = l; j <= r - 1; j++) {
            if (arr[j] <= x) {
                placeholder = arr[i];
                arr[i] = arr[j];
                arr[j] = placeholder;
                i++;
            }
        }
        placeholder = arr[i];
        arr[i] = arr[r];
        arr[r] = placeholder;
        return i;
    }

    int randPartition(T* arr, int l, int r){
        int randIndex = l + rand() % (r - l);
        placeholder = arr[randIndex];
        arr[randIndex] = arr[r];
        arr[r] = placeholder;
        return partition(arr, l, r);
    }

    int kthSmallest(T* arr, int l, int r, int k){
        if (k > 0 && k <= r - l + 1) {
            int index = randPartition(arr, l, r);
            if (index - l == k - 1){
                return arr[index];
            } 
            if (index - l > k - 1) {
                return kthSmallest(arr, l, index - 1, k);
            }
            return kthSmallest(arr, index + 1, r, k - index + l - 1);
        }
        return blankReturn;
    }

    T QuickSelect(int k){
        if(k>size || k < 0){
            //just so it doesn't break
            return blankReturn;
        }
        T* temp = new T[capacityNum];
        for(int count = 0; count < size; count++){
            int rayPos = rayPosFromUserPos(count);
            temp[count] = ray[rayPos];
        }
        return kthSmallest(temp, 0, size-1, k);
    };

   void Merge(T* numbers, int i, int mid, int k){
        int mergeSize = k - i + 1;
        int mergePos = 0;
        int leftPos = i;
        int rightPos = mid + 1;
        T *mergedRay = new T[mergeSize];

        while (leftPos <= mid && rightPos <= k){
            if (numbers[leftPos] < numbers[rightPos]){
                mergedRay[mergePos] = numbers[leftPos];
                leftPos++;
            }
            else{
                mergedRay[mergePos] = numbers[rightPos];
                rightPos++;
            }
            mergePos++;
        }
        while (leftPos <= mid){
            mergedRay[mergePos] = numbers[leftPos];
            leftPos++;
            mergePos++;
        }
        while (rightPos <= k){
            mergedRay[mergePos] = numbers[rightPos];
            rightPos++;
            mergePos++;
        }
        for (mergePos = 0; mergePos < mergeSize; mergePos++){
            numbers[i + mergePos] = mergedRay[mergePos];
        }
        delete[] mergedRay;
    }

    void MergeSort(T* numbers, int i, int k){
        int mid;
        if (i < k){
            mid = (i + k) / 2;
            MergeSort(numbers, i, mid);
            MergeSort(numbers, mid + 1, k);
            Merge(numbers, i, mid, k);
        }
    }

    void stableSort(){
        T* temp = new T[capacityNum];
        for(int count = 0; count < size; count++){
            int rayPos = rayPosFromUserPos(count);
            temp[count] = ray[rayPos];
        }
        delete[] ray;
        ray = temp;
        front = 0;
        back = size-1;
        reverseBool = false;
        MergeSort(ray, 0, size-1);
    };

    int linearSearch(T e){
        for(int count = 0; count < size; count++){
            if((*this)[count] == e){
                return count;
            }
        }
        return -1;
    }

    int binarySearch(T arr[], int l, int r, T x)
    {
        if (r >= l) {
            int mid = l + (r - l) / 2;
            if ((*this)[mid] == x){
                return mid;
            }
            if ((*this)[mid] > x){
                return binarySearch(arr, l, mid - 1, x);
            }else{
                return binarySearch(arr, mid + 1, r, x);
            }
        }
        return -1;
    };  

    int binSearch(T e){
        return binarySearch(ray, 0, size-1, e);
    }

    void reverse(){
        int temp = front;
        front = back;
        back = temp;
        reverseBool = !reverseBool;
    }    

    int rayPosFromUserPos(int l){
        int newPos;
        if(!reverseBool){
            newPos = ((front + l)+capacityNum) % capacityNum;
        }else{
            newPos = ((front - l)+capacityNum) % capacityNum;
        }
        return newPos;
    }

    void resizeArray(int i){
        T *tempRay = new T[i];
        for(int count = 0; count < size; count++){
            int pos = rayPosFromUserPos(count);
            tempRay[count] = ray[pos];
        }
        delete[] ray;
        ray = tempRay;
        front = 0;
        back = size-1;
        capacityNum = i;
        reverseBool = false;
    }
    
};
