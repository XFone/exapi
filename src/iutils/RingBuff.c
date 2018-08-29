/*
 * $Id: $
 *
 * Implementation: a simplest circular(ring) buffer
 *
 * Copyright (c) 2004-2006 @Compant. All rights reserved.
 *
 * $Log: RingBuff.c,v $
 *
 */

#include "Base.h"
#include "Log.h"
#include "RingBuff.h"

#include <string.h>

typedef struct _ringbuf {
    void  *buffer;            // data buffer
    void  *buffer_end;        // end of data buffer
    size_t capacity;          // maximum number of items in the buffer
    size_t count;             // number of items in the buffer
    size_t sz;                // size of each item in the buffer
    void  *head;              // pointer to head
    void  *tail;              // pointer to tail
} ringbuf_t;

p_ringbuf rb_init(size_t capacity, size_t sz)
{
    ringbuf_t *rb;
    
    if (NULL == (rb = malloc(sizeof(*rb) + capacity * sz))) {        
        // LOGFILE(LOG_ERROR, "No enough memory in %s", __FUNCTION__);
        assert(0);
    }
    
    rb->buffer = rb + 1;
    rb->buffer_end = (char *)rb->buffer + capacity * sz;
    rb->capacity = capacity;
    rb->count = 0;
    rb->sz = sz;
    rb->head = rb->buffer;
    rb->tail = rb->buffer;

    LOGFILE(LOG_DEBUG, "initialized ring buffer %d x %d",
            capacity, sz);
    return rb;
}

size_t rb_count(p_ringbuf rb)
{
    return rb->count;
}

int rb_push_back(p_ringbuf rb, const void *item)
{
    if(rb->count == rb->capacity) {
        // handle error
        LOGFILE(LOG_WARN, "ring buffer full");
        return -EDQUOT; // Quota exceeded
    }
    
    memcpy(rb->head, item, rb->sz);
    rb->head = (char*)rb->head + rb->sz;
    if(rb->head == rb->buffer_end) {        
        rb->head = rb->buffer;
    }    

    rb->count++;
    return 0;
}

int rb_pop_front(p_ringbuf rb, void *item)
{
    if(rb->count == 0) {        
        // handle error
        // LOGFILE(LOG_WARN, "ring buffer empty");
        return -ENOSR; // Out of streams resources
    }

    memcpy(item, rb->tail, rb->sz);
    rb->tail = (char*)rb->tail + rb->sz;
    if(rb->tail == rb->buffer_end) {        
        rb->tail = rb->buffer;
    }
    
    rb->count--;
    return 0;
}
