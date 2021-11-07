package pipeline

func CreateQueue(n int) *Queue_t {
	q := new(Queue_t)
	q.Q = make([](*Task_t), n, n)
	q.Size = -1
	return q
}

func (q *Queue_t) push(item *Task_t){
	if q.Size != len(q.Q) - 1 {
		q.Q[q.Size + 1] = item
		q.Size++
	}
}

func (q *Queue_t) pop() *Task_t {
	item := q.Q[0]
	q.Q = q.Q[1:]
	q.Size--

	return item
}