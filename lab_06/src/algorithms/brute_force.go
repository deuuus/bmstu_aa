package algorithms

func BruteForce(a [][]int) (int, []int) {
	n := len(a)
	path := make([]int, n)
	for i := 0; i < n; i++ {
		path[i] = i + 1
	}

	min_len := dist(a, path)
	min_path := make([]int, n)

	cur_path, state := path, true 
	copy(min_path, cur_path)

	for ; state == true; {
		cur_len := dist(a, cur_path)
		if cur_len < min_len {
			min_len = cur_len
			copy(min_path, cur_path)
		}
		cur_path, state = have_next_permutation(cur_path)
	}
	return min_len, min_path
}

func have_next_permutation(a []int) ([]int, bool) {
	j := len(a) - 2
	for j != -1 && a[j] >= a[j+1] {
		j--
	}
	if j == -1 {
		return a, false
	}
	k := len(a) - 1
	for a[j] >= a[k] {
		k--
	}
	swap(a, j, k)
	l := j + 1
	r := len(a) - 1
	for l < r {
		swap(a, l, r)
		l++
		r--
	}
	return a, true
}

func swap(a []int, i int, j int) {
	s := a[i]
	a[i] = a[j]
	a[j] = s
}

func dist(a [][]int, path []int) (int) {
	dist := 0
	for k := 0; k < (len(path) - 1); k++ {
		i := path[k] - 1
		j := path[k + 1] - 1
		dist += a[i][j]
	}
	dist += a[path[0] - 1][path[len(path) - 1] - 1]
	return dist
}