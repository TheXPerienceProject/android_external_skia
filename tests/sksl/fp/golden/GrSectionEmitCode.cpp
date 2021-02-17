

/**************************************************************************************************
 *** This file was autogenerated from GrSectionEmitCode.fp; do not modify.
 **************************************************************************************************/
#include "GrSectionEmitCode.h"

#include "src/core/SkUtils.h"
#include "src/gpu/GrTexture.h"
#include "src/gpu/glsl/GrGLSLFragmentProcessor.h"
#include "src/gpu/glsl/GrGLSLFragmentShaderBuilder.h"
#include "src/gpu/glsl/GrGLSLProgramBuilder.h"
#include "src/sksl/SkSLCPP.h"
#include "src/sksl/SkSLUtil.h"
class GrGLSLSectionEmitCode : public GrGLSLFragmentProcessor {
public:
    GrGLSLSectionEmitCode() {}
    void emitCode(EmitArgs& args) override {
        GrGLSLFPFragmentBuilder* fragBuilder = args.fFragBuilder;
        const GrSectionEmitCode& _outer = args.fFp.cast<GrSectionEmitCode>();
        (void) _outer;
x = 10.0;
 fragBuilder->codeAppendf("half y = %d\n", x * 2);         fragBuilder->codeAppendf(
R"SkSL(half x = %f;
return half4(1.0);
)SkSL"
, x);
    }
private:
    void onSetData(const GrGLSLProgramDataManager& pdman, const GrFragmentProcessor& _proc) override {
    }
float x = 0;
};
GrGLSLFragmentProcessor* GrSectionEmitCode::onCreateGLSLInstance() const {
    return new GrGLSLSectionEmitCode();
}
void GrSectionEmitCode::onGetGLSLProcessorKey(const GrShaderCaps& caps, GrProcessorKeyBuilder* b) const {
}
bool GrSectionEmitCode::onIsEqual(const GrFragmentProcessor& other) const {
    const GrSectionEmitCode& that = other.cast<GrSectionEmitCode>();
    (void) that;
    return true;
}
GrSectionEmitCode::GrSectionEmitCode(const GrSectionEmitCode& src)
: INHERITED(kGrSectionEmitCode_ClassID, src.optimizationFlags()) {
        this->cloneAndRegisterAllChildProcessors(src);
}
std::unique_ptr<GrFragmentProcessor> GrSectionEmitCode::clone() const {
    return std::make_unique<GrSectionEmitCode>(*this);
}
#if GR_TEST_UTILS
SkString GrSectionEmitCode::onDumpInfo() const {
    return SkString();
}
#endif
