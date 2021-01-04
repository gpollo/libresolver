project('libresolver', ['cpp'])

################
# dependencies #
################

libcapstone_dep = dependency('capstone', required: true)

###############
# libresolver #
###############

libresolver_include = include_directories('include')

libresolver_find_include = '`find include -type f -iname \'*.hpp\'`'
libresolver_find_source = '`find source -type f -iname \'*.cpp\'`'
libresolver_find_tests = '`find tests -type f -iname \'*.cpp\'`'
libresolver_find_format = libresolver_find_include + ' ' + libresolver_find_source + ' ' + libresolver_find_tests

run_target('format',
    command: [
        'sh', '-c',
        'clang-format --verbose -i ' + libresolver_find_format
    ],
)

#########
# tests #
#########

tests_include = include_directories('tests')

tests_source = [
    'source/cases/case_1.cpp',
    'source/cases/case_2.cpp',
    'source/cases/case_3.cpp',
    'source/cases/case_4.cpp',
    'source/cases/case_5.cpp',
    'source/cases/case_6.cpp',
    'source/cases/case_7.cpp',
    'source/pattern/tree.cpp',
    'source/placeholder.cpp',
    'source/utils/registers.cpp',
    'source/watch_list.cpp',
    'tests/cases/case_1.cpp',
    'tests/cases/case_2.cpp',
    'tests/cases/case_3.cpp',
    'tests/cases/case_4.cpp',
    'tests/cases/case_5.cpp',
    'tests/cases/case_6.cpp',
    'tests/cases/case_7.cpp',
    'tests/context.cpp',
    'tests/main.cpp',
    'tests/pattern/tree.cpp',
    'tests/watch_list.cpp',
]

tests_deps = [
    libcapstone_dep,
]

tests_executable = executable(
    'tests', [tests_source],
    include_directories: [tests_include, libresolver_include],
    dependencies: tests_deps,
    cpp_args: ['-std=c++20', '-g'],
)

run_target('check',
    command: [tests_executable],
    depends: [tests_executable],
)