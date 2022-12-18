// Generated from c:\School_3rdyr\Gradwork\Project\Compiler\Antlr\grammar\Cricket.g4 by ANTLR 4.9.2
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class CricketParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.9.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, WS=5, TReturn=6, Number=7, CharLiteral=8, 
		TInt=9, TChar=10, TVoid=11, LessThan=12, GreaterThan=13, BitwiseAnd=14, 
		BitwiseOr=15, BitwiseXor=16, Equal=17, NotEqual=18, LessOrEqual=19, GreaterOrEqual=20, 
		Assign=21, Colon=22, StatementSeparator=23, Plus=24, Minus=25, Mul=26, 
		Div=27, Mod=28, PlusEqual=29, MinusEqual=30, MulEqual=31, DivEqual=32, 
		OpenPar=33, ClosePar=34, OpenCurly=35, CloseCurly=36, ExclamationMark=37, 
		Comma=38, Identifier=39, Comment=40, IncludeDirective=41, Newline=42;
	public static final int
		RULE_start = 0, RULE_vartype = 1, RULE_beginBlock = 2, RULE_endBlock = 3, 
		RULE_prog = 4, RULE_funcDeclr = 5, RULE_mainDeclrHeader = 6, RULE_mainDeclr = 7, 
		RULE_body = 8, RULE_expr = 9, RULE_primaryExpr = 10, RULE_assign = 11, 
		RULE_ifStatement = 12, RULE_elseStatement = 13, RULE_whileStatement = 14, 
		RULE_declr = 15, RULE_varDeclr = 16, RULE_varDeclrAndAssign = 17, RULE_flowControl = 18;
	private static String[] makeRuleNames() {
		return new String[] {
			"start", "vartype", "beginBlock", "endBlock", "prog", "funcDeclr", "mainDeclrHeader", 
			"mainDeclr", "body", "expr", "primaryExpr", "assign", "ifStatement", 
			"elseStatement", "whileStatement", "declr", "varDeclr", "varDeclrAndAssign", 
			"flowControl"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'main'", "'if'", "'else'", "'while'", null, "'return'", null, 
			null, "'int'", "'char'", "'void'", "'<'", "'>'", "'&'", "'|'", "'^'", 
			"'=='", "'!='", "'<='", "'>='", "'='", "':'", "';'", "'+'", "'-'", "'*'", 
			"'/'", "'%'", "'+='", "'-='", "'*='", "'/='", "'('", "')'", "'{'", "'}'", 
			"'!'", "','"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, "WS", "TReturn", "Number", "CharLiteral", 
			"TInt", "TChar", "TVoid", "LessThan", "GreaterThan", "BitwiseAnd", "BitwiseOr", 
			"BitwiseXor", "Equal", "NotEqual", "LessOrEqual", "GreaterOrEqual", "Assign", 
			"Colon", "StatementSeparator", "Plus", "Minus", "Mul", "Div", "Mod", 
			"PlusEqual", "MinusEqual", "MulEqual", "DivEqual", "OpenPar", "ClosePar", 
			"OpenCurly", "CloseCurly", "ExclamationMark", "Comma", "Identifier", 
			"Comment", "IncludeDirective", "Newline"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "Cricket.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public CricketParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	public static class StartContext extends ParserRuleContext {
		public ProgContext prog() {
			return getRuleContext(ProgContext.class,0);
		}
		public TerminalNode EOF() { return getToken(CricketParser.EOF, 0); }
		public StartContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_start; }
	}

	public final StartContext start() throws RecognitionException {
		StartContext _localctx = new StartContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_start);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(38);
			prog();
			setState(39);
			match(EOF);
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

	public static class VartypeContext extends ParserRuleContext {
		public TerminalNode TInt() { return getToken(CricketParser.TInt, 0); }
		public TerminalNode TChar() { return getToken(CricketParser.TChar, 0); }
		public VartypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_vartype; }
	}

	public final VartypeContext vartype() throws RecognitionException {
		VartypeContext _localctx = new VartypeContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_vartype);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(41);
			_la = _input.LA(1);
			if ( !(_la==TInt || _la==TChar) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
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

	public static class BeginBlockContext extends ParserRuleContext {
		public TerminalNode OpenCurly() { return getToken(CricketParser.OpenCurly, 0); }
		public BeginBlockContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_beginBlock; }
	}

	public final BeginBlockContext beginBlock() throws RecognitionException {
		BeginBlockContext _localctx = new BeginBlockContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_beginBlock);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(43);
			match(OpenCurly);
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

	public static class EndBlockContext extends ParserRuleContext {
		public TerminalNode CloseCurly() { return getToken(CricketParser.CloseCurly, 0); }
		public EndBlockContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_endBlock; }
	}

	public final EndBlockContext endBlock() throws RecognitionException {
		EndBlockContext _localctx = new EndBlockContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_endBlock);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(45);
			match(CloseCurly);
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

	public static class ProgContext extends ParserRuleContext {
		public MainDeclrContext mainDeclr() {
			return getRuleContext(MainDeclrContext.class,0);
		}
		public List<FuncDeclrContext> funcDeclr() {
			return getRuleContexts(FuncDeclrContext.class);
		}
		public FuncDeclrContext funcDeclr(int i) {
			return getRuleContext(FuncDeclrContext.class,i);
		}
		public ProgContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_prog; }
	}

	public final ProgContext prog() throws RecognitionException {
		ProgContext _localctx = new ProgContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_prog);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(50);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,0,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(47);
					funcDeclr();
					}
					} 
				}
				setState(52);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,0,_ctx);
			}
			setState(53);
			mainDeclr();
			setState(57);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << TInt) | (1L << TChar) | (1L << TVoid))) != 0)) {
				{
				{
				setState(54);
				funcDeclr();
				}
				}
				setState(59);
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

	public static class FuncDeclrContext extends ParserRuleContext {
		public Token FTYPE;
		public List<TerminalNode> Identifier() { return getTokens(CricketParser.Identifier); }
		public TerminalNode Identifier(int i) {
			return getToken(CricketParser.Identifier, i);
		}
		public TerminalNode OpenPar() { return getToken(CricketParser.OpenPar, 0); }
		public TerminalNode ClosePar() { return getToken(CricketParser.ClosePar, 0); }
		public BeginBlockContext beginBlock() {
			return getRuleContext(BeginBlockContext.class,0);
		}
		public BodyContext body() {
			return getRuleContext(BodyContext.class,0);
		}
		public EndBlockContext endBlock() {
			return getRuleContext(EndBlockContext.class,0);
		}
		public List<TerminalNode> TVoid() { return getTokens(CricketParser.TVoid); }
		public TerminalNode TVoid(int i) {
			return getToken(CricketParser.TVoid, i);
		}
		public TerminalNode TInt() { return getToken(CricketParser.TInt, 0); }
		public TerminalNode TChar() { return getToken(CricketParser.TChar, 0); }
		public TerminalNode StatementSeparator() { return getToken(CricketParser.StatementSeparator, 0); }
		public List<VartypeContext> vartype() {
			return getRuleContexts(VartypeContext.class);
		}
		public VartypeContext vartype(int i) {
			return getRuleContext(VartypeContext.class,i);
		}
		public List<TerminalNode> Comma() { return getTokens(CricketParser.Comma); }
		public TerminalNode Comma(int i) {
			return getToken(CricketParser.Comma, i);
		}
		public FuncDeclrContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_funcDeclr; }
	}

	public final FuncDeclrContext funcDeclr() throws RecognitionException {
		FuncDeclrContext _localctx = new FuncDeclrContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_funcDeclr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(60);
			((FuncDeclrContext)_localctx).FTYPE = _input.LT(1);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << TInt) | (1L << TChar) | (1L << TVoid))) != 0)) ) {
				((FuncDeclrContext)_localctx).FTYPE = (Token)_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			setState(61);
			match(Identifier);
			setState(62);
			match(OpenPar);
			setState(79);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,5,_ctx) ) {
			case 1:
				{
				setState(74);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==TInt || _la==TChar) {
					{
					setState(63);
					vartype();
					setState(64);
					match(Identifier);
					setState(71);
					_errHandler.sync(this);
					_la = _input.LA(1);
					while (_la==Comma) {
						{
						{
						setState(65);
						match(Comma);
						setState(66);
						vartype();
						setState(67);
						match(Identifier);
						}
						}
						setState(73);
						_errHandler.sync(this);
						_la = _input.LA(1);
					}
					}
				}

				}
				break;
			case 2:
				{
				setState(77);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==TVoid) {
					{
					setState(76);
					match(TVoid);
					}
				}

				}
				break;
			}
			setState(81);
			match(ClosePar);
			setState(82);
			beginBlock();
			setState(83);
			body();
			setState(84);
			endBlock();
			setState(86);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==StatementSeparator) {
				{
				setState(85);
				match(StatementSeparator);
				}
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

	public static class MainDeclrHeaderContext extends ParserRuleContext {
		public Token FTYPE;
		public TerminalNode OpenPar() { return getToken(CricketParser.OpenPar, 0); }
		public TerminalNode ClosePar() { return getToken(CricketParser.ClosePar, 0); }
		public List<TerminalNode> TVoid() { return getTokens(CricketParser.TVoid); }
		public TerminalNode TVoid(int i) {
			return getToken(CricketParser.TVoid, i);
		}
		public TerminalNode TInt() { return getToken(CricketParser.TInt, 0); }
		public MainDeclrHeaderContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mainDeclrHeader; }
	}

	public final MainDeclrHeaderContext mainDeclrHeader() throws RecognitionException {
		MainDeclrHeaderContext _localctx = new MainDeclrHeaderContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_mainDeclrHeader);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(88);
			((MainDeclrHeaderContext)_localctx).FTYPE = _input.LT(1);
			_la = _input.LA(1);
			if ( !(_la==TInt || _la==TVoid) ) {
				((MainDeclrHeaderContext)_localctx).FTYPE = (Token)_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			setState(89);
			match(T__0);
			setState(90);
			match(OpenPar);
			setState(92);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==TVoid) {
				{
				setState(91);
				match(TVoid);
				}
			}

			setState(94);
			match(ClosePar);
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

	public static class MainDeclrContext extends ParserRuleContext {
		public MainDeclrHeaderContext mainDeclrHeader() {
			return getRuleContext(MainDeclrHeaderContext.class,0);
		}
		public BeginBlockContext beginBlock() {
			return getRuleContext(BeginBlockContext.class,0);
		}
		public BodyContext body() {
			return getRuleContext(BodyContext.class,0);
		}
		public EndBlockContext endBlock() {
			return getRuleContext(EndBlockContext.class,0);
		}
		public TerminalNode StatementSeparator() { return getToken(CricketParser.StatementSeparator, 0); }
		public MainDeclrContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mainDeclr; }
	}

	public final MainDeclrContext mainDeclr() throws RecognitionException {
		MainDeclrContext _localctx = new MainDeclrContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_mainDeclr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(96);
			mainDeclrHeader();
			setState(97);
			beginBlock();
			setState(98);
			body();
			setState(99);
			endBlock();
			setState(101);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==StatementSeparator) {
				{
				setState(100);
				match(StatementSeparator);
				}
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

	public static class BodyContext extends ParserRuleContext {
		public DeclrContext declr() {
			return getRuleContext(DeclrContext.class,0);
		}
		public TerminalNode StatementSeparator() { return getToken(CricketParser.StatementSeparator, 0); }
		public List<BodyContext> body() {
			return getRuleContexts(BodyContext.class);
		}
		public BodyContext body(int i) {
			return getRuleContext(BodyContext.class,i);
		}
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public FlowControlContext flowControl() {
			return getRuleContext(FlowControlContext.class,0);
		}
		public IfStatementContext ifStatement() {
			return getRuleContext(IfStatementContext.class,0);
		}
		public WhileStatementContext whileStatement() {
			return getRuleContext(WhileStatementContext.class,0);
		}
		public BeginBlockContext beginBlock() {
			return getRuleContext(BeginBlockContext.class,0);
		}
		public EndBlockContext endBlock() {
			return getRuleContext(EndBlockContext.class,0);
		}
		public BodyContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_body; }
	}

	public final BodyContext body() throws RecognitionException {
		BodyContext _localctx = new BodyContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_body);
		int _la;
		try {
			setState(130);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case TInt:
			case TChar:
				enterOuterAlt(_localctx, 1);
				{
				setState(103);
				declr();
				setState(104);
				match(StatementSeparator);
				setState(105);
				body();
				}
				break;
			case Number:
			case CharLiteral:
			case Minus:
			case OpenPar:
			case ExclamationMark:
			case Identifier:
				enterOuterAlt(_localctx, 2);
				{
				setState(107);
				expr();
				setState(108);
				match(StatementSeparator);
				setState(109);
				body();
				}
				break;
			case TReturn:
				enterOuterAlt(_localctx, 3);
				{
				setState(111);
				flowControl();
				setState(112);
				match(StatementSeparator);
				setState(113);
				body();
				}
				break;
			case T__1:
				enterOuterAlt(_localctx, 4);
				{
				setState(115);
				ifStatement();
				setState(116);
				body();
				}
				break;
			case T__3:
				enterOuterAlt(_localctx, 5);
				{
				setState(118);
				whileStatement();
				setState(119);
				body();
				}
				break;
			case OpenCurly:
				enterOuterAlt(_localctx, 6);
				{
				setState(121);
				beginBlock();
				setState(122);
				body();
				setState(123);
				endBlock();
				setState(125);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==StatementSeparator) {
					{
					setState(124);
					match(StatementSeparator);
					}
				}

				setState(127);
				body();
				}
				break;
			case CloseCurly:
				enterOuterAlt(_localctx, 7);
				{
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

	public static class ExprContext extends ParserRuleContext {
		public AssignContext assign() {
			return getRuleContext(AssignContext.class,0);
		}
		public PrimaryExprContext primaryExpr() {
			return getRuleContext(PrimaryExprContext.class,0);
		}
		public ExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expr; }
	}

	public final ExprContext expr() throws RecognitionException {
		ExprContext _localctx = new ExprContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_expr);
		try {
			setState(134);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,11,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(132);
				assign();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(133);
				primaryExpr(0);
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

	public static class PrimaryExprContext extends ParserRuleContext {
		public PrimaryExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_primaryExpr; }
	 
		public PrimaryExprContext() { }
		public void copyFrom(PrimaryExprContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class ParExprContext extends PrimaryExprContext {
		public TerminalNode OpenPar() { return getToken(CricketParser.OpenPar, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public TerminalNode ClosePar() { return getToken(CricketParser.ClosePar, 0); }
		public ParExprContext(PrimaryExprContext ctx) { copyFrom(ctx); }
	}
	public static class VarExprContext extends PrimaryExprContext {
		public TerminalNode Identifier() { return getToken(CricketParser.Identifier, 0); }
		public VarExprContext(PrimaryExprContext ctx) { copyFrom(ctx); }
	}
	public static class UnaryExprContext extends PrimaryExprContext {
		public Token UNARY;
		public PrimaryExprContext primaryExpr() {
			return getRuleContext(PrimaryExprContext.class,0);
		}
		public TerminalNode Minus() { return getToken(CricketParser.Minus, 0); }
		public TerminalNode ExclamationMark() { return getToken(CricketParser.ExclamationMark, 0); }
		public UnaryExprContext(PrimaryExprContext ctx) { copyFrom(ctx); }
	}
	public static class FuncExprContext extends PrimaryExprContext {
		public TerminalNode Identifier() { return getToken(CricketParser.Identifier, 0); }
		public TerminalNode OpenPar() { return getToken(CricketParser.OpenPar, 0); }
		public TerminalNode ClosePar() { return getToken(CricketParser.ClosePar, 0); }
		public List<PrimaryExprContext> primaryExpr() {
			return getRuleContexts(PrimaryExprContext.class);
		}
		public PrimaryExprContext primaryExpr(int i) {
			return getRuleContext(PrimaryExprContext.class,i);
		}
		public List<TerminalNode> Comma() { return getTokens(CricketParser.Comma); }
		public TerminalNode Comma(int i) {
			return getToken(CricketParser.Comma, i);
		}
		public FuncExprContext(PrimaryExprContext ctx) { copyFrom(ctx); }
	}
	public static class AddSubExprContext extends PrimaryExprContext {
		public Token ADDITIVE;
		public List<PrimaryExprContext> primaryExpr() {
			return getRuleContexts(PrimaryExprContext.class);
		}
		public PrimaryExprContext primaryExpr(int i) {
			return getRuleContext(PrimaryExprContext.class,i);
		}
		public TerminalNode Plus() { return getToken(CricketParser.Plus, 0); }
		public TerminalNode Minus() { return getToken(CricketParser.Minus, 0); }
		public AddSubExprContext(PrimaryExprContext ctx) { copyFrom(ctx); }
	}
	public static class CmpEqualityLessGreaterExprContext extends PrimaryExprContext {
		public Token LGEQ;
		public List<PrimaryExprContext> primaryExpr() {
			return getRuleContexts(PrimaryExprContext.class);
		}
		public PrimaryExprContext primaryExpr(int i) {
			return getRuleContext(PrimaryExprContext.class,i);
		}
		public TerminalNode LessOrEqual() { return getToken(CricketParser.LessOrEqual, 0); }
		public TerminalNode GreaterOrEqual() { return getToken(CricketParser.GreaterOrEqual, 0); }
		public CmpEqualityLessGreaterExprContext(PrimaryExprContext ctx) { copyFrom(ctx); }
	}
	public static class BitwiseExprContext extends PrimaryExprContext {
		public Token BITWISE;
		public List<PrimaryExprContext> primaryExpr() {
			return getRuleContexts(PrimaryExprContext.class);
		}
		public PrimaryExprContext primaryExpr(int i) {
			return getRuleContext(PrimaryExprContext.class,i);
		}
		public TerminalNode BitwiseAnd() { return getToken(CricketParser.BitwiseAnd, 0); }
		public TerminalNode BitwiseOr() { return getToken(CricketParser.BitwiseOr, 0); }
		public TerminalNode BitwiseXor() { return getToken(CricketParser.BitwiseXor, 0); }
		public BitwiseExprContext(PrimaryExprContext ctx) { copyFrom(ctx); }
	}
	public static class MulDivModExprContext extends PrimaryExprContext {
		public Token MULTIPLICATIVE;
		public List<PrimaryExprContext> primaryExpr() {
			return getRuleContexts(PrimaryExprContext.class);
		}
		public PrimaryExprContext primaryExpr(int i) {
			return getRuleContext(PrimaryExprContext.class,i);
		}
		public TerminalNode Mul() { return getToken(CricketParser.Mul, 0); }
		public TerminalNode Div() { return getToken(CricketParser.Div, 0); }
		public TerminalNode Mod() { return getToken(CricketParser.Mod, 0); }
		public MulDivModExprContext(PrimaryExprContext ctx) { copyFrom(ctx); }
	}
	public static class CmpEqualityExprContext extends PrimaryExprContext {
		public Token EQ;
		public List<PrimaryExprContext> primaryExpr() {
			return getRuleContexts(PrimaryExprContext.class);
		}
		public PrimaryExprContext primaryExpr(int i) {
			return getRuleContext(PrimaryExprContext.class,i);
		}
		public TerminalNode Equal() { return getToken(CricketParser.Equal, 0); }
		public TerminalNode NotEqual() { return getToken(CricketParser.NotEqual, 0); }
		public CmpEqualityExprContext(PrimaryExprContext ctx) { copyFrom(ctx); }
	}
	public static class CmpLessOrGreaterExprContext extends PrimaryExprContext {
		public Token CMP;
		public List<PrimaryExprContext> primaryExpr() {
			return getRuleContexts(PrimaryExprContext.class);
		}
		public PrimaryExprContext primaryExpr(int i) {
			return getRuleContext(PrimaryExprContext.class,i);
		}
		public TerminalNode LessThan() { return getToken(CricketParser.LessThan, 0); }
		public TerminalNode GreaterThan() { return getToken(CricketParser.GreaterThan, 0); }
		public CmpLessOrGreaterExprContext(PrimaryExprContext ctx) { copyFrom(ctx); }
	}
	public static class ConstExprContext extends PrimaryExprContext {
		public Token CONST;
		public TerminalNode Number() { return getToken(CricketParser.Number, 0); }
		public TerminalNode CharLiteral() { return getToken(CricketParser.CharLiteral, 0); }
		public ConstExprContext(PrimaryExprContext ctx) { copyFrom(ctx); }
	}

	public final PrimaryExprContext primaryExpr() throws RecognitionException {
		return primaryExpr(0);
	}

	private PrimaryExprContext primaryExpr(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		PrimaryExprContext _localctx = new PrimaryExprContext(_ctx, _parentState);
		PrimaryExprContext _prevctx = _localctx;
		int _startState = 20;
		enterRecursionRule(_localctx, 20, RULE_primaryExpr, _p);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(158);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,14,_ctx) ) {
			case 1:
				{
				_localctx = new ParExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;

				setState(137);
				match(OpenPar);
				setState(138);
				expr();
				setState(139);
				match(ClosePar);
				}
				break;
			case 2:
				{
				_localctx = new UnaryExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(141);
				((UnaryExprContext)_localctx).UNARY = _input.LT(1);
				_la = _input.LA(1);
				if ( !(_la==Minus || _la==ExclamationMark) ) {
					((UnaryExprContext)_localctx).UNARY = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(142);
				primaryExpr(10);
				}
				break;
			case 3:
				{
				_localctx = new FuncExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(143);
				match(Identifier);
				setState(144);
				match(OpenPar);
				setState(153);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Number) | (1L << CharLiteral) | (1L << Minus) | (1L << OpenPar) | (1L << ExclamationMark) | (1L << Identifier))) != 0)) {
					{
					setState(145);
					primaryExpr(0);
					setState(150);
					_errHandler.sync(this);
					_la = _input.LA(1);
					while (_la==Comma) {
						{
						{
						setState(146);
						match(Comma);
						setState(147);
						primaryExpr(0);
						}
						}
						setState(152);
						_errHandler.sync(this);
						_la = _input.LA(1);
					}
					}
				}

				setState(155);
				match(ClosePar);
				}
				break;
			case 4:
				{
				_localctx = new ConstExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(156);
				((ConstExprContext)_localctx).CONST = _input.LT(1);
				_la = _input.LA(1);
				if ( !(_la==Number || _la==CharLiteral) ) {
					((ConstExprContext)_localctx).CONST = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				}
				break;
			case 5:
				{
				_localctx = new VarExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(157);
				match(Identifier);
				}
				break;
			}
			_ctx.stop = _input.LT(-1);
			setState(180);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,16,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(178);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,15,_ctx) ) {
					case 1:
						{
						_localctx = new MulDivModExprContext(new PrimaryExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_primaryExpr);
						setState(160);
						if (!(precpred(_ctx, 9))) throw new FailedPredicateException(this, "precpred(_ctx, 9)");
						setState(161);
						((MulDivModExprContext)_localctx).MULTIPLICATIVE = _input.LT(1);
						_la = _input.LA(1);
						if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Mul) | (1L << Div) | (1L << Mod))) != 0)) ) {
							((MulDivModExprContext)_localctx).MULTIPLICATIVE = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(162);
						primaryExpr(10);
						}
						break;
					case 2:
						{
						_localctx = new AddSubExprContext(new PrimaryExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_primaryExpr);
						setState(163);
						if (!(precpred(_ctx, 8))) throw new FailedPredicateException(this, "precpred(_ctx, 8)");
						setState(164);
						((AddSubExprContext)_localctx).ADDITIVE = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==Plus || _la==Minus) ) {
							((AddSubExprContext)_localctx).ADDITIVE = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(165);
						primaryExpr(9);
						}
						break;
					case 3:
						{
						_localctx = new CmpLessOrGreaterExprContext(new PrimaryExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_primaryExpr);
						setState(166);
						if (!(precpred(_ctx, 7))) throw new FailedPredicateException(this, "precpred(_ctx, 7)");
						setState(167);
						((CmpLessOrGreaterExprContext)_localctx).CMP = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==LessThan || _la==GreaterThan) ) {
							((CmpLessOrGreaterExprContext)_localctx).CMP = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(168);
						primaryExpr(8);
						}
						break;
					case 4:
						{
						_localctx = new CmpEqualityExprContext(new PrimaryExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_primaryExpr);
						setState(169);
						if (!(precpred(_ctx, 6))) throw new FailedPredicateException(this, "precpred(_ctx, 6)");
						setState(170);
						((CmpEqualityExprContext)_localctx).EQ = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==Equal || _la==NotEqual) ) {
							((CmpEqualityExprContext)_localctx).EQ = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(171);
						primaryExpr(7);
						}
						break;
					case 5:
						{
						_localctx = new CmpEqualityLessGreaterExprContext(new PrimaryExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_primaryExpr);
						setState(172);
						if (!(precpred(_ctx, 5))) throw new FailedPredicateException(this, "precpred(_ctx, 5)");
						setState(173);
						((CmpEqualityLessGreaterExprContext)_localctx).LGEQ = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==LessOrEqual || _la==GreaterOrEqual) ) {
							((CmpEqualityLessGreaterExprContext)_localctx).LGEQ = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(174);
						primaryExpr(6);
						}
						break;
					case 6:
						{
						_localctx = new BitwiseExprContext(new PrimaryExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_primaryExpr);
						setState(175);
						if (!(precpred(_ctx, 4))) throw new FailedPredicateException(this, "precpred(_ctx, 4)");
						setState(176);
						((BitwiseExprContext)_localctx).BITWISE = _input.LT(1);
						_la = _input.LA(1);
						if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << BitwiseAnd) | (1L << BitwiseOr) | (1L << BitwiseXor))) != 0)) ) {
							((BitwiseExprContext)_localctx).BITWISE = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(177);
						primaryExpr(5);
						}
						break;
					}
					} 
				}
				setState(182);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,16,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	public static class AssignContext extends ParserRuleContext {
		public AssignContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_assign; }
	 
		public AssignContext() { }
		public void copyFrom(AssignContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class AssignExprContext extends AssignContext {
		public TerminalNode Identifier() { return getToken(CricketParser.Identifier, 0); }
		public TerminalNode Assign() { return getToken(CricketParser.Assign, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public AssignExprContext(AssignContext ctx) { copyFrom(ctx); }
	}
	public static class PmmdEqualContext extends AssignContext {
		public Token OPPMMD;
		public TerminalNode Identifier() { return getToken(CricketParser.Identifier, 0); }
		public PrimaryExprContext primaryExpr() {
			return getRuleContext(PrimaryExprContext.class,0);
		}
		public TerminalNode PlusEqual() { return getToken(CricketParser.PlusEqual, 0); }
		public TerminalNode MinusEqual() { return getToken(CricketParser.MinusEqual, 0); }
		public TerminalNode MulEqual() { return getToken(CricketParser.MulEqual, 0); }
		public TerminalNode DivEqual() { return getToken(CricketParser.DivEqual, 0); }
		public PmmdEqualContext(AssignContext ctx) { copyFrom(ctx); }
	}

	public final AssignContext assign() throws RecognitionException {
		AssignContext _localctx = new AssignContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_assign);
		int _la;
		try {
			setState(189);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,17,_ctx) ) {
			case 1:
				_localctx = new AssignExprContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(183);
				match(Identifier);
				setState(184);
				match(Assign);
				setState(185);
				expr();
				}
				break;
			case 2:
				_localctx = new PmmdEqualContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(186);
				match(Identifier);
				setState(187);
				((PmmdEqualContext)_localctx).OPPMMD = _input.LT(1);
				_la = _input.LA(1);
				if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << PlusEqual) | (1L << MinusEqual) | (1L << MulEqual) | (1L << DivEqual))) != 0)) ) {
					((PmmdEqualContext)_localctx).OPPMMD = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(188);
				primaryExpr(0);
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

	public static class IfStatementContext extends ParserRuleContext {
		public TerminalNode OpenPar() { return getToken(CricketParser.OpenPar, 0); }
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode ClosePar() { return getToken(CricketParser.ClosePar, 0); }
		public BeginBlockContext beginBlock() {
			return getRuleContext(BeginBlockContext.class,0);
		}
		public BodyContext body() {
			return getRuleContext(BodyContext.class,0);
		}
		public EndBlockContext endBlock() {
			return getRuleContext(EndBlockContext.class,0);
		}
		public ElseStatementContext elseStatement() {
			return getRuleContext(ElseStatementContext.class,0);
		}
		public TerminalNode StatementSeparator() { return getToken(CricketParser.StatementSeparator, 0); }
		public FlowControlContext flowControl() {
			return getRuleContext(FlowControlContext.class,0);
		}
		public IfStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_ifStatement; }
	}

	public final IfStatementContext ifStatement() throws RecognitionException {
		IfStatementContext _localctx = new IfStatementContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_ifStatement);
		int _la;
		try {
			setState(219);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,21,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(191);
				match(T__1);
				setState(192);
				match(OpenPar);
				setState(193);
				expr();
				setState(194);
				match(ClosePar);
				setState(195);
				beginBlock();
				setState(196);
				body();
				setState(197);
				endBlock();
				setState(199);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==T__2) {
					{
					setState(198);
					elseStatement();
					}
				}

				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(201);
				match(T__1);
				setState(202);
				match(OpenPar);
				setState(203);
				expr();
				setState(204);
				match(ClosePar);
				setState(205);
				expr();
				setState(206);
				match(StatementSeparator);
				setState(208);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==T__2) {
					{
					setState(207);
					elseStatement();
					}
				}

				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(210);
				match(T__1);
				setState(211);
				match(OpenPar);
				setState(212);
				expr();
				setState(213);
				match(ClosePar);
				setState(214);
				flowControl();
				setState(215);
				match(StatementSeparator);
				setState(217);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==T__2) {
					{
					setState(216);
					elseStatement();
					}
				}

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

	public static class ElseStatementContext extends ParserRuleContext {
		public BeginBlockContext beginBlock() {
			return getRuleContext(BeginBlockContext.class,0);
		}
		public BodyContext body() {
			return getRuleContext(BodyContext.class,0);
		}
		public EndBlockContext endBlock() {
			return getRuleContext(EndBlockContext.class,0);
		}
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public TerminalNode StatementSeparator() { return getToken(CricketParser.StatementSeparator, 0); }
		public FlowControlContext flowControl() {
			return getRuleContext(FlowControlContext.class,0);
		}
		public ElseStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_elseStatement; }
	}

	public final ElseStatementContext elseStatement() throws RecognitionException {
		ElseStatementContext _localctx = new ElseStatementContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_elseStatement);
		try {
			setState(234);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,22,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(221);
				match(T__2);
				setState(222);
				beginBlock();
				setState(223);
				body();
				setState(224);
				endBlock();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(226);
				match(T__2);
				setState(227);
				expr();
				setState(228);
				match(StatementSeparator);
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(230);
				match(T__2);
				setState(231);
				flowControl();
				setState(232);
				match(StatementSeparator);
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

	public static class WhileStatementContext extends ParserRuleContext {
		public TerminalNode OpenPar() { return getToken(CricketParser.OpenPar, 0); }
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode ClosePar() { return getToken(CricketParser.ClosePar, 0); }
		public BeginBlockContext beginBlock() {
			return getRuleContext(BeginBlockContext.class,0);
		}
		public BodyContext body() {
			return getRuleContext(BodyContext.class,0);
		}
		public EndBlockContext endBlock() {
			return getRuleContext(EndBlockContext.class,0);
		}
		public TerminalNode StatementSeparator() { return getToken(CricketParser.StatementSeparator, 0); }
		public FlowControlContext flowControl() {
			return getRuleContext(FlowControlContext.class,0);
		}
		public WhileStatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_whileStatement; }
	}

	public final WhileStatementContext whileStatement() throws RecognitionException {
		WhileStatementContext _localctx = new WhileStatementContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_whileStatement);
		try {
			setState(258);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,23,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(236);
				match(T__3);
				setState(237);
				match(OpenPar);
				setState(238);
				expr();
				setState(239);
				match(ClosePar);
				setState(240);
				beginBlock();
				setState(241);
				body();
				setState(242);
				endBlock();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(244);
				match(T__3);
				setState(245);
				match(OpenPar);
				setState(246);
				expr();
				setState(247);
				match(ClosePar);
				setState(248);
				expr();
				setState(249);
				match(StatementSeparator);
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(251);
				match(T__3);
				setState(252);
				match(OpenPar);
				setState(253);
				expr();
				setState(254);
				match(ClosePar);
				setState(255);
				flowControl();
				setState(256);
				match(StatementSeparator);
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

	public static class DeclrContext extends ParserRuleContext {
		public VarDeclrContext varDeclr() {
			return getRuleContext(VarDeclrContext.class,0);
		}
		public VarDeclrAndAssignContext varDeclrAndAssign() {
			return getRuleContext(VarDeclrAndAssignContext.class,0);
		}
		public DeclrContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_declr; }
	}

	public final DeclrContext declr() throws RecognitionException {
		DeclrContext _localctx = new DeclrContext(_ctx, getState());
		enterRule(_localctx, 30, RULE_declr);
		try {
			setState(262);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,24,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(260);
				varDeclr();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(261);
				varDeclrAndAssign();
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

	public static class VarDeclrContext extends ParserRuleContext {
		public VartypeContext vartype() {
			return getRuleContext(VartypeContext.class,0);
		}
		public List<TerminalNode> Identifier() { return getTokens(CricketParser.Identifier); }
		public TerminalNode Identifier(int i) {
			return getToken(CricketParser.Identifier, i);
		}
		public List<TerminalNode> Comma() { return getTokens(CricketParser.Comma); }
		public TerminalNode Comma(int i) {
			return getToken(CricketParser.Comma, i);
		}
		public VarDeclrContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_varDeclr; }
	}

	public final VarDeclrContext varDeclr() throws RecognitionException {
		VarDeclrContext _localctx = new VarDeclrContext(_ctx, getState());
		enterRule(_localctx, 32, RULE_varDeclr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(264);
			vartype();
			setState(265);
			match(Identifier);
			setState(270);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==Comma) {
				{
				{
				setState(266);
				match(Comma);
				setState(267);
				match(Identifier);
				}
				}
				setState(272);
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

	public static class VarDeclrAndAssignContext extends ParserRuleContext {
		public VartypeContext vartype() {
			return getRuleContext(VartypeContext.class,0);
		}
		public TerminalNode Identifier() { return getToken(CricketParser.Identifier, 0); }
		public TerminalNode Assign() { return getToken(CricketParser.Assign, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public VarDeclrAndAssignContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_varDeclrAndAssign; }
	}

	public final VarDeclrAndAssignContext varDeclrAndAssign() throws RecognitionException {
		VarDeclrAndAssignContext _localctx = new VarDeclrAndAssignContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_varDeclrAndAssign);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(273);
			vartype();
			setState(274);
			match(Identifier);
			setState(275);
			match(Assign);
			setState(276);
			expr();
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

	public static class FlowControlContext extends ParserRuleContext {
		public FlowControlContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_flowControl; }
	 
		public FlowControlContext() { }
		public void copyFrom(FlowControlContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class EmptyReturnContext extends FlowControlContext {
		public TerminalNode TReturn() { return getToken(CricketParser.TReturn, 0); }
		public EmptyReturnContext(FlowControlContext ctx) { copyFrom(ctx); }
	}
	public static class ExprReturnContext extends FlowControlContext {
		public TerminalNode TReturn() { return getToken(CricketParser.TReturn, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public ExprReturnContext(FlowControlContext ctx) { copyFrom(ctx); }
	}

	public final FlowControlContext flowControl() throws RecognitionException {
		FlowControlContext _localctx = new FlowControlContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_flowControl);
		try {
			setState(281);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,26,_ctx) ) {
			case 1:
				_localctx = new ExprReturnContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(278);
				match(TReturn);
				setState(279);
				expr();
				}
				break;
			case 2:
				_localctx = new EmptyReturnContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(280);
				match(TReturn);
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

	public boolean sempred(RuleContext _localctx, int ruleIndex, int predIndex) {
		switch (ruleIndex) {
		case 10:
			return primaryExpr_sempred((PrimaryExprContext)_localctx, predIndex);
		}
		return true;
	}
	private boolean primaryExpr_sempred(PrimaryExprContext _localctx, int predIndex) {
		switch (predIndex) {
		case 0:
			return precpred(_ctx, 9);
		case 1:
			return precpred(_ctx, 8);
		case 2:
			return precpred(_ctx, 7);
		case 3:
			return precpred(_ctx, 6);
		case 4:
			return precpred(_ctx, 5);
		case 5:
			return precpred(_ctx, 4);
		}
		return true;
	}

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\3,\u011e\4\2\t\2\4"+
		"\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t"+
		"\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\3\2\3\2\3\2\3\3\3\3\3\4\3\4\3\5\3\5\3\6\7\6\63\n"+
		"\6\f\6\16\6\66\13\6\3\6\3\6\7\6:\n\6\f\6\16\6=\13\6\3\7\3\7\3\7\3\7\3"+
		"\7\3\7\3\7\3\7\3\7\7\7H\n\7\f\7\16\7K\13\7\5\7M\n\7\3\7\5\7P\n\7\5\7R"+
		"\n\7\3\7\3\7\3\7\3\7\3\7\5\7Y\n\7\3\b\3\b\3\b\3\b\5\b_\n\b\3\b\3\b\3\t"+
		"\3\t\3\t\3\t\3\t\5\th\n\t\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n"+
		"\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\5\n\u0080\n\n\3\n\3\n\3\n"+
		"\5\n\u0085\n\n\3\13\3\13\5\13\u0089\n\13\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3"+
		"\f\3\f\3\f\3\f\3\f\7\f\u0097\n\f\f\f\16\f\u009a\13\f\5\f\u009c\n\f\3\f"+
		"\3\f\3\f\5\f\u00a1\n\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f"+
		"\3\f\3\f\3\f\3\f\3\f\3\f\7\f\u00b5\n\f\f\f\16\f\u00b8\13\f\3\r\3\r\3\r"+
		"\3\r\3\r\3\r\5\r\u00c0\n\r\3\16\3\16\3\16\3\16\3\16\3\16\3\16\3\16\5\16"+
		"\u00ca\n\16\3\16\3\16\3\16\3\16\3\16\3\16\3\16\5\16\u00d3\n\16\3\16\3"+
		"\16\3\16\3\16\3\16\3\16\3\16\5\16\u00dc\n\16\5\16\u00de\n\16\3\17\3\17"+
		"\3\17\3\17\3\17\3\17\3\17\3\17\3\17\3\17\3\17\3\17\3\17\5\17\u00ed\n\17"+
		"\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20"+
		"\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\5\20\u0105\n\20\3\21\3\21\5\21"+
		"\u0109\n\21\3\22\3\22\3\22\3\22\7\22\u010f\n\22\f\22\16\22\u0112\13\22"+
		"\3\23\3\23\3\23\3\23\3\23\3\24\3\24\3\24\5\24\u011c\n\24\3\24\2\3\26\25"+
		"\2\4\6\b\n\f\16\20\22\24\26\30\32\34\36 \"$&\2\16\3\2\13\f\3\2\13\r\4"+
		"\2\13\13\r\r\4\2\33\33\'\'\3\2\t\n\3\2\34\36\3\2\32\33\3\2\16\17\3\2\23"+
		"\24\3\2\25\26\3\2\20\22\3\2\37\"\2\u0134\2(\3\2\2\2\4+\3\2\2\2\6-\3\2"+
		"\2\2\b/\3\2\2\2\n\64\3\2\2\2\f>\3\2\2\2\16Z\3\2\2\2\20b\3\2\2\2\22\u0084"+
		"\3\2\2\2\24\u0088\3\2\2\2\26\u00a0\3\2\2\2\30\u00bf\3\2\2\2\32\u00dd\3"+
		"\2\2\2\34\u00ec\3\2\2\2\36\u0104\3\2\2\2 \u0108\3\2\2\2\"\u010a\3\2\2"+
		"\2$\u0113\3\2\2\2&\u011b\3\2\2\2()\5\n\6\2)*\7\2\2\3*\3\3\2\2\2+,\t\2"+
		"\2\2,\5\3\2\2\2-.\7%\2\2.\7\3\2\2\2/\60\7&\2\2\60\t\3\2\2\2\61\63\5\f"+
		"\7\2\62\61\3\2\2\2\63\66\3\2\2\2\64\62\3\2\2\2\64\65\3\2\2\2\65\67\3\2"+
		"\2\2\66\64\3\2\2\2\67;\5\20\t\28:\5\f\7\298\3\2\2\2:=\3\2\2\2;9\3\2\2"+
		"\2;<\3\2\2\2<\13\3\2\2\2=;\3\2\2\2>?\t\3\2\2?@\7)\2\2@Q\7#\2\2AB\5\4\3"+
		"\2BI\7)\2\2CD\7(\2\2DE\5\4\3\2EF\7)\2\2FH\3\2\2\2GC\3\2\2\2HK\3\2\2\2"+
		"IG\3\2\2\2IJ\3\2\2\2JM\3\2\2\2KI\3\2\2\2LA\3\2\2\2LM\3\2\2\2MR\3\2\2\2"+
		"NP\7\r\2\2ON\3\2\2\2OP\3\2\2\2PR\3\2\2\2QL\3\2\2\2QO\3\2\2\2RS\3\2\2\2"+
		"ST\7$\2\2TU\5\6\4\2UV\5\22\n\2VX\5\b\5\2WY\7\31\2\2XW\3\2\2\2XY\3\2\2"+
		"\2Y\r\3\2\2\2Z[\t\4\2\2[\\\7\3\2\2\\^\7#\2\2]_\7\r\2\2^]\3\2\2\2^_\3\2"+
		"\2\2_`\3\2\2\2`a\7$\2\2a\17\3\2\2\2bc\5\16\b\2cd\5\6\4\2de\5\22\n\2eg"+
		"\5\b\5\2fh\7\31\2\2gf\3\2\2\2gh\3\2\2\2h\21\3\2\2\2ij\5 \21\2jk\7\31\2"+
		"\2kl\5\22\n\2l\u0085\3\2\2\2mn\5\24\13\2no\7\31\2\2op\5\22\n\2p\u0085"+
		"\3\2\2\2qr\5&\24\2rs\7\31\2\2st\5\22\n\2t\u0085\3\2\2\2uv\5\32\16\2vw"+
		"\5\22\n\2w\u0085\3\2\2\2xy\5\36\20\2yz\5\22\n\2z\u0085\3\2\2\2{|\5\6\4"+
		"\2|}\5\22\n\2}\177\5\b\5\2~\u0080\7\31\2\2\177~\3\2\2\2\177\u0080\3\2"+
		"\2\2\u0080\u0081\3\2\2\2\u0081\u0082\5\22\n\2\u0082\u0085\3\2\2\2\u0083"+
		"\u0085\3\2\2\2\u0084i\3\2\2\2\u0084m\3\2\2\2\u0084q\3\2\2\2\u0084u\3\2"+
		"\2\2\u0084x\3\2\2\2\u0084{\3\2\2\2\u0084\u0083\3\2\2\2\u0085\23\3\2\2"+
		"\2\u0086\u0089\5\30\r\2\u0087\u0089\5\26\f\2\u0088\u0086\3\2\2\2\u0088"+
		"\u0087\3\2\2\2\u0089\25\3\2\2\2\u008a\u008b\b\f\1\2\u008b\u008c\7#\2\2"+
		"\u008c\u008d\5\24\13\2\u008d\u008e\7$\2\2\u008e\u00a1\3\2\2\2\u008f\u0090"+
		"\t\5\2\2\u0090\u00a1\5\26\f\f\u0091\u0092\7)\2\2\u0092\u009b\7#\2\2\u0093"+
		"\u0098\5\26\f\2\u0094\u0095\7(\2\2\u0095\u0097\5\26\f\2\u0096\u0094\3"+
		"\2\2\2\u0097\u009a\3\2\2\2\u0098\u0096\3\2\2\2\u0098\u0099\3\2\2\2\u0099"+
		"\u009c\3\2\2\2\u009a\u0098\3\2\2\2\u009b\u0093\3\2\2\2\u009b\u009c\3\2"+
		"\2\2\u009c\u009d\3\2\2\2\u009d\u00a1\7$\2\2\u009e\u00a1\t\6\2\2\u009f"+
		"\u00a1\7)\2\2\u00a0\u008a\3\2\2\2\u00a0\u008f\3\2\2\2\u00a0\u0091\3\2"+
		"\2\2\u00a0\u009e\3\2\2\2\u00a0\u009f\3\2\2\2\u00a1\u00b6\3\2\2\2\u00a2"+
		"\u00a3\f\13\2\2\u00a3\u00a4\t\7\2\2\u00a4\u00b5\5\26\f\f\u00a5\u00a6\f"+
		"\n\2\2\u00a6\u00a7\t\b\2\2\u00a7\u00b5\5\26\f\13\u00a8\u00a9\f\t\2\2\u00a9"+
		"\u00aa\t\t\2\2\u00aa\u00b5\5\26\f\n\u00ab\u00ac\f\b\2\2\u00ac\u00ad\t"+
		"\n\2\2\u00ad\u00b5\5\26\f\t\u00ae\u00af\f\7\2\2\u00af\u00b0\t\13\2\2\u00b0"+
		"\u00b5\5\26\f\b\u00b1\u00b2\f\6\2\2\u00b2\u00b3\t\f\2\2\u00b3\u00b5\5"+
		"\26\f\7\u00b4\u00a2\3\2\2\2\u00b4\u00a5\3\2\2\2\u00b4\u00a8\3\2\2\2\u00b4"+
		"\u00ab\3\2\2\2\u00b4\u00ae\3\2\2\2\u00b4\u00b1\3\2\2\2\u00b5\u00b8\3\2"+
		"\2\2\u00b6\u00b4\3\2\2\2\u00b6\u00b7\3\2\2\2\u00b7\27\3\2\2\2\u00b8\u00b6"+
		"\3\2\2\2\u00b9\u00ba\7)\2\2\u00ba\u00bb\7\27\2\2\u00bb\u00c0\5\24\13\2"+
		"\u00bc\u00bd\7)\2\2\u00bd\u00be\t\r\2\2\u00be\u00c0\5\26\f\2\u00bf\u00b9"+
		"\3\2\2\2\u00bf\u00bc\3\2\2\2\u00c0\31\3\2\2\2\u00c1\u00c2\7\4\2\2\u00c2"+
		"\u00c3\7#\2\2\u00c3\u00c4\5\24\13\2\u00c4\u00c5\7$\2\2\u00c5\u00c6\5\6"+
		"\4\2\u00c6\u00c7\5\22\n\2\u00c7\u00c9\5\b\5\2\u00c8\u00ca\5\34\17\2\u00c9"+
		"\u00c8\3\2\2\2\u00c9\u00ca\3\2\2\2\u00ca\u00de\3\2\2\2\u00cb\u00cc\7\4"+
		"\2\2\u00cc\u00cd\7#\2\2\u00cd\u00ce\5\24\13\2\u00ce\u00cf\7$\2\2\u00cf"+
		"\u00d0\5\24\13\2\u00d0\u00d2\7\31\2\2\u00d1\u00d3\5\34\17\2\u00d2\u00d1"+
		"\3\2\2\2\u00d2\u00d3\3\2\2\2\u00d3\u00de\3\2\2\2\u00d4\u00d5\7\4\2\2\u00d5"+
		"\u00d6\7#\2\2\u00d6\u00d7\5\24\13\2\u00d7\u00d8\7$\2\2\u00d8\u00d9\5&"+
		"\24\2\u00d9\u00db\7\31\2\2\u00da\u00dc\5\34\17\2\u00db\u00da\3\2\2\2\u00db"+
		"\u00dc\3\2\2\2\u00dc\u00de\3\2\2\2\u00dd\u00c1\3\2\2\2\u00dd\u00cb\3\2"+
		"\2\2\u00dd\u00d4\3\2\2\2\u00de\33\3\2\2\2\u00df\u00e0\7\5\2\2\u00e0\u00e1"+
		"\5\6\4\2\u00e1\u00e2\5\22\n\2\u00e2\u00e3\5\b\5\2\u00e3\u00ed\3\2\2\2"+
		"\u00e4\u00e5\7\5\2\2\u00e5\u00e6\5\24\13\2\u00e6\u00e7\7\31\2\2\u00e7"+
		"\u00ed\3\2\2\2\u00e8\u00e9\7\5\2\2\u00e9\u00ea\5&\24\2\u00ea\u00eb\7\31"+
		"\2\2\u00eb\u00ed\3\2\2\2\u00ec\u00df\3\2\2\2\u00ec\u00e4\3\2\2\2\u00ec"+
		"\u00e8\3\2\2\2\u00ed\35\3\2\2\2\u00ee\u00ef\7\6\2\2\u00ef\u00f0\7#\2\2"+
		"\u00f0\u00f1\5\24\13\2\u00f1\u00f2\7$\2\2\u00f2\u00f3\5\6\4\2\u00f3\u00f4"+
		"\5\22\n\2\u00f4\u00f5\5\b\5\2\u00f5\u0105\3\2\2\2\u00f6\u00f7\7\6\2\2"+
		"\u00f7\u00f8\7#\2\2\u00f8\u00f9\5\24\13\2\u00f9\u00fa\7$\2\2\u00fa\u00fb"+
		"\5\24\13\2\u00fb\u00fc\7\31\2\2\u00fc\u0105\3\2\2\2\u00fd\u00fe\7\6\2"+
		"\2\u00fe\u00ff\7#\2\2\u00ff\u0100\5\24\13\2\u0100\u0101\7$\2\2\u0101\u0102"+
		"\5&\24\2\u0102\u0103\7\31\2\2\u0103\u0105\3\2\2\2\u0104\u00ee\3\2\2\2"+
		"\u0104\u00f6\3\2\2\2\u0104\u00fd\3\2\2\2\u0105\37\3\2\2\2\u0106\u0109"+
		"\5\"\22\2\u0107\u0109\5$\23\2\u0108\u0106\3\2\2\2\u0108\u0107\3\2\2\2"+
		"\u0109!\3\2\2\2\u010a\u010b\5\4\3\2\u010b\u0110\7)\2\2\u010c\u010d\7("+
		"\2\2\u010d\u010f\7)\2\2\u010e\u010c\3\2\2\2\u010f\u0112\3\2\2\2\u0110"+
		"\u010e\3\2\2\2\u0110\u0111\3\2\2\2\u0111#\3\2\2\2\u0112\u0110\3\2\2\2"+
		"\u0113\u0114\5\4\3\2\u0114\u0115\7)\2\2\u0115\u0116\7\27\2\2\u0116\u0117"+
		"\5\24\13\2\u0117%\3\2\2\2\u0118\u0119\7\b\2\2\u0119\u011c\5\24\13\2\u011a"+
		"\u011c\7\b\2\2\u011b\u0118\3\2\2\2\u011b\u011a\3\2\2\2\u011c\'\3\2\2\2"+
		"\35\64;ILOQX^g\177\u0084\u0088\u0098\u009b\u00a0\u00b4\u00b6\u00bf\u00c9"+
		"\u00d2\u00db\u00dd\u00ec\u0104\u0108\u0110\u011b";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}