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
//

#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("dumpSymbol");

#include "DumpSymbol.h"

#include "ShowType.h"

#include "BCDFile.h"

#include "BTIndex.h"
#include "CTXIndex.h"
#include "ExtIndex.h"
#include "HTIndex.h"
#include "MDIndex.h"
#include "SEIndex.h"

static int indentWidth = 4;
static int indentLevel = 0;
void DumpSymbol::nest() {
	indentLevel++;
}
void DumpSymbol::unnest() {
	indentLevel--;
	if (indentLevel < 0) ERROR();
}
QString DumpSymbol::indent() {
	return QString("%1").arg("", indentWidth * indentLevel);
}

void DumpSymbol::dumpSymbol(QString filePath, QString outDirPath) {
	// Check existence of file
	if (!QFile::exists(filePath)) {
		logger.fatal("File does not exist. pathFile = %s", filePath.toLocal8Bit().constData());
		ERROR();
	}
	BCDFile* file = BCDFile::getInstance(filePath);
	if (!file->isBCDFile()) {
		logger.info("file is no bcd file. pathFile = %s", filePath.toLocal8Bit().constData());
		return;
	}

	// Read bcd file
	BCD* bcd = new BCD(file);

	// Locate target segment
	int symbolBase = -1;
	if (file->isSymbolsFile()) {
		logger.info("file is symbol file. pathFile = %s", filePath.toLocal8Bit().constData());
		symbolBase = 2;
	} else {
	   for (SGRecord* p : bcd->sg.values()) {
			if (p->segClass != SGRecord::SegClass::SYMBOLS)
				continue;
			if (p->file->isSelf()) {
				logger.info("found symbol segment  %s", p->toString().toLocal8Bit().constData());
				symbolBase = p->base;
				break;
			}
		}
	}
	if (symbolBase == -1) {
		logger.fatal("No Symbol Segment");
		ERROR();
	}

	// Read symbols
	Symbols symbols(bcd, symbolBase);

	// Prepare output file
	QString outFilePath;
	{
		QDir outDir(outDirPath);
		if (!outDir.exists()) {
			logger.fatal("outDirPath does not exist. outDirPath = %s", outDir.absolutePath().toLocal8Bit().constData());
			ERROR();
		}

		QString fileName(QFileInfo(filePath).baseName() + ".symbol");
		outFilePath = outDir.absoluteFilePath(fileName);
		logger.info("outFilePath = %s", outFilePath.toLocal8Bit().constData());
	}

	QFile outFile(outFilePath);
	{
		bool result = outFile.open(QIODevice::OpenModeFlag::WriteOnly | QIODevice::OpenModeFlag::Truncate);
		if (!result) {
			logger.fatal("File open error %s", outFile.errorString().toLocal8Bit().constData());
			ERROR();
		}
	}
	QTextStream out(&outFile);

	// Print Header
	{
		MDRecord* self = symbols.md[MDIndex::MD_OWN];
		//FTRecord* self = bcd->ft[FTRecord::FT_SELF];

		QFileInfo info(filePath);

		out << "--" << endl;
		out << QString("-- File   %1 %2").arg(self->stamp->toString()).arg(self->fileId->getValue().value) << endl;
		if (!bcd->sourceFile->isNull()) out << QString("-- Source %1 %2").arg(bcd->sourceFile->version->toString()).arg(bcd->sourceFile->name) << endl;
		out << "--" << endl;
		out << endl;
	}

	// Print Directory
	{
		int mdSize = symbols.md.size();
		if (1 < mdSize) {
			out << "DIRECTORY" << endl;
			nest();
			QListIterator<CARD16> i(symbols.md.keys());
			for(;;) {
				if (!i.hasNext()) {
					break;
				} else {
					CARD16 index = i.next();
					if (index == MDIndex::MD_OWN) continue;
	                MDRecord* e = symbols.md[index];

	                out << indent() << QString("%1 %2%3").arg(e->moduleId->getValue().value, -30).arg(e->stamp->toString()).arg(i.hasNext() ? "," : ";") << endl;
				}
			}
			unnest();
			out << endl;
		}
	}

	// Module body
    {
		MDRecord* self = symbols.md[MDIndex::MD_OWN];

		out << indent() << QString("%1 %2 = BEGIN").arg(self->moduleId->getValue().value).arg(self->stamp->toString()) << endl;
    }
    nest();

    const CTXRecord& outerCtx = symbols.outerCtx->getValue();
    logger.info(QString::asprintf("ctx %3d - %d", outerCtx.getIndex(), outerCtx.level).toLocal8Bit().constData());

    for (const SEIndex* sei = outerCtx.seList; !sei->isNull(); sei = sei->nextSe()) {
    	out << indent();
    	printSym(out, sei, ": ");
    	out << ";" << endl;
    }

    unnest();
    out << "END." << endl;
}

void DumpSymbol::printSym(QTextStream& out, const SEIndex* sei, QString colonString) {
	const SERecord::Id& id = sei->getValue().getId();
	if (!id.hash->isNull()) {
		out << id.hash->getValue().value << colonString;
	}
	if (!id.public_) {
		out << "PRIVATE ";
	}
	if (id.idType->isTypeType()) {
		SEIndex* typeSei = sei->find(id.idInfo);
		out << "TYPE";

		if (typeSei->getValue().tag == SERecord::Tag::CONS) {
			if (typeSei->getValue().getCons().tag != SERecord::Cons::Tag::OPAQUE) {
				out << " = ";
			}
		} else {
			out << " = ";
		}

		ValFormat vf = printType(out, typeSei, {});
		ShowType::printDefaultValue(out, sei, vf);
	} else {
		const SEIndex* typeSei = id.idType;
		if (id.immutable && (!id.constant)) {
			switch (typeSei->xferMode()) {
			case SEIndex::TransferMode::NONE:
			case SEIndex::TransferMode::PROCESS:
				out << "READONLY ";
				break;
			default:
				break;
			}
		}
		ValFormat vf = printType(out, typeSei, {});
		if (id.constant) {
			if (vf.tag == ValFormat::Tag::TRANSFER) {
				out << " = ";
				const BTRecord* bt = BTRecord::find(sei->getSymbols(), id.idInfo);
				if (bt) {
					const Symbols::TransferMode mode = vf.getTransfer().mode;
					if (mode == Symbols::TransferMode::SIGNAL || mode == Symbols::TransferMode::ERROR) {
						out << "CODE";
					} else if (bt->getCallable().inline_) {
						out << "INLINE";
					} else {
						out << "BODY";
					}
				} else if (sei->getValue().getId().extended) {
					out << "OPCODE";
				} else {
					ShowType::printTypedVal(out, sei->getValue().getId().idValue, vf);
				}
			} else {
				out << " = ";
				if (sei->getValue().getId().extended) {
					ExtRecord* ext = ExtRecord::find(sei);
					ShowType::printTreeLink(out, ext->tree, vf, 0, false);
				} else {
					const SEIndex* underTypeSei = typeSei->underType();
					if (underTypeSei->getValue().getCons().tag == SERecord::Cons::Tag::SUBRANGE) {
						ShowType::printTypedVal(out, sei->getValue().getId().idValue + underTypeSei->getValue().getCons().getSubrange().origin, vf);
					} else {
						ShowType::printTypedVal(out, sei->getValue().getId().idValue, vf);
					}
				}
			}
		}
	}
}


QString DumpSymbol::getBitSpec(const SEIndex* isei) {
	const SERecord::Id& t(isei->getValue().getId());

	int a = t.idValue;
	int wd = bitField(a, 0, 11);
	int bd = bitField(a, 12, 15);
	int s = t.idInfo;

	return QString(" (%1%2): ").arg(wd).arg(s ? QString(":%1..%2").arg(bd).arg(bd + s - 1) : "");
}


void DumpSymbol::printFieldCtx(QTextStream& out, const CTXIndex* ctx) {
	const SEIndex* isei = ctx->firstCtxSe();
	bool first = true;
	if ((!isei->isNull()) && (!isei->getValue().getId().idCtx->equals(ctx)))
		isei = isei->nextSe();
	if (isei->isNull()) {
		out << "[]";
		return;
	}
	out << "[" << endl;
	nest();
	for (; !isei->isNull(); isei = isei->nextSe()) {
		if (first)
			first = false;
		else {
			out << "," << endl;
		}
		out << indent();
		printSym(out, isei, getBitSpec(isei));
		printDefaultValue(out, isei, ShowType::getValFormat(isei->getValue().getId().idType));
	}
	out << "]";
	unnest();
}

void DumpSymbol::printDefaultValue(QTextStream& out, const SEIndex* sei, ValFormat vf) {
	ExtRecord* ext = ExtRecord::find(sei);
	if (ext == 0) return;
	if (ext->type != Symbols::ExtensionType::DEFAULT) return;
	out << " = ";
	ShowType::printTreeLink(out, ext->tree, vf, 0);
}

ValFormat DumpSymbol::printType(QTextStream& out, const SEIndex* tsei, std::function<void()> dosub) {
	ValFormat vf = ShowType::getValFormat(tsei);
	switch (tsei->getValue().tag) {
	case SERecord::Tag::ID:
	{
		const SERecord::Id& t(tsei->getValue().getId());
        for (;;) {
            const SEIndex* tseiNext = tsei->typeLink();
            if (tseiNext->isNull())
                break;
            if (tsei->getValue().tag == SERecord::Tag::ID) {
                out << tsei->getValue().getId().hash->getValue().value << " ";
            }
            tsei = tseiNext;
        }

        if (!t.idCtx->isStandardContext()) {
            const CTXRecord& c = t.idCtx->getValue();
            switch (c.tag) {
            case CTXRecord::Tag::INCLUDED:
                out << c.getIncluded().module->getValue().moduleId->getValue().value << ".";
                break;
            case CTXRecord::Tag::SIMPLE:
                /* putCurrentModuleDot(out); */
                break;
            default:
                break;
            }
        }
        out << tsei->getValue().getId().hash->getValue().value;
        if (dosub) dosub();
	}
		break;
	case SERecord::Tag::CONS:
	{
		const SERecord::Cons& cons(tsei->getValue().getCons());
		switch(cons.tag) {
		case SERecord::Cons::Tag::ENUMERATED:
		{
			const SERecord::Cons::Enumerated& t(tsei->getValue().getCons().getEnumerated());

			bool first = true;
            out << "{" << endl;
            nest();
            for (const SEIndex* isei = t.valueCtx->firstCtxSe(); !isei->isNull(); isei = isei->nextSe()) {
                if (first)
                    first = false;
                else {
                	out << "," << endl;
                }
                out << indent();
                const HTIndex* hti = isei->getValue().getId().hash;
                if (!hti->isNull()) out << hti->getValue().value;
                int sv = isei->getValue().getId().idValue;
                out << QString("(%1)").arg(sv);
            }
            out << "}";
            unnest();
		}
			break;
		case SERecord::Cons::Tag::RECORD:
		{
			const SERecord::Cons::Record& t(tsei->getValue().getCons().getRecord());
			if (t.fieldCtx->getValue().level != Symbols::LZ) {
                const CTXIndex* fctx = t.fieldCtx;
                out << "FRAME [";

                for(BTRecord* e: tsei->getBTRecordList()) {
                	if (e->tag == BTRecord::Tag::CALLABLE) {
                		const BTRecord::Callable& callable = e->getCallable();
                		if (e->localCtx->equals(fctx)) {
                			out << callable.id->getValue().getId().hash->getValue().value << "]";
                			break;
                		}
                	}
                }
			}
			else {
                if (t.privateFields) out << "PRIVATE ";
                if (t.monitored) out << "MONITORED ";
                out << "RECORD ";
                printFieldCtx(out, t.fieldCtx);
			}
		}
			break;
		case SERecord::Cons::Tag::REF:
		{
			const SERecord::Cons::Ref& t(tsei->getValue().getCons().getRef());
            if (t.var) {
                out << (t.readOnly ? "READONLY " : "VAR ");
                printType(out, t.refType, {});
            } else {
                if (t.ordered) out << "ORDERED ";
                if (t.basing) out << "BASE ";
                out << "POINTER";
                if (dosub) {
                	out << " ";
                 	dosub();
                }

                const SERecord::Cons& c(t.refType->underType()->getValue().getCons());
                if (c.tag == SERecord::Cons::Tag::BASIC) {
                	if (c.getBasic().code == Symbols::CODE_ANY && (!t.readOnly)) goto noprint;
                }
                out << " TO ";
                if (t.readOnly) out << "READONLY ";
                printType(out, t.refType, {});
            }
noprint:;
		}
			break;
		case SERecord::Cons::Tag::ARRAY:
		{
			const SERecord::Cons::Array& t(tsei->getValue().getCons().getArray());
			if (t.packed) out << "PACKED ";
			out << "ARRAY ";
			printType(out, t.indexType, {});
			out << " OF ";
			printType(out, t.componentType, {});
		}
			break;
		case SERecord::Cons::Tag::ARRAYDESC:
		{
			const SERecord::Cons::ArrayDesc& t(tsei->getValue().getCons().getArrayDesc());
			out << "DESCRIPTOR FOR ";
			if (t.readOnly) out << "READONLY ";
			printType(out, t.describedType, {});
		}
			break;
		case SERecord::Cons::Tag::TRANSFER:
		{
			const SERecord::Cons::Transfer& t(tsei->getValue().getCons().getTransfer());
			ShowType::putModeName(out, t.mode);
			if (!t.typeIn->isNull()) {
				out << " ";
				outArgType(out, t.typeIn);
			}
			if (!t.typeOut->isNull()) {
                out << endl;
                nest();
                out << indent() << "RETURNS";
                outArgType(out, t.typeOut);
                unnest();
			}
		}
			break;
		case SERecord::Cons::Tag::UNION:
		{
			const SERecord::Cons::Union& t(tsei->getValue().getCons().getUnion());
			out << "SELECT ";
			if (!t.controlled) {
				out << (t.overlaid ? "OVERLAID " : "COMPUTED");
			} else {
				out << t.tagSei->getValue().getId().hash->getValue().value;
				out << getBitSpec(t.tagSei);
			}

			const SEIndex* tagType = t.tagSei->getValue().getId().idType;
            if (!t.tagSei->getValue().getId().public_) out << "PRIVATE ";
            switch (tagType->getValue().tag) {
            case SERecord::Tag::ID:
                printType(out, tagType, {});
                break;
            case SERecord::Tag::CONS:
                 out << "*";
                break;
            }
            out << " FROM" << endl;
            nest();
			const SEIndex* temp;
			for(const SEIndex* isei = t.caseCtx->firstCtxSe(); !isei->isNull(); isei = temp) {
				out << indent();
				out << isei->getValue().getId().hash->getValue().value;
				const SEIndex* varRec = isei->find(isei->getValue().getId().idInfo)->underType();



				for(temp = isei->nextSe();; temp = temp->nextSe()) {
					if (temp->isNull()) break;
					if (!isei->equals(temp->getValue().getId().idInfo)) break;
					out << ", " << temp->getValue().getId().hash->getValue().value;
				}
				out << " => ";
				printFieldCtx(out, varRec->getValue().getCons().getRecord().fieldCtx);
				out << "," << endl;
			}
			out << indent() << "ENDCASE";
			unnest();
		}
			break;
		case SERecord::Cons::Tag::RELATIVE:
		{
			const SERecord::Cons::Relative& t(tsei->getValue().getCons().getRelative());
			if (!t.baseType->isNull()) printType(out, t.baseType, {});
			out << " RELATIVE ";
			printType(out, t.offsetType, dosub);
		}
			break;
		case SERecord::Cons::Tag::SEQUENCE:
		{
			const SERecord::Cons::Sequence& t(tsei->getValue().getCons().getSequence());
			if (t.packed) out << "PACKED ";
			out << "SEQUENCE ";
			if (!t.controlled)
				out << "COMPUTED ";
			else {

				out << t.tagSei->getValue().getId().hash->getValue().value;
				out << getBitSpec(t.tagSei);
			}
			const SEIndex* tagType = t.tagSei->getValue().getId().idType;
			if (!t.tagSei->getValue().getId().public_)
				out << "PRIVATE ";
			printType(out, tagType, {});
			out << " OF ";
			printType(out, t.componentType, {});
		}
			break;
		case SERecord::Cons::Tag::SUBRANGE:
		{
			const SERecord::Cons::Subrange& t(tsei->getValue().getCons().getSubrange());
			INT16  org  = t.origin;
			CARD16 size = t.range;
//
			std::function<void()> doit = [&] {
				ValFormat vfSub;
				if (vf.tag == ValFormat::Tag::ENUM) vfSub = vf;
				else if (org < 0) vfSub = ValFormat::SIGNED();
				else vfSub = ValFormat::UNSIGNED();
				out << "[";
				ShowType::printTypedVal(out, org, vfSub);
				out << "..";
				if (t.empty) {
					ShowType::printTypedVal(out, org, vfSub);
					out << ")";
				} else {
					ShowType::printTypedVal(out, org + size, vfSub);
					out << "]";
				}
			};
//
			printType(out, t.rangeType, doit);
		}
			break;
		case SERecord::Cons::Tag::ZONE:
		{
			const SERecord::Cons::Zone& t(tsei->getValue().getCons().getZone());
			if (t.counted) out << "ZONE";
			else if (t.mds) out << "MDSZone";
			else out << "UNCOUNTED ZONE";
		}
			break;
		case SERecord::Cons::Tag::OPAQUE:
		{
			const SERecord::Cons::Opaque& t(tsei->getValue().getCons().getOpaque());
			if (t.lengthKnown) {
				out << "[" << (t.length/16) << "]";
			}
		}
			break;
		case SERecord::Cons::Tag::LONG:
		{
			const SERecord::Cons::Long& t(tsei->getValue().getCons().getLong());
			if (!ShowType::isVar(t.rangeType)) out << "LONG ";
			printType(out, t.rangeType, {});
		}
			break;
		case SERecord::Cons::Tag::REAL:
			out << "REAL";
			break;
		default:
			ERROR();
			break;
		}
	}
		break;
	default:
		ERROR();
		break;
	}
	return vf;
}

void DumpSymbol::outArgType(QTextStream& out, const SEIndex* sei) {
	if (sei->isNull()) out << ": NIL";
	else {
		switch(sei->getValue().getCons().tag) {
		case SERecord::Cons::Tag::RECORD:
			printFieldCtx(out, sei->getValue().getCons().getRecord().fieldCtx);
			break;
		case SERecord::Cons::Tag::ANY:
			out << ": ANY";
			break;
		default:
			break;
		}
	}
}
