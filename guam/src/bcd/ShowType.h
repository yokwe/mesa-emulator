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
// ShowType.h
//

#ifndef SHOWTYPE_H__
#define SHOWTYPE_H__

#include "../util/Util.h"
#include "../mesa/MesaBasic.h"

#include "SERecord.h"

class CTXIndex;
class TreeLink;

// From AMesa/14.0/Sword/Private/ITShowType.mesa

class ShowType {
public:
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
		enum class Tag {
			SIGNED, UNSIGNED, CHAR, ENUM, ARRAY, TRANSFER, REF, OTHER,
		};
		static QString toString(Tag value);

		static ValFormat Signed() {
			return ValFormat(Tag::SIGNED);
		}
		static ValFormat Unsigned() {
			return ValFormat(Tag::UNSIGNED);
		}
		static ValFormat Char() {
			return ValFormat(Tag::CHAR);
		}
		static ValFormat Enum(SEIndex* index_) {
			return ValFormat(Tag::ENUM, index_);
		}
		static ValFormat Array(SEIndex* index_) {
			return ValFormat(Tag::ARRAY, index_);
		}
		static ValFormat Transfer(Symbols::TransferMode mode) {
			return ValFormat(Tag::TRANSFER, mode);
		}
		static ValFormat Ref() {
			return ValFormat(Tag::REF);
		}
		static ValFormat Other() {
			return ValFormat(Tag::OTHER);
		}

		ValFormat(const ValFormat& that);

		Tag tag;
		union {
			struct {
				SEIndex* esei;
			} enum_;
			struct {
				SEIndex* componentType;
			} array;
			struct {
				Symbols::TransferMode mode;
			} transfer;
		};

        QString toString();
	private:
        ValFormat(Tag tag_) {                        // SIGNED  UNSIGNED  CHAR  REF  OTHER
            tag   = tag_;
        }
        ValFormat(Tag tag_, SEIndex* value);         // ENUM  ARRAY
        ValFormat(Tag tag_, Symbols::TransferMode); // TRANSFER
	};


    //GetBitSpec: PROCEDURE [isei: Symbols.ISEIndex, bitspec: LONG STRING] =
    static void getBitSpec(SEIndex* isei, QString& bitspec);

    //GetValFormat: PROCEDURE [tsei: Symbols.SEIndex] RETURNS [vf: ValFormat] =
    static ValFormat getValFormat(SEIndex* tsei);

    //IsVar: PROC [tsei: Symbols.SEIndex] RETURNS [BOOLEAN] =
	static bool isVar(SEIndex* index);

    //OutArgType: PROCEDURE [sei: Symbols.CSEIndex] = {
    static void outArgType(QString& out, SEIndex* sei);

    //PrintDefaultValue: PROCEDURE [sei: Symbols.ISEIndex, vf: ValFormat] =
	static void printDefaultValue(QString& out, SEIndex* sei, ValFormat& vf);

    //PrintFieldCtx: PROCEDURE [ctx: Symbols.CTXIndex, md: BOOLEAN � FALSE] =
    static void printFieldCtx(QString& out, CTXIndex* ctx, bool md = false);

    //PrintHti: PROCEDURE [hti: Symbols.HTIndex] =
    static void printHti(QString& out, HTIndex* hti);

    //PrintSei: PROCEDURE [sei: Symbols.ISEIndex] = {
    static void printSei(QString& out, SEIndex* sei);

    //PrintSym: PROCEDURE [sei: Symbols.ISEIndex, colonstring: LONG STRING] =
	static void printSym(QString& out, SEIndex* sei, QString colonString);

    //PrintTreeLink: PROCEDURE [tree: Tree.Link, vf: ValFormat, recur: CARDINAL, sonOfDot: BOOLEAN � FALSE] =
    static void printTreeLink(QString& out, TreeLink* tree, ValFormat& vf, int recur, bool sonOfDot = false);

    //PrintType: PROCEDURE [tsei: Symbols.SEIndex, dosub: PROCEDURE] RETURNS [vf: ValFormat] =
    static ValFormat printType(QString& out, SEIndex* tsei /*, Runnable dosub*/);

    //PrintTypedVal: PROCEDURE [val: UNSPECIFIED, vf: ValFormat] =
    static void printTypedVal(QString& out, int val, ValFormat& vf);

    // PutCurrentModuleDot: PROCEDURE = {

    //PutEnum: PROCEDURE [val: UNSPECIFIED, esei: EnumeratedSEIndex] =
    static void putEnum(QString& out, int val, SEIndex* esei);

    //PutModeName: PROCEDURE [n: Symbols.TransferMode] =
    static void putModeName(QString& out, Symbols::TransferMode n);

    // PutWordSeq: PROCEDURE [seq: LONG DESCRIPTOR FOR ARRAY OF UNSPECIFIED] =
    static void putWordSeq(QString& out, CARD16 length, CARD16* seq);


private:
    //defaultPublic: BOOLEAN � TRUE;     --outer RECORD is public or private?
    static bool defaultPublic;
    //indent: CARDINAL;             --how many spaces to indent variants
    //showBits: BOOLEAN;  --if TRUE, show bit positions even if not MACHINE DEPENDENT
    static bool showBits;
};

#endif
