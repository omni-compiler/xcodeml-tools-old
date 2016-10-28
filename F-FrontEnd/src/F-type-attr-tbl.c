/**
 * \file F-type-attr-tbl.c
 */

#include "F-front.h"

struct type_attr_check type_attr_checker[] = {
    {
        TYPE_ATTR_PARAMETER,
        (TYPE_ATTR_PARAMETER|TYPE_ATTR_PUBLIC|TYPE_ATTR_PRIVATE|TYPE_ATTR_SEQUENCE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE),
        "parameter",
    },
    {
        TYPE_ATTR_ALLOCATABLE,
        (TYPE_ATTR_ALLOCATABLE|TYPE_ATTR_SAVE|TYPE_ATTR_TARGET|TYPE_ATTR_PUBLIC|TYPE_ATTR_PRIVATE|TYPE_ATTR_SEQUENCE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE|TYPE_ATTR_CLASS),
        "allocatable",
    },
    {
        TYPE_ATTR_EXTERNAL,
        (TYPE_ATTR_EXTERNAL|TYPE_ATTR_OPTIONAL|TYPE_ATTR_PUBLIC|TYPE_ATTR_PRIVATE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE),
        "external",
    },
    {
        TYPE_ATTR_INTRINSIC,
        (TYPE_ATTR_INTRINSIC|TYPE_ATTR_PUBLIC|TYPE_ATTR_PRIVATE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE),
        "intrinsic",
    },
    {
        TYPE_ATTR_OPTIONAL,
        (TYPE_ATTR_OPTIONAL|TYPE_ATTR_EXTERNAL|TYPE_ATTR_INTENT_IN|TYPE_ATTR_INTENT_OUT|
                                TYPE_ATTR_INTENT_INOUT|TYPE_ATTR_POINTER|TYPE_ATTR_TARGET|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE),
        "optional",
    },
    {
        TYPE_ATTR_POINTER,
        (TYPE_ATTR_OPTIONAL|TYPE_ATTR_POINTER|TYPE_ATTR_PUBLIC|TYPE_ATTR_PRIVATE|TYPE_ATTR_SEQUENCE|TYPE_ATTR_SAVE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_INTENT_IN|TYPE_ATTR_INTENT_OUT|TYPE_ATTR_INTENT_INOUT|TYPE_ATTR_VOLATILE|TYPE_ATTR_CLASS),
        "pointer",
    },
    {
        TYPE_ATTR_SAVE,
        (TYPE_ATTR_ALLOCATABLE|TYPE_ATTR_POINTER|TYPE_ATTR_PUBLIC|TYPE_ATTR_PRIVATE|
                                TYPE_ATTR_SAVE|TYPE_ATTR_TARGET|TYPE_ATTR_SEQUENCE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE),
        "save",
    },
    {
        TYPE_ATTR_TARGET,
        (TYPE_ATTR_ALLOCATABLE|TYPE_ATTR_INTENT_IN|TYPE_ATTR_INTENT_OUT|TYPE_ATTR_INTENT_INOUT|
                TYPE_ATTR_OPTIONAL|TYPE_ATTR_PUBLIC|TYPE_ATTR_PRIVATE|TYPE_ATTR_SEQUENCE|TYPE_ATTR_SAVE|TYPE_ATTR_TARGET|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE),
        "target",
    },
    {
        TYPE_ATTR_PUBLIC,
        (TYPE_ATTR_ALLOCATABLE|TYPE_ATTR_EXTERNAL|TYPE_ATTR_INTRINSIC|TYPE_ATTR_PARAMETER|
                                TYPE_ATTR_POINTER|TYPE_ATTR_PUBLIC|TYPE_ATTR_INTERNAL_PRIVATE|TYPE_ATTR_SAVE|TYPE_ATTR_TARGET|TYPE_ATTR_SEQUENCE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE),
        "public",
    },
    {
        TYPE_ATTR_PRIVATE,
        (TYPE_ATTR_ALLOCATABLE|TYPE_ATTR_EXTERNAL|TYPE_ATTR_INTRINSIC|TYPE_ATTR_PARAMETER|
                                TYPE_ATTR_POINTER|TYPE_ATTR_PRIVATE|TYPE_ATTR_INTERNAL_PRIVATE|TYPE_ATTR_SAVE|TYPE_ATTR_TARGET|TYPE_ATTR_SEQUENCE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE),
        "private",
    },
    {
        TYPE_ATTR_INTENT_IN,
        (TYPE_ATTR_INTENT_IN|TYPE_ATTR_OPTIONAL|TYPE_ATTR_TARGET|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_POINTER),
        "intent in",
    },
    {
        TYPE_ATTR_INTENT_OUT,
        (TYPE_ATTR_INTENT_OUT|TYPE_ATTR_OPTIONAL|TYPE_ATTR_TARGET|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_POINTER|TYPE_ATTR_VOLATILE),
        "intent out",
    },
    {
        TYPE_ATTR_INTENT_INOUT,
        (TYPE_ATTR_INTENT_INOUT|TYPE_ATTR_OPTIONAL|TYPE_ATTR_TARGET|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_POINTER),
        "intent out",
    },
    {
        TYPE_ATTR_VOLATILE,
        (TYPE_ATTR_VOLATILE|
         TYPE_ATTR_PARAMETER|TYPE_ATTR_ALLOCATABLE|TYPE_ATTR_EXTERNAL|
         TYPE_ATTR_INTRINSIC|TYPE_ATTR_OPTIONAL|TYPE_ATTR_POINTER|TYPE_ATTR_SAVE|
         TYPE_ATTR_TARGET|TYPE_ATTR_PUBLIC|TYPE_ATTR_PRIVATE|TYPE_ATTR_INTENT_OUT|
         TYPE_ATTR_SEQUENCE|TYPE_ATTR_INTERNAL_PRIVATE|TYPE_ATTR_RECURSIVE|
         TYPE_ATTR_PURE|TYPE_ATTR_ELEMENTAL|TYPE_ATTR_PROTECTED),
        "volatile",
    },
    {
        TYPE_ATTR_KIND,
        (TYPE_ATTR_KIND),
        "KIND",
    },
    {
        TYPE_ATTR_LEN,
        (TYPE_ATTR_LEN),
        "LEN",
    },
    {  /* terminater, DO NOT remove */
        0,0,NULL
    }
};
