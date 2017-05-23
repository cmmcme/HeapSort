#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
using namespace std;
int tc, m1, m2, n;
int Size = 1;
int aSize = 1;
int heapSize;
int AheapSize;
int m2res = 0;
vector<int> vec;
vector<int> accvec;
void constructHeap(int);
void heapSort();
int deleteMax(int);
int fixHeap(int, int, int);
int comp(int);
void AccconstructHeap(int);
void AcceleratedheapSort();
int AccdeleteMax(int);
void fixHeapFast(int, int, int, int);
int promote(int, int, int, int);
void bubbleUpHeap(int, int, int);
int Acomp(int);
int main(int argc, const char * argv[]) {
    
    //	freopen("input.txt", "r", stdin);
    //	freopen("output.txt","w+",stdout);
    
    scanf("%d", &tc);
    while (tc--) {
        Size = 1;
        aSize = 1;
        vec.clear();
        accvec.clear();
        heapSize = 0;
        AheapSize = 0;
        m2res = 0;
        scanf("%d", &n);
        while (1) {
            if (Size > n)
                break;
            Size *= 2;
        }
        aSize = Size;
        vec.resize(Size);
        accvec.resize(aSize);
        for (int i = 0; i < n; i++) {
            int a;
            scanf("%d", &a);
            vec[i + 1] = a;
            accvec[i + 1] = a;
        }
        
        scanf("%d%d", &m1, &m2);
        heapSize = n + 1;
        AheapSize = n + 1;
        clock_t start_time, end_time;
        
        start_time = clock();
        heapSort();
        
        end_time = clock();
        double simpleHeapTime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        
        start_time = clock();
        AcceleratedheapSort();
        
        end_time = clock();
        double FastHeapTime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        
        printf("%0.3f %0.3f\n", FastHeapTime, simpleHeapTime);
        
    }
    return 0;
}

void constructHeap(int node) {
    if (node <= (Size) / 2) {
        constructHeap(2 * node);
        constructHeap(2 * node + 1);
        int K = vec[node];
        fixHeap(node, K, 0);
    }
    
    return;
}
int comp(int node) {
    
    if (2 * node + 1 >= heapSize)
        return 2 * node;
    
    return (vec[node * 2] < vec[node * 2 + 1] ? node * 2 + 1 : node * 2);
}
int fixHeap(int node, int key, int depth) {
    
    if (node > (heapSize) / 2 || 2 * node >= heapSize) {
        vec[node] = key;
        return depth - 1;
    }
    
    else {
        int maxChildIdx = comp(node);
        
        
        if (vec[maxChildIdx] <= key) {
            vec[node] = key;
            return depth - 1;
        }
        else {
            int temp = vec[maxChildIdx];
            vec[maxChildIdx] = key;
            vec[node] = temp;
            
            return fixHeap(maxChildIdx, key, depth + 1);
        }
    }
}
int deleteMax(int lastIdx) {
    int ret = vec[1];
    int last = vec[lastIdx];
    
    swap(vec[1], vec[lastIdx]);
    
    heapSize = heapSize - 1;
    int res = fixHeap(1, last, 0);
    
    if (m1 == n - lastIdx + 1) {
        printf("%d ", res);
    }
    
    return ret;
}
void heapSort() {
    constructHeap(1);
    for (int i = n; i >= 1; i--) {
        int curMax = deleteMax(i);
        vec[i] = curMax;
        if (n - i + 1 == m1) {
            return;
        }
    }
}
void AcceleratedheapSort() {
    AccconstructHeap(1);
    for (int i = n; i >= 1; i--) {
        int curMax = AccdeleteMax(i);
        
        accvec[i] = curMax;
        if (n - i + 1 == m2) {
            return;
        }
    }
}
int AccdeleteMax(int lastIdx) {
    int ret = accvec[1];
    int last = accvec[lastIdx];
    AheapSize = AheapSize - 1;
    swap(accvec[1], accvec[lastIdx]);
    
    m2res = 0;
    fixHeapFast(last, 1, (int)log2(AheapSize), n - lastIdx + 1);
    if (n - lastIdx + 1 == m2) {
        printf("%d\n", m2res);
    }
    
    
    return ret;
}
void AccconstructHeap(int node) {
    if (node < (aSize) / 2) {
        AccconstructHeap(2 * node);
        AccconstructHeap(2 * node + 1);
        int K = accvec[node];
        fixHeapFast(K, node, (int)log2(aSize) - (int)log2(node) - 1, INT_MAX);
    }
    return;
}
int Acomp(int node) {
    if (2 * node >= AheapSize)
        return -1;
    if (2 * node + 1 >= AheapSize)
        return 2 * node;
    
    return (accvec[node * 2] < accvec[node * 2 + 1] ? node * 2 + 1 : node * 2);
}
int promote(int hStop, int vacant, int h, int key) {
    int vacStop;
    if (h <= hStop) {
        vacStop = vacant;
    }
    else {
        int idx = Acomp(vacant);
        if (idx == -1)
            return vacant;
        accvec[vacant] = accvec[idx];
        vacStop = promote(hStop, idx, h - 1, key);
    }
    return vacStop;
    
}

void bubbleUpHeap(int root, int key, int vacant) {
    
    if (root == vacant)
        accvec[vacant] = key;
    else {
        int parent = vacant / 2;
        if (accvec[parent] <= key) {
            swap(accvec[vacant], accvec[parent]);
            bubbleUpHeap(root, key, parent);
        }
        else {
            accvec[vacant] = key;
        }
    }
}

void fixHeapFast(int key, int vacant, int h, int cnt) {
    if (h <= 0 || AheapSize == 2) {
        return;
    }
    else if (h == 1) {
        int maxChildNode = Acomp(vacant);
        if (maxChildNode == -1) {
            return;
        }
        swap(accvec[vacant], accvec[maxChildNode]);
        if (accvec[vacant] <= key) {
            if (cnt == m2) {
                m2res = accvec[vacant];
            }
            swap(accvec[vacant], accvec[maxChildNode]);
        }
    }
    else {
        int hStop = (h) / 2;
        int vacStop = promote(hStop, vacant, h, key);
        
        accvec[vacStop] = key;
        int vacParent = vacStop / 2;
        if (accvec[vacParent] <= accvec[vacStop]) {
            if (cnt == m2) {
                m2res = accvec[vacParent];
            }
            accvec[vacStop] = accvec[vacParent];
            
            bubbleUpHeap(vacant, key, vacParent);
        }
        else {
            fixHeapFast(key, vacStop, hStop, cnt);
        }
    }
}
