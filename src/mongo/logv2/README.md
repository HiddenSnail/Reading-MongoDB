# Log System Overview

The new log system adds capability to produce structured logs in the [Relaxed Extended JSON 2.0.0](https://github.com/mongodb/specifications/blob/master/source/extended-json.rst) format. The new API requires names to be given to variables, forming field names for the variables in structured JSON logs. Named variables are called attributes in the log system. Human readable log messages are built with a [libfmt](https://fmt.dev/6.1.1/index.html) inspired API, where attributes are inserted using replacement fields instead of being streamed together using the streaming operator `<<`.

# Style guide

## In general

Log lines are composed primarily of a message (`msg`) and attributes (`attr.*` fields).

## Philosophy

As you write log messages, keep the following in mind: A big thing that makes (J|B)SON unique as a data format is the ability to provide rich field names.

What makes logv2 machine readable is that we write an intact Extended BSON format.

But, what makes these lines human readable is that the `msg` provides a simple, clear context for interpreting well-formed field names and values in the `attr` subdocument.

## Specific Guidance

For maximum readability, a log message additionally has the least amount of repetition possible, and shares attribute names with other related log lines.

### Message (the msg field)

The `msg` field predicates a reader's interpretation of the log line. It should be crafted with care and attention.

* Concisely describe what the log line is reporting, providing enough context necessary for interpreting attribute field names and values
* Avoid unnecessary punctuation and do not conclude with punctuation
* For new log messages, do __not__ use a format string/substitution for new log messages
* For updating existing log messages, provide both a format string/substitution, __and__ a substitution-free message string

### Attributes (fields in the attr subdocument)

The `attr` subdocument includes important metrics/statistics about the logged event for the purposes of debugging or performance analysis. These variables should be named very well, as though intended for a very human-readable portion of the codebase (like config variable declaration, abstract class definitions, etc.)

For `attr` fields, do the following:

#### Use camelCased words understandable in the context of the message (msg)

The bar for understanding should be:

* Someone with reasonable understanding of mongod behavior should understand immediately what is being logged
* Someone with reasonable troubleshooting skill should be able to extract doc- or code-searchable phrases to learn about what is being logged

#### Precisely describe values and units

Exception: Do not add a unit suffix when logging a Duration type. The system automatically adds this unit.

#### When providing an execution time attribute, ensure it is named "durationMillis"

To describe the execution time of an operation using our preferred method: Specify an `attr` name of “duration” and provide a value using the Milliseconds Duration type. The log system will automatically append "Millis" to the attribute name.

Alternatively, specify an `attr` name of “durationMillis” and provide the number of milliseconds as an integer type.

__Importantly__: downstream analysis tools will rely on this convention, as a replacement for the "[0-9]+ms$" format of prior logs.

#### Use certain specific terms whenever possible

When logging the below information, do so with these specific terms:

* __namespace__ - instead of "ns"
* __db__ - instead of "database"
* __error__ - when an error occurs, instead of "status". Use this for objects of type Status and DBException
* __reason__ - to provide rationale for an event/action when "error" isn't appropriate

### Examples

For new log lines:

 ```
LOGV2(1041, "Transition to PRIMARY complete");

{ ... , "id": 1041, "msg": "Transition to PRIMARY complete", "attr": {} }
```

```
LOGV2(1042, "Slow query", "duration"_attr = getDurationMillis());

{ ..., "id": 1042, "msg": "Slow query", "attr": { "durationMillis": 1000 } }

```

For updating existing log lines:

```
LOGV2(1040, 
      "Replica set state transition from {oldState} to {newState} on this node", 
      "Replica set state transition on this node", 
      "oldState"_attr = getOldState(), "newState"_attr = getNewState());

{ ..., "id": 1040, "msg": "Replica set state transition on this node", "attr": { "oldState": "SECONARY", "newState": "PRIMARY" } }
```

# Basic Usage

The log system is made available with the following header:

`#include "mongo/logv2/log.h"`

To be able to include it a default log component needs to be defined in the cpp file before including `log.h`:

`#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kDefault`

Logging is performed using function style macros:

`LOGV2(ID, format-string, "name0"_attr = var0, ..., "nameN"_attr = varN);`

`LOGV2(ID, format-string, message-string, "name0"_attr = var0, ..., "nameN"_attr = varN);`

The ID is a signed 32bit integer in the same number space as the error code numbers. It is used to uniquely identify a log statement. If changing existing code, using a new ID is strongly advised to avoid any parsing ambiguity. 

The format string contains the description of the log event with libfmt style replacement fields optionally embedded within it. The format string must comply with the [format syntax](https://fmt.dev/6.1.1/syntax.html#formatspec) from libfmt. The purpose of embedding the replacement fields is to be able to create a human readable message used by the text output format or a tool that converts JSON logs to a human readable format.

Replacement fields are placed in the format string with curly braces `{}`. Everything not surrounded with curly braces is part of the message text. Curly brace characters can be output by escaping them using double braces: `{{` or `}}`. 

Attributes are created with the `_attr` user-defined literal. The intermediate object that gets instantiated provides the assignment operator `=` for assigning a value to the attribute.

Attributes are associated with replacement fields in the format string by name or index, using names is strongly recommended. When using unnamed replacement fields, attributes map to replacement fields in the order they appear in the format string. 

It is allowed to have more attributes than replacement fields in a log statement. However, having fewer attributes than replacement fields is not allowed.

As shown above there is also an API taking both a format string and a message string. This is an API to help with the transition from text output to JSON output. JSON logs have no need for embedded replacement fields in the description, if written in a short and descriptive manner providing context for the attribute names. But a format string may still be needed to provide good JSON to human readable text conversion. See the JSON output format and style guide below for more information.

Both the format string and the message string must be compile time constants. This is to avoid dynamic attribute names in the log output and to be able to add compile time verification of log statements in the future.

##### Examples

```
LOGV2(1000, "Logging event, no replacement fields is OK");
```
```
const BSONObj& slowOperation = ...;
Milliseconds time = ...;
LOGV2(1001, "Operation {op} is slow, took: {duration}", "op"_attr = slowOperation, "duration"_attr = time);
```
```
LOGV2(1002, "Replication state change", "from"_attr = getOldState(), "to"_attr = getNewState());
```

### Log Component

To override the default component, a separate logging API can be used that takes a `LogOptions` structure:

`LOGV2_OPTIONS(options, message-string, attr0, ..., attrN);`

`LogOptions` can be constructed with a `LogComponent` to avoid verbosity in the log statement.

##### Examples

```
LOGV2_OPTIONS(1003, {LogComponent::kCommand}, "Log event to specified component");
```

### Log Severity

`LOGV2` is the logging macro for the default informational (0) severity. To log to different severities there are separate logging macros to be used, they all take paramaters like `LOGV2`:

* `LOGV2_WARNING`
* `LOGV2_ERROR`
* `LOGV2_FATAL`
* `LOGV2_FATAL_NOTRACE`
* `LOGV2_FATAL_CONTINUE`

There is also variations that take `LogOptions` if needed:

* `LOGV2_WARNING_OPTIONS`
* `LOGV2_ERROR_OPTIONS`
* `LOGV2_FATAL_OPTIONS`

Fatal level log statements using `LOGV2_FATAL` perform `fassert` after logging, using the provided ID as assert id. `LOGV2_FATAL_NOTRACE` perform `fassertNoTrace` and `LOGV2_FATAL_CONTINUE` does not `fassert` allowing for continued execution. `LOGV2_FATAL_CONTINUE` is meant to be used when a fatal error has occured but a different way of halting execution is desired such as `std::terminate` or `fassertFailedWithStatus`.

`LOGV2_FATAL_OPTIONS` performs `fassert` by default like `LOGV2_FATAL` but this can be changed by setting the `FatalMode` on the `LogOptions`.

Debug-level logging is slightly different where an additional parameter (as integer) required to indicate the desired debug level:

`LOGV2_DEBUG(ID, debug-level, format-string, attr0, ..., attrN);`

`LOGV2_DEBUG(ID, debug-level, format-string, message-string, attr0, ..., attrN);`

`LOGV2_DEBUG_OPTIONS(ID, debug-level, options, format-string, attr0, ..., attrN);`

`LOGV2_DEBUG_OPTIONS(ID, debug-level, options, format-string, message-string, attr0, ..., attrN);`

##### Examples

```
Status status = ...;
int remainingAttempts = ...;
LOGV2_ERROR(1004, "Initial sync failed. {remaining} attempts left. Reason: {reason}", "reason"_attr = status, "remaining"_attr = remainingAttempts);
```

### Log Tags

Log tags are replacing the Tee from the old log system as the way to indicate that the log should also be written to a `RamLog` (accessible with the `getLog` command).

Tags are added to a log statement with the options API similarly to how non-default components are specified by constructing a `LogOptions`.

Multiple tags can be attached to a log statement using the bitwise or operator `|`.

##### Examples

```
LOGV2_WARNING_OPTIONS(1005, {LogTag::kStartupWarnings}, "XFS filesystem is recommended with WiredTiger");
```

### Dynamic attributes

Sometimes there is a need to add attributes depending on runtime conditionals. To support this there is the `DynamicAttributes` class that has an `add` method to add named attributes one by one. This class is meant to be used when you have this specific requirement and is not the general logging API.

When finished, it is logged using the regular logging API but the `DynamicAttributes` instance is passed as the first attribute parameter. Mixing `_attr` literals with the `DynamicAttributes` is not supported.

When using the `DynamicAttributes` you need to be careful about parameter lifetimes. The `DynamicAttributes` binds attributes *by reference* and the reference must be valid when passing the `DynamicAttributes` to the log statement.

##### Examples

```
DynamicAttributes attrs;
attrs.add("str", "StringData value"_sd);
if (condition) {
    // getExtraInfo() returns a reference that is valid until the LOGV2 call below.
    // Be careful of functions returning by value
    attrs.add("extra", getExtraInfo());
}
LOGV2(1030, "Dynamic attributes", attrs);
```

# Type Support

### Built-in 

Many basic types have built in support:

* Boolean
* Integral types
  * Single char is logged as integer
* Enums
  * Logged as their underlying integral type
* Floating point types
  * long double is prohibited
* String types
  * std::string
  * StringData
  * const char*
* Duration types
  * Special formatting, see below
* BSON types
  * BSONObj
  * BSONArray
  * BSONElement
* BSON appendable types
  * `BSONObjBuilder::append` overload available
* boost::optional of any loggable type

### User defined types

To make a user defined type loggable it needs a serialization member function that the log system can bind to. At a minimum a type needs a stringification function. This would be used to produce text output and used as a fallback for JSON output.

In order to offer more its string representation in JSON, a type would need to supply a  structured serialization function.

The system will bind a stringification function and optionally a structured serialization function. The system binds to the serialization functions by looking for functions in the following priority order:

##### Structured serialization function signatures

Member functions:

1. `void serialize(BSONObjBuilder*) const`
2. `BSONObj toBSON() const`
3. `BSONArray toBSONArray() const`

Non-member functions:

4. `toBSON(const T& val)` (non-member function)

##### Stringification function signatures

Member functions:

1. `void serialize(fmt::memory_buffer&) const`
2. `std::string toString() const`

Non-member functions:

3. `toString(const T& val)` (non-member function) 

Enums will only try to bind a `toString(const T& val)` non-member function. If one is not available the enum value will be logged as its underlying integral type.

*NOTE: No `operator<<` overload is used even if available*

##### Examples

```
class UserDefinedType {
public:
    void serialize(BSONObjBuilder* builder) const {
        builder->append("str"_sd, _str);
        builder->append("int"_sd, _int);
    }

    void serialize(fmt::memory_buffer& buffer) const {
        fmt::format_to(buffer, "UserDefinedType: (str: {}, int: {})", _str, _int);
    }

private:
    std::string _str;
    int32_t _int;
};
```

### Container support

STL containers and data structures that have STL like interfaces are loggable as long as they contain loggable elements (built-in, user-defined or other containers).

#### Sequential containers

Sequential containers like `std::vector`, `std::deque` and `std::list` are loggable and the elements get formatted as JSON array in structured output.

#### Associative containers

Associative containers such as `std::map` and `stdx::unordered_map` loggable with the requirement that they key is of a string type. The structured format is a JSON object where the field names are the key.

#### Ranges

Ranges is loggable via helpers to indicate what type of range it is

* `seqLog(begin, end)`
* `mapLog(begin, end)`

seqLog indicates that it is a sequential range where the iterators point to loggable value directly.

mapLog indicates that it is a range coming from an associative container where the iterators point to a key-value pair.


##### Examples

```
std::array<int, 20> arrayOfInts = ...;
LOGV2(1010, "Log container directly: {values}", "values"_attr = arrayOfInts);
LOGV2(1011, "Log iterator range: {values}", "values"_attr = seqLog(arrayOfInts.begin(), arrayOfInts.end());
LOGV2(1012, "Log first five elements: {values}", "values"_attr = seqLog(arrayOfInts.data(), arrayOfInts.data() + 5);
``` 

```
StringMap<BSONObj> bsonMap = ...;
LOGV2(1013, "Log map directly: {values}", "values"_attr = bsonMap);
LOGV2(1014, "Log map iterator range: {values}", "values"_attr = mapLog(bsonMap.begin(), bsonMap.end());
``` 

### Duration types

Duration types have special formatting to match existing practices in the server code base. Their resulting format depends on the context they are logged.

When durations are formatted as JSON or BSON a unit suffix is added to the attribute name when building the field name. The value will be count of the duration as a number.

When logging containers with durations there is no attribute per duration instance that can have the suffix added. In this case durations are instead formatted as a BSON object.

##### Examples

`"duration"_attr = Milliseconds(10)`

Text | JSON/BSON
---- | ---------
`10 ms` | `"durationMillis": 10`

```
std::vector<Nanoseconds> nanos = {Nanoseconds(200), Nanoseconds(400)};
"samples"_attr = nanos
```

Text | JSON/BSON
---- | ---------
`(200 ns, 400 ns)` | `"samples": [{"durationNanos": 200}, {"durationNanos": 400}]`

# Attribute naming abstraction

The style guide contains recommendations for attribute naming in certain cases. To make abstraction of attribute naming possible a `logAttrs` function can be implemented as a friend function in a class with the following signature:

```
class AnyUserType {
public:
  friend auto logAttrs(const AnyUserType& instance) {
    return "name"_attr=instance;
  }

  BSONObj toBSON() const; // Type needs to be loggable
};
```

##### Examples
```
const AnyUserType& t = ...;
LOGV2(2001, "Log of user type", logAttr(t));
```

## Multiple attributes

In some cases a loggable type might be composed as a hierarchy in the C++ type system which would lead to a very verbose structured log output as every level in the hierarcy needs a name when outputted as JSON. The attribute naming abstraction system can also be used to collapse such hierarchies. Instead of making a type loggable it can instead return one or more attributes from its members by using `multipleAttrs` in `logAttrs` functions.

`multipleAttrs(...)` accepts attributes or instances of types with `logAttrs` functions implemented.

##### Examples

```
class NotALoggableType {
  std::string name;
  BSONObj data;

  friend auto logAttrs(const NotALoggableType& instance) {
    return logv2::multipleAttrs("name"_attr=instance.name, "data"_attr=instance.data);
  }
};

NotALoggableType t = ...;

// These two log statements would produce the same output (apart from different id)

LOGV2(2002, "Log of non-loggable type's members", logAttrs(t));

LOGV2(2003, "Log of non-loggable type's members", "name"_attr=t.name, "data"_attr=t.data);

```

## Handling temporary lifetime with multiple attributes

To avoid lifetime issues (log attributes bind their values by reference) it is recommended to **not** create attributes when using `multipleAttrs` unless attributes are created for members directly. If `logAttrs` or `""_attr=` is used inside a `logAttrs` function on the return of a function returning by value it will result in a dangling reference. The following example illustrates the problem

```
class SomeSubType {
public:
  BSONObj toBSON() const {...};

  friend auto logAttrs(const SomeSubType& sub) {
    return "subAttr"_attr=sub;
  }
};

class SomeType {
public:
  const std::string& name() const { return name_; }
  SomeSubType sub() const { return sub_; } // Returning by value!

  friend auto logAttrs(const SomeType& type) {
    // logAttrs(type.sub()) below will contain a danling reference!
    return logv2::multipleAttrs("name"_attr=type.name(), logAttrs(type.sub()));
  }
private:
  SomeSubType sub_;
  std::string name_;
};
``` 
The better implementation would be to let the log system control the lifetime by passing the instance to `multipleAttrs` without creating the attribute. The log system will detect that it is not an attribute and will attempt to create attributes by calling `logAttrs`:
```
friend auto logAttrs(const SomeType& type) {
  return logv2::multipleAttrs("name"_attr=type.name(), type.sub());
}
```

# Additional features

## Combining uassert with log statement

Code that emits a high severity log statement may also need to emit a `uassert` after the log. There is the `UserAssertAfterLog` logging option that allows you to re-use the log statement to do the formatting required for the `uassert`. The assertion id can be either the logging ID by passing `UserAssertAfterLog` with no arguments or the assertion id can set by constructing `UserAssertAfterLog` with an `ErrorCodes::Error`. 

The assertion reason string will be a plain text formatted log (replacement fields filled in format-string). If replacement fields are not provided in the message string, attribute values will be missing from the assertion message.


##### Examples
```
LOGV2_ERROR_OPTIONS(1050000, {UserAssertAfterLog()}, "Assertion after log");
```
Would emit a `uassert` after performing the log that is equivalent to:
```
uasserted(1050000, "Assertion after log");
```
Using a named error code:
```
LOGV2_ERROR_OPTIONS(1050, {UserAssertAfterLog(ErrorCodes::DataCorruptionDetected)}, "Data corruption detected for {recordId}, "recordId"_attr=RecordId(123456));
```
Would emit a `uassert` after performing the log that is equivalent to:
```
uasserted(ErrorCodes::DataCorruptionDetected, "Data corruption detected for RecordId(123456)");
```

## Unstructured logging for local development

To make it easier to use the log system for tracing in local development, there is a special API that does not use IDs or attribute names: 

`logd(format-string, value0, ..., valueN);`

It formats the string using libfmt similarly to what `fmt::format(format-string, value0, ..., valueN)` would produce but using the regular log system type support on how types are made loggable. The formatted string is logged as the `msg` field in the JSON output, with no `attr` subobject.

When using `logd` the log will emitted with standard severity and the default component.

A difference from regular logging, `logd` is allowed to be used in header files by including `logv2/log_debug.h`.

Unstructured logging is not allowed to be used in code committed to master, there is a lint check to validate this. It is however allowed to be used in Evergreen patch builds.

##### Examples

```
UserDefinedType t; // Defined in previous example
logd("this is a debug log, value 1: {} and value 2: {}", 1, t);
```


# JSON output format

Produces structured logs of the [Relaxed Extended JSON 2.0.0](https://github.com/mongodb/specifications/blob/master/source/extended-json.rst) format. Below is an example of a log statement in C++ and a pretty-printed JSON output:

```
BSONObjBuilder builder;
builder.append("first"_sd, 1);
builder.append("second"_sd, "str");

std::vector<int> vec = {1, 2, 3};

LOGV2_ERROR(1020, "Example (b: {bson}), (vec: {vector})", 
            "bson"_attr = builder.obj(), 
            "vector"_attr = vec,
            "optional"_attr = boost::none);
```

```
{  
    "t": {  
        "$date": "2020-01-06T19:10:54.246Z"
    },
    "s": "E",
    "c": "NETWORK",
    "ctx": "conn1",
    "id": 23453,
    "msg": "Example (b: {bson}), (vec: {vector})",
    "attr": {  
        "bson": {
            "first": 1,
            "second": "str"
        },
        "vector": [1, 2, 3],
        "optional": null
    }
}
```

## BSON

The BSON formatter is an internal formatter that may or may not be removed. It produces BSON documents close to the JSON document above. Due to the lack of unsigned integer types in BSON, logged C++ types are handled according to the table below for BSON:

C++ Type | BSON Type
-------- | ---------
char | int32 (0x10)
signed char | int32 (0x10)
unsigned char | int32 (0x10)
short | int32 (0x10)
unsigned short | int32 (0x10)
int | int32 (0x10)
unsigned int | int64 (0x12)
long | int64 (0x12)
unsigned long | int64 (0x12)
long long | int64 (0x12)
unsigned long long | int64 (0x12)

# FAQ

### Why are we doing this?

Structured logging brings __significant__ potential for log analysis to the codebase that isn't present with earlier logging facilities. This is an improvement that facilitates many future improvements.

Not only that, logv2 removes most parsing/post-processing concerns for automated downstream consumption of logs.

### Why are we doing this so fast?

Maintaining multiple output formats for even a single version would present serious overhead for both support and engineering. This dual support would last for years given the adoption curve, and effectively creates __four__ formats (old, new, new-old, and newer).

By making a full cutover in a single release, we are in a much better position.

### Why shouldn't we use formatting strings and substitution for new log lines?

Human readability suffers significantly when `attr` field names are included both in the `attr` subdocument and within `msg` string. This is a powerful feature that we don't want to exclude entirely, but it makes sense to lean on it only when absolutely necessary.
