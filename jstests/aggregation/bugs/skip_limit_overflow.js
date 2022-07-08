/**
 * SERVER-39788 Test that the values in the $limit and $skip stages do not overflow when the
 * pipeline is optimized and the $sort stage doesn't crash the server due to unreasonable memory
 * requirements.
 *
 * This test makes assumptions about how the explain output will be formatted, so cannot be
 * transformed to be put inside a $facet stage, in a sharded explain output or when pipeline
 * optimization is disabled.
 * @tags: [do_not_wrap_aggregations_in_facets,assumes_unsharded_collection,
 *         requires_pipeline_optimization]
 */
(function() {
"use strict";

load("jstests/libs/analyze_plan.js");  // For 'aggPlanHasStages' and other explain helpers.

const coll = db.server39788;
coll.drop();
assert.commandWorked(db.runCommand({create: coll.getName()}));

function testPipeline(pipeline, expectedResult, optimizedAwayStages) {
    const explainOutput = coll.explain().aggregate(pipeline);

    if (optimizedAwayStages) {
        optimizedAwayStages.forEach(
            (stage) =>
                assert(!aggPlanHasStage(explainOutput, stage),
                       "Expected pipeline " + tojsononeline(pipeline) + " to *not* include a " +
                           stage + " stage in the explain output: " + tojson(explainOutput)));
    }

    for (const stageName in expectedResult) {
        const path = expectedResult[stageName].path;
        const expectedValue = expectedResult[stageName].expectedValue;

        const stages = getAggPlanStages(explainOutput, stageName);
        assert(stages !== null,
               "Expected pipeline " + tojsononeline(pipeline) + " to include a " + stageName +
                   " stage in the explain output: " + tojson(explainOutput));
        assert.eq(stages.length,
                  expectedValue.length,
                  "Expected pipeline " + tojsononeline(pipeline) + " to include " +
                      expectedValue.length + " " + stageName +
                      " stages in the explain output: " + tojson(explainOutput));

        const subPaths = path.split(".");
        assert.eq(
            stages.reduce(
                (res, stage) => {
                    res.push(subPaths.reduce((res, cur) => res[cur], stage));
                    return res;
                },
                []),
            expectedValue,
            "Stage: " + stageName + ", path: " + path + ", explain: " + tojson(explainOutput));
    }

    // Ensure the aggregate command doesn't fail.
    assert.eq(coll.aggregate(pipeline).toArray(), []);
}

// Case where overflow of limit + skip prevents limit stage from being absorbed. Values
// are specified as integrals > MAX_LONG. Note that we cannot specify this huge value as
// a NumberLong, as we get a number conversion error (even if it's passed as a string).
testPipeline([{$sort: {x: -1}}, {$skip: 18446744073709552000}, {$limit: 6}], {
    $limit: {path: "$limit", expectedValue: [NumberLong(6)]},
    $skip: {path: "$skip", expectedValue: [NumberLong("9223372036854775807")]}
});
testPipeline([{$sort: {x: -1}}, {$skip: 6}, {$limit: 18446744073709552000}], {
    $limit: {path: "$limit", expectedValue: [NumberLong("9223372036854775807")]},
    $skip: {path: "$skip", expectedValue: [NumberLong(6)]}
});

// Case where overflow of limit + skip prevents limit stage from being absorbed. One of the
// values == MAX_LONG, another one is 1.
testPipeline([{$sort: {x: -1}}, {$skip: NumberLong("9223372036854775807")}, {$limit: 1}], {
    $limit: {path: "$limit", expectedValue: [NumberLong(1)]},
    $skip: {path: "$skip", expectedValue: [NumberLong("9223372036854775807")]}
});
testPipeline([{$sort: {x: -1}}, {$skip: 1}, {$limit: NumberLong("9223372036854775807")}], {
    $limit: {path: "$limit", expectedValue: [NumberLong("9223372036854775807")]},
    $skip: {path: "$skip", expectedValue: [NumberLong(1)]}
});

// Case where limit + skip do not overflow. Limit == MAX_LONG and skip is 0. Should be able to
// absorb the limit and skip stages.
// Note that we cannot specify limit == 0, so we expect an error in this case.
testPipeline([{$sort: {x: -1}}, {$skip: 0}, {$limit: NumberLong("9223372036854775807")}],
             {SORT: {path: "limitAmount", expectedValue: [NumberLong("9223372036854775807")]}},
             ["$skip", "$limit"]);

// Case where limit + skip do not overflow. One value is MAX_LONG - 1 and another one is 1.
// Should be able to absorb the limit stage.
testPipeline([{$sort: {x: -1}}, {$skip: NumberLong("9223372036854775806")}, {$limit: 1}],
             {
                 SORT: {path: "limitAmount", expectedValue: [NumberLong("9223372036854775807")]},
                 $skip: {path: "$skip", expectedValue: [NumberLong("9223372036854775806")]}
             },
             ["$limit"]);
testPipeline([{$sort: {x: -1}}, {$skip: 1}, {$limit: NumberLong("9223372036854775806")}],
             {
                 SORT: {path: "limitAmount", expectedValue: [NumberLong("9223372036854775807")]},
                 $skip: {path: "$skip", expectedValue: [NumberLong(1)]}
             },
             ["$limit"]);

// Case where the first $limit can be pushed down, but the second overflows and thus remains in
// place.
testPipeline(
    [
        {$sort: {x: -1}},
        {$skip: NumberLong("9223372036854775800")},
        {$limit: 7},
        {$skip: 10},
        {$limit: 1}
    ],
    {
        SORT: {path: "limitAmount", expectedValue: [NumberLong("9223372036854775807")]},
        $limit: {path: "$limit", expectedValue: [NumberLong(1)]}
    });

// Case with multiple $limit and $skip stages where the second $limit ends up being the
// smallest. There is no overflow in this case.
testPipeline(
    [
        {$sort: {x: -1}},
        {$skip: NumberLong("9223372036854775800")},
        {$limit: 7},
        {$skip: 3},
        {$limit: 1}
    ],
    {
        SORT: {path: "limitAmount", expectedValue: [NumberLong("9223372036854775804")]},
        $skip: {path: "$skip", expectedValue: [NumberLong("9223372036854775803")]}
    });

// Case where limit + skip do not overflow. Both values are < MAX_LONG.
testPipeline([{$sort: {x: -1}}, {$skip: 674761616283}, {$limit: 35361718}],
             {
                 SORT: {path: "limitAmount", expectedValue: [NumberLong(674796978001)]},
                 $skip: {path: "$skip", expectedValue: [NumberLong(674761616283)]}
             },
             ["$limit"]);
testPipeline([{$sort: {x: -1}}, {$skip: 35361718}, {$limit: 674761616283}],
             {
                 SORT: {path: "limitAmount", expectedValue: [NumberLong(674796978001)]},
                 $skip: {path: "$skip", expectedValue: [NumberLong(35361718)]}
             },
             ["$limit"]);

// Case where where overflow of limit + skip + skip prevents limit stage from being absorbed.
// One skip == MAX_LONG - 1, another one is 1. Should merge two skip stages into one.
testPipeline(
    [{$sort: {x: -1}}, {$skip: 1}, {$skip: NumberLong("9223372036854775806")}, {$limit: 1}],
    {
        $limit: {path: "$limit", expectedValue: [NumberLong(1)]},
        $skip: {path: "$skip", expectedValue: [NumberLong("9223372036854775807")]}
    },
    ["$sort"]);

// Case where where overflow of limit + skip + skip prevents limit stage from being absorbed.
// One skip == MAX_LONG, another one is 1. Should not absorb or merge any stages.
testPipeline(
    [{$sort: {x: -1}}, {$skip: 1}, {$skip: NumberLong("9223372036854775807")}, {$limit: 1}],
    {
        $limit: {path: "$limit", expectedValue: [NumberLong(1)]},
        $skip: {path: "$skip", expectedValue: [NumberLong(1), NumberLong("9223372036854775807")]}
    },
    ["$sort"]);

// Case where sample size is > MAX_LONG.
testPipeline([{$sample: {size: 18446744073709552000}}],
             {$sample: {path: "$sample.size", expectedValue: [NumberLong("9223372036854775807")]}});
// Case where sample size is == MAX_LONG.
testPipeline([{$sample: {size: NumberLong("9223372036854775807")}}],
             {$sample: {path: "$sample.size", expectedValue: [NumberLong("9223372036854775807")]}});
// Case where sample size is == MAX_LONG - 1.
testPipeline([{$sample: {size: NumberLong("9223372036854775806")}}],
             {$sample: {path: "$sample.size", expectedValue: [NumberLong("9223372036854775806")]}});
})();
