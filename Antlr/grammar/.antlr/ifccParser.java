// Generated from c:\School_3rdyr\Gradwork\Project\Compiler\Antlr\grammar\ifcc.g4 by ANTLR 4.9.2
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class ifccParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.9.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, T__10=11, T__11=12, T__12=13, T__13=14, T__14=15, T__15=16, T__16=17, 
		T__17=18, T__18=19, T__19=20, T__20=21, T__21=22, T__22=23, T__23=24, 
		T__24=25, T__25=26, T__26=27, T__27=28, T__28=29, T__29=30, WS=31, RETURN=32, 
		CONST=33, NUMBER=34, CHAR=35, TINT=36, TCHAR=37, TVOID=38, TOKENNAME=39, 
		MULTICOMMENT=40, SINGLECOMMENT=41, DIRECTIVE=42;
	public static final int
		RULE_axiom = 0, RULE_vtype = 1, RULE_beginBlock = 2, RULE_endBlock = 3, 
		RULE_prog = 4, RULE_funcDeclr = 5, RULE_mainDeclrHeader = 6, RULE_mainDeclr = 7, 
		RULE_body = 8, RULE_expr2 = 9, RULE_expr = 10, RULE_affect = 11, RULE_ifStatement = 12, 
		RULE_elseStatement = 13, RULE_whileStatement = 14, RULE_declr = 15, RULE_varDeclr = 16, 
		RULE_varDeclrAndAffect = 17, RULE_end = 18;
	private static String[] makeRuleNames() {
		return new String[] {
			"axiom", "vtype", "beginBlock", "endBlock", "prog", "funcDeclr", "mainDeclrHeader", 
			"mainDeclr", "body", "expr2", "expr", "affect", "ifStatement", "elseStatement", 
			"whileStatement", "declr", "varDeclr", "varDeclrAndAffect", "end"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'{'", "'}'", "'('", "','", "')'", "';'", "'main'", "'-'", "'!'", 
			"'*'", "'/'", "'%'", "'+'", "'<'", "'>'", "'=='", "'!='", "'<='", "'>='", 
			"'&'", "'^'", "'|'", "'='", "'+='", "'-='", "'*='", "'/='", "'if'", "'else'", 
			"'while'", null, "'return'", null, null, null, "'int'", "'char'", "'void'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, "WS", "RETURN", "CONST", "NUMBER", 
			"CHAR", "TINT", "TCHAR", "TVOID", "TOKENNAME", "MULTICOMMENT", "SINGLECOMMENT", 
			"DIRECTIVE"
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
	public String getGrammarFileName() { return "ifcc.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public ifccParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	public static class AxiomContext extends ParserRuleContext {
		public ProgContext prog() {
			return getRuleContext(ProgContext.class,0);
		}
		public TerminalNode EOF() { return getToken(ifccParser.EOF, 0); }
		public AxiomContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_axiom; }
	}

	public final AxiomContext axiom() throws RecognitionException {
		AxiomContext _localctx = new AxiomContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_axiom);
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

	public static class VtypeContext extends ParserRuleContext {
		public TerminalNode TINT() { return getToken(ifccParser.TINT, 0); }
		public TerminalNode TCHAR() { return getToken(ifccParser.TCHAR, 0); }
		public VtypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_vtype; }
	}

	public final VtypeContext vtype() throws RecognitionException {
		VtypeContext _localctx = new VtypeContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_vtype);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(41);
			_la = _input.LA(1);
			if ( !(_la==TINT || _la==TCHAR) ) {
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
			match(T__0);
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
			match(T__1);
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
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << TINT) | (1L << TCHAR) | (1L << TVOID))) != 0)) {
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
		public List<TerminalNode> TOKENNAME() { return getTokens(ifccParser.TOKENNAME); }
		public TerminalNode TOKENNAME(int i) {
			return getToken(ifccParser.TOKENNAME, i);
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
		public List<TerminalNode> TVOID() { return getTokens(ifccParser.TVOID); }
		public TerminalNode TVOID(int i) {
			return getToken(ifccParser.TVOID, i);
		}
		public TerminalNode TINT() { return getToken(ifccParser.TINT, 0); }
		public TerminalNode TCHAR() { return getToken(ifccParser.TCHAR, 0); }
		public List<VtypeContext> vtype() {
			return getRuleContexts(VtypeContext.class);
		}
		public VtypeContext vtype(int i) {
			return getRuleContext(VtypeContext.class,i);
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
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << TINT) | (1L << TCHAR) | (1L << TVOID))) != 0)) ) {
				((FuncDeclrContext)_localctx).FTYPE = (Token)_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			setState(61);
			match(TOKENNAME);
			setState(62);
			match(T__2);
			setState(79);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,5,_ctx) ) {
			case 1:
				{
				setState(74);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==TINT || _la==TCHAR) {
					{
					setState(63);
					vtype();
					setState(64);
					match(TOKENNAME);
					setState(71);
					_errHandler.sync(this);
					_la = _input.LA(1);
					while (_la==T__3) {
						{
						{
						setState(65);
						match(T__3);
						setState(66);
						vtype();
						setState(67);
						match(TOKENNAME);
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
				if (_la==TVOID) {
					{
					setState(76);
					match(TVOID);
					}
				}

				}
				break;
			}
			setState(81);
			match(T__4);
			setState(82);
			beginBlock();
			setState(83);
			body();
			setState(84);
			endBlock();
			setState(86);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__5) {
				{
				setState(85);
				match(T__5);
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
		public List<TerminalNode> TVOID() { return getTokens(ifccParser.TVOID); }
		public TerminalNode TVOID(int i) {
			return getToken(ifccParser.TVOID, i);
		}
		public TerminalNode TINT() { return getToken(ifccParser.TINT, 0); }
		public MainDeclrHeaderWithRetContext(MainDeclrHeaderContext ctx) { copyFrom(ctx); }
	}
	public static class MainDeclrHeaderNoRetContext extends MainDeclrHeaderContext {
		public TerminalNode TVOID() { return getToken(ifccParser.TVOID, 0); }
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
			case TINT:
			case TVOID:
				_localctx = new MainDeclrHeaderWithRetContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(88);
				((MainDeclrHeaderWithRetContext)_localctx).FTYPE = _input.LT(1);
				_la = _input.LA(1);
				if ( !(_la==TINT || _la==TVOID) ) {
					((MainDeclrHeaderWithRetContext)_localctx).FTYPE = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(89);
				match(T__6);
				setState(90);
				match(T__2);
				setState(92);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==TVOID) {
					{
					setState(91);
					match(TVOID);
					}
				}

				setState(94);
				match(T__4);
				}
				break;
			case T__6:
				_localctx = new MainDeclrHeaderNoRetContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(95);
				match(T__6);
				setState(96);
				match(T__2);
				setState(98);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==TVOID) {
					{
					setState(97);
					match(TVOID);
					}
				}

				setState(100);
				match(T__4);
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
			if (_la==T__5) {
				{
				setState(107);
				match(T__5);
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
		public List<BodyContext> body() {
			return getRuleContexts(BodyContext.class);
		}
		public BodyContext body(int i) {
			return getRuleContext(BodyContext.class,i);
		}
		public Expr2Context expr2() {
			return getRuleContext(Expr2Context.class,0);
		}
		public EndContext end() {
			return getRuleContext(EndContext.class,0);
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
			case TINT:
			case TCHAR:
				enterOuterAlt(_localctx, 1);
				{
				setState(110);
				declr();
				setState(111);
				match(T__5);
				setState(112);
				body();
				}
				break;
			case T__2:
			case T__7:
			case T__8:
			case CONST:
			case TOKENNAME:
				enterOuterAlt(_localctx, 2);
				{
				setState(114);
				expr2();
				setState(115);
				match(T__5);
				setState(116);
				body();
				}
				break;
			case RETURN:
				enterOuterAlt(_localctx, 3);
				{
				setState(118);
				end();
				setState(119);
				match(T__5);
				setState(120);
				body();
				}
				break;
			case T__27:
				enterOuterAlt(_localctx, 4);
				{
				setState(122);
				ifStatement();
				setState(123);
				body();
				}
				break;
			case T__29:
				enterOuterAlt(_localctx, 5);
				{
				setState(125);
				whileStatement();
				setState(126);
				body();
				}
				break;
			case T__0:
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
				if (_la==T__5) {
					{
					setState(131);
					match(T__5);
					}
				}

				setState(134);
				body();
				}
				break;
			case T__1:
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

	public static class Expr2Context extends ParserRuleContext {
		public AffectContext affect() {
			return getRuleContext(AffectContext.class,0);
		}
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public Expr2Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expr2; }
	}

	public final Expr2Context expr2() throws RecognitionException {
		Expr2Context _localctx = new Expr2Context(_ctx, getState());
		enterRule(_localctx, 18, RULE_expr2);
		try {
			setState(141);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,13,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(139);
				affect();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(140);
				expr(0);
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

	public static class ExprContext extends ParserRuleContext {
		public ExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expr; }
	 
		public ExprContext() { }
		public void copyFrom(ExprContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class FuncExprContext extends ExprContext {
		public TerminalNode TOKENNAME() { return getToken(ifccParser.TOKENNAME, 0); }
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public FuncExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class AddSubExprContext extends ExprContext {
		public Token OP2;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public AddSubExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class OrExprContext extends ExprContext {
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public OrExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class MulDivModExprContext extends ExprContext {
		public Token OP1;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public MulDivModExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class CmpEqualityExprContext extends ExprContext {
		public Token EQ;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public CmpEqualityExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class XorExprContext extends ExprContext {
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public XorExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class ParExprContext extends ExprContext {
		public Expr2Context expr2() {
			return getRuleContext(Expr2Context.class,0);
		}
		public ParExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class VarExprContext extends ExprContext {
		public TerminalNode TOKENNAME() { return getToken(ifccParser.TOKENNAME, 0); }
		public VarExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class UnaryExprContext extends ExprContext {
		public Token UNARY;
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public UnaryExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class CmpEqualityLessGreaterExprContext extends ExprContext {
		public Token EQLG;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public CmpEqualityLessGreaterExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class CmpLessOrGreaterExprContext extends ExprContext {
		public Token CMP;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public CmpLessOrGreaterExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class ConstExprContext extends ExprContext {
		public TerminalNode CONST() { return getToken(ifccParser.CONST, 0); }
		public ConstExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class AndExprContext extends ExprContext {
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public AndExprContext(ExprContext ctx) { copyFrom(ctx); }
	}

	public final ExprContext expr() throws RecognitionException {
		return expr(0);
	}

	private ExprContext expr(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		ExprContext _localctx = new ExprContext(_ctx, _parentState);
		ExprContext _prevctx = _localctx;
		int _startState = 20;
		enterRecursionRule(_localctx, 20, RULE_expr, _p);
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
				match(T__2);
				setState(145);
				expr2();
				setState(146);
				match(T__4);
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
				if ( !(_la==T__7 || _la==T__8) ) {
					((UnaryExprContext)_localctx).UNARY = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(149);
				expr(12);
				}
				break;
			case 3:
				{
				_localctx = new FuncExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(150);
				match(TOKENNAME);
				setState(151);
				match(T__2);
				setState(160);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << T__2) | (1L << T__7) | (1L << T__8) | (1L << CONST) | (1L << TOKENNAME))) != 0)) {
					{
					setState(152);
					expr(0);
					setState(157);
					_errHandler.sync(this);
					_la = _input.LA(1);
					while (_la==T__3) {
						{
						{
						setState(153);
						match(T__3);
						setState(154);
						expr(0);
						}
						}
						setState(159);
						_errHandler.sync(this);
						_la = _input.LA(1);
					}
					}
				}

				setState(162);
				match(T__4);
				}
				break;
			case 4:
				{
				_localctx = new ConstExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(163);
				match(CONST);
				}
				break;
			case 5:
				{
				_localctx = new VarExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(164);
				match(TOKENNAME);
				}
				break;
			}
			_ctx.stop = _input.LT(-1);
			setState(193);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,18,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(191);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,17,_ctx) ) {
					case 1:
						{
						_localctx = new MulDivModExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(167);
						if (!(precpred(_ctx, 11))) throw new FailedPredicateException(this, "precpred(_ctx, 11)");
						setState(168);
						((MulDivModExprContext)_localctx).OP1 = _input.LT(1);
						_la = _input.LA(1);
						if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << T__9) | (1L << T__10) | (1L << T__11))) != 0)) ) {
							((MulDivModExprContext)_localctx).OP1 = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(169);
						expr(12);
						}
						break;
					case 2:
						{
						_localctx = new AddSubExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(170);
						if (!(precpred(_ctx, 10))) throw new FailedPredicateException(this, "precpred(_ctx, 10)");
						setState(171);
						((AddSubExprContext)_localctx).OP2 = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==T__7 || _la==T__12) ) {
							((AddSubExprContext)_localctx).OP2 = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(172);
						expr(11);
						}
						break;
					case 3:
						{
						_localctx = new CmpLessOrGreaterExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(173);
						if (!(precpred(_ctx, 9))) throw new FailedPredicateException(this, "precpred(_ctx, 9)");
						setState(174);
						((CmpLessOrGreaterExprContext)_localctx).CMP = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==T__13 || _la==T__14) ) {
							((CmpLessOrGreaterExprContext)_localctx).CMP = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(175);
						expr(10);
						}
						break;
					case 4:
						{
						_localctx = new CmpEqualityExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(176);
						if (!(precpred(_ctx, 8))) throw new FailedPredicateException(this, "precpred(_ctx, 8)");
						setState(177);
						((CmpEqualityExprContext)_localctx).EQ = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==T__15 || _la==T__16) ) {
							((CmpEqualityExprContext)_localctx).EQ = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(178);
						expr(9);
						}
						break;
					case 5:
						{
						_localctx = new CmpEqualityLessGreaterExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(179);
						if (!(precpred(_ctx, 7))) throw new FailedPredicateException(this, "precpred(_ctx, 7)");
						setState(180);
						((CmpEqualityLessGreaterExprContext)_localctx).EQLG = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==T__17 || _la==T__18) ) {
							((CmpEqualityLessGreaterExprContext)_localctx).EQLG = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(181);
						expr(8);
						}
						break;
					case 6:
						{
						_localctx = new AndExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(182);
						if (!(precpred(_ctx, 6))) throw new FailedPredicateException(this, "precpred(_ctx, 6)");
						setState(183);
						match(T__19);
						setState(184);
						expr(7);
						}
						break;
					case 7:
						{
						_localctx = new XorExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(185);
						if (!(precpred(_ctx, 5))) throw new FailedPredicateException(this, "precpred(_ctx, 5)");
						setState(186);
						match(T__20);
						setState(187);
						expr(6);
						}
						break;
					case 8:
						{
						_localctx = new OrExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(188);
						if (!(precpred(_ctx, 4))) throw new FailedPredicateException(this, "precpred(_ctx, 4)");
						setState(189);
						match(T__21);
						setState(190);
						expr(5);
						}
						break;
					}
					} 
				}
				setState(195);
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

	public static class AffectContext extends ParserRuleContext {
		public AffectContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_affect; }
	 
		public AffectContext() { }
		public void copyFrom(AffectContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class PmmdEqualContext extends AffectContext {
		public Token OPPMMD;
		public TerminalNode TOKENNAME() { return getToken(ifccParser.TOKENNAME, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public PmmdEqualContext(AffectContext ctx) { copyFrom(ctx); }
	}
	public static class AffExprContext extends AffectContext {
		public TerminalNode TOKENNAME() { return getToken(ifccParser.TOKENNAME, 0); }
		public Expr2Context expr2() {
			return getRuleContext(Expr2Context.class,0);
		}
		public AffExprContext(AffectContext ctx) { copyFrom(ctx); }
	}

	public final AffectContext affect() throws RecognitionException {
		AffectContext _localctx = new AffectContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_affect);
		int _la;
		try {
			setState(202);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,19,_ctx) ) {
			case 1:
				_localctx = new AffExprContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(196);
				match(TOKENNAME);
				setState(197);
				match(T__22);
				setState(198);
				expr2();
				}
				break;
			case 2:
				_localctx = new PmmdEqualContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(199);
				match(TOKENNAME);
				setState(200);
				((PmmdEqualContext)_localctx).OPPMMD = _input.LT(1);
				_la = _input.LA(1);
				if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << T__23) | (1L << T__24) | (1L << T__25) | (1L << T__26))) != 0)) ) {
					((PmmdEqualContext)_localctx).OPPMMD = (Token)_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(201);
				expr(0);
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
		public List<Expr2Context> expr2() {
			return getRuleContexts(Expr2Context.class);
		}
		public Expr2Context expr2(int i) {
			return getRuleContext(Expr2Context.class,i);
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
		public ElseStatementContext elseStatement() {
			return getRuleContext(ElseStatementContext.class,0);
		}
		public EndContext end() {
			return getRuleContext(EndContext.class,0);
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
			setState(232);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,23,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(204);
				match(T__27);
				setState(205);
				match(T__2);
				setState(206);
				expr2();
				setState(207);
				match(T__4);
				setState(208);
				beginBlock();
				setState(209);
				body();
				setState(210);
				endBlock();
				setState(212);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==T__28) {
					{
					setState(211);
					elseStatement();
					}
				}

				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(214);
				match(T__27);
				setState(215);
				match(T__2);
				setState(216);
				expr2();
				setState(217);
				match(T__4);
				setState(218);
				expr2();
				setState(219);
				match(T__5);
				setState(221);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==T__28) {
					{
					setState(220);
					elseStatement();
					}
				}

				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(223);
				match(T__27);
				setState(224);
				match(T__2);
				setState(225);
				expr2();
				setState(226);
				match(T__4);
				setState(227);
				end();
				setState(228);
				match(T__5);
				setState(230);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (_la==T__28) {
					{
					setState(229);
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
		public Expr2Context expr2() {
			return getRuleContext(Expr2Context.class,0);
		}
		public EndContext end() {
			return getRuleContext(EndContext.class,0);
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
			setState(247);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,24,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(234);
				match(T__28);
				setState(235);
				beginBlock();
				setState(236);
				body();
				setState(237);
				endBlock();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(239);
				match(T__28);
				setState(240);
				expr2();
				setState(241);
				match(T__5);
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(243);
				match(T__28);
				setState(244);
				end();
				setState(245);
				match(T__5);
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
		public List<Expr2Context> expr2() {
			return getRuleContexts(Expr2Context.class);
		}
		public Expr2Context expr2(int i) {
			return getRuleContext(Expr2Context.class,i);
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
		public EndContext end() {
			return getRuleContext(EndContext.class,0);
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
			setState(271);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,25,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(249);
				match(T__29);
				setState(250);
				match(T__2);
				setState(251);
				expr2();
				setState(252);
				match(T__4);
				setState(253);
				beginBlock();
				setState(254);
				body();
				setState(255);
				endBlock();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(257);
				match(T__29);
				setState(258);
				match(T__2);
				setState(259);
				expr2();
				setState(260);
				match(T__4);
				setState(261);
				expr2();
				setState(262);
				match(T__5);
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(264);
				match(T__29);
				setState(265);
				match(T__2);
				setState(266);
				expr2();
				setState(267);
				match(T__4);
				setState(268);
				end();
				setState(269);
				match(T__5);
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
		public VarDeclrAndAffectContext varDeclrAndAffect() {
			return getRuleContext(VarDeclrAndAffectContext.class,0);
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
			setState(275);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,26,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(273);
				varDeclr();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(274);
				varDeclrAndAffect();
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
		public VtypeContext vtype() {
			return getRuleContext(VtypeContext.class,0);
		}
		public List<TerminalNode> TOKENNAME() { return getTokens(ifccParser.TOKENNAME); }
		public TerminalNode TOKENNAME(int i) {
			return getToken(ifccParser.TOKENNAME, i);
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
			setState(277);
			vtype();
			setState(278);
			match(TOKENNAME);
			setState(283);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__3) {
				{
				{
				setState(279);
				match(T__3);
				setState(280);
				match(TOKENNAME);
				}
				}
				setState(285);
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

	public static class VarDeclrAndAffectContext extends ParserRuleContext {
		public VtypeContext vtype() {
			return getRuleContext(VtypeContext.class,0);
		}
		public TerminalNode TOKENNAME() { return getToken(ifccParser.TOKENNAME, 0); }
		public Expr2Context expr2() {
			return getRuleContext(Expr2Context.class,0);
		}
		public VarDeclrAndAffectContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_varDeclrAndAffect; }
	}

	public final VarDeclrAndAffectContext varDeclrAndAffect() throws RecognitionException {
		VarDeclrAndAffectContext _localctx = new VarDeclrAndAffectContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_varDeclrAndAffect);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(286);
			vtype();
			setState(287);
			match(TOKENNAME);
			setState(288);
			match(T__22);
			setState(289);
			expr2();
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

	public static class EndContext extends ParserRuleContext {
		public EndContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_end; }
	 
		public EndContext() { }
		public void copyFrom(EndContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class EmptyEndContext extends EndContext {
		public TerminalNode RETURN() { return getToken(ifccParser.RETURN, 0); }
		public EmptyEndContext(EndContext ctx) { copyFrom(ctx); }
	}
	public static class ExprEndContext extends EndContext {
		public TerminalNode RETURN() { return getToken(ifccParser.RETURN, 0); }
		public Expr2Context expr2() {
			return getRuleContext(Expr2Context.class,0);
		}
		public ExprEndContext(EndContext ctx) { copyFrom(ctx); }
	}

	public final EndContext end() throws RecognitionException {
		EndContext _localctx = new EndContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_end);
		try {
			setState(294);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,28,_ctx) ) {
			case 1:
				_localctx = new ExprEndContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(291);
				match(RETURN);
				setState(292);
				expr2();
				}
				break;
			case 2:
				_localctx = new EmptyEndContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(293);
				match(RETURN);
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
			return expr_sempred((ExprContext)_localctx, predIndex);
		}
		return true;
	}
	private boolean expr_sempred(ExprContext _localctx, int predIndex) {
		switch (predIndex) {
		case 0:
			return precpred(_ctx, 11);
		case 1:
			return precpred(_ctx, 10);
		case 2:
			return precpred(_ctx, 9);
		case 3:
			return precpred(_ctx, 8);
		case 4:
			return precpred(_ctx, 7);
		case 5:
			return precpred(_ctx, 6);
		case 6:
			return precpred(_ctx, 5);
		case 7:
			return precpred(_ctx, 4);
		}
		return true;
	}

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\3,\u012b\4\2\t\2\4"+
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
		"\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3\f\3"+
		"\f\3\f\3\f\7\f\u00c2\n\f\f\f\16\f\u00c5\13\f\3\r\3\r\3\r\3\r\3\r\3\r\5"+
		"\r\u00cd\n\r\3\16\3\16\3\16\3\16\3\16\3\16\3\16\3\16\5\16\u00d7\n\16\3"+
		"\16\3\16\3\16\3\16\3\16\3\16\3\16\5\16\u00e0\n\16\3\16\3\16\3\16\3\16"+
		"\3\16\3\16\3\16\5\16\u00e9\n\16\5\16\u00eb\n\16\3\17\3\17\3\17\3\17\3"+
		"\17\3\17\3\17\3\17\3\17\3\17\3\17\3\17\3\17\5\17\u00fa\n\17\3\20\3\20"+
		"\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20"+
		"\3\20\3\20\3\20\3\20\3\20\3\20\5\20\u0112\n\20\3\21\3\21\5\21\u0116\n"+
		"\21\3\22\3\22\3\22\3\22\7\22\u011c\n\22\f\22\16\22\u011f\13\22\3\23\3"+
		"\23\3\23\3\23\3\23\3\24\3\24\3\24\5\24\u0129\n\24\3\24\2\3\26\25\2\4\6"+
		"\b\n\f\16\20\22\24\26\30\32\34\36 \"$&\2\f\3\2&\'\3\2&(\4\2&&((\3\2\n"+
		"\13\3\2\f\16\4\2\n\n\17\17\3\2\20\21\3\2\22\23\3\2\24\25\3\2\32\35\2\u0145"+
		"\2(\3\2\2\2\4+\3\2\2\2\6-\3\2\2\2\b/\3\2\2\2\n\64\3\2\2\2\f>\3\2\2\2\16"+
		"g\3\2\2\2\20i\3\2\2\2\22\u008b\3\2\2\2\24\u008f\3\2\2\2\26\u00a7\3\2\2"+
		"\2\30\u00cc\3\2\2\2\32\u00ea\3\2\2\2\34\u00f9\3\2\2\2\36\u0111\3\2\2\2"+
		" \u0115\3\2\2\2\"\u0117\3\2\2\2$\u0120\3\2\2\2&\u0128\3\2\2\2()\5\n\6"+
		"\2)*\7\2\2\3*\3\3\2\2\2+,\t\2\2\2,\5\3\2\2\2-.\7\3\2\2.\7\3\2\2\2/\60"+
		"\7\4\2\2\60\t\3\2\2\2\61\63\5\f\7\2\62\61\3\2\2\2\63\66\3\2\2\2\64\62"+
		"\3\2\2\2\64\65\3\2\2\2\65\67\3\2\2\2\66\64\3\2\2\2\67;\5\20\t\28:\5\f"+
		"\7\298\3\2\2\2:=\3\2\2\2;9\3\2\2\2;<\3\2\2\2<\13\3\2\2\2=;\3\2\2\2>?\t"+
		"\3\2\2?@\7)\2\2@Q\7\5\2\2AB\5\4\3\2BI\7)\2\2CD\7\6\2\2DE\5\4\3\2EF\7)"+
		"\2\2FH\3\2\2\2GC\3\2\2\2HK\3\2\2\2IG\3\2\2\2IJ\3\2\2\2JM\3\2\2\2KI\3\2"+
		"\2\2LA\3\2\2\2LM\3\2\2\2MR\3\2\2\2NP\7(\2\2ON\3\2\2\2OP\3\2\2\2PR\3\2"+
		"\2\2QL\3\2\2\2QO\3\2\2\2RS\3\2\2\2ST\7\7\2\2TU\5\6\4\2UV\5\22\n\2VX\5"+
		"\b\5\2WY\7\b\2\2XW\3\2\2\2XY\3\2\2\2Y\r\3\2\2\2Z[\t\4\2\2[\\\7\t\2\2\\"+
		"^\7\5\2\2]_\7(\2\2^]\3\2\2\2^_\3\2\2\2_`\3\2\2\2`h\7\7\2\2ab\7\t\2\2b"+
		"d\7\5\2\2ce\7(\2\2dc\3\2\2\2de\3\2\2\2ef\3\2\2\2fh\7\7\2\2gZ\3\2\2\2g"+
		"a\3\2\2\2h\17\3\2\2\2ij\5\16\b\2jk\5\6\4\2kl\5\22\n\2ln\5\b\5\2mo\7\b"+
		"\2\2nm\3\2\2\2no\3\2\2\2o\21\3\2\2\2pq\5 \21\2qr\7\b\2\2rs\5\22\n\2s\u008c"+
		"\3\2\2\2tu\5\24\13\2uv\7\b\2\2vw\5\22\n\2w\u008c\3\2\2\2xy\5&\24\2yz\7"+
		"\b\2\2z{\5\22\n\2{\u008c\3\2\2\2|}\5\32\16\2}~\5\22\n\2~\u008c\3\2\2\2"+
		"\177\u0080\5\36\20\2\u0080\u0081\5\22\n\2\u0081\u008c\3\2\2\2\u0082\u0083"+
		"\5\6\4\2\u0083\u0084\5\22\n\2\u0084\u0086\5\b\5\2\u0085\u0087\7\b\2\2"+
		"\u0086\u0085\3\2\2\2\u0086\u0087\3\2\2\2\u0087\u0088\3\2\2\2\u0088\u0089"+
		"\5\22\n\2\u0089\u008c\3\2\2\2\u008a\u008c\3\2\2\2\u008bp\3\2\2\2\u008b"+
		"t\3\2\2\2\u008bx\3\2\2\2\u008b|\3\2\2\2\u008b\177\3\2\2\2\u008b\u0082"+
		"\3\2\2\2\u008b\u008a\3\2\2\2\u008c\23\3\2\2\2\u008d\u0090\5\30\r\2\u008e"+
		"\u0090\5\26\f\2\u008f\u008d\3\2\2\2\u008f\u008e\3\2\2\2\u0090\25\3\2\2"+
		"\2\u0091\u0092\b\f\1\2\u0092\u0093\7\5\2\2\u0093\u0094\5\24\13\2\u0094"+
		"\u0095\7\7\2\2\u0095\u00a8\3\2\2\2\u0096\u0097\t\5\2\2\u0097\u00a8\5\26"+
		"\f\16\u0098\u0099\7)\2\2\u0099\u00a2\7\5\2\2\u009a\u009f\5\26\f\2\u009b"+
		"\u009c\7\6\2\2\u009c\u009e\5\26\f\2\u009d\u009b\3\2\2\2\u009e\u00a1\3"+
		"\2\2\2\u009f\u009d\3\2\2\2\u009f\u00a0\3\2\2\2\u00a0\u00a3\3\2\2\2\u00a1"+
		"\u009f\3\2\2\2\u00a2\u009a\3\2\2\2\u00a2\u00a3\3\2\2\2\u00a3\u00a4\3\2"+
		"\2\2\u00a4\u00a8\7\7\2\2\u00a5\u00a8\7#\2\2\u00a6\u00a8\7)\2\2\u00a7\u0091"+
		"\3\2\2\2\u00a7\u0096\3\2\2\2\u00a7\u0098\3\2\2\2\u00a7\u00a5\3\2\2\2\u00a7"+
		"\u00a6\3\2\2\2\u00a8\u00c3\3\2\2\2\u00a9\u00aa\f\r\2\2\u00aa\u00ab\t\6"+
		"\2\2\u00ab\u00c2\5\26\f\16\u00ac\u00ad\f\f\2\2\u00ad\u00ae\t\7\2\2\u00ae"+
		"\u00c2\5\26\f\r\u00af\u00b0\f\13\2\2\u00b0\u00b1\t\b\2\2\u00b1\u00c2\5"+
		"\26\f\f\u00b2\u00b3\f\n\2\2\u00b3\u00b4\t\t\2\2\u00b4\u00c2\5\26\f\13"+
		"\u00b5\u00b6\f\t\2\2\u00b6\u00b7\t\n\2\2\u00b7\u00c2\5\26\f\n\u00b8\u00b9"+
		"\f\b\2\2\u00b9\u00ba\7\26\2\2\u00ba\u00c2\5\26\f\t\u00bb\u00bc\f\7\2\2"+
		"\u00bc\u00bd\7\27\2\2\u00bd\u00c2\5\26\f\b\u00be\u00bf\f\6\2\2\u00bf\u00c0"+
		"\7\30\2\2\u00c0\u00c2\5\26\f\7\u00c1\u00a9\3\2\2\2\u00c1\u00ac\3\2\2\2"+
		"\u00c1\u00af\3\2\2\2\u00c1\u00b2\3\2\2\2\u00c1\u00b5\3\2\2\2\u00c1\u00b8"+
		"\3\2\2\2\u00c1\u00bb\3\2\2\2\u00c1\u00be\3\2\2\2\u00c2\u00c5\3\2\2\2\u00c3"+
		"\u00c1\3\2\2\2\u00c3\u00c4\3\2\2\2\u00c4\27\3\2\2\2\u00c5\u00c3\3\2\2"+
		"\2\u00c6\u00c7\7)\2\2\u00c7\u00c8\7\31\2\2\u00c8\u00cd\5\24\13\2\u00c9"+
		"\u00ca\7)\2\2\u00ca\u00cb\t\13\2\2\u00cb\u00cd\5\26\f\2\u00cc\u00c6\3"+
		"\2\2\2\u00cc\u00c9\3\2\2\2\u00cd\31\3\2\2\2\u00ce\u00cf\7\36\2\2\u00cf"+
		"\u00d0\7\5\2\2\u00d0\u00d1\5\24\13\2\u00d1\u00d2\7\7\2\2\u00d2\u00d3\5"+
		"\6\4\2\u00d3\u00d4\5\22\n\2\u00d4\u00d6\5\b\5\2\u00d5\u00d7\5\34\17\2"+
		"\u00d6\u00d5\3\2\2\2\u00d6\u00d7\3\2\2\2\u00d7\u00eb\3\2\2\2\u00d8\u00d9"+
		"\7\36\2\2\u00d9\u00da\7\5\2\2\u00da\u00db\5\24\13\2\u00db\u00dc\7\7\2"+
		"\2\u00dc\u00dd\5\24\13\2\u00dd\u00df\7\b\2\2\u00de\u00e0\5\34\17\2\u00df"+
		"\u00de\3\2\2\2\u00df\u00e0\3\2\2\2\u00e0\u00eb\3\2\2\2\u00e1\u00e2\7\36"+
		"\2\2\u00e2\u00e3\7\5\2\2\u00e3\u00e4\5\24\13\2\u00e4\u00e5\7\7\2\2\u00e5"+
		"\u00e6\5&\24\2\u00e6\u00e8\7\b\2\2\u00e7\u00e9\5\34\17\2\u00e8\u00e7\3"+
		"\2\2\2\u00e8\u00e9\3\2\2\2\u00e9\u00eb\3\2\2\2\u00ea\u00ce\3\2\2\2\u00ea"+
		"\u00d8\3\2\2\2\u00ea\u00e1\3\2\2\2\u00eb\33\3\2\2\2\u00ec\u00ed\7\37\2"+
		"\2\u00ed\u00ee\5\6\4\2\u00ee\u00ef\5\22\n\2\u00ef\u00f0\5\b\5\2\u00f0"+
		"\u00fa\3\2\2\2\u00f1\u00f2\7\37\2\2\u00f2\u00f3\5\24\13\2\u00f3\u00f4"+
		"\7\b\2\2\u00f4\u00fa\3\2\2\2\u00f5\u00f6\7\37\2\2\u00f6\u00f7\5&\24\2"+
		"\u00f7\u00f8\7\b\2\2\u00f8\u00fa\3\2\2\2\u00f9\u00ec\3\2\2\2\u00f9\u00f1"+
		"\3\2\2\2\u00f9\u00f5\3\2\2\2\u00fa\35\3\2\2\2\u00fb\u00fc\7 \2\2\u00fc"+
		"\u00fd\7\5\2\2\u00fd\u00fe\5\24\13\2\u00fe\u00ff\7\7\2\2\u00ff\u0100\5"+
		"\6\4\2\u0100\u0101\5\22\n\2\u0101\u0102\5\b\5\2\u0102\u0112\3\2\2\2\u0103"+
		"\u0104\7 \2\2\u0104\u0105\7\5\2\2\u0105\u0106\5\24\13\2\u0106\u0107\7"+
		"\7\2\2\u0107\u0108\5\24\13\2\u0108\u0109\7\b\2\2\u0109\u0112\3\2\2\2\u010a"+
		"\u010b\7 \2\2\u010b\u010c\7\5\2\2\u010c\u010d\5\24\13\2\u010d\u010e\7"+
		"\7\2\2\u010e\u010f\5&\24\2\u010f\u0110\7\b\2\2\u0110\u0112\3\2\2\2\u0111"+
		"\u00fb\3\2\2\2\u0111\u0103\3\2\2\2\u0111\u010a\3\2\2\2\u0112\37\3\2\2"+
		"\2\u0113\u0116\5\"\22\2\u0114\u0116\5$\23\2\u0115\u0113\3\2\2\2\u0115"+
		"\u0114\3\2\2\2\u0116!\3\2\2\2\u0117\u0118\5\4\3\2\u0118\u011d\7)\2\2\u0119"+
		"\u011a\7\6\2\2\u011a\u011c\7)\2\2\u011b\u0119\3\2\2\2\u011c\u011f\3\2"+
		"\2\2\u011d\u011b\3\2\2\2\u011d\u011e\3\2\2\2\u011e#\3\2\2\2\u011f\u011d"+
		"\3\2\2\2\u0120\u0121\5\4\3\2\u0121\u0122\7)\2\2\u0122\u0123\7\31\2\2\u0123"+
		"\u0124\5\24\13\2\u0124%\3\2\2\2\u0125\u0126\7\"\2\2\u0126\u0129\5\24\13"+
		"\2\u0127\u0129\7\"\2\2\u0128\u0125\3\2\2\2\u0128\u0127\3\2\2\2\u0129\'"+
		"\3\2\2\2\37\64;ILOQX^dgn\u0086\u008b\u008f\u009f\u00a2\u00a7\u00c1\u00c3"+
		"\u00cc\u00d6\u00df\u00e8\u00ea\u00f9\u0111\u0115\u011d\u0128";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}