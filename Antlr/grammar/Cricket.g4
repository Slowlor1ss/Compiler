//https://riptutorial.com/antlr/
grammar Cricket;

start : prog EOF;

vartype: TInt | TChar ;
beginBlock : '{' ;
endBlock : '}' ;

prog :
	(funcDeclr)* mainDeclr (funcDeclr)*
;

//void https://stackoverflow.com/questions/5075530/use-of-void-in-a-function-parameter
funcDeclr :
	FTYPE=(TVoid|TInt|TChar) Identifier OpenPar ( (vartype Identifier (Comma vartype Identifier)*)? | (TVoid)? ) ClosePar 
    beginBlock 
    body
    endBlock (StatementSeparator)?
;

//# are labels 
//TODO: maybe add -> int main (int argc, char *argv[]);
mainDeclrHeader:
	FTYPE=(TVoid|TInt) 'main' OpenPar (TVoid)? ClosePar 		 		    #mainDeclrHeaderWithRet
	| 'main' OpenPar (TVoid)? ClosePar 									    #mainDeclrHeaderNoRet
;

mainDeclr : 
	mainDeclrHeader beginBlock body endBlock (StatementSeparator)?
;

body : 
	declr StatementSeparator body
	| expr StatementSeparator body
	| flowControl StatementSeparator body
	| ifStatement body
	| whileStatement body
	| beginBlock body endBlock (StatementSeparator)? body
	|
;

expr :
    assign
    | primaryExpr
;

primaryExpr :
	OpenPar expr ClosePar 										            #parExpr
	| UNARY=(Minus|ExclamationMark) primaryExpr 		   					#unaryExpr
	| primaryExpr MULTIPLICATIVE=(Mul|Div|Mod) primaryExpr 					#mulDivModExpr	
	| primaryExpr ADDITIVE=(Plus|Minus) primaryExpr 						#addSubExpr
	| primaryExpr CMP=(LessThan | GreaterThan) primaryExpr			        #cmpLessOrGreaterExpr
	| primaryExpr EQ=(Equal|NotEqual) primaryExpr							#cmpEqualityExpr
	| primaryExpr LGEQ=(LessOrEqual|GreaterOrEqual) primaryExpr				#cmpEqualityLessGreaterExpr
    | primaryExpr BITWISE=(BitwiseAnd|BitwiseOr|BitwiseXor) primaryExpr     #BitwiseExpr
	| Identifier OpenPar (primaryExpr (Comma primaryExpr)*)? ClosePar		#funcExpr
	| CONST=(Number|CharLiteral) 											                    #constExpr 
	| Identifier										                    #varExpr
;

assign :
    Identifier Assign expr									                        #assignExpr
    | Identifier OPPMMD=(PlusEqual|MinusEqual|MulEqual|DivEqual) primaryExpr      	#pmmdEqual
;

ifStatement :
	'if' OpenPar expr ClosePar beginBlock body endBlock (elseStatement)?
	| 'if' OpenPar expr ClosePar expr StatementSeparator (elseStatement)?
	| 'if' OpenPar expr ClosePar flowControl StatementSeparator (elseStatement)?
;

elseStatement :
	'else' beginBlock body endBlock
	|'else' expr StatementSeparator
	|'else' flowControl StatementSeparator
;

whileStatement :
	'while' OpenPar expr ClosePar beginBlock body endBlock
	|'while' OpenPar expr ClosePar expr StatementSeparator
	|'while' OpenPar expr ClosePar flowControl StatementSeparator
;

//ex. int a, b, c; or int a = 5; 
declr :
	varDeclr 
	| varDeclrAndAssign
;
varDeclr : 
	vartype Identifier (Comma Identifier)*
;
varDeclrAndAssign :
	vartype Identifier Assign expr
;

//TODO: maybe add continue
flowControl :
	TReturn expr										#exprReturn
	| TReturn 											#emptyReturn
;


//Any rules that begin with a capital letter (sometimes captilize the entire rule name to make it obvious) is a Lexer rule.

WS : [ \r\t\n] -> skip ;
TReturn : 'return' ;
//Const : Number | CharLiteral ;
Number : Digit+ ; //+ is one or more of whats before the +
//Digit : [0-9]+ ;
CharLiteral : '\'' . '\'' ;
TInt : 'int' ;
TChar : 'char' ;
TVoid: 'void' ;

LessThan: '<' ;
GreaterThan:  '>' ;
BitwiseAnd: '&' ;
BitwiseOr: '|' ;
BitwiseXor: '^' ;

Equal: '==' ;
NotEqual: '!=' ;
LessOrEqual: '<=' ;
GreaterOrEqual: '>=' ;

Assign: '=' ;
Colon: ':' ;
StatementSeparator: ';' ;
Plus: '+' ;
Minus: '-' ;
Mul: '*' ;
Div: '/' ;
Mod: '%' ;

PlusEqual: '+=' ;
MinusEqual: '-=' ;
MulEqual: '*=' ;
DivEqual: '/=' ;

OpenPar: '(' ;
ClosePar: ')' ;
OpenCurly: '{' ;
CloseCurly: '}' ;
ExclamationMark: '!' ;
Comma: ',' ;

//Identifier : [a-zA-Z_][a-zA-Z0-9_]* ;

Identifier
    :   Nondigit
        (   Nondigit
        |   Digit
        )*
    ;
//Fragments are reusable parts of lexer rules which cannot match on their own - they need to be referenced from a lexer rule.
fragment Nondigit
        :   [a-zA-Z_]
        ;
fragment Digit
        :   [0-9]
        ;

Comment
    :   ( '//' ~[\r\n]* '\r'? '\n'
        | '/*' .*? '*/'
        ) -> skip
    ;
IncludeDirective
:   '#' WS? 'include' WS? (('"' ~[\r\n]* '"') | ('<' ~[\r\n]* '>' )) WS? Newline
    -> skip
;
Newline
    :   (   '\r' '\n'?
        |   '\n'
        )
        -> skip
    ;