package pipeline

import (
	"fmt"
	"strings"
	"time"
	"os"
)

func Log(q_pipeline *Queue_t, q_sync *Queue_t) {

	f, _ := os.Create("Log.txt")

	fmt.Println(strings.Repeat("-", 94))
	fmt.Printf("|%62v%s|\n", "Время начала обработки заявки, ns", strings.Repeat(" ", 30))
	fmt.Println(strings.Repeat("-", 94))
	fmt.Println("|        |         Конвейерная обработка           |        Последовательная обработка       |")
	fmt.Printf("%94v\n", strings.Repeat("-", 94))
	fmt.Println("|№ заявки|   Линия 1   |   Линия 2   |   Линия 3   |   Линия 1   |   Линия 2   |   Линия 3   |")
	fmt.Printf("%94v\n", strings.Repeat("-", 94))

	p_start := q_pipeline.Q[0].start_move
	s_start := q_sync.Q[0].start_move

	for i := 0; i <= q_pipeline.Size; i++ {
		fmt.Printf("|%8d", i + 1)
		fmt.Printf("|%13v", q_pipeline.Q[i].start_move.Sub(p_start).Nanoseconds())
		fmt.Printf("|%13v", q_pipeline.Q[i].start_rotate.Sub(p_start).Nanoseconds())
		fmt.Printf("|%13v", q_pipeline.Q[i].start_scale.Sub(p_start).Nanoseconds())
		fmt.Printf("|%13v", q_sync.Q[i].start_move.Sub(s_start).Nanoseconds())
		fmt.Printf("|%13v", q_sync.Q[i].start_rotate.Sub(s_start).Nanoseconds())
		fmt.Printf("|%13v|\n", q_sync.Q[i].start_scale.Sub(s_start).Nanoseconds())
		fmt.Printf("%94v\n", strings.Repeat("-", 94))

		fmt.Fprintf(f, "%d&", i + 1)
		fmt.Fprintf(f, "%v&", q_pipeline.Q[i].start_move.Sub(p_start).Nanoseconds())
		fmt.Fprintf(f, "%v&", q_pipeline.Q[i].start_rotate.Sub(p_start).Nanoseconds())
		fmt.Fprintf(f, "%v&", q_pipeline.Q[i].start_scale.Sub(p_start).Nanoseconds())
		fmt.Fprintf(f, "%v&", q_sync.Q[i].start_move.Sub(s_start).Nanoseconds())
		fmt.Fprintf(f, "%v&", q_sync.Q[i].start_rotate.Sub(s_start).Nanoseconds())
		fmt.Fprintf(f, "%v\n", q_sync.Q[i].start_scale.Sub(s_start).Nanoseconds())
	}

	p_min_time := q_pipeline.Q[0].end_scale.Sub(q_pipeline.Q[0].start_move)
	p_max_time := p_min_time
	p_avg_time := p_min_time

	s_min_time := q_sync.Q[0].end_scale.Sub(q_sync.Q[0].start_move)
	s_max_time := s_min_time
	s_avg_time := s_min_time

	for i := 0; i <= q_pipeline.Size; i++ {
		t := q_pipeline.Q[i].end_scale.Sub(q_pipeline.Q[i].start_move)
		if t < p_min_time {
			p_min_time = t
		}
		if t > p_max_time {
			p_max_time = t
		}
		p_avg_time += t

		t = q_sync.Q[i].end_scale.Sub(q_sync.Q[i].start_move)
		if t < s_min_time {
			s_min_time = t
		}
		if t > p_max_time {
			s_max_time = t
		}
		s_avg_time += t
	}

	fmt.Println()
	fmt.Println(strings.Repeat("-", 61))
	fmt.Printf("|%50v%s|\n","Время нахождения заявки в системе, ns", strings.Repeat(" ", 9))
	fmt.Println(strings.Repeat("-", 61))
	fmt.Println("|   |  Конвейерная обработка  | Последовательная обработка  |")
	fmt.Println(strings.Repeat("-", 61))
	fmt.Printf("|Min|%25v|%29v|\n", p_min_time.Nanoseconds(), s_min_time.Nanoseconds())
	fmt.Println(strings.Repeat("-", 61))
	fmt.Printf("|Max|%25v|%29v|\n", p_max_time.Nanoseconds(), s_max_time.Nanoseconds())
	fmt.Println(strings.Repeat("-", 61))
	fmt.Printf("|Avg|%25v|%29v|\n", time.Duration((int(p_avg_time)) / (q_pipeline.Size + 1)).Nanoseconds(),
									time.Duration((int(s_avg_time)) / (q_sync.Size + 1)).Nanoseconds())
	fmt.Println(strings.Repeat("-", 61))

	fmt.Fprintf(f, "\n")
	fmt.Fprintf(f, "%v&%v\n", p_min_time.Nanoseconds(), s_min_time.Nanoseconds())
	fmt.Fprintf(f, "%v&%v\n", p_max_time.Nanoseconds(), s_max_time.Nanoseconds())
	fmt.Fprintf(f, "%v&%v\n", time.Duration((int(p_avg_time)) / (q_pipeline.Size + 1)).Nanoseconds(),
									time.Duration((int(s_avg_time)) / (q_sync.Size + 1)).Nanoseconds())


	p_min_diff := q_pipeline.Q[0].start_rotate.Sub(q_pipeline.Q[0].end_move) +
				  q_pipeline.Q[0].start_scale.Sub(q_pipeline.Q[0].end_rotate)
	p_max_diff := p_min_diff
	p_avg_diff := p_min_diff

	s_min_diff := q_sync.Q[0].start_rotate.Sub(q_sync.Q[0].end_move) +
					q_sync.Q[0].start_scale.Sub(q_sync.Q[0].end_rotate)
	s_max_diff := s_min_diff
	s_avg_diff := s_min_diff

	for i := 0; i <= q_pipeline.Size; i++ {
		diff := q_pipeline.Q[i].start_rotate.Sub(q_pipeline.Q[i].end_move) +
			q_pipeline.Q[i].start_scale.Sub(q_pipeline.Q[i].end_rotate)
		if diff < p_min_time {
			p_min_diff = diff
		}
		if diff > p_max_diff {
			p_max_diff = diff
		}
		p_avg_time += diff

		diff = q_sync.Q[i].start_rotate.Sub(q_sync.Q[i].end_move) +
				q_sync.Q[i].start_scale.Sub(q_sync.Q[i].end_rotate)
		if diff < s_min_time {
			s_min_time = diff
		}
		if diff > p_max_time {
			s_max_time = diff
		}
		s_avg_time += diff
	}

	fmt.Println()
	fmt.Println(strings.Repeat("-", 61))
	fmt.Printf("|%50v%s|\n","Время простоя заявки в системе, ns", strings.Repeat(" ", 9))
	fmt.Println(strings.Repeat("-", 61))
	fmt.Println("|   |  Конвейерная обработка  | Последовательная обработка  |")
	fmt.Println(strings.Repeat("-", 61))
	fmt.Printf("|Min|%25v|%29v|\n", p_min_diff.Nanoseconds(), s_min_diff.Nanoseconds())
	fmt.Println(strings.Repeat("-", 61))
	fmt.Printf("|Max|%25v|%29v|\n", p_max_diff.Nanoseconds(), s_max_diff.Nanoseconds())
	fmt.Println(strings.Repeat("-", 61))
	fmt.Printf("|Avg|%25v|%29v|\n", time.Duration((int(p_avg_diff)) / (q_pipeline.Size + 1)).Nanoseconds(),
									time.Duration((int(s_avg_diff)) / (q_sync.Size + 1)).Nanoseconds())
	fmt.Println(strings.Repeat("-", 61))

	fmt.Fprint(f, "\n")
	fmt.Fprintf(f, "%v&%v\n", p_min_diff.Nanoseconds(), s_min_diff.Nanoseconds())
	fmt.Fprintf(f, "%v&%v\n", p_max_diff.Nanoseconds(), s_max_diff.Nanoseconds())
	fmt.Fprintf(f, "%v&%v\n", time.Duration((int(p_avg_diff)) / (q_pipeline.Size + 1)).Nanoseconds(),
									time.Duration((int(s_avg_diff)) / (q_sync.Size + 1)).Nanoseconds())

	defer f.Close()
}