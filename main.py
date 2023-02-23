import numpy as np
from scipy.sparse import lil_matrix, csr_matrix
from scipy.sparse.linalg import spsolve

# Определяем размеры сетки
nx, ny, nz = 11, 11, 11

# Определяем размеры области, в которой будет рассчитываться поле
x_min, x_max = 0, 1
y_min, y_max = 0, 1
z_min, z_max = 0, 1

# Создаем равномерно распределенную сетку внутри области
x = np.linspace(x_min, x_max, nx)
y = np.linspace(y_min, y_max, ny)
z = np.linspace(z_min, z_max, nz)

# Создаем сетку координат
X, Y, Z = np.meshgrid(x, y, z)

# Определяем начальные и граничные условия
V = np.zeros((nx, ny, nz))
V[0,:,:] = 1
V[nx-1,:,:] = -1

# Создаем разреженную матрицу для решения уравнения Пуассона
A = lil_matrix((nx*ny*nz, nx*ny*nz))

# Заполняем матрицу A соответствующими значениями
for i in range(nx):
    for j in range(ny):
        for k in range(nz):
            n = i*ny*nz + j*nz + k
            if i == 0 or i == nx-1 or j == 0 or j == ny-1 or k == 0 or k == nz-1:
                A[n, n] = 1
            else:
                A[n, n] = -6
                A[n, n-1] = 1
                A[n, n+1] = 1
                A[n, n-ny*nz] = 1
                A[n, n+ny*nz] = 1
                A[n, n-nz] = 1
                A[n, n+nz] = 1

# Преобразуем матрицу в CSR-формат для ускорения вычислений
A = csr_matrix(A)

# Решаем уравнение Пуассона для определения потенциала V
b = np.zeros(nx*ny*nz)
b = -1 * np.ravel(V)

V_flat = spsolve(A, b)

#Преобразуем полученный одномерный массив в трехмерный массив
V = np.reshape(V_flat, (nx, ny, nz))

#Вычисляем электрическое поле как градиент потенциала
Ex, Ey, Ez = np.gradient(-V)

#Визуализируем результаты с помощью библиотеки Matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import axes3d

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

#Визуализация потенциала
ax.set_title('Electric potential')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.plot_surface(X, Y, Z, rstride=1, cstride=1, facecolors=plt.cm.viridis(V))

fig.show()

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

#Визуализация электрического поля
ax.set_title('Electric field')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.quiver(X, Y, Z, Ex, Ey, Ez, length=0.1, normalize=True)

fig.show()
