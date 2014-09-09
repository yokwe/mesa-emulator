// Generated from java-src/mh/bikini/courier/antlr/Courier.g by ANTLR 4.1
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
		T__3=10, T__2=11, T__1=12, T__0=13, ARRAY=14, BEGIN=15, BOOLEAN=16, CARDINAL=17, 
		CHOICE=18, DEPENDS=19, END=20, ERROR=21, INTEGER=22, LONG=23, OF=24, PROCEDURE=25, 
		PROGRAM=26, RECORD=27, REPORTS=28, RETURNS=29, SEQUENCE=30, STRING=31, 
		TYPE=32, UNSPECIFIED=33, UNSPECIFIED2=34, UNSPECIFIED3=35, UPON=36, VERSION=37, 
		TRUE=38, FALSE=39, ID=40, NUMBER=41, STR=42, COMMENT_PARTIAL=43, COMMENT_LINE=44, 
		SPACE=45;
	public static final String[] tokenNames = {
		"<INVALID>", "']'", "'=>'", "')'", "'.'", "','", "'['", "'-'", "'('", 
		"':'", "'='", "';'", "'{'", "'}'", "'ARRAY'", "'BEGIN'", "'BOOLEAN'", 
		"'CARDINAL'", "'CHOICE'", "'DEPENDS'", "'END'", "'ERROR'", "'INTEGER'", 
		"'LONG'", "'OF'", "'PROCEDURE'", "'PROGRAM'", "'RECORD'", "'REPORTS'", 
		"'RETURNS'", "'SEQUENCE'", "'STRING'", "'TYPE'", "'UNSPECIFIED'", "'UNSPECIFIED2'", 
		"'UNSPECIFIED3'", "'UPON'", "'VERSION'", "'TRUE'", "'FALSE'", "ID", "NUMBER", 
		"STR", "COMMENT_PARTIAL", "COMMENT_LINE", "SPACE"
	};
	public static final int
		RULE_courierProgram = 0, RULE_programHeader = 1, RULE_programBody = 2, 
		RULE_dependencyList = 3, RULE_referencedProgramList = 4, RULE_referencedProgram = 5, 
		RULE_declarationList = 6, RULE_declaration = 7, RULE_type = 8, RULE_predefinedType = 9, 
		RULE_constructedType = 10, RULE_referencedType = 11, RULE_correspondenceList = 12, 
		RULE_correspondence = 13, RULE_maximumNumber = 14, RULE_numericValue = 15, 
		RULE_typedCandidateList = 16, RULE_typedCandidate = 17, RULE_anonCandidateList = 18, 
		RULE_anonCandidate = 19, RULE_argumentList = 20, RULE_resultList = 21, 
		RULE_errorList = 22, RULE_fieldList = 23, RULE_field = 24, RULE_constant = 25, 
		RULE_predefinedConstant = 26, RULE_constructedConstant = 27, RULE_referencedConstant = 28, 
		RULE_elementList = 29, RULE_componentList = 30, RULE_component = 31, RULE_nameList = 32;
	public static final String[] ruleNames = {
		"courierProgram", "programHeader", "programBody", "dependencyList", "referencedProgramList", 
		"referencedProgram", "declarationList", "declaration", "type", "predefinedType", 
		"constructedType", "referencedType", "correspondenceList", "correspondence", 
		"maximumNumber", "numericValue", "typedCandidateList", "typedCandidate", 
		"anonCandidateList", "anonCandidate", "argumentList", "resultList", "errorList", 
		"fieldList", "field", "constant", "predefinedConstant", "constructedConstant", 
		"referencedConstant", "elementList", "componentList", "component", "nameList"
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
			setState(66); programHeader();
			setState(67); match(10);
			setState(68); programBody();
			setState(69); match(4);
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
		public TerminalNode PROGRAM() { return getToken(CourierParser.PROGRAM, 0); }
		public List<TerminalNode> NUMBER() { return getTokens(CourierParser.NUMBER); }
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
			setState(71); ((ProgramHeaderContext)_localctx).name = match(ID);
			setState(72); match(9);
			setState(73); match(PROGRAM);
			setState(74); ((ProgramHeaderContext)_localctx).program = match(NUMBER);
			setState(75); match(VERSION);
			setState(76); ((ProgramHeaderContext)_localctx).version = match(NUMBER);
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
		public DependencyListContext dependencyList() {
			return getRuleContext(DependencyListContext.class,0);
		}
		public TerminalNode BEGIN() { return getToken(CourierParser.BEGIN, 0); }
		public TerminalNode END() { return getToken(CourierParser.END, 0); }
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
			setState(78); match(BEGIN);
			setState(79); dependencyList();
			setState(80); declarationList();
			setState(81); match(END);
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
		public ReferencedProgramListContext referencedProgramList() {
			return getRuleContext(ReferencedProgramListContext.class,0);
		}
		public TerminalNode UPON() { return getToken(CourierParser.UPON, 0); }
		public TerminalNode DEPENDS() { return getToken(CourierParser.DEPENDS, 0); }
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
			setState(89);
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
				setState(84); match(DEPENDS);
				setState(85); match(UPON);
				setState(86); referencedProgramList();
				setState(87); match(11);
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
		public List<ReferencedProgramContext> referencedProgram() {
			return getRuleContexts(ReferencedProgramContext.class);
		}
		public ReferencedProgramContext referencedProgram(int i) {
			return getRuleContext(ReferencedProgramContext.class,i);
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
			setState(91); ((ReferencedProgramListContext)_localctx).referencedProgram = referencedProgram();
			((ReferencedProgramListContext)_localctx).elements.add(((ReferencedProgramListContext)_localctx).referencedProgram);
			setState(96);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==5) {
				{
				{
				setState(92); match(5);
				setState(93); ((ReferencedProgramListContext)_localctx).referencedProgram = referencedProgram();
				((ReferencedProgramListContext)_localctx).elements.add(((ReferencedProgramListContext)_localctx).referencedProgram);
				}
				}
				setState(98);
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
			setState(99); ((ReferencedProgramContext)_localctx).program = match(ID);
			setState(100); match(8);
			setState(101); ((ReferencedProgramContext)_localctx).number = match(NUMBER);
			setState(102); match(3);
			setState(103); match(VERSION);
			setState(104); ((ReferencedProgramContext)_localctx).version = match(NUMBER);
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
			setState(112);
			switch (_input.LA(1)) {
			case END:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case ID:
				enterOuterAlt(_localctx, 2);
				{
				setState(108); 
				_errHandler.sync(this);
				_la = _input.LA(1);
				do {
					{
					{
					setState(107); ((DeclarationListContext)_localctx).declaration = declaration();
					((DeclarationListContext)_localctx).elements.add(((DeclarationListContext)_localctx).declaration);
					}
					}
					setState(110); 
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
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public ConstantContext constant() {
			return getRuleContext(ConstantContext.class,0);
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
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public TerminalNode TYPE() { return getToken(CourierParser.TYPE, 0); }
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
			setState(128);
			switch ( getInterpreter().adaptivePredict(_input,4,_ctx) ) {
			case 1:
				_localctx = new DeclTypeContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(114); ((DeclTypeContext)_localctx).name = match(ID);
				setState(115); match(9);
				setState(116); match(TYPE);
				setState(117); match(10);
				setState(118); type();
				setState(119); match(11);
				}
				break;

			case 2:
				_localctx = new DeclConstContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(121); ((DeclConstContext)_localctx).name = match(ID);
				setState(122); match(9);
				setState(123); type();
				setState(124); match(10);
				setState(125); constant();
				setState(126); match(11);
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
			setState(133);
			switch (_input.LA(1)) {
			case BOOLEAN:
			case CARDINAL:
			case LONG:
			case STRING:
			case UNSPECIFIED:
			case UNSPECIFIED2:
			case UNSPECIFIED3:
				enterOuterAlt(_localctx, 1);
				{
				setState(130); predefinedType();
				}
				break;
			case 12:
			case ARRAY:
			case CHOICE:
			case ERROR:
			case PROCEDURE:
			case RECORD:
			case SEQUENCE:
				enterOuterAlt(_localctx, 2);
				{
				setState(131); constructedType();
				}
				break;
			case ID:
				enterOuterAlt(_localctx, 3);
				{
				setState(132); referencedType();
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
	public static class TypeBooleanContext extends PredefinedTypeContext {
		public TerminalNode BOOLEAN() { return getToken(CourierParser.BOOLEAN, 0); }
		public TypeBooleanContext(PredefinedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeBoolean(this);
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
	public static class TypeUnspecified3Context extends PredefinedTypeContext {
		public TerminalNode UNSPECIFIED3() { return getToken(CourierParser.UNSPECIFIED3, 0); }
		public TypeUnspecified3Context(PredefinedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeUnspecified3(this);
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
	public static class TypeUnspecified2Context extends PredefinedTypeContext {
		public TerminalNode UNSPECIFIED2() { return getToken(CourierParser.UNSPECIFIED2, 0); }
		public TypeUnspecified2Context(PredefinedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeUnspecified2(this);
			else return visitor.visitChildren(this);
		}
	}

	public final PredefinedTypeContext predefinedType() throws RecognitionException {
		PredefinedTypeContext _localctx = new PredefinedTypeContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_predefinedType);
		try {
			setState(143);
			switch (_input.LA(1)) {
			case BOOLEAN:
				_localctx = new TypeBooleanContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(135); match(BOOLEAN);
				}
				break;
			case CARDINAL:
				_localctx = new TypeCardinalContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(136); match(CARDINAL);
				}
				break;
			case LONG:
				_localctx = new TypeLongCardinalContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(137); match(LONG);
				setState(138); match(CARDINAL);
				}
				break;
			case STRING:
				_localctx = new TypeStringContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(139); match(STRING);
				}
				break;
			case UNSPECIFIED:
				_localctx = new TypeUnspecifiedContext(_localctx);
				enterOuterAlt(_localctx, 5);
				{
				setState(140); match(UNSPECIFIED);
				}
				break;
			case UNSPECIFIED2:
				_localctx = new TypeUnspecified2Context(_localctx);
				enterOuterAlt(_localctx, 6);
				{
				setState(141); match(UNSPECIFIED2);
				}
				break;
			case UNSPECIFIED3:
				_localctx = new TypeUnspecified3Context(_localctx);
				enterOuterAlt(_localctx, 7);
				{
				setState(142); match(UNSPECIFIED3);
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
		public ArgumentListContext argumentList() {
			return getRuleContext(ArgumentListContext.class,0);
		}
		public TerminalNode PROCEDURE() { return getToken(CourierParser.PROCEDURE, 0); }
		public ErrorListContext errorList() {
			return getRuleContext(ErrorListContext.class,0);
		}
		public ResultListContext resultList() {
			return getRuleContext(ResultListContext.class,0);
		}
		public TypeProcedureContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeProcedure(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeErrorContext extends ConstructedTypeContext {
		public ArgumentListContext argumentList() {
			return getRuleContext(ArgumentListContext.class,0);
		}
		public TerminalNode ERROR() { return getToken(CourierParser.ERROR, 0); }
		public TypeErrorContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeError(this);
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
	public static class TypeEnumContext extends ConstructedTypeContext {
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
	public static class TypeChoiceTypedContext extends ConstructedTypeContext {
		public TerminalNode CHOICE() { return getToken(CourierParser.CHOICE, 0); }
		public ReferencedTypeContext referencedType() {
			return getRuleContext(ReferencedTypeContext.class,0);
		}
		public TypedCandidateListContext typedCandidateList() {
			return getRuleContext(TypedCandidateListContext.class,0);
		}
		public TerminalNode OF() { return getToken(CourierParser.OF, 0); }
		public TypeChoiceTypedContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeChoiceTyped(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeArrayContext extends ConstructedTypeContext {
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public TerminalNode OF() { return getToken(CourierParser.OF, 0); }
		public NumericValueContext numericValue() {
			return getRuleContext(NumericValueContext.class,0);
		}
		public TerminalNode ARRAY() { return getToken(CourierParser.ARRAY, 0); }
		public TypeArrayContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeArray(this);
			else return visitor.visitChildren(this);
		}
	}
	public static class TypeSequenceContext extends ConstructedTypeContext {
		public MaximumNumberContext maximumNumber() {
			return getRuleContext(MaximumNumberContext.class,0);
		}
		public TerminalNode SEQUENCE() { return getToken(CourierParser.SEQUENCE, 0); }
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
		}
		public TerminalNode OF() { return getToken(CourierParser.OF, 0); }
		public TypeSequenceContext(ConstructedTypeContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitTypeSequence(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ConstructedTypeContext constructedType() throws RecognitionException {
		ConstructedTypeContext _localctx = new ConstructedTypeContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_constructedType);
		try {
			setState(187);
			switch ( getInterpreter().adaptivePredict(_input,7,_ctx) ) {
			case 1:
				_localctx = new TypeEnumContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(145); match(12);
				setState(146); correspondenceList();
				setState(147); match(13);
				}
				break;

			case 2:
				_localctx = new TypeArrayContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(149); match(ARRAY);
				setState(150); numericValue();
				setState(151); match(OF);
				setState(152); type();
				}
				break;

			case 3:
				_localctx = new TypeSequenceContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(154); match(SEQUENCE);
				setState(155); maximumNumber();
				setState(156); match(OF);
				setState(157); type();
				}
				break;

			case 4:
				_localctx = new TypeRecordContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(159); match(RECORD);
				setState(160); match(6);
				setState(161); fieldList();
				setState(162); match(1);
				}
				break;

			case 5:
				_localctx = new TypeEmptyRecordContext(_localctx);
				enterOuterAlt(_localctx, 5);
				{
				setState(164); match(RECORD);
				setState(165); match(6);
				setState(166); match(1);
				}
				break;

			case 6:
				_localctx = new TypeChoiceTypedContext(_localctx);
				enterOuterAlt(_localctx, 6);
				{
				setState(167); match(CHOICE);
				setState(168); referencedType();
				setState(169); match(OF);
				setState(170); match(12);
				setState(171); typedCandidateList();
				setState(172); match(13);
				}
				break;

			case 7:
				_localctx = new TypeChoiceAnonContext(_localctx);
				enterOuterAlt(_localctx, 7);
				{
				setState(174); match(CHOICE);
				setState(175); match(OF);
				setState(176); match(12);
				setState(177); anonCandidateList();
				setState(178); match(13);
				}
				break;

			case 8:
				_localctx = new TypeProcedureContext(_localctx);
				enterOuterAlt(_localctx, 8);
				{
				setState(180); match(PROCEDURE);
				setState(181); argumentList();
				setState(182); resultList();
				setState(183); errorList();
				}
				break;

			case 9:
				_localctx = new TypeErrorContext(_localctx);
				enterOuterAlt(_localctx, 9);
				{
				setState(185); match(ERROR);
				setState(186); argumentList();
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
		enterRule(_localctx, 22, RULE_referencedType);
		try {
			setState(193);
			switch ( getInterpreter().adaptivePredict(_input,8,_ctx) ) {
			case 1:
				_localctx = new TypeRefContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(189); ((TypeRefContext)_localctx).name = match(ID);
				}
				break;

			case 2:
				_localctx = new TypeRefQContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(190); ((TypeRefQContext)_localctx).program = match(ID);
				setState(191); match(4);
				setState(192); ((TypeRefQContext)_localctx).name = match(ID);
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
		enterRule(_localctx, 24, RULE_correspondenceList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(195); ((CorrespondenceListContext)_localctx).correspondence = correspondence();
			((CorrespondenceListContext)_localctx).elements.add(((CorrespondenceListContext)_localctx).correspondence);
			setState(200);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==5) {
				{
				{
				setState(196); match(5);
				setState(197); ((CorrespondenceListContext)_localctx).correspondence = correspondence();
				((CorrespondenceListContext)_localctx).elements.add(((CorrespondenceListContext)_localctx).correspondence);
				}
				}
				setState(202);
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
		enterRule(_localctx, 26, RULE_correspondence);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(203); match(ID);
			setState(204); match(8);
			setState(205); numericValue();
			setState(206); match(3);
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
		enterRule(_localctx, 28, RULE_maximumNumber);
		try {
			setState(210);
			switch (_input.LA(1)) {
			case OF:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case NUMBER:
				enterOuterAlt(_localctx, 2);
				{
				setState(209); numericValue();
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
		enterRule(_localctx, 30, RULE_numericValue);
		try {
			_localctx = new ValueNumberContext(_localctx);
			enterOuterAlt(_localctx, 1);
			{
			setState(212); match(NUMBER);
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
		public List<TypedCandidateContext> typedCandidate() {
			return getRuleContexts(TypedCandidateContext.class);
		}
		public TypedCandidateContext typedCandidate(int i) {
			return getRuleContext(TypedCandidateContext.class,i);
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
		enterRule(_localctx, 32, RULE_typedCandidateList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(214); ((TypedCandidateListContext)_localctx).typedCandidate = typedCandidate();
			((TypedCandidateListContext)_localctx).elements.add(((TypedCandidateListContext)_localctx).typedCandidate);
			setState(219);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==5) {
				{
				{
				setState(215); match(5);
				setState(216); ((TypedCandidateListContext)_localctx).typedCandidate = typedCandidate();
				((TypedCandidateListContext)_localctx).elements.add(((TypedCandidateListContext)_localctx).typedCandidate);
				}
				}
				setState(221);
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
		public NameListContext nameList() {
			return getRuleContext(NameListContext.class,0);
		}
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
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
		enterRule(_localctx, 34, RULE_typedCandidate);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(222); nameList();
			setState(223); match(2);
			setState(224); type();
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
		public List<AnonCandidateContext> anonCandidate() {
			return getRuleContexts(AnonCandidateContext.class);
		}
		public AnonCandidateContext anonCandidate(int i) {
			return getRuleContext(AnonCandidateContext.class,i);
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
		enterRule(_localctx, 36, RULE_anonCandidateList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(226); ((AnonCandidateListContext)_localctx).anonCandidate = anonCandidate();
			((AnonCandidateListContext)_localctx).elements.add(((AnonCandidateListContext)_localctx).anonCandidate);
			setState(231);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==5) {
				{
				{
				setState(227); match(5);
				setState(228); ((AnonCandidateListContext)_localctx).anonCandidate = anonCandidate();
				((AnonCandidateListContext)_localctx).elements.add(((AnonCandidateListContext)_localctx).anonCandidate);
				}
				}
				setState(233);
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
		enterRule(_localctx, 38, RULE_anonCandidate);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(234); correspondenceList();
			setState(235); match(2);
			setState(236); type();
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
		enterRule(_localctx, 40, RULE_argumentList);
		try {
			setState(243);
			switch (_input.LA(1)) {
			case 1:
			case 5:
			case 10:
			case 11:
			case 13:
			case REPORTS:
			case RETURNS:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 2);
				{
				setState(239); match(6);
				setState(240); fieldList();
				setState(241); match(1);
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
		enterRule(_localctx, 42, RULE_resultList);
		try {
			setState(251);
			switch (_input.LA(1)) {
			case 1:
			case 5:
			case 10:
			case 11:
			case 13:
			case REPORTS:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case RETURNS:
				enterOuterAlt(_localctx, 2);
				{
				setState(246); match(RETURNS);
				setState(247); match(6);
				setState(248); fieldList();
				setState(249); match(1);
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
		public NameListContext nameList() {
			return getRuleContext(NameListContext.class,0);
		}
		public TerminalNode REPORTS() { return getToken(CourierParser.REPORTS, 0); }
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
		enterRule(_localctx, 44, RULE_errorList);
		try {
			setState(259);
			switch (_input.LA(1)) {
			case 1:
			case 5:
			case 10:
			case 11:
			case 13:
				enterOuterAlt(_localctx, 1);
				{
				}
				break;
			case REPORTS:
				enterOuterAlt(_localctx, 2);
				{
				setState(254); match(REPORTS);
				setState(255); match(6);
				setState(256); nameList();
				setState(257); match(1);
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
		enterRule(_localctx, 46, RULE_fieldList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(261); ((FieldListContext)_localctx).field = field();
			((FieldListContext)_localctx).elements.add(((FieldListContext)_localctx).field);
			setState(266);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==5) {
				{
				{
				setState(262); match(5);
				setState(263); ((FieldListContext)_localctx).field = field();
				((FieldListContext)_localctx).elements.add(((FieldListContext)_localctx).field);
				}
				}
				setState(268);
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
		public NameListContext nameList() {
			return getRuleContext(NameListContext.class,0);
		}
		public TypeContext type() {
			return getRuleContext(TypeContext.class,0);
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
		enterRule(_localctx, 48, RULE_field);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(269); nameList();
			setState(270); match(9);
			setState(271); type();
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
		public ReferencedConstantContext referencedConstant() {
			return getRuleContext(ReferencedConstantContext.class,0);
		}
		public ConstructedConstantContext constructedConstant() {
			return getRuleContext(ConstructedConstantContext.class,0);
		}
		public PredefinedConstantContext predefinedConstant() {
			return getRuleContext(PredefinedConstantContext.class,0);
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
		enterRule(_localctx, 50, RULE_constant);
		try {
			setState(276);
			switch ( getInterpreter().adaptivePredict(_input,17,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(273); predefinedConstant();
				}
				break;

			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(274); constructedConstant();
				}
				break;

			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(275); referencedConstant();
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
	public static class ConstNumberNegativeContext extends PredefinedConstantContext {
		public TerminalNode NUMBER() { return getToken(CourierParser.NUMBER, 0); }
		public ConstNumberNegativeContext(PredefinedConstantContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstNumberNegative(this);
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
	public static class ConstTrueContext extends PredefinedConstantContext {
		public TerminalNode TRUE() { return getToken(CourierParser.TRUE, 0); }
		public ConstTrueContext(PredefinedConstantContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstTrue(this);
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
	public static class ConstStringContext extends PredefinedConstantContext {
		public TerminalNode STR() { return getToken(CourierParser.STR, 0); }
		public ConstStringContext(PredefinedConstantContext ctx) { copyFrom(ctx); }
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof CourierVisitor ) return ((CourierVisitor<? extends T>)visitor).visitConstString(this);
			else return visitor.visitChildren(this);
		}
	}

	public final PredefinedConstantContext predefinedConstant() throws RecognitionException {
		PredefinedConstantContext _localctx = new PredefinedConstantContext(_ctx, getState());
		enterRule(_localctx, 52, RULE_predefinedConstant);
		try {
			setState(284);
			switch (_input.LA(1)) {
			case TRUE:
				_localctx = new ConstTrueContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(278); match(TRUE);
				}
				break;
			case FALSE:
				_localctx = new ConstFalseContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(279); match(FALSE);
				}
				break;
			case NUMBER:
				_localctx = new ConstNumberContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(280); match(NUMBER);
				}
				break;
			case 7:
				_localctx = new ConstNumberNegativeContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(281); match(7);
				setState(282); match(NUMBER);
				}
				break;
			case STR:
				_localctx = new ConstStringContext(_localctx);
				enterOuterAlt(_localctx, 5);
				{
				setState(283); match(STR);
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
		enterRule(_localctx, 54, RULE_constructedConstant);
		try {
			setState(298);
			switch ( getInterpreter().adaptivePredict(_input,19,_ctx) ) {
			case 1:
				_localctx = new ConstArrayContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(286); match(6);
				setState(287); elementList();
				setState(288); match(1);
				}
				break;

			case 2:
				_localctx = new ConstRecordContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(290); match(6);
				setState(291); componentList();
				setState(292); match(1);
				}
				break;

			case 3:
				_localctx = new ConstEmptyContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(294); match(6);
				setState(295); match(1);
				}
				break;

			case 4:
				_localctx = new ConstChoiceContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(296); match(ID);
				setState(297); constant();
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
		enterRule(_localctx, 56, RULE_referencedConstant);
		try {
			setState(304);
			switch ( getInterpreter().adaptivePredict(_input,20,_ctx) ) {
			case 1:
				_localctx = new ConstRefContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(300); ((ConstRefContext)_localctx).name = match(ID);
				}
				break;

			case 2:
				_localctx = new ConstRefQContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(301); ((ConstRefQContext)_localctx).program = match(ID);
				setState(302); match(4);
				setState(303); ((ConstRefQContext)_localctx).name = match(ID);
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
		public ConstantContext constant(int i) {
			return getRuleContext(ConstantContext.class,i);
		}
		public List<ConstantContext> constant() {
			return getRuleContexts(ConstantContext.class);
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
		enterRule(_localctx, 58, RULE_elementList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(306); ((ElementListContext)_localctx).constant = constant();
			((ElementListContext)_localctx).elements.add(((ElementListContext)_localctx).constant);
			setState(311);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==5) {
				{
				{
				setState(307); match(5);
				setState(308); ((ElementListContext)_localctx).constant = constant();
				((ElementListContext)_localctx).elements.add(((ElementListContext)_localctx).constant);
				}
				}
				setState(313);
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
		enterRule(_localctx, 60, RULE_componentList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(314); ((ComponentListContext)_localctx).component = component();
			((ComponentListContext)_localctx).elements.add(((ComponentListContext)_localctx).component);
			setState(319);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==5) {
				{
				{
				setState(315); match(5);
				setState(316); ((ComponentListContext)_localctx).component = component();
				((ComponentListContext)_localctx).elements.add(((ComponentListContext)_localctx).component);
				}
				}
				setState(321);
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
		public NameListContext nameList() {
			return getRuleContext(NameListContext.class,0);
		}
		public ConstantContext constant() {
			return getRuleContext(ConstantContext.class,0);
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
		enterRule(_localctx, 62, RULE_component);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(322); nameList();
			setState(323); match(9);
			setState(324); constant();
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
		enterRule(_localctx, 64, RULE_nameList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(326); ((NameListContext)_localctx).ID = match(ID);
			((NameListContext)_localctx).elements.add(((NameListContext)_localctx).ID);
			setState(331);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==5) {
				{
				{
				setState(327); match(5);
				setState(328); ((NameListContext)_localctx).ID = match(ID);
				((NameListContext)_localctx).elements.add(((NameListContext)_localctx).ID);
				}
				}
				setState(333);
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
		"\3\uacf5\uee8c\u4f5d\u8b0d\u4a45\u78bd\u1b2f\u3378\3/\u0151\4\2\t\2\4"+
		"\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t"+
		"\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\4\"\t\"\3\2\3\2\3\2\3\2\3\2\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\4\3\4\3"+
		"\4\3\4\3\4\3\5\3\5\3\5\3\5\3\5\3\5\5\5\\\n\5\3\6\3\6\3\6\7\6a\n\6\f\6"+
		"\16\6d\13\6\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\b\3\b\6\bo\n\b\r\b\16\bp\5\b"+
		"s\n\b\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\5\t\u0083"+
		"\n\t\3\n\3\n\3\n\5\n\u0088\n\n\3\13\3\13\3\13\3\13\3\13\3\13\3\13\3\13"+
		"\5\13\u0092\n\13\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3"+
		"\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f"+
		"\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\5\f\u00be\n\f\3\r\3\r\3\r"+
		"\3\r\5\r\u00c4\n\r\3\16\3\16\3\16\7\16\u00c9\n\16\f\16\16\16\u00cc\13"+
		"\16\3\17\3\17\3\17\3\17\3\17\3\20\3\20\5\20\u00d5\n\20\3\21\3\21\3\22"+
		"\3\22\3\22\7\22\u00dc\n\22\f\22\16\22\u00df\13\22\3\23\3\23\3\23\3\23"+
		"\3\24\3\24\3\24\7\24\u00e8\n\24\f\24\16\24\u00eb\13\24\3\25\3\25\3\25"+
		"\3\25\3\26\3\26\3\26\3\26\3\26\5\26\u00f6\n\26\3\27\3\27\3\27\3\27\3\27"+
		"\3\27\5\27\u00fe\n\27\3\30\3\30\3\30\3\30\3\30\3\30\5\30\u0106\n\30\3"+
		"\31\3\31\3\31\7\31\u010b\n\31\f\31\16\31\u010e\13\31\3\32\3\32\3\32\3"+
		"\32\3\33\3\33\3\33\5\33\u0117\n\33\3\34\3\34\3\34\3\34\3\34\3\34\5\34"+
		"\u011f\n\34\3\35\3\35\3\35\3\35\3\35\3\35\3\35\3\35\3\35\3\35\3\35\3\35"+
		"\5\35\u012d\n\35\3\36\3\36\3\36\3\36\5\36\u0133\n\36\3\37\3\37\3\37\7"+
		"\37\u0138\n\37\f\37\16\37\u013b\13\37\3 \3 \3 \7 \u0140\n \f \16 \u0143"+
		"\13 \3!\3!\3!\3!\3\"\3\"\3\"\7\"\u014c\n\"\f\"\16\"\u014f\13\"\3\"\2#"+
		"\2\4\6\b\n\f\16\20\22\24\26\30\32\34\36 \"$&(*,.\60\62\64\668:<>@B\2\2"+
		"\u015a\2D\3\2\2\2\4I\3\2\2\2\6P\3\2\2\2\b[\3\2\2\2\n]\3\2\2\2\fe\3\2\2"+
		"\2\16r\3\2\2\2\20\u0082\3\2\2\2\22\u0087\3\2\2\2\24\u0091\3\2\2\2\26\u00bd"+
		"\3\2\2\2\30\u00c3\3\2\2\2\32\u00c5\3\2\2\2\34\u00cd\3\2\2\2\36\u00d4\3"+
		"\2\2\2 \u00d6\3\2\2\2\"\u00d8\3\2\2\2$\u00e0\3\2\2\2&\u00e4\3\2\2\2(\u00ec"+
		"\3\2\2\2*\u00f5\3\2\2\2,\u00fd\3\2\2\2.\u0105\3\2\2\2\60\u0107\3\2\2\2"+
		"\62\u010f\3\2\2\2\64\u0116\3\2\2\2\66\u011e\3\2\2\28\u012c\3\2\2\2:\u0132"+
		"\3\2\2\2<\u0134\3\2\2\2>\u013c\3\2\2\2@\u0144\3\2\2\2B\u0148\3\2\2\2D"+
		"E\5\4\3\2EF\7\f\2\2FG\5\6\4\2GH\7\6\2\2H\3\3\2\2\2IJ\7*\2\2JK\7\13\2\2"+
		"KL\7\34\2\2LM\7+\2\2MN\7\'\2\2NO\7+\2\2O\5\3\2\2\2PQ\7\21\2\2QR\5\b\5"+
		"\2RS\5\16\b\2ST\7\26\2\2T\7\3\2\2\2U\\\3\2\2\2VW\7\25\2\2WX\7&\2\2XY\5"+
		"\n\6\2YZ\7\r\2\2Z\\\3\2\2\2[U\3\2\2\2[V\3\2\2\2\\\t\3\2\2\2]b\5\f\7\2"+
		"^_\7\7\2\2_a\5\f\7\2`^\3\2\2\2ad\3\2\2\2b`\3\2\2\2bc\3\2\2\2c\13\3\2\2"+
		"\2db\3\2\2\2ef\7*\2\2fg\7\n\2\2gh\7+\2\2hi\7\5\2\2ij\7\'\2\2jk\7+\2\2"+
		"k\r\3\2\2\2ls\3\2\2\2mo\5\20\t\2nm\3\2\2\2op\3\2\2\2pn\3\2\2\2pq\3\2\2"+
		"\2qs\3\2\2\2rl\3\2\2\2rn\3\2\2\2s\17\3\2\2\2tu\7*\2\2uv\7\13\2\2vw\7\""+
		"\2\2wx\7\f\2\2xy\5\22\n\2yz\7\r\2\2z\u0083\3\2\2\2{|\7*\2\2|}\7\13\2\2"+
		"}~\5\22\n\2~\177\7\f\2\2\177\u0080\5\64\33\2\u0080\u0081\7\r\2\2\u0081"+
		"\u0083\3\2\2\2\u0082t\3\2\2\2\u0082{\3\2\2\2\u0083\21\3\2\2\2\u0084\u0088"+
		"\5\24\13\2\u0085\u0088\5\26\f\2\u0086\u0088\5\30\r\2\u0087\u0084\3\2\2"+
		"\2\u0087\u0085\3\2\2\2\u0087\u0086\3\2\2\2\u0088\23\3\2\2\2\u0089\u0092"+
		"\7\22\2\2\u008a\u0092\7\23\2\2\u008b\u008c\7\31\2\2\u008c\u0092\7\23\2"+
		"\2\u008d\u0092\7!\2\2\u008e\u0092\7#\2\2\u008f\u0092\7$\2\2\u0090\u0092"+
		"\7%\2\2\u0091\u0089\3\2\2\2\u0091\u008a\3\2\2\2\u0091\u008b\3\2\2\2\u0091"+
		"\u008d\3\2\2\2\u0091\u008e\3\2\2\2\u0091\u008f\3\2\2\2\u0091\u0090\3\2"+
		"\2\2\u0092\25\3\2\2\2\u0093\u0094\7\16\2\2\u0094\u0095\5\32\16\2\u0095"+
		"\u0096\7\17\2\2\u0096\u00be\3\2\2\2\u0097\u0098\7\20\2\2\u0098\u0099\5"+
		" \21\2\u0099\u009a\7\32\2\2\u009a\u009b\5\22\n\2\u009b\u00be\3\2\2\2\u009c"+
		"\u009d\7 \2\2\u009d\u009e\5\36\20\2\u009e\u009f\7\32\2\2\u009f\u00a0\5"+
		"\22\n\2\u00a0\u00be\3\2\2\2\u00a1\u00a2\7\35\2\2\u00a2\u00a3\7\b\2\2\u00a3"+
		"\u00a4\5\60\31\2\u00a4\u00a5\7\3\2\2\u00a5\u00be\3\2\2\2\u00a6\u00a7\7"+
		"\35\2\2\u00a7\u00a8\7\b\2\2\u00a8\u00be\7\3\2\2\u00a9\u00aa\7\24\2\2\u00aa"+
		"\u00ab\5\30\r\2\u00ab\u00ac\7\32\2\2\u00ac\u00ad\7\16\2\2\u00ad\u00ae"+
		"\5\"\22\2\u00ae\u00af\7\17\2\2\u00af\u00be\3\2\2\2\u00b0\u00b1\7\24\2"+
		"\2\u00b1\u00b2\7\32\2\2\u00b2\u00b3\7\16\2\2\u00b3\u00b4\5&\24\2\u00b4"+
		"\u00b5\7\17\2\2\u00b5\u00be\3\2\2\2\u00b6\u00b7\7\33\2\2\u00b7\u00b8\5"+
		"*\26\2\u00b8\u00b9\5,\27\2\u00b9\u00ba\5.\30\2\u00ba\u00be\3\2\2\2\u00bb"+
		"\u00bc\7\27\2\2\u00bc\u00be\5*\26\2\u00bd\u0093\3\2\2\2\u00bd\u0097\3"+
		"\2\2\2\u00bd\u009c\3\2\2\2\u00bd\u00a1\3\2\2\2\u00bd\u00a6\3\2\2\2\u00bd"+
		"\u00a9\3\2\2\2\u00bd\u00b0\3\2\2\2\u00bd\u00b6\3\2\2\2\u00bd\u00bb\3\2"+
		"\2\2\u00be\27\3\2\2\2\u00bf\u00c4\7*\2\2\u00c0\u00c1\7*\2\2\u00c1\u00c2"+
		"\7\6\2\2\u00c2\u00c4\7*\2\2\u00c3\u00bf\3\2\2\2\u00c3\u00c0\3\2\2\2\u00c4"+
		"\31\3\2\2\2\u00c5\u00ca\5\34\17\2\u00c6\u00c7\7\7\2\2\u00c7\u00c9\5\34"+
		"\17\2\u00c8\u00c6\3\2\2\2\u00c9\u00cc\3\2\2\2\u00ca\u00c8\3\2\2\2\u00ca"+
		"\u00cb\3\2\2\2\u00cb\33\3\2\2\2\u00cc\u00ca\3\2\2\2\u00cd\u00ce\7*\2\2"+
		"\u00ce\u00cf\7\n\2\2\u00cf\u00d0\5 \21\2\u00d0\u00d1\7\5\2\2\u00d1\35"+
		"\3\2\2\2\u00d2\u00d5\3\2\2\2\u00d3\u00d5\5 \21\2\u00d4\u00d2\3\2\2\2\u00d4"+
		"\u00d3\3\2\2\2\u00d5\37\3\2\2\2\u00d6\u00d7\7+\2\2\u00d7!\3\2\2\2\u00d8"+
		"\u00dd\5$\23\2\u00d9\u00da\7\7\2\2\u00da\u00dc\5$\23\2\u00db\u00d9\3\2"+
		"\2\2\u00dc\u00df\3\2\2\2\u00dd\u00db\3\2\2\2\u00dd\u00de\3\2\2\2\u00de"+
		"#\3\2\2\2\u00df\u00dd\3\2\2\2\u00e0\u00e1\5B\"\2\u00e1\u00e2\7\4\2\2\u00e2"+
		"\u00e3\5\22\n\2\u00e3%\3\2\2\2\u00e4\u00e9\5(\25\2\u00e5\u00e6\7\7\2\2"+
		"\u00e6\u00e8\5(\25\2\u00e7\u00e5\3\2\2\2\u00e8\u00eb\3\2\2\2\u00e9\u00e7"+
		"\3\2\2\2\u00e9\u00ea\3\2\2\2\u00ea\'\3\2\2\2\u00eb\u00e9\3\2\2\2\u00ec"+
		"\u00ed\5\32\16\2\u00ed\u00ee\7\4\2\2\u00ee\u00ef\5\22\n\2\u00ef)\3\2\2"+
		"\2\u00f0\u00f6\3\2\2\2\u00f1\u00f2\7\b\2\2\u00f2\u00f3\5\60\31\2\u00f3"+
		"\u00f4\7\3\2\2\u00f4\u00f6\3\2\2\2\u00f5\u00f0\3\2\2\2\u00f5\u00f1\3\2"+
		"\2\2\u00f6+\3\2\2\2\u00f7\u00fe\3\2\2\2\u00f8\u00f9\7\37\2\2\u00f9\u00fa"+
		"\7\b\2\2\u00fa\u00fb\5\60\31\2\u00fb\u00fc\7\3\2\2\u00fc\u00fe\3\2\2\2"+
		"\u00fd\u00f7\3\2\2\2\u00fd\u00f8\3\2\2\2\u00fe-\3\2\2\2\u00ff\u0106\3"+
		"\2\2\2\u0100\u0101\7\36\2\2\u0101\u0102\7\b\2\2\u0102\u0103\5B\"\2\u0103"+
		"\u0104\7\3\2\2\u0104\u0106\3\2\2\2\u0105\u00ff\3\2\2\2\u0105\u0100\3\2"+
		"\2\2\u0106/\3\2\2\2\u0107\u010c\5\62\32\2\u0108\u0109\7\7\2\2\u0109\u010b"+
		"\5\62\32\2\u010a\u0108\3\2\2\2\u010b\u010e\3\2\2\2\u010c\u010a\3\2\2\2"+
		"\u010c\u010d\3\2\2\2\u010d\61\3\2\2\2\u010e\u010c\3\2\2\2\u010f\u0110"+
		"\5B\"\2\u0110\u0111\7\13\2\2\u0111\u0112\5\22\n\2\u0112\63\3\2\2\2\u0113"+
		"\u0117\5\66\34\2\u0114\u0117\58\35\2\u0115\u0117\5:\36\2\u0116\u0113\3"+
		"\2\2\2\u0116\u0114\3\2\2\2\u0116\u0115\3\2\2\2\u0117\65\3\2\2\2\u0118"+
		"\u011f\7(\2\2\u0119\u011f\7)\2\2\u011a\u011f\7+\2\2\u011b\u011c\7\t\2"+
		"\2\u011c\u011f\7+\2\2\u011d\u011f\7,\2\2\u011e\u0118\3\2\2\2\u011e\u0119"+
		"\3\2\2\2\u011e\u011a\3\2\2\2\u011e\u011b\3\2\2\2\u011e\u011d\3\2\2\2\u011f"+
		"\67\3\2\2\2\u0120\u0121\7\b\2\2\u0121\u0122\5<\37\2\u0122\u0123\7\3\2"+
		"\2\u0123\u012d\3\2\2\2\u0124\u0125\7\b\2\2\u0125\u0126\5> \2\u0126\u0127"+
		"\7\3\2\2\u0127\u012d\3\2\2\2\u0128\u0129\7\b\2\2\u0129\u012d\7\3\2\2\u012a"+
		"\u012b\7*\2\2\u012b\u012d\5\64\33\2\u012c\u0120\3\2\2\2\u012c\u0124\3"+
		"\2\2\2\u012c\u0128\3\2\2\2\u012c\u012a\3\2\2\2\u012d9\3\2\2\2\u012e\u0133"+
		"\7*\2\2\u012f\u0130\7*\2\2\u0130\u0131\7\6\2\2\u0131\u0133\7*\2\2\u0132"+
		"\u012e\3\2\2\2\u0132\u012f\3\2\2\2\u0133;\3\2\2\2\u0134\u0139\5\64\33"+
		"\2\u0135\u0136\7\7\2\2\u0136\u0138\5\64\33\2\u0137\u0135\3\2\2\2\u0138"+
		"\u013b\3\2\2\2\u0139\u0137\3\2\2\2\u0139\u013a\3\2\2\2\u013a=\3\2\2\2"+
		"\u013b\u0139\3\2\2\2\u013c\u0141\5@!\2\u013d\u013e\7\7\2\2\u013e\u0140"+
		"\5@!\2\u013f\u013d\3\2\2\2\u0140\u0143\3\2\2\2\u0141\u013f\3\2\2\2\u0141"+
		"\u0142\3\2\2\2\u0142?\3\2\2\2\u0143\u0141\3\2\2\2\u0144\u0145\5B\"\2\u0145"+
		"\u0146\7\13\2\2\u0146\u0147\5\64\33\2\u0147A\3\2\2\2\u0148\u014d\7*\2"+
		"\2\u0149\u014a\7\7\2\2\u014a\u014c\7*\2\2\u014b\u0149\3\2\2\2\u014c\u014f"+
		"\3\2\2\2\u014d\u014b\3\2\2\2\u014d\u014e\3\2\2\2\u014eC\3\2\2\2\u014f"+
		"\u014d\3\2\2\2\32[bpr\u0082\u0087\u0091\u00bd\u00c3\u00ca\u00d4\u00dd"+
		"\u00e9\u00f5\u00fd\u0105\u010c\u0116\u011e\u012c\u0132\u0139\u0141\u014d";
	public static final ATN _ATN =
		ATNSimulator.deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}