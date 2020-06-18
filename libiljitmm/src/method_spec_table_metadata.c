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
/**
   *@file method_spec_table_metadata.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <jitsystem.h>
#include <compiler_memory_manager.h>

// My headers
#include <metadata/tables/metadata_table_manager.h>
#include <mtm_utils.h>
#include <iljitmm-system.h>
// End

JITINT32 load_method_spec_table (t_method_spec_table *table, void *binary, JITUINT64 valid, JITUINT16 heap_size, JITINT8 *row_buf, JITINT8 *table_buf, JITUINT32 *bytes) {
    JITUINT32 cardinality;
    JITUINT32 bytes_read;

    /* Assertions	*/
    assert(bytes != 0);
    assert(table != 0);
    if ( ((valid >> METHOD_SPEC_TABLE) & 0x1 ) == 0) {
        *bytes = 0;
        return 0;
    }
    assert(table->cardinality != 0);

    /* Create the table	*/
    PDEBUG("TABLE MANAGER:	Method spec table\n");
    bytes_read      = 0;
    table->table    = (t_row_method_spec_table **) sharedAllocFunction(sizeof(t_row_method_spec_table *) * (table->cardinality));
    assert(table->table != NULL);

    /* Fill up the table	*/
    cardinality = table->cardinality;
    for (cardinality = 0; cardinality < (table->cardinality); cardinality++) {
        t_row_method_spec_table *temp_row;
        temp_row                = (t_row_method_spec_table *) sharedAllocFunction(sizeof(t_row_method_spec_table));
        temp_row->index         = cardinality + 1;                // The index of the table start at one value
        temp_row->binary = binary;
        if (private_is_method_def_or_ref_token_32()) {
            /* Index MethodDefOrRef is 32 bit		*/
            read_from_buffer(table_buf, bytes_read, 4, &(temp_row->method));
            bytes_read += 4;
        } else {
            /* Index MethodDefOrRef is 16 bit		*/
            read_from_buffer(table_buf, bytes_read, 2, &(temp_row->method));
            bytes_read += 2;
        }
        if (blob_index_is_32(heap_size)) {
            // Index of blob stream is 32 bit
            read_from_buffer(table_buf, bytes_read, 4, &(temp_row->instantiation));
            bytes_read += 4;
        } else {
            // Index of blob stream is 16 bit
            read_from_buffer(table_buf, bytes_read, 2, &(temp_row->instantiation));
            bytes_read += 2;
        }
        table->table[cardinality] = temp_row;
    }

#ifdef PRINTDEBUG
    JITINT32 count;
    PDEBUG("			method SPEC TABLE		Cardinality = %d\n", cardinality);
    PDEBUG("Index		Method		Instantiation\n");
    PDEBUG("						(Blob index)	\n");
    for (count = 0; count < cardinality; count++) {
        t_row_method_spec_table *temp;
        temp = table->table[count];
        assert(temp != NULL);
        PDEBUG("%d		0x%X	%d\n", (count + 1), temp->method, temp->instantiation);
    }
    PDEBUG("\n");
    PDEBUG("TABLE MANAGER:		Bytes read = %d\n", bytes_read);
#endif

    (*bytes) = bytes_read;
    return 0;
}

void shutdown_method_spec_table (t_method_spec_table *table) {
    JITINT32 count;
    JITINT32 size;

    /* Assertions	*/
    assert(table != NULL);

    if (table->table == NULL) {
        return;
    }

    assert(table->table != NULL);
    PDEBUG("TABLE MANAGER:		Method spec table shutting down\n");
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
