# Homework 3

## Plan / Psuedocode

```C
# define MIN_LENGTH_PARALLEL 1 << 4
```

```C
// Entry function
mergeSortParallel(Node*) -> Node*


nodeListToArray(Node*) -> int*

// Thread work
// Starts w/ 2 threads
// ...
// mergeLists(...)

arrayToNodeList(int*) -> Node*
```

```
type SortArg = {
    list: int*
    start: int
    end: int
}
```

```
// Sorts the subset of elements in the given array
recursiveSort(SortArg*) {
    int start = SortArg -> start
    int end = SortArg -> end
    int length = end - start

    int* list = SortArg -> list

    if (length >= MIN_LENGTH_PARALLEL) {
        // Split in half, then pass to 2 more threads
        SortArg left, right;
        // ...

        // Merge lists
        mergeLists(left, right)
    } else {
        // Perform sequential sort
        mergeSort(SortArg*)
    }
}
```