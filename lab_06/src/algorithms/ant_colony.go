package algorithms

import (
	"math"
	"math/rand"
	"time"
)

const (
	alpha = 3.0
	beta = 1.0
	q     = 20.0
	p     = 0.8
	ph_coef = 0.5
	days = 50
)

type Params_t struct {
	alpha float64
	beta float64
	q float64
	p float64
	ph_coef float64
	days int
}

type Colony_t struct {
	matrix [][]int
	pheromon [][]float64	
	params Params_t
}

type Ant_t struct {
	colony *Colony_t
	visited [][]int
	path []int
	vertex int
}

func AntsSearch(matrix [][]int) (int, []int) {
	params := new(Params_t)
	params.alpha = alpha
	params.beta = beta
	params.q = q
	params.p = p
	params.ph_coef = ph_coef
	params.days = days
	params.q = float64(calc_q(matrix))
	colony := create_colony(matrix, params)
	return live(colony)
}

func calc_q(matrix [][]int) (float64) {
	sum := float64(0)
	for i := 0; i < len(matrix); i++ {
		for j := 0; j < len(matrix); j++ {
			sum += float64(matrix[i][j])
		}
	}
	sum /= float64(len(matrix))
	return sum
}

func create_colony(matrix [][]int, params *Params_t) *Colony_t {
	n := len(matrix)
	colony := new(Colony_t)
	colony.matrix = matrix
	colony.params = *params
	colony.pheromon = make([][]float64, n)
	for i := 0; i < n; i++ {
		colony.pheromon[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			colony.pheromon[i][j] = colony.params.ph_coef
		}
	}
	return colony
}

func live(colony *Colony_t) (int, []int) {
	n := len(colony.matrix)
	min_dist := 0
	min_path := make([]int, n)
	for i := 0; i < colony.params.days; i++ {
		for j := 0; j < n; j++ {
			ant := colony.create_ant(j)
			ant.find_path()
			ant.update_pheromon()
			cur_dist := ant.distance()
			if (cur_dist < min_dist) || (min_dist == 0) {
				min_dist = cur_dist
				copy(min_path, ant.path)
			}
		}
	}
	return min_dist, min_path
}

func (colony *Colony_t) create_ant(vertex int) *Ant_t {
	n := len(colony.matrix)
	ant := new(Ant_t)
	ant.colony = colony
	ant.visited = make([][]int, n)
	for i := 0; i < n; i++ {
		ant.visited[i] = make([]int, n)
		for j := 0; j < n; j++ {
			ant.visited[i][j] = colony.matrix[i][j]
		}
	}
	ant.vertex = vertex
	ant.path = make([]int, 0)
	ant.path = append(ant.path, ant.vertex)
	return ant
}

func (ant *Ant_t) find_path() {
	for {
		p := ant.get_probabilities()
		vertex := choose_vertex(p)
		if vertex != -1 {
			ant.move(vertex)
		} else {
			break
		}
	}
}

func (ant *Ant_t) get_probabilities() []float64 {
	n := len(ant.colony.matrix)
	probabilities := make([]float64, n)
	psum := float64(0)
	var (
		param1 float64 
		param2 float64
		)
	for i := 0; i < n; i++ {
		if ant.visited[ant.vertex][i] != 0 {
			param1 = ant.colony.pheromon[ant.vertex][i]
			param2 = (float64(1) / float64(ant.colony.matrix[ant.vertex][i]))
			d := math.Pow(param1, ant.colony.params.beta) * math.Pow(param2, ant.colony.params.alpha)
			probabilities[i] = d
			psum += d
		} else {
			probabilities[i] = 0
		}
	}
	for i := 0; i < n; i++ {
		probabilities[i] /= psum;
	}
	return probabilities
}

func choose_vertex(p []float64) int {
	n := len(p)
	sum := float64(0)
	for i := 0; i < n; i++ {
		sum += p[i]
	}
	random_probability := rand.New(rand.NewSource(time.Now().UnixNano())).Float64() * sum
	sum = 0
	for i := 0; i < n; i++ {
		if random_probability < sum + p[i] && random_probability > sum {
			return i
		}
		sum += p[i]
	}
	return -1
}

func (ant *Ant_t) move(new_vertex int) {
	n := len(ant.colony.matrix)
	for i := 0; i < n; i++ {
		ant.visited[i][ant.vertex] = 0
	}
	ant.path = append(ant.path, new_vertex)
	ant.vertex = new_vertex
}

func (ant *Ant_t) update_pheromon() {
	delta := float64(0)
	for i := 0; i < len(ant.colony.pheromon); i++ {
		for j := 0; j < len(ant.colony.pheromon); j++ {
			if ant.colony.matrix[i][j] != 0 {
				f := false
				for k := 0; k < len(ant.path); k++ {
					src := ant.path[k]
					dst := ant.path[(k + 1) % len(ant.path)]
					if (src == i && dst == j) || (dst == i && src == j) {f = true}
				}
				if f {
					delta += ant.colony.params.q / float64(ant.colony.matrix[i][j])
				}
				
			}
			ant.colony.pheromon[i][j] = (1 - p) * (float64(ant.colony.pheromon[i][j]) + delta)
			if ant.colony.pheromon[i][j] <= 0 {
				ant.colony.pheromon[i][j] = 0.1
			}
		}
	}
}

func (ant *Ant_t) distance() int {
	distance := 0
	n := len(ant.path) 
	for i := 0; i < n; i++ {
		src := ant.path[i]
		dst := ant.path[(i + 1) % len(ant.path)]
		distance += ant.colony.matrix[src][dst]
	}
	return distance
}