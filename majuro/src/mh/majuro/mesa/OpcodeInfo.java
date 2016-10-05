package mh.majuro.mesa;


public enum OpcodeInfo {
	NOOP(OpcodeType.NORMAL, 00, "NOOP"),
	LL0 (OpcodeType.NORMAL, 01, "LL0"),
	LL1 (OpcodeType.NORMAL, 02, "LL1"),
	LL2 (OpcodeType.NORMAL, 03, "LL2"),
	LL3 (OpcodeType.NORMAL, 04, "LL3"),
	LL4 (OpcodeType.NORMAL, 05, "LL4"),
	LL5 (OpcodeType.NORMAL, 06, "LL5"),
	LL6 (OpcodeType.NORMAL, 07, "LL6"),

	LL7 (OpcodeType.NORMAL, 010, "LL7"),
	LL8 (OpcodeType.NORMAL, 011, "LL8"),
	LL9 (OpcodeType.NORMAL, 012, "LL9"),
	LL10(OpcodeType.NORMAL, 013, "LL10"),
	LL11(OpcodeType.NORMAL, 014, "LL11"),
	LLB (OpcodeType.NORMAL, 015, "LLB"),
	LLD0(OpcodeType.NORMAL, 016, "LLD0"),
	LLD1(OpcodeType.NORMAL, 017, "LLD1"),

	LLD2 (OpcodeType.NORMAL, 020, "LLD2"),
	LLD3 (OpcodeType.NORMAL, 021, "LLD3"),
	LLD4 (OpcodeType.NORMAL, 022, "LLD4"),
	LLD5 (OpcodeType.NORMAL, 023, "LLD5"),
	LLD6 (OpcodeType.NORMAL, 024, "LLD6"),
	LLD7 (OpcodeType.NORMAL, 025, "LLD7"),
	LLD8 (OpcodeType.NORMAL, 026, "LLD8"),
	LLD10(OpcodeType.NORMAL, 027, "LLD10"),

	LLDB(OpcodeType.NORMAL, 030, "LLDB"),
	SL0 (OpcodeType.NORMAL, 031, "SL0"),
	SL1 (OpcodeType.NORMAL, 032, "SL1"),
	SL2 (OpcodeType.NORMAL, 033, "SL2"),
	SL3 (OpcodeType.NORMAL, 034, "SL3"),
	SL4 (OpcodeType.NORMAL, 035, "SL4"),
	SL5 (OpcodeType.NORMAL, 036, "SL5"),
	SL6 (OpcodeType.NORMAL, 037, "SL6"),

	SL7 (OpcodeType.NORMAL, 040, "SL7"),
	SL8 (OpcodeType.NORMAL, 041, "SL8"),
	SL9 (OpcodeType.NORMAL, 042, "SL9"),
	SL10(OpcodeType.NORMAL, 043, "SL10"),
	SLB (OpcodeType.NORMAL, 044, "SLB"),
	SLD0(OpcodeType.NORMAL, 045, "SLD0"),
	SLD1(OpcodeType.NORMAL, 046, "SLD1"),
	SLD2(OpcodeType.NORMAL, 047, "SLD2"),

	SLD3(OpcodeType.NORMAL, 050, "SLD3"),
	SLD4(OpcodeType.NORMAL, 051, "SLD4"),
	SLD5(OpcodeType.NORMAL, 052, "SLD5"),
	SLD6(OpcodeType.NORMAL, 053, "SLD6"),
	SLD8(OpcodeType.NORMAL, 054, "SLD8"),
	PL0 (OpcodeType.NORMAL, 055, "PL0"),
	PL1 (OpcodeType.NORMAL, 056, "PL1"),
	PL2 (OpcodeType.NORMAL, 057, "PL2"),

	PL3 (OpcodeType.NORMAL, 060, "PL3"),
	PLB (OpcodeType.NORMAL, 061, "PLB"),
	PLD0(OpcodeType.NORMAL, 062, "PLD0"),
	PLDB(OpcodeType.NORMAL, 063, "PLDB"),
	LG0 (OpcodeType.NORMAL, 064, "LG0"),
	LG1 (OpcodeType.NORMAL, 065, "LG1"),
	LG2 (OpcodeType.NORMAL, 066, "LG2"),
	LGB (OpcodeType.NORMAL, 067, "LGB"),

	LGD0 (OpcodeType.NORMAL, 070, "LGD0"),
	LGD2 (OpcodeType.NORMAL, 071, "LGD2"),
	LGDB (OpcodeType.NORMAL, 072, "LGDB"),
	SGB  (OpcodeType.NORMAL, 073, "SGB"),
	BNDCK(OpcodeType.NORMAL, 074, "BNDCK"),
	BRK  (OpcodeType.NORMAL, 075, "BRK"),
	//
	//

	R0  (OpcodeType.NORMAL, 0100, "R0"),
	R1  (OpcodeType.NORMAL, 0101, "R1"),
	RB  (OpcodeType.NORMAL, 0102, "RB"),
	RL0 (OpcodeType.NORMAL, 0103, "RL0"),
	RLB (OpcodeType.NORMAL, 0104, "RLB"),
	RD0 (OpcodeType.NORMAL, 0105, "RD0"),
	RDB (OpcodeType.NORMAL, 0106, "RDB"),
	RDL0(OpcodeType.NORMAL, 0107, "RDL0"),

	RDLB(OpcodeType.NORMAL, 0110, "RDLB"),
	W0  (OpcodeType.NORMAL, 0111, "W0"),
	WB  (OpcodeType.NORMAL, 0112, "WB"),
	PSB (OpcodeType.NORMAL, 0113, "PSB"),
	WLB (OpcodeType.NORMAL, 0114, "WLB"),
	PSLB(OpcodeType.NORMAL, 0115, "PSLB"),
	WDB (OpcodeType.NORMAL, 0116, "WDB"),
	PSD0(OpcodeType.NORMAL, 0117, "PSD0"),

	PSDB (OpcodeType.NORMAL, 0120, "PSDB"),
	WDLB (OpcodeType.NORMAL, 0121, "WDLB"),
	PSDLB(OpcodeType.NORMAL, 0122, "PSDLB"),
	RLI00(OpcodeType.NORMAL, 0123, "RLI00"),
	RLI01(OpcodeType.NORMAL, 0124, "RLI01"),
	RLI02(OpcodeType.NORMAL, 0125, "RLI02"),
	RLI03(OpcodeType.NORMAL, 0126, "RLI03"),
	RLIP (OpcodeType.NORMAL, 0127, "RLIP"),

	RLILP (OpcodeType.NORMAL, 0130, "RLILP"),
	RLDI00(OpcodeType.NORMAL, 0131, "RLDI00"),
	RLDIP (OpcodeType.NORMAL, 0132, "RLDIP"),
	RLDILP(OpcodeType.NORMAL, 0133, "RLDILP"),
	RGIP  (OpcodeType.NORMAL, 0134, "RGIP"),
	RGILP (OpcodeType.NORMAL, 0135, "RGILP"),
	WLIP  (OpcodeType.NORMAL, 0136, "WLIP"),
	WLILP (OpcodeType.NORMAL, 0137, "WLILP"),

	WLDILP(OpcodeType.NORMAL, 0140, "WLDILP"),
	RS    (OpcodeType.NORMAL, 0141, "RS"),
	RLS   (OpcodeType.NORMAL, 0142, "RLS"),
	WS    (OpcodeType.NORMAL, 0143, "WS"),
	WLS   (OpcodeType.NORMAL, 0144, "WLS"),
	R0F   (OpcodeType.NORMAL, 0145, "R0F"),
	RF    (OpcodeType.NORMAL, 0146, "RF"),
	RL0F  (OpcodeType.NORMAL, 0147, "RL0F"),

	RLF   (OpcodeType.NORMAL, 0150, "RLF"),
	RLFS  (OpcodeType.NORMAL, 0151, "RLFS"),
	RLIPF (OpcodeType.NORMAL, 0152, "RLIPF"),
	RLILPF(OpcodeType.NORMAL, 0153, "RLILPF"),
	W0F   (OpcodeType.NORMAL, 0154, "W0F"),
	WF    (OpcodeType.NORMAL, 0155, "WF"),
	PSF   (OpcodeType.NORMAL, 0156, "PSF"),
	PS0F  (OpcodeType.NORMAL, 0157, "PS0F"),

	WS0F(OpcodeType.NORMAL, 0160, "WS0F"),
	WL0F(OpcodeType.NORMAL, 0161, "WL0F"),
	WLF (OpcodeType.NORMAL, 0162, "WLF"),
	PSLF(OpcodeType.NORMAL, 0163, "PSLF"),
	WLFS(OpcodeType.NORMAL, 0164, "WLFS"),
	SLDB(OpcodeType.NORMAL, 0165, "SLDB"),
	SGDB(OpcodeType.NORMAL, 0166, "SGDB"),
	LLKB(OpcodeType.NORMAL, 0167, "LLKB"),

	RKIB   (OpcodeType.NORMAL, 0170, "RKIB"),
	RKDIB  (OpcodeType.NORMAL, 0171, "RKDIB"),
	LKB    (OpcodeType.NORMAL, 0172, "LKB"),
	SHIFT  (OpcodeType.NORMAL, 0173, "SHIFT"),
	SHIFTSB(OpcodeType.NORMAL, 0174, "SHIFTSB"),
	//
	//
	//


	CATCH(OpcodeType.NORMAL, 0200, "CATCH"),
	J2   (OpcodeType.NORMAL, 0201, "J2"),
	J3   (OpcodeType.NORMAL, 0202, "J3"),
	J4   (OpcodeType.NORMAL, 0203, "J4"),
	J5   (OpcodeType.NORMAL, 0204, "J5"),
	J6   (OpcodeType.NORMAL, 0205, "J6"),
	J7   (OpcodeType.NORMAL, 0206, "J7"),
	J8   (OpcodeType.NORMAL, 0207, "J8"),

	JB   (OpcodeType.NORMAL, 0210, "JB"),
	JW   (OpcodeType.NORMAL, 0211, "JW"),
	JEP  (OpcodeType.NORMAL, 0212, "JEP"),
	JEB  (OpcodeType.NORMAL, 0213, "JEB"),
	JEBB (OpcodeType.NORMAL, 0214, "JEBB"),
	JNEP (OpcodeType.NORMAL, 0215, "JNEP"),
	JNEB (OpcodeType.NORMAL, 0216, "JNEB"),
	JNEBB(OpcodeType.NORMAL, 0217, "JNEBB"),

	JLB  (OpcodeType.NORMAL, 0220, "JLB"),
	JGEB (OpcodeType.NORMAL, 0221, "JGEB"),
	JGB  (OpcodeType.NORMAL, 0222, "JGB"),
	JLEB (OpcodeType.NORMAL, 0223, "JLEB"),
	JULB (OpcodeType.NORMAL, 0224, "JULB"),
	JUGEB(OpcodeType.NORMAL, 0225, "JUGEB"),
	JUGB (OpcodeType.NORMAL, 0226, "JUGB"),
	JULEB(OpcodeType.NORMAL, 0227, "JULEB"),

	JZ3  (OpcodeType.NORMAL, 0230, "JZ3"),
	JZ4  (OpcodeType.NORMAL, 0231, "JZ4"),
	JZB  (OpcodeType.NORMAL, 0232, "JZB"),
	JNZ3 (OpcodeType.NORMAL, 0233, "JNZ3"),
	JNZ4 (OpcodeType.NORMAL, 0234, "JNZ4"),
	JNZB (OpcodeType.NORMAL, 0235, "JNZB"),
	JDEB (OpcodeType.NORMAL, 0236, "JDEB"),
	JDNEB(OpcodeType.NORMAL, 0237, "JDNEB"),

	JIB  (OpcodeType.NORMAL, 0240, "JIB"),
	JIW  (OpcodeType.NORMAL, 0241, "JIW"),
	REC  (OpcodeType.NORMAL, 0242, "REC"),
	REC2 (OpcodeType.NORMAL, 0243, "REC2"),
	DIS  (OpcodeType.NORMAL, 0244, "DIS"),
	DIS2 (OpcodeType.NORMAL, 0245, "DIS2"),
	EXCH (OpcodeType.NORMAL, 0246, "EXCH"),
	DEXCH(OpcodeType.NORMAL, 0247, "DEXCH"),

	DUP  (OpcodeType.NORMAL, 0250, "DUP"),
	DDUP (OpcodeType.NORMAL, 0251, "DDUP"),
	EXDIS(OpcodeType.NORMAL, 0252, "EXDIS"),
	NEG  (OpcodeType.NORMAL, 0253, "NEG"),
	INC  (OpcodeType.NORMAL, 0254, "INC"),
	DEC  (OpcodeType.NORMAL, 0255, "DEC"),
	DINC (OpcodeType.NORMAL, 0256, "DINC"),
	DBL  (OpcodeType.NORMAL, 0257, "DBL"),

	DDBL (OpcodeType.NORMAL, 0260, "DDBL"),
	TRPL (OpcodeType.NORMAL, 0261, "TRPL"),
	AND  (OpcodeType.NORMAL, 0262, "AND"),
	IOR  (OpcodeType.NORMAL, 0263, "IOR"),
	ADDSB(OpcodeType.NORMAL, 0264, "ADDSB"),
	ADD  (OpcodeType.NORMAL, 0265, "ADD"),
	SUB  (OpcodeType.NORMAL, 0266, "SUB"),
	DADD (OpcodeType.NORMAL, 0267, "DADD"),

	DSUB  (OpcodeType.NORMAL, 0270, "DSUB"),
	ADC   (OpcodeType.NORMAL, 0271, "ADC"),
	ACD   (OpcodeType.NORMAL, 0272, "ACD"),
	AL0IB (OpcodeType.NORMAL, 0273, "AL0IB"),
	MUL   (OpcodeType.NORMAL, 0274, "MUL"),
	DCMP  (OpcodeType.NORMAL, 0275, "DCMP"),
	UDCMP (OpcodeType.NORMAL, 0276, "UDCMP"),
	VMFIND(OpcodeType.NORMAL, 0277, "VMFIND"),

	
	LI0(OpcodeType.NORMAL, 0300, "LI0"),
	LI1(OpcodeType.NORMAL, 0301, "LI1"),
	LI2(OpcodeType.NORMAL, 0302, "LI2"),
	LI3(OpcodeType.NORMAL, 0303, "LI3"),
	LI4(OpcodeType.NORMAL, 0304, "LI4"),
	LI5(OpcodeType.NORMAL, 0305, "LI5"),
	LI6(OpcodeType.NORMAL, 0306, "LI6"),
	LI7(OpcodeType.NORMAL, 0307, "LI7"),

	LI8 (OpcodeType.NORMAL, 0310, "LI8"),
	LI9 (OpcodeType.NORMAL, 0311, "LI9"),
	LI10(OpcodeType.NORMAL, 0312, "LI10"),
	LIN1(OpcodeType.NORMAL, 0313, "LIN1"),
	LINI(OpcodeType.NORMAL, 0314, "LINI"),
	LIB (OpcodeType.NORMAL, 0315, "LIB"),
	LIW (OpcodeType.NORMAL, 0316, "LIW"),
	LINB(OpcodeType.NORMAL, 0317, "LINB"),

	LIHB(OpcodeType.NORMAL, 0320, "LIHB"),
	LID0(OpcodeType.NORMAL, 0321, "LID0"),
	LA0 (OpcodeType.NORMAL, 0322, "LA0"),
	LA1 (OpcodeType.NORMAL, 0323, "LA1"),
	LA2 (OpcodeType.NORMAL, 0324, "LA2"),
	LA3 (OpcodeType.NORMAL, 0325, "LA3"),
	LA6 (OpcodeType.NORMAL, 0326, "LA6"),
	LA8 (OpcodeType.NORMAL, 0327, "LA8"),

	LAB (OpcodeType.NORMAL, 0330, "LAB"),
	LAW (OpcodeType.NORMAL, 0331, "LAW"),
	GA0 (OpcodeType.NORMAL, 0332, "GA0"),
	GA1 (OpcodeType.NORMAL, 0333, "GA1"),
	GAB (OpcodeType.NORMAL, 0334, "GAB"),
	GAW (OpcodeType.NORMAL, 0335, "GAW"),
	//
	EFC0(OpcodeType.NORMAL, 0337, "EFC0"),

	EFC1(OpcodeType.NORMAL, 0340, "EFC1"),
	EFC2(OpcodeType.NORMAL, 0341, "EFC2"),
	EFC3(OpcodeType.NORMAL, 0342, "EFC3"),
	EFC4(OpcodeType.NORMAL, 0343, "EFC4"),
	EFC5(OpcodeType.NORMAL, 0344, "EFC5"),
	EFC6(OpcodeType.NORMAL, 0345, "EFC6"),
	EFC7(OpcodeType.NORMAL, 0346, "EFC7"),
	EFC8(OpcodeType.NORMAL, 0347, "EFC8"),

	EFC9 (OpcodeType.NORMAL, 0350, "EFC9"),
	EFC10(OpcodeType.NORMAL, 0351, "EFC10"),
	EFC11(OpcodeType.NORMAL, 0352, "EFC11"),
	EFC12(OpcodeType.NORMAL, 0353, "EFC12"),
	EFCB (OpcodeType.NORMAL, 0354, "EFCB"),
	LFC  (OpcodeType.NORMAL, 0355, "LFC"),
	SFC  (OpcodeType.NORMAL, 0356, "SFC"),
	RET  (OpcodeType.NORMAL, 0357, "RET"),

	KFCB (OpcodeType.NORMAL, 0360, "KFCB"),
	ME   (OpcodeType.NORMAL, 0361, "ME"),
	MX   (OpcodeType.NORMAL, 0362, "MX"),
	BLT  (OpcodeType.NORMAL, 0363, "BLT"),
	BLTL (OpcodeType.NORMAL, 0364, "BLTL"),
	BLTC (OpcodeType.NORMAL, 0365, "BLTC"),
	BLTCL(OpcodeType.NORMAL, 0366, "BLTCL"),
	LP   (OpcodeType.NORMAL, 0367, "LP"),

	ESC   (OpcodeType.NORMAL, 0370, "ESC"),
	ESCL  (OpcodeType.NORMAL, 0371, "ESCL"),
	LGA0  (OpcodeType.NORMAL, 0372, "LGA0"),
	LGAB  (OpcodeType.NORMAL, 0373, "LGAB"),
	LGAW  (OpcodeType.NORMAL, 0374, "LGAW"),
	DESC  (OpcodeType.NORMAL, 0375, "DESC"),
	//
	RESRVD(OpcodeType.NORMAL, 0377, "RESRVD"),

	//
	MW (OpcodeType.ESC, 02, "MW"),
	MR (OpcodeType.ESC, 03, "MR"),
	NC (OpcodeType.ESC, 04, "NC"),
	BC (OpcodeType.ESC, 05, "BC"),
	REQ(OpcodeType.ESC, 06, "REQ"),
	SM (OpcodeType.ESC, 07, "SM"),

	SMF(OpcodeType.ESC, 010, "SMF"),
	GMF(OpcodeType.ESC, 011, "GMF"),
	AF (OpcodeType.ESC, 012, "AF"),
	FF (OpcodeType.ESC, 013, "FF"),
	PI (OpcodeType.ESC, 014, "PI"),
	PO (OpcodeType.ESC, 015, "PO"),
	POR(OpcodeType.ESC, 016, "POR"),
	SPP(OpcodeType.ESC, 017, "SPP"),

	DI    (OpcodeType.ESC, 020, "DI"),
	EI    (OpcodeType.ESC, 021, "EI"),
	XOR   (OpcodeType.ESC, 022, "XOR"),
	DAND  (OpcodeType.ESC, 023, "DAND"),
	DIOR  (OpcodeType.ESC, 024, "DIOR"),
	DXOR  (OpcodeType.ESC, 025, "DXOR"),
	ROTATE(OpcodeType.ESC, 026, "ROTATE"),
	DSHIFT(OpcodeType.ESC, 027, "DSHIFT"),
	
	LINT (OpcodeType.ESC,  030, "LINT"),
	JS   (OpcodeType.ESC,  031, "JS"),
	RCFS (OpcodeType.ESC,  032, "RCFS"),
	RC   (OpcodeType.ESCL, 033, "RC"),
	UDIV (OpcodeType.ESC,  034, "UDIV"),
	LUDIV(OpcodeType.ESC,  035, "LUDIV"),
	ROB  (OpcodeType.ESCL, 036, "ROB"),
	WOB  (OpcodeType.ESCL, 037, "WOB"),

	DSK   (OpcodeType.ESCL, 040, "DSK"),
	XE    (OpcodeType.ESCL, 041, "XE"),
	XF    (OpcodeType.ESCL, 042, "XF"),
	LSK   (OpcodeType.ESCL, 043, "LSK"),
	BNDCKL(OpcodeType.ESC,  044, "BNDCKL"),
	NILCK (OpcodeType.ESC,  045, "NILCK"),
	NILCKL(OpcodeType.ESC,  046, "NILCKL"),
	BLTLR (OpcodeType.ESC,  047, "BLTLR"),
	
	BLEL   (OpcodeType.ESC, 050, "BLEL"),
	BLECL  (OpcodeType.ESC, 051, "BLECL"),
	CKSUM  (OpcodeType.ESC, 052, "CKSUM"),
	BITBLT (OpcodeType.ESC, 053, "BITBLT"),
	TXTBLT (OpcodeType.ESC, 054, "TXTBLT"),
	BYTBLT (OpcodeType.ESC, 055, "BYTBLT"),
	BYTBLTR(OpcodeType.ESC, 056, "BYTBLTR"),
	VERSION(OpcodeType.ESC, 057, "VERSION"),

	DMUL (OpcodeType.ESC, 060, "DMUL"),
	SDIV (OpcodeType.ESC, 061, "SDIV"),
	SDDIV(OpcodeType.ESC, 062, "SDDIV"),
	UDDIV(OpcodeType.ESC, 063, "UDDIV"),

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
	WRPSB(OpcodeType.ESC, 0160, "WRPSB"),
	WRMDS(OpcodeType.ESC, 0161, "WRMDS"),
	WRWP (OpcodeType.ESC, 0162, "WRWP"),
	WRWDC(OpcodeType.ESC, 0163, "WRWDC"),
	WRPTC(OpcodeType.ESC, 0164, "WRPTC"),
	WRIT (OpcodeType.ESC, 0165, "WRIT"),
	WRXTS(OpcodeType.ESC, 0166, "WRXTS"),
	WRMP (OpcodeType.ESC, 0167, "WRMP"),
	
	RRPSB(OpcodeType.ESC, 0170, "RRPSB"),
	RRMDS(OpcodeType.ESC, 0171, "RRMDS"),
	RRWP (OpcodeType.ESC, 0172, "RRWP"),
	RRWDC(OpcodeType.ESC, 0173, "RRWDC"),
	RRPTC(OpcodeType.ESC, 0174, "RRPTC"),
	RRIT (OpcodeType.ESC, 0175, "RRIT"),
	RRXTS(OpcodeType.ESC, 0176, "RRXTS"),
	//

	
	// Guam
	//   GuamInputOutput.mesa
	CALLAGENT   (OpcodeType.ESC, 0211, "CALLAGENT"),
	MAPDISPLAY  (OpcodeType.ESC, 0212, "MAPDISPLAY"),
	STOPEMULATOR(OpcodeType.ESC, 0213, "STOPEMULATOR"),
	A214        (OpcodeType.ESC, 0214, "A214"),
	//
	//
	//


	// ColorBlt.mesa
	COLORBLT  (OpcodeType.ESC, 0300, "COLORBLT"),
	WRITEPIXEL(OpcodeType.ESC, 0301, "WRITEPIXEL"),
	BITBLTX   (OpcodeType.ESC, 0302, "BITBLTX"),
	//
	//
	A305      (OpcodeType.ESC, 0305, "A305"),
	A306      (OpcodeType.ESC, 0306, "A306");

	
	public enum OpcodeType {NORMAL, ESC, ESCL};

	public final OpcodeType   type;
	public final int    code;
	public       String name;
	
	OpcodeInfo(OpcodeType type, int code, String name) {
		this.type = type;
		this.code = code;
		this.name = name;
	}
}
