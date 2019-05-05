// Generated from src-java/mesa/courier/antlr/Courier.g by ANTLR 4.1
package mesa.courier.antlr;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class CourierParser extends Parser {
	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__13=1, T__12=2, T__11=3, T__10=4, T__9=5, T__8=6, T__7=7, T__6=8, T__5=9, 
		T__4=10, T__3=11, T__2=12, T__1=13, T__0=14, ARRAY=15, BEGIN=16, BLOCK=17, 
		BOOLEAN=18, BYTE=19, CARDINAL=20, CHOICE=21, DEPENDS=22, DEPENDENT=23, 
		END=24, ERROR=25, INTEGER=26, LONG=27, MACHINE=28, OF=29, PROCEDURE=30, 
		PROGRAM=31, RECORD=32, REPORTS=33, RETURNS=34, SEQUENCE=35, STRING=36, 
		TYPE=37, UNSPECIFIED=38, UNSPECIFIED2=39, UNSPECIFIED3=40, UPON=41, VERSION=42, 
		TRUE=43, FALSE=44, ID=45, NUMBER=46, STR=47, COMMENT_PARTIAL=48, COMMENT_LINE=49, 
		SPACE=50;
	public static final String[] tokenNames = {
		"<INVALID>", "':'", "'['", "'{'", "';'", "'..'", "']'", "'}'", "'='", 
		"'=>'", "'('", "')'", "','", "'-'", "'.'", "'ARRAY'", "'BEGIN'", "'BLOCK'", 
		"'BOOLEAN'", "'BYTE'", "'CARDINAL'", "'CHOICE'", "'DEPENDS'", "'DEPENDENT'", 
		"'END'", "'ERROR'", "'INTEGER'", "'LONG'", "'MACHINE'", "'OF'", "'PROCEDURE'", 
		"'PROGRAM'", "'RECORD'", "'REPORTS'", "'RETURNS'", "'SEQUENCE'", "'STRING'", 
		"'TYPE'", "'UNSPECIFIED'", "'UNSPECIFIED2'", "'UNSPECIFIED3'", "'UPON'", 
		"'VERSION'", "'TRUE'", "'FALSE'", "ID", "NUMBER", "STR", "COMMENT_PARTIAL", 
		"COMMENT_LINE", "SPACE"
	};
	public static final int
		RULE_courierProgram = 0, RULE_programHeader = 1, RULE_programBody = 2, 
		RULE_dependencyList = 3, RULE_referencedProgramList = 4, RULE_referencedProgram = 5, 
		RULE_declarationList = 6, RULE_declaration = 7, RULE_type = 8, RULE_predefinedType = 9, 
		RULE_constructedType = 10, RULE_enumType = 11, RULE_mdType = 12, RULE_referencedType = 13, 
		RULE_correspondenceList = 14, RULE_correspondence = 15, RULE_maximumNumber = 16, 
		RULE_numericValue = 17, RULE_typedCandidateList = 18, RULE_typedCandidate = 19, 
		RULE_anonCandidateList = 20, RULE_anonCandidate = 21, RULE_argumentList = 22, 
		RULE_resultList = 23, RULE_errorList = 24, RULE_fieldList = 25, RULE_field = 26, 
		RULE_mdFieldList = 27, RULE_mdFieldType = 28, RULE_mdField = 29, RULE_constant = 30, 
		RULE_predefinedConstant = 31, RULE_constructedConstant = 32, RULE_referencedConstant = 33, 
		RULE_elementList = 34, RULE_componentList = 35, RULE_component = 36, RULE_nameList = 37;
	public static final String[] ruleNames = {
		"courierProgram", "programHeader", "programBody", "dependencyList", "referencedProgramList", 
		"referencedProgram", "declarationList", "declaration", "type", "predefinedType", 
		"constructedType", "enumType", "mdType", "referencedType", "correspondenceList", 
		"correspondence", "maximumNumber", "numericValue", "typedCandidateList", 
		"typedCandidate", "anonCandidateList", "anonCandidate", "argumentList", 
		"resultList", "errorList", "fieldList", "field", "mdFieldList", "mdFieldType", 
		"mdField", "constant", "predefinedConstant", "constructedConstant", "referencedConstant", 
		"elementList", "componentList", "component", "nameList"
	};

	@Override
	public String getGrammarFileName() { return "Courier.g"; }

	@Override
	public String[] getTokenNames() { return tokenNames; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public CourierParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}
	public static class CourierProgramContext extends ParserRuleContext {
		public ProgramBodyContext programBody() {
			return getRuleContext(ProgramBodyContext.class,0);
		}
		public ProgramHeaderContext programHeader() {
			return getRuleContext(ProgramHeaderContext.class,0);
		}
		public CourierProgramContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_courierProgram; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitCourierProgram(this);
			else return visitor.visitChildren(this);
		}
	}

	public final CourierProgramContext courierProgram() throws RecognitionException {
		CourierProgramContext _localctx = new CourierProgramContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_courierProgram);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(76); programHeader();
			setState(77); match(8);
			setState(78); programBody();
			setState(79); match(14);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ProgramHeaderContext extends ParserRuleContext {
		public Token name;
		public Token program;
		public Token version;
		public TerminalNode VERSION() { return getToken(CourierParser.VERSION, 0); }
		public TerminalNode ID() { return getToken(CourierParser.ID, 0); }
		public TerminalNode NUMBER(int i) {
			return getToken(CourierParser.NUMBER, i);
		}
		public List<TerminalNode> NUMBER() { return getTokens(CourierParser.NUMBER); }
		public TerminalNode PROGRAM() { return getToken(CourierParser.PROGRAM, 0); }
		public ProgramHeaderContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_programHeader; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitProgramHeader(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ProgramHeaderContext programHeader() throws RecognitionException {
		ProgramHeaderContext _localctx = new ProgramHeaderContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_programHeader);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(81); ((ProgramHeaderContext)_localctx).name = match(ID);
			setState(82); match(1);
			setState(83); match(PROGRAM);
			setState(84); ((ProgramHeaderContext)_localctx).program = match(NUMBER);
			setState(85); match(VERSION);
			setState(86); ((ProgramHeaderContext)_localctx).version = match(NUMBER);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ProgramBodyContext extends ParserRuleContext {
		public DeclarationListContext declarationList() {
			return getRuleContext(DeclarationListContext.class,0);
		}
		public TerminalNode BEGIN() { return getToken(CourierParser.BEGIN, 0); }
		public TerminalNode END() { return getToken(CourierParser.END, 0); }
		public DependencyListContext dependencyList() {
			return getRuleContext(DependencyListContext.class,0);
		}
		public ProgramBodyContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_programBody; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitProgramBody(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ProgramBodyContext programBody() throws RecognitionException {
		ProgramBodyContext _localctx = new ProgramBodyContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_programBody);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(88); match(BEGIN);
			setState(89); dependencyList();
			setState(90); declarationList();
			setState(91); match(END);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DependencyListContext extends ParserRuleContext {
		public TerminalNode DEPENDS() { return getToken(CourierParser.DEPENDS, 0); }
		public ReferencedProgramListContext referencedProgramList() {
			return getRuleContext(ReferencedProgramListContext.class,0);
		}
		public TerminalNode UPON() { return getToken(CourierParser.UPON, 0); }
		public DependencyListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_dependencyList; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitDependencyList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final DependencyListContext dependencyList() throws RecognitionException {
		DependencyListContext _localctx = new DependencyListContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_dependencyList);
		try {
			setState(99);
			switch (_input.LA(1)) {
			case END:
			case ID:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case DEPENDS:
				enterOuterAlt(_localctx, 2);
				{
				setState(94); match(DEPENDS);
				setState(95); match(UPON);
				setState(96); referencedProgramList();
				setState(97); match(4);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ReferencedProgramListContext extends ParserRuleContext {
		public ReferencedProgramContext referencedProgram;
		public List<ReferencedProgramContext> elements = new ArrayList<ReferencedProgramContext>();
		public ReferencedProgramContext referencedProgram(int i) {
			return getRuleContext(ReferencedProgramContext.class,i);
		}
		public List<ReferencedProgramContext> referencedProgram() {
			return getRuleContexts(ReferencedProgramContext.class);
		}
		public ReferencedProgramListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_referencedProgramList; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitReferencedProgramList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ReferencedProgramListContext referencedProgramList() throws RecognitionException {
		ReferencedProgramListContext _localctx = new ReferencedProgramListContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_referencedProgramList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(101); ((ReferencedProgramListContext)_localctx).referencedProgram = referencedProgram();
			((ReferencedProgramListContext)_localctx).elements.add(((ReferencedProgramListContext)_localctx).referencedProgram);
			setState(106);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==12) {
				{
				{
				setState(102); match(12);
				setState(103); ((ReferencedProgramListContext)_localctx).referencedProgram = referencedProgram();
				((ReferencedProgramListContext)_localctx).elements.add(((ReferencedProgramListContext)_localctx).referencedProgram);
				}
				}
				setState(108);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ReferencedProgramContext extends ParserRuleContext {
		public Token program;
		public Token number;
		public Token version;
		public TerminalNode VERSION() { return getToken(CourierParser.VERSION, 0); }
		public TerminalNode ID() { return getToken(CourierParser.ID, 0); }
		public TerminalNode NUMBER(int i) {
			return getToken(CourierParser.NUMBER, i);
		}
		public List<TerminalNode> NUMBER() { return getTokens(CourierParser.NUMBER); }
		public ReferencedProgramContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_referencedProgram; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitReferencedProgram(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ReferencedProgramContext referencedProgram() throws RecognitionException {
		ReferencedProgramContext _localctx = new ReferencedProgramContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_referencedProgram);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(109); ((ReferencedProgramContext)_localctx).program = match(ID);
			setState(110); match(10);
			setState(111); ((ReferencedProgramContext)_localctx).number = match(NUMBER);
			setState(112); match(11);
			setState(113); match(VERSION);
			setState(114); ((ReferencedProgramContext)_localctx).version = match(NUMBER);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DeclarationListContext extends ParserRuleContext {
		public DeclarationContext declaration;
		public List<DeclarationContext> elements = new ArrayList<DeclarationContext>();
		public List<DeclarationContext> declaration() {
			return getRuleContexts(DeclarationContext.class);
		}
		public DeclarationContext declaration(int i) {
			return getRuleContext(DeclarationContext.class,i);
		}
		public DeclarationListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_declarationList; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitDeclarationList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final DeclarationListContext declarationList() throws RecognitionException {
		DeclarationListContext _localctx = new DeclarationListContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_declarationList);
		int _la;
		try {
			setState(122);
			switch (_input.LA(1)) {
			case END:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case ID:
				enterOuterAlt(_localctx, 2);
				{
				setState(118); 
				_errHandler.sync(this);
				_la = _input.LA(1);
				do {
					{
					{
					setState(117); ((DeclarationListContext)_localctx).declaration = declaration();
					((DeclarationListContext)_localctx).elements.add(((DeclarationListContext)_localctx).declaration);
					}
					}
					setState(120); 
					_errHandler.sync(this);
					_la = _input.LA(1);
				} while ( _la==ID );
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DeclarationContext extends ParserRuleContext {
		public DeclarationContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_declaration; }
	 
		public DeclarationContext() { }
		public void copyFrom(DeclarationContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class DeclConstContext extends DeclarationContext {
		public Token name;
		public TerminalNode ID() { return getToken(CourierParser.ID, 0); }
		public ConstantContext constant() {
			return getRuleContext(ConstantContext.class,0);
		}
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public DeclConstContext(DeclarationContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitDeclConst(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class DeclTypeContext extends DeclarationContext {
		public Token name;
		public TerminalNode ID() { return getToken(CourierParser.ID, 0); }
		public TerminalNode TYPE() { return getToken(CourierParser.TYPE, 0); }
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public DeclTypeContext(DeclarationContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitDeclType(this);
			else return visitor.visitChildren(this);
		}
	}

	public final DeclarationContext declaration() throws RecognitionException {
		DeclarationContext _localctx = new DeclarationContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_declaration);
		try {
			setState(138);
			switch ( getInterpreter().adaptivePredict(_input,4,_ctx) ) {
			case 1:
				_localctx = new DeclTypeContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(124); ((DeclTypeContext)_localctx).name = match(ID);
				setState(125); match(1);
				setState(126); match(TYPE);
				setState(127); match(8);
				setState(128); type();
				setState(129); match(4);
				}
				break;

			case 2:
				_localctx = new DeclConstContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(131); ((DeclConstContext)_localctx).name = match(ID);
				setState(132); match(1);
				setState(133); type();
				setState(134); match(8);
				setState(135); constant();
				setState(136); match(4);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TypeContext extends ParserRuleContext {
		public ConstructedTypeContext constructedType() {
			return getRuleContext(ConstructedTypeContext.class,0);
		}
		public ReferencedTypeContext referencedType() {
			return getRuleContext(ReferencedTypeContext.class,0);
		}
		public PredefinedTypeContext predefinedType() {
			return getRuleContext(PredefinedTypeContext.class,0);
		}
		public TypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_type; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitType(this);
			else return visitor.visitChildren(this);
		}
	}

	public final TypeContext type() throws RecognitionException {
		TypeContext _localctx = new TypeContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_type);
		try {
			setState(143);
			switch (_input.LA(1)) {
			case BLOCK:
			case BOOLEAN:
			case BYTE:
			case CARDINAL:
			case LONG:
			case STRING:
			case UNSPECIFIED:
			case UNSPECIFIED2:
			case UNSPECIFIED3:
				enterOuterAlt(_localctx, 1);
				{
				setState(140); predefinedType();
				}
				break;
			case 3:
			case ARRAY:
			case CHOICE:
			case ERROR:
			case MACHINE:
			case PROCEDURE:
			case RECORD:
			case SEQUENCE:
				enterOuterAlt(_localctx, 2);
				{
				setState(141); constructedType();
				}
				break;
			case ID:
				enterOuterAlt(_localctx, 3);
				{
				setState(142); referencedType();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PredefinedTypeContext extends ParserRuleContext {
		public PredefinedTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_predefinedType; }
	 
		public PredefinedTypeContext() { }
		public void copyFrom(PredefinedTypeContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class TypeByteContext extends PredefinedTypeContext {
		public TerminalNode BYTE() { return getToken(CourierParser.BYTE, 0); }
		public TypeByteContext(PredefinedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeByte(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeCardinalContext extends PredefinedTypeContext {
		public TerminalNode CARDINAL() { return getToken(CourierParser.CARDINAL, 0); }
		public TypeCardinalContext(PredefinedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeCardinal(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeUnspecified2Context extends PredefinedTypeContext {
		public TerminalNode UNSPECIFIED2() { return getToken(CourierParser.UNSPECIFIED2, 0); }
		public TypeUnspecified2Context(PredefinedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeUnspecified2(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeBooleanContext extends PredefinedTypeContext {
		public TerminalNode BOOLEAN() { return getToken(CourierParser.BOOLEAN, 0); }
		public TypeBooleanContext(PredefinedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeBoolean(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeUnspecified3Context extends PredefinedTypeContext {
		public TerminalNode UNSPECIFIED3() { return getToken(CourierParser.UNSPECIFIED3, 0); }
		public TypeUnspecified3Context(PredefinedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeUnspecified3(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeUnspecifiedContext extends PredefinedTypeContext {
		public TerminalNode UNSPECIFIED() { return getToken(CourierParser.UNSPECIFIED, 0); }
		public TypeUnspecifiedContext(PredefinedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeUnspecified(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeBlockContext extends PredefinedTypeContext {
		public TerminalNode BLOCK() { return getToken(CourierParser.BLOCK, 0); }
		public TypeBlockContext(PredefinedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeBlock(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeLongCardinalContext extends PredefinedTypeContext {
		public TerminalNode LONG() { return getToken(CourierParser.LONG, 0); }
		public TerminalNode CARDINAL() { return getToken(CourierParser.CARDINAL, 0); }
		public TypeLongCardinalContext(PredefinedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeLongCardinal(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeStringContext extends PredefinedTypeContext {
		public TerminalNode STRING() { return getToken(CourierParser.STRING, 0); }
		public TypeStringContext(PredefinedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeString(this);
			else return visitor.visitChildren(this);
		}
	}

	public final PredefinedTypeContext predefinedType() throws RecognitionException {
		PredefinedTypeContext _localctx = new PredefinedTypeContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_predefinedType);
		try {
			setState(155);
			switch (_input.LA(1)) {
			case BLOCK:
				_localctx = new TypeBlockContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(145); match(BLOCK);
				}
				break;
			case BOOLEAN:
				_localctx = new TypeBooleanContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(146); match(BOOLEAN);
				}
				break;
			case BYTE:
				_localctx = new TypeByteContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(147); match(BYTE);
				}
				break;
			case CARDINAL:
				_localctx = new TypeCardinalContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(148); match(CARDINAL);
				}
				break;
			case LONG:
				_localctx = new TypeLongCardinalContext(_localctx);
				enterOuterAlt(_localctx, 5);
				{
				setState(149); match(LONG);
				setState(150); match(CARDINAL);
				}
				break;
			case STRING:
				_localctx = new TypeStringContext(_localctx);
				enterOuterAlt(_localctx, 6);
				{
				setState(151); match(STRING);
				}
				break;
			case UNSPECIFIED:
				_localctx = new TypeUnspecifiedContext(_localctx);
				enterOuterAlt(_localctx, 7);
				{
				setState(152); match(UNSPECIFIED);
				}
				break;
			case UNSPECIFIED2:
				_localctx = new TypeUnspecified2Context(_localctx);
				enterOuterAlt(_localctx, 8);
				{
				setState(153); match(UNSPECIFIED2);
				}
				break;
			case UNSPECIFIED3:
				_localctx = new TypeUnspecified3Context(_localctx);
				enterOuterAlt(_localctx, 9);
				{
				setState(154); match(UNSPECIFIED3);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConstructedTypeContext extends ParserRuleContext {
		public ConstructedTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_constructedType; }
	 
		public ConstructedTypeContext() { }
		public void copyFrom(ConstructedTypeContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class TypeArrayContext extends ConstructedTypeContext {
		public TerminalNode ARRAY() { return getToken(CourierParser.ARRAY, 0); }
		public TerminalNode OF() { return getToken(CourierParser.OF, 0); }
		public NumericValueContext numericValue() {
			return getRuleContext(NumericValueContext.class,0);
		}
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public TypeArrayContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeArray(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeChoiceTypedContext extends ConstructedTypeContext {
		public TerminalNode CHOICE() { return getToken(CourierParser.CHOICE, 0); }
		public ReferencedTypeContext referencedType() {
			return getRuleContext(ReferencedTypeContext.class,0);
		}
		public TerminalNode OF() { return getToken(CourierParser.OF, 0); }
		public TypedCandidateListContext typedCandidateList() {
			return getRuleContext(TypedCandidateListContext.class,0);
		}
		public TypeChoiceTypedContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeChoiceTyped(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeEmptyEnumContext extends ConstructedTypeContext {
		public CorrespondenceListContext correspondenceList() {
			return getRuleContext(CorrespondenceListContext.class,0);
		}
		public TypeEmptyEnumContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeEmptyEnum(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeSequenceContext extends ConstructedTypeContext {
		public TerminalNode SEQUENCE() { return getToken(CourierParser.SEQUENCE, 0); }
		public TerminalNode OF() { return getToken(CourierParser.OF, 0); }
		public MaximumNumberContext maximumNumber() {
			return getRuleContext(MaximumNumberContext.class,0);
		}
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public TypeSequenceContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeSequence(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeEnumContext extends ConstructedTypeContext {
		public EnumTypeContext enumType() {
			return getRuleContext(EnumTypeContext.class,0);
		}
		public TerminalNode OF() { return getToken(CourierParser.OF, 0); }
		public CorrespondenceListContext correspondenceList() {
			return getRuleContext(CorrespondenceListContext.class,0);
		}
		public TypeEnumContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeEnum(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeErrorContext extends ConstructedTypeContext {
		public TerminalNode ERROR() { return getToken(CourierParser.ERROR, 0); }
		public ArgumentListContext argumentList() {
			return getRuleContext(ArgumentListContext.class,0);
		}
		public TypeErrorContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeError(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeChoiceAnonContext extends ConstructedTypeContext {
		public TerminalNode CHOICE() { return getToken(CourierParser.CHOICE, 0); }
		public TerminalNode OF() { return getToken(CourierParser.OF, 0); }
		public AnonCandidateListContext anonCandidateList() {
			return getRuleContext(AnonCandidateListContext.class,0);
		}
		public TypeChoiceAnonContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeChoiceAnon(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeRecordContext extends ConstructedTypeContext {
		public FieldListContext fieldList() {
			return getRuleContext(FieldListContext.class,0);
		}
		public TerminalNode RECORD() { return getToken(CourierParser.RECORD, 0); }
		public TypeRecordContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeRecord(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeProcedureContext extends ConstructedTypeContext {
		public ErrorListContext errorList() {
			return getRuleContext(ErrorListContext.class,0);
		}
		public TerminalNode PROCEDURE() { return getToken(CourierParser.PROCEDURE, 0); }
		public ResultListContext resultList() {
			return getRuleContext(ResultListContext.class,0);
		}
		public ArgumentListContext argumentList() {
			return getRuleContext(ArgumentListContext.class,0);
		}
		public TypeProcedureContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeProcedure(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeEmptyRecordContext extends ConstructedTypeContext {
		public TerminalNode RECORD() { return getToken(CourierParser.RECORD, 0); }
		public TypeEmptyRecordContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeEmptyRecord(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeMachineContext extends ConstructedTypeContext {
		public TerminalNode MACHINE() { return getToken(CourierParser.MACHINE, 0); }
		public MdTypeContext mdType() {
			return getRuleContext(MdTypeContext.class,0);
		}
		public TerminalNode OF() { return getToken(CourierParser.OF, 0); }
		public TerminalNode DEPENDENT() { return getToken(CourierParser.DEPENDENT, 0); }
		public MdFieldListContext mdFieldList() {
			return getRuleContext(MdFieldListContext.class,0);
		}
		public TypeMachineContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeMachine(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ConstructedTypeContext constructedType() throws RecognitionException {
		ConstructedTypeContext _localctx = new ConstructedTypeContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_constructedType);
		try {
			setState(213);
			switch ( getInterpreter().adaptivePredict(_input,7,_ctx) ) {
			case 1:
				_localctx = new TypeEmptyEnumContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(157); match(3);
				setState(158); correspondenceList();
				setState(159); match(7);
				}
				break;

			case 2:
				_localctx = new TypeEnumContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(161); match(3);
				setState(162); correspondenceList();
				setState(163); match(7);
				setState(164); match(OF);
				setState(165); enumType();
				}
				break;

			case 3:
				_localctx = new TypeArrayContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(167); match(ARRAY);
				setState(168); numericValue();
				setState(169); match(OF);
				setState(170); type();
				}
				break;

			case 4:
				_localctx = new TypeSequenceContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(172); match(SEQUENCE);
				setState(173); maximumNumber();
				setState(174); match(OF);
				setState(175); type();
				}
				break;

			case 5:
				_localctx = new TypeRecordContext(_localctx);
				enterOuterAlt(_localctx, 5);
				{
				setState(177); match(RECORD);
				setState(178); match(2);
				setState(179); fieldList();
				setState(180); match(6);
				}
				break;

			case 6:
				_localctx = new TypeEmptyRecordContext(_localctx);
				enterOuterAlt(_localctx, 6);
				{
				setState(182); match(RECORD);
				setState(183); match(2);
				setState(184); match(6);
				}
				break;

			case 7:
				_localctx = new TypeChoiceTypedContext(_localctx);
				enterOuterAlt(_localctx, 7);
				{
				setState(185); match(CHOICE);
				setState(186); referencedType();
				setState(187); match(OF);
				setState(188); match(3);
				setState(189); typedCandidateList();
				setState(190); match(7);
				}
				break;

			case 8:
				_localctx = new TypeChoiceAnonContext(_localctx);
				enterOuterAlt(_localctx, 8);
				{
				setState(192); match(CHOICE);
				setState(193); match(OF);
				setState(194); match(3);
				setState(195); anonCandidateList();
				setState(196); match(7);
				}
				break;

			case 9:
				_localctx = new TypeProcedureContext(_localctx);
				enterOuterAlt(_localctx, 9);
				{
				setState(198); match(PROCEDURE);
				setState(199); argumentList();
				setState(200); resultList();
				setState(201); errorList();
				}
				break;

			case 10:
				_localctx = new TypeErrorContext(_localctx);
				enterOuterAlt(_localctx, 10);
				{
				setState(203); match(ERROR);
				setState(204); argumentList();
				}
				break;

			case 11:
				_localctx = new TypeMachineContext(_localctx);
				enterOuterAlt(_localctx, 11);
				{
				setState(205); match(MACHINE);
				setState(206); match(DEPENDENT);
				setState(207); match(2);
				setState(208); mdFieldList();
				setState(209); match(6);
				setState(210); match(OF);
				setState(211); mdType();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class EnumTypeContext extends ParserRuleContext {
		public TerminalNode UNSPECIFIED() { return getToken(CourierParser.UNSPECIFIED, 0); }
		public TerminalNode UNSPECIFIED3() { return getToken(CourierParser.UNSPECIFIED3, 0); }
		public TerminalNode UNSPECIFIED2() { return getToken(CourierParser.UNSPECIFIED2, 0); }
		public TerminalNode BYTE() { return getToken(CourierParser.BYTE, 0); }
		public EnumTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_enumType; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitEnumType(this);
			else return visitor.visitChildren(this);
		}
	}

	public final EnumTypeContext enumType() throws RecognitionException {
		EnumTypeContext _localctx = new EnumTypeContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_enumType);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(215);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << BYTE) | (1L << UNSPECIFIED) | (1L << UNSPECIFIED2) | (1L << UNSPECIFIED3))) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			consume();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class MdTypeContext extends ParserRuleContext {
		public TerminalNode UNSPECIFIED() { return getToken(CourierParser.UNSPECIFIED, 0); }
		public TerminalNode BYTE() { return getToken(CourierParser.BYTE, 0); }
		public MdTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mdType; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitMdType(this);
			else return visitor.visitChildren(this);
		}
	}

	public final MdTypeContext mdType() throws RecognitionException {
		MdTypeContext _localctx = new MdTypeContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_mdType);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(217);
			_la = _input.LA(1);
			if ( !(_la==BYTE || _la==UNSPECIFIED) ) {
			_errHandler.recoverInline(this);
			}
			consume();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ReferencedTypeContext extends ParserRuleContext {
		public ReferencedTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_referencedType; }
	 
		public ReferencedTypeContext() { }
		public void copyFrom(ReferencedTypeContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class TypeRefQContext extends ReferencedTypeContext {
		public Token program;
		public Token name;
		public List<TerminalNode> ID() { return getTokens(CourierParser.ID); }
		public TerminalNode ID(int i) {
			return getToken(CourierParser.ID, i);
		}
		public TypeRefQContext(ReferencedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeRefQ(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeRefContext extends ReferencedTypeContext {
		public Token name;
		public TerminalNode ID() { return getToken(CourierParser.ID, 0); }
		public TypeRefContext(ReferencedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeRef(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ReferencedTypeContext referencedType() throws RecognitionException {
		ReferencedTypeContext _localctx = new ReferencedTypeContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_referencedType);
		try {
			setState(223);
			switch ( getInterpreter().adaptivePredict(_input,8,_ctx) ) {
			case 1:
				_localctx = new TypeRefContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(219); ((TypeRefContext)_localctx).name = match(ID);
				}
				break;

			case 2:
				_localctx = new TypeRefQContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(220); ((TypeRefQContext)_localctx).program = match(ID);
				setState(221); match(14);
				setState(222); ((TypeRefQContext)_localctx).name = match(ID);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class CorrespondenceListContext extends ParserRuleContext {
		public CorrespondenceContext correspondence;
		public List<CorrespondenceContext> elements = new ArrayList<CorrespondenceContext>();
		public CorrespondenceContext correspondence(int i) {
			return getRuleContext(CorrespondenceContext.class,i);
		}
		public List<CorrespondenceContext> correspondence() {
			return getRuleContexts(CorrespondenceContext.class);
		}
		public CorrespondenceListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_correspondenceList; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitCorrespondenceList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final CorrespondenceListContext correspondenceList() throws RecognitionException {
		CorrespondenceListContext _localctx = new CorrespondenceListContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_correspondenceList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(225); ((CorrespondenceListContext)_localctx).correspondence = correspondence();
			((CorrespondenceListContext)_localctx).elements.add(((CorrespondenceListContext)_localctx).correspondence);
			setState(230);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==12) {
				{
				{
				setState(226); match(12);
				setState(227); ((CorrespondenceListContext)_localctx).correspondence = correspondence();
				((CorrespondenceListContext)_localctx).elements.add(((CorrespondenceListContext)_localctx).correspondence);
				}
				}
				setState(232);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class CorrespondenceContext extends ParserRuleContext {
		public TerminalNode ID() { return getToken(CourierParser.ID, 0); }
		public NumericValueContext numericValue() {
			return getRuleContext(NumericValueContext.class,0);
		}
		public CorrespondenceContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_correspondence; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitCorrespondence(this);
			else return visitor.visitChildren(this);
		}
	}

	public final CorrespondenceContext correspondence() throws RecognitionException {
		CorrespondenceContext _localctx = new CorrespondenceContext(_ctx, getState());
		enterRule(_localctx, 30, RULE_correspondence);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(233); match(ID);
			setState(234); match(10);
			setState(235); numericValue();
			setState(236); match(11);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class MaximumNumberContext extends ParserRuleContext {
		public NumericValueContext numericValue() {
			return getRuleContext(NumericValueContext.class,0);
		}
		public MaximumNumberContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_maximumNumber; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitMaximumNumber(this);
			else return visitor.visitChildren(this);
		}
	}

	public final MaximumNumberContext maximumNumber() throws RecognitionException {
		MaximumNumberContext _localctx = new MaximumNumberContext(_ctx, getState());
		enterRule(_localctx, 32, RULE_maximumNumber);
		try {
			setState(240);
			switch (_input.LA(1)) {
			case OF:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case ID:
			case NUMBER:
				enterOuterAlt(_localctx, 2);
				{
				setState(239); numericValue();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class NumericValueContext extends ParserRuleContext {
		public NumericValueContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_numericValue; }
	 
		public NumericValueContext() { }
		public void copyFrom(NumericValueContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class ValueRefConstsContext extends NumericValueContext {
		public ReferencedConstantContext referencedConstant() {
			return getRuleContext(ReferencedConstantContext.class,0);
		}
		public ValueRefConstsContext(NumericValueContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitValueRefConsts(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class ValueNumberContext extends NumericValueContext {
		public TerminalNode NUMBER() { return getToken(CourierParser.NUMBER, 0); }
		public ValueNumberContext(NumericValueContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitValueNumber(this);
			else return visitor.visitChildren(this);
		}
	}

	public final NumericValueContext numericValue() throws RecognitionException {
		NumericValueContext _localctx = new NumericValueContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_numericValue);
		try {
			setState(244);
			switch (_input.LA(1)) {
			case NUMBER:
				_localctx = new ValueNumberContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(242); match(NUMBER);
				}
				break;
			case ID:
				_localctx = new ValueRefConstsContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(243); referencedConstant();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TypedCandidateListContext extends ParserRuleContext {
		public TypedCandidateContext typedCandidate;
		public List<TypedCandidateContext> elements = new ArrayList<TypedCandidateContext>();
		public TypedCandidateContext typedCandidate(int i) {
			return getRuleContext(TypedCandidateContext.class,i);
		}
		public List<TypedCandidateContext> typedCandidate() {
			return getRuleContexts(TypedCandidateContext.class);
		}
		public TypedCandidateListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_typedCandidateList; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypedCandidateList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final TypedCandidateListContext typedCandidateList() throws RecognitionException {
		TypedCandidateListContext _localctx = new TypedCandidateListContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_typedCandidateList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(246); ((TypedCandidateListContext)_localctx).typedCandidate = typedCandidate();
			((TypedCandidateListContext)_localctx).elements.add(((TypedCandidateListContext)_localctx).typedCandidate);
			setState(251);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==12) {
				{
				{
				setState(247); match(12);
				setState(248); ((TypedCandidateListContext)_localctx).typedCandidate = typedCandidate();
				((TypedCandidateListContext)_localctx).elements.add(((TypedCandidateListContext)_localctx).typedCandidate);
				}
				}
				setState(253);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TypedCandidateContext extends ParserRuleContext {
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public NameListContext nameList() {
			return getRuleContext(NameListContext.class,0);
		}
		public TypedCandidateContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_typedCandidate; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypedCandidate(this);
			else return visitor.visitChildren(this);
		}
	}

	public final TypedCandidateContext typedCandidate() throws RecognitionException {
		TypedCandidateContext _localctx = new TypedCandidateContext(_ctx, getState());
		enterRule(_localctx, 38, RULE_typedCandidate);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(254); nameList();
			setState(255); match(9);
			setState(256); type();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AnonCandidateListContext extends ParserRuleContext {
		public AnonCandidateContext anonCandidate;
		public List<AnonCandidateContext> elements = new ArrayList<AnonCandidateContext>();
		public AnonCandidateContext anonCandidate(int i) {
			return getRuleContext(AnonCandidateContext.class,i);
		}
		public List<AnonCandidateContext> anonCandidate() {
			return getRuleContexts(AnonCandidateContext.class);
		}
		public AnonCandidateListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_anonCandidateList; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitAnonCandidateList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final AnonCandidateListContext anonCandidateList() throws RecognitionException {
		AnonCandidateListContext _localctx = new AnonCandidateListContext(_ctx, getState());
		enterRule(_localctx, 40, RULE_anonCandidateList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(258); ((AnonCandidateListContext)_localctx).anonCandidate = anonCandidate();
			((AnonCandidateListContext)_localctx).elements.add(((AnonCandidateListContext)_localctx).anonCandidate);
			setState(263);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==12) {
				{
				{
				setState(259); match(12);
				setState(260); ((AnonCandidateListContext)_localctx).anonCandidate = anonCandidate();
				((AnonCandidateListContext)_localctx).elements.add(((AnonCandidateListContext)_localctx).anonCandidate);
				}
				}
				setState(265);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AnonCandidateContext extends ParserRuleContext {
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public CorrespondenceListContext correspondenceList() {
			return getRuleContext(CorrespondenceListContext.class,0);
		}
		public AnonCandidateContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_anonCandidate; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitAnonCandidate(this);
			else return visitor.visitChildren(this);
		}
	}

	public final AnonCandidateContext anonCandidate() throws RecognitionException {
		AnonCandidateContext _localctx = new AnonCandidateContext(_ctx, getState());
		enterRule(_localctx, 42, RULE_anonCandidate);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(266); correspondenceList();
			setState(267); match(9);
			setState(268); type();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ArgumentListContext extends ParserRuleContext {
		public FieldListContext fieldList() {
			return getRuleContext(FieldListContext.class,0);
		}
		public ArgumentListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_argumentList; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitArgumentList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ArgumentListContext argumentList() throws RecognitionException {
		ArgumentListContext _localctx = new ArgumentListContext(_ctx, getState());
		enterRule(_localctx, 44, RULE_argumentList);
		try {
			setState(275);
			switch (_input.LA(1)) {
			case 4:
			case 6:
			case 7:
			case 8:
			case 12:
			case REPORTS:
			case RETURNS:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(271); match(2);
				setState(272); fieldList();
				setState(273); match(6);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ResultListContext extends ParserRuleContext {
		public FieldListContext fieldList() {
			return getRuleContext(FieldListContext.class,0);
		}
		public TerminalNode RETURNS() { return getToken(CourierParser.RETURNS, 0); }
		public ResultListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_resultList; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitResultList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ResultListContext resultList() throws RecognitionException {
		ResultListContext _localctx = new ResultListContext(_ctx, getState());
		enterRule(_localctx, 46, RULE_resultList);
		try {
			setState(283);
			switch (_input.LA(1)) {
			case 4:
			case 6:
			case 7:
			case 8:
			case 12:
			case REPORTS:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case RETURNS:
				enterOuterAlt(_localctx, 2);
				{
				setState(278); match(RETURNS);
				setState(279); match(2);
				setState(280); fieldList();
				setState(281); match(6);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ErrorListContext extends ParserRuleContext {
		public TerminalNode REPORTS() { return getToken(CourierParser.REPORTS, 0); }
		public NameListContext nameList() {
			return getRuleContext(NameListContext.class,0);
		}
		public ErrorListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_errorList; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitErrorList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ErrorListContext errorList() throws RecognitionException {
		ErrorListContext _localctx = new ErrorListContext(_ctx, getState());
		enterRule(_localctx, 48, RULE_errorList);
		try {
			setState(291);
			switch (_input.LA(1)) {
			case 4:
			case 6:
			case 7:
			case 8:
			case 12:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case REPORTS:
				enterOuterAlt(_localctx, 2);
				{
				setState(286); match(REPORTS);
				setState(287); match(2);
				setState(288); nameList();
				setState(289); match(6);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FieldListContext extends ParserRuleContext {
		public FieldContext field;
		public List<FieldContext> elements = new ArrayList<FieldContext>();
		public FieldContext field(int i) {
			return getRuleContext(FieldContext.class,i);
		}
		public List<FieldContext> field() {
			return getRuleContexts(FieldContext.class);
		}
		public FieldListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_fieldList; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitFieldList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final FieldListContext fieldList() throws RecognitionException {
		FieldListContext _localctx = new FieldListContext(_ctx, getState());
		enterRule(_localctx, 50, RULE_fieldList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(293); ((FieldListContext)_localctx).field = field();
			((FieldListContext)_localctx).elements.add(((FieldListContext)_localctx).field);
			setState(298);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==12) {
				{
				{
				setState(294); match(12);
				setState(295); ((FieldListContext)_localctx).field = field();
				((FieldListContext)_localctx).elements.add(((FieldListContext)_localctx).field);
				}
				}
				setState(300);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FieldContext extends ParserRuleContext {
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public NameListContext nameList() {
			return getRuleContext(NameListContext.class,0);
		}
		public FieldContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_field; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitField(this);
			else return visitor.visitChildren(this);
		}
	}

	public final FieldContext field() throws RecognitionException {
		FieldContext _localctx = new FieldContext(_ctx, getState());
		enterRule(_localctx, 52, RULE_field);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(301); nameList();
			setState(302); match(1);
			setState(303); type();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class MdFieldListContext extends ParserRuleContext {
		public MdFieldContext mdField;
		public List<MdFieldContext> elements = new ArrayList<MdFieldContext>();
		public List<MdFieldContext> mdField() {
			return getRuleContexts(MdFieldContext.class);
		}
		public MdFieldContext mdField(int i) {
			return getRuleContext(MdFieldContext.class,i);
		}
		public MdFieldListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mdFieldList; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitMdFieldList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final MdFieldListContext mdFieldList() throws RecognitionException {
		MdFieldListContext _localctx = new MdFieldListContext(_ctx, getState());
		enterRule(_localctx, 54, RULE_mdFieldList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(305); ((MdFieldListContext)_localctx).mdField = mdField();
			((MdFieldListContext)_localctx).elements.add(((MdFieldListContext)_localctx).mdField);
			setState(310);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==12) {
				{
				{
				setState(306); match(12);
				setState(307); ((MdFieldListContext)_localctx).mdField = mdField();
				((MdFieldListContext)_localctx).elements.add(((MdFieldListContext)_localctx).mdField);
				}
				}
				setState(312);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class MdFieldTypeContext extends ParserRuleContext {
		public TerminalNode UNSPECIFIED() { return getToken(CourierParser.UNSPECIFIED, 0); }
		public TerminalNode BOOLEAN() { return getToken(CourierParser.BOOLEAN, 0); }
		public TerminalNode CARDINAL() { return getToken(CourierParser.CARDINAL, 0); }
		public MdFieldTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mdFieldType; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitMdFieldType(this);
			else return visitor.visitChildren(this);
		}
	}

	public final MdFieldTypeContext mdFieldType() throws RecognitionException {
		MdFieldTypeContext _localctx = new MdFieldTypeContext(_ctx, getState());
		enterRule(_localctx, 56, RULE_mdFieldType);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(313);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << BOOLEAN) | (1L << CARDINAL) | (1L << UNSPECIFIED))) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			consume();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class MdFieldContext extends ParserRuleContext {
		public Token name;
		public Token start;
		public Token stop;
		public MdFieldTypeContext mdFieldType() {
			return getRuleContext(MdFieldTypeContext.class,0);
		}
		public TerminalNode ID() { return getToken(CourierParser.ID, 0); }
		public TerminalNode NUMBER(int i) {
			return getToken(CourierParser.NUMBER, i);
		}
		public List<TerminalNode> NUMBER() { return getTokens(CourierParser.NUMBER); }
		public MdFieldContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mdField; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitMdField(this);
			else return visitor.visitChildren(this);
		}
	}

	public final MdFieldContext mdField() throws RecognitionException {
		MdFieldContext _localctx = new MdFieldContext(_ctx, getState());
		enterRule(_localctx, 58, RULE_mdField);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(315); ((MdFieldContext)_localctx).name = match(ID);
			setState(316); match(10);
			setState(317); ((MdFieldContext)_localctx).start = match(NUMBER);
			setState(318); match(5);
			setState(319); ((MdFieldContext)_localctx).stop = match(NUMBER);
			setState(320); match(11);
			setState(321); match(1);
			setState(322); mdFieldType();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConstantContext extends ParserRuleContext {
		public PredefinedConstantContext predefinedConstant() {
			return getRuleContext(PredefinedConstantContext.class,0);
		}
		public ReferencedConstantContext referencedConstant() {
			return getRuleContext(ReferencedConstantContext.class,0);
		}
		public ConstructedConstantContext constructedConstant() {
			return getRuleContext(ConstructedConstantContext.class,0);
		}
		public ConstantContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_constant; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstant(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ConstantContext constant() throws RecognitionException {
		ConstantContext _localctx = new ConstantContext(_ctx, getState());
		enterRule(_localctx, 60, RULE_constant);
		try {
			setState(327);
			switch ( getInterpreter().adaptivePredict(_input,19,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(324); predefinedConstant();
				}
				break;

			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(325); constructedConstant();
				}
				break;

			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(326); referencedConstant();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PredefinedConstantContext extends ParserRuleContext {
		public PredefinedConstantContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_predefinedConstant; }
	 
		public PredefinedConstantContext() { }
		public void copyFrom(PredefinedConstantContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class ConstTrueContext extends PredefinedConstantContext {
		public TerminalNode TRUE() { return getToken(CourierParser.TRUE, 0); }
		public ConstTrueContext(PredefinedConstantContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstTrue(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class ConstNumberNegativeContext extends PredefinedConstantContext {
		public TerminalNode NUMBER() { return getToken(CourierParser.NUMBER, 0); }
		public ConstNumberNegativeContext(PredefinedConstantContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstNumberNegative(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class ConstStringContext extends PredefinedConstantContext {
		public TerminalNode STR() { return getToken(CourierParser.STR, 0); }
		public ConstStringContext(PredefinedConstantContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstString(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class ConstFalseContext extends PredefinedConstantContext {
		public TerminalNode FALSE() { return getToken(CourierParser.FALSE, 0); }
		public ConstFalseContext(PredefinedConstantContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstFalse(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class ConstNumberContext extends PredefinedConstantContext {
		public TerminalNode NUMBER() { return getToken(CourierParser.NUMBER, 0); }
		public ConstNumberContext(PredefinedConstantContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstNumber(this);
			else return visitor.visitChildren(this);
		}
	}

	public final PredefinedConstantContext predefinedConstant() throws RecognitionException {
		PredefinedConstantContext _localctx = new PredefinedConstantContext(_ctx, getState());
		enterRule(_localctx, 62, RULE_predefinedConstant);
		try {
			setState(335);
			switch (_input.LA(1)) {
			case TRUE:
				_localctx = new ConstTrueContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(329); match(TRUE);
				}
				break;
			case FALSE:
				_localctx = new ConstFalseContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(330); match(FALSE);
				}
				break;
			case NUMBER:
				_localctx = new ConstNumberContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(331); match(NUMBER);
				}
				break;
			case 13:
				_localctx = new ConstNumberNegativeContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(332); match(13);
				setState(333); match(NUMBER);
				}
				break;
			case STR:
				_localctx = new ConstStringContext(_localctx);
				enterOuterAlt(_localctx, 5);
				{
				setState(334); match(STR);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConstructedConstantContext extends ParserRuleContext {
		public ConstructedConstantContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_constructedConstant; }
	 
		public ConstructedConstantContext() { }
		public void copyFrom(ConstructedConstantContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class ConstChoiceContext extends ConstructedConstantContext {
		public TerminalNode ID() { return getToken(CourierParser.ID, 0); }
		public ConstantContext constant() {
			return getRuleContext(ConstantContext.class,0);
		}
		public ConstChoiceContext(ConstructedConstantContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstChoice(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class ConstArrayContext extends ConstructedConstantContext {
		public ElementListContext elementList() {
			return getRuleContext(ElementListContext.class,0);
		}
		public ConstArrayContext(ConstructedConstantContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstArray(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class ConstRecordContext extends ConstructedConstantContext {
		public ComponentListContext componentList() {
			return getRuleContext(ComponentListContext.class,0);
		}
		public ConstRecordContext(ConstructedConstantContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstRecord(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class ConstEmptyContext extends ConstructedConstantContext {
		public ConstEmptyContext(ConstructedConstantContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstEmpty(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ConstructedConstantContext constructedConstant() throws RecognitionException {
		ConstructedConstantContext _localctx = new ConstructedConstantContext(_ctx, getState());
		enterRule(_localctx, 64, RULE_constructedConstant);
		try {
			setState(349);
			switch ( getInterpreter().adaptivePredict(_input,21,_ctx) ) {
			case 1:
				_localctx = new ConstArrayContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(337); match(2);
				setState(338); elementList();
				setState(339); match(6);
				}
				break;

			case 2:
				_localctx = new ConstRecordContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(341); match(2);
				setState(342); componentList();
				setState(343); match(6);
				}
				break;

			case 3:
				_localctx = new ConstEmptyContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(345); match(2);
				setState(346); match(6);
				}
				break;

			case 4:
				_localctx = new ConstChoiceContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(347); match(ID);
				setState(348); constant();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ReferencedConstantContext extends ParserRuleContext {
		public ReferencedConstantContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_referencedConstant; }
	 
		public ReferencedConstantContext() { }
		public void copyFrom(ReferencedConstantContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class ConstRefContext extends ReferencedConstantContext {
		public Token name;
		public TerminalNode ID() { return getToken(CourierParser.ID, 0); }
		public ConstRefContext(ReferencedConstantContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstRef(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class ConstRefQContext extends ReferencedConstantContext {
		public Token program;
		public Token name;
		public List<TerminalNode> ID() { return getTokens(CourierParser.ID); }
		public TerminalNode ID(int i) {
			return getToken(CourierParser.ID, i);
		}
		public ConstRefQContext(ReferencedConstantContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstRefQ(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ReferencedConstantContext referencedConstant() throws RecognitionException {
		ReferencedConstantContext _localctx = new ReferencedConstantContext(_ctx, getState());
		enterRule(_localctx, 66, RULE_referencedConstant);
		try {
			setState(355);
			switch ( getInterpreter().adaptivePredict(_input,22,_ctx) ) {
			case 1:
				_localctx = new ConstRefContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(351); ((ConstRefContext)_localctx).name = match(ID);
				}
				break;

			case 2:
				_localctx = new ConstRefQContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(352); ((ConstRefQContext)_localctx).program = match(ID);
				setState(353); match(14);
				setState(354); ((ConstRefQContext)_localctx).name = match(ID);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ElementListContext extends ParserRuleContext {
		public ConstantContext constant;
		public List<ConstantContext> elements = new ArrayList<ConstantContext>();
		public List<ConstantContext> constant() {
			return getRuleContexts(ConstantContext.class);
		}
		public ConstantContext constant(int i) {
			return getRuleContext(ConstantContext.class,i);
		}
		public ElementListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_elementList; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitElementList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ElementListContext elementList() throws RecognitionException {
		ElementListContext _localctx = new ElementListContext(_ctx, getState());
		enterRule(_localctx, 68, RULE_elementList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(357); ((ElementListContext)_localctx).constant = constant();
			((ElementListContext)_localctx).elements.add(((ElementListContext)_localctx).constant);
			setState(362);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==12) {
				{
				{
				setState(358); match(12);
				setState(359); ((ElementListContext)_localctx).constant = constant();
				((ElementListContext)_localctx).elements.add(((ElementListContext)_localctx).constant);
				}
				}
				setState(364);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ComponentListContext extends ParserRuleContext {
		public ComponentContext component;
		public List<ComponentContext> elements = new ArrayList<ComponentContext>();
		public ComponentContext component(int i) {
			return getRuleContext(ComponentContext.class,i);
		}
		public List<ComponentContext> component() {
			return getRuleContexts(ComponentContext.class);
		}
		public ComponentListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_componentList; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitComponentList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ComponentListContext componentList() throws RecognitionException {
		ComponentListContext _localctx = new ComponentListContext(_ctx, getState());
		enterRule(_localctx, 70, RULE_componentList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(365); ((ComponentListContext)_localctx).component = component();
			((ComponentListContext)_localctx).elements.add(((ComponentListContext)_localctx).component);
			setState(370);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==12) {
				{
				{
				setState(366); match(12);
				setState(367); ((ComponentListContext)_localctx).component = component();
				((ComponentListContext)_localctx).elements.add(((ComponentListContext)_localctx).component);
				}
				}
				setState(372);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ComponentContext extends ParserRuleContext {
		public ConstantContext constant() {
			return getRuleContext(ConstantContext.class,0);
		}
		public NameListContext nameList() {
			return getRuleContext(NameListContext.class,0);
		}
		public ComponentContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_component; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitComponent(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ComponentContext component() throws RecognitionException {
		ComponentContext _localctx = new ComponentContext(_ctx, getState());
		enterRule(_localctx, 72, RULE_component);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(373); nameList();
			setState(374); match(1);
			setState(375); constant();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class NameListContext extends ParserRuleContext {
		public Token ID;
		public List<Token> elements = new ArrayList<Token>();
		public List<TerminalNode> ID() { return getTokens(CourierParser.ID); }
		public TerminalNode ID(int i) {
			return getToken(CourierParser.ID, i);
		}
		public NameListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_nameList; }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitNameList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final NameListContext nameList() throws RecognitionException {
		NameListContext _localctx = new NameListContext(_ctx, getState());
		enterRule(_localctx, 74, RULE_nameList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(377); ((NameListContext)_localctx).ID = match(ID);
			((NameListContext)_localctx).elements.add(((NameListContext)_localctx).ID);
			setState(382);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==12) {
				{
				{
				setState(378); match(12);
				setState(379); ((NameListContext)_localctx).ID = match(ID);
				((NameListContext)_localctx).elements.add(((NameListContext)_localctx).ID);
				}
				}
				setState(384);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\3\uacf5\uee8c\u4f5d\u8b0d\u4a45\u78bd\u1b2f\u3378\3\64\u0184\4\2\t\2"+
		"\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13"+
		"\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\4&\t&\4\'\t\'\3\2\3\2\3\2\3\2\3\2\3\3\3"+
		"\3\3\3\3\3\3\3\3\3\3\3\3\4\3\4\3\4\3\4\3\4\3\5\3\5\3\5\3\5\3\5\3\5\5\5"+
		"f\n\5\3\6\3\6\3\6\7\6k\n\6\f\6\16\6n\13\6\3\7\3\7\3\7\3\7\3\7\3\7\3\7"+
		"\3\b\3\b\6\by\n\b\r\b\16\bz\5\b}\n\b\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3"+
		"\t\3\t\3\t\3\t\3\t\3\t\5\t\u008d\n\t\3\n\3\n\3\n\5\n\u0092\n\n\3\13\3"+
		"\13\3\13\3\13\3\13\3\13\3\13\3\13\3\13\3\13\5\13\u009e\n\13\3\f\3\f\3"+
		"\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f"+
		"\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3"+
		"\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f"+
		"\3\f\5\f\u00d8\n\f\3\r\3\r\3\16\3\16\3\17\3\17\3\17\3\17\5\17\u00e2\n"+
		"\17\3\20\3\20\3\20\7\20\u00e7\n\20\f\20\16\20\u00ea\13\20\3\21\3\21\3"+
		"\21\3\21\3\21\3\22\3\22\5\22\u00f3\n\22\3\23\3\23\5\23\u00f7\n\23\3\24"+
		"\3\24\3\24\7\24\u00fc\n\24\f\24\16\24\u00ff\13\24\3\25\3\25\3\25\3\25"+
		"\3\26\3\26\3\26\7\26\u0108\n\26\f\26\16\26\u010b\13\26\3\27\3\27\3\27"+
		"\3\27\3\30\3\30\3\30\3\30\3\30\5\30\u0116\n\30\3\31\3\31\3\31\3\31\3\31"+
		"\3\31\5\31\u011e\n\31\3\32\3\32\3\32\3\32\3\32\3\32\5\32\u0126\n\32\3"+
		"\33\3\33\3\33\7\33\u012b\n\33\f\33\16\33\u012e\13\33\3\34\3\34\3\34\3"+
		"\34\3\35\3\35\3\35\7\35\u0137\n\35\f\35\16\35\u013a\13\35\3\36\3\36\3"+
		"\37\3\37\3\37\3\37\3\37\3\37\3\37\3\37\3\37\3 \3 \3 \5 \u014a\n \3!\3"+
		"!\3!\3!\3!\3!\5!\u0152\n!\3\"\3\"\3\"\3\"\3\"\3\"\3\"\3\"\3\"\3\"\3\""+
		"\3\"\5\"\u0160\n\"\3#\3#\3#\3#\5#\u0166\n#\3$\3$\3$\7$\u016b\n$\f$\16"+
		"$\u016e\13$\3%\3%\3%\7%\u0173\n%\f%\16%\u0176\13%\3&\3&\3&\3&\3\'\3\'"+
		"\3\'\7\'\u017f\n\'\f\'\16\'\u0182\13\'\3\'\2(\2\4\6\b\n\f\16\20\22\24"+
		"\26\30\32\34\36 \"$&(*,.\60\62\64\668:<>@BDFHJL\2\5\4\2\25\25(*\4\2\25"+
		"\25((\5\2\24\24\26\26((\u018e\2N\3\2\2\2\4S\3\2\2\2\6Z\3\2\2\2\be\3\2"+
		"\2\2\ng\3\2\2\2\fo\3\2\2\2\16|\3\2\2\2\20\u008c\3\2\2\2\22\u0091\3\2\2"+
		"\2\24\u009d\3\2\2\2\26\u00d7\3\2\2\2\30\u00d9\3\2\2\2\32\u00db\3\2\2\2"+
		"\34\u00e1\3\2\2\2\36\u00e3\3\2\2\2 \u00eb\3\2\2\2\"\u00f2\3\2\2\2$\u00f6"+
		"\3\2\2\2&\u00f8\3\2\2\2(\u0100\3\2\2\2*\u0104\3\2\2\2,\u010c\3\2\2\2."+
		"\u0115\3\2\2\2\60\u011d\3\2\2\2\62\u0125\3\2\2\2\64\u0127\3\2\2\2\66\u012f"+
		"\3\2\2\28\u0133\3\2\2\2:\u013b\3\2\2\2<\u013d\3\2\2\2>\u0149\3\2\2\2@"+
		"\u0151\3\2\2\2B\u015f\3\2\2\2D\u0165\3\2\2\2F\u0167\3\2\2\2H\u016f\3\2"+
		"\2\2J\u0177\3\2\2\2L\u017b\3\2\2\2NO\5\4\3\2OP\7\n\2\2PQ\5\6\4\2QR\7\20"+
		"\2\2R\3\3\2\2\2ST\7/\2\2TU\7\3\2\2UV\7!\2\2VW\7\60\2\2WX\7,\2\2XY\7\60"+
		"\2\2Y\5\3\2\2\2Z[\7\22\2\2[\\\5\b\5\2\\]\5\16\b\2]^\7\32\2\2^\7\3\2\2"+
		"\2_f\3\2\2\2`a\7\30\2\2ab\7+\2\2bc\5\n\6\2cd\7\6\2\2df\3\2\2\2e_\3\2\2"+
		"\2e`\3\2\2\2f\t\3\2\2\2gl\5\f\7\2hi\7\16\2\2ik\5\f\7\2jh\3\2\2\2kn\3\2"+
		"\2\2lj\3\2\2\2lm\3\2\2\2m\13\3\2\2\2nl\3\2\2\2op\7/\2\2pq\7\f\2\2qr\7"+
		"\60\2\2rs\7\r\2\2st\7,\2\2tu\7\60\2\2u\r\3\2\2\2v}\3\2\2\2wy\5\20\t\2"+
		"xw\3\2\2\2yz\3\2\2\2zx\3\2\2\2z{\3\2\2\2{}\3\2\2\2|v\3\2\2\2|x\3\2\2\2"+
		"}\17\3\2\2\2~\177\7/\2\2\177\u0080\7\3\2\2\u0080\u0081\7\'\2\2\u0081\u0082"+
		"\7\n\2\2\u0082\u0083\5\22\n\2\u0083\u0084\7\6\2\2\u0084\u008d\3\2\2\2"+
		"\u0085\u0086\7/\2\2\u0086\u0087\7\3\2\2\u0087\u0088\5\22\n\2\u0088\u0089"+
		"\7\n\2\2\u0089\u008a\5> \2\u008a\u008b\7\6\2\2\u008b\u008d\3\2\2\2\u008c"+
		"~\3\2\2\2\u008c\u0085\3\2\2\2\u008d\21\3\2\2\2\u008e\u0092\5\24\13\2\u008f"+
		"\u0092\5\26\f\2\u0090\u0092\5\34\17\2\u0091\u008e\3\2\2\2\u0091\u008f"+
		"\3\2\2\2\u0091\u0090\3\2\2\2\u0092\23\3\2\2\2\u0093\u009e\7\23\2\2\u0094"+
		"\u009e\7\24\2\2\u0095\u009e\7\25\2\2\u0096\u009e\7\26\2\2\u0097\u0098"+
		"\7\35\2\2\u0098\u009e\7\26\2\2\u0099\u009e\7&\2\2\u009a\u009e\7(\2\2\u009b"+
		"\u009e\7)\2\2\u009c\u009e\7*\2\2\u009d\u0093\3\2\2\2\u009d\u0094\3\2\2"+
		"\2\u009d\u0095\3\2\2\2\u009d\u0096\3\2\2\2\u009d\u0097\3\2\2\2\u009d\u0099"+
		"\3\2\2\2\u009d\u009a\3\2\2\2\u009d\u009b\3\2\2\2\u009d\u009c\3\2\2\2\u009e"+
		"\25\3\2\2\2\u009f\u00a0\7\5\2\2\u00a0\u00a1\5\36\20\2\u00a1\u00a2\7\t"+
		"\2\2\u00a2\u00d8\3\2\2\2\u00a3\u00a4\7\5\2\2\u00a4\u00a5\5\36\20\2\u00a5"+
		"\u00a6\7\t\2\2\u00a6\u00a7\7\37\2\2\u00a7\u00a8\5\30\r\2\u00a8\u00d8\3"+
		"\2\2\2\u00a9\u00aa\7\21\2\2\u00aa\u00ab\5$\23\2\u00ab\u00ac\7\37\2\2\u00ac"+
		"\u00ad\5\22\n\2\u00ad\u00d8\3\2\2\2\u00ae\u00af\7%\2\2\u00af\u00b0\5\""+
		"\22\2\u00b0\u00b1\7\37\2\2\u00b1\u00b2\5\22\n\2\u00b2\u00d8\3\2\2\2\u00b3"+
		"\u00b4\7\"\2\2\u00b4\u00b5\7\4\2\2\u00b5\u00b6\5\64\33\2\u00b6\u00b7\7"+
		"\b\2\2\u00b7\u00d8\3\2\2\2\u00b8\u00b9\7\"\2\2\u00b9\u00ba\7\4\2\2\u00ba"+
		"\u00d8\7\b\2\2\u00bb\u00bc\7\27\2\2\u00bc\u00bd\5\34\17\2\u00bd\u00be"+
		"\7\37\2\2\u00be\u00bf\7\5\2\2\u00bf\u00c0\5&\24\2\u00c0\u00c1\7\t\2\2"+
		"\u00c1\u00d8\3\2\2\2\u00c2\u00c3\7\27\2\2\u00c3\u00c4\7\37\2\2\u00c4\u00c5"+
		"\7\5\2\2\u00c5\u00c6\5*\26\2\u00c6\u00c7\7\t\2\2\u00c7\u00d8\3\2\2\2\u00c8"+
		"\u00c9\7 \2\2\u00c9\u00ca\5.\30\2\u00ca\u00cb\5\60\31\2\u00cb\u00cc\5"+
		"\62\32\2\u00cc\u00d8\3\2\2\2\u00cd\u00ce\7\33\2\2\u00ce\u00d8\5.\30\2"+
		"\u00cf\u00d0\7\36\2\2\u00d0\u00d1\7\31\2\2\u00d1\u00d2\7\4\2\2\u00d2\u00d3"+
		"\58\35\2\u00d3\u00d4\7\b\2\2\u00d4\u00d5\7\37\2\2\u00d5\u00d6\5\32\16"+
		"\2\u00d6\u00d8\3\2\2\2\u00d7\u009f\3\2\2\2\u00d7\u00a3\3\2\2\2\u00d7\u00a9"+
		"\3\2\2\2\u00d7\u00ae\3\2\2\2\u00d7\u00b3\3\2\2\2\u00d7\u00b8\3\2\2\2\u00d7"+
		"\u00bb\3\2\2\2\u00d7\u00c2\3\2\2\2\u00d7\u00c8\3\2\2\2\u00d7\u00cd\3\2"+
		"\2\2\u00d7\u00cf\3\2\2\2\u00d8\27\3\2\2\2\u00d9\u00da\t\2\2\2\u00da\31"+
		"\3\2\2\2\u00db\u00dc\t\3\2\2\u00dc\33\3\2\2\2\u00dd\u00e2\7/\2\2\u00de"+
		"\u00df\7/\2\2\u00df\u00e0\7\20\2\2\u00e0\u00e2\7/\2\2\u00e1\u00dd\3\2"+
		"\2\2\u00e1\u00de\3\2\2\2\u00e2\35\3\2\2\2\u00e3\u00e8\5 \21\2\u00e4\u00e5"+
		"\7\16\2\2\u00e5\u00e7\5 \21\2\u00e6\u00e4\3\2\2\2\u00e7\u00ea\3\2\2\2"+
		"\u00e8\u00e6\3\2\2\2\u00e8\u00e9\3\2\2\2\u00e9\37\3\2\2\2\u00ea\u00e8"+
		"\3\2\2\2\u00eb\u00ec\7/\2\2\u00ec\u00ed\7\f\2\2\u00ed\u00ee\5$\23\2\u00ee"+
		"\u00ef\7\r\2\2\u00ef!\3\2\2\2\u00f0\u00f3\3\2\2\2\u00f1\u00f3\5$\23\2"+
		"\u00f2\u00f0\3\2\2\2\u00f2\u00f1\3\2\2\2\u00f3#\3\2\2\2\u00f4\u00f7\7"+
		"\60\2\2\u00f5\u00f7\5D#\2\u00f6\u00f4\3\2\2\2\u00f6\u00f5\3\2\2\2\u00f7"+
		"%\3\2\2\2\u00f8\u00fd\5(\25\2\u00f9\u00fa\7\16\2\2\u00fa\u00fc\5(\25\2"+
		"\u00fb\u00f9\3\2\2\2\u00fc\u00ff\3\2\2\2\u00fd\u00fb\3\2\2\2\u00fd\u00fe"+
		"\3\2\2\2\u00fe\'\3\2\2\2\u00ff\u00fd\3\2\2\2\u0100\u0101\5L\'\2\u0101"+
		"\u0102\7\13\2\2\u0102\u0103\5\22\n\2\u0103)\3\2\2\2\u0104\u0109\5,\27"+
		"\2\u0105\u0106\7\16\2\2\u0106\u0108\5,\27\2\u0107\u0105\3\2\2\2\u0108"+
		"\u010b\3\2\2\2\u0109\u0107\3\2\2\2\u0109\u010a\3\2\2\2\u010a+\3\2\2\2"+
		"\u010b\u0109\3\2\2\2\u010c\u010d\5\36\20\2\u010d\u010e\7\13\2\2\u010e"+
		"\u010f\5\22\n\2\u010f-\3\2\2\2\u0110\u0116\3\2\2\2\u0111\u0112\7\4\2\2"+
		"\u0112\u0113\5\64\33\2\u0113\u0114\7\b\2\2\u0114\u0116\3\2\2\2\u0115\u0110"+
		"\3\2\2\2\u0115\u0111\3\2\2\2\u0116/\3\2\2\2\u0117\u011e\3\2\2\2\u0118"+
		"\u0119\7$\2\2\u0119\u011a\7\4\2\2\u011a\u011b\5\64\33\2\u011b\u011c\7"+
		"\b\2\2\u011c\u011e\3\2\2\2\u011d\u0117\3\2\2\2\u011d\u0118\3\2\2\2\u011e"+
		"\61\3\2\2\2\u011f\u0126\3\2\2\2\u0120\u0121\7#\2\2\u0121\u0122\7\4\2\2"+
		"\u0122\u0123\5L\'\2\u0123\u0124\7\b\2\2\u0124\u0126\3\2\2\2\u0125\u011f"+
		"\3\2\2\2\u0125\u0120\3\2\2\2\u0126\63\3\2\2\2\u0127\u012c\5\66\34\2\u0128"+
		"\u0129\7\16\2\2\u0129\u012b\5\66\34\2\u012a\u0128\3\2\2\2\u012b\u012e"+
		"\3\2\2\2\u012c\u012a\3\2\2\2\u012c\u012d\3\2\2\2\u012d\65\3\2\2\2\u012e"+
		"\u012c\3\2\2\2\u012f\u0130\5L\'\2\u0130\u0131\7\3\2\2\u0131\u0132\5\22"+
		"\n\2\u0132\67\3\2\2\2\u0133\u0138\5<\37\2\u0134\u0135\7\16\2\2\u0135\u0137"+
		"\5<\37\2\u0136\u0134\3\2\2\2\u0137\u013a\3\2\2\2\u0138\u0136\3\2\2\2\u0138"+
		"\u0139\3\2\2\2\u01399\3\2\2\2\u013a\u0138\3\2\2\2\u013b\u013c\t\4\2\2"+
		"\u013c;\3\2\2\2\u013d\u013e\7/\2\2\u013e\u013f\7\f\2\2\u013f\u0140\7\60"+
		"\2\2\u0140\u0141\7\7\2\2\u0141\u0142\7\60\2\2\u0142\u0143\7\r\2\2\u0143"+
		"\u0144\7\3\2\2\u0144\u0145\5:\36\2\u0145=\3\2\2\2\u0146\u014a\5@!\2\u0147"+
		"\u014a\5B\"\2\u0148\u014a\5D#\2\u0149\u0146\3\2\2\2\u0149\u0147\3\2\2"+
		"\2\u0149\u0148\3\2\2\2\u014a?\3\2\2\2\u014b\u0152\7-\2\2\u014c\u0152\7"+
		".\2\2\u014d\u0152\7\60\2\2\u014e\u014f\7\17\2\2\u014f\u0152\7\60\2\2\u0150"+
		"\u0152\7\61\2\2\u0151\u014b\3\2\2\2\u0151\u014c\3\2\2\2\u0151\u014d\3"+
		"\2\2\2\u0151\u014e\3\2\2\2\u0151\u0150\3\2\2\2\u0152A\3\2\2\2\u0153\u0154"+
		"\7\4\2\2\u0154\u0155\5F$\2\u0155\u0156\7\b\2\2\u0156\u0160\3\2\2\2\u0157"+
		"\u0158\7\4\2\2\u0158\u0159\5H%\2\u0159\u015a\7\b\2\2\u015a\u0160\3\2\2"+
		"\2\u015b\u015c\7\4\2\2\u015c\u0160\7\b\2\2\u015d\u015e\7/\2\2\u015e\u0160"+
		"\5> \2\u015f\u0153\3\2\2\2\u015f\u0157\3\2\2\2\u015f\u015b\3\2\2\2\u015f"+
		"\u015d\3\2\2\2\u0160C\3\2\2\2\u0161\u0166\7/\2\2\u0162\u0163\7/\2\2\u0163"+
		"\u0164\7\20\2\2\u0164\u0166\7/\2\2\u0165\u0161\3\2\2\2\u0165\u0162\3\2"+
		"\2\2\u0166E\3\2\2\2\u0167\u016c\5> \2\u0168\u0169\7\16\2\2\u0169\u016b"+
		"\5> \2\u016a\u0168\3\2\2\2\u016b\u016e\3\2\2\2\u016c\u016a\3\2\2\2\u016c"+
		"\u016d\3\2\2\2\u016dG\3\2\2\2\u016e\u016c\3\2\2\2\u016f\u0174\5J&\2\u0170"+
		"\u0171\7\16\2\2\u0171\u0173\5J&\2\u0172\u0170\3\2\2\2\u0173\u0176\3\2"+
		"\2\2\u0174\u0172\3\2\2\2\u0174\u0175\3\2\2\2\u0175I\3\2\2\2\u0176\u0174"+
		"\3\2\2\2\u0177\u0178\5L\'\2\u0178\u0179\7\3\2\2\u0179\u017a\5> \2\u017a"+
		"K\3\2\2\2\u017b\u0180\7/\2\2\u017c\u017d\7\16\2\2\u017d\u017f\7/\2\2\u017e"+
		"\u017c\3\2\2\2\u017f\u0182\3\2\2\2\u0180\u017e\3\2\2\2\u0180\u0181\3\2"+
		"\2\2\u0181M\3\2\2\2\u0182\u0180\3\2\2\2\34elz|\u008c\u0091\u009d\u00d7"+
		"\u00e1\u00e8\u00f2\u00f6\u00fd\u0109\u0115\u011d\u0125\u012c\u0138\u0149"+
		"\u0151\u015f\u0165\u016c\u0174\u0180";
	public static final ATN _ATN =
		ATNSimulator.deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}