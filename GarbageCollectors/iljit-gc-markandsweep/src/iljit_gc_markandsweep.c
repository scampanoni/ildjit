/*
 * Copyright (C) 2006  Campanoni Simone
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// My header
#include <iljit_gc_markandsweep.h>
#include <config.h>
// End

#define MARKANDSWEEP_DEFAULT_HEAP_SIZE	80000 * 1024

t_garbage_collector_plugin plugin_interface={
	init				,
	gc_shutdown			,
	markandsweep_getSupport		,
	gc_allocObject			,
	collect				,
	markandsweep_threadCreate	,
	gcInformations			,
	markandsweep_getName		,
	markandsweep_getVersion		,
	markandsweep_getAuthor
};

JITINT16 init (t_gc_behavior *gcBehavior, JITNUINT sizeMemory){
	JITINT16 	error;
	JITNUINT	correctSize;
	JITNINT		pageSize;
	JITNUINT	numPages;
	
	/* Assertions			*/
	assert(gcBehavior != NULL);
	PDEBUG("GC MarkAndSweep: init: Start\n");

	/* Store the behavior		*/
	memcpy(&behavior, gcBehavior, sizeof(t_gc_behavior));

	/* Initialize the mutex of the	*
	 * GC				*/
	sem_init(&gcMutex, 0, 1);

	/* Compute the correct size to 	*
	 * be a multiply of the page	*
	 * size				*/
	PDEBUG("GC MarkAndSweep: init: 	Request heap size	= %u\n", (JITUINT32)sizeMemory);
	pageSize	= getpagesize();
	assert(pageSize > 0);
	PDEBUG("GC MarkAndSweep: init: 	Memory page size	= %u\n", (JITUINT32) pageSize);
	if (sizeMemory == 0) {
		sizeMemory	= MARKANDSWEEP_DEFAULT_HEAP_SIZE;
	}
	if ((sizeMemory % pageSize) == 0){
		correctSize	= sizeMemory;
	} else {
		numPages	= sizeMemory / pageSize;
		numPages++;
		correctSize	= numPages * pageSize;
	}
	PDEBUG("GC MarkAndSweep: init: 	Heap size allocated	= %u Bytes\n", (JITUINT32)correctSize);

	/* Initialize the memory heap	*/
	error	= init_memory(&memory, correctSize);

	/* Initialize the time 		*
	 * informations			*/
	gcTime.collectTime		= 0;
	gcTime.allocTime		= 0;

	/* Return			*/
	PDEBUG("GC MarkAndSweep: init: Exit\n");
	return error;
}

JITINT16 gc_shutdown (void){
	JITINT16	error;
	JITINT16	globalError;

	/* Assertions			*/
	PDEBUG("GC MarkAndSweep: Shutdown: Start\n");

	/* Initialize the variables	*/
	globalError	= 0;
	error		= 0;

	/* Shutdown the memory		*/
	PDEBUG("GC MarkAndSweep: Shutdown: 	Free the raw memory\n");
	error	= memory.shutdown(&memory, &behavior);
	if (error != 0) globalError = error;

	/* Return			*/
	PDEBUG("GC MarkAndSweep: Shutdown: Exit\n");
	return globalError;
}

void * gc_allocObject (JITNUINT size){
	void			*free_memory;
	struct timespec		startTime;
	struct timespec		stopTime;

	/* Assertions			*/
	assert(size > 0);

	/* Begin the critical zone	*/
	sem_wait(&gcMutex);
	PDEBUG("GC MarkAndSweep: allocObject: Start\n");
	
	/* Fetch the start time		*/
	if (behavior.profile){
		clock_gettime(CLOCK_REALTIME, &startTime);
	}

	/* Initialize the variable	*/
	free_memory		= NULL;
	
	/* Print the layout of the object	*/
	PDEBUG("GC MarkAndSweep: allocObject:	Object size	 	= %u Bytes\n", (JITUINT32)size);

	/* Search a memory block		*/
	free_memory	= memory.fetchFreeMemory(&memory, size, &behavior);

	/* The free_memory pointer, points to 	*
	 * the first byte of the memory block	*/
	if (free_memory != NULL){

		/* Initialize the fields of the object	*/
		PDEBUG("GC MarkAndSweep: allocObject:	Object 	 	=	 %p\n", free_memory);
		memset(((void *)free_memory), 0, size);
	
		/* Add the reference of the new object	*
		 * to the list of the GC		*/
		memory.addObjectReference(&memory, free_memory + HEADER_FIXED_SIZE);
	} else {
		PDEBUG("GC MarkAndSweep: allocObject:	Memory full\n");
	}

	/* Fetch the stop time		*/
	if (behavior.profile){
		clock_gettime(CLOCK_REALTIME, &stopTime);
	
		/* Store profiling informations		*/
		gcTime.allocTime	+= diff_time(startTime, stopTime);
	}

	/* End the critical zone	*/
	sem_post(&gcMutex);

	/* Return the new object	*/
	PDEBUG("GC MarkAndSweep: allocObject: Exit\n");
	return free_memory;
}

void collect (void){
	PDEBUG("GC MarkAndSweep: collect: Start\n");
	
	/* Begin the critical zone	*/
	sem_wait(&gcMutex);

	/* Collect the garbage memory	*/
	PDEBUG("GC MarkAndSweep: collect: 	Run the collection\n");
	memory.collect(&memory, &behavior);

	/* End the critical zone	*/
	sem_post(&gcMutex);

	/* Return			*/
	PDEBUG("GC MarkAndSweep: collect: Exit\n");
}

void gcInformations (t_gc_informations *gcInfo){

	/* Assertions			*/
	assert(gcInfo != NULL);

	/* Store the heap informations	*/
	gcInfo->maxHeapMemory		= memory.maxHeap;
	gcInfo->actualHeapMemory	= ((JITUINT32)(memory.top - memory.bottom)) - memory.freeMemorySize(&memory);
	if (gcInfo->actualHeapMemory > gcInfo->maxHeapMemory){
		gcInfo->maxHeapMemory	= gcInfo->actualHeapMemory;
	}
	gcInfo->heapMemoryAllocated	= (JITUINT32) (memory.top - memory.bottom);

	/* Store the memory over head	*
	 * informations			*/
	gcInfo->overHead		= memory.objectsReference.size;
	gcInfo->overHead		+= memory.memoryBlocks.size;
	if (memory.objectsReference.top > memory.objectsReference.max){
		gcInfo->overHead	+= memory.objectsReference.top * sizeof(JITNUINT);
	} else {
		gcInfo->overHead	+= memory.objectsReference.max * sizeof(JITNUINT);
	}
	if (memory.memoryBlocks.top > memory.memoryBlocks.max){
		gcInfo->overHead	+= memory.memoryBlocks.top * sizeof(t_memory_block);
	} else {
		gcInfo->overHead	+= memory.memoryBlocks.max * sizeof(t_memory_block);
	}
		
	/* Store the time informations	*/
	gcInfo->totalCollectTime	= gcTime.collectTime;
	gcInfo->totalAllocTime		= gcTime.allocTime;

	/* Return			*/
	return;
}

void markandsweep_threadCreate (pthread_t *thread, pthread_attr_t *attr, void *(*startRouting)(void *), void *arg){
	abort();
}

char * markandsweep_getName (void){
	return "MarkAndSweep";
}

char * markandsweep_getVersion (void){
	return VERSION;
}

char * markandsweep_getAuthor (void){
	return "Simone Campanoni";
}

JITINT32 markandsweep_getSupport (void){
	return ILDJIT_GCSUPPORT_ROOTSET;
}
