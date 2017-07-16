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
#include "ExtIndex.h"
#include "HTIndex.h"
#include "SEIndex.h"
#include "Tree.h"

//
// ValFormat
//
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
	    ValFormat vf = printType(out, typeSei, 0);
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
		ValFormat vf = printType(out, typeSei);
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
















