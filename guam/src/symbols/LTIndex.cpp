/*
Copyright (c) 2017, Yasuhiro Hasegawa
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
// LTIndex.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("lt");

#include "LTIndex.h"
#include "BCDFile.h"

#include "SEIndex.h"

//
// LTIndex
//

LTIndex* LTIndex::getNull() {
	static LTIndex ret(0, LTIndex::LT_NULL);
	return &ret;
}
LTIndex* LTIndex::getInstance(Symbols* symbols, CARD16 index) {
	return new LTIndex(symbols, index);
}
QString LTIndex::toString() const {
	if (isNull()) return QString("%1-NULL").arg(PREFIX);
	return QString("%1-%2").arg(PREFIX).arg(index);
}
const LTRecord& LTIndex::getValue() const {
	LTRecord* ret = LTRecord::find(symbols, index);
	if (ret == 0) {
		logger.fatal("Cannot find  symbols = %p  index = %d", symbols, index);
		ERROR();
	}
	return *ret;
}


//
// LTRecord
//

//WordSequence: TYPE = ARRAY [0..0) OF WORD;
//
//LTRecord: TYPE = RECORD [
//  link(0:0..12): LTIndex,
//  datum(0:13..47): SELECT kind(0:13..15): * FROM
//    short => [value(1:0..15): WORD],
//    long => [
//      codeIndex(1:0..15): [0..LAST[CARDINAL]/2],
//      length(2:0..15): CARDINAL,
//      value(3): WordSequence]
//    ENDCASE];

QMap<LTRecord::Key, LTRecord*> LTRecord::all;
LTRecord* LTRecord::find(Symbols* symbols, CARD16 index) {
	Key key(symbols, index);
	return all.value(key, 0);
}

QString LTRecord::toString(Tag value) {
	TO_STRING_PROLOGUE(Tag)

	MAP_ENTRY(SHORT)
	MAP_ENTRY(LONG)

	TO_STRING_EPILOGUE
}

QString LTRecord::Short::toString() const {
	return QString("%1").arg(value);
}

QString LTRecord::Long::toString() const {
	QString values;
	for(CARD16 i = 0; i < length; i++) {
		values.append(QString(i ? " %1" : "%1").arg(i));
	}
	return QString("%1 (%2)%3").arg(codeIndex).arg(length).arg(values);
}

const LTRecord::Short&   LTRecord::getShort() const {
	if (tag != Tag::SHORT) ERROR();
	if (tagValue == 0) ERROR();
	Short* ret = (Short*)tagValue;
	return *ret;
}

const LTRecord::Long&   LTRecord::getLong() const {
	if (tag != Tag::LONG) ERROR();
	if (tagValue == 0) ERROR();
	Long* ret = (Long*)tagValue;
	return *ret;
}

LTRecord* LTRecord::getInstance(Symbols* symbols, CARD16 index) {
 	CARD16 u0 = symbols->file->getCARD16();
 	Tag   tag = (Tag)bitField(u0, 13, 15);
 	void* tagValue;

 	switch(tag) {
 	case Tag::SHORT:
 		tagValue = new Short(symbols->file->getCARD16());
 		break;
 	case Tag::LONG:
 	{
 		CARD16  codeIndex = symbols->file->getCARD16();
 		CARD16  length    = symbols->file->getCARD16();
 		CARD16* value     = new CARD16[length];
 		for(CARD16 i = 0; i < length; i++) value[i] = symbols->file->getCARD16();

 		tagValue = new Long(codeIndex, length, value);
 	}
 		break;
 	default:
 		ERROR();
 		tagValue = 0;
 		break;
 	}

    return new LTRecord(symbols, index, tag, tagValue);
}

QString LTRecord::toString() const {
	switch(tag) {
	case Tag::SHORT:
		return QString("%1 %2").arg(toString(tag)).arg(getShort().toString());
	case Tag::LONG:
		return QString("%1 %2").arg(toString(tag)).arg(getLong().toString());
	default:
		ERROR();
		return "???";
	}
}


LitRecord* LitRecord::getInstance(Symbols* symbols, CARD16 u0) {
	// IMPORTANT
	//    LitRecord: TYPE = RECORD [
	//      SELECT litTag(0:0..0): * FROM
	//      word => [index(0:1..13): LTIndex],
	//      string => [index(0:1..13): STIndex]
	//      ENDCASE];
	//   Size of LitRecord is 14 bits.
	//   So need to shift 2 bit before process u0.
	u0 <<= 2;

	Tag tag = (Tag)bitField(u0, 0);
	void* tagValue;
	switch(tag) {
	case Tag::WORD:
	{
		LTIndex* index = LTIndex::getInstance(symbols, bitField(u0, 1, 13));
		tagValue = new Word(index);
	}
		break;
	case Tag::STRING:
	{
		CARD16 index = bitField(u0, 1, 13);
		tagValue = new String(index);
	}
		break;
	default:
		ERROR();
		tagValue = 0;
	}
	return new LitRecord(tag, tagValue);
}
QString LitRecord::toString() const {
	switch(tag) {
	case Tag::WORD:
		return QString("%1 %2").arg(toString(tag)).arg(getWord().toString());
	case Tag::STRING:
		return QString("%1 %2").arg(toString(tag)).arg(getString().toString());
	default:
		ERROR();
		return "???";
	}
}

QString LitRecord::toString(Tag value) {
	TO_STRING_PROLOGUE(Tag)

	MAP_ENTRY(WORD)
	MAP_ENTRY(STRING)

	TO_STRING_EPILOGUE
}

QString LitRecord::Word::toString() const {
	return QString("%1").arg(index->toString());
}

QString LitRecord::String::toString() const {
	return QString("%1").arg(index);
}

const LitRecord::Word& LitRecord::getWord() const {
	if (tag != Tag::WORD) ERROR();
	if (tagValue == 0) ERROR();
	Word* ret = (Word*)tagValue;
	return *ret;
}

const LitRecord::String& LitRecord::getString() const {
	if (tag != Tag::STRING) ERROR();
	if (tagValue == 0) ERROR();
	String* ret = (String*)tagValue;
	return *ret;
}


