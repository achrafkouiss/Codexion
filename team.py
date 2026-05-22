def Next_Round():
    n, k = map(int, input().split())

    *scores,  = map(int, input().split())
    if len(scores) != n:
        return False
    counter = 0
    count = sum(1 for x in scores if x >= k and x == scores[4])
    for index in range(len(scores)):
        if index == k:
            break 
        if scores[index] > 0:
            counter += 1
    i = index
    while (i < n and scores[i] != 0):
        if (scores[index] == scores[i]):
            counter += 1
        else:
            break
        i += 1
    print(counter)



Next_Round()