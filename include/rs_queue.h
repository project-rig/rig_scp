/**
 * An implementation of a growable Rig SCP request/response FIFO queue.
 */

#ifndef RS_QUEUE_H
#define RS_QUEUE_H

#include <stdint.h>
#include <stdbool.h>
#include <uv.h>

#include <rs_scp.h>

/**
 * Types of requests/responses.
 */
typedef enum {
	// A single SCP packet/response
	RS__Q_PACKET,
	
	// A bulk read request/response
	RS__Q_READ,
	
	// A bulk write request/response
	RS__Q_WRITE,
} rs__q_type_t;


/**
 * An entry in a queue.
 */
struct rs__q_entry;
typedef struct rs__q_entry rs__q_entry_t;
struct rs__q_entry {
	// The type of packet
	rs__q_type_t type;
	
	// A unique identifier to be used to match requests/responses
	unsigned int id;
	
	// The request data
	union {
		// type == RS__Q_PACKET: An SCP packet
		struct {
			// Command/return code
			uint16_t cmd_rc;
			
			// Sequence number
			uint16_t seq;
			
			// Arguments
			uint32_t arg1;
			uint32_t arg2;
			uint32_t arg3;
			
			// Number of arguments to be sent/received. If less than 3, the space
			// usually used by the arguments will be given over to carry data.
			unsigned int n_args_send;
			unsigned int n_args_recv;
			
			// Data payload (allocated separately)
			uv_buf_t data;
		} packet;
		
		// type == RS__Q_READ || type == RS__Q_WRITE: Bulk read/write
		struct {
			// The address to read/write the data to in the machine
			uint32_t addr;
			
			// The buffer out-of/in-to which data will be read/written. Also gives the
			// size of the data block to transfer.
			uv_buf_t data;
			
			// Offset past the data already read/written
			unsigned int offset;
		} rw;
	} data;
	
	// Is this entry empty?
	bool empty;
	
	// Pointers forming a linked-list between entries in the queue.
	rs__q_entry_t *next;
};


/**
 * Data type which represents the queue.
 */
struct rs__q;
typedef struct rs__q rs__q_t;


/**
 * Allocate a new queue in memory.
 *
 * Returns a pointer to a newly allocated queue structure or NULL on failure.
 * This structure must be freed using rs__q_free.
 */
rs__q_t *rs__q_init(void);


/**
 * Attempt to insert an entry into the queue.
 *
 * Returns a pointer to an entry in the queue. The user *must* initialise the
 * entry before the next queue operation commences.
 *
 * Returns NULL on failure.
 */
rs__q_entry_t *rs__q_insert(rs__q_t *q);


/**
 * Attempt to remove an entry into the queue.
 *
 * Returns a pointer to an entry in the queue. The user *must* copy any data out
 * of the queue entry before the next queue operation commences.
 *
 * Returns NULL if the queue is empty.
 */
rs__q_entry_t *rs__q_remove(rs__q_t *q);


/**
 * Get a pointer to the tail of the queue or NULL of empty.
 */
rs__q_entry_t *rs__q_peek(rs__q_t *q);


/**
 * Free all memory associated with a queue.
 */
void rs__q_free(rs__q_t *q);

#endif
