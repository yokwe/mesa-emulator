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
// ShowType.cpp
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("showtype");

#include "ShowType.h"

#include "Symbols.h"
#include "SERecord.h"
#include "CTXRecord.h"
#include "ExtRecord.h"


// From AMesa/14.0/Sword/Private/ITShowType.mesa

bool ShowType::defaultPublic = true;
bool ShowType::showBits = true;


ShowType::ValFormat::ValFormat(const ValFormat& that) {
	tag = that.tag;
	switch(tag) {
	case Tag::SIGNED:
	case Tag::UNSIGNED:
	case Tag::CHAR:
	case Tag::REF:
	case Tag::OTHER:
		break;
	case Tag::ENUM:
		enum_.esei = that.enum_.esei;
		break;
	case Tag::ARRAY:
		array.componentType = that.array.componentType;
		break;
	case Tag::TRANSFER:
		transfer.mode = that.transfer.mode;
		break;
	default:
		ERROR();
		break;
	}
}

QString ShowType::ValFormat::toString(Tag value) {
	TO_STRING_PROLOGUE(Tag)

	MAP_ENTRY(SIGNED)
	MAP_ENTRY(SIGNED)
	MAP_ENTRY(CHAR)
	MAP_ENTRY(ENUM)
	MAP_ENTRY(ARRAY)
	MAP_ENTRY(TRANSFER)
	MAP_ENTRY(REF)
	MAP_ENTRY(OTHER)

	TO_STRING_EPILOGUE
}

QString ShowType::ValFormat::toString() {
	switch(tag) {
	case Tag::SIGNED:
	case Tag::UNSIGNED:
	case Tag::CHAR:
	case Tag::REF:
	case Tag::OTHER:
		return QString("%1").arg(toString(tag));
	case Tag::ENUM:
		return QString("%1 %2").arg(toString(tag)).arg(enum_.esei->toString());
	case Tag::ARRAY:
		return QString("%1 %2").arg(toString(tag)).arg(array.componentType->toString());
	case Tag::TRANSFER:
		return QString("%1 %2").arg(toString(tag)).arg(SERecord::toString(transfer.mode));
	default:
		ERROR();
		return "???";
	}
}

//GetBitSpec: PROCEDURE [isei: Symbols.ISEIndex, bitspec: LONG STRING] =
void ShowType::getBitSpec(SEIndex* isei, QString& bitspec) {
    //BEGIN
    //  a: Symbols.BitAddress = SymbolOps.ToBitAddr[symbols.seb[isei].idValue];
	CARD16 a = isei->value->id.idValue;
	CARD16 wd = bitField(a, 0, 11);
	CARD16 bd = bitField(a, 12, 15);
    //  s: CARDINAL = SymbolOps.ToCard[symbols.seb[isei].idInfo];
	CARD16 s = isei->value->id.idInfo;
    //  bitspec.length � 0;
	bitspec.resize(0);
    //  String.AppendString[bitspec, " ("L];
    //  String.AppendDecimal[bitspec, a.wd];
	bitspec.append(QString(" (%1").arg(wd));
    //  IF s # 0 THEN
	if (s) {
    //    {String.AppendChar[bitspec, ':];
    //    String.AppendDecimal[bitspec, a.bd];
    //    String.AppendString[bitspec, ".."L];
    //    String.AppendDecimal[bitspec, a.bd+s-1]};
		bitspec.append(QString(":%1..%2").arg(bd).arg(bd + s - 1));
	}
    //  String.AppendString[bitspec, "): "L];
	bitspec.append("): ");
    //END;
}


//GetValFormat: PROCEDURE [tsei: Symbols.SEIndex] RETURNS [vf: ValFormat] =
ShowType::ValFormat ShowType::getValFormat(SEIndex* tsei) {
	//BEGIN
	//  WITH t: symbols.seb[tsei] SELECT FROM
	SERecord* t = tsei->value;
	switch(t->seTag) {
	//    id => RETURN[GetValFormat[SymbolOps.UnderType[symbols, tsei]]];
	case SERecord::SeTag::ID:
		return getValFormat(tsei->underType());
	//    cons =>
	case SERecord::SeTag::CONS:
		switch(t->cons.typeTag) {
	//	    WITH t SELECT FROM
	//	      basic =>
		case SERecord::TypeClass::BASIC:
	//	        SELECT code FROM
			switch(t->cons.basic.code) {
	//	  	      Symbols.codeANY => vf � [unsigned[]];
			case Symbols::codeANY:
				return ValFormat::Unsigned();
	//	  	      Symbols.codeINT => vf � [signed[]];
			case Symbols::codeINT:
				return ValFormat::Signed();
	//	  	      Symbols.codeCHAR => vf � [char[]];
			case Symbols::codeCHAR:
				return ValFormat::Char();
	//	  	      ENDCASE;
			default:
				ERROR();
				return ValFormat::Other();
			}
	//	  	  enumerated => vf � [enum[LOOPHOLE[tsei]]];
			case SERecord::TypeClass::ENUMERATED:
				return ValFormat::Enum(tsei);
	//	  	  array => vf � [array[componentType]];
			case SERecord::TypeClass::ARRAY:
				return ValFormat::Array(tsei->value->cons.array.componentType);
	//	  	  transfer => vf � [transfer[mode]];
			case SERecord::TypeClass::TRANSFER:
				return ValFormat::Transfer(tsei->value->cons.transfer.mode);
	//	  	  relative => vf � GetValFormat[offsetType];
			case SERecord::TypeClass::RELATIVE:
				return getValFormat(tsei->value->cons.relative.offsetType);
	//	  	  subrange =>
			case SERecord::TypeClass::SUBRANGE:
			{
	//	  	     {vf � GetValFormat[rangeType];
				ValFormat vf = getValFormat(t->cons.subrange.rangeType);
	//	  	     IF vf.tag = signed AND origin = 0 THEN vf � [unsigned[]]};
				if (vf.tag == ValFormat::Tag::SIGNED && t->cons.subrange.origin == 0) {
					return ValFormat::Unsigned();
				} else {
					return vf;
				}
			}
	//	  	  long => vf � GetValFormat[rangeType];
			case SERecord::TypeClass::LONG:
				return getValFormat(t->cons.long_.rangeType);
	//	  	  ref => vf � [ref[]];
			case SERecord::TypeClass::REF:
				return ValFormat::Ref();
	//	  	  ENDCASE => vf � [other[]];
			default:
				return ValFormat::Other();
		}
	//	   ENDCASE => vf � [other[]];
		default:
			return ValFormat::Other();
		}
	//END;
}


//IsVar: PROC [tsei: Symbols.SEIndex] RETURNS [BOOLEAN] =
//  BEGIN
bool ShowType::isVar(SEIndex* tsei) {
//  WITH t: symbols.seb[tsei] SELECT FROM
	SERecord* t = tsei->value;
	switch(t->seTag) {
//    id => RETURN [FALSE];
	case SERecord::SeTag::ID:
		return false;
//    cons => WITH t2: t SELECT FROM
	case SERecord::SeTag::CONS:
		switch(t->cons.typeTag) {
//      ref => RETURN [t2.var]
		case SERecord::TypeClass::REF:
			return t->cons.ref.var;
//	    ENDCASE => RETURN [FALSE];
		default:
			return false;
		}
		break;
//    ENDCASE => RETURN [FALSE];
	default:
		return false;
	}
//  END;
}


//OutArgType: PROCEDURE [sei: Symbols.CSEIndex] = {
void ShowType::outArgType(QString& out, SEIndex* sei) {
//  IF sei = Symbols.SENull THEN out[": NIL"L, cd]
	if (sei->isNull()) {
		out.append(": NIL");
//  ELSE
	} else {
//    WITH t: symbols.seb[sei] SELECT FROM
		SERecord* t = sei->value;
		if (t->seTag != SERecord::SeTag::CONS) ERROR();

		switch(t->cons.typeTag) {
//      record => PrintFieldCtx[t.fieldCtx];
		case SERecord::TypeClass::RECORD:
			printFieldCtx(out, t->cons.record.fieldCtx);
			break;
//	    any => out[": ANY"L, cd];
		case SERecord::TypeClass::ANY:
			out.append(": ANY");
			break;
//	    ENDCASE};
		default:
			break;
		}
	}
}

//PrintDefaultValue: PROCEDURE [sei: Symbols.ISEIndex, vf: ValFormat] =
//  BEGIN
//  extType: Symbols.ExtensionType;
//  tree: Tree.Link;
//  [extType, tree] � SymbolOps.FindExtension[symbols, sei];
//  IF extType # default THEN RETURN;
//  out[" � "L, cd];
//  PrintTreeLink[tree, vf, 0];
//  END;
void ShowType::printDefaultValue(QString& out, SEIndex* sei, ValFormat& vf) {
	ExtRecord* record = sei->symbols->findExtention(sei);
	if (record->type != ExtRecord::ExtensionType::DEFAULT) return;
	out.append(" = ");
	printTreeLink(out, record->tree, vf, false);
}

//PrintFieldCtx: PROCEDURE [ctx: Symbols.CTXIndex, md: BOOLEAN � FALSE] =
//  BEGIN
void ShowType::printFieldCtx(QString& out, CTXIndex* ctx, bool md) {
//  isei: Symbols.ISEIndex � SymbolOps.FirstCtxSe[symbols, ctx];
//  bitspec: LONG STRING � [20];
//  first: BOOLEAN � TRUE;
//  IF ~md THEN String.AppendString[bitspec, ": "L];
//  IF isei # Symbols.ISENull AND symbols.seb[isei].idCtx # ctx THEN
//    isei � SymbolOps.NextSe[symbols, isei];
//  PutChar['[];
//  FOR isei � isei, SymbolOps.NextSe[symbols, isei] UNTIL isei = Symbols.ISENull DO
//    IF first THEN first � FALSE ELSE out[", "L, cd];
//    IF md THEN GetBitSpec[isei, bitspec];
//    PrintSym[isei, bitspec];
//    PrintDefaultValue[isei, GetValFormat[symbols.seb[isei].idType]];
//    ENDLOOP;
//  PutChar[']];
//  END;
}

//PrintHti: PROCEDURE [hti: Symbols.HTIndex] =
void ShowType::printHti(QString& out, HTIndex* hti);

//PrintSei: PROCEDURE [sei: Symbols.ISEIndex] = {
void ShowType::printSei(QString& out, SEIndex* sei);

//PrintSym: PROCEDURE [sei: Symbols.ISEIndex, colonstring: LONG STRING] =
void ShowType::printSym(QString& out, SEIndex* sei, QString colonString);

//PrintTreeLink: PROCEDURE [tree: Tree.Link, vf: ValFormat, recur: CARDINAL, sonOfDot: BOOLEAN � FALSE] =
void ShowType::printTreeLink(QString& out, TreeLink* tree, ValFormat& vf, int recur, bool sonOfDot);

//PrintType: PROCEDURE [tsei: Symbols.SEIndex, dosub: PROCEDURE] RETURNS [vf: ValFormat] =
static ShowType::ValFormat ShowType::printType(QString& out, SEIndex* tsei /*, Runnable dosub*/);

//PrintTypedVal: PROCEDURE [val: UNSPECIFIED, vf: ValFormat] =
void ShowType::printTypedVal(QString& out, int val, ValFormat& vf);

// PutCurrentModuleDot: PROCEDURE = {

//PutEnum: PROCEDURE [val: UNSPECIFIED, esei: EnumeratedSEIndex] =
void ShowType::putEnum(QString& out, int val, SEIndex* esei);

//PutModeName: PROCEDURE [n: Symbols.TransferMode] =
void ShowType::putModeName(QString& out, SERecord::TransferMode n);

// PutWordSeq: PROCEDURE [seq: LONG DESCRIPTOR FOR ARRAY OF UNSPECIFIED] =
void ShowType::putWordSeq(QString& out, CARD16 length, CARD16* seq);
