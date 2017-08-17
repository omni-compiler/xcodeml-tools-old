/**
 * \file F-type-attr-tbl.c
 */

#include "F-front.h"

struct type_attr_check type_attr_checker[] = {
    {
        TYPE_ATTR_PARAMETER,
        (TYPE_ATTR_PARAMETER|TYPE_ATTR_PUBLIC|TYPE_ATTR_PRIVATE|TYPE_ATTR_SEQUENCE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE|TYPE_ATTR_VALUE),
        "parameter",
    },
    {
        TYPE_ATTR_ALLOCATABLE,
        (TYPE_ATTR_ALLOCATABLE|TYPE_ATTR_SAVE|TYPE_ATTR_TARGET|TYPE_ATTR_PUBLIC|
         TYPE_ATTR_PRIVATE|TYPE_ATTR_SEQUENCE|TYPE_ATTR_RECURSIVE|
         TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE|TYPE_ATTR_CLASS|TYPE_ATTR_CONTIGUOUS|
         TYPE_ATTR_INTENT_OUT|TYPE_ATTR_INTENT_INOUT),
        "allocatable",
    },
    {
        TYPE_ATTR_EXTERNAL,
        (TYPE_ATTR_EXTERNAL|TYPE_ATTR_OPTIONAL|TYPE_ATTR_PUBLIC|TYPE_ATTR_PRIVATE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE|TYPE_ATTR_VALUE|TYPE_ATTR_CONTIGUOUS),
        "external",
    },
    {
        TYPE_ATTR_INTRINSIC,
        (TYPE_ATTR_INTRINSIC|TYPE_ATTR_PUBLIC|TYPE_ATTR_PRIVATE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE|TYPE_ATTR_VALUE|TYPE_ATTR_CONTIGUOUS),
        "intrinsic",
    },
    {
        TYPE_ATTR_OPTIONAL,
        (TYPE_ATTR_OPTIONAL|TYPE_ATTR_EXTERNAL|TYPE_ATTR_INTENT_IN|TYPE_ATTR_INTENT_OUT|
                                TYPE_ATTR_INTENT_INOUT|TYPE_ATTR_POINTER|TYPE_ATTR_TARGET|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE|TYPE_ATTR_VALUE|TYPE_ATTR_CONTIGUOUS),
        "optional",
    },
    {
        TYPE_ATTR_POINTER,
        (TYPE_ATTR_OPTIONAL|TYPE_ATTR_POINTER|TYPE_ATTR_PUBLIC|TYPE_ATTR_PRIVATE|TYPE_ATTR_SEQUENCE|TYPE_ATTR_SAVE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_INTENT_IN|TYPE_ATTR_INTENT_OUT|TYPE_ATTR_INTENT_INOUT|TYPE_ATTR_VOLATILE|TYPE_ATTR_CLASS|TYPE_ATTR_PROCEDURE|TYPE_ATTR_CONTIGUOUS),
        "pointer",
    },
    {
        TYPE_ATTR_SAVE,
        (TYPE_ATTR_ALLOCATABLE|TYPE_ATTR_POINTER|TYPE_ATTR_PUBLIC|
         TYPE_ATTR_PRIVATE|TYPE_ATTR_SAVE|TYPE_ATTR_TARGET|TYPE_ATTR_SEQUENCE|
         TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE|TYPE_ATTR_VALUE|
         TYPE_ATTR_CONTIGUOUS|TYPE_ATTR_BIND
        ),
        "save",
    },
    {
        TYPE_ATTR_TARGET,
        (TYPE_ATTR_ALLOCATABLE|TYPE_ATTR_INTENT_IN|TYPE_ATTR_INTENT_OUT|
         TYPE_ATTR_INTENT_INOUT|TYPE_ATTR_OPTIONAL|TYPE_ATTR_PUBLIC|
         TYPE_ATTR_PRIVATE|TYPE_ATTR_SEQUENCE|TYPE_ATTR_SAVE|
         TYPE_ATTR_TARGET|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_VOLATILE|
         TYPE_ATTR_VALUE|TYPE_ATTR_CONTIGUOUS|TYPE_ATTR_BIND|TYPE_ATTR_CLASS
        ),
        "target",
    },
    {
        TYPE_ATTR_PUBLIC,
        (TYPE_ATTR_ALLOCATABLE|TYPE_ATTR_EXTERNAL|TYPE_ATTR_INTRINSIC|
         TYPE_ATTR_PARAMETER|TYPE_ATTR_POINTER|TYPE_ATTR_PUBLIC|
         TYPE_ATTR_INTERNAL_PRIVATE|TYPE_ATTR_SAVE|TYPE_ATTR_TARGET|
         TYPE_ATTR_SEQUENCE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|
         TYPE_ATTR_VOLATILE|TYPE_ATTR_VALUE|TYPE_ATTR_CONTIGUOUS|
         TYPE_ATTR_ABSTRACT|TYPE_ATTR_BIND
        ),
        "public",
    },
    {
        TYPE_ATTR_PRIVATE,
        (TYPE_ATTR_ALLOCATABLE|TYPE_ATTR_EXTERNAL|TYPE_ATTR_INTRINSIC|
         TYPE_ATTR_PARAMETER|TYPE_ATTR_POINTER|TYPE_ATTR_PRIVATE|
         TYPE_ATTR_INTERNAL_PRIVATE|TYPE_ATTR_SAVE|TYPE_ATTR_TARGET|
         TYPE_ATTR_SEQUENCE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|
         TYPE_ATTR_VOLATILE|TYPE_ATTR_VALUE|TYPE_ATTR_CONTIGUOUS|
         TYPE_ATTR_ABSTRACT|TYPE_ATTR_BIND
        ),
        "private",
    },
    {
        TYPE_ATTR_INTENT_IN,
        (TYPE_ATTR_INTENT_IN|TYPE_ATTR_OPTIONAL|TYPE_ATTR_TARGET|
         TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_POINTER|TYPE_ATTR_CLASS|
         TYPE_ATTR_VALUE|TYPE_ATTR_CONTIGUOUS),
        "intent in",
    },
    {
        TYPE_ATTR_INTENT_OUT,
        (TYPE_ATTR_INTENT_OUT|TYPE_ATTR_OPTIONAL|TYPE_ATTR_TARGET|
         TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_POINTER|
         TYPE_ATTR_VOLATILE|TYPE_ATTR_CLASS|TYPE_ATTR_CONTIGUOUS|
	 TYPE_ATTR_ALLOCATABLE
        ),
        "intent out",
    },
    {
        TYPE_ATTR_VOLATILE,
        (TYPE_ATTR_VOLATILE|
         TYPE_ATTR_PARAMETER|TYPE_ATTR_ALLOCATABLE|TYPE_ATTR_EXTERNAL|
         TYPE_ATTR_INTRINSIC|TYPE_ATTR_OPTIONAL|TYPE_ATTR_POINTER|
         TYPE_ATTR_SAVE|TYPE_ATTR_TARGET|TYPE_ATTR_PUBLIC|TYPE_ATTR_PRIVATE|
         TYPE_ATTR_INTENT_OUT|TYPE_ATTR_SEQUENCE|TYPE_ATTR_INTERNAL_PRIVATE|
         TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|TYPE_ATTR_ELEMENTAL|
         TYPE_ATTR_PROTECTED|TYPE_ATTR_VALUE|TYPE_ATTR_CONTIGUOUS|
         TYPE_ATTR_BIND
        ),
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
    {
        TYPE_ATTR_VALUE,
        (TYPE_ATTR_PARAMETER|TYPE_ATTR_EXTERNAL|TYPE_ATTR_INTRINSIC|
         TYPE_ATTR_OPTIONAL|TYPE_ATTR_SAVE|TYPE_ATTR_TARGET|TYPE_ATTR_PUBLIC|
         TYPE_ATTR_PRIVATE|TYPE_ATTR_INTENT_IN|TYPE_ATTR_SEQUENCE|
         TYPE_ATTR_INTERNAL_PRIVATE|TYPE_ATTR_RECURSIVE|TYPE_ATTR_PURE|
         TYPE_ATTR_ELEMENTAL|TYPE_ATTR_PROTECTED|TYPE_ATTR_KIND|TYPE_ATTR_LEN|
         TYPE_ATTR_CLASS|TYPE_ATTR_BIND|TYPE_ATTR_VALUE|TYPE_ATTR_MODULE|
         TYPE_ATTR_PROCEDURE),
        "value",
    },
    {  /* terminater, DO NOT remove */
        0,0,NULL
    }
};
