package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

var N, M int

func toKey(board [][]int) string {
	s := ""
	for i := 0; i < N; i++ {
		for j := 0; j < M; j++ {
			s += strconv.Itoa(board[i][j])
		}
	}
	return s
}

func copyBoard(board [][]int) [][]int {
	newBoard := make([][]int, N)
	for i := 0; i < N; i++ {
		newBoard[i] = make([]int, M)
		for j := 0; j < M; j++ {
			newBoard[i][j] = board[i][j]
		}
	}
	return newBoard
}

func isChess(board [][]int) bool {
	v := board[0][0]
	for i := 0; i < N; i++ {
		for j := 0; j < M; j++ {
			var expected int
			if (i+j)%2 == 0 {
				expected = v
			} else {
				expected = 1 - v
			}
			if board[i][j] != expected {
				return false
			}
		}
	}
	return true
}

func printBoard(board [][]int) {
	for i := 0; i < N; i++ {
		for j := 0; j < M; j++ {
			fmt.Print(board[i][j])
		}
		fmt.Println()
	}
}

func bfs(start [][]int) (int, [][]int) {
	queue := [][][]int{start}
	dist := make(map[string]int)
	dist[toKey(start)] = 0

	for len(queue) > 0 {
		cur := queue[0]
		queue = queue[1:]
		d := dist[toKey(cur)]

		if isChess(cur) {
			return d, cur
		}

		for i := 0; i < N; i++ {
			for j := 0; j < M; j++ {
				if j+1 < M {
					nb := copyBoard(cur)
					nb[i][j] = 1 - nb[i][j]
					nb[i][j+1] = 1 - nb[i][j+1]
					key := toKey(nb)
					if _, ok := dist[key]; !ok {
						dist[key] = d + 1
						queue = append(queue, nb)
					}
				}
				if i+1 < N {
					nb := copyBoard(cur)
					nb[i][j] = 1 - nb[i][j]
					nb[i+1][j] = 1 - nb[i+1][j]
					key := toKey(nb)
					if _, ok := dist[key]; !ok {
						dist[key] = d + 1
						queue = append(queue, nb)
					}
				}
			}
		}
	}
	return -1, nil
}

func main() {
	reader := bufio.NewReader(os.Stdin)

	fmt.Print("Введите размеры доски N и M: ")
	fmt.Scan(&N, &M)

	start := make([][]int, N)
	for i := 0; i < N; i++ {
		start[i] = make([]int, M)
	}

	fmt.Println("Введите доску (0 - орёл, 1 - решка):")
	for i := 0; i < N; i++ {
		var s string
		fmt.Scan(&s)
		for j := 0; j < M; j++ {
			start[i][j] = int(s[j] - '0')
		}
	}

	moves, finalBoard := bfs(start)

	fmt.Printf("\nМинимальное количество ходов: %d\n", moves)
	fmt.Println("Финальная доска (шахматный паттерн):")
	printBoard(finalBoard)
}
