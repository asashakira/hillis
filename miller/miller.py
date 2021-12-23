import numpy as np

N = 5
POP_SIZE = 50
lr = 0.4
epochs = 10000

x = np.array([
    [0, 0],
    [0, 1],
    [1, 0],
    [1, 1],
])
y = np.array([[0], [1], [1], [0]])

def sigmoid(x): return 1/(1+np.exp(-x))
def d_sigmoid(x): return x*(1-x)

def forward(v, w):
    for i in range(4):
        for j in range(2, N):
            for k in range(N+1):
                v[i][j] += v[i][k]*w[j][k]
            v[i][j] = sigmoid(v[i][j])
    return v, w

def backward(v, w, con):
    dw = np.zeros((N, N+1))
    for i in range(4):
        r = (y[i]-v[i][-2]) * d_sigmoid(v[i][-2])
        for j in range(N+1):
            if con[-1][j] == 1:
                dw[-1][j] += v[i][j]*r

        for j in range(2, 4):
            for k in range(0, 2):
                if con[j][k]:
                    dw[j][k] += x[i][k] * d_sigmoid(v[i][j]) * w[-1][j] * r
            if con[j][-1]:
                dw[j][-1] += d_sigmoid(v[i][j]) * w[-1][j] * r

    return w + dw*lr


def fitness(con):
    w = np.random.uniform(size=(N, N+1))
    for i in range(N):
        for j in range(N+1):
            if con[i][j]:
                w[i][j] = 0

    error = 0
    for it in range(epochs):
        v = np.c_[x, np.zeros((4, N-2)), np.ones((4, 1))]
        v, w = forward(v, w)
        for i in range(4):
            error += (y[i]-v[i][-2])[0] ** 2
        w = backward(v, w, con)

    def test():
        for i in range(4):
            X = x[i]
            Y = y[i]
            z = v[i][-2]
            print(f"{X} -> {Y} ->", "[%-.5f]" % z, end=' ')
            if Y == 0:
                if z < 0.5: print("OK")
                else: print("NG")
            else:
                if z < 0.5: print("NG")
                else: print("OK")

    test()
    return error


def crossover(parents):
    offspring = parents[:]

def mutation():
    pass

def main():
    def L(): return np.random.randint(2)
    C = []
    C.append(np.array([
        [  0,   0,   0,   0,   0,   0],
        [  0,   0,   0,   0,   0,   0],
        [  1,   1,   0,   0,   0,   1],
        [  1,   1,   0,   0,   0,   0],
        [  0,   0,   1,   1,   0,   1],
    ]))
    for _ in range(POP_SIZE-1):
        C.append(np.array([
            [  0,   0,   0,   0,   0,   0],
            [  0,   0,   0,   0,   0,   0],
            [L(), L(),   0,   0,   0, L()],
            [L(), L(),   0,   0,   0, L()],
            [L(), L(), L(), L(),   0, L()],
        ]))

    C = np.array(C)

    for i in range(POP_SIZE):
        print(fitness(C[0]))

if "__main__" == __name__:
    main()

