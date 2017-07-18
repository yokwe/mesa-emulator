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

#include "BTIndex.h"
#include "CTXIndex.h"
#include "ExtIndex.h"
#include "HTIndex.h"
#include "MDIndex.h"
#include "SEIndex.h"
#include "Tree.h"

//
// ValFormat
//
QString ValFormat::toString(Tag value) {
	TO_STRING_PROLOGUE(Tag)

	MAP_ENTRY(SIGNED)
	MAP_ENTRY(UNSIGNED)
	MAP_ENTRY(CHAR)
	MAP_ENTRY(ENUM)
	MAP_ENTRY(ARRAY)
	MAP_ENTRY(TRANSFER)
	MAP_ENTRY(REF)
	MAP_ENTRY(OTHER)

	TO_STRING_EPILOGUE
}
QString ValFormat::Enum::toString() const {
	return QString("%1").arg(esei->toString());
}
QString ValFormat::Array::toString() const {
	return QString("%1").arg(componentType->toString());
}
QString ValFormat::Transfer::toString() const {
	return QString("%1").arg(ValFormat::toString(mode));
}
const ValFormat::Enum&     ValFormat::getEnum()     const {
	if (tag != Tag::ENUM) ERROR();
	if (tagValue == 0) ERROR();
	Enum* ret = (Enum*)tagValue;
	return *ret;
}
const ValFormat::Array&    ValFormat::getArray()    const {
	if (tag != Tag::ARRAY) ERROR();
	if (tagValue == 0) ERROR();
	Array* ret = (Array*)tagValue;
	return *ret;
}
const ValFormat::Transfer& ValFormat::getTransfer() const {
	if (tag != Tag::TRANSFER) ERROR();
	if (tagValue == 0) ERROR();
	Transfer* ret = (Transfer*)tagValue;
	return *ret;
}
QString ValFormat::toString() const {
	switch(tag) {
	case Tag::ENUM:
		return QString("[%1 %2]").arg(toString(tag)).arg(getEnum().toString());
	case Tag::ARRAY:
		return QString("[%1 %2]").arg(toString(tag)).arg(getArray().toString());
	case Tag::TRANSFER:
		return QString("[%1 %2]").arg(toString(tag)).arg(getTransfer().toString());
	case Tag::SIGNED:
	case Tag::UNSIGNED:
	case Tag::CHAR:
	case Tag::REF:
	case Tag::OTHER:
		return QString("[%1]").arg(toString(tag));
	default:
		ERROR();
		return "???";
	}
}


bool ShowType::defaultPublic = true;
bool ShowType::showBits = true;

//static std::function<void()> nosub = []{};

//PrintSym: PROCEDURE [sei: Symbols.ISEIndex, colonstring: LONG STRING] =
void ShowType::printSym(QTextStream& out, const SEIndex* sei, QString colonString) {
//    BEGIN
//    savePublic: BOOLEAN � defaultPublic;
	bool savePublic = defaultPublic;
	const SERecord::Id id(sei->getValue().getId());
//    typeSei: Symbols.SEIndex;
//    IF symbols.seb[sei].hash # Symbols.HTNull THEN {
	if (!id.hash->isNull()) {
//      PrintSei[sei]; out[colonstring, cd]};
		printSei(out, sei);
		out << colonString;
	}
//    IF symbols.seb[sei].public # defaultPublic THEN {
	if (id.public_ != defaultPublic) {
//      defaultPublic � symbols.seb[sei].public;
		defaultPublic = id.public_;
//      out[IF defaultPublic THEN "PUBLIC "L ELSE "PRIVATE "L, cd]};
		out << (defaultPublic ? "PUBLIC " : "PRIVATE ");
	}
//    IF symbols.seb[sei].idType = Symbols.typeTYPE THEN
	if (id.idType->isTypeType()) {
//      BEGIN
//      vf: ValFormat;
//      typeSei � SymbolOps.ToSei[symbols.seb[sei].idInfo];
		SEIndex* typeSei = sei->find(id.idInfo);
//      out["TYPE"L, cd];
		out << "TYPE";
//      WITH t: symbols.seb[typeSei] SELECT FROM
		switch(typeSei->getValue().tag) {
//        cons => WITH t SELECT FROM
		case SERecord::Tag::CONS:
			switch(typeSei->getValue().getCons().tag) {
//	        opaque => NULL;
			case SERecord::Cons::Tag::OPAQUE:
				break;
//          ENDCASE => out[" = "L, cd];
			default:
				out << " = ";
			}
			break;
		default:
//        ENDCASE => out[" = "L, cd];
			out << " = ";
			break;
		}
//      vf � PrintType[typeSei, NoSub];
	    ValFormat vf = printType(out, typeSei, {});
//      PrintDefaultValue [sei, vf];
	    printDefaultValue(out, sei, vf);
//      END
	}
//    ELSE
	else {
//      BEGIN
//      vf: ValFormat;
//      typeSei � symbols.seb[sei].idType;
		const SERecord::Id& id(sei->getValue().getId());
		const SEIndex* typeSei = id.idType;
//      IF symbols.seb[sei].immutable
//        AND NOT symbols.seb[sei].constant
		if (id.immutable && (!id.constant)) {
//	      AND (SELECT SymbolOps.XferMode[symbols, typeSei] FROM
//	        none, process => TRUE, ENDCASE => FALSE)
//	    --it's not a proc, signal, error, program, or port
//        THEN out["READONLY "L, cd];
			SEIndex::TransferMode mode = typeSei->xferMode();
			switch(mode) {
			case SEIndex::TransferMode::NONE:
			case SEIndex::TransferMode::PROCESS:
				out << "READONLY ";
				break;
			default:
				break;
			}
		}
//      vf � PrintType[typeSei, NoSub];
		ValFormat vf = printType(out, typeSei, {});
//      IF symbols.seb[sei].constant THEN
		if (id.constant) {
//        WITH vf SELECT FROM
			switch(vf.tag) {
//	        transfer =>
			case ValFormat::Tag::TRANSFER:
//	          BEGIN
			{
				vf.getTransfer();
//	          bti: Symbols.CBTIndex = SymbolOps.ToBti[symbols.seb[sei].idInfo];
				BTRecord* bt = BTRecord::find(sei->getSymbols(), id.idInfo);
//	          out[" = "L, cd];
				out << " = ";
//	          SELECT TRUE FROM
//	            bti # Symbols.BTNull =>
				if (bt) {
//	              out[SELECT TRUE FROM
//                  mode = signal, mode = error =>"CODE"L,
					if (vf.getTransfer().mode == ValFormat::TransferMode::SIGNAL) {
						out << "CODE";
					} else if (vf.getTransfer().mode == ValFormat::TransferMode::ERROR) {
						out << "CODE";
//		            symbols.bb[bti].inline => "(INLINE)"L	,
					} else if (bt->getCallable().inline_) {
						out << "(INLINE)";
//		            ENDCASE => "(procedure body)"L, cd];
					} else {
						out << "(procedure body)";
					}
				}
//	            symbols.seb[sei].extended =>
				else if (id.extended) {
//	              out["(MACHINE CODE)"L, cd];
					out << "(MACHINE CODE)";
				}
//	            ENDCASE =>
				else {
//	              PrintTypedVal[symbols.seb[sei].idValue, vf] --a hardcoded constant
					printTypedVal(out, id.idValue, vf);
				}
//	          END;
			}
				break;
//	        ENDCASE =>
			default:
//	          BEGIN
			{
//	          out[" = "L, cd];
				out << " = ";
//	          IF symbols.seb[sei].extended
				if (id.extended) {
//	            THEN PrintTreeLink[SymbolOps.FindExtension[symbols, sei].tree, vf, 0]
					ExtRecord* ext = ExtRecord::find(sei);
					printTreeLink(out, ext->tree, vf, 0);
				}
//	            ELSE
				else {
//	              BEGIN
//	              underTypeSei: Symbols.CSEIndex = SymbolOps.UnderType[symbols, typeSei];
					const SEIndex* underTypeSei = typeSei->underType();
//	              WITH symbols.seb[underTypeSei] SELECT FROM
					switch(underTypeSei->getValue().getCons().tag) {
//	                subrange => PrintTypedVal[SymbolOps.ToCard[symbols.seb[sei].idValue] + origin, vf];
					case SERecord::Cons::Tag::SUBRANGE:
						printTypedVal(out, (id.idValue + underTypeSei->getValue().getCons().getSubrange().origin), vf);
						break;
//		            --normalize subrange
//	                ENDCASE => PrintTypedVal[symbols.seb[sei].idValue, vf];
					default:
						printTypedVal(out, id.idValue, vf);
						break;
//		          END;
					}
				}
//	          END;
			}
				break;
			}
//      END;
		}
	}
//      defaultPublic � savePublic;
	defaultPublic = savePublic;
//    END;
}


//PrintTypedVal: PROCEDURE [val: UNSPECIFIED, vf: ValFormat] =
void ShowType::printTypedVal(QTextStream& out, CARD16 val, ValFormat vf) {
//  BEGIN
//  loophole: BOOLEAN � FALSE;
	bool loophole = false;
//  WITH vf SELECT FROM
	switch(vf.tag) {
//    signed => PutSigned[val];
	case ValFormat::Tag::SIGNED:
		out << (INT16)val;
		break;
//    unsigned => PutUnsigned[val];
	case ValFormat::Tag::UNSIGNED:
		out << val;
		break;
//    char => {
//      Format.Octal[out, val, cd]; PutChar['C]};
	case ValFormat::Tag::CHAR:
		out << QString("%1C").arg(val, 0, 8);
		break;
//    enum => PutEnum[val, esei];
	case ValFormat::Tag::ENUM:
		putEnum(out, val, vf.getEnum().esei);
		break;
//    transfer, ref => IF val = 0 THEN out ["NIL"L, cd] ELSE loophole � TRUE;
	case ValFormat::Tag::TRANSFER:
	case ValFormat::Tag::REF:
		if (val == 0) {
			out << "NIL";
		} else {
			loophole = true;
		}
		break;
//    ENDCASE => loophole � TRUE;
	default:
		loophole = true;
	}
//  IF loophole THEN
//      {out["LOOPHOLE ["L, cd];
//	      PutUnsigned[val];
//	      PutChar[']]};
	if (loophole) {
		out << "LOOPHOLE [" << val << "]";
	}
//  END;
}

//GetBitSpec: PROCEDURE [isei: Symbols.ISEIndex, bitspec: LONG STRING] =
//  BEGIN
//  a: Symbols.BitAddress = SymbolOps.ToBitAddr[symbols.seb[isei].idValue];
//  s: CARDINAL = SymbolOps.ToCard[symbols.seb[isei].idInfo];
//  bitspec.length � 0;
//  String.AppendString[bitspec, " ("L];
//  String.AppendDecimal[bitspec, a.wd];
//  IF s # 0 THEN
//    {String.AppendChar[bitspec, ':];
//    String.AppendDecimal[bitspec, a.bd];
//    String.AppendString[bitspec, ".."L];
//    String.AppendDecimal[bitspec, a.bd+s-1]};
//  String.AppendString[bitspec, "): "L]
//  END;
void ShowType::getBitSpec(const SEIndex* isei, QString& bitspec) {
	CARD16 a = isei->getValue().getId().idValue;
	CARD16 s = isei->getValue().getId().idInfo;

	CARD16 wd = a / 16;
	CARD16 bd = a % 16;

	bitspec.clear();
	bitspec.append(QString(" (%1").arg(wd));
	if (s) {
		bitspec.append(QString(":%1..%2")).arg(bd).arg(bd + s - 1);
	}
	bitspec.append("): ");
}


//OutArgType: PROCEDURE [sei: Symbols.CSEIndex] = {
//  IF sei = Symbols.SENull THEN out[": NIL"L, cd]
//  ELSE
//    WITH t: symbols.seb[sei] SELECT FROM
//      record => PrintFieldCtx[t.fieldCtx];
//	    any => out[": ANY"L, cd];
//	ENDCASE};
void ShowType::outArgType(QTextStream& out, const SEIndex* sei) {
	if (sei->isNull()) out << ": NIL";
	else {
		switch(sei->getValue().getCons().tag) {
		case SERecord::Cons::Tag::RECORD:
			printFieldCtx(out, sei->getValue().getCons().getRecord().fieldCtx);
			break;
		case SERecord::Cons::Tag::ANY:
			out << ": ANY";
		default:
			break;
		}
	}
}


//PrintFieldCtx: PROCEDURE [ctx: Symbols.CTXIndex, md: BOOLEAN � FALSE] =
//  BEGIN
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
void ShowType::printFieldCtx(QTextStream& out, const CTXIndex* ctx, bool md) {
	const SEIndex* isei = ctx->firstCtxSe();
	QString bitspec;
	bool first = true;
	if (!md) bitspec.append(": ");
	if (!(isei->isNull()) && !(isei->getValue().getId().idCtx->equals(ctx))) {
		isei = isei->nextSe();
	}
	out << "[";
	for(; !isei->isNull(); isei = isei->nextSe()) {
		if (first) {
			first = false;
		} else {
			out << ", ";
		}
		if (md) getBitSpec(isei, bitspec);
		printSym(out, isei, bitspec);
		printDefaultValue(out, isei, getValFormat(isei->getValue().getId().idType));
	}
	out << "]";
}


//PrintHti: PROCEDURE [hti: Symbols.HTIndex] =
//  BEGIN
//  desc: String.SubStringDescriptor;
//  s: String.SubString = @desc;
//  IF hti = Symbols.HTNull THEN out["(anonymous)"L, cd]
//  ELSE {
//    SymbolOps.SubStringForHash[symbols, s, hti]; Format.SubString[out, s, cd]};
//  RETURN
//  END;
void ShowType::printHti(QTextStream& out, const HTIndex* hti) {
	if (hti->isNull()) out << "(anonymous)";
	else out << hti->getValue().value;
}


//PrintSei: PROCEDURE [sei: Symbols.ISEIndex] = {
//  PrintHti[
//    IF sei = Symbols.SENull
//    THEN Symbols.HTNull
//    ELSE symbols.seb[sei].hash]};
void ShowType::printSei(QTextStream& out, const SEIndex* sei) {
	printHti(out, sei->isNull() ? HTIndex::getNull() : sei->getValue().getId().hash);
}


//EnumeratedSEIndex: TYPE = Table.Base RELATIVE POINTER [0..Table.Limit)
//  TO enumerated cons Symbols.SERecord;
//PutEnum: PROCEDURE [val: UNSPECIFIED, esei: EnumeratedSEIndex] =
void ShowType::putEnum(QTextStream& out, CARD16 val, const SEIndex* esei) {
//  BEGIN
//  FOR sei: Symbols.ISEIndex �
//    SymbolOps.FirstCtxSe[symbols, symbols.seb[esei].valueCtx], SymbolOps.NextSe[symbols, sei]
//    WHILE sei # Symbols.ISENull DO
	for(const SEIndex* sei = esei->getValue().getCons().getEnumerated().valueCtx->firstCtxSe(); !sei->isNull(); sei = sei->nextSe()) {
//    IF symbols.seb[sei].idValue = val THEN {PrintSei[sei]; RETURN};
		if (sei->getValue().getId().idValue == val) {
			ShowType::printSei(out, sei);
			return;
		}
		//    ENDLOOP;
	}
//  out["LOOPHOLE ["L, cd];
//  PutUnsigned[val]; PutChar[']];
	out << "LOOPHOLE [" << val << "]";
//  END;
}


//GetValFormat: PROCEDURE [tsei: Symbols.SEIndex] RETURNS [vf: ValFormat] =
//  BEGIN
ValFormat ShowType::getValFormat(const SEIndex* tsei) {
//  WITH t: symbols.seb[tsei] SELECT FROM
	switch(tsei->getValue().tag) {
//    id => RETURN[GetValFormat[SymbolOps.UnderType[symbols, tsei]]];
	case SERecord::Tag::ID:
		return getValFormat(tsei->underType());
//    cons =>
	case SERecord::Tag::CONS:
	{
//	    WITH t SELECT FROM
		const SERecord::Cons& cons(tsei->getValue().getCons());
		switch(tsei->getValue().getCons().tag) {
//	      basic =>
		case SERecord::Cons::Tag::BASIC:
//	        SELECT code FROM
			switch (cons.getBasic().code) {
//	          Symbols.codeANY => vf � [unsigned[]];
			case Symbols::CODE_ANY:
				return ValFormat::UNSIGNED();
//    	      Symbols.codeINT => vf � [signed[]];
			case Symbols::CODE_INT:
				return ValFormat::SIGNED();
//	          Symbols.codeCHAR => vf � [char[]];
			case Symbols::CODE_CHAR:
				return ValFormat::CHAR();
//	          ENDCASE;
			default:
				ERROR();
			}
//	      enumerated => vf � [enum[LOOPHOLE[tsei]]];
		case SERecord::Cons::Tag::ENUMERATED:
			return ValFormat::ENUM(tsei);
//	      array => vf � [array[componentType]];
		case SERecord::Cons::Tag::ARRAY:
			return ValFormat::ARRAY(cons.getArray().componentType);
//	      transfer => vf � [transfer[mode]];
		case SERecord::Cons::Tag::TRANSFER:
			return ValFormat::TRASNFER(cons.getTransfer().mode);
//	      relative => vf � GetValFormat[offsetType];
		case SERecord::Cons::Tag::RELATIVE:
			return getValFormat(cons.getRelative().offsetType);
//	      subrange =>
		case SERecord::Cons::Tag::SUBRANGE:
		{
//	         {vf � GetValFormat[rangeType];
			ValFormat vf = getValFormat(cons.getSubrange().rangeType);
//	         IF vf.tag = signed AND origin = 0 THEN vf � [unsigned[]]};
			if (vf.tag == ValFormat::Tag::SIGNED && cons.getSubrange().origin == 0) {
				return ValFormat::UNSIGNED();
			} else {
				return vf;
			}
		}
			break;
//	      long => vf � GetValFormat[rangeType];
		case SERecord::Cons::Tag::LONG:
			return getValFormat(tsei->getValue().getCons().getLong().rangeType);
//	      ref => vf � [ref[]];
		case SERecord::Cons::Tag::REF:
			return ValFormat::REF();
//	      ENDCASE => vf � [other[]];
		default:
			return ValFormat::OTHER();
		}
	}
		break;
//     ENDCASE => vf � [other[]];
	default:
		return ValFormat::OTHER();
//   END;
	}
}


//PrintType: PROCEDURE [tsei: Symbols.SEIndex, dosub: PROCEDURE] RETURNS [vf: ValFormat] =
ValFormat ShowType::printType(QTextStream& out, const SEIndex* tsei, std::function<void()> dosub) {
//  BEGIN
//  bitspec: LONG STRING = [20];
//  vf � GetValFormat[tsei];
	ValFormat vf = getValFormat(tsei);
//  WITH t: symbols.seb[tsei] SELECT FROM
	switch (tsei->getValue().tag) {
//    id =>
	case SERecord::Tag::ID:
	{
		const SERecord::Id& t(tsei->getValue().getId());
//  	BEGIN
//	    --print adjectives, if any
//      tseiNext: Symbols.SEIndex;
//    	UNTIL (tseiNext � SymbolOps.TypeLink[symbols, tsei]) = Symbols.SENull DO
		for(;;) {
			const SEIndex* tseiNext = tsei->typeLink();
			if (tseiNext->isNull()) break;
//    	  WITH symbols.seb[tsei] SELECT FROM
			switch(tsei->getValue().tag) {
//    	    id => {PrintSei[LOOPHOLE[tsei]]; PutBlanks[1]};
			case SERecord::Tag::ID:
				printSei(out, tsei);
				out << " ";
				break;
//    	    ENDCASE;
			default:
				break;
			}
//    	  tsei � tseiNext;
			tsei = tseiNext;
//    	  ENDLOOP;
		}
//
//    	--print module qualification of last ID in chain
//    	IF t.idCtx NOT IN Symbols.StandardContext THEN
		if (!t.idCtx->isStandardContext()) {
//    	  WITH c: symbols.ctxb[t.idCtx] SELECT FROM
			const CTXRecord& c(t.idCtx->getValue());
			switch(t.idCtx->getValue().tag) {
//    	    included =>
			case CTXRecord::Tag::INCLUDED:
			{
//    	      BEGIN
//    	      hti: Symbols.HTIndex = symbols.mdb[c.module].moduleId;
				const HTIndex* hti = c.getIncluded().module->getValue().moduleId;
//    	      PrintHti[hti]; --interface name
				printHti(out, hti);
//    	      PutChar['.]; -- dot qualification
				out << ".";
//    	      END;
			}
				break;
//    	    simple =>
			case CTXRecord::Tag::SIMPLE:
//    	      PutCurrentModuleDot[];
				break;
//    	    ENDCASE;
			default:
				break;
			}
		}
//
//    	--finally print that last ID
//    	PrintSei[LOOPHOLE[tsei]];
		printSei(out, tsei);
//    	dosub[];
		if (dosub) dosub();
//    	END;
	}
		break;
//
//    cons =>
	case SERecord::Tag::CONS:
	{
		const SERecord::Cons& cons(tsei->getValue().getCons());
//    	WITH t SELECT FROM
		switch(cons.tag) {
//    	  --basic =>  won't see one, see the id first.
//    	  enumerated =>
		case SERecord::Cons::Tag::ENUMERATED:
		{
//    	    BEGIN
			const SERecord::Cons::Enumerated& t(tsei->getValue().getCons().getEnumerated());

//    	    isei: Symbols.ISEIndex;
//    	    v: CARDINAL � 0;
			CARD16 v = 0;
//    	    sv: CARDINAL;
//    	    first: BOOLEAN � TRUE;
			bool first = true;
//    	    IF machineDep THEN out["MACHINE DEPENDENT "L, cd];
			if (t.machineDep) out << "MACHINE DEPENDENT ";
//    	    PutChar['{];
			out << "{";
//    	    FOR isei � SymbolOps.FirstCtxSe[symbols, valueCtx], SymbolOps.NextSe[symbols, isei]
//    	      UNTIL isei = Symbols.ISENull DO
			for(const SEIndex* isei = t.valueCtx->firstCtxSe(); !isei->isNull(); isei = isei->nextSe()) {
//    	      IF first THEN first � FALSE ELSE out[", "L, cd];
				if (first) {
					first = false;
				} else {
					out << ", ";
				}
//    	      IF machineDep OR showBits THEN {
				if (t.machineDep || showBits) {
//    	        hti: Symbols.HTIndex = symbols.seb[isei].hash;
					const HTIndex* hti = isei->getValue().getId().hash;
//    		    sv � SymbolOps.ToCard[symbols.seb[isei].idValue];
					CARD16 sv = isei->getValue().getId().idValue;
//    	    	IF hti # Symbols.HTNull THEN PrintSei[isei];
					if (!hti->isNull()) printSei(out, isei);
//    		    IF sv # v OR hti = Symbols.HTNull OR showBits
					if (sv != v || hti->isNull() || showBits) {
//    		       THEN {
//    		         PutChar['(];
//    		         PutUnsigned[sv];
//    		         PutChar[')]};
						out << "(" << sv << ")";
					}
//    		v � sv + 1}
					v = sv + 1;
				}
//    	      ELSE PrintSei[isei];
				else printSei(out, isei);
//    	      IF Abort[] THEN {
//    	        PutLine["  ...aborted}"L]; RETURN};
//    	      ENDLOOP;
			}
//    	    PutChar['}];
			out << "}";
//    	    END;
		}
			break;
//    	  record =>
		case SERecord::Cons::Tag::RECORD:
		{
			const SERecord::Cons::Record& t(tsei->getValue().getCons().getRecord());
//    	    BEGIN
//    	    IF symbols.ctxb[fieldCtx].level # Symbols.lZ THEN
			if (t.fieldCtx->getValue().level != Symbols::LZ) {
//    	      BEGIN
//    	      fctx: Symbols.CTXIndex = fieldCtx;
				const CTXIndex* fctx = t.fieldCtx;
//    	      bti: Symbols.BTIndex � FIRST[Symbols.BTIndex];
//    	      btlimit: Symbols.BTIndex = bti + symbols.stHandle.bodyBlock.size;
//    	      out["FRAME ["L, cd];
				out << "FRAME [";
//    	      UNTIL LOOPHOLE[bti, CARDINAL] >= LOOPHOLE[btlimit, CARDINAL] DO
				for(BTRecord* e: tsei->getBTRecordList()) {
//    		    WITH entry: symbols.bb[bti] SELECT FROM
//    		      Callable =>
					if (e->tag == BTRecord::Tag::CALLABLE) {
//    		        BEGIN
//    		        IF entry.localCtx = fctx THEN {
						if (e->localCtx == fctx) {
//    		          PrintSei[entry.id]; PutChar[']]; EXIT};
							printSei(out, e->getCallable().id);
							out << "]";
							break;
						}
//    		        bti �
//    	    	      bti +
//    			      (WITH entry SELECT FROM
//    		  	         Inner => SIZE[Inner Callable Symbols.BodyRecord],
//    			         Catch => SIZE[Catch Callable Symbols.BodyRecord],
//    			         ENDCASE => SIZE[Outer Callable Symbols.BodyRecord]);
//    		        END;
//    		      ENDCASE => bti � bti + SIZE[Other Symbols.BodyRecord];
					}
//    		    ENDLOOP;
				}
//    	      END
			}
//    	    ELSE
			else {
//    	      BEGIN
//    	      dp: BOOLEAN � defaultPublic;
				bool dp = defaultPublic;
//    	      IF defaultPublic AND hints.privateFields THEN {
//    	        out["PRIVATE "L, cd];
//    		    defaultPublic � FALSE};
				if (defaultPublic && t.privateFields) {
					out << "PRIVATE ";
					defaultPublic = false;
				}
//    	      IF monitored THEN out["MONITORED "L, cd];
				if (t.monitored) out << "MONITORED ";
//    	      IF machineDep THEN out["MACHINE DEPENDENT "L, cd];
				if (t.machineDep) out << "MACHINE DEPENDENT ";
//    	      out["RECORD "L, cd];
				out << "RECORD ";
//    	      PrintFieldCtx[fieldCtx, machineDep OR showBits];
				printFieldCtx(out, t.fieldCtx, t.machineDep || showBits);
//    	      defaultPublic � dp;
				defaultPublic = dp;
//    	      END;
			}
//    	    END;
		}
			break;
//    	  ref =>
		case SERecord::Cons::Tag::REF:
		{
			const SERecord::Cons::Ref& t(tsei->getValue().getCons().getRef());
//    	    BEGIN
//    	    IF var
			if (t.var) {
//    	      THEN IF readOnly
//    	        THEN out ["READONLY "L, cd]
//    		    ELSE out ["VAR "L, cd]
				if (t.readOnly) out << "READONLY ";
				else out << "VAR ";
			} else {
//    	      ELSE
//    	        BEGIN
//    		    IF ordered THEN out["ORDERED "L, cd];
				if (t.ordered) out << "ORDERED ";
//    		    IF basing THEN out["BASE "L, cd];
				if (t.basing) out << "BASE ";
//    		    out["POINTER"L, cd];
				out << "POINTER";
//    		    IF dosub # NoSub THEN {PutBlanks[1]; dosub[]};
				if (dosub) {
					out << " ";
					dosub();
				}
//    		    WITH symbols.seb[SymbolOps.UnderType[symbols, refType]] SELECT FROM
//    	           basic => IF code = Symbols.codeANY AND ~readOnly THEN
//    	             GO TO noprint;
				if (t.refType->underType()->getValue().getCons().tag == SERecord::Cons::Tag::BASIC) {
					const SERecord::Cons::Basic& basic(t.refType->underType()->getValue().getCons().getBasic());
					if (basic.code == Symbols::CODE_ANY && t.readOnly) goto noprint;
				}
//    	           ENDCASE;
//    	        out[" TO "L, cd];
				out << " TO ";
//    	        IF readOnly THEN out["READONLY "L, cd];
				if (t.readOnly) out << "READONLY ";
//    		    END;
			}
//    	    [] � PrintType[refType, NoSub];
			printType(out, t.refType, {});
//    	    EXITS noprint => NULL;
noprint:;
//    	    END;
		}
			break;
//    	  array =>
		case SERecord::Cons::Tag::ARRAY:
		{
			const SERecord::Cons::Array& t(tsei->getValue().getCons().getArray());
//    	    BEGIN
//    	    IF packed THEN out["PACKED "L, cd];
			if (t.packed) out << "PACKED ";
//    	    out["ARRAY "L, cd];
			out << "ARRAY ";
//    	    [] � PrintType[indexType, NoSub];
			printType(out, t.indexType, {});
//    	    out[" OF "L, cd];
			out << " OF ";
//    	    [] � PrintType[componentType, NoSub];
			printType(out, t.componentType, {});
//    	    END;
		}
			break;
//    	  arraydesc =>
		case SERecord::Cons::Tag::ARRAYDESC:
		{
			const SERecord::Cons::ArrayDesc& t(tsei->getValue().getCons().getArrayDesc());
//    	    BEGIN
//    	    out["DESCRIPTOR FOR "L, cd];
			out << "DESCRIPTOR FOR ";
//    	    IF readOnly THEN out["READONLY "L, cd];
			if (t.readOnly) out << "READONLY ";
//    	    [] � PrintType[describedType, NoSub];
			printType(out, t.describedType, {});
//    	    END;
		}
			break;
//    	  transfer =>
		case SERecord::Cons::Tag::TRANSFER:
		{
			const SERecord::Cons::Transfer& t(tsei->getValue().getCons().getTransfer());
//    	    BEGIN
//    	    PutModeName[mode];
			putModeName(out, t.mode);
//    	    IF typeIn # Symbols.RecordSENull THEN {
//    	      PutBlanks[1]; OutArgType[typeIn]};
			if (!t.typeIn->isNull()) {
				out << " ";
				outArgType(out, t.typeIn);
			}
//    	    IF typeOut # Symbols.RecordSENull THEN
//    	      BEGIN
//    	      out[" RETURNS "L, cd];
//    	      OutArgType[typeOut];
//    	      END;
			if (!t.typeOut->isNull()) {
				out << " RETURNS ";
				outArgType(out, t.typeOut);
			}
//    	    END;
		}
			break;
//    	  union =>
		case SERecord::Cons::Tag::UNION:
		{
			const SERecord::Cons::Union& t(tsei->getValue().getCons().getUnion());
//    	    BEGIN
//    	    tagType: Symbols.SEIndex;
//    	    NewLine: PROCEDURE = {
//    	      Format.CR[out, cd];
//    	      PutBlanks[indent]};
//    	    indent � indent + 2;
//    	    out["SELECT "L, cd];
			out << "SELECT ";
//    	    IF ~controlled THEN
			if (!t.controlled) {
//    	      IF overlaid THEN out["OVERLAID "L, cd]
//    	      ELSE out["COMPUTED "L, cd]
				out << (t.overlaid ? "OVERLAID " : "COMPUTED");
//    	    ELSE
			} else {
//    	      BEGIN
//    	      PrintSei[tagSei];
				printSei(out, t.tagSei);
//    	      IF machineDep OR showBits THEN {
//    	        GetBitSpec[tagSei, bitspec]; out[bitspec, cd]}
//    	      ELSE out[": "L, cd];
				if (t.machineDep || showBits) {
					QString bitspec;
					getBitSpec(t.tagSei, bitspec);
					out << bitspec;
				} else {
					out << ": ";
				}
//    	      END;
			}
//    	    tagType � symbols.seb[tagSei].idType;
			const SEIndex* tagType = t.tagSei->getValue().getId().idType;
//    	    IF symbols.seb[tagSei].public # defaultPublic THEN
//    	      out[IF defaultPublic THEN "PRIVATE "L ELSE "PUBLIC "L, cd];
			if (t.tagSei->getValue().getId().public_ != defaultPublic) {
				out << (defaultPublic ? "PRIVATE " : "PUBLIC ");
			}
//    	    WITH symbols.seb[tagType] SELECT FROM
//    	      id => [] � PrintType[tagType, NoSub];
//    	      cons => PutChar['*];
//    	      ENDCASE;
			switch(tagType->getValue().tag) {
			case SERecord::Tag::ID:
				printType(out, tagType, {});
				break;
			case SERecord::Tag::CONS:
				out << "*";
				break;
			default:
				ERROR();
			}
//    	    out[" FROM "L, cd];
			out << " FROM ";
//    	    BEGIN
//    	    temp, isei: Symbols.ISEIndex;
//    	    varRec: Symbols.RecordSEIndex;
//    	    FOR isei � SymbolOps.FirstCtxSe[symbols, caseCtx], temp
//    	      UNTIL isei = Symbols.ISENull DO
			for(const SEIndex* isei = t.caseCtx->firstCtxSe(); !isei->isNull();) {
//    	      NewLine[];
				out << endl;
//    	      PrintSei[isei];
				printSei(out, isei);
//    	      varRec � LOOPHOLE[SymbolOps.UnderType[symbols, SymbolOps.ToSei[symbols.seb[isei].idInfo]]];
				const SEIndex* varRec = isei->find(isei->getValue().getId().idInfo)->underType();
//    	      FOR temp � SymbolOps.NextSe[symbols, isei], SymbolOps.NextSe[symbols, temp]
				const SEIndex* temp = isei->nextSe();
				for(;;) {
//    		  UNTIL temp = Symbols.ISENull
//    		    OR SymbolOps.ToSei[symbols.seb[temp].idInfo] # isei DO
					if (temp->isNull()) break;
					if (isei->equals(temp->getValue().getId().idInfo)) break;
//    	        out[", "L, cd];
					out << ", ";
//    	        PrintSei[temp];
					printSei(out, temp);
//    	        ENDLOOP
				}
				isei = temp;
//    	      out[" => "L, cd];
				out << " => ";
//    	      PrintFieldCtx[symbols.seb[varRec].fieldCtx, machineDep OR showBits];
				printFieldCtx(out, varRec->getValue().getCons().getRecord().fieldCtx, t.machineDep || showBits);
//    	      PutChar[',]; --comma
				out << ",";
//    	      ENDLOOP;
			}
//    	    NewLine[];
			out << endl;
//    	    out["ENDCASE"L, cd];
			out << "ENDCASE";
//    	    indent � indent - 2;
//    	    END;
//    	    END;
		}
			break;
//    	  relative =>
		case SERecord::Cons::Tag::RELATIVE:
		{
			const SERecord::Cons::Relative& t(tsei->getValue().getCons().getRelative());
//    	    BEGIN
//    	    IF baseType # Symbols.SENull THEN [] � PrintType[baseType, NoSub];
			if (!t.baseType->isNull()) printType(out, t.baseType, {});
//    	    out[" RELATIVE "L, cd];
			out << " RELATIVE ";
//    	    [] � PrintType[offsetType, dosub];
			printType(out, t.offsetType, dosub);
//    	    END;
		}
			break;
//    	  sequence =>
		case SERecord::Cons::Tag::SEQUENCE:
		{
			const SERecord::Cons::Sequence& t(tsei->getValue().getCons().getSequence());
//    	    BEGIN
//    	    tagType: Symbols.SEIndex;
//    	    IF packed THEN out["PACKED "L, cd];
			if (t.packed) out << "PACKED ";
//    	    out["SEQUENCE "L, cd];
			out << "SEQUENCE ";
//    	    IF ~controlled THEN out["COMPUTED "L, cd]
			if (!t.controlled) out << "COMPUTED ";
//    	    ELSE
			else {
//    	      BEGIN
//    	      PrintSei[tagSei];
				printSei(out, t.tagSei);
//    	      IF machineDep OR showBits THEN {
//    	        GetBitSpec[tagSei, bitspec]; out[bitspec, cd]}
				if (t.machineDep || showBits) {
					QString bitspec;
					getBitSpec(t.tagSei, bitspec);
					out << bitspec;
				}
//    	      ELSE out[": "L, cd];
				else out << ": ";
//    	      END;
			}
//    	    tagType � symbols.seb[tagSei].idType;
			const SEIndex* tagType = t.tagSei->getValue().getId().idType;
//    	    IF symbols.seb[tagSei].public # defaultPublic THEN
			if (t.tagSei->getValue().getId().public_ != defaultPublic)
//    	      out[IF defaultPublic THEN "PRIVATE "L ELSE "PUBLIC "L, cd];
				out << (defaultPublic ? "PRIVATE " : "PUBLIC ");
//    	    [] � PrintType[tagType, NoSub];
			printType(out, tagType, {});
//    	    out[" OF "L, cd];
			out << " OF ";
//    	    [] � PrintType[componentType, NoSub];
			printType(out, t.componentType, {});
//    	    END;
		}
			break;
//    	  subrange =>
		case SERecord::Cons::Tag::SUBRANGE:
		{
			const SERecord::Cons::Subrange& t(tsei->getValue().getCons().getSubrange());
//    	    BEGIN
//    	    org: INTEGER � origin;
//    	    size: CARDINAL � range;
			INT16  org  = t.origin;
			CARD16 size = t.range;
//
//    	    doit: PROCEDURE =
//    	      BEGIN
//              vfSub: ValFormat = SELECT TRUE FROM
//                vf.tag = enum => vf,
//    	          org < 0 => [signed[]]
//    	          ENDCASE => [unsigned[]];
//    	        PutChar['[];
//    	        PrintTypedVal[org, vfSub];
//    	        out[".."L, cd];
//    	        IF empty THEN {
//    		      PrintTypedVal[org, vfSub]; PutChar[')]}
//    	        ELSE {
//    	          PrintTypedVal[org + size, vfSub]; PutChar[']]};
//    	      END;
			std::function<void()> doit = [&] {
				ValFormat vfSub;
				if (vf.tag == ValFormat::Tag::ENUM) vfSub = vf;
				else if (org < 0) vfSub = ValFormat::SIGNED();
				else vfSub = ValFormat::UNSIGNED();
				out << "[";
				printTypedVal(out, org, vfSub);
				out << "..";
				if (t.empty) {
					printTypedVal(out, org, vfSub);
					out << ")";
				} else {
					printTypedVal(out, org + size, vfSub);
					out << "]";
				}
			};
//
//    	    [] � PrintType[rangeType, doit];
			printType(out, t.rangeType, doit);
//    	    END;
		}
			break;
//    	  zone => SELECT TRUE FROM
		case SERecord::Cons::Tag::ZONE:
		{
			const SERecord::Cons::Zone& t(tsei->getValue().getCons().getZone());
//    	    counted => out["ZONE"L, cd];
			if (t.counted) out << "ZONE";
//    	    mds => out["MDSZone"L, cd];
			else if (t.mds) out << "MDSZone";
//    	    ENDCASE => out["UNCOUNTED ZONE"L, cd];
			else out << "UNCOUNTED ZONE";
		}
			break;
//    	  opaque => {
		case SERecord::Cons::Tag::OPAQUE:
		{
			const SERecord::Cons::Opaque& t(tsei->getValue().getCons().getOpaque());
//    	    IF lengthKnown THEN {
			if (t.lengthKnown) {
//    	      PutChar['[];
//    	      PutUnsigned[length/Environment.bitsPerWord];

//    	      PutChar[']]}};
				out << "[" << (t.length/16) << "]";
			}
		}
			break;
//    	  long =>
		case SERecord::Cons::Tag::LONG:
		{
			const SERecord::Cons::Long& t(tsei->getValue().getCons().getLong());
//    	    {IF NOT IsVar [rangeType] THEN out["LONG "L, cd];
			if (!isVar(t.rangeType)) out << "LONG ";
//    	    [] � PrintType[rangeType, NoSub]};
			printType(out, t.rangeType, {});
		}
			break;
//    	  real => out["REAL"L, cd];
		case SERecord::Cons::Tag::REAL:
			out << "REAL";
			break;
//    	  ENDCASE => out["(? unknown TYPE)"L, cd];
		default:
			ERROR();
			break;
		}
	}
		break;
//    ENDCASE;
	default:
		ERROR();
		break;
	}
//  END;
	return vf;
}



//IsVar: PROC [tsei: Symbols.SEIndex] RETURNS [BOOLEAN] =
//  BEGIN
//  WITH t: symbols.seb[tsei] SELECT FROM
//    id => RETURN [FALSE];
//    cons => WITH t2: t SELECT FROM
//      ref => RETURN [t2.var]
//	    ENDCASE => RETURN [FALSE];
//    ENDCASE => RETURN [FALSE];
//  END;
bool ShowType::isVar(const SEIndex* tsei) {
	switch(tsei->getValue().tag) {
	case SERecord::Tag::ID:
		return false;
	case SERecord::Tag::CONS:
		switch(tsei->getValue().getCons().tag) {
		case SERecord::Cons::Tag::REF:
			return tsei->getValue().getCons().getRef().var;
		default:
			return false;
		}
		break;
	default:
		return false;
	}
}

//PutModeName: PROCEDURE [n: Symbols.TransferMode] =
//  BEGIN
//  ModePrintName: ARRAY Symbols.TransferMode OF LONG STRING =
//    ["PROCEDURE"L, "PORT"L, "SIGNAL"L, "ERROR"L, "PROCESS"L, "PROGRAM"L,
//	"NONE"L];
//  out[ModePrintName[n], cd];
//  END;
void ShowType::putModeName(QTextStream& out, Symbols::TransferMode n) {
	switch(n) {
	case Symbols::TransferMode::PROC:
		out << "PROCEDURE";
		break;
	case Symbols::TransferMode::PORT:
		out << "PORT";
		break;
	case Symbols::TransferMode::SIGNAL:
		out << "SIGNAL";
		break;
	case Symbols::TransferMode::ERROR:
		out << "ERROR";
		break;
	case Symbols::TransferMode::PROCESS:
		out << "PROCESS";
		break;
	case Symbols::TransferMode::PROGRAM:
		out << "PROGRAM";
		break;
	case Symbols::TransferMode::NONE:
		out << "NONE";
		break;
	default:
		ERROR();
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
void ShowType::printDefaultValue(QTextStream& out, const SEIndex* sei, ValFormat vf) {
	ExtRecord* ext = ExtRecord::find(sei);
	if (ext == 0) ERROR();
	if (ext->type != ExtRecord::ExtensionType::DEFAULT) return;
	out << " = ";
	printTreeLink(out, ext->tree, vf, 0);
}


//PrintTreeLink: PROCEDURE [tree: Tree.Link, vf: ValFormat, recur: CARDINAL, sonOfDot: BOOLEAN � FALSE] =
// TODO void ShowType::printTreeLink(QTextStream& out, const TreeLink* tree, ValFormat vf, int recur, bool sonOfDot = false);


//PutWordSeq: PROCEDURE [seq: LONG DESCRIPTOR FOR ARRAY OF UNSPECIFIED] =
//  BEGIN
//  PutChar['[];
//  FOR i: CARDINAL IN [0..LENGTH[seq]-1) DO
//    PutUnsigned[seq[i]];
//    out[", "L, cd];
//    ENDLOOP;
//  PutUnsigned[seq[LENGTH[seq]-1]];
//  PutChar[']];
//  END;
void ShowType::putWordSeq(QTextStream& out, CARD16 length, CARD16* value) {
	out << "[";
	for(CARD16 i = 0; i < length; i++) {
		if (i) out << ", ";
		out << value[i];
	}
	out << "]";
}




