package find

import "fmt"
import "os"

func BruteForce(dict Dict_t, key int) string {
	for i := 0; i < len(dict); i++ {
		if key - 1 == i {
			return dict[i]
		}
	} 
	return "ERROR:NOT_FOUND"
}

func LogBruteForce(dict Dict_t) {
	f, _ := os.Create("data/log_bf.txt")
	var cmps int
    for i := 0; i < len(dict); i += 6 {
		for j := 0; j < 6; j++ {
			cmps = CountCmpBruteForce(dict, i + j + 1)
			fmt.Fprintf(f, "%v %v\n", i + j + 1, cmps)
		}
	}
	f.Close()
}

func CountCmpBruteForce(dict Dict_t, key int) int {
	for i := 0; i < len(dict); i++ {
		if key - 1 == i {
			return i
		}
	} 
	return len(dict)
}