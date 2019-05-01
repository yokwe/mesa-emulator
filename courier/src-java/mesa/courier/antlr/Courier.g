grammar Courier;

ARRAY        : 'ARRAY';
BEGIN        : 'BEGIN';
BOOLEAN      : 'BOOLEAN';
BYTE         : 'BYTE';
CARDINAL     : 'CARDINAL';
CHOICE       : 'CHOICE';
DEPENDS      : 'DEPENDS';
END          : 'END';
ERROR        : 'ERROR';
INTEGER      : 'INTEGER';
LONG         : 'LONG';
OF           : 'OF';
PROCEDURE    : 'PROCEDURE';
PROGRAM      : 'PROGRAM';
RECORD       : 'RECORD';
REPORTS      : 'REPORTS';
RETURNS      : 'RETURNS';
SEQUENCE     : 'SEQUENCE';
STRING       : 'STRING';
TYPE         : 'TYPE';
UNSPECIFIED  : 'UNSPECIFIED';
UNSPECIFIED2 : 'UNSPECIFIED2';
UNSPECIFIED3 : 'UNSPECIFIED3';
UPON         : 'UPON';
VERSION      : 'VERSION';
TRUE         : 'TRUE';
FALSE        : 'FALSE';


fragment CHAR_ALPHA : [A-Z|a-z];
fragment CHAR_AF    : [A-F|a-f];
fragment CHAR_DEC   : [0-9];
fragment CHAR_OCT   : [0-7];

ID  : CHAR_ALPHA (CHAR_ALPHA | CHAR_DEC | '_')*;

NUMBER
    : CHAR_DEC+
    | CHAR_DEC+[Dd]
    | CHAR_OCT+[Bb]
    | CHAR_DEC(CHAR_DEC|CHAR_AF)*[Xx]
    ;

STR : '"' ('""' | ~('"'))* '"';

COMMENT_PARTIAL
    : '--' ~[\n\r]*? '--'      ->skip;

COMMENT_LINE
    : '--' ~[\n\r]* '\r'? '\n' ->skip;

SPACE
    : [ \r\t\n]                ->skip;


courierProgram
    :    programHeader '=' programBody '.'
    ;

programHeader
    :    name=ID ':' PROGRAM program=NUMBER VERSION version=NUMBER
    ;

programBody
    :    BEGIN dependencyList declarationList END
    ;

dependencyList
    :    /* empty */
    |    DEPENDS UPON referencedProgramList ';'
    ;

referencedProgramList
    :    elements+=referencedProgram ( ',' elements+=referencedProgram )*
    ;

referencedProgram
    :    program=ID '(' number=NUMBER ')' VERSION version=NUMBER
    ;

declarationList
    :    /* empty */
    |    elements+=declaration+
    ;

declaration
    :    name=ID ':' TYPE '=' type ';'     # DeclType
    |    name=ID ':' type '=' constant ';' # DeclConst
    ;

type
    :    predefinedType
    |    constructedType
    |    referencedType
    ;

predefinedType
    :    BOOLEAN        # TypeBoolean
    |    BYTE           # TypeByte
    |    CARDINAL       # TypeCardinal
    |    LONG CARDINAL  # TypeLongCardinal
/*  |    INTEGER        # TypeInteger     */
/*  |    LONG INTEGER   # TypeLongInteger */
    |    STRING         # TypeString
    |    UNSPECIFIED    # TypeUnspecified
    |    UNSPECIFIED2   # TypeUnspecified2
    |    UNSPECIFIED3   # TypeUnspecified3
    ;

constructedType
    :    '{' correspondenceList '}'                          # TypeEmptyEnum
    |    '{' correspondenceList '}' OF enumType              # TypeEnum
    |    ARRAY numericValue OF type                          # TypeArray
    |    SEQUENCE maximumNumber OF type                      # TypeSequence
    |    RECORD '[' fieldList ']'                            # TypeRecord
    |    RECORD '[' ']'                                      # TypeEmptyRecord
    |    CHOICE referencedType OF '{' typedCandidateList '}' # TypeChoiceTyped
    |    CHOICE OF '{' anonCandidateList '}'                 # TypeChoiceAnon
    |    PROCEDURE argumentList resultList errorList         # TypeProcedure
    |    ERROR argumentList                                  # TypeError
    ;

enumType
    :    BYTE
    |    UNSPECIFIED
    |    UNSPECIFIED2
    |    UNSPECIFIED3
    ;
referencedType
    :    name=ID                # TypeRef
    |    program=ID '.' name=ID # TypeRefQ
    ;

correspondenceList
    :    elements+=correspondence (',' elements+=correspondence)*
    ;

correspondence
    :    ID '(' numericValue ')'
    ;

maximumNumber
    :    /* empty */
    |    numericValue
    ;

numericValue
    :    NUMBER             # ValueNumber
    |    referencedConstant # ValueRefConsts
    ;

typedCandidateList
    :    elements+=typedCandidate (',' elements+=typedCandidate)*
    ;

typedCandidate
    :    nameList '=>' type
    ;

anonCandidateList
    :    elements+=anonCandidate (',' elements+=anonCandidate)*
    ;

anonCandidate
    :    correspondenceList '=>' type
    ;

argumentList
    :    /* empty */
    |    '[' fieldList ']'
    ;

resultList
    :    /* empty */
    |    RETURNS '[' fieldList ']'
    ;

errorList
    :    /* empty */
    |    REPORTS '[' nameList ']'
    ;

fieldList
    :    elements+=field (',' elements+=field)*
    ;

field
    :    nameList ':' type
    ;

constant
    :    predefinedConstant
    |    constructedConstant
    |    referencedConstant
    ;

predefinedConstant
    :    TRUE                      # ConstTrue
    |    FALSE                     # ConstFalse
    |    NUMBER                    # ConstNumber
    |    '-' NUMBER                # ConstNumberNegative
    |    STR                       # ConstString
    ;

constructedConstant
    :    '[' elementList ']'   # ConstArray
    |    '[' componentList ']' # ConstRecord
    |    '[' ']'               # ConstEmpty
    |    ID constant           # ConstChoice
    ;

referencedConstant
    :    name=ID                # ConstRef
    |    program=ID '.' name=ID # ConstRefQ
    ;

elementList
    :    elements+=constant (',' elements+=constant)*
    ;

componentList
    :    elements+=component (',' elements+=component)*
    ;

component
    :    nameList ':' constant
    ;

nameList
    :    elements+=ID (',' elements+=ID)*
    ;