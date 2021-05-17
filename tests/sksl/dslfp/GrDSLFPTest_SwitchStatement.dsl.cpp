

/**************************************************************************************************
 *** This file was autogenerated from GrDSLFPTest_SwitchStatement.fp; do not modify.
 **************************************************************************************************/
/* TODO(skia:11854): DSLCPPCodeGenerator is currently a work in progress. */
#include "GrDSLFPTest_SwitchStatement.h"

#include "src/core/SkUtils.h"
#include "src/gpu/GrTexture.h"
#include "src/gpu/glsl/GrGLSLFragmentProcessor.h"
#include "src/gpu/glsl/GrGLSLFragmentShaderBuilder.h"
#include "src/gpu/glsl/GrGLSLProgramBuilder.h"
#include "src/sksl/SkSLCPP.h"
#include "src/sksl/SkSLUtil.h"
#include "src/sksl/dsl/priv/DSLFPs.h"
#include "src/sksl/dsl/priv/DSLWriter.h"

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wcomma"
#endif

class GrGLSLDSLFPTest_SwitchStatement : public GrGLSLFragmentProcessor {
public:
    GrGLSLDSLFPTest_SwitchStatement() {}
    void emitCode(EmitArgs& args) override {
        const GrDSLFPTest_SwitchStatement& _outer = args.fFp.cast<GrDSLFPTest_SwitchStatement>();
        (void) _outer;

        using namespace SkSL::dsl;
        StartFragmentProcessor(this, &args);
Var color(kNo_Modifier, DSLType(kInt4_Type), "color", Int4(0));
Declare(color);
Switch(color.x(),
    Case(0, ++color.y()),
    Case(1, Break()),
    Case(2, Return(Half4(0.0f))),
    Case(3),
    Case(4, ++color.x()),
    Case(5, Block(++color.z()), Break()),
    Default(Block(--color.y(), Break())));
Switch(color.y(),
    Case(1, Break()),
    Case(0, Block(color.x() = 1, color.z() = 1)));
Block(color.w() = color.y());
Return(Half4(color));
        EndFragmentProcessor();
    }
private:
    void onSetData(const GrGLSLProgramDataManager& pdman, const GrFragmentProcessor& _proc) override {
    }
};
std::unique_ptr<GrGLSLFragmentProcessor> GrDSLFPTest_SwitchStatement::onMakeProgramImpl() const {
    return std::make_unique<GrGLSLDSLFPTest_SwitchStatement>();
}
void GrDSLFPTest_SwitchStatement::onGetGLSLProcessorKey(const GrShaderCaps& caps, GrProcessorKeyBuilder* b) const {
}
bool GrDSLFPTest_SwitchStatement::onIsEqual(const GrFragmentProcessor& other) const {
    const GrDSLFPTest_SwitchStatement& that = other.cast<GrDSLFPTest_SwitchStatement>();
    (void) that;
    return true;
}
GrDSLFPTest_SwitchStatement::GrDSLFPTest_SwitchStatement(const GrDSLFPTest_SwitchStatement& src)
: INHERITED(kGrDSLFPTest_SwitchStatement_ClassID, src.optimizationFlags()) {
        this->cloneAndRegisterAllChildProcessors(src);
}
std::unique_ptr<GrFragmentProcessor> GrDSLFPTest_SwitchStatement::clone() const {
    return std::make_unique<GrDSLFPTest_SwitchStatement>(*this);
}
#if GR_TEST_UTILS
SkString GrDSLFPTest_SwitchStatement::onDumpInfo() const {
    return SkString();
}
#endif
