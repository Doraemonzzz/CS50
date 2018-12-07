from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def distances(a, b):
    """Calculate edit distance from a to b"""
    n1 = len(a)
    n2 = len(b)
    cost = [[] for i in range(n1 + 1)]
    cost[0].append((0, None))
    for i in range(1, n1 + 1):
        cost[i].append((i, Operation.DELETED))

    for i in range(1, n2 + 1):
        cost[0].append((i, Operation.INSERTED))


    for i in range(1, n1 + 1):
        for j in range(1, n2 + 1):
            temp1 = (cost[i - 1][j][0] + 1, Operation.DELETED)
            temp2 = (cost[i][j - 1][0] + 1, Operation.INSERTED)
            if(a[i - 1] != b[j - 1]):
                temp3 = (cost[i - 1][j - 1][0] + 1, Operation.SUBSTITUTED)
            else:
                temp3 = (cost[i - 1][j - 1][0], Operation.SUBSTITUTED)

            temp = [temp1, temp2, temp3]
            index = 0
            for k in range(3):
                if(temp[k][0] < temp[index][0]):
                    index = k

            cost[i].append(temp[index])

    # TODO
    return cost
