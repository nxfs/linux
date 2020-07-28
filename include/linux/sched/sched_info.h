/*
 * Copyright 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 */

#ifndef _LINUX_SCHED_SCHED_INFO_H
#define _LINUX_SCHED_SCHED_INFO_H

#define SCHED_INFO_FREQ (60 * HZ + 1) /* 60 sec intervals */

#ifdef CONFIG_SCHED_INFO
extern unsigned long sched_info_update;

void calc_global_sched_info(void);
#endif

#endif /* _LINUX_SCHED_SCHED_INFO_H */

