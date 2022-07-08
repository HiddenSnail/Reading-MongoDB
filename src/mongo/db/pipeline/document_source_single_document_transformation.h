/**
 *    Copyright (C) 2018-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#pragma once

#include <type_traits>

#include "mongo/db/pipeline/document_source.h"
#include "mongo/db/pipeline/transformer_interface.h"

namespace mongo {

/**
 * This class is for DocumentSources that take in and return one document at a time, in a 1:1
 * transformation. It should only be used via an alias that passes the transformation logic through
 * a ParsedSingleDocumentTransformation. It is not a registered DocumentSource, and it cannot be
 * created from BSON.
 */
class DocumentSourceSingleDocumentTransformation final : public DocumentSource {
public:
    virtual boost::intrusive_ptr<DocumentSource> clone() const {
        auto list = DocumentSource::parse(pExpCtx, serialize().getDocument().toBson());
        invariant(list.size() == 1);
        return list.front();
    }

    DocumentSourceSingleDocumentTransformation(
        const boost::intrusive_ptr<ExpressionContext>& pExpCtx,
        std::unique_ptr<TransformerInterface> parsedTransform,
        const StringData name,
        bool independentOfAnyCollection);

    // virtuals from DocumentSource
    const char* getSourceName() const final;

    boost::intrusive_ptr<DocumentSource> optimize() final;
    Value serialize(boost::optional<ExplainOptions::Verbosity> explain = boost::none) const final;
    DepsTracker::State getDependencies(DepsTracker* deps) const final;
    GetModPathsReturn getModifiedPaths() const final;
    StageConstraints constraints(Pipeline::SplitState pipeState) const final {
        StageConstraints constraints(StreamType::kStreaming,
                                     PositionRequirement::kNone,
                                     HostTypeRequirement::kNone,
                                     DiskUseRequirement::kNoDiskUse,
                                     FacetRequirement::kAllowed,
                                     TransactionRequirement::kAllowed,
                                     LookupRequirement::kAllowed,
                                     UnionRequirement::kAllowed,
                                     ChangeStreamRequirement::kWhitelist);
        constraints.canSwapWithMatch = true;
        constraints.canSwapWithLimitAndSample = true;
        constraints.isAllowedWithinUpdatePipeline = true;
        // This transformation could be part of a 'collectionless' change stream on an entire
        // database or cluster, mark as independent of any collection if so.
        constraints.isIndependentOfAnyCollection = _isIndependentOfAnyCollection;
        return constraints;
    }

    boost::optional<DistributedPlanLogic> distributedPlanLogic() final {
        return boost::none;
    }

    TransformerInterface::TransformerType getType() const {
        return _parsedTransform->getType();
    }

    const auto& getTransformer() const {
        return *_parsedTransform;
    }

protected:
    GetNextResult doGetNext() final;
    void doDispose() final;

    Pipeline::SourceContainer::iterator doOptimizeAt(Pipeline::SourceContainer::iterator itr,
                                                     Pipeline::SourceContainer* container) final;

private:
    // Stores transformation logic.
    std::unique_ptr<TransformerInterface> _parsedTransform;

    // Specific name of the transformation.
    std::string _name;

    // Set to true if this transformation stage can be run on the collectionless namespace.
    bool _isIndependentOfAnyCollection;

    // Cached stage options in case this DocumentSource is disposed before serialized (e.g. explain
    // with a sort which will auto-dispose of the pipeline).
    Document _cachedStageOptions;
};

}  // namespace mongo
