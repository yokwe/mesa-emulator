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
		T__12=1, T__11=2, T__10=3, T__9=4, T__8=5, T__7=6, T__6=7, T__5=8, T__4=9, 
		T__3=10, T__2=11, T__1=12, T__0=13, ARRAY=14, BEGIN=15, BOOLEAN=16, BYTE=17, 
		CARDINAL=18, CHOICE=19, DEPENDS=20, END=21, ERROR=22, INTEGER=23, LONG=24, 
		OF=25, PROCEDURE=26, PROGRAM=27, RECORD=28, REPORTS=29, RETURNS=30, SEQUENCE=31, 
		STRING=32, TYPE=33, UNSPECIFIED=34, UNSPECIFIED2=35, UNSPECIFIED3=36, 
		UPON=37, VERSION=38, TRUE=39, FALSE=40, ID=41, NUMBER=42, STR=43, COMMENT_PARTIAL=44, 
		COMMENT_LINE=45, SPACE=46;
	public static final String[] tokenNames = {
		"<INVALID>", "':'", "'['", "'{'", "';'", "']'", "'}'", "'='", "'=>'", 
		"'('", "')'", "','", "'-'", "'.'", "'ARRAY'", "'BEGIN'", "'BOOLEAN'", 
		"'BYTE'", "'CARDINAL'", "'CHOICE'", "'DEPENDS'", "'END'", "'ERROR'", "'INTEGER'", 
		"'LONG'", "'OF'", "'PROCEDURE'", "'PROGRAM'", "'RECORD'", "'REPORTS'", 
		"'RETURNS'", "'SEQUENCE'", "'STRING'", "'TYPE'", "'UNSPECIFIED'", "'UNSPECIFIED2'", 
		"'UNSPECIFIED3'", "'UPON'", "'VERSION'", "'TRUE'", "'FALSE'", "ID", "NUMBER", 
		"STR", "COMMENT_PARTIAL", "COMMENT_LINE", "SPACE"
	};
	public static final int
		RULE_courierProgram = 0, RULE_programHeader = 1, RULE_programBody = 2, 
		RULE_dependencyList = 3, RULE_referencedProgramList = 4, RULE_referencedProgram = 5, 
		RULE_declarationList = 6, RULE_declaration = 7, RULE_type = 8, RULE_predefinedType = 9, 
		RULE_constructedType = 10, RULE_enumType = 11, RULE_referencedType = 12, 
		RULE_correspondenceList = 13, RULE_correspondence = 14, RULE_maximumNumber = 15, 
		RULE_numericValue = 16, RULE_typedCandidateList = 17, RULE_typedCandidate = 18, 
		RULE_anonCandidateList = 19, RULE_anonCandidate = 20, RULE_argumentList = 21, 
		RULE_resultList = 22, RULE_errorList = 23, RULE_fieldList = 24, RULE_field = 25, 
		RULE_constant = 26, RULE_predefinedConstant = 27, RULE_constructedConstant = 28, 
		RULE_referencedConstant = 29, RULE_elementList = 30, RULE_componentList = 31, 
		RULE_component = 32, RULE_nameList = 33;
	public static final String[] ruleNames = {
		"courierProgram", "programHeader", "programBody", "dependencyList", "referencedProgramList", 
		"referencedProgram", "declarationList", "declaration", "type", "predefinedType", 
		"constructedType", "enumType", "referencedType", "correspondenceList", 
		"correspondence", "maximumNumber", "numericValue", "typedCandidateList", 
		"typedCandidate", "anonCandidateList", "anonCandidate", "argumentList", 
		"resultList", "errorList", "fieldList", "field", "constant", "predefinedConstant", 
		"constructedConstant", "referencedConstant", "elementList", "componentList", 
		"component", "nameList"
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
			setState(68); programHeader();
			setState(69); match(7);
			setState(70); programBody();
			setState(71); match(13);
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
			setState(73); ((ProgramHeaderContext)_localctx).name = match(ID);
			setState(74); match(1);
			setState(75); match(PROGRAM);
			setState(76); ((ProgramHeaderContext)_localctx).program = match(NUMBER);
			setState(77); match(VERSION);
			setState(78); ((ProgramHeaderContext)_localctx).version = match(NUMBER);
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
			setState(80); match(BEGIN);
			setState(81); dependencyList();
			setState(82); declarationList();
			setState(83); match(END);
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
			setState(91);
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
				setState(86); match(DEPENDS);
				setState(87); match(UPON);
				setState(88); referencedProgramList();
				setState(89); match(4);
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
			setState(93); ((ReferencedProgramListContext)_localctx).referencedProgram = referencedProgram();
			((ReferencedProgramListContext)_localctx).elements.add(((ReferencedProgramListContext)_localctx).referencedProgram);
			setState(98);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==11) {
				{
				{
				setState(94); match(11);
				setState(95); ((ReferencedProgramListContext)_localctx).referencedProgram = referencedProgram();
				((ReferencedProgramListContext)_localctx).elements.add(((ReferencedProgramListContext)_localctx).referencedProgram);
				}
				}
				setState(100);
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
			setState(101); ((ReferencedProgramContext)_localctx).program = match(ID);
			setState(102); match(9);
			setState(103); ((ReferencedProgramContext)_localctx).number = match(NUMBER);
			setState(104); match(10);
			setState(105); match(VERSION);
			setState(106); ((ReferencedProgramContext)_localctx).version = match(NUMBER);
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
			setState(114);
			switch (_input.LA(1)) {
			case END:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case ID:
				enterOuterAlt(_localctx, 2);
				{
				setState(110); 
				_errHandler.sync(this);
				_la = _input.LA(1);
				do {
					{
					{
					setState(109); ((DeclarationListContext)_localctx).declaration = declaration();
					((DeclarationListContext)_localctx).elements.add(((DeclarationListContext)_localctx).declaration);
					}
					}
					setState(112); 
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
			setState(130);
			switch ( getInterpreter().adaptivePredict(_input,4,_ctx) ) {
			case 1:
				_localctx = new DeclTypeContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(116); ((DeclTypeContext)_localctx).name = match(ID);
				setState(117); match(1);
				setState(118); match(TYPE);
				setState(119); match(7);
				setState(120); type();
				setState(121); match(4);
				}
				break;

			case 2:
				_localctx = new DeclConstContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(123); ((DeclConstContext)_localctx).name = match(ID);
				setState(124); match(1);
				setState(125); type();
				setState(126); match(7);
				setState(127); constant();
				setState(128); match(4);
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
			setState(135);
			switch (_input.LA(1)) {
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
				setState(132); predefinedType();
				}
				break;
			case 3:
			case ARRAY:
			case CHOICE:
			case ERROR:
			case PROCEDURE:
			case RECORD:
			case SEQUENCE:
				enterOuterAlt(_localctx, 2);
				{
				setState(133); constructedType();
				}
				break;
			case ID:
				enterOuterAlt(_localctx, 3);
				{
				setState(134); referencedType();
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
			setState(146);
			switch (_input.LA(1)) {
			case BOOLEAN:
				_localctx = new TypeBooleanContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(137); match(BOOLEAN);
				}
				break;
			case BYTE:
				_localctx = new TypeByteContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(138); match(BYTE);
				}
				break;
			case CARDINAL:
				_localctx = new TypeCardinalContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(139); match(CARDINAL);
				}
				break;
			case LONG:
				_localctx = new TypeLongCardinalContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(140); match(LONG);
				setState(141); match(CARDINAL);
				}
				break;
			case STRING:
				_localctx = new TypeStringContext(_localctx);
				enterOuterAlt(_localctx, 5);
				{
				setState(142); match(STRING);
				}
				break;
			case UNSPECIFIED:
				_localctx = new TypeUnspecifiedContext(_localctx);
				enterOuterAlt(_localctx, 6);
				{
				setState(143); match(UNSPECIFIED);
				}
				break;
			case UNSPECIFIED2:
				_localctx = new TypeUnspecified2Context(_localctx);
				enterOuterAlt(_localctx, 7);
				{
				setState(144); match(UNSPECIFIED2);
				}
				break;
			case UNSPECIFIED3:
				_localctx = new TypeUnspecified3Context(_localctx);
				enterOuterAlt(_localctx, 8);
				{
				setState(145); match(UNSPECIFIED3);
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

	public final ConstructedTypeContext constructedType() throws RecognitionException {
		ConstructedTypeContext _localctx = new ConstructedTypeContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_constructedType);
		try {
			setState(196);
			switch ( getInterpreter().adaptivePredict(_input,7,_ctx) ) {
			case 1:
				_localctx = new TypeEmptyEnumContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(148); match(3);
				setState(149); correspondenceList();
				setState(150); match(6);
				}
				break;

			case 2:
				_localctx = new TypeEnumContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(152); match(3);
				setState(153); correspondenceList();
				setState(154); match(6);
				setState(155); match(OF);
				setState(156); enumType();
				}
				break;

			case 3:
				_localctx = new TypeArrayContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(158); match(ARRAY);
				setState(159); numericValue();
				setState(160); match(OF);
				setState(161); type();
				}
				break;

			case 4:
				_localctx = new TypeSequenceContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(163); match(SEQUENCE);
				setState(164); maximumNumber();
				setState(165); match(OF);
				setState(166); type();
				}
				break;

			case 5:
				_localctx = new TypeRecordContext(_localctx);
				enterOuterAlt(_localctx, 5);
				{
				setState(168); match(RECORD);
				setState(169); match(2);
				setState(170); fieldList();
				setState(171); match(5);
				}
				break;

			case 6:
				_localctx = new TypeEmptyRecordContext(_localctx);
				enterOuterAlt(_localctx, 6);
				{
				setState(173); match(RECORD);
				setState(174); match(2);
				setState(175); match(5);
				}
				break;

			case 7:
				_localctx = new TypeChoiceTypedContext(_localctx);
				enterOuterAlt(_localctx, 7);
				{
				setState(176); match(CHOICE);
				setState(177); referencedType();
				setState(178); match(OF);
				setState(179); match(3);
				setState(180); typedCandidateList();
				setState(181); match(6);
				}
				break;

			case 8:
				_localctx = new TypeChoiceAnonContext(_localctx);
				enterOuterAlt(_localctx, 8);
				{
				setState(183); match(CHOICE);
				setState(184); match(OF);
				setState(185); match(3);
				setState(186); anonCandidateList();
				setState(187); match(6);
				}
				break;

			case 9:
				_localctx = new TypeProcedureContext(_localctx);
				enterOuterAlt(_localctx, 9);
				{
				setState(189); match(PROCEDURE);
				setState(190); argumentList();
				setState(191); resultList();
				setState(192); errorList();
				}
				break;

			case 10:
				_localctx = new TypeErrorContext(_localctx);
				enterOuterAlt(_localctx, 10);
				{
				setState(194); match(ERROR);
				setState(195); argumentList();
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
			setState(198);
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
		enterRule(_localctx, 24, RULE_referencedType);
		try {
			setState(204);
			switch ( getInterpreter().adaptivePredict(_input,8,_ctx) ) {
			case 1:
				_localctx = new TypeRefContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(200); ((TypeRefContext)_localctx).name = match(ID);
				}
				break;

			case 2:
				_localctx = new TypeRefQContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(201); ((TypeRefQContext)_localctx).program = match(ID);
				setState(202); match(13);
				setState(203); ((TypeRefQContext)_localctx).name = match(ID);
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
		enterRule(_localctx, 26, RULE_correspondenceList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(206); ((CorrespondenceListContext)_localctx).correspondence = correspondence();
			((CorrespondenceListContext)_localctx).elements.add(((CorrespondenceListContext)_localctx).correspondence);
			setState(211);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==11) {
				{
				{
				setState(207); match(11);
				setState(208); ((CorrespondenceListContext)_localctx).correspondence = correspondence();
				((CorrespondenceListContext)_localctx).elements.add(((CorrespondenceListContext)_localctx).correspondence);
				}
				}
				setState(213);
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
		enterRule(_localctx, 28, RULE_correspondence);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(214); match(ID);
			setState(215); match(9);
			setState(216); numericValue();
			setState(217); match(10);
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
		enterRule(_localctx, 30, RULE_maximumNumber);
		try {
			setState(221);
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
				setState(220); numericValue();
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
		enterRule(_localctx, 32, RULE_numericValue);
		try {
			setState(225);
			switch (_input.LA(1)) {
			case NUMBER:
				_localctx = new ValueNumberContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(223); match(NUMBER);
				}
				break;
			case ID:
				_localctx = new ValueRefConstsContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(224); referencedConstant();
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
		enterRule(_localctx, 34, RULE_typedCandidateList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(227); ((TypedCandidateListContext)_localctx).typedCandidate = typedCandidate();
			((TypedCandidateListContext)_localctx).elements.add(((TypedCandidateListContext)_localctx).typedCandidate);
			setState(232);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==11) {
				{
				{
				setState(228); match(11);
				setState(229); ((TypedCandidateListContext)_localctx).typedCandidate = typedCandidate();
				((TypedCandidateListContext)_localctx).elements.add(((TypedCandidateListContext)_localctx).typedCandidate);
				}
				}
				setState(234);
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
		enterRule(_localctx, 36, RULE_typedCandidate);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(235); nameList();
			setState(236); match(8);
			setState(237); type();
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
		enterRule(_localctx, 38, RULE_anonCandidateList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(239); ((AnonCandidateListContext)_localctx).anonCandidate = anonCandidate();
			((AnonCandidateListContext)_localctx).elements.add(((AnonCandidateListContext)_localctx).anonCandidate);
			setState(244);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==11) {
				{
				{
				setState(240); match(11);
				setState(241); ((AnonCandidateListContext)_localctx).anonCandidate = anonCandidate();
				((AnonCandidateListContext)_localctx).elements.add(((AnonCandidateListContext)_localctx).anonCandidate);
				}
				}
				setState(246);
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
		enterRule(_localctx, 40, RULE_anonCandidate);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(247); correspondenceList();
			setState(248); match(8);
			setState(249); type();
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
		enterRule(_localctx, 42, RULE_argumentList);
		try {
			setState(256);
			switch (_input.LA(1)) {
			case 4:
			case 5:
			case 6:
			case 7:
			case 11:
			case REPORTS:
			case RETURNS:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(252); match(2);
				setState(253); fieldList();
				setState(254); match(5);
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
		enterRule(_localctx, 44, RULE_resultList);
		try {
			setState(264);
			switch (_input.LA(1)) {
			case 4:
			case 5:
			case 6:
			case 7:
			case 11:
			case REPORTS:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case RETURNS:
				enterOuterAlt(_localctx, 2);
				{
				setState(259); match(RETURNS);
				setState(260); match(2);
				setState(261); fieldList();
				setState(262); match(5);
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
		enterRule(_localctx, 46, RULE_errorList);
		try {
			setState(272);
			switch (_input.LA(1)) {
			case 4:
			case 5:
			case 6:
			case 7:
			case 11:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case REPORTS:
				enterOuterAlt(_localctx, 2);
				{
				setState(267); match(REPORTS);
				setState(268); match(2);
				setState(269); nameList();
				setState(270); match(5);
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
		enterRule(_localctx, 48, RULE_fieldList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(274); ((FieldListContext)_localctx).field = field();
			((FieldListContext)_localctx).elements.add(((FieldListContext)_localctx).field);
			setState(279);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==11) {
				{
				{
				setState(275); match(11);
				setState(276); ((FieldListContext)_localctx).field = field();
				((FieldListContext)_localctx).elements.add(((FieldListContext)_localctx).field);
				}
				}
				setState(281);
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
		enterRule(_localctx, 50, RULE_field);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(282); nameList();
			setState(283); match(1);
			setState(284); type();
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
		enterRule(_localctx, 52, RULE_constant);
		try {
			setState(289);
			switch ( getInterpreter().adaptivePredict(_input,18,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(286); predefinedConstant();
				}
				break;

			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(287); constructedConstant();
				}
				break;

			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(288); referencedConstant();
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
		enterRule(_localctx, 54, RULE_predefinedConstant);
		try {
			setState(297);
			switch (_input.LA(1)) {
			case TRUE:
				_localctx = new ConstTrueContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(291); match(TRUE);
				}
				break;
			case FALSE:
				_localctx = new ConstFalseContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(292); match(FALSE);
				}
				break;
			case NUMBER:
				_localctx = new ConstNumberContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(293); match(NUMBER);
				}
				break;
			case 12:
				_localctx = new ConstNumberNegativeContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(294); match(12);
				setState(295); match(NUMBER);
				}
				break;
			case STR:
				_localctx = new ConstStringContext(_localctx);
				enterOuterAlt(_localctx, 5);
				{
				setState(296); match(STR);
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
		enterRule(_localctx, 56, RULE_constructedConstant);
		try {
			setState(311);
			switch ( getInterpreter().adaptivePredict(_input,20,_ctx) ) {
			case 1:
				_localctx = new ConstArrayContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(299); match(2);
				setState(300); elementList();
				setState(301); match(5);
				}
				break;

			case 2:
				_localctx = new ConstRecordContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(303); match(2);
				setState(304); componentList();
				setState(305); match(5);
				}
				break;

			case 3:
				_localctx = new ConstEmptyContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(307); match(2);
				setState(308); match(5);
				}
				break;

			case 4:
				_localctx = new ConstChoiceContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(309); match(ID);
				setState(310); constant();
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
		enterRule(_localctx, 58, RULE_referencedConstant);
		try {
			setState(317);
			switch ( getInterpreter().adaptivePredict(_input,21,_ctx) ) {
			case 1:
				_localctx = new ConstRefContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(313); ((ConstRefContext)_localctx).name = match(ID);
				}
				break;

			case 2:
				_localctx = new ConstRefQContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(314); ((ConstRefQContext)_localctx).program = match(ID);
				setState(315); match(13);
				setState(316); ((ConstRefQContext)_localctx).name = match(ID);
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
		enterRule(_localctx, 60, RULE_elementList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(319); ((ElementListContext)_localctx).constant = constant();
			((ElementListContext)_localctx).elements.add(((ElementListContext)_localctx).constant);
			setState(324);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==11) {
				{
				{
				setState(320); match(11);
				setState(321); ((ElementListContext)_localctx).constant = constant();
				((ElementListContext)_localctx).elements.add(((ElementListContext)_localctx).constant);
				}
				}
				setState(326);
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
		enterRule(_localctx, 62, RULE_componentList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(327); ((ComponentListContext)_localctx).component = component();
			((ComponentListContext)_localctx).elements.add(((ComponentListContext)_localctx).component);
			setState(332);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==11) {
				{
				{
				setState(328); match(11);
				setState(329); ((ComponentListContext)_localctx).component = component();
				((ComponentListContext)_localctx).elements.add(((ComponentListContext)_localctx).component);
				}
				}
				setState(334);
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
		enterRule(_localctx, 64, RULE_component);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(335); nameList();
			setState(336); match(1);
			setState(337); constant();
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
		enterRule(_localctx, 66, RULE_nameList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(339); ((NameListContext)_localctx).ID = match(ID);
			((NameListContext)_localctx).elements.add(((NameListContext)_localctx).ID);
			setState(344);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==11) {
				{
				{
				setState(340); match(11);
				setState(341); ((NameListContext)_localctx).ID = match(ID);
				((NameListContext)_localctx).elements.add(((NameListContext)_localctx).ID);
				}
				}
				setState(346);
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
		"\3\uacf5\uee8c\u4f5d\u8b0d\u4a45\u78bd\u1b2f\u3378\3\60\u015e\4\2\t\2"+
		"\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13"+
		"\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\4\"\t\"\4#\t#\3\2\3\2\3\2\3\2\3\2\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\4"+
		"\3\4\3\4\3\4\3\4\3\5\3\5\3\5\3\5\3\5\3\5\5\5^\n\5\3\6\3\6\3\6\7\6c\n\6"+
		"\f\6\16\6f\13\6\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\b\3\b\6\bq\n\b\r\b\16\b"+
		"r\5\bu\n\b\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\5\t"+
		"\u0085\n\t\3\n\3\n\3\n\5\n\u008a\n\n\3\13\3\13\3\13\3\13\3\13\3\13\3\13"+
		"\3\13\3\13\5\13\u0095\n\13\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f"+
		"\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3"+
		"\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f"+
		"\3\f\3\f\5\f\u00c7\n\f\3\r\3\r\3\16\3\16\3\16\3\16\5\16\u00cf\n\16\3\17"+
		"\3\17\3\17\7\17\u00d4\n\17\f\17\16\17\u00d7\13\17\3\20\3\20\3\20\3\20"+
		"\3\20\3\21\3\21\5\21\u00e0\n\21\3\22\3\22\5\22\u00e4\n\22\3\23\3\23\3"+
		"\23\7\23\u00e9\n\23\f\23\16\23\u00ec\13\23\3\24\3\24\3\24\3\24\3\25\3"+
		"\25\3\25\7\25\u00f5\n\25\f\25\16\25\u00f8\13\25\3\26\3\26\3\26\3\26\3"+
		"\27\3\27\3\27\3\27\3\27\5\27\u0103\n\27\3\30\3\30\3\30\3\30\3\30\3\30"+
		"\5\30\u010b\n\30\3\31\3\31\3\31\3\31\3\31\3\31\5\31\u0113\n\31\3\32\3"+
		"\32\3\32\7\32\u0118\n\32\f\32\16\32\u011b\13\32\3\33\3\33\3\33\3\33\3"+
		"\34\3\34\3\34\5\34\u0124\n\34\3\35\3\35\3\35\3\35\3\35\3\35\5\35\u012c"+
		"\n\35\3\36\3\36\3\36\3\36\3\36\3\36\3\36\3\36\3\36\3\36\3\36\3\36\5\36"+
		"\u013a\n\36\3\37\3\37\3\37\3\37\5\37\u0140\n\37\3 \3 \3 \7 \u0145\n \f"+
		" \16 \u0148\13 \3!\3!\3!\7!\u014d\n!\f!\16!\u0150\13!\3\"\3\"\3\"\3\""+
		"\3#\3#\3#\7#\u0159\n#\f#\16#\u015c\13#\3#\2$\2\4\6\b\n\f\16\20\22\24\26"+
		"\30\32\34\36 \"$&(*,.\60\62\64\668:<>@BD\2\3\4\2\23\23$&\u0169\2F\3\2"+
		"\2\2\4K\3\2\2\2\6R\3\2\2\2\b]\3\2\2\2\n_\3\2\2\2\fg\3\2\2\2\16t\3\2\2"+
		"\2\20\u0084\3\2\2\2\22\u0089\3\2\2\2\24\u0094\3\2\2\2\26\u00c6\3\2\2\2"+
		"\30\u00c8\3\2\2\2\32\u00ce\3\2\2\2\34\u00d0\3\2\2\2\36\u00d8\3\2\2\2 "+
		"\u00df\3\2\2\2\"\u00e3\3\2\2\2$\u00e5\3\2\2\2&\u00ed\3\2\2\2(\u00f1\3"+
		"\2\2\2*\u00f9\3\2\2\2,\u0102\3\2\2\2.\u010a\3\2\2\2\60\u0112\3\2\2\2\62"+
		"\u0114\3\2\2\2\64\u011c\3\2\2\2\66\u0123\3\2\2\28\u012b\3\2\2\2:\u0139"+
		"\3\2\2\2<\u013f\3\2\2\2>\u0141\3\2\2\2@\u0149\3\2\2\2B\u0151\3\2\2\2D"+
		"\u0155\3\2\2\2FG\5\4\3\2GH\7\t\2\2HI\5\6\4\2IJ\7\17\2\2J\3\3\2\2\2KL\7"+
		"+\2\2LM\7\3\2\2MN\7\35\2\2NO\7,\2\2OP\7(\2\2PQ\7,\2\2Q\5\3\2\2\2RS\7\21"+
		"\2\2ST\5\b\5\2TU\5\16\b\2UV\7\27\2\2V\7\3\2\2\2W^\3\2\2\2XY\7\26\2\2Y"+
		"Z\7\'\2\2Z[\5\n\6\2[\\\7\6\2\2\\^\3\2\2\2]W\3\2\2\2]X\3\2\2\2^\t\3\2\2"+
		"\2_d\5\f\7\2`a\7\r\2\2ac\5\f\7\2b`\3\2\2\2cf\3\2\2\2db\3\2\2\2de\3\2\2"+
		"\2e\13\3\2\2\2fd\3\2\2\2gh\7+\2\2hi\7\13\2\2ij\7,\2\2jk\7\f\2\2kl\7(\2"+
		"\2lm\7,\2\2m\r\3\2\2\2nu\3\2\2\2oq\5\20\t\2po\3\2\2\2qr\3\2\2\2rp\3\2"+
		"\2\2rs\3\2\2\2su\3\2\2\2tn\3\2\2\2tp\3\2\2\2u\17\3\2\2\2vw\7+\2\2wx\7"+
		"\3\2\2xy\7#\2\2yz\7\t\2\2z{\5\22\n\2{|\7\6\2\2|\u0085\3\2\2\2}~\7+\2\2"+
		"~\177\7\3\2\2\177\u0080\5\22\n\2\u0080\u0081\7\t\2\2\u0081\u0082\5\66"+
		"\34\2\u0082\u0083\7\6\2\2\u0083\u0085\3\2\2\2\u0084v\3\2\2\2\u0084}\3"+
		"\2\2\2\u0085\21\3\2\2\2\u0086\u008a\5\24\13\2\u0087\u008a\5\26\f\2\u0088"+
		"\u008a\5\32\16\2\u0089\u0086\3\2\2\2\u0089\u0087\3\2\2\2\u0089\u0088\3"+
		"\2\2\2\u008a\23\3\2\2\2\u008b\u0095\7\22\2\2\u008c\u0095\7\23\2\2\u008d"+
		"\u0095\7\24\2\2\u008e\u008f\7\32\2\2\u008f\u0095\7\24\2\2\u0090\u0095"+
		"\7\"\2\2\u0091\u0095\7$\2\2\u0092\u0095\7%\2\2\u0093\u0095\7&\2\2\u0094"+
		"\u008b\3\2\2\2\u0094\u008c\3\2\2\2\u0094\u008d\3\2\2\2\u0094\u008e\3\2"+
		"\2\2\u0094\u0090\3\2\2\2\u0094\u0091\3\2\2\2\u0094\u0092\3\2\2\2\u0094"+
		"\u0093\3\2\2\2\u0095\25\3\2\2\2\u0096\u0097\7\5\2\2\u0097\u0098\5\34\17"+
		"\2\u0098\u0099\7\b\2\2\u0099\u00c7\3\2\2\2\u009a\u009b\7\5\2\2\u009b\u009c"+
		"\5\34\17\2\u009c\u009d\7\b\2\2\u009d\u009e\7\33\2\2\u009e\u009f\5\30\r"+
		"\2\u009f\u00c7\3\2\2\2\u00a0\u00a1\7\20\2\2\u00a1\u00a2\5\"\22\2\u00a2"+
		"\u00a3\7\33\2\2\u00a3\u00a4\5\22\n\2\u00a4\u00c7\3\2\2\2\u00a5\u00a6\7"+
		"!\2\2\u00a6\u00a7\5 \21\2\u00a7\u00a8\7\33\2\2\u00a8\u00a9\5\22\n\2\u00a9"+
		"\u00c7\3\2\2\2\u00aa\u00ab\7\36\2\2\u00ab\u00ac\7\4\2\2\u00ac\u00ad\5"+
		"\62\32\2\u00ad\u00ae\7\7\2\2\u00ae\u00c7\3\2\2\2\u00af\u00b0\7\36\2\2"+
		"\u00b0\u00b1\7\4\2\2\u00b1\u00c7\7\7\2\2\u00b2\u00b3\7\25\2\2\u00b3\u00b4"+
		"\5\32\16\2\u00b4\u00b5\7\33\2\2\u00b5\u00b6\7\5\2\2\u00b6\u00b7\5$\23"+
		"\2\u00b7\u00b8\7\b\2\2\u00b8\u00c7\3\2\2\2\u00b9\u00ba\7\25\2\2\u00ba"+
		"\u00bb\7\33\2\2\u00bb\u00bc\7\5\2\2\u00bc\u00bd\5(\25\2\u00bd\u00be\7"+
		"\b\2\2\u00be\u00c7\3\2\2\2\u00bf\u00c0\7\34\2\2\u00c0\u00c1\5,\27\2\u00c1"+
		"\u00c2\5.\30\2\u00c2\u00c3\5\60\31\2\u00c3\u00c7\3\2\2\2\u00c4\u00c5\7"+
		"\30\2\2\u00c5\u00c7\5,\27\2\u00c6\u0096\3\2\2\2\u00c6\u009a\3\2\2\2\u00c6"+
		"\u00a0\3\2\2\2\u00c6\u00a5\3\2\2\2\u00c6\u00aa\3\2\2\2\u00c6\u00af\3\2"+
		"\2\2\u00c6\u00b2\3\2\2\2\u00c6\u00b9\3\2\2\2\u00c6\u00bf\3\2\2\2\u00c6"+
		"\u00c4\3\2\2\2\u00c7\27\3\2\2\2\u00c8\u00c9\t\2\2\2\u00c9\31\3\2\2\2\u00ca"+
		"\u00cf\7+\2\2\u00cb\u00cc\7+\2\2\u00cc\u00cd\7\17\2\2\u00cd\u00cf\7+\2"+
		"\2\u00ce\u00ca\3\2\2\2\u00ce\u00cb\3\2\2\2\u00cf\33\3\2\2\2\u00d0\u00d5"+
		"\5\36\20\2\u00d1\u00d2\7\r\2\2\u00d2\u00d4\5\36\20\2\u00d3\u00d1\3\2\2"+
		"\2\u00d4\u00d7\3\2\2\2\u00d5\u00d3\3\2\2\2\u00d5\u00d6\3\2\2\2\u00d6\35"+
		"\3\2\2\2\u00d7\u00d5\3\2\2\2\u00d8\u00d9\7+\2\2\u00d9\u00da\7\13\2\2\u00da"+
		"\u00db\5\"\22\2\u00db\u00dc\7\f\2\2\u00dc\37\3\2\2\2\u00dd\u00e0\3\2\2"+
		"\2\u00de\u00e0\5\"\22\2\u00df\u00dd\3\2\2\2\u00df\u00de\3\2\2\2\u00e0"+
		"!\3\2\2\2\u00e1\u00e4\7,\2\2\u00e2\u00e4\5<\37\2\u00e3\u00e1\3\2\2\2\u00e3"+
		"\u00e2\3\2\2\2\u00e4#\3\2\2\2\u00e5\u00ea\5&\24\2\u00e6\u00e7\7\r\2\2"+
		"\u00e7\u00e9\5&\24\2\u00e8\u00e6\3\2\2\2\u00e9\u00ec\3\2\2\2\u00ea\u00e8"+
		"\3\2\2\2\u00ea\u00eb\3\2\2\2\u00eb%\3\2\2\2\u00ec\u00ea\3\2\2\2\u00ed"+
		"\u00ee\5D#\2\u00ee\u00ef\7\n\2\2\u00ef\u00f0\5\22\n\2\u00f0\'\3\2\2\2"+
		"\u00f1\u00f6\5*\26\2\u00f2\u00f3\7\r\2\2\u00f3\u00f5\5*\26\2\u00f4\u00f2"+
		"\3\2\2\2\u00f5\u00f8\3\2\2\2\u00f6\u00f4\3\2\2\2\u00f6\u00f7\3\2\2\2\u00f7"+
		")\3\2\2\2\u00f8\u00f6\3\2\2\2\u00f9\u00fa\5\34\17\2\u00fa\u00fb\7\n\2"+
		"\2\u00fb\u00fc\5\22\n\2\u00fc+\3\2\2\2\u00fd\u0103\3\2\2\2\u00fe\u00ff"+
		"\7\4\2\2\u00ff\u0100\5\62\32\2\u0100\u0101\7\7\2\2\u0101\u0103\3\2\2\2"+
		"\u0102\u00fd\3\2\2\2\u0102\u00fe\3\2\2\2\u0103-\3\2\2\2\u0104\u010b\3"+
		"\2\2\2\u0105\u0106\7 \2\2\u0106\u0107\7\4\2\2\u0107\u0108\5\62\32\2\u0108"+
		"\u0109\7\7\2\2\u0109\u010b\3\2\2\2\u010a\u0104\3\2\2\2\u010a\u0105\3\2"+
		"\2\2\u010b/\3\2\2\2\u010c\u0113\3\2\2\2\u010d\u010e\7\37\2\2\u010e\u010f"+
		"\7\4\2\2\u010f\u0110\5D#\2\u0110\u0111\7\7\2\2\u0111\u0113\3\2\2\2\u0112"+
		"\u010c\3\2\2\2\u0112\u010d\3\2\2\2\u0113\61\3\2\2\2\u0114\u0119\5\64\33"+
		"\2\u0115\u0116\7\r\2\2\u0116\u0118\5\64\33\2\u0117\u0115\3\2\2\2\u0118"+
		"\u011b\3\2\2\2\u0119\u0117\3\2\2\2\u0119\u011a\3\2\2\2\u011a\63\3\2\2"+
		"\2\u011b\u0119\3\2\2\2\u011c\u011d\5D#\2\u011d\u011e\7\3\2\2\u011e\u011f"+
		"\5\22\n\2\u011f\65\3\2\2\2\u0120\u0124\58\35\2\u0121\u0124\5:\36\2\u0122"+
		"\u0124\5<\37\2\u0123\u0120\3\2\2\2\u0123\u0121\3\2\2\2\u0123\u0122\3\2"+
		"\2\2\u0124\67\3\2\2\2\u0125\u012c\7)\2\2\u0126\u012c\7*\2\2\u0127\u012c"+
		"\7,\2\2\u0128\u0129\7\16\2\2\u0129\u012c\7,\2\2\u012a\u012c\7-\2\2\u012b"+
		"\u0125\3\2\2\2\u012b\u0126\3\2\2\2\u012b\u0127\3\2\2\2\u012b\u0128\3\2"+
		"\2\2\u012b\u012a\3\2\2\2\u012c9\3\2\2\2\u012d\u012e\7\4\2\2\u012e\u012f"+
		"\5> \2\u012f\u0130\7\7\2\2\u0130\u013a\3\2\2\2\u0131\u0132\7\4\2\2\u0132"+
		"\u0133\5@!\2\u0133\u0134\7\7\2\2\u0134\u013a\3\2\2\2\u0135\u0136\7\4\2"+
		"\2\u0136\u013a\7\7\2\2\u0137\u0138\7+\2\2\u0138\u013a\5\66\34\2\u0139"+
		"\u012d\3\2\2\2\u0139\u0131\3\2\2\2\u0139\u0135\3\2\2\2\u0139\u0137\3\2"+
		"\2\2\u013a;\3\2\2\2\u013b\u0140\7+\2\2\u013c\u013d\7+\2\2\u013d\u013e"+
		"\7\17\2\2\u013e\u0140\7+\2\2\u013f\u013b\3\2\2\2\u013f\u013c\3\2\2\2\u0140"+
		"=\3\2\2\2\u0141\u0146\5\66\34\2\u0142\u0143\7\r\2\2\u0143\u0145\5\66\34"+
		"\2\u0144\u0142\3\2\2\2\u0145\u0148\3\2\2\2\u0146\u0144\3\2\2\2\u0146\u0147"+
		"\3\2\2\2\u0147?\3\2\2\2\u0148\u0146\3\2\2\2\u0149\u014e\5B\"\2\u014a\u014b"+
		"\7\r\2\2\u014b\u014d\5B\"\2\u014c\u014a\3\2\2\2\u014d\u0150\3\2\2\2\u014e"+
		"\u014c\3\2\2\2\u014e\u014f\3\2\2\2\u014fA\3\2\2\2\u0150\u014e\3\2\2\2"+
		"\u0151\u0152\5D#\2\u0152\u0153\7\3\2\2\u0153\u0154\5\66\34\2\u0154C\3"+
		"\2\2\2\u0155\u015a\7+\2\2\u0156\u0157\7\r\2\2\u0157\u0159\7+\2\2\u0158"+
		"\u0156\3\2\2\2\u0159\u015c\3\2\2\2\u015a\u0158\3\2\2\2\u015a\u015b\3\2"+
		"\2\2\u015bE\3\2\2\2\u015c\u015a\3\2\2\2\33]drt\u0084\u0089\u0094\u00c6"+
		"\u00ce\u00d5\u00df\u00e3\u00ea\u00f6\u0102\u010a\u0112\u0119\u0123\u012b"+
		"\u0139\u013f\u0146\u014e\u015a";
	public static final ATN _ATN =
		ATNSimulator.deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}