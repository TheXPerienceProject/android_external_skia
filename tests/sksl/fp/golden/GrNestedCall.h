/*#pragma settings NoInline*/

/**************************************************************************************************
 *** This file was autogenerated from GrNestedCall.fp; do not modify.
 **************************************************************************************************/
#ifndef GrNestedCall_DEFINED
#define GrNestedCall_DEFINED

#include "include/core/SkM44.h"
#include "include/core/SkTypes.h"


#include "src/gpu/GrFragmentProcessor.h"

class GrNestedCall : public GrFragmentProcessor {
public:
    static std::unique_ptr<GrFragmentProcessor> Make(std::unique_ptr<GrFragmentProcessor> fp) {
        return std::unique_ptr<GrFragmentProcessor>(new GrNestedCall(std::move(fp)));
    }
    GrNestedCall(const GrNestedCall& src);
    std::unique_ptr<GrFragmentProcessor> clone() const override;
    const char* name() const override { return "NestedCall"; }
private:
    GrNestedCall(std::unique_ptr<GrFragmentProcessor> fp)
    : INHERITED(kGrNestedCall_ClassID, kNone_OptimizationFlags) {
        this->registerChild(std::move(fp), SkSL::SampleUsage::PassThrough());    }
    GrGLSLFragmentProcessor* onCreateGLSLInstance() const override;
    void onGetGLSLProcessorKey(const GrShaderCaps&, GrProcessorKeyBuilder*) const override;
    bool onIsEqual(const GrFragmentProcessor&) const override;
#if GR_TEST_UTILS
    SkString onDumpInfo() const override;
#endif
    GR_DECLARE_FRAGMENT_PROCESSOR_TEST
    using INHERITED = GrFragmentProcessor;
};
#endif
