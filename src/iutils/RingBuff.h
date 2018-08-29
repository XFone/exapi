/*
 * $Id: $
 *
 * A simplest circular(ring) buffer
 *
 * Copyright (c) 2004-2006 @Compant. All rights reserved.
 *
 * $Log: RingBuff.h,v $
 *
 */

#ifndef _UTILS_RINGBUFF_H_
#define _UTILS_RINGBUFF_H_

/** @file RingBuff.h A simplest circular(ring) buffer */

BEGIN_EXTERN_C

/**
 * Ring buffer
 */
typedef struct _ringbuf *p_ringbuf;

/**
 * Initialize ring buffer
 * @param capacity maximum count of items
 * @param sz size of item in bytes
 * @return pointer to ring-buffer instance (malloced)
 */
p_ringbuf rb_init(size_t capacity, size_t sz);

/**
 * Item count in ring buffer
 * @param rb ring-buffer instance
 * @return count
 */
size_t rb_count(p_ringbuf rb);

/**
 * Pushback a new item into ringbuffer
 * @param rb ring-buffer instance
 * @param item
 * return 0 for success, elsewise -ERRNO
 */
int rb_push_back(p_ringbuf rb, const void *item);

/**
 * Pop front from ringbuffer
 * @param rb ring-buffer instance
 * @param item
 * return 0 for success, elsewise -ERRNO
 */
int rb_pop_front(p_ringbuf rb, void *item);

END_EXTERN_C

#endif /* !_UTILS_RINGBUFF_H_ */
