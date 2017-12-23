import math

# Система непересекающихся множеств
parents = [] # Массив родителей для каждой из вершин
ranks = [] # Ранг для эвристики присоединения меньшего множества к большему

# Поиск и простановка родительской вершины
def find(city):
    if parents[city] == city:
        return city
    else:
        parents[city] = find(parents[city])
        return parents[city]

# Объединение двух множеств
def union(cityOne, cityTwo):
    parentCityOne = find(cityOne)
    parentCityTwo = find(cityTwo)
    if (parentCityOne != parentCityTwo):
        if ranks[parentCityOne] == ranks[parentCityTwo]:
            ranks[parentCityOne] += 1
        if ranks[parentCityOne] < ranks[parentCityTwo]:
            parents[parentCityOne] = parentCityTwo
        else:
            parents[parentCityTwo] = parentCityOne
        return True
    else:
        return False

# Подсчет веса ребра между двумя городами
def distance(pointOne, pointTwo):
    return math.sqrt((pointTwo[0] - pointOne[0])**2 + (pointTwo[1] - pointOne[1])**2)

# Построение полного графа - дорогая железная, если её длина превосходит заданный порог
def getRoads(coordinates, r):
    roads = []
    railroads = []
    for i in range(len(coordinates)):
        for j in range(i+1, len(coordinates)):
            pointsDistance = distance(coordinates[i], coordinates[j])
            if pointsDistance < r:
                roads.append([pointsDistance, (i, j)])
            else:
                railroads.append([pointsDistance, (i, j)])
    return [roads, railroads]

# Подсчет числа дорог и их суммарного веса при построении MST.
def getCount(edges, r, n):
    roadsCount = 0
    sumWeight = 0
    for e in edges:
        if roadsCount == n-1:
           break
        if union(e[1][0], e[1][1]):
            roadsCount += 1
            sumWeight += e[0]
    return [roadsCount+1, sumWeight]

if __name__=='__main__':
    testCasesNumber = int(input())
    for testCaseNumber in range(testCasesNumber):
        n, r = list(map(int, input().split(' '))) # Считываем данные
        parents = list(range(n))
        ranks = [0 for _ in range(n)]
        coordinates = []
        for coordinate in range(n):
            coordinates.append(list(map(int, input().split(' '))))
        
        roads, railroads = getRoads(coordinates, r)
        roads.sort(key= lambda x : x[0])
        railroads.sort(key= lambda x : x[0])
        
        # Построение MST применить последовательно сначала для обычных дорог, а затем для железных, т.к. алгоритм жадный, а минимальная длина железной дороги больше максимальной длины обычной дороги
        _, roadsWeight = getCount(roads, r, n)
        statesCount, railroadsWeight = getCount(railroads, r, n)
        print("Case #{}: {} {} {}".format(testCaseNumber+1, statesCount, int(round(roadsWeight,0) ), int(round(railroadsWeight,0) )))
