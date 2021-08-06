project('libresolver', ['cpp'])

################
# dependencies #
################

libcapstone_dep = dependency('capstone', required: true)
libkeystone_dep = dependency('keystone', required: false)

###############
# libresolver #
###############

libresolver_includes = [
    include_directories('include')
]

libresolver_sources = [
    'source/cases/case_1.cpp',
    'source/cases/case_2.cpp',
    'source/cases/case_3.cpp',
    'source/cases/case_4.cpp',
    'source/cases/case_5.cpp',
    'source/cases/case_6.cpp',
    'source/cases/case_7.cpp',
    'source/cases/case_8.cpp',
    'source/cases/case_9.cpp',
    'source/cases/case_10.cpp',
    'source/cases/case_11.cpp',
    'source/cases/case_12.cpp',
    'source/cases/case_13.cpp',
    'source/cases/case_14.cpp',
    'source/cases/case_15.cpp',
    'source/cases/case_16.cpp',
    'source/cases/case_17.cpp',
    'source/cases/case_18.cpp',
    'source/cases/case_19.cpp',
    'source/cases/case_20.cpp',
    'source/cases/case_21.cpp',
    'source/cases/case_22.cpp',
    'source/cases/case_base.cpp',
    'source/instruction.cpp',
    'source/memory.cpp',
    'source/pattern/actions.cpp',
    'source/pattern/tree.cpp',
    'source/placeholder.cpp',
    'source/porcelain.cpp',
    'source/utils/registers.cpp',
    'source/watch_list.cpp',
]

libresolver_deps = [
    libcapstone_dep,
]

libresolver = static_library(
    'resolver', libresolver_sources,
    include_directories: libresolver_includes,
    dependencies: libresolver_deps,
    cpp_args: ['-DLIBRESOLVER_BUILD', '-std=c++20', '-g'],
)

libresolver_dep = declare_dependency(
    include_directories : libresolver_includes,
    link_with : libresolver,
)

###########
# headers #
###########

install_headers('include/libresolver/porcelain.hpp', subdir: 'libresolver')

##########
# format #
##########

libresolver_find_include = '`find @0@/include -type f -iname \'*.hpp\'`'.format(meson.source_root())
libresolver_find_source = '`find @0@/source -type f -iname \'*.cpp\'`'.format(meson.source_root())
libresolver_find_tests = '`find @0@/tests -type f -iname \'*.cpp\'`'.format(meson.source_root())
libresolver_find_format = libresolver_find_include + ' ' + libresolver_find_source + ' ' + libresolver_find_tests

run_target('format',
    command: [
        'sh', '-c',
        'clang-format --verbose -i ' + libresolver_find_format
    ],
)

#########
# check #
#########

tests_includes = [
    libresolver_includes,
    include_directories('tests')
]

tests_sources = [
    libresolver_sources,
    'tests/cases/case_1.cpp',
    'tests/cases/case_2.cpp',
    'tests/cases/case_3.cpp',
    'tests/cases/case_4.cpp',
    'tests/cases/case_5.cpp',
    'tests/cases/case_6.cpp',
    'tests/cases/case_7.cpp',
    'tests/cases/case_8.cpp',
    'tests/cases/case_9.cpp',
    'tests/cases/case_10.cpp',
    'tests/cases/case_11.cpp',
    'tests/cases/case_12.cpp',
    'tests/cases/case_13.cpp',
    'tests/cases/case_14.cpp',
    'tests/cases/case_15.cpp',
    'tests/cases/case_16.cpp',
    'tests/cases/case_17.cpp',
    'tests/cases/case_18.cpp',
    'tests/cases/case_19.cpp',
    'tests/cases/case_19.cpp',
    'tests/cases/case_20.cpp',
    'tests/cases/case_21.cpp',
    'tests/cases/case_22.cpp',
    'tests/context.cpp',
    'tests/main.cpp',
    'tests/memory.cpp',
    'tests/pattern/tree.cpp',
    'tests/watch_list.cpp',
    'tests/x86/engine.cpp',
]

tests_deps = [
    libcapstone_dep,
    libkeystone_dep,
]

if libkeystone_dep.found() and not(meson.is_subproject())
    tests_executable = executable(
        'tests', tests_sources,
        include_directories: tests_includes,
        dependencies: tests_deps,
        cpp_args: ['-DLIBRESOLVER_BUILD', '-std=c++20', '-g'],
    )

    run_target('check',
        command: [tests_executable],
        depends: [tests_executable],
    )
else
    run_target('check',
        command: ['echo', 'Tests are disabled. Either keystone library is not found or we\'re in a subproject.'],
    )
endif