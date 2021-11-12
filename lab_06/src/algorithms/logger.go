package algorithms

import (
	"fmt"
	"strings"
	"math/rand"
	"os"
)

//var map1 = [][]int{{0, 4, 2, 7, 3}, {4, 0, 6, 10, 9}, {2, 6, 0, 8, 5}, {7, 10, 8, 0, 1}, {3, 9, 5, 1, 0}}
//var map2 = [][]int{{0, 9, 8, 4, 10}, {6, 0, 4, 5, 7}, {5, 3, 0, 6, 2}, {1, 7, 2, 0, 5}, {2, 4, 5, 2, 0}}
//var map3 = [][]int{{0, 10, 5, 9, 11}, {10, 0, 11, 8, 12}, {5, 11, 0, 3, 9}, {9, 8, 3, 0, 12}, {11, 12, 9, 12, 0}}

var param_alpha = []float64{0, 1, 2, 3}
var param_p = []float64{0.2, 0.4, 0.6, 0.8}
var param_days = []int{2, 3, 4}

func Log() {
	f, _ := os.Create("Log.txt")
	
	params := new(Params_t)
	fmt.Println()
	fmt.Println(strings.Repeat("-", 64))
	fmt.Println("  №Map  |  Aplha  |   P   |  Days  |   L   |  L_best  |  Diff  |")
	map1 := generate_matrix(10)
	map2 := generate_matrix(10)
	map3 := generate_matrix(10)
	fmt.Println(strings.Repeat("-", 64))
	for i := 0; i < len(param_alpha); i++ {
		for j := 0; j < len(param_p); j++ {
			for k := 0; k < len(param_days); k++ {
				params.alpha = param_alpha[i]
				params.beta = 4 - params.alpha
				params.p = param_p[j]
				params.days = param_days[k]

				fmt.Printf("|%8v|%9v|%7v|%8v|", 1, params.alpha, params.p, params.days)
				res1, _ := BruteForce(map1)
				res2, _ := live(create_colony(map1, params))
				fmt.Printf("%6v|%10v|%8v|\n", res2, res1, res2 - res1)
				fmt.Println(strings.Repeat("-", 64))
				fmt.Fprintf(f, "%v & %v & %v & %v & %v & %v & %v\n", 1, params.alpha, params.p, params.days, res2, res1, res2 - res1)

				fmt.Printf("|%8v|%9v|%7v|%8v|", 2, params.alpha, params.p, params.days)
				res1, _ = BruteForce(map2)
				res2, _ = live(create_colony(map2, params))
				fmt.Printf("%6v|%10v|%8v|\n", res2, res1, res2 - res1)
				fmt.Fprintf(f, "%v & %v & %v & %v & %v & %v & %v\n", 2, params.alpha, params.p, params.days, res2, res1, res2 - res1)
				fmt.Println(strings.Repeat("-", 64))

				fmt.Printf("|%8v|%9v|%7v|%8v|", 3, params.alpha, params.p, params.days)
				res1, _ = BruteForce(map3)
				res2, _ = live(create_colony(map3, params))
				fmt.Printf("%6v|%10v|%8v|\n", res2, res1, res2 - res1)
				fmt.Fprintf(f, "%v & %v & %v & %v & %v & %v & %v\n", 3, params.alpha, params.p, params.days, res2, res1, res2 - res1)
				fmt.Println(strings.Repeat("-", 64))
			}
		}
	}
}

func max(x, y int) int {
    if x < y {
        return y
    }
    return x
}

func generate_matrix(n int) [][]int {
	a := make([][]int, n)
	for i := 0; i < n; i++ {
		a[i] = make([]int, n)
		for j := i + 1; j < n; j++ {
			a[i][j] = rand.Intn(n - 1) + 1
		}
	}
	for i := 0; i < n; i++ {
		for j := 0; j < i; j++ {
			a[i][j] = a[j][i]
		}
	}
	return a
}