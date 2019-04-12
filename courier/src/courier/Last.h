/*
Copyright (c) 2019, Yasuhiro Hasegawa
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

#ifndef COURIER_LAST_H_
#define COURIER_LAST_H_

namespace Courier {

//
// This header file need to be last include of source file.
// Because Courier::toString(), Courier::serialize() and Courier::deserialize()
// used in template below must declared beforehand.
//

//
// Template functions for ARRAY
//

template <typename T, int N>
QString toString(const SEQUENCE<T, N>& value) {
	QStringList list;
	quint16 size = value.getSize();
	for(quint16 i = 0; i < size; i++) {
		list << Courier::toString(value[i]);
	}
	return QString("(%1)[%2]").arg(size).arg(list.join(" "));
}
template <typename T, int N>
void serialize(BLOCK& block, const SEQUENCE<T, N>& value) {
	quint16 size = value.getSize();
	Courier::serialize(block, size);
	for(int i = 0; i < size; i++) {
		Courier::serialize(block, value[i]);
	}
}
template <typename T, int N>
void deserialize(BLOCK& block, SEQUENCE<T, N>& value) {
	quint16 size;
	Courier::deserialize(block, size);
	value.setSize(size);
	for(int i = 0; i < size; i++) {
		Courier::deserialize(block, value[i]);
	}
}

//
// Template functions for ARRAY
//
template <typename T, int N>
QString toString(const ARRAY<T, N>& value) {
	quint16 maxSize = value.maxSize;
	QStringList list;
	for(int i = 0; i < maxSize; i++) {
		list << Courier::toString(value[i]);
	}
	return QString("(%1)[%2]").arg(maxSize).arg(list.join(" "));
}
template <typename T, int N>
void serialize(BLOCK& block, const ARRAY<T, N>& value) {
	for(int i = 0; i < value.maxSize; i++) {
		Courier::serialize(block, value[i]);
	}
}
template <typename T, int N>
void deserialize(BLOCK& block, ARRAY<T, N>& value) {
	for(int i = 0; i < value.maxSize; i++) {
		Courier::deserialize(block, value[i]);
	}
}

}

#endif /* COURIER_LAST_H_ */
