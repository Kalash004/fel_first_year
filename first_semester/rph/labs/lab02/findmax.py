import time

def timeit(func, arr):
    start = time.perf_counter();
    print(func(arr));
    end = time.perf_counter();
    elapsed = end - start
    print(f'Time taken: {elapsed:.6f} seconds')


def find_max(lst: [int]) -> int:
    if type(lst) != type([]):
        return None;
    if len(lst) == 0:
        return None;
    lst.sort(reverse=True);
    return lst[0];


def find_max_my_algo(lst: [int]) -> int:
    if type(lst) != type([int|float]):
        return None;
    if len(lst) == 0:
        return None;
    cur_max = lst[0]
    for i in lst:
        if i > cur_max:
            cur_max = i;
    return cur_max;


if __name__ == "__main__":
    lst = [19,29,690,55,22111,2,1,59,32,31,653,19,29,690,55,22111,2,1,59,32,31,653,19,29,690,55,22111,2,1,59,32,31,653,19,29,690,55,22111,2,1,59,32,31,653, 12.3] 
    # lst = 1;
    timeit(find_max_my_algo, lst);
    timeit(find_max, lst);