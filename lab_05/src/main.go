package main

import "fmt"
import "./pipeline"
import "time"

func main() {

	var action int

	action = -1

	for ; action != 3; {
		 
		action = choose_action()
		for ; action == -1; {
			action = choose_action()
		}

		if action == 1 {
			var data pipeline.File_data_t
			data = pipeline.ReadFile()
			start := time.Now()
			wait_chanel := make(chan int)
			q := pipeline.Pipeline(data.Figure.Size, wait_chanel, &data, 1)
			<-wait_chanel
			end := time.Now()

			fmt.Printf("\nВремя, затраченное на обработку файла local.txt: %v\n", end.Sub(start))

			pipeline.WriteFile(q, "pipeline_result.txt")
			q_s := pipeline.Sync(data)
			pipeline.WriteFile(q_s, "sync_result.txt")
		}
		if action == 2 {
			var data pipeline.File_data_t
			data = pipeline.ReadFile()
			wait_chanel := make(chan int)
			q_p := pipeline.Pipeline(data.Figure.Size, wait_chanel, &data, 0)
			<-wait_chanel

			q_s := pipeline.Sync(data)

			pipeline.Log(q_p, q_s)
		}
	} 
}

func choose_action() int {
	var (
		action int
		err error
	)

	fmt.Println()
	fmt.Println("Выберите действие:")
	fmt.Println("1) Считать данные из файла и произвести операции преобразования с помощью конвейера")
	fmt.Println("2) Показать таблицу логирования")
	fmt.Println("3) Завершить программу")
	fmt.Println()
	fmt.Print("Ваш ответ: ")
	fmt.Scan(&action)

	if err != nil || action < 1 || action > 3 {
		fmt.Println("Ошибка ввода. Должно быть введено целое число, являющееся одним из пунктов меню.")
		return -1
	}

	return action
}