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


//
// StreamOf.h
//

#ifndef COURIER_STREAMOF_H__
#define COURIER_STREAMOF_H__

#include "../courier/Courier.h"

namespace Courier {

//
// StreamOfSegmentType
//
enum class StreamOfSegmentType : quint16 {
    nextSegment = 0,
    lastSegment = 1,
};

QString toString(const Courier::StreamOfSegmentType value);
void serialize(BLOCK& block, const Courier::StreamOfSegmentType value);
void deserialize(BLOCK& block, Courier::StreamOfSegmentType& value);

//
// StreamOfSegment
//
template <typename T>
struct StreamOfSegment {
	using TYPE = T;

	StreamOfSegmentType segmentType;
	QVector<T>          segment;
};
template <typename T>
QString toString(const StreamOfSegment<T>& value) {
	QStringList list;
	int size = value.segment.size();
	for(int i = 0; i < size; i++) {
		list << Courier::toString(value.segment[i]);
	}
	return QString("[%1 (%2)[%3]]").arg(Courier::toString(value.segmentType)).arg(size).arg(list.join(" "));
}
template <typename T>
void serialize(BLOCK& block, const Courier::StreamOfSegment<T>& value) {
	// type
	Courier::serialize(block, value.segmentType);
	// size
	quint16 size = (quint16)value.segment.size();
	Courier::serialize(block, size);
	// data
	for(int i = 0; i < size; i++) {
		const T& data = value.segment.at(i);
		Courier::serialize(block, data);
	}
}
template <typename T>
void deserialize(BLOCK& block, Courier::StreamOfSegment<T>& value) {
	// type
	Courier::deserialize(block, value.segmentType);
	// size
	quint16 size;
	Courier::deserialize(block, size);
	value.segment.reserve(size);
	// data
	for(int i = 0; i < size; i++) {
		T data;
		Courier::deserialize(block, data);
		value.segment.append(data);
	}
}

//
// StreamOf
//
template <typename T>
struct StreamOf {
	using TYPE = T;

	QVector<StreamOfSegment<T>> segmens;
};
template <typename T>
QString toString(const StreamOf<T>& value) {
	QStringList list;
	int size = value.segmens.size();
	for(int i = 0; i < size; i++) {
		const StreamOfSegment<T>& segment = value.segments.at(i);
		list << Courier::toString(segment);
	}
	return QString("(%1)[%2]").arg(size).arg(list.join(" "));
}
template <typename T>
void serialize(BLOCK& block, const Courier::StreamOf<T>& value) {
	int size = value.segmens.size();
	for(int i = 0; i < size; i++) {
		const StreamOfSegment<T>& segment = value.segments.at(i);
		Courier::serialize(block, segment);
	}
}
template <typename T>
void deserialize(BLOCK& block, Courier::StreamOf<T>& value) {
	for(;;) {
		StreamOfSegment<T> segment;
		Courier::deserialize(block, segment);
		value.segments.append(segment);

		if (segment.segmentType == StreamOfSegmentType::lastSegment) break;
	}
}

}
#endif

