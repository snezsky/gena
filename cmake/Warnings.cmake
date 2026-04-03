function(__gena_target_enable_warnings TARGET_NAME)

    if (NOT GENA_ENABLE_WARNINGS)
        return()
    endif()
    
    set_target_properties(
        ${TARGET_NAME} PROPERTIES
        COMPILE_WARNING_AS_ERROR ON)
    
    target_compile_options(
        ${TARGET_NAME} PRIVATE
    
        $<$<CXX_COMPILER_ID:MSVC>:
            /W4          # Baseline reasonable warnings
            /w14242      # conversion from 'type1' to 'type2', possible loss of data
            /w14254      # conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
            /w14263      # member function does not override any base class virtual member function
            /w14265      # class has virtual functions, but destructor is not virtual
            /w14287      # unsigned/negative constant mismatch
            /we4289      # loop control variable declared in the for-loop is used outside the for-loop scope
            /w14296      # expression is always 'boolean_value'
            /w14311      # pointer truncation from 'type1' to 'type2'
            /w14545      # expression before comma evaluates to a function which is missing an argument list
            /w14546      # function call before comma missing argument list
            /w14547      # operator before comma has no effect; expected operator with side-effect
            /w14549      # operator before comma has no effect; did you intend 'operator'?
            /w14555      # expression has no effect; expected expression with side-effect
            /w14619      # pragma warning: there is no warning number 'number'
            /w14640      # warning on thread un-safe static member initialization
            /w14826      # conversion from 'type1' to 'type2' is sign-extended
            /w14905      # wide string literal cast to 'LPSTR'
            /w14906      # string literal cast to 'LPWSTR'
            /w14928      # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
            /permissive- # standards conformance mode for MSVC compiler.
        >
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
            -Wall
            -Wextra                # reasonable and standard
            -Wshadow               # warn the user if a variable declaration shadows one from a parent context
            -Wnon-virtual-dtor     # warn the user if a class with virtual functions has a non-virtual destructor
            -Wold-style-cast       # warn for c-style casts
            -Wcast-align           # warn for potential performance problem casts
            -Wunused               # warn on anything being unused
            -Woverloaded-virtual   # warn if you overload (not override) a virtual function
            -Wpedantic             # warn if non-standard C++ is used
            -Wconversion           # warn on type conversions that may lose data
            -Wsign-conversion      # warn on sign conversions
            -Wnull-dereference     # warn if a null dereference is detected
            -Wdouble-promotion     # warn if float is implicit promoted to double
            -Wformat=2             # warn on security issues around functions that format output (ie printf)
            -Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation
        >
        $<$<CXX_COMPILER_ID:GNU>:
            -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
            -Wduplicated-cond        # warn if if / else chain has duplicated conditions
            -Wduplicated-branches    # warn if if / else branches have duplicated code
            -Wlogical-op             # warn about logical operations being used where bitwise were probably wanted
            -Wuseless-cast           # warn if you perform a cast to the same type
            -Wsuggest-override>)     # warn if an overridden member function is not marked 'override' or 'final'
        
endfunction()