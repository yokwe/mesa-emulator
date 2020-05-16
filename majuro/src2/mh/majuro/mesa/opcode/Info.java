package mh.majuro.mesa.opcode;

public enum Info {
	NOOP(Type.MOP, 00, "NOOP"),
	LL0 (Type.MOP, 01, "LL0"),
	LL1 (Type.MOP, 02, "LL1"),
	LL2 (Type.MOP, 03, "LL2"),
	LL3 (Type.MOP, 04, "LL3"),
	LL4 (Type.MOP, 05, "LL4"),
	LL5 (Type.MOP, 06, "LL5"),
	LL6 (Type.MOP, 07, "LL6"),

	LL7 (Type.MOP, 010, "LL7"),
	LL8 (Type.MOP, 011, "LL8"),
	LL9 (Type.MOP, 012, "LL9"),
	LL10(Type.MOP, 013, "LL10"),
	LL11(Type.MOP, 014, "LL11"),
	LLB (Type.MOP, 015, "LLB"),
	LLD0(Type.MOP, 016, "LLD0"),
	LLD1(Type.MOP, 017, "LLD1"),

	LLD2 (Type.MOP, 020, "LLD2"),
	LLD3 (Type.MOP, 021, "LLD3"),
	LLD4 (Type.MOP, 022, "LLD4"),
	LLD5 (Type.MOP, 023, "LLD5"),
	LLD6 (Type.MOP, 024, "LLD6"),
	LLD7 (Type.MOP, 025, "LLD7"),
	LLD8 (Type.MOP, 026, "LLD8"),
	LLD10(Type.MOP, 027, "LLD10"),

	LLDB(Type.MOP, 030, "LLDB"),
	SL0 (Type.MOP, 031, "SL0"),
	SL1 (Type.MOP, 032, "SL1"),
	SL2 (Type.MOP, 033, "SL2"),
	SL3 (Type.MOP, 034, "SL3"),
	SL4 (Type.MOP, 035, "SL4"),
	SL5 (Type.MOP, 036, "SL5"),
	SL6 (Type.MOP, 037, "SL6"),

	SL7 (Type.MOP, 040, "SL7"),
	SL8 (Type.MOP, 041, "SL8"),
	SL9 (Type.MOP, 042, "SL9"),
	SL10(Type.MOP, 043, "SL10"),
	SLB (Type.MOP, 044, "SLB"),
	SLD0(Type.MOP, 045, "SLD0"),
	SLD1(Type.MOP, 046, "SLD1"),
	SLD2(Type.MOP, 047, "SLD2"),

	SLD3(Type.MOP, 050, "SLD3"),
	SLD4(Type.MOP, 051, "SLD4"),
	SLD5(Type.MOP, 052, "SLD5"),
	SLD6(Type.MOP, 053, "SLD6"),
	SLD8(Type.MOP, 054, "SLD8"),
	PL0 (Type.MOP, 055, "PL0"),
	PL1 (Type.MOP, 056, "PL1"),
	PL2 (Type.MOP, 057, "PL2"),

	PL3 (Type.MOP, 060, "PL3"),
	PLB (Type.MOP, 061, "PLB"),
	PLD0(Type.MOP, 062, "PLD0"),
	PLDB(Type.MOP, 063, "PLDB"),
	LG0 (Type.MOP, 064, "LG0"),
	LG1 (Type.MOP, 065, "LG1"),
	LG2 (Type.MOP, 066, "LG2"),
	LGB (Type.MOP, 067, "LGB"),

	LGD0 (Type.MOP, 070, "LGD0"),
	LGD2 (Type.MOP, 071, "LGD2"),
	LGDB (Type.MOP, 072, "LGDB"),
	SGB  (Type.MOP, 073, "SGB"),
	BNDCK(Type.MOP, 074, "BNDCK"),
	BRK  (Type.MOP, 075, "BRK"),
	//
	//

	R0  (Type.MOP, 0100, "R0"),
	R1  (Type.MOP, 0101, "R1"),
	RB  (Type.MOP, 0102, "RB"),
	RL0 (Type.MOP, 0103, "RL0"),
	RLB (Type.MOP, 0104, "RLB"),
	RD0 (Type.MOP, 0105, "RD0"),
	RDB (Type.MOP, 0106, "RDB"),
	RDL0(Type.MOP, 0107, "RDL0"),

	RDLB(Type.MOP, 0110, "RDLB"),
	W0  (Type.MOP, 0111, "W0"),
	WB  (Type.MOP, 0112, "WB"),
	PSB (Type.MOP, 0113, "PSB"),
	WLB (Type.MOP, 0114, "WLB"),
	PSLB(Type.MOP, 0115, "PSLB"),
	WDB (Type.MOP, 0116, "WDB"),
	PSD0(Type.MOP, 0117, "PSD0"),

	PSDB (Type.MOP, 0120, "PSDB"),
	WDLB (Type.MOP, 0121, "WDLB"),
	PSDLB(Type.MOP, 0122, "PSDLB"),
	RLI00(Type.MOP, 0123, "RLI00"),
	RLI01(Type.MOP, 0124, "RLI01"),
	RLI02(Type.MOP, 0125, "RLI02"),
	RLI03(Type.MOP, 0126, "RLI03"),
	RLIP (Type.MOP, 0127, "RLIP"),

	RLILP (Type.MOP, 0130, "RLILP"),
	RLDI00(Type.MOP, 0131, "RLDI00"),
	RLDIP (Type.MOP, 0132, "RLDIP"),
	RLDILP(Type.MOP, 0133, "RLDILP"),
	RGIP  (Type.MOP, 0134, "RGIP"),
	RGILP (Type.MOP, 0135, "RGILP"),
	WLIP  (Type.MOP, 0136, "WLIP"),
	WLILP (Type.MOP, 0137, "WLILP"),

	WLDILP(Type.MOP, 0140, "WLDILP"),
	RS    (Type.MOP, 0141, "RS"),
	RLS   (Type.MOP, 0142, "RLS"),
	WS    (Type.MOP, 0143, "WS"),
	WLS   (Type.MOP, 0144, "WLS"),
	R0F   (Type.MOP, 0145, "R0F"),
	RF    (Type.MOP, 0146, "RF"),
	RL0F  (Type.MOP, 0147, "RL0F"),

	RLF   (Type.MOP, 0150, "RLF"),
	RLFS  (Type.MOP, 0151, "RLFS"),
	RLIPF (Type.MOP, 0152, "RLIPF"),
	RLILPF(Type.MOP, 0153, "RLILPF"),
	W0F   (Type.MOP, 0154, "W0F"),
	WF    (Type.MOP, 0155, "WF"),
	PSF   (Type.MOP, 0156, "PSF"),
	PS0F  (Type.MOP, 0157, "PS0F"),

	WS0F(Type.MOP, 0160, "WS0F"),
	WL0F(Type.MOP, 0161, "WL0F"),
	WLF (Type.MOP, 0162, "WLF"),
	PSLF(Type.MOP, 0163, "PSLF"),
	WLFS(Type.MOP, 0164, "WLFS"),
	SLDB(Type.MOP, 0165, "SLDB"),
	SGDB(Type.MOP, 0166, "SGDB"),
	LLKB(Type.MOP, 0167, "LLKB"),

	RKIB   (Type.MOP, 0170, "RKIB"),
	RKDIB  (Type.MOP, 0171, "RKDIB"),
	LKB    (Type.MOP, 0172, "LKB"),
	SHIFT  (Type.MOP, 0173, "SHIFT"),
	SHIFTSB(Type.MOP, 0174, "SHIFTSB"),
	//
	//
	//


	CATCH(Type.MOP, 0200, "CATCH"),
	J2   (Type.MOP, 0201, "J2"),
	J3   (Type.MOP, 0202, "J3"),
	J4   (Type.MOP, 0203, "J4"),
	J5   (Type.MOP, 0204, "J5"),
	J6   (Type.MOP, 0205, "J6"),
	J7   (Type.MOP, 0206, "J7"),
	J8   (Type.MOP, 0207, "J8"),

	JB   (Type.MOP, 0210, "JB"),
	JW   (Type.MOP, 0211, "JW"),
	JEP  (Type.MOP, 0212, "JEP"),
	JEB  (Type.MOP, 0213, "JEB"),
	JEBB (Type.MOP, 0214, "JEBB"),
	JNEP (Type.MOP, 0215, "JNEP"),
	JNEB (Type.MOP, 0216, "JNEB"),
	JNEBB(Type.MOP, 0217, "JNEBB"),

	JLB  (Type.MOP, 0220, "JLB"),
	JGEB (Type.MOP, 0221, "JGEB"),
	JGB  (Type.MOP, 0222, "JGB"),
	JLEB (Type.MOP, 0223, "JLEB"),
	JULB (Type.MOP, 0224, "JULB"),
	JUGEB(Type.MOP, 0225, "JUGEB"),
	JUGB (Type.MOP, 0226, "JUGB"),
	JULEB(Type.MOP, 0227, "JULEB"),

	JZ3  (Type.MOP, 0230, "JZ3"),
	JZ4  (Type.MOP, 0231, "JZ4"),
	JZB  (Type.MOP, 0232, "JZB"),
	JNZ3 (Type.MOP, 0233, "JNZ3"),
	JNZ4 (Type.MOP, 0234, "JNZ4"),
	JNZB (Type.MOP, 0235, "JNZB"),
	JDEB (Type.MOP, 0236, "JDEB"),
	JDNEB(Type.MOP, 0237, "JDNEB"),

	JIB  (Type.MOP, 0240, "JIB"),
	JIW  (Type.MOP, 0241, "JIW"),
	REC  (Type.MOP, 0242, "REC"),
	REC2 (Type.MOP, 0243, "REC2"),
	DIS  (Type.MOP, 0244, "DIS"),
	DIS2 (Type.MOP, 0245, "DIS2"),
	EXCH (Type.MOP, 0246, "EXCH"),
	DEXCH(Type.MOP, 0247, "DEXCH"),

	DUP  (Type.MOP, 0250, "DUP"),
	DDUP (Type.MOP, 0251, "DDUP"),
	EXDIS(Type.MOP, 0252, "EXDIS"),
	NEG  (Type.MOP, 0253, "NEG"),
	INC  (Type.MOP, 0254, "INC"),
	DEC  (Type.MOP, 0255, "DEC"),
	DINC (Type.MOP, 0256, "DINC"),
	DBL  (Type.MOP, 0257, "DBL"),

	DDBL (Type.MOP, 0260, "DDBL"),
	TRPL (Type.MOP, 0261, "TRPL"),
	AND  (Type.MOP, 0262, "AND"),
	IOR  (Type.MOP, 0263, "IOR"),
	ADDSB(Type.MOP, 0264, "ADDSB"),
	ADD  (Type.MOP, 0265, "ADD"),
	SUB  (Type.MOP, 0266, "SUB"),
	DADD (Type.MOP, 0267, "DADD"),

	DSUB  (Type.MOP, 0270, "DSUB"),
	ADC   (Type.MOP, 0271, "ADC"),
	ACD   (Type.MOP, 0272, "ACD"),
	AL0IB (Type.MOP, 0273, "AL0IB"),
	MUL   (Type.MOP, 0274, "MUL"),
	DCMP  (Type.MOP, 0275, "DCMP"),
	UDCMP (Type.MOP, 0276, "UDCMP"),
	VMFIND(Type.MOP, 0277, "VMFIND"),

	
	LI0(Type.MOP, 0300, "LI0"),
	LI1(Type.MOP, 0301, "LI1"),
	LI2(Type.MOP, 0302, "LI2"),
	LI3(Type.MOP, 0303, "LI3"),
	LI4(Type.MOP, 0304, "LI4"),
	LI5(Type.MOP, 0305, "LI5"),
	LI6(Type.MOP, 0306, "LI6"),
	LI7(Type.MOP, 0307, "LI7"),

	LI8 (Type.MOP, 0310, "LI8"),
	LI9 (Type.MOP, 0311, "LI9"),
	LI10(Type.MOP, 0312, "LI10"),
	LIN1(Type.MOP, 0313, "LIN1"),
	LINI(Type.MOP, 0314, "LINI"),
	LIB (Type.MOP, 0315, "LIB"),
	LIW (Type.MOP, 0316, "LIW"),
	LINB(Type.MOP, 0317, "LINB"),

	LIHB(Type.MOP, 0320, "LIHB"),
	LID0(Type.MOP, 0321, "LID0"),
	LA0 (Type.MOP, 0322, "LA0"),
	LA1 (Type.MOP, 0323, "LA1"),
	LA2 (Type.MOP, 0324, "LA2"),
	LA3 (Type.MOP, 0325, "LA3"),
	LA6 (Type.MOP, 0326, "LA6"),
	LA8 (Type.MOP, 0327, "LA8"),

	LAB (Type.MOP, 0330, "LAB"),
	LAW (Type.MOP, 0331, "LAW"),
	GA0 (Type.MOP, 0332, "GA0"),
	GA1 (Type.MOP, 0333, "GA1"),
	GAB (Type.MOP, 0334, "GAB"),
	GAW (Type.MOP, 0335, "GAW"),
	//
	EFC0(Type.MOP, 0337, "EFC0"),

	EFC1(Type.MOP, 0340, "EFC1"),
	EFC2(Type.MOP, 0341, "EFC2"),
	EFC3(Type.MOP, 0342, "EFC3"),
	EFC4(Type.MOP, 0343, "EFC4"),
	EFC5(Type.MOP, 0344, "EFC5"),
	EFC6(Type.MOP, 0345, "EFC6"),
	EFC7(Type.MOP, 0346, "EFC7"),
	EFC8(Type.MOP, 0347, "EFC8"),

	EFC9 (Type.MOP, 0350, "EFC9"),
	EFC10(Type.MOP, 0351, "EFC10"),
	EFC11(Type.MOP, 0352, "EFC11"),
	EFC12(Type.MOP, 0353, "EFC12"),
	EFCB (Type.MOP, 0354, "EFCB"),
	LFC  (Type.MOP, 0355, "LFC"),
	SFC  (Type.MOP, 0356, "SFC"),
	RET  (Type.MOP, 0357, "RET"),

	KFCB (Type.MOP, 0360, "KFCB"),
	ME   (Type.MOP, 0361, "ME"),
	MX   (Type.MOP, 0362, "MX"),
	BLT  (Type.MOP, 0363, "BLT"),
	BLTL (Type.MOP, 0364, "BLTL"),
	BLTC (Type.MOP, 0365, "BLTC"),
	BLTCL(Type.MOP, 0366, "BLTCL"),
	LP   (Type.MOP, 0367, "LP"),

	ESC   (Type.MOP, 0370, "ESC"),
	ESCL  (Type.MOP, 0371, "ESCL"),
	LGA0  (Type.MOP, 0372, "LGA0"),
	LGAB  (Type.MOP, 0373, "LGAB"),
	LGAW  (Type.MOP, 0374, "LGAW"),
	DESC  (Type.MOP, 0375, "DESC"),
	//
	RESRVD(Type.MOP, 0377, "RESRVD"),

	//
	MW (Type.ESC, 02, "MW"),
	MR (Type.ESC, 03, "MR"),
	NC (Type.ESC, 04, "NC"),
	BC (Type.ESC, 05, "BC"),
	REQ(Type.ESC, 06, "REQ"),
	SM (Type.ESC, 07, "SM"),

	SMF(Type.ESC, 010, "SMF"),
	GMF(Type.ESC, 011, "GMF"),
	AF (Type.ESC, 012, "AF"),
	FF (Type.ESC, 013, "FF"),
	PI (Type.ESC, 014, "PI"),
	PO (Type.ESC, 015, "PO"),
	POR(Type.ESC, 016, "POR"),
	SPP(Type.ESC, 017, "SPP"),

	DI    (Type.ESC, 020, "DI"),
	EI    (Type.ESC, 021, "EI"),
	XOR   (Type.ESC, 022, "XOR"),
	DAND  (Type.ESC, 023, "DAND"),
	DIOR  (Type.ESC, 024, "DIOR"),
	DXOR  (Type.ESC, 025, "DXOR"),
	ROTATE(Type.ESC, 026, "ROTATE"),
	DSHIFT(Type.ESC, 027, "DSHIFT"),
	
	LINT (Type.ESC,  030, "LINT"),
	JS   (Type.ESC,  031, "JS"),
	RCFS (Type.ESC,  032, "RCFS"),
	RC   (Type.ESCL, 033, "RC"),
	UDIV (Type.ESC,  034, "UDIV"),
	LUDIV(Type.ESC,  035, "LUDIV"),
	ROB  (Type.ESCL, 036, "ROB"),
	WOB  (Type.ESCL, 037, "WOB"),

	DSK   (Type.ESCL, 040, "DSK"),
	XE    (Type.ESCL, 041, "XE"),
	XF    (Type.ESCL, 042, "XF"),
	LSK   (Type.ESCL, 043, "LSK"),
	BNDCKL(Type.ESC,  044, "BNDCKL"),
	NILCK (Type.ESC,  045, "NILCK"),
	NILCKL(Type.ESC,  046, "NILCKL"),
	BLTLR (Type.ESC,  047, "BLTLR"),
	
	BLEL   (Type.ESC, 050, "BLEL"),
	BLECL  (Type.ESC, 051, "BLECL"),
	CKSUM  (Type.ESC, 052, "CKSUM"),
	BITBLT (Type.ESC, 053, "BITBLT"),
	TXTBLT (Type.ESC, 054, "TXTBLT"),
	BYTBLT (Type.ESC, 055, "BYTBLT"),
	BYTBLTR(Type.ESC, 056, "BYTBLTR"),
	VERSION(Type.ESC, 057, "VERSION"),

	DMUL (Type.ESC, 060, "DMUL"),
	SDIV (Type.ESC, 061, "SDIV"),
	SDDIV(Type.ESC, 062, "SDDIV"),
	UDDIV(Type.ESC, 063, "UDDIV"),

	// Floating Point (100B-137B are reserved)
//	FADD (Type.ESC, 0100, "FADD"),
//	FSUB (Type.ESC, 0101, "FSUB"),
//	FMUL (Type.ESC, 0102, "FMUL"),
//	FDIV (Type.ESC, 0103, "FDIV"),
//	FCOMP(Type.ESC, 0104, "FCOMP"),
//	FIX  (Type.ESC, 0105, "FIX"),
//	FLOAT(Type.ESC, 0106, "FLOAT"),
//	FIXI (Type.ESC, 0107, "FIXI"),
//	
//	FIXC   (Type.ESC, 0110, "FIXC"),
//	FSTICKY(Type.ESC, 0111, "FSTICKY"),
//	FREM   (Type.ESC, 0112, "FREM"),
//	ROUND  (Type.ESC, 0113, "ROUND"),
//	ROUNDI (Type.ESC, 0114, "ROUNDI"),
//	ROUNDC (Type.ESC, 0115, "ROUNDC"),
//	FSQRT  (Type.ESC, 0116, "FSQRT"),
	//

	//  Read / Write Registers
	WRPSB(Type.ESC, 0160, "WRPSB"),
	WRMDS(Type.ESC, 0161, "WRMDS"),
	WRWP (Type.ESC, 0162, "WRWP"),
	WRWDC(Type.ESC, 0163, "WRWDC"),
	WRPTC(Type.ESC, 0164, "WRPTC"),
	WRIT (Type.ESC, 0165, "WRIT"),
	WRXTS(Type.ESC, 0166, "WRXTS"),
	WRMP (Type.ESC, 0167, "WRMP"),
	
	RRPSB(Type.ESC, 0170, "RRPSB"),
	RRMDS(Type.ESC, 0171, "RRMDS"),
	RRWP (Type.ESC, 0172, "RRWP"),
	RRWDC(Type.ESC, 0173, "RRWDC"),
	RRPTC(Type.ESC, 0174, "RRPTC"),
	RRIT (Type.ESC, 0175, "RRIT"),
	RRXTS(Type.ESC, 0176, "RRXTS"),
	//

	
	// Guam
	//   GuamInputOutput.mesa
	CALLAGENT   (Type.ESC, 0211, "CALLAGENT"),
	MAPDISPLAY  (Type.ESC, 0212, "MAPDISPLAY"),
	STOPEMULATOR(Type.ESC, 0213, "STOPEMULATOR"),
	A214        (Type.ESC, 0214, "A214"),
	//
	//
	//


	// ColorBlt.mesa
	COLORBLT  (Type.ESC, 0300, "COLORBLT"),
	WRITEPIXEL(Type.ESC, 0301, "WRITEPIXEL"),
	BITBLTX   (Type.ESC, 0302, "BITBLTX"),
	//
	//
	A305      (Type.ESC, 0305, "A305"),
	A306      (Type.ESC, 0306, "A306");

	
	public enum Type {MOP, ESC, ESCL};

	public final Type   type;
	public final int    code;
	public       String name;
	
	Info(Type type, int code, String name) {
		this.type = type;
		this.code = code;
		this.name = name;
	}
}
