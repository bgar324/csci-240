void collect_le_k(vector<pair<int, char>>& heap, int index, int k, vector<pair<int, char>>& result) {
    if (index >= heap.size()) return;
    if (heap[index].first > k) return;

    result.push_back(heap[index]);
    collect_le_k(heap, 2 * index, k, result);     // left child
    collect_le_k(heap, 2 * index + 1, k, result); // right child
}
