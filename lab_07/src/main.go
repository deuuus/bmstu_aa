package main

import (
	"errors"
	"fmt"
	"io"
	"os"
	"./find"
)

func main() {
	d := ReadData()
	var action int
	var key int
	for action != 5 {
		action = ChooseAction()
		for action == -1 {
			action = ChooseAction()
		}
		if action == 1 {
			PrintTableDict(d)
		}
		if action == 2 {
			PrintTextDict()
		}
		if action == 3 {
			key = GetKey()
			for key == -1 {
				key = GetKey()
			}
			fmt.Println()
			fmt.Println("Поиск методом частотного анализа: ") 
			fmt.Printf("dictionary[%v] = %v\n", key, find.FreqFind(d, key))

			fmt.Println()
			fmt.Println("Поиск методом полного перебора: ") 
			fmt.Printf("dictionary[%v] = %v\n", key, find.BruteForce(d, key))

			fmt.Println()
			fmt.Println("Поиск методом бинарного поиска: ") 
			fmt.Printf("dictionary[%v] = %v\n", key, find.BinarySearch(d, key))
		}
		if action == 4 {
			find.LogBruteForce(d)
			find.LogBinarySearch(d)
			find.LogFreqFind(d)
		}
	}
}

func PrintTableDict(dict find.Dict_t) {
	fmt.Println("Dictionary:")
	for i := 0; i < len(dict); i += 6 {
		for j := 0; j < 6 && (j + i) < len(dict); j++ {
			fmt.Printf("|%4v -> %15v  ", i + j + 1, dict[i + j])
		}
		fmt.Println("|")
	}
}

func PrintTextDict() {
	fmt.Println()
	fmt.Println("Dictionary:")
	f, _ := os.Open("data/fairytale.txt")

	var err error
	var temp string
	_, err = fmt.Fscanf(f, "%s", &temp)
	for !errors.Is(err, io.EOF) {
		fmt.Printf("%s ", temp)
		_, err = fmt.Fscanf(f, "%s", &temp)
	}
	f.Close()
	fmt.Println()
}

func ReadData() find.Dict_t {
	dict := make(find.Dict_t)
	f, _ := os.Open("data/data.txt")

	var err error
	var temp string
	_, err = fmt.Fscanf(f, "%s", &temp)
	i := 0
	for err == nil {
		dict[i] = temp
		i += 1
		_, err = fmt.Fscanf(f, "%s", &temp)
	}
	f.Close()
	return dict
}

func ChooseAction() int {
	var (
		action int
		err error
	)

	fmt.Println()
	fmt.Println("Выберите действие:")
	fmt.Println("1) Вывести данные из словаря в табличном формате")
	fmt.Println("2) Вывести данные из словаря в текстовом формате")
	fmt.Println("3) Найти значение в словаре по ключу")
	fmt.Println("4) Логировать характеристики алгоритмов в файлы")
	fmt.Println("5) Завершить программу")
	fmt.Println()
	fmt.Print("Ваш ответ: ")
	fmt.Scan(&action)

	if err != nil || action < 1 || action > 5 {
		fmt.Println("Ошибка ввода. Должно быть введено целое число, являющееся одним из пунктов меню.")
		return -1
	}

	return action
}

func GetKey() int {
	var key int
	var err error
	fmt.Println()
	fmt.Print("Введите значение ключа: ")
	fmt.Scan(&key)
	if err != nil || key < 0{
		fmt.Println("Ошибка ввода. Должно быть введено целое неотрицательное число.")
		return -1
	}
	return key
}