#pragma once

#include "Element.h"

struct Document : public Node
{

    Document() :
        documentElement(std::make_shared<Element>())
    {
    }

    std::shared_ptr<Element> documentElement;
    const std::shared_ptr<Element> &getDocumentElement() const {
        return documentElement;
    }

    static duk_ret_t createElement(duk::Context &ctx, duk_idx_t nargs);

    template <class Inspector>
    static void inspect(Inspector &i) {
        i.construct(&std::make_shared<Document>);
        i.constant("readyState", "ready");
        i.property("documentElement", &Document::getDocumentElement);
        i.function("createElement", &Document::createElement);
    }

};

DUK_CPP_DEF_CLASS_NAME(Document);
DUK_CPP_DEF_BASE_CLASS(Document, Node);
