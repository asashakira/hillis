#!/usr/bin/env python
# coding: utf-8

import numpy as np

filename = "1_03"
INF = 1001001001
MAX_GEN = 10000
POP_SIZE = 10
MAX_MATING = POP_SIZE//2

W = []
V = []
with open('in/'+filename) as f:
    N, M = map(int, f.readline().split())
    for i in range(N):
        w, v = map(int, f.readline().split())
        W.append(w)
        V.append(v)
weights = np.array(W)
values = np.array(V)

def init():
    pop = np.random.randint(2, size=(POP_SIZE, N))
    return pop

def calc_fit(weights, values, pop):
    fitness = []
    for i in range(POP_SIZE):
        w, v = 0, 0
        for j in range(N):
            if pop[i][j] == 1:
                w += weights[j]
                v += values[j]
        fitness.append(0 if w > M else v)
    return np.array(fitness)

def selection(pop, fitness, n_parents):
    parents = np.empty((n_parents, pop.shape[1]), dtype=int)
    for i in range(n_parents):
        idx = np.where(fitness == np.max(fitness))[0][0]
        parents[i, :] = pop[idx, :]
        fitness[idx] = -INF
    return parents

def crossover(parents, n_offspring):
    offspring = np.empty(n_offspring, dtype=int)    
    crossover_point = np.uint8(n_offspring[1]/2)
    for i in range(n_offspring[0]):
        par1_idx = i % parents.shape[0]
        par2_idx = (i+1) % parents.shape[0]
        offspring[i, 0:crossover_point] = parents[par1_idx, 0:crossover_point]
        offspring[i, crossover_point:] = parents[par2_idx, crossover_point:]
    return offspring

def mutation(offspring):
    for i in range(offspring.shape[0]):
        j = np.random.randint(N)
        #offspring[i, j] ^= 1
    return offspring

for _ in range(MAX_GEN):
    pop = init()
    fitness = calc_fit(weights, values, pop)
    print(fitness[np.where(fitness == np.max(fitness))[0][0]])
    parents = selection(pop, fitness, MAX_MATING)
    offspring = crossover(parents, n_offspring=(POP_SIZE-parents.shape[0], N))
    offspring = mutation(offspring)
    pop[0:parents.shape[0], :] = parents
    pop[parents.shape[0]:, :] = offspring
    
fitness = calc_fit(weights, values, pop)
#print(fitness[np.where(fitness == np.max(fitness))[0][0]])

with open('out/'+filename) as f:
    print(f.readline())

