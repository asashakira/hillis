import numpy as np

x = np.array([ [0, 0], [0, 1], [1, 0], [1, 1]])
x = np.c_[np.ones(4, dtype=int), x]
y = np.array([[0], [1], [1], [0]])

n_x, n_h, n_y = 2+1, 2+1, 1
w1 = np.random.uniform(size=(n_x, n_h))
w2 = np.random.uniform(size=(n_h, n_y))

lr = 0.1

def sigmoid(x):
    return 1/(1+np.exp(-x))
def d_sigmoid(x):
    return x*(1-x)

it = 10000
for _ in range(it):
    z1 = x @ w1
    a1 = sigmoid(z1)
    a1[:][0] = 1
    z2 = a1 @ w2
    a2 = sigmoid(z2)

    r = (y-a2) * d_sigmoid(a2)
    dw2 = a1.T @ r
    dz1 = r * d_sigmoid(a1) * w2.T
    dw1 = x.T @ dz1
    w2 += dw2*lr
    w1 += dw1*lr

for i in range(4):
    print(y[i], a2[i])

