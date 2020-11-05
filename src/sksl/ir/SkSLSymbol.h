/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SKSL_SYMBOL
#define SKSL_SYMBOL

#include "src/sksl/ir/SkSLIRNode.h"
#include "src/sksl/ir/SkSLProgramElement.h"

namespace SkSL {

/**
 * Represents a symboltable entry.
 */
class Symbol : public IRNode {
public:
    enum class Kind {
        kExternal = (int) ProgramElement::Kind::kLast + 1,
        kField,
        kFunctionDeclaration,
        kSymbolAlias,
        kType,
        kUnresolvedFunction,
        kVariable,

        kFirst = kExternal,
        kLast = kVariable
    };

    Symbol(int offset, Kind kind, StringFragment name, const Type* type = nullptr)
    : INHERITED(offset, (int) kind, SymbolData{name, type}) {
        SkASSERT(kind >= Kind::kFirst && kind <= Kind::kLast);
    }

    Symbol(int offset, const FieldData& data)
    : INHERITED(offset, (int) Kind::kField, data) {}

    Symbol(int offset, const FunctionDeclarationData& data)
    : INHERITED(offset, (int) Kind::kFunctionDeclaration, data) {}

    Symbol(int offset, const SymbolAliasData& data)
    : INHERITED(offset, (int) Kind::kSymbolAlias, data) {}

    Symbol(int offset, const UnresolvedFunctionData& data)
    : INHERITED(offset, (int) Kind::kUnresolvedFunction, data) {}

    Symbol(int offset, const VariableData& data)
    : INHERITED(offset, (int) Kind::kVariable, data) {}

    Symbol& operator=(const Symbol&) = default;

    ~Symbol() override {}

    virtual const Type& type() const {
        return *this->symbolData().fType;
    }

    Kind kind() const {
        return (Kind) fKind;
    }

    virtual StringFragment name() const {
        return this->symbolData().fName;
    }

    /**
     *  Use is<T> to check the type of a symbol.
     *  e.g. replace `sym.kind() == Symbol::Kind::kVariable` with `sym.is<Variable>()`.
     */
    template <typename T>
    bool is() const {
        return this->kind() == T::kSymbolKind;
    }

    /**
     *  Use as<T> to downcast symbols. e.g. replace `(Variable&) sym` with `sym.as<Variable>()`.
     */
    template <typename T>
    const T& as() const {
        SkASSERT(this->is<T>());
        return static_cast<const T&>(*this);
    }

    template <typename T>
    T& as() {
        SkASSERT(this->is<T>());
        return static_cast<T&>(*this);
    }

private:
    using INHERITED = IRNode;
};

}  // namespace SkSL

#endif
