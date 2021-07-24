/*
Copyright (c) 2014, 2017, Yasuhiro Hasegawa
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
// Perf.cpp
//

#include "Perf.h"

long long perf_Dispatch = 0;
long long perf_Fetch = 0;
long long perf_Store = 0;
long long perf_ReadDbl = 0;
long long perf_FetchCode = 0;
long long perf_FetchMds = 0;
long long perf_StoreMds = 0;
long long perf_ReadDblMds = 0;
long long perf_FetchLF = 0;
long long perf_StoreLF = 0;
long long perf_ReadDblLF = 0;
long long perf_GetCodeByte = 0;
long long perf_GetCodeWord = 0;
long long perf_FetchByte = 0;
long long perf_StoreByte = 0;
long long perf_ReadField = 0;
long long perf_WriteField = 0;
long long perf_WriteMap = 0;
long long perf_GetAddress = 0;
long long perf_FetchPda = 0;
long long perf_StorePda = 0;
long long perf_MemoryFetch = 0;
long long perf_MemoryStore = 0;

// Fault
long long perf_FrameFault = 0;
long long perf_PageFault = 0;
// Trap
long long perf_CodeTrap = 0;
long long perf_EscOpcodeTrap = 0;
long long perf_OpcodeTrap = 0;
long long perf_UnboundTrap = 0;
