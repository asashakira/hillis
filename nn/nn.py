import numpy as np

def sigmoid(x): return 1/(1+np.exp(-x))
def d_sigmoid(x): return x*(1-x)

class NN:
    def __init__(self, x, y, n_x, n_h, n_y, lr=0.4, epochs=10000):
        self.x = np.c_[np.ones(len(x), dtype=int), x]
        self.y = y
        self.lr = lr
        self.epochs = epochs

        self.n_x = n_x + 1
        self.n_h = n_h + 1
        self.n_y = n_y

        self.w1 = np.random.uniform(size=(self.n_x, self.n_h))
        self.w2 = np.random.uniform(size=(self.n_h, self.n_y))

    def forward_prop(self, x, w1, w2):
        z1 = x @ w1
        a1 = sigmoid(z1)
        for i in range(len(a1)):
            a1[i][0] = 1
        z2 = a1 @ w2
        a2 = sigmoid(z2)
        return a1, a2

    def back_prop(self, x, y, w1, w2, a1, a2):
        r = (y-a2) * d_sigmoid(a2)
        dw2 = a1.T @ r
        dz1 = r * d_sigmoid(a1) * w2.T
        dw1 = x.T @ dz1
        return dw1, dw2

    def train(self):
        for _ in range(self.epochs):
            a1, a2 = self.forward_prop(self.x, self.w1, self.w2)
            dw1, dw2 = self.back_prop(self.x, self.y, self.w1, self.w2, a1, a2)
            self.w1 += dw1 * self.lr
            self.w2 += dw2 * self.lr

    def test(self):
        a1, a2 = self.forward_prop(self.x, self.w1, self.w2)
        for i in range(len(self.x)):
            x = self.x[i]
            y = self.y[i]
            z = a2[i]
            print(f"{x[1:]} -> {y} ->", "[%-.5f]" % z, end=' ')
            if y == 0:
                if z < 0.5: print("OK")
                else: print("NG")
            else:
                if z < 0.5: print("NG")
                else: print("OK")

def xor():
    x = np.array([
        [0, 0],
        [0, 1],
        [1, 0],
        [1, 1]
    ])
    y = np.array([[0], [1], [1], [0]])

    xor = NN(x, y, 2, 2, 1)
    xor.train()
    xor.test()

def msd():
    a = []
    b = []
    for bit in range(1 << 6):
        x = []
        for i in range(6):
            if bit & (1 << i): x.append(1)
            else: x.append(0)
        a.append(x)
        y = [1] if x[0] == x[5] and x[1] == x[4] and x[2] == x[3] else [0]
        b.append(y)

    if 0:
        for i in range(64):
            r = np.random.randint(low=0, high=64)
            a[i], a[r] = a[r], a[i]

    x = np.array(a)
    y = np.array(b)

    msd = NN(x, y, 6, 2, 1, lr=0.7, epochs=50000)
    msd.train()
    msd.test()

if __name__ == "__main__":
    xor()
