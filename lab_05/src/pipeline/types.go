package pipeline

import "time"

type Dot_t struct {
	x int
	y int
}

type Figure_t struct {
	Size int
	Dots []Dot_t
}

type File_data_t struct {
	Figure Figure_t
	params [3][4]int
}

type Task_t struct {
	dot Dot_t
	index int

	start_move   time.Time
	end_move     time.Time

	start_rotate time.Time
	end_rotate   time.Time

	start_scale  time.Time
	end_scale    time.Time
}

type Queue_t struct {
	Q [](*Task_t)
	Size int
}