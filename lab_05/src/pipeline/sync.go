package pipeline

import (
	"math"
	"time"
)

func Sync(data File_data_t) *Queue_t {
	pipeline_1 := CreateQueue(data.Figure.Size)
	pipeline_2 := CreateQueue(data.Figure.Size)
	pipeline_3 := CreateQueue(data.Figure.Size)

	dx := data.params[0][0]
	dy := data.params[0][1]

	kx := data.params[2][2]
	ky := data.params[2][3]

	a := data.params[1][2]

	r_xc := data.params[1][0]
	r_yc := data.params[1][1]

	s_xc := data.params[2][0]
	s_yc := data.params[2][1]

	for i := 0; i < data.Figure.Size; i++ {
		task := new(Task_t)
		task.start_move = time.Now()
		task.dot.x, task.dot.y = smove(data.Figure.Dots[i].x, data.Figure.Dots[i].y, dx, dy)
		task.end_move = time.Now()
		pipeline_1.push(task)
	}

	for i := 0; i < data.Figure.Size; i++ {
		task := pipeline_1.pop()
		task.start_rotate = time.Now()
		task.dot.x, task.dot.y = srotate(task.dot.x, task.dot.y, r_xc, r_yc, a)
		task.end_rotate = time.Now()
		pipeline_2.push(task)
	}

	for i := 0; i < data.Figure.Size; i++ {
		task := pipeline_2.pop()
		task.start_scale = time.Now()
		task.dot.x, task.dot.y = sscale(task.dot.x, task.dot.y, s_xc, s_yc, kx, ky)
		pipeline_3.push(task)
		task.end_scale = time.Now()
	}

	return pipeline_3
}

func smove(x int, y int, dx int, dy int) (int, int) {
	x += dx
	y += dy

	return x, y
}

func sscale(x int, y int, xc int, yc int, kx int, ky int) (int, int) {
	x = (x - xc) * kx + xc
	y = (y - yc) * ky + yc
	return x, y
}

func srotate(x int, y int, xc int, yc int, angle int) (int, int) {
	xt := x
	yt := y

	a := float64(float64(angle) * 3.14 / 180)

	x = xc + int(float64(xt - xc) * math.Cos(a) + float64(yt - yc) * math.Sin(a))
	y = yc - int(float64(xt - xc) * math.Sin(a) - float64(yt - yc) * math.Cos(a))

	return x, y
}