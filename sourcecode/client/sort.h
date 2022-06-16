#ifndef SORT_H
#define SORT_H
#include<QWidget>

template<class T>
class Sort {
    Q_OBJECT
public:
    Sort();
    ~Sort();
    void Merge(T *array,int l, int mid, int r, bool (*compare)(T x, T y));
    void sort(T *array, int l, int r, bool (*compare)(T x, T y));
};

template<typename T>
inline void Sort<T>::Merge(T *array, int l, int mid, int r, bool (*compare)(T x, T y)) {
    T *temp = new T[r - l + 1];
    int i = l;
    int j = mid + 1;
    int k = 0;
    while (i <= mid &&j <= r) {
        if (compare(array[j], array[i])) temp[k++] = array[i++];
        else temp[k++] = array[j++];
    }
    while (i <= mid) temp[k++] = array[i++];
    while (j <= r) temp[k++] = array[j++];
    for (i = l, k = 0; i <= r;) array[i++] = temp[k++];
    delete []temp;
}

template<typename T>
inline void Sort<T>::sort(T * array, int l, int r, bool (*compare)(T x, T y)) {
    if (l < r) {
        int mid = (l + r) / 2;
        sort(array, l, mid, compare);
        sort(array, mid + 1, r, compare);
        Merge(array, l, mid, r, compare);
    }
}

#endif // SORT_H
