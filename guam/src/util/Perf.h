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
// Perf.h
//

#ifndef PERF_H__
#define PERF_H__

static const int PERF_ENABLE    = 1;

extern int perf_stop_at_mp_8000;

extern long long perf_Dispatch;
extern long long perf_EscOpcodeTrap;
extern long long perf_OpcodeTrap;
extern long long perf_Fetch;
extern long long perf_Store;
extern long long perf_ReadDbl;
extern long long perf_FetchCode;
extern long long perf_FetchMds;
extern long long perf_StoreMds;
extern long long perf_ReadDblMds;
extern long long perf_FetchLF;
extern long long perf_StoreLF;
extern long long perf_ReadDblLF;
extern long long perf_GetCodeByte;
extern long long perf_GetCodeWord;
extern long long perf_FetchByte;
extern long long perf_StoreByte;
extern long long perf_ReadField;
extern long long perf_WriteField;
extern long long perf_WriteMap;
extern long long perf_GetAddress;
extern long long perf_FetchPda;
extern long long perf_StorePda;
extern long long perf_storeLF;
extern long long perf_storeLFHit;
extern long long perf_storeMDS;
extern long long perf_storeMDSHit;
extern long long perf_storePDA;
extern long long perf_storePDAHit;
extern long long perf_MemoryFetch;
extern long long perf_MemoryStore;

#define Perf_log() if (PERF_ENABLE) { \
		logger.info("perf_Dispatch      = %10llu", perf_Dispatch); \
		logger.info("perf_EscOpcodeTrap = %10llu", perf_EscOpcodeTrap); \
		logger.info("perf_OpcodeTrap    = %10llu", perf_OpcodeTrap); \
		logger.info("perf_Fetch         = %10llu", perf_Fetch); \
		logger.info("perf_Store         = %10llu", perf_Store); \
		logger.info("perf_ReadDbl       = %10llu", perf_ReadDbl); \
		logger.info("perf_FetchMds      = %10llu", perf_FetchMds); \
		logger.info("perf_StoreMds      = %10llu", perf_StoreMds); \
		logger.info("perf_ReadDblMds    = %10llu", perf_ReadDblMds); \
		logger.info("perf_FetchLF       = %10llu", perf_FetchLF); \
		logger.info("perf_StoreLF       = %10llu", perf_StoreLF); \
		logger.info("perf_ReadDblLF     = %10llu", perf_ReadDblLF); \
		logger.info("perf_FetchCode     = %10llu", perf_FetchCode); \
		logger.info("perf_GetCodeByte   = %10llu", perf_GetCodeByte); \
		logger.info("perf_GetCodeWord   = %10llu", perf_GetCodeWord); \
		logger.info("perf_FetchByte     = %10llu", perf_FetchByte); \
		logger.info("perf_StoreByte     = %10llu", perf_StoreByte); \
		logger.info("perf_ReadField     = %10llu", perf_ReadField); \
		logger.info("perf_WriteField    = %10llu", perf_WriteField); \
		logger.info("perf_WriteMap      = %10llu", perf_WriteMap); \
		logger.info("perf_GetAddress    = %10llu", perf_GetAddress); \
		logger.info("perf_FetchPda      = %10llu", perf_FetchPda); \
		logger.info("perf_StorePda      = %10llu", perf_StorePda); \
		logger.info("perf_storeLF       = %10llu", perf_storeLF); \
		logger.info("perf_storeLFHit    = %10llu", perf_storeLFHit); \
		logger.info("perf_storeMDS      = %10llu", perf_storeMDS); \
		logger.info("perf_storeMDSHit   = %10llu", perf_storeMDSHit); \
		logger.info("perf_storePDA      = %10llu", perf_storePDA); \
		logger.info("perf_storePDAHit   = %10llu", perf_storePDAHit); \
		logger.info("perf_MemoryFetch   = %10llu", perf_MemoryFetch); \
		logger.info("perf_MemoryStore   = %10llu", perf_MemoryStore); \
}

#endif
