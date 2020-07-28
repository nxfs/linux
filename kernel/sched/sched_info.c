/*
 * Copyright 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 */

#include "sched.h"

unsigned long sched_info_update;

/*
 * Update the time bucketed sched_info metrics once every SCHED_INFO_FREQ
 * period.
 *
 * Called from the global timer code.
 */
void calc_global_sched_info(void)
{
	unsigned long sample_window;
	struct task_struct *p, *t;

	sample_window = READ_ONCE(sched_info_update);
	if (time_before(jiffies, sample_window))
		return;

	rcu_read_lock();
	for_each_process_thread (p, t) {
		/*
		 * An atomic compare and exchange operation (atomic_cmpxchg) is
		 * used here to avoid a race between reading and resetting
		 * run_delay_max_current. If this race were allowed we may lose
		 * local maxima between reading the value and resetting it.
		 *
		 * Preemption is already disabled by the timer. This ensures the
		 * updating thread will not preempt this loop on the same
		 * hardware thread.
		 */
		do {
			t->sched_info.run_delay_max_1m = atomic64_read(
				&t->sched_info.run_delay_max_current);
		} while (atomic64_cmpxchg(&t->sched_info.run_delay_max_current,
					  t->sched_info.run_delay_max_1m,
					  0) != t->sched_info.run_delay_max_1m);
	}
	rcu_read_unlock();

	WRITE_ONCE(sched_info_update, sample_window + SCHED_INFO_FREQ);
}

