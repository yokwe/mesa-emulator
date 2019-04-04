// Generated from src-java/mesa/courier/antlr/Courier.g by ANTLR 4.1
package mesa.courier.antlr;
import org.antlr.v4.runtime.misc.NotNull;
import org.antlr.v4.runtime.tree.ParseTreeVisitor;

/**
 * This interface defines a complete generic visitor for a parse tree produced
 * by {@link CourierParser}.
 *
 * @param <T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public interface CourierVisitor<T> extends ParseTreeVisitor<T> {
	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeByte}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeByte(@NotNull CourierParser.TypeByteContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeUnspecified2}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeUnspecified2(@NotNull CourierParser.TypeUnspecified2Context ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeUnspecified3}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeUnspecified3(@NotNull CourierParser.TypeUnspecified3Context ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#constant}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstant(@NotNull CourierParser.ConstantContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#DeclConst}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitDeclConst(@NotNull CourierParser.DeclConstContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeRef}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeRef(@NotNull CourierParser.TypeRefContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#declarationList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitDeclarationList(@NotNull CourierParser.DeclarationListContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#componentList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitComponentList(@NotNull CourierParser.ComponentListContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#DeclType}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitDeclType(@NotNull CourierParser.DeclTypeContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#type}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitType(@NotNull CourierParser.TypeContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeString}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeString(@NotNull CourierParser.TypeStringContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#argumentList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitArgumentList(@NotNull CourierParser.ArgumentListContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeRefQ}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeRefQ(@NotNull CourierParser.TypeRefQContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#ConstChoice}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstChoice(@NotNull CourierParser.ConstChoiceContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#typedCandidateList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypedCandidateList(@NotNull CourierParser.TypedCandidateListContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#ConstArray}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstArray(@NotNull CourierParser.ConstArrayContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeLongCardinal}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeLongCardinal(@NotNull CourierParser.TypeLongCardinalContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#dependencyList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitDependencyList(@NotNull CourierParser.DependencyListContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#elementList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitElementList(@NotNull CourierParser.ElementListContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeArray}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeArray(@NotNull CourierParser.TypeArrayContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeChoiceTyped}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeChoiceTyped(@NotNull CourierParser.TypeChoiceTypedContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#programHeader}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitProgramHeader(@NotNull CourierParser.ProgramHeaderContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#anonCandidateList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAnonCandidateList(@NotNull CourierParser.AnonCandidateListContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#ValueRefConsts}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitValueRefConsts(@NotNull CourierParser.ValueRefConstsContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#ConstFalse}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstFalse(@NotNull CourierParser.ConstFalseContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#ConstTrue}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstTrue(@NotNull CourierParser.ConstTrueContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#field}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitField(@NotNull CourierParser.FieldContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#maximumNumber}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMaximumNumber(@NotNull CourierParser.MaximumNumberContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeError}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeError(@NotNull CourierParser.TypeErrorContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#referencedProgram}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitReferencedProgram(@NotNull CourierParser.ReferencedProgramContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#resultList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitResultList(@NotNull CourierParser.ResultListContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeRecord}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeRecord(@NotNull CourierParser.TypeRecordContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeProcedure}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeProcedure(@NotNull CourierParser.TypeProcedureContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#ConstNumber}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstNumber(@NotNull CourierParser.ConstNumberContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#ConstRefQ}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstRefQ(@NotNull CourierParser.ConstRefQContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeUnspecified}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeUnspecified(@NotNull CourierParser.TypeUnspecifiedContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#ConstRecord}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstRecord(@NotNull CourierParser.ConstRecordContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#ConstEmpty}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstEmpty(@NotNull CourierParser.ConstEmptyContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#ConstRef}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstRef(@NotNull CourierParser.ConstRefContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#referencedProgramList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitReferencedProgramList(@NotNull CourierParser.ReferencedProgramListContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeChoiceAnon}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeChoiceAnon(@NotNull CourierParser.TypeChoiceAnonContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#fieldList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFieldList(@NotNull CourierParser.FieldListContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#courierProgram}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCourierProgram(@NotNull CourierParser.CourierProgramContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeEmptyRecord}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeEmptyRecord(@NotNull CourierParser.TypeEmptyRecordContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#anonCandidate}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAnonCandidate(@NotNull CourierParser.AnonCandidateContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#ConstNumberNegative}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstNumberNegative(@NotNull CourierParser.ConstNumberNegativeContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#correspondence}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCorrespondence(@NotNull CourierParser.CorrespondenceContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#errorList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitErrorList(@NotNull CourierParser.ErrorListContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#typedCandidate}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypedCandidate(@NotNull CourierParser.TypedCandidateContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeCardinal}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeCardinal(@NotNull CourierParser.TypeCardinalContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeBoolean}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeBoolean(@NotNull CourierParser.TypeBooleanContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#component}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitComponent(@NotNull CourierParser.ComponentContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#correspondenceList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCorrespondenceList(@NotNull CourierParser.CorrespondenceListContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#programBody}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitProgramBody(@NotNull CourierParser.ProgramBodyContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeSequence}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeSequence(@NotNull CourierParser.TypeSequenceContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#TypeEnum}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeEnum(@NotNull CourierParser.TypeEnumContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#nameList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitNameList(@NotNull CourierParser.NameListContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#ValueNumber}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitValueNumber(@NotNull CourierParser.ValueNumberContext ctx);

	/**
	 * Visit a parse tree produced by {@link CourierParser#ConstString}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConstString(@NotNull CourierParser.ConstStringContext ctx);
}