package pipeline

import (
	"time"
	"math"
	"os"
	"fmt"
	"github.com/logrusorgru/aurora"
)

func Pipeline(n int, ch chan int, data *File_data_t, output int) *Queue_t {
	pipeline_1 := make(chan *Task_t, 50000)
	pipeline_2 := make(chan *Task_t, 50000)
	pipeline_3 := make(chan *Task_t, 50000)

	q := CreateQueue(n)

	dx := data.params[0][0]
	dy := data.params[0][1]

	r_xc := data.params[1][0]
	r_yc := data.params[1][1]
	angle := float64(data.params[1][2])
	a := float64(float64(angle) * 3.14 / 180)

	s_xc := data.params[2][0]
	s_yc := data.params[2][1]
	kx := data.params[2][2]
	ky := data.params[2][3]

	go func() {
		for {
			select {
			case task := <-pipeline_1:

				task.start_move = time.Now()
				task.dot.x += dx
				task.dot.y += dy
				task.end_move = time.Now()

				if output == 1 {
					fmt.Println(aurora.BgBrightBlack(aurora.Sprintf(aurora.Red("1 лента. Перемещение точки:     (%3d;%3d), №%3d, time:%s"), 
								task.dot.x, task.dot.y, task.index, time.Now())))
				}

				pipeline_2 <- task

			case task := <- pipeline_2:

				task.start_rotate = time.Now()
				xt := task.dot.x
				yt := task.dot.y
				task.dot.x = int(float64(r_xc) + float64((xt - r_xc)) * math.Cos(a) + float64((yt - r_yc)) * math.Sin(a))
				task.dot.y = int(float64(r_yc) - float64((xt - r_xc)) * math.Sin(a) + float64((yt - r_yc)) * math.Cos(a))
				task.end_rotate = time.Now()

				if output == 1 {
					fmt.Println(aurora.BgBrightBlack(aurora.Sprintf(aurora.Yellow("2 лента. Поворот точки:         (%3d;%3d), №%3d, time:%s"), 
								task.dot.x, task.dot.y, task.index, time.Now())))
				}

				pipeline_3 <- task

			case task:= <- pipeline_3:

				task.start_scale = time.Now()
				task.dot.x = (task.dot.x - s_xc) * kx + s_xc
				task.dot.y = (task.dot.y - s_yc) * ky + s_yc
				task.end_scale = time.Now()

				if output == 1 {
					fmt.Println(aurora.BgBrightBlack(aurora.Sprintf(aurora.Green("3 лента. Масштабирование точки: (%3d;%3d), №%3d, time:%s"), 
								task.dot.x, task.dot.y, task.index, time.Now())))
				}

				q.push(task)
				if task.index == (n-1) {
					ch <- 0
				}
			}
		}
	}()

	for i := 0; i < n; i++ {
		task := new(Task_t)
		task.dot = data.Figure.Dots[i]
		task.index = i
		pipeline_1 <- task
	}

	return q
}

func WriteFile(q *Queue_t, name string) {
	f, _ := os.Create(name)

	fmt.Fprintf(f, "%d\n", len(q.Q))

	for i := 0; i < q.Size; i++ {
		fmt.Fprintf(f, "%d %d\n", q.Q[i].dot.x, q.Q[i].dot.y)
	}

	defer f.Close()
}

func ReadFile() File_data_t {
	f, _ := os.Open("localfile.txt")

	var data File_data_t

	fmt.Fscanf(f, "%d %d\n", &data.params[0][0], &data.params[0][1])
	fmt.Fscanf(f, "%d %d %d %d\n", &data.params[1][0], &data.params[1][1], &data.params[1][2], &data.params[1][3])
	fmt.Fscanf(f, "%d %d %d %d\n", &data.params[2][0], &data.params[2][1], &data.params[2][2], &data.params[2][3])

	fmt.Fscanf(f, "%d\n", &data.Figure.Size)

	data.Figure.Dots = make([]Dot_t, data.Figure.Size)

	for i := 0; i < data.Figure.Size; i++ {
		fmt.Fscanf(f, "%d %d\n", &data.Figure.Dots[i].x, &data.Figure.Dots[i].y)
	} 

	return data
}