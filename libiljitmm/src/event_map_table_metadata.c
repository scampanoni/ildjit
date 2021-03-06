/*
 * Copyright (C) 2009  Campanoni Simone, Michele Tartara
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
/**
   *@file event_map_table_metadata.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <compiler_memory_manager.h>

// My headers
#include <metadata/tables/metadata_table_manager.h>
#include <jitsystem.h>
#include <mtm_utils.h>
#include <iljitmm-system.h>
// End

JITINT32 load_event_map_table (t_event_map_table *table, void *binary, JITUINT64 valid, JITUINT16 heap_size, JITINT8 *row_buf, JITINT8 *table_buf, JITUINT32 *bytes) {
    JITUINT32 cardinality;
    JITUINT32 bytes_read;

    /* Assertions	*/
    assert(bytes != 0);
    assert(table != NULL);
    assert(row_buf != NULL);
    if ( ((valid >> EVENT_MAP_TABLE) & 0x1 ) == 0) {
        *bytes = 0;
        return 0;
    }
    assert(table->cardinality != 0);

    /* Create the table	*/
    PDEBUG("TABLE MANAGER:	Event map table\n");
    bytes_read      = 0;
    table->table    = (t_row_event_map_table **) sharedAllocFunction(sizeof(t_row_event_map_table *) * (table->cardinality));
    assert(table->table != NULL);

    /* Fill up the table	*/
    for (cardinality = 0; cardinality < (table->cardinality); cardinality++) {
        t_row_event_map_table *temp_row;
        temp_row                        = (t_row_event_map_table *) sharedAllocFunction(sizeof(t_row_event_map_table));
        temp_row->index                 = cardinality + 1;        //Index with initial value equal to one
        temp_row->binary = binary;
        if (    (((t_type_def_table *) private_get_table(NULL, TYPE_DEF_TABLE))->cardinality > METADATA_TABLE_LIMIT_16_BITS_ENCODING)) {
            // Index of TypeDef is 32 bit
            read_from_buffer(table_buf, bytes_read, 4, &(temp_row->parent));
            bytes_read += 4;
        } else {
            // Index of TypeDef is 16 bit
            read_from_buffer(table_buf, bytes_read, 2, &(temp_row->parent));
            bytes_read += 2;
        }
        if (    (((t_event_table *) private_get_table(NULL, EVENT_TABLE))->cardinality > METADATA_TABLE_LIMIT_16_BITS_ENCODING)) {
            // Index of Event is 32 bit
            read_from_buffer(table_buf, bytes_read, 4, &(temp_row->event_list));
            bytes_read += 4;
        } else {
            // Index of Event is 16 bit
            read_from_buffer(table_buf, bytes_read, 2, &(temp_row->event_list));
            bytes_read += 2;
        }
        table->table[cardinality] = temp_row;
    }

#ifdef PRINTDEBUG
    JITINT32 count;
    PDEBUG("			EVENT MAP TABLE	Cardinality = %d\n", cardinality);
    PDEBUG("Index		Parent		Event list\n");
    for (count = 0; count < cardinality; count++) {
        t_row_event_map_table *temp;
        temp = table->table[count];
        assert(temp != NULL);
        PDEBUG("%d		%d		%d\n", (count + 1), temp->parent, temp->event_list);
    }
    PDEBUG("\n");
    PDEBUG("TABLE MANAGER:		Bytes read = %d\n", bytes_read);
#endif

    (*bytes) = bytes_read;
    return 0;
}

void shutdown_event_map_table (t_event_map_table *table) {
    JITINT32 count;
    JITINT32 size;

    /* Assertions		*/
    assert(table != NULL);

    if (table->table == NULL) {
        return;
    }

    PDEBUG("TABLE MANAGER:		Event map shutting down\n");
    size = table->cardinality;
    PDEBUG("TABLE MANAGER:			Rows=%d\n", size);

    /* Delete the rows	*/
    for (count = 0; count < size; count++) {
        freeFunction(table->table[count]);
    }

    /* Delete the table	*/
    freeFunction(table->table);
    PDEBUG("TABLE MANAGER:			Table deleted\n");
}
