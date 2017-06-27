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
// Trace.h
//

#ifndef TRACE_H_
#define TRACE_H_ 1

#include "../mesa/Pilot.h"


#define READ_OBJECT_PROLOGUE() \
{ \
	if (ptr == 0) { \
		logger.fatal("ptr is zero"); \
		ERROR(); \
		return; \
	} \
	if (target == 0) { \
		logger.fatal("target is zero"); \
		ERROR(); \
		return; \
	} \
	::memset(target, 0, sizeof(*target)); \
}

#define READ_TARGET(t, f) readObject(ptr + OFFSET(t, f), &(target->f));


// readObject
void readObject(CARD32 ptr, CARD16*                          target);
void readObject(CARD32 ptr, CARD32*                          target);

void readObject(CARD32 ptr, LoadStateFormat::ModuleInfo*     target);
void readObject(CARD32 ptr, CPSwapDefs::ExternalStateVector* target);
void readObject(CARD32 ptr, LoadStateFormat::BcdInfo*        target);
void readObject(CARD32 ptr, LoadStateFormat::Object*         target);
void readObject(CARD32 ptr, TimeStamp::Stamp*                target);
void readObject(CARD32 ptr, BcdDefs::BCD*                    target);
void readObject(CARD32 ptr, PrincOpsExtras2::GFTItem*        target);

#endif


