QUICKSELECT(A, k, left, right):
    if left == right:
        return A[left]
    pivotIndex = RANDOM(left, right)
    pivotIndex = PARTITION(A, left, right, pivotIndex)
    if k == pivotIndex:
        return A[k]
    else if k < pivotIndex:
        return QUICKSELECT(A, k, left, pivotIndex - 1)
    else:
        return QUICKSELECT(A, k, pivotIndex + 1, right)

PARTITION(A, left, right, pivotIndex):
    pivotValue = A[pivotIndex]
    SWAP A[pivotIndex] and A[right]
    storeIndex = left
    for i = left to right - 1:
        if A[i] < pivotValue:
            SWAP A[storeIndex] and A[i]
            storeIndex = storeIndex + 1
    SWAP A[right] and A[storeIndex]
    return storeIndex
