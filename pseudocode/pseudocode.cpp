using namespace std;

void mergeSort(vector<int> list) {
    if (list.size() <= 1) {
        return;
    }

    int mid = list.size() / 2;
    vector<int> left;
    vector<int> right;

    left = list.subList(0, mid);
    right = list.subList(mid, list.size());

    mergeSort(left);
    mergeSort(right);
    merge(list, left, right);    
}