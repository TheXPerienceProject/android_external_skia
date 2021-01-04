

/**************************************************************************************************
 *** This file was autogenerated from GrSectionFields.fp; do not modify.
 **************************************************************************************************/
#include "GrSectionFields.h"

#include "src/core/SkUtils.h"
#include "src/gpu/GrTexture.h"
#include "src/gpu/glsl/GrGLSLFragmentProcessor.h"
#include "src/gpu/glsl/GrGLSLFragmentShaderBuilder.h"
#include "src/gpu/glsl/GrGLSLProgramBuilder.h"
#include "src/sksl/SkSLCPP.h"
#include "src/sksl/SkSLUtil.h"
class GrGLSLSectionFields : public GrGLSLFragmentProcessor {
public:
    GrGLSLSectionFields() {}
    void emitCode(EmitArgs& args) override {
        GrGLSLFPFragmentBuilder* fragBuilder = args.fFragBuilder;
        const GrSectionFields& _outer = args.fFp.cast<GrSectionFields>();
        (void) _outer;
        fragBuilder->codeAppendf(
R"SkSL(%s = half4(1.0);
)SkSL"
, args.fOutputColor);
    }
private:
    void onSetData(const GrGLSLProgramDataManager& pdman, const GrFragmentProcessor& _proc) override {
    }
};
GrGLSLFragmentProcessor* GrSectionFields::onCreateGLSLInstance() const {
    return new GrGLSLSectionFields();
}
void GrSectionFields::onGetGLSLProcessorKey(const GrShaderCaps& caps, GrProcessorKeyBuilder* b) const {
}
bool GrSectionFields::onIsEqual(const GrFragmentProcessor& other) const {
    const GrSectionFields& that = other.cast<GrSectionFields>();
    (void) that;
    return true;
}
bool GrSectionFields::usesExplicitReturn() const {
    return false;
}
#if GR_TEST_UTILS
SkString GrSectionFields::onDumpInfo() const {

}
#endif
