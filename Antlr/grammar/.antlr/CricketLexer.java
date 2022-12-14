// Generated from c:\School_3rdyr\Gradwork\Project\Compiler\Antlr\grammar\Cricket.g4 by ANTLR 4.9.2
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class CricketLexer extends Lexer {
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
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"T__0", "T__1", "T__2", "T__3", "WS", "TReturn", "Number", "CharLiteral", 
			"TInt", "TChar", "TVoid", "LessThan", "GreaterThan", "BitwiseAnd", "BitwiseOr", 
			"BitwiseXor", "Equal", "NotEqual", "LessOrEqual", "GreaterOrEqual", "Assign", 
			"Colon", "StatementSeparator", "Plus", "Minus", "Mul", "Div", "Mod", 
			"PlusEqual", "MinusEqual", "MulEqual", "DivEqual", "OpenPar", "ClosePar", 
			"OpenCurly", "CloseCurly", "ExclamationMark", "Comma", "Identifier", 
			"Nondigit", "Digit", "Comment", "IncludeDirective", "Newline"
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


	public CricketLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "Cricket.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2,\u0127\b\1\4\2\t"+
		"\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13"+
		"\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\4&\t&\4\'\t\'\4(\t(\4)\t)\4*\t*\4+\t+\4"+
		",\t,\4-\t-\3\2\3\2\3\2\3\2\3\2\3\3\3\3\3\3\3\4\3\4\3\4\3\4\3\4\3\5\3\5"+
		"\3\5\3\5\3\5\3\5\3\6\3\6\3\6\3\6\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\b\6\b{"+
		"\n\b\r\b\16\b|\3\t\3\t\3\t\3\t\3\n\3\n\3\n\3\n\3\13\3\13\3\13\3\13\3\13"+
		"\3\f\3\f\3\f\3\f\3\f\3\r\3\r\3\16\3\16\3\17\3\17\3\20\3\20\3\21\3\21\3"+
		"\22\3\22\3\22\3\23\3\23\3\23\3\24\3\24\3\24\3\25\3\25\3\25\3\26\3\26\3"+
		"\27\3\27\3\30\3\30\3\31\3\31\3\32\3\32\3\33\3\33\3\34\3\34\3\35\3\35\3"+
		"\36\3\36\3\36\3\37\3\37\3\37\3 \3 \3 \3!\3!\3!\3\"\3\"\3#\3#\3$\3$\3%"+
		"\3%\3&\3&\3\'\3\'\3(\3(\3(\7(\u00d2\n(\f(\16(\u00d5\13(\3)\3)\3*\3*\3"+
		"+\3+\3+\3+\7+\u00df\n+\f+\16+\u00e2\13+\3+\5+\u00e5\n+\3+\3+\3+\3+\3+"+
		"\7+\u00ec\n+\f+\16+\u00ef\13+\3+\3+\5+\u00f3\n+\3+\3+\3,\3,\5,\u00f9\n"+
		",\3,\3,\3,\3,\3,\3,\3,\3,\3,\5,\u0104\n,\3,\3,\7,\u0108\n,\f,\16,\u010b"+
		"\13,\3,\3,\3,\7,\u0110\n,\f,\16,\u0113\13,\3,\5,\u0116\n,\3,\5,\u0119"+
		"\n,\3,\3,\3,\3,\3-\3-\5-\u0121\n-\3-\5-\u0124\n-\3-\3-\3\u00ed\2.\3\3"+
		"\5\4\7\5\t\6\13\7\r\b\17\t\21\n\23\13\25\f\27\r\31\16\33\17\35\20\37\21"+
		"!\22#\23%\24\'\25)\26+\27-\30/\31\61\32\63\33\65\34\67\359\36;\37= ?!"+
		"A\"C#E$G%I&K\'M(O)Q\2S\2U*W+Y,\3\2\6\5\2\13\f\17\17\"\"\5\2C\\aac|\3\2"+
		"\62;\4\2\f\f\17\17\2\u0133\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2"+
		"\2\2\2\13\3\2\2\2\2\r\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2"+
		"\25\3\2\2\2\2\27\3\2\2\2\2\31\3\2\2\2\2\33\3\2\2\2\2\35\3\2\2\2\2\37\3"+
		"\2\2\2\2!\3\2\2\2\2#\3\2\2\2\2%\3\2\2\2\2\'\3\2\2\2\2)\3\2\2\2\2+\3\2"+
		"\2\2\2-\3\2\2\2\2/\3\2\2\2\2\61\3\2\2\2\2\63\3\2\2\2\2\65\3\2\2\2\2\67"+
		"\3\2\2\2\29\3\2\2\2\2;\3\2\2\2\2=\3\2\2\2\2?\3\2\2\2\2A\3\2\2\2\2C\3\2"+
		"\2\2\2E\3\2\2\2\2G\3\2\2\2\2I\3\2\2\2\2K\3\2\2\2\2M\3\2\2\2\2O\3\2\2\2"+
		"\2U\3\2\2\2\2W\3\2\2\2\2Y\3\2\2\2\3[\3\2\2\2\5`\3\2\2\2\7c\3\2\2\2\th"+
		"\3\2\2\2\13n\3\2\2\2\rr\3\2\2\2\17z\3\2\2\2\21~\3\2\2\2\23\u0082\3\2\2"+
		"\2\25\u0086\3\2\2\2\27\u008b\3\2\2\2\31\u0090\3\2\2\2\33\u0092\3\2\2\2"+
		"\35\u0094\3\2\2\2\37\u0096\3\2\2\2!\u0098\3\2\2\2#\u009a\3\2\2\2%\u009d"+
		"\3\2\2\2\'\u00a0\3\2\2\2)\u00a3\3\2\2\2+\u00a6\3\2\2\2-\u00a8\3\2\2\2"+
		"/\u00aa\3\2\2\2\61\u00ac\3\2\2\2\63\u00ae\3\2\2\2\65\u00b0\3\2\2\2\67"+
		"\u00b2\3\2\2\29\u00b4\3\2\2\2;\u00b6\3\2\2\2=\u00b9\3\2\2\2?\u00bc\3\2"+
		"\2\2A\u00bf\3\2\2\2C\u00c2\3\2\2\2E\u00c4\3\2\2\2G\u00c6\3\2\2\2I\u00c8"+
		"\3\2\2\2K\u00ca\3\2\2\2M\u00cc\3\2\2\2O\u00ce\3\2\2\2Q\u00d6\3\2\2\2S"+
		"\u00d8\3\2\2\2U\u00f2\3\2\2\2W\u00f6\3\2\2\2Y\u0123\3\2\2\2[\\\7o\2\2"+
		"\\]\7c\2\2]^\7k\2\2^_\7p\2\2_\4\3\2\2\2`a\7k\2\2ab\7h\2\2b\6\3\2\2\2c"+
		"d\7g\2\2de\7n\2\2ef\7u\2\2fg\7g\2\2g\b\3\2\2\2hi\7y\2\2ij\7j\2\2jk\7k"+
		"\2\2kl\7n\2\2lm\7g\2\2m\n\3\2\2\2no\t\2\2\2op\3\2\2\2pq\b\6\2\2q\f\3\2"+
		"\2\2rs\7t\2\2st\7g\2\2tu\7v\2\2uv\7w\2\2vw\7t\2\2wx\7p\2\2x\16\3\2\2\2"+
		"y{\5S*\2zy\3\2\2\2{|\3\2\2\2|z\3\2\2\2|}\3\2\2\2}\20\3\2\2\2~\177\7)\2"+
		"\2\177\u0080\13\2\2\2\u0080\u0081\7)\2\2\u0081\22\3\2\2\2\u0082\u0083"+
		"\7k\2\2\u0083\u0084\7p\2\2\u0084\u0085\7v\2\2\u0085\24\3\2\2\2\u0086\u0087"+
		"\7e\2\2\u0087\u0088\7j\2\2\u0088\u0089\7c\2\2\u0089\u008a\7t\2\2\u008a"+
		"\26\3\2\2\2\u008b\u008c\7x\2\2\u008c\u008d\7q\2\2\u008d\u008e\7k\2\2\u008e"+
		"\u008f\7f\2\2\u008f\30\3\2\2\2\u0090\u0091\7>\2\2\u0091\32\3\2\2\2\u0092"+
		"\u0093\7@\2\2\u0093\34\3\2\2\2\u0094\u0095\7(\2\2\u0095\36\3\2\2\2\u0096"+
		"\u0097\7~\2\2\u0097 \3\2\2\2\u0098\u0099\7`\2\2\u0099\"\3\2\2\2\u009a"+
		"\u009b\7?\2\2\u009b\u009c\7?\2\2\u009c$\3\2\2\2\u009d\u009e\7#\2\2\u009e"+
		"\u009f\7?\2\2\u009f&\3\2\2\2\u00a0\u00a1\7>\2\2\u00a1\u00a2\7?\2\2\u00a2"+
		"(\3\2\2\2\u00a3\u00a4\7@\2\2\u00a4\u00a5\7?\2\2\u00a5*\3\2\2\2\u00a6\u00a7"+
		"\7?\2\2\u00a7,\3\2\2\2\u00a8\u00a9\7<\2\2\u00a9.\3\2\2\2\u00aa\u00ab\7"+
		"=\2\2\u00ab\60\3\2\2\2\u00ac\u00ad\7-\2\2\u00ad\62\3\2\2\2\u00ae\u00af"+
		"\7/\2\2\u00af\64\3\2\2\2\u00b0\u00b1\7,\2\2\u00b1\66\3\2\2\2\u00b2\u00b3"+
		"\7\61\2\2\u00b38\3\2\2\2\u00b4\u00b5\7\'\2\2\u00b5:\3\2\2\2\u00b6\u00b7"+
		"\7-\2\2\u00b7\u00b8\7?\2\2\u00b8<\3\2\2\2\u00b9\u00ba\7/\2\2\u00ba\u00bb"+
		"\7?\2\2\u00bb>\3\2\2\2\u00bc\u00bd\7,\2\2\u00bd\u00be\7?\2\2\u00be@\3"+
		"\2\2\2\u00bf\u00c0\7\61\2\2\u00c0\u00c1\7?\2\2\u00c1B\3\2\2\2\u00c2\u00c3"+
		"\7*\2\2\u00c3D\3\2\2\2\u00c4\u00c5\7+\2\2\u00c5F\3\2\2\2\u00c6\u00c7\7"+
		"}\2\2\u00c7H\3\2\2\2\u00c8\u00c9\7\177\2\2\u00c9J\3\2\2\2\u00ca\u00cb"+
		"\7#\2\2\u00cbL\3\2\2\2\u00cc\u00cd\7.\2\2\u00cdN\3\2\2\2\u00ce\u00d3\5"+
		"Q)\2\u00cf\u00d2\5Q)\2\u00d0\u00d2\5S*\2\u00d1\u00cf\3\2\2\2\u00d1\u00d0"+
		"\3\2\2\2\u00d2\u00d5\3\2\2\2\u00d3\u00d1\3\2\2\2\u00d3\u00d4\3\2\2\2\u00d4"+
		"P\3\2\2\2\u00d5\u00d3\3\2\2\2\u00d6\u00d7\t\3\2\2\u00d7R\3\2\2\2\u00d8"+
		"\u00d9\t\4\2\2\u00d9T\3\2\2\2\u00da\u00db\7\61\2\2\u00db\u00dc\7\61\2"+
		"\2\u00dc\u00e0\3\2\2\2\u00dd\u00df\n\5\2\2\u00de\u00dd\3\2\2\2\u00df\u00e2"+
		"\3\2\2\2\u00e0\u00de\3\2\2\2\u00e0\u00e1\3\2\2\2\u00e1\u00e4\3\2\2\2\u00e2"+
		"\u00e0\3\2\2\2\u00e3\u00e5\7\17\2\2\u00e4\u00e3\3\2\2\2\u00e4\u00e5\3"+
		"\2\2\2\u00e5\u00e6\3\2\2\2\u00e6\u00f3\7\f\2\2\u00e7\u00e8\7\61\2\2\u00e8"+
		"\u00e9\7,\2\2\u00e9\u00ed\3\2\2\2\u00ea\u00ec\13\2\2\2\u00eb\u00ea\3\2"+
		"\2\2\u00ec\u00ef\3\2\2\2\u00ed\u00ee\3\2\2\2\u00ed\u00eb\3\2\2\2\u00ee"+
		"\u00f0\3\2\2\2\u00ef\u00ed\3\2\2\2\u00f0\u00f1\7,\2\2\u00f1\u00f3\7\61"+
		"\2\2\u00f2\u00da\3\2\2\2\u00f2\u00e7\3\2\2\2\u00f3\u00f4\3\2\2\2\u00f4"+
		"\u00f5\b+\2\2\u00f5V\3\2\2\2\u00f6\u00f8\7%\2\2\u00f7\u00f9\5\13\6\2\u00f8"+
		"\u00f7\3\2\2\2\u00f8\u00f9\3\2\2\2\u00f9\u00fa\3\2\2\2\u00fa\u00fb\7k"+
		"\2\2\u00fb\u00fc\7p\2\2\u00fc\u00fd\7e\2\2\u00fd\u00fe\7n\2\2\u00fe\u00ff"+
		"\7w\2\2\u00ff\u0100\7f\2\2\u0100\u0101\7g\2\2\u0101\u0103\3\2\2\2\u0102"+
		"\u0104\5\13\6\2\u0103\u0102\3\2\2\2\u0103\u0104\3\2\2\2\u0104\u0115\3"+
		"\2\2\2\u0105\u0109\7$\2\2\u0106\u0108\n\5\2\2\u0107\u0106\3\2\2\2\u0108"+
		"\u010b\3\2\2\2\u0109\u0107\3\2\2\2\u0109\u010a\3\2\2\2\u010a\u010c\3\2"+
		"\2\2\u010b\u0109\3\2\2\2\u010c\u0116\7$\2\2\u010d\u0111\7>\2\2\u010e\u0110"+
		"\n\5\2\2\u010f\u010e\3\2\2\2\u0110\u0113\3\2\2\2\u0111\u010f\3\2\2\2\u0111"+
		"\u0112\3\2\2\2\u0112\u0114\3\2\2\2\u0113\u0111\3\2\2\2\u0114\u0116\7@"+
		"\2\2\u0115\u0105\3\2\2\2\u0115\u010d\3\2\2\2\u0116\u0118\3\2\2\2\u0117"+
		"\u0119\5\13\6\2\u0118\u0117\3\2\2\2\u0118\u0119\3\2\2\2\u0119\u011a\3"+
		"\2\2\2\u011a\u011b\5Y-\2\u011b\u011c\3\2\2\2\u011c\u011d\b,\2\2\u011d"+
		"X\3\2\2\2\u011e\u0120\7\17\2\2\u011f\u0121\7\f\2\2\u0120\u011f\3\2\2\2"+
		"\u0120\u0121\3\2\2\2\u0121\u0124\3\2\2\2\u0122\u0124\7\f\2\2\u0123\u011e"+
		"\3\2\2\2\u0123\u0122\3\2\2\2\u0124\u0125\3\2\2\2\u0125\u0126\b-\2\2\u0126"+
		"Z\3\2\2\2\22\2|\u00d1\u00d3\u00e0\u00e4\u00ed\u00f2\u00f8\u0103\u0109"+
		"\u0111\u0115\u0118\u0120\u0123\3\b\2\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}