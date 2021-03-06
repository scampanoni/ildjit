/*
 * Copyright (C) 2009-2010  Mastrandrea Daniele
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
 * @file whilifier.h
 * @brief ILDJIT Optimizer: do-while to while loops transformer
 * @author Mastrandrea Daniele
 * @version 0.1.0
 * @date 2009-2010
**/

#ifndef OPTIMIZER_DOWHILE_TO_WHILE_H
#define OPTIMIZER_DOWHILE_TO_WHILE_H

#include "implementation.h"

/**
 * @brief Stucture t_loop_extended.
 *
 * This structure encloses some info for each loop in the IR method.
 * It extends the structur t_loop defined by ILDJIT.
**/
typedef struct {
	JITNUINT 		loop_type;
	t_loop			* loop;
	t_loop			* parent_loop;
	XanList			* loop_instructions;
	XanList 		* loop_exits;
	XanList 		* loop_back_edges;
	t_ir_instruction 	* BB_cond_inst;
	t_ir_instruction 	* loop_head;
	t_ir_instruction 	* loop_cond;
} t_loop_extended;

/**
 * @defgroup Loops
 * Loop types
**/

/**
* @def LOOP_CATCHER
* @ingroup Loops
* @brief Loop generated by a catcher routine.
**/
#define LOOP_CATCHER		1

/**
 * @def LOOP_DOWHILE
 * @ingroup Loops
 * @brief Regular do-while loop.
**/
#define LOOP_DOWHILE		2

/**
 * @def LOOP_DOWHILE_JUMPS
 * @ingroup Loops
 * @brief Do-while loop containing one jump.
**/
#define LOOP_DOWHILE_JUMP	3

/**
 * @def LOOP_JUMPS
 * @ingroup Loops
 * @brief Generic loop containing more than one jumps.
**/
#define LOOP_JUMPS		4

/**
 * @def LOOP_MALFORMED
 * @ingroup Loops
 * @brief Generic loop whose loop exits or back edges are not branchif(not).
**/
#define LOOP_MALFORMED		5

/**
 * @def LOOP_NESTED
 * @ingroup Loops
 * @brief Generic nested loop whose parent have the same header.
**/
#define LOOP_NESTED		6

/**
 * @def LOOP_ODD
 * @ingroup Loops
 * @brief Generic loop containing more than one consecutive back edges and loop exits.
**/
#define LOOP_ODD		7

/**
 * @def LOOP_ONE_BLOCK
 * @ingroup Loops
 * @brief Generic loop made of only one loop (most likely it is a do-while one).
**/
#define LOOP_ONE_BLOCK		8

/**
 * @def LOOP_UNKNOWN
 * @ingroup Loops
 * @brief Generic loop.
**/
#define LOOP_UNKNOWN		9

/**
 * @def LOOP_UNSUPPORTED
 * @ingroup Loops
 * @brief Generic unsupported loop.
**/
#define LOOP_UNSUPPORTED	10

/**
 * @def LOOP_WRONG
 * @ingroup Loops
 * @brief Generic loop whose identification went wrong somewhere.
**/
#define LOOP_WRONG		11

/**
 * @def LOOP_WHILE
 * @ingroup Loops
 * @brief Generic while (or for) loop.
**/
#define LOOP_WHILE		12

#ifdef SUPERDEBUG
#define SDEBUG(fmt, args...) pretty_fprintf(pretty_printer, stderr, fmt, ## args)
#else
#define SDEBUG(fmt, args...)
#endif /* SUPERDEBUG */

#ifdef SUPERDEBUG
#ifndef PRINTDEBUG
#define PRINTDEBUG
#endif /* PRINTDEBUG */
#endif /* SUPERDEBUG */

#ifdef PRINTDEBUG
#define PDEBUG(fmt, args...) pretty_fprintf(pretty_printer, stderr, fmt, ## args)
#define PP_INIT() pretty_init(pretty_printer)
#define PP_INDENT() pretty_indent(pretty_printer)
#define PP_UNINDENT() pretty_unindent(pretty_printer)
#define PP_MARK() pretty_mark(pretty_printer)
#define PP_UNMARK() pretty_unmark(pretty_printer)
#else
#define PDEBUG(fmt, args...)
#define PP_INDENT()
#define PP_UNINDENT()
#define PP_MARK()
#define PP_UNMARK()
#endif /* PRINTDEBUG */

#endif /* OPTIMIZER_DOWHILE_TO_WHILE_H */
