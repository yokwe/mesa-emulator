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
// ShowTypes.h
//

#ifndef SHOWTYPES_H__
#define SHOWTYPES_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "Symbols.h"

#include <QTextStream>

#include <functional>


//EnumeratedSEIndex: TYPE = Table.Base RELATIVE POINTER [0..Table.Limit)
//  TO enumerated cons Symbols.SERecord;
//
//ValFormat: TYPE = RECORD [
//  SELECT tag: * FROM
//    signed => [], --an INTEGER or subrange with base < 0
//    unsigned => [], -- a CARDINAL, WORD, UNSPECIFIED, or subrange w/ base >= 0
//    char => [], --a character
//    enum => [esei: EnumeratedSEIndex], --an enumerated type
//    array => [componentType: Symbols.SEIndex],
//    transfer => [mode: Symbols.TransferMode], --a PROCEDURE, SIGNAL, ERROR, PROGRAM, or PORT
//    ref => [], --a pointer
//    other => [], --anything else (whether single word or multi-word)
//    ENDCASE];
class ValFormat {
public:
	typedef Symbols::TransferMode TransferMode;
	static QString toString(TransferMode value) {
		return Symbols::toString(value);
	}

	enum class Tag {SIGNED, UNSIGNED, CHAR, ENUM, ARRAY, TRANSFER, REF, OTHER};
	static QString toString(Tag value);

	class Enum {
	public:
		const SEIndex* esei;

		QString toString() const;

		Enum(const SEIndex* esei_) : esei(esei_) {}
	};

	class Array {
	public:
		const SEIndex* componentType;

		QString toString() const;

		Array(const SEIndex* componentType_) : componentType(componentType_) {}
	};

	class Transfer {
	public:
		const TransferMode mode;;

		QString toString() const;

		Transfer(const TransferMode mode_) : mode(mode_) {}
	};

	static ValFormat SIGNED() {
		ValFormat ret(Tag::SIGNED, 0);
		return ret;
	}
	static ValFormat UNSIGNED() {
		ValFormat ret(Tag::UNSIGNED, 0);
		return ret;
	}
	static ValFormat CHAR() {
		ValFormat ret(Tag::CHAR, 0);
		return ret;
	}
	static ValFormat ENUM(const SEIndex* esei) {
		ValFormat ret(Tag::ENUM, new Enum(esei));
		return ret;
	}
	static ValFormat ARRAY(const SEIndex* componentType) {
		ValFormat ret(Tag::ARRAY, new Array(componentType));
		return ret;
	}
	static ValFormat TRASNFER(const TransferMode mode) {
		ValFormat ret(Tag::TRANSFER, new Transfer(mode));
		return ret;
	}
	static ValFormat REF() {
		ValFormat ret(Tag::REF, 0);
		return ret;
	}
	static ValFormat OTHER() {
		ValFormat ret(Tag::OTHER, 0);
		return ret;
	}

	ValFormat() : tag(Tag::OTHER), tagValue(0) {}
	ValFormat(const ValFormat& that) : tag(that.tag), tagValue(that.tagValue) {}

	Tag   tag;
	void* tagValue;

	const Enum&     getEnum()     const;
	const Array&    getArray()    const;
	const Transfer& getTransfer() const;

	QString toString() const;
private:
	ValFormat(Tag tag_, void* tagValue_) : tag(tag_), tagValue(tagValue_) {}
};

class ShowType {
public:
    //PrintSym: PROCEDURE [sei: Symbols.ISEIndex, colonstring: LONG STRING] =
    static void printSym(QTextStream& out, const SEIndex* sei, QString colonString);

    //PrintTypedVal: PROCEDURE [val: UNSPECIFIED, vf: ValFormat] =
    static void printTypedVal(QTextStream& out, CARD16 val, ValFormat vf);

    //GetBitSpec: PROCEDURE [isei: Symbols.ISEIndex, bitspec: LONG STRING] =
    static void getBitSpec(const SEIndex* isei, QString& bitspec);

    //OutArgType: PROCEDURE [sei: Symbols.CSEIndex] = {
    static void outArgType(QTextStream& out, const SEIndex* sei);

    //PrintFieldCtx: PROCEDURE [ctx: Symbols.CTXIndex, md: BOOLEAN � FALSE] =
    static void printFieldCtx(QTextStream& out, const CTXIndex* ctx, bool md = false);

    //PrintHti: PROCEDURE [hti: Symbols.HTIndex] =
    static void printHti(QTextStream& out, const HTIndex* hti);

    //PrintSei: PROCEDURE [sei: Symbols.ISEIndex] = {
    static void printSei(QTextStream& out, const SEIndex* sei);

    //PutEnum: PROCEDURE [val: UNSPECIFIED, esei: EnumeratedSEIndex] =
    static void putEnum(QTextStream& out, CARD16 val, const SEIndex* esei);

    //GetValFormat: PROCEDURE [tsei: Symbols.SEIndex] RETURNS [vf: ValFormat] =
    static ValFormat getValFormat(const SEIndex* tsei);

    //PrintType: PROCEDURE [tsei: Symbols.SEIndex, dosub: PROCEDURE] RETURNS [vf: ValFormat] =
    static ValFormat printType(QTextStream& out, const SEIndex* tsei, std::function<void()> dosub);

    //IsVar: PROC [tsei: Symbols.SEIndex] RETURNS [BOOLEAN] =
    static bool isVar(const SEIndex* tsei);

    //PutModeName: PROCEDURE [n: Symbols.TransferMode] =
    static void putModeName(QTextStream& out, Symbols::TransferMode n);

    //PrintDefaultValue: PROCEDURE [sei: Symbols.ISEIndex, vf: ValFormat] =
    static void printDefaultValue(QTextStream& out, const SEIndex* sei, ValFormat vf);

    //PrintTreeLink: PROCEDURE [tree: Tree.Link, vf: ValFormat, recur: CARDINAL, sonOfDot: BOOLEAN � FALSE] =
    static void printTreeLink(QTextStream& out, const TreeLink* tree, ValFormat vf, int recur, bool sonOfDot = false);

    // PutWordSeq: PROCEDURE [seq: LONG DESCRIPTOR FOR ARRAY OF UNSPECIFIED] =
    static void putWordSeq(QTextStream& out, CARD16 length, const CARD16* value);

    // for debug
    static void dump(Symbols* symbols);
    static void dumpSymbol(QString filePath, QString outDirPath);

private:
    //defaultPublic: BOOLEAN � TRUE;     --outer RECORD is public or private?
    static bool defaultPublic;

    //showBits: BOOLEAN;  --if TRUE, show bit positions even if not MACHINE DEPENDENT
    static bool showBits;
};

#endif
