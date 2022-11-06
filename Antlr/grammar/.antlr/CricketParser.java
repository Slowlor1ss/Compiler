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
		T__0=1, T__1=2, T__2=3, T__3=4, WS=5, TReturn=6, Const=7, Number=8, CharLiteral=9, 
		TInt=10, TChar=11, TVoid=12, LessThan=13, GreaterThan=14, BitwiseAnd=15, 
		BitwiseOr=16, BitwiseXor=17, Equal=18, NotEqual=19, LessOrEqual=20, GreaterOrEqual=21, 
		Assign=22, Colon=23, StatementSeparator=24, Plus=25, Minus=26, Mul=27, 
		Div=28, Mod=29, PlusEqual=30, MinusEqual=31, MulEqual=32, DivEqual=33, 
		OpenPar=34, ClosePar=35, OpenCurly=36, CloseCurly=37, ExclamationMark=38, 
		Comma=39, Identifier=40, Comment=41, IncludeDirective=42, Newline=43;
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
			null, null, "'int'", "'char'", "'void'", "'<'", "'>'", "'&'", "'|'", 
			"'^'", "'=='", "'!='", "'<='", "'>='", "'='", "':'", "';'", "'+'", "'-'", 
			"'*'", "'/'", "'%'", "'+='", "'-='", "'*='", "'/='", "'('", "')'", "'{'", 
			"'}'", "'!'", "','"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, "WS", "TReturn", "Const", "Number", "CharLiteral", 
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
		public MainDeclrHeaderContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mainDeclrHeader; }
	 
		public MainDeclrHeaderContext() { }
		public void copyFrom(MainDeclrHeaderContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class MainDeclrHeaderWithRetContext extends MainDeclrHeaderContext {
		public Token FTYPE;
		public TerminalNode OpenPar() { return getToken(CricketParser.OpenPar, 0); }
		public TerminalNode ClosePar() { return getToken(CricketParser.ClosePar, 0); }
		public List<TerminalNode> TVoid() { return getTokens(CricketParser.TVoid); }
		public TerminalNode TVoid(int i) {
			return getToken(CricketParser.TVoid, i);
		}
		public TerminalNode TInt() { return getToken(CricketParser.TInt, 0); }
		public MainDeclrHeaderWithRetContext(MainDeclrHeaderContext ctx) { copyFrom(ctx); }
	}
	public static class MainDeclrHeaderNoRetContext extends MainDeclrHeaderContext {
		public TerminalNode OpenPar() { return getToken(CricketParser.OpenPar, 0); }
		public TerminalNode ClosePar() { return getToken(CricketParser.ClosePar, 0); }
		public TerminalNode TVoid() { return getToken(CricketParser.TVoid, 0); }
		public MainDeclrHeaderNoRetContext(MainDeclrHeaderContext ctx) { copyFrom(ctx); }
	}

	public final MainDeclrHeaderContext mainDeclrHeader() throws RecognitionException {
		MainDeclrHeaderContext _localctx = new MainDeclrHeaderContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_mainDeclrHeader);
		int _la;
		try {
			setState(101);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case TInt:
			case TVoid:
				_localctx = new MainDeclrHeaderWithRetContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(88);
				((MainDeclrHeaderWithRetContext)_localctx).FTYPE = _input.LT(1);
				_la = _input.LA(1);
				if ( !(_la==TInt || _la==TVoid) ) {
					((MainDeclrHeaderWithRetContext)_localctx).FTYPE = (Token)_errHandler.recoverInline(this);
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
				break;
			case T__0:
				_localctx = new MainDeclrHeaderNoRetContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(95);
				match(T__0);
				setState(96);
				match(OpenPar);
				setState(98);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==TVoid) {
					{
					setState(97);
					match(TVoid);
					}
				}

				setState(100);
				match(ClosePar);
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
			setState(103);
			mainDeclrHeader();
			setState(104);
			beginBlock();
			setState(105);
			body();
			setState(106);
			endBlock();
			setState(108);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==StatementSeparator) {
				{
				setState(107);
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
			setState(137);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case TInt:
			case TChar:
				enterOuterAlt(_localctx, 1);
				{
				setState(110);
				declr();
				setState(111);
				match(StatementSeparator);
				setState(112);
				body();
				}
				break;
			case Const:
			case Minus:
			case OpenPar:
			case ExclamationMark:
			case Identifier:
				enterOuterAlt(_localctx, 2);
				{
				setState(114);
				expr();
				setState(115);
				match(StatementSeparator);
				setState(116);
				body();
				}
				break;
			case TReturn:
				enterOuterAlt(_localctx, 3);
				{
				setState(118);
				flowControl();
				setState(119);
				match(StatementSeparator);
				setState(120);
				body();
				}
				break;
			case T__1:
				enterOuterAlt(_localctx, 4);
				{
				setState(122);
				ifStatement();
				setState(123);
				body();
				}
				break;
			case T__3:
				enterOuterAlt(_localctx, 5);
				{
				setState(125);
				whileStatement();
				setState(126);
				body();
				}
				break;
			case OpenCurly:
				enterOuterAlt(_localctx, 6);
				{
				setState(128);
				beginBlock();
				setState(129);
				body();
				setState(130);
				endBlock();
				setState(132);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==StatementSeparator) {
					{
					setState(131);
					match(StatementSeparator);
					}
				}

				setState(134);
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
			setState(141);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,13,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(139);
				assign();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(140);
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
		public Token OP2;
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
		public Token EQLG;
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
		public Token EBW;
		public PrimaryExprContext primaryExpr() {
			return getRuleContext(PrimaryExprContext.class,0);
		}
		public TerminalNode BitwiseAnd() { return getToken(CricketParser.BitwiseAnd, 0); }
		public TerminalNode BitwiseOr() { return getToken(CricketParser.BitwiseOr, 0); }
		public TerminalNode BitwiseXor() { return getToken(CricketParser.BitwiseXor, 0); }
		public BitwiseExprContext(PrimaryExprContext ctx) { copyFrom(ctx); }
	}
	public static class MulDivModExprContext extends PrimaryExprContext {
		public Token OP1;
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
		public TerminalNode Const() { return getToken(CricketParser.Const, 0); }
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
			setState(165);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,16,_ctx) ) {
			case 1:
				{
				_localctx = new ParExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;

				setState(144);
				match(OpenPar);
				setState(145);
				expr();
				setState(146);
				match(ClosePar);
				}
				break;
			case 2:
				{
				_localctx = new UnaryExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(148);
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
				setState(149);
				primaryExpr(10);
				}
				break;
			case 3:
				{
				_localctx = new FuncExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(150);
				match(Identifier);
				setState(151);
				match(OpenPar);
				setState(160);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Const) | (1L << Minus) | (1L << OpenPar) | (1L << ExclamationMark) | (1L << Identifier))) != 0)) {
					{
					setState(152);
					primaryExpr(0);
					setState(157);
					_errHandler.sync(this);
					_la = _input.LA(1);
					while (_la==Comma) {
						{
						{
						setState(153);
						match(Comma);
						setState(154);
						primaryExpr(0);
						}
						}
						setState(159);
						_errHandler.sync(this);
						_la = _input.LA(1);
					}
					}
				}

				setState(162);
				match(ClosePar);
				}
				break;
			case 4:
				{
				_localctx = new ConstExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(163);
				match(Const);
				}
				break;
			case 5:
				{
				_localctx = new VarExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(164);
				match(Identifier);
				}
				break;
			}
			_ctx.stop = _input.LT(-1);
			setState(186);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,18,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(184);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,17,_ctx) ) {
					case 1:
						{
						_localctx = new MulDivModExprContext(new PrimaryExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_primaryExpr);
						setState(167);
						if (!(precpred(_ctx, 9))) throw new FailedPredicateException(this, "precpred(_ctx, 9)");
						setState(168);
						((MulDivModExprContext)_localctx).OP1 = _input.LT(1);
						_la = _input.LA(1);
						if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Mul) | (1L << Div) | (1L << Mod))) != 0)) ) {
							((MulDivModExprContext)_localctx).OP1 = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(169);
						primaryExpr(10);
						}
						break;
					case 2:
						{
						_localctx = new AddSubExprContext(new PrimaryExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_primaryExpr);
						setState(170);
						if (!(precpred(_ctx, 8))) throw new FailedPredicateException(this, "precpred(_ctx, 8)");
						setState(171);
						((AddSubExprContext)_localctx).OP2 = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==Plus || _la==Minus) ) {
							((AddSubExprContext)_localctx).OP2 = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(172);
						primaryExpr(9);
						}
						break;
					case 3:
						{
						_localctx = new CmpLessOrGreaterExprContext(new PrimaryExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_primaryExpr);
						setState(173);
						if (!(precpred(_ctx, 7))) throw new FailedPredicateException(this, "precpred(_ctx, 7)");
						setState(174);
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
						setState(175);
						primaryExpr(8);
						}
						break;
					case 4:
						{
						_localctx = new CmpEqualityExprContext(new PrimaryExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_primaryExpr);
						setState(176);
						if (!(precpred(_ctx, 6))) throw new FailedPredicateException(this, "precpred(_ctx, 6)");
						setState(177);
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
						setState(178);
						primaryExpr(7);
						}
						break;
					case 5:
						{
						_localctx = new CmpEqualityLessGreaterExprContext(new PrimaryExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_primaryExpr);
						setState(179);
						if (!(precpred(_ctx, 5))) throw new FailedPredicateException(this, "precpred(_ctx, 5)");
						setState(180);
						((CmpEqualityLessGreaterExprContext)_localctx).EQLG = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==LessOrEqual || _la==GreaterOrEqual) ) {
							((CmpEqualityLessGreaterExprContext)_localctx).EQLG = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(181);
						primaryExpr(6);
						}
						break;
					case 6:
						{
						_localctx = new BitwiseExprContext(new PrimaryExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_primaryExpr);
						setState(182);
						if (!(precpred(_ctx, 4))) throw new FailedPredicateException(this, "precpred(_ctx, 4)");
						setState(183);
						((BitwiseExprContext)_localctx).EBW = _input.LT(1);
						_la = _input.LA(1);
						if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << BitwiseAnd) | (1L << BitwiseOr) | (1L << BitwiseXor))) != 0)) ) {
							((BitwiseExprContext)_localctx).EBW = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						}
						break;
					}
					} 
				}
				setState(188);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,18,_ctx);
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
			setState(195);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,19,_ctx) ) {
			case 1:
				_localctx = new AssignExprContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(189);
				match(Identifier);
				setState(190);
				match(Assign);
				setState(191);
				expr();
				}
				break;
			case 2:
				_localctx = new PmmdEqualContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(192);
				match(Identifier);
				setState(193);
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
				setState(194);
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
			setState(225);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,23,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(197);
				match(T__1);
				setState(198);
				match(OpenPar);
				setState(199);
				expr();
				setState(200);
				match(ClosePar);
				setState(201);
				beginBlock();
				setState(202);
				body();
				setState(203);
				endBlock();
				setState(205);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==T__2) {
					{
					setState(204);
					elseStatement();
					}
				}

				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(207);
				match(T__1);
				setState(208);
				match(OpenPar);
				setState(209);
				expr();
				setState(210);
				match(ClosePar);
				setState(211);
				expr();
				setState(212);
				match(StatementSeparator);
				setState(214);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==T__2) {
					{
					setState(213);
					elseStatement();
					}
				}

				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(216);
				match(T__1);
				setState(217);
				match(OpenPar);
				setState(218);
				expr();
				setState(219);
				match(ClosePar);
				setState(220);
				flowControl();
				setState(221);
				match(StatementSeparator);
				setState(223);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==T__2) {
					{
					setState(222);
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
			setState(240);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,24,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(227);
				match(T__2);
				setState(228);
				beginBlock();
				setState(229);
				body();
				setState(230);
				endBlock();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(232);
				match(T__2);
				setState(233);
				expr();
				setState(234);
				match(StatementSeparator);
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(236);
				match(T__2);
				setState(237);
				flowControl();
				setState(238);
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
			setState(264);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,25,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(242);
				match(T__3);
				setState(243);
				match(OpenPar);
				setState(244);
				expr();
				setState(245);
				match(ClosePar);
				setState(246);
				beginBlock();
				setState(247);
				body();
				setState(248);
				endBlock();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(250);
				match(T__3);
				setState(251);
				match(OpenPar);
				setState(252);
				expr();
				setState(253);
				match(ClosePar);
				setState(254);
				expr();
				setState(255);
				match(StatementSeparator);
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(257);
				match(T__3);
				setState(258);
				match(OpenPar);
				setState(259);
				expr();
				setState(260);
				match(ClosePar);
				setState(261);
				flowControl();
				setState(262);
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
			setState(268);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,26,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(266);
				varDeclr();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(267);
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
			setState(270);
			vartype();
			setState(271);
			match(Identifier);
			setState(276);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==Comma) {
				{
				{
				setState(272);
				match(Comma);
				setState(273);
				match(Identifier);
				}
				}
				setState(278);
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
			setState(279);
			vartype();
			setState(280);
			match(Identifier);
			setState(281);
			match(Assign);
			setState(282);
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
			setState(287);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,28,_ctx) ) {
			case 1:
				_localctx = new ExprReturnContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(284);
				match(TReturn);
				setState(285);
				expr();
				}
				break;
			case 2:
				_localctx = new EmptyReturnContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(286);
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
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\3-\u0124\4\2\t\2\4"+
		"\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t"+
		"\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\3\2\3\2\3\2\3\3\3\3\3\4\3\4\3\5\3\5\3\6\7\6\63\n"+
		"\6\f\6\16\6\66\13\6\3\6\3\6\7\6:\n\6\f\6\16\6=\13\6\3\7\3\7\3\7\3\7\3"+
		"\7\3\7\3\7\3\7\3\7\7\7H\n\7\f\7\16\7K\13\7\5\7M\n\7\3\7\5\7P\n\7\5\7R"+
		"\n\7\3\7\3\7\3\7\3\7\3\7\5\7Y\n\7\3\b\3\b\3\b\3\b\5\b_\n\b\3\b\3\b\3\b"+
		"\3\b\5\be\n\b\3\b\5\bh\n\b\3\t\3\t\3\t\3\t\3\t\5\to\n\t\3\n\3\n\3\n\3"+
		"\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n"+
		"\3\n\5\n\u0087\n\n\3\n\3\n\3\n\5\n\u008c\n\n\3\13\3\13\5\13\u0090\n\13"+
		"\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\7\f\u009e\n\f\f\f\16"+
		"\f\u00a1\13\f\5\f\u00a3\n\f\3\f\3\f\3\f\5\f\u00a8\n\f\3\f\3\f\3\f\3\f"+
		"\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\7\f\u00bb\n\f\f\f"+
		"\16\f\u00be\13\f\3\r\3\r\3\r\3\r\3\r\3\r\5\r\u00c6\n\r\3\16\3\16\3\16"+
		"\3\16\3\16\3\16\3\16\3\16\5\16\u00d0\n\16\3\16\3\16\3\16\3\16\3\16\3\16"+
		"\3\16\5\16\u00d9\n\16\3\16\3\16\3\16\3\16\3\16\3\16\3\16\5\16\u00e2\n"+
		"\16\5\16\u00e4\n\16\3\17\3\17\3\17\3\17\3\17\3\17\3\17\3\17\3\17\3\17"+
		"\3\17\3\17\3\17\5\17\u00f3\n\17\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20"+
		"\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20"+
		"\5\20\u010b\n\20\3\21\3\21\5\21\u010f\n\21\3\22\3\22\3\22\3\22\7\22\u0115"+
		"\n\22\f\22\16\22\u0118\13\22\3\23\3\23\3\23\3\23\3\23\3\24\3\24\3\24\5"+
		"\24\u0122\n\24\3\24\2\3\26\25\2\4\6\b\n\f\16\20\22\24\26\30\32\34\36 "+
		"\"$&\2\r\3\2\f\r\3\2\f\16\4\2\f\f\16\16\4\2\34\34((\3\2\35\37\3\2\33\34"+
		"\3\2\17\20\3\2\24\25\3\2\26\27\3\2\21\23\3\2 #\2\u013c\2(\3\2\2\2\4+\3"+
		"\2\2\2\6-\3\2\2\2\b/\3\2\2\2\n\64\3\2\2\2\f>\3\2\2\2\16g\3\2\2\2\20i\3"+
		"\2\2\2\22\u008b\3\2\2\2\24\u008f\3\2\2\2\26\u00a7\3\2\2\2\30\u00c5\3\2"+
		"\2\2\32\u00e3\3\2\2\2\34\u00f2\3\2\2\2\36\u010a\3\2\2\2 \u010e\3\2\2\2"+
		"\"\u0110\3\2\2\2$\u0119\3\2\2\2&\u0121\3\2\2\2()\5\n\6\2)*\7\2\2\3*\3"+
		"\3\2\2\2+,\t\2\2\2,\5\3\2\2\2-.\7&\2\2.\7\3\2\2\2/\60\7\'\2\2\60\t\3\2"+
		"\2\2\61\63\5\f\7\2\62\61\3\2\2\2\63\66\3\2\2\2\64\62\3\2\2\2\64\65\3\2"+
		"\2\2\65\67\3\2\2\2\66\64\3\2\2\2\67;\5\20\t\28:\5\f\7\298\3\2\2\2:=\3"+
		"\2\2\2;9\3\2\2\2;<\3\2\2\2<\13\3\2\2\2=;\3\2\2\2>?\t\3\2\2?@\7*\2\2@Q"+
		"\7$\2\2AB\5\4\3\2BI\7*\2\2CD\7)\2\2DE\5\4\3\2EF\7*\2\2FH\3\2\2\2GC\3\2"+
		"\2\2HK\3\2\2\2IG\3\2\2\2IJ\3\2\2\2JM\3\2\2\2KI\3\2\2\2LA\3\2\2\2LM\3\2"+
		"\2\2MR\3\2\2\2NP\7\16\2\2ON\3\2\2\2OP\3\2\2\2PR\3\2\2\2QL\3\2\2\2QO\3"+
		"\2\2\2RS\3\2\2\2ST\7%\2\2TU\5\6\4\2UV\5\22\n\2VX\5\b\5\2WY\7\32\2\2XW"+
		"\3\2\2\2XY\3\2\2\2Y\r\3\2\2\2Z[\t\4\2\2[\\\7\3\2\2\\^\7$\2\2]_\7\16\2"+
		"\2^]\3\2\2\2^_\3\2\2\2_`\3\2\2\2`h\7%\2\2ab\7\3\2\2bd\7$\2\2ce\7\16\2"+
		"\2dc\3\2\2\2de\3\2\2\2ef\3\2\2\2fh\7%\2\2gZ\3\2\2\2ga\3\2\2\2h\17\3\2"+
		"\2\2ij\5\16\b\2jk\5\6\4\2kl\5\22\n\2ln\5\b\5\2mo\7\32\2\2nm\3\2\2\2no"+
		"\3\2\2\2o\21\3\2\2\2pq\5 \21\2qr\7\32\2\2rs\5\22\n\2s\u008c\3\2\2\2tu"+
		"\5\24\13\2uv\7\32\2\2vw\5\22\n\2w\u008c\3\2\2\2xy\5&\24\2yz\7\32\2\2z"+
		"{\5\22\n\2{\u008c\3\2\2\2|}\5\32\16\2}~\5\22\n\2~\u008c\3\2\2\2\177\u0080"+
		"\5\36\20\2\u0080\u0081\5\22\n\2\u0081\u008c\3\2\2\2\u0082\u0083\5\6\4"+
		"\2\u0083\u0084\5\22\n\2\u0084\u0086\5\b\5\2\u0085\u0087\7\32\2\2\u0086"+
		"\u0085\3\2\2\2\u0086\u0087\3\2\2\2\u0087\u0088\3\2\2\2\u0088\u0089\5\22"+
		"\n\2\u0089\u008c\3\2\2\2\u008a\u008c\3\2\2\2\u008bp\3\2\2\2\u008bt\3\2"+
		"\2\2\u008bx\3\2\2\2\u008b|\3\2\2\2\u008b\177\3\2\2\2\u008b\u0082\3\2\2"+
		"\2\u008b\u008a\3\2\2\2\u008c\23\3\2\2\2\u008d\u0090\5\30\r\2\u008e\u0090"+
		"\5\26\f\2\u008f\u008d\3\2\2\2\u008f\u008e\3\2\2\2\u0090\25\3\2\2\2\u0091"+
		"\u0092\b\f\1\2\u0092\u0093\7$\2\2\u0093\u0094\5\24\13\2\u0094\u0095\7"+
		"%\2\2\u0095\u00a8\3\2\2\2\u0096\u0097\t\5\2\2\u0097\u00a8\5\26\f\f\u0098"+
		"\u0099\7*\2\2\u0099\u00a2\7$\2\2\u009a\u009f\5\26\f\2\u009b\u009c\7)\2"+
		"\2\u009c\u009e\5\26\f\2\u009d\u009b\3\2\2\2\u009e\u00a1\3\2\2\2\u009f"+
		"\u009d\3\2\2\2\u009f\u00a0\3\2\2\2\u00a0\u00a3\3\2\2\2\u00a1\u009f\3\2"+
		"\2\2\u00a2\u009a\3\2\2\2\u00a2\u00a3\3\2\2\2\u00a3\u00a4\3\2\2\2\u00a4"+
		"\u00a8\7%\2\2\u00a5\u00a8\7\t\2\2\u00a6\u00a8\7*\2\2\u00a7\u0091\3\2\2"+
		"\2\u00a7\u0096\3\2\2\2\u00a7\u0098\3\2\2\2\u00a7\u00a5\3\2\2\2\u00a7\u00a6"+
		"\3\2\2\2\u00a8\u00bc\3\2\2\2\u00a9\u00aa\f\13\2\2\u00aa\u00ab\t\6\2\2"+
		"\u00ab\u00bb\5\26\f\f\u00ac\u00ad\f\n\2\2\u00ad\u00ae\t\7\2\2\u00ae\u00bb"+
		"\5\26\f\13\u00af\u00b0\f\t\2\2\u00b0\u00b1\t\b\2\2\u00b1\u00bb\5\26\f"+
		"\n\u00b2\u00b3\f\b\2\2\u00b3\u00b4\t\t\2\2\u00b4\u00bb\5\26\f\t\u00b5"+
		"\u00b6\f\7\2\2\u00b6\u00b7\t\n\2\2\u00b7\u00bb\5\26\f\b\u00b8\u00b9\f"+
		"\6\2\2\u00b9\u00bb\t\13\2\2\u00ba\u00a9\3\2\2\2\u00ba\u00ac\3\2\2\2\u00ba"+
		"\u00af\3\2\2\2\u00ba\u00b2\3\2\2\2\u00ba\u00b5\3\2\2\2\u00ba\u00b8\3\2"+
		"\2\2\u00bb\u00be\3\2\2\2\u00bc\u00ba\3\2\2\2\u00bc\u00bd\3\2\2\2\u00bd"+
		"\27\3\2\2\2\u00be\u00bc\3\2\2\2\u00bf\u00c0\7*\2\2\u00c0\u00c1\7\30\2"+
		"\2\u00c1\u00c6\5\24\13\2\u00c2\u00c3\7*\2\2\u00c3\u00c4\t\f\2\2\u00c4"+
		"\u00c6\5\26\f\2\u00c5\u00bf\3\2\2\2\u00c5\u00c2\3\2\2\2\u00c6\31\3\2\2"+
		"\2\u00c7\u00c8\7\4\2\2\u00c8\u00c9\7$\2\2\u00c9\u00ca\5\24\13\2\u00ca"+
		"\u00cb\7%\2\2\u00cb\u00cc\5\6\4\2\u00cc\u00cd\5\22\n\2\u00cd\u00cf\5\b"+
		"\5\2\u00ce\u00d0\5\34\17\2\u00cf\u00ce\3\2\2\2\u00cf\u00d0\3\2\2\2\u00d0"+
		"\u00e4\3\2\2\2\u00d1\u00d2\7\4\2\2\u00d2\u00d3\7$\2\2\u00d3\u00d4\5\24"+
		"\13\2\u00d4\u00d5\7%\2\2\u00d5\u00d6\5\24\13\2\u00d6\u00d8\7\32\2\2\u00d7"+
		"\u00d9\5\34\17\2\u00d8\u00d7\3\2\2\2\u00d8\u00d9\3\2\2\2\u00d9\u00e4\3"+
		"\2\2\2\u00da\u00db\7\4\2\2\u00db\u00dc\7$\2\2\u00dc\u00dd\5\24\13\2\u00dd"+
		"\u00de\7%\2\2\u00de\u00df\5&\24\2\u00df\u00e1\7\32\2\2\u00e0\u00e2\5\34"+
		"\17\2\u00e1\u00e0\3\2\2\2\u00e1\u00e2\3\2\2\2\u00e2\u00e4\3\2\2\2\u00e3"+
		"\u00c7\3\2\2\2\u00e3\u00d1\3\2\2\2\u00e3\u00da\3\2\2\2\u00e4\33\3\2\2"+
		"\2\u00e5\u00e6\7\5\2\2\u00e6\u00e7\5\6\4\2\u00e7\u00e8\5\22\n\2\u00e8"+
		"\u00e9\5\b\5\2\u00e9\u00f3\3\2\2\2\u00ea\u00eb\7\5\2\2\u00eb\u00ec\5\24"+
		"\13\2\u00ec\u00ed\7\32\2\2\u00ed\u00f3\3\2\2\2\u00ee\u00ef\7\5\2\2\u00ef"+
		"\u00f0\5&\24\2\u00f0\u00f1\7\32\2\2\u00f1\u00f3\3\2\2\2\u00f2\u00e5\3"+
		"\2\2\2\u00f2\u00ea\3\2\2\2\u00f2\u00ee\3\2\2\2\u00f3\35\3\2\2\2\u00f4"+
		"\u00f5\7\6\2\2\u00f5\u00f6\7$\2\2\u00f6\u00f7\5\24\13\2\u00f7\u00f8\7"+
		"%\2\2\u00f8\u00f9\5\6\4\2\u00f9\u00fa\5\22\n\2\u00fa\u00fb\5\b\5\2\u00fb"+
		"\u010b\3\2\2\2\u00fc\u00fd\7\6\2\2\u00fd\u00fe\7$\2\2\u00fe\u00ff\5\24"+
		"\13\2\u00ff\u0100\7%\2\2\u0100\u0101\5\24\13\2\u0101\u0102\7\32\2\2\u0102"+
		"\u010b\3\2\2\2\u0103\u0104\7\6\2\2\u0104\u0105\7$\2\2\u0105\u0106\5\24"+
		"\13\2\u0106\u0107\7%\2\2\u0107\u0108\5&\24\2\u0108\u0109\7\32\2\2\u0109"+
		"\u010b\3\2\2\2\u010a\u00f4\3\2\2\2\u010a\u00fc\3\2\2\2\u010a\u0103\3\2"+
		"\2\2\u010b\37\3\2\2\2\u010c\u010f\5\"\22\2\u010d\u010f\5$\23\2\u010e\u010c"+
		"\3\2\2\2\u010e\u010d\3\2\2\2\u010f!\3\2\2\2\u0110\u0111\5\4\3\2\u0111"+
		"\u0116\7*\2\2\u0112\u0113\7)\2\2\u0113\u0115\7*\2\2\u0114\u0112\3\2\2"+
		"\2\u0115\u0118\3\2\2\2\u0116\u0114\3\2\2\2\u0116\u0117\3\2\2\2\u0117#"+
		"\3\2\2\2\u0118\u0116\3\2\2\2\u0119\u011a\5\4\3\2\u011a\u011b\7*\2\2\u011b"+
		"\u011c\7\30\2\2\u011c\u011d\5\24\13\2\u011d%\3\2\2\2\u011e\u011f\7\b\2"+
		"\2\u011f\u0122\5\24\13\2\u0120\u0122\7\b\2\2\u0121\u011e\3\2\2\2\u0121"+
		"\u0120\3\2\2\2\u0122\'\3\2\2\2\37\64;ILOQX^dgn\u0086\u008b\u008f\u009f"+
		"\u00a2\u00a7\u00ba\u00bc\u00c5\u00cf\u00d8\u00e1\u00e3\u00f2\u010a\u010e"+
		"\u0116\u0121";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}