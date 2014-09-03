/*
Copyright (c) 2014, Yasuhiro Hasegawa
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/


//
// Debug.h
//

#ifndef DEBUG_H__
#define DEBUG_H__

static const int DEBUG_SHOW_EVENT_KEY       = 0;
static const int DEBUG_SHOW_EVENT_MOUSE     = 0;
static const int DEBUG_SHOW_RUNNING         = 0;

// Show opcode that has dummy implementation
static const int DEBUG_SHOW_DUMMY_IMPL_OPCODE = 0;

// Fault
static const int DEBUG_SHOW_FRAME_FAULT         = 0;
static const int DEBUG_SHOW_PAGE_FAULT          = 0;
static const int DEBUG_SHOW_WRITE_PROTECT_FAULT = 1;

// Trap
static const int DEBUG_SHOW_BOUNDS_TRAP     = 0;
static const int DEBUG_SHOW_BREAK_TRAP      = 0;
static const int DEBUG_SHOW_CODE_TRAP       = 0;
static const int DEBUG_SHOW_CONTROL_TRAP    = 0;
static const int DEBUG_SHOW_DIV_CHECK_TRAP  = 1;
static const int DEBUG_SHOW_DIV_ZERO_TRAP   = 1;
static const int DEBUG_SHOW_ESC_OPCODE_TRAP = 0;
static const int DEBUG_SHOW_INTERRUPT_ERROR = 1;
static const int DEBUG_SHOW_OPCODE_TRAP     = 0;
static const int DEBUG_SHOW_POINTER_TRAP    = 1;
static const int DEBUG_SHOW_PROCESS_TRAP    = 1;
static const int DEBUG_SHOW_RESCHEDULE_TRAP = 1;
static const int DEBUG_SHOW_STACK_ERROR     = 1;
static const int DEBUG_SHOW_UNBOUND_TRAP    = 1;
static const int DEBUG_SHOW_HARDWARE_ERROR  = 1;
static const int DEBUG_SHOW_XFER_TRAP       = 1;

// Agent
static const int DEBUG_SHOW_AGENT_BEEP      = 0;
static const int DEBUG_SHOW_AGENT_DISK      = 0;
static const int DEBUG_SHOW_AGENT_DISPLAY   = 0;
static const int DEBUG_SHOW_AGENT_FLOPPY    = 0;
static const int DEBUG_SHOW_AGENT_MOUSE     = 0;
static const int DEBUG_SHOW_AGENT_NETWORK   = 0;
static const int DEBUG_SHOW_AGENT_PROCESSOR = 0;
static const int DEBUG_SHOW_AGENT_STREAM    = 1;

static const int DEBUG_SHOW_NETWORK_PACKET  = 0;
static const int DEBUG_TRACE_NETWORK_PACKET = 0;


static const int DEBUG_TRACE_XFER           = 0;

static const int DEBUG_STOP_AT_CONTROL_TRAP = 1;
static const int DEBUG_STOP_AT_UNBOUND_TRAP = 0;
static const int DEBUG_STOP_AT_OPCODE_TRAP  = 0;
static const int DEBUG_STOP_AT_PAGE_FAULT   = 0;
static const int DEBUG_STOP_AT_MP_915       = 0;
static const int DEBUG_STOP_AT_STACK_ERROR  = 1;

static const int DEBUG_STOP_MESSAGE_UNTIL_MP = 0;

static const int DEBUG_SHOW_STATS_OPCODE    = 0;
static const int DEBUG_SHOW_STATS_RUN       = 0;

static const int DEBUG_ENABLE_HOTSPOT       = 0;
static const int DEBUG_ENABLE_PEEPHOLE      = 0;
static const int DEBUG_SHOW_HOTSPOT_COMPILE = 0;
static const int DEBUG_SHOW_HOTSPOT_RUN     = 0;
static const int DEBUG_SHOW_HOTSPOT_DETAIL  = 0;
static const int DEBUG_SHOW_HOTSPOT_USAGE   = 0;

static const int DEBUG_TRACE_COMPILE        = 0;
static const int DEBUG_TRACE_RUN            = 0;
static const int DEBUG_USE_COMPILE_RUN      = 0;

#endif
