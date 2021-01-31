#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_20 {

TEST_CASE("pattern tree should match pattern case 20", "[libresolver::cases::case_20]") {
    x86::engine engine;
    dummy::memory memory;

    libresolver::pattern::tree tree = tests::pattern::create_tree();
    libresolver::pattern::matcher matcher(tree);
    const libresolver::context& context = matcher.get_context();

    SECTION("example 1") {
        const unsigned char bytes[] =
            "\x8d\x43\xec"                 /* lea    -20(%rbx),%eax */
            "\x3c\x1e"                     /* cmp    $30,%al */
            "\x0f\x87\xe6\xee\xff\xff"     /* ja     10 */
            "\x0f\xbe\xc3"                 /* movsbl %bl,%eax */
            "\x48\x8d\x3d\xd0\x07\x00\x00" /* lea    2000(%rip),%rdi */
            "\x48\x8d\x0d\xe8\x03\x00\x00" /* lea    1000(%rip),%rcx */
            "\x83\xe8\x14"                 /* sub    $20,%eax */
            "\x48\x98"                     /* cltq */
            "\x0f\xb6\x04\x07"             /* movzbl (%rdi,%rax,1),%eax */
            "\x48\x8b\x04\xc1"             /* mov    (%rcx,%rax,8),%rax */
            "\xff\xe0";                    /* jmpq   *%rax */
        const size_t bytes_len = sizeof(bytes) - 1;

        memory.u8_ = {{2001, 1},  {2002, 2},  {2003, 3},  {2004, 4},  {2005, 5},  {2006, 6},  {2007, 7},  {2008, 8},
                      {2009, 9},  {2010, 10}, {2011, 11}, {2012, 12}, {2013, 13}, {2014, 14}, {2015, 15}, {2016, 16},
                      {2017, 17}, {2018, 18}, {2019, 19}, {2020, 20}, {2021, 21}, {2022, 22}, {2023, 23}, {2024, 24},
                      {2025, 25}, {2026, 26}, {2027, 27}, {2028, 28}, {2029, 29}, {2030, 30}, {2031, 31}, {2032, 32},
                      {2033, 33}, {2034, 34}, {2035, 35}, {2036, 36}, {2037, 37}, {2038, 38}, {2039, 39}, {2040, 40},
                      {2041, 41}, {2042, 42}, {2043, 43}, {2044, 44}, {2045, 45}, {2046, 46}, {2047, 47}, {2048, 48},
                      {2049, 49}, {2050, 50}, {2051, 51}};

        memory.u64_ = {{1036, 1010}, {1044, 1020}, {1052, 1030}, {1060, 1040}, {1068, 1050}, {1076, 1060}, {1084, 1070},
                       {1092, 1080}, {1100, 1090}, {1108, 1100}, {1116, 1110}, {1124, 1120}, {1132, 1130}, {1140, 1140},
                       {1148, 1150}, {1156, 1160}, {1164, 1170}, {1172, 1180}, {1180, 1190}, {1188, 1200}, {1196, 1210},
                       {1204, 1220}, {1212, 1230}, {1220, 1240}, {1228, 1250}, {1236, 1260}, {1244, 1270}, {1252, 1280},
                       {1260, 1290}, {1268, 1300}, {1276, 1310}, {1284, 1320}, {1292, 1330}, {1300, 1340}, {1308, 1350},
                       {1316, 1360}, {1324, 1370}, {1332, 1380}, {1340, 1390}, {1348, 1400}, {1356, 1410}, {1364, 1420},
                       {1372, 1430}, {1380, 1440}, {1388, 1450}, {1396, 1460}, {1404, 1470}, {1412, 1480}, {1420, 1490},
                       {1428, 1500}, {1436, 1510}};

        auto instructions = engine.disassemble(bytes, bytes_len, 0, 11);
        auto matches      = matcher.match_instructions(instructions->get());
        auto values       = matches[0]->evaluate(context, memory);

        REQUIRE(matches.size() == 1);
        REQUIRE(context.get(reg::REG_1).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RAX);
        REQUIRE(context.get(reg::REG_2).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RCX);
        REQUIRE(context.get(reg::REG_3).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RDI);
        REQUIRE(context.get(reg::REG_4).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RBX);
        REQUIRE(context.get(reg::REG_5).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_6).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_7).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_8).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_9).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(value::VALUE_1).value_or(-1) == 0);
        REQUIRE(context.get(value::VALUE_2).value_or(-1) == 8);
        REQUIRE(context.get(value::VALUE_3).value_or(-1) == 0);
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 1);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == 20);
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == 1000);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == 2000);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == 18446744073709547249);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == 30);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == static_cast<uint64_t>(-20));
        REQUIRE(context.get_rip(0).value_or(-1) == 28);
        REQUIRE(context.get_rip(1).value_or(-1) == 21);
        REQUIRE(values.size() == 51);
        REQUIRE(values.contains(1010));
        REQUIRE(values.contains(1020));
        REQUIRE(values.contains(1030));
        REQUIRE(values.contains(1040));
        REQUIRE(values.contains(1050));
        REQUIRE(values.contains(1060));
        REQUIRE(values.contains(1070));
        REQUIRE(values.contains(1080));
        REQUIRE(values.contains(1090));
        REQUIRE(values.contains(1100));
        REQUIRE(values.contains(1110));
        REQUIRE(values.contains(1120));
        REQUIRE(values.contains(1130));
        REQUIRE(values.contains(1140));
        REQUIRE(values.contains(1150));
        REQUIRE(values.contains(1160));
        REQUIRE(values.contains(1170));
        REQUIRE(values.contains(1180));
        REQUIRE(values.contains(1190));
        REQUIRE(values.contains(1200));
        REQUIRE(values.contains(1210));
        REQUIRE(values.contains(1220));
        REQUIRE(values.contains(1230));
        REQUIRE(values.contains(1240));
        REQUIRE(values.contains(1250));
        REQUIRE(values.contains(1260));
        REQUIRE(values.contains(1270));
        REQUIRE(values.contains(1280));
        REQUIRE(values.contains(1290));
        REQUIRE(values.contains(1300));
        REQUIRE(values.contains(1310));
        REQUIRE(values.contains(1320));
        REQUIRE(values.contains(1330));
        REQUIRE(values.contains(1340));
        REQUIRE(values.contains(1350));
        REQUIRE(values.contains(1360));
        REQUIRE(values.contains(1370));
        REQUIRE(values.contains(1380));
        REQUIRE(values.contains(1390));
        REQUIRE(values.contains(1400));
        REQUIRE(values.contains(1410));
        REQUIRE(values.contains(1420));
        REQUIRE(values.contains(1430));
        REQUIRE(values.contains(1440));
        REQUIRE(values.contains(1450));
        REQUIRE(values.contains(1460));
        REQUIRE(values.contains(1470));
        REQUIRE(values.contains(1480));
        REQUIRE(values.contains(1490));
        REQUIRE(values.contains(1500));
        REQUIRE(values.contains(1510));
    }
}

} /* namespace tests::cases::case_20 */
