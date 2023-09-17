/**
 * @file Rd-01_recv.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-09-13
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef RD_01_RECV_H
#define RD_01_RECV_H

#define RD_01_IO 3

extern int rd_01_detected;
extern void* rd_01_nodet_time;

void Rd_01_recv_init(void);

#endif