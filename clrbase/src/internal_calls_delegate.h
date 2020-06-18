/*
 * Copyright (C) 2008  Campanoni Simone
 *
 * iljit - This is a Just-in-time for the CIL language specified with the ECMA-335
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
#ifndef INTERNAL_CALLS_DELEGATE_H
#define INTERNAL_CALLS_DELEGATE_H

#include <jitsystem.h>
#include <ilmethod.h>

void * System_Delegate_CreateBlankDelegate (void *type, void *method);

/**
 * Fix output parameters
 *
 * @param delegate the delegate who has performed the call
 * @param args the call arguments
 * @param outParams the output parameters to set
 */
void System_Runtime_Remoting_Messaging_AsyncResult_SetOutParams (void* delegate,
        void* args,
        void* outParams
                                                                );

#endif /* INTERNAL_CALLS_DELEGATE_H */