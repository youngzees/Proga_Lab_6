package main

import (
	"fmt"
	"math"
)

const n = 4
const eps = 1e-3

var A = [n][n]float64{
	{0.91, -0.04, 0.21, -1.16},
	{0.25, -1.23, -0.23, -0.09},
	{-0.21, -0.23, 0.8, -0.13},
	{0.15, -1.31, 0.06, -1.04},
}

var b = [n]float64{-1.24, -1.04, 2.56, 0.91}

func luDecomposition(L, U *[n][n]float64) {
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			L[i][j] = 0
			U[i][j] = 0
		}
		L[i][i] = 1
	}

	for i := 0; i < n; i++ {
		for j := i; j < n; j++ {
			sum := 0.0
			for k := 0; k < i; k++ {
				sum += L[i][k] * U[k][j]
			}
			U[i][j] = A[i][j] - sum
		}

		for j := i + 1; j < n; j++ {
			sum := 0.0
			for k := 0; k < i; k++ {
				sum += L[j][k] * U[k][i]
			}
			L[j][i] = (A[j][i] - sum) / U[i][i]
		}
	}
}

func solveLy(L *[n][n]float64, b [n]float64, y *[n]float64) {
	for i := 0; i < n; i++ {
		sum := 0.0
		for j := 0; j < i; j++ {
			sum += L[i][j] * y[j]
		}
		y[i] = b[i] - sum
	}
}

func solveUx(U *[n][n]float64, y [n]float64, x *[n]float64) {
	for i := n - 1; i >= 0; i-- {
		sum := 0.0
		for j := i + 1; j < n; j++ {
			sum += U[i][j] * x[j]
		}
		x[i] = (y[i] - sum) / U[i][i]
	}
}

func directMethod(x *[n]float64) {
	fmt.Println("\nПРЯМОЙ МЕТОД: LU-разложение\n")

	var L, U [n][n]float64
	luDecomposition(&L, &U)

	fmt.Println("Матрица L:")
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			fmt.Printf("%10.4f ", L[i][j])
		}
		fmt.Println()
	}

	fmt.Println("\nМатрица U:")
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			fmt.Printf("%10.4f ", U[i][j])
		}
		fmt.Println()
	}

	var y [n]float64
	solveLy(&L, b, &y)

	fmt.Println("\nВектор y (решение Ly = b):")
	for i := 0; i < n; i++ {
		fmt.Printf("y[%d] = %10.6f\n", i, y[i])
	}

	solveUx(&U, y, x)

	fmt.Println("\nРешение системы (прямой метод):")
	for i := 0; i < n; i++ {
		fmt.Printf("x[%d] = %12.6f\n", i, x[i])
	}
}

func seidelMethod(xStart [n]float64, eps float64) {
	fmt.Println("\nПРИБЛИЖЁННЫЙ МЕТОД: Зейделя\n")

	var x, xPrev [n]float64
	for i := 0; i < n; i++ {
		x[i] = xStart[i]
		xPrev[i] = xStart[i]
	}

	// Проверка диагонального преобладания
	diagDom := true
	for i := 0; i < n; i++ {
		sum := 0.0
		for j := 0; j < n; j++ {
			if i != j {
				sum += math.Abs(A[i][j])
			}
		}
		if math.Abs(A[i][i]) <= sum {
			diagDom = false
		}
	}
	if diagDom {
		fmt.Println("Диагональное преобладание есть -> сходимость гарантирована")
	} else {
		fmt.Println("Диагонального преобладания нет, но метод может сойтись")
	}

	fmt.Println("\nN   x1         x2         x3         x4         epsilon")

	iter := 0
	var maxDiff float64

	for {
		for i := 0; i < n; i++ {
			xPrev[i] = x[i]
		}

		for i := 0; i < n; i++ {
			sum := b[i]
			for j := 0; j < n; j++ {
				if j != i {
					sum -= A[i][j] * x[j]
				}
			}
			x[i] = sum / A[i][i]
		}

		maxDiff = 0
		for i := 0; i < n; i++ {
			diff := math.Abs(x[i] - xPrev[i])
			if diff > maxDiff {
				maxDiff = diff
			}
		}

		fmt.Printf("%d   %10.6f %10.6f %10.6f %10.6f %10.6f\n", iter, x[0], x[1], x[2], x[3], maxDiff)

		iter++

		if maxDiff <= eps || iter >= 1000 {
			break
		}
	}

	fmt.Printf("\nРешение системы (метод Зейделя, eps = %g):\n", eps)
	for i := 0; i < n; i++ {
		fmt.Printf("x[%d] = %12.6f\n", i, x[i])
	}
	fmt.Printf("Количество итераций: %d\n", iter)
}

func main() {
	fmt.Println("Вариант 8: M = 0.91, N = -0.23, P = -1.04")
	fmt.Println("Прямой метод: LU-разложение")
	fmt.Println("Приближённый метод: Зейделя")
	fmt.Printf("Точность eps = %g\n", eps)

	fmt.Println("\nИсходная матрица A:")
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			fmt.Printf("%10.4f ", A[i][j])
		}
		fmt.Printf("| %10.4f\n", b[i])
	}

	var xDirect [n]float64
	directMethod(&xDirect)

	var xStart [n]float64 = [n]float64{0, 0, 0, 0}
	seidelMethod(xStart, eps)
}
