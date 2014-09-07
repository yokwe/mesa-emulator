#!/bin/awk -f

Begin {
	DISCARD = 0;
	# 0 => OUTSIDE  1 => inside RECORD  2 => in ENUM inside RECORD
	STATE   = 0;
}

function PROCESS_ENUM(name) {
}

function FATAL(message) {
	print message > "/dev/stderr"
	exit
}

# //
$1 == "//" { next }

# empty line
NF == 0 { next }

# skip block comment
substr($1, 1, 2) == "/*" {
	DISCARD = 1
	next
}
substr($1, 1, 2) == "*/" {
 	DISCARD = 0
 	next
}
DISCARD == 1 { next }

# skip preprocessor directive
substr($0, 1, 1) == "#" { next }

# skip typedef
$1 == "typedef" { next }

# skip static const
$1 == "static" && $2 == "const" { next }


# RECORD Ethernet {
NF == 3 && $1 == "RECORD" && $3 == "{" {
	RECORD_NAME = $2
	if (STATE == 0) STATE = 1
	else FATAL("STATE is no 0")

	printf("RECORD %s\n", $0)
	next
}

# ENUM Type : quint16 {
NF == 5 && $1 == "ENUM" && $3 == ":" && $5 == "{" {
	ENUM_NAME = $2
	ENUM_TYPE = $4
	
	if (STATE == 1) STATE = 2
	else FATAL("STATE is not 1")
	
	printf("ENUM %s\n", $0)
	next
}

# CHOICE {
NF == 2 && $1 == "CHOICE" && $2 == "{" {
	if (STATE == 1) STATE = 3
	else FATAL("STATE is not 1")
	next
}

# };
NF == 1 && $1 == "};" {
	if (STATE == 2) {
		STATE = 1
		next
	}
	if (STATE == 3) {
		STATE = 1
		next
	}
	if (STATE == 1) {
		STATE = 0
		next
	}
	printf(sprintf("FNR %d  STATE = %d\n", FNR, STATE))
	next
}

# IDP = 0x0600,
NF == 3 && $2 == "=" && substr($3, length($3)) == "," {
	printf("ENUM MEMBER %s\n", $0)
	next
}

# quint48 dest;
NF == 2 && substr($2, length($2)) == ";" {
	printf("RECORD FIELD %s\n", $0)
	next
}

{
	FATAL(sprintf("UNKNOWN  FNR = %d  NF = %d  \$0 = \"%s\"\n", FNR, NF, $0))
}