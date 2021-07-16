
cmake_minimum_required(VERSION 3.15)
project(ft_printf)
set(CMAKE_C_STANDARD 99)
set(SOURCE_FILES
        src/analysis.c
        src/create_string.c
        src/decimal.c
        src/float.c
        src/float2.c
        src/float3.c
        src/float4.c
        src/free.c
        src/ft_printf.c
        src/libfunctions.c
        src/oct.c
        src/parsing.c
        src/print_color.c
        src/printnum.c
        src/putchar_str.c
        src/services_itoa.c
        src/string_char.c
        )
set(INCLUDE_FILES
        includes/ft_printf.h
        includes/header_pf.h
        )
add_executable(ft_printf ${SOURCE_FILES} ${INCLUDE_FILES})

















