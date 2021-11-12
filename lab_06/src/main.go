package main

import (
	"fmt"
	"os"
	"./algorithms"
)

func main() {
	action := -1

	for ; action != 4; {
		action = choose_action()
		for ; action == -1; {
			action = choose_action()
		}
		if action == 1 {
			a := read_matrix()
			print_matrix(a)
			l, path := algorithms.BruteForce(a)
			fmt.Printf("\nМинимальная суммарная длина пути: %d\n", l)
			fmt.Printf("Путь: ")
			for i := 0; i < len(path); i++ {
				fmt.Printf("%d вершина ", path[i])
				if i != (len(path) - 1) {
					fmt.Printf(" -> ")
				}
			}
			fmt.Printf("-> %d вершина\n", path[0])
		}
		if action == 2 {
			a := read_matrix()
			print_matrix(a)
			l, path := algorithms.AntsSearch(a)
			fmt.Printf("\nМинимальная суммарная длина пути: %d\n", l)
			fmt.Printf("Путь: ")
			for i := 0; i < len(path); i++ {
				fmt.Printf("%d вершина ", path[i] + 1)
				if i != (len(path) - 1) {
					fmt.Printf(" -> ")
				}
			}
			fmt.Printf("-> %d вершина\n", path[0] + 1)
		}
		if action == 3 {
			algorithms.Log()
		}
	}
}

func read_matrix() ([][]int) {
	f, _ := os.Open("../localfile.txt")
	var n int
	fmt.Fscanf(f, "%d", &n)
	var a = make([][]int, n)
	for i := 0; i < n; i++ {
		a[i] = make([]int, n)
		for j := 0; j < n; j++ {
			fmt.Fscanf(f, "%d", &a[i][j])
		}
	}
	return a
}

func print_matrix(a [][]int) {
	n := len(a)
	fmt.Printf("\nМатрица смежности:\n\n")
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++{
			fmt.Printf("%d ", a[i][j])
		}
		fmt.Printf("\n")
	}
}

func choose_action() int {
	var (
		action int
		err error
	)
	fmt.Println()
	fmt.Println("Выберите действие:")
	fmt.Println("1) Решить задачу коммивояжера полным перебором")
	fmt.Println("2) Решить задачу коммивояжера муравьиным алгоритмом")
	fmt.Println("3) Показать сравнительный анализ алгоритмов")
	fmt.Println("4) Завершить программу")
	fmt.Println()
	fmt.Print("Ваш ответ: ")
	fmt.Scan(&action)
	if err != nil || action < 1 || action > 4 {
		fmt.Println("Ошибка ввода. Должно быть введено целое число, являющееся одним из пунктов меню.")
		return -1
	}
	return action
}
