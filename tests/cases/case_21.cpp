#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_21 {

TEST_CASE("pattern tree should match pattern case 21", "[libresolver::cases::case_21]") {
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
            "\x83\xe8\x14"                 /* sub    $20,%eax */
            "\x48\x98"                     /* cltq */
            "\x0f\xb6\x04\x07"             /* movzbl (%rdi,%rax,1),%eax */
            "\x48\x8d\x3d\xe8\x03\x00\x00" /* lea    1000(%rip),%rdi */
            "\x48\x8b\x04\xc7"             /* mov    (%rdi,%rax,8),%rax */
            "\xff\xe0";                    /* jmpq   *%rax */
        const size_t bytes_len = sizeof(bytes) - 1;

        memory.u8_ = {{2001, 1},  {2002, 2},  {2003, 3},  {2004, 4},  {2005, 5},  {2006, 6},  {2007, 7},  {2008, 8},
                      {2009, 9},  {2010, 10}, {2011, 11}, {2012, 12}, {2013, 13}, {2014, 14}, {2015, 15}, {2016, 16},
                      {2017, 17}, {2018, 18}, {2019, 19}, {2020, 20}, {2021, 21}, {2022, 22}, {2023, 23}, {2024, 24},
                      {2025, 25}, {2026, 26}, {2027, 27}, {2028, 28}, {2029, 29}, {2030, 30}, {2031, 31}, {2032, 32},
                      {2033, 33}, {2034, 34}, {2035, 35}, {2036, 36}, {2037, 37}, {2038, 38}, {2039, 39}, {2040, 40},
                      {2041, 41}, {2042, 42}, {2043, 43}, {2044, 44}, {2045, 45}, {2046, 46}, {2047, 47}, {2048, 48},
                      {2049, 49}, {2050, 50}, {2051, 51}};

        memory.u64_ = {{1045, 1010}, {1053, 1020}, {1061, 1030}, {1069, 1040}, {1077, 1050}, {1085, 1060}, {1093, 1070},
                       {1101, 1080}, {1109, 1090}, {1117, 1100}, {1125, 1110}, {1133, 1120}, {1141, 1130}, {1149, 1140},
                       {1157, 1150}, {1165, 1160}, {1173, 1170}, {1181, 1180}, {1189, 1190}, {1197, 1200}, {1205, 1210},
                       {1213, 1220}, {1221, 1230}, {1229, 1240}, {1237, 1250}, {1245, 1260}, {1253, 1270}, {1261, 1280},
                       {1269, 1290}, {1277, 1300}, {1285, 1310}, {1293, 1320}, {1301, 1330}, {1309, 1340}, {1317, 1350},
                       {1325, 1360}, {1333, 1370}, {1341, 1380}, {1349, 1390}, {1357, 1400}, {1365, 1410}, {1373, 1420},
                       {1381, 1430}, {1389, 1440}, {1397, 1450}, {1405, 1460}, {1413, 1470}, {1421, 1480}, {1429, 1490},
                       {1437, 1500}, {1445, 1510}};

        auto instructions = engine.disassemble(bytes, bytes_len, 0, 11);
        auto matches      = matcher.match_instructions(instructions->get());
        auto values       = matches[0]->evaluate(context, memory);

        REQUIRE(matches.size() == 1);
        REQUIRE(context.get(reg::REG_1).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RAX);
        REQUIRE(context.get(reg::REG_2).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RDI);
        REQUIRE(context.get(reg::REG_3).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RBX);
        REQUIRE(context.get(reg::REG_4).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_5).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_6).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_7).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_8).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_9).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(value::VALUE_1).value_or(-1) == 0);
        REQUIRE(context.get(value::VALUE_2).value_or(-1) == 8);
        REQUIRE(context.get(value::VALUE_3).value_or(-1) == 1000);
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 0);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == 1);
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == 20);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == 2000);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == 18446744073709547249);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == 30);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == static_cast<uint64_t>(-20));
        REQUIRE(context.get_rip(0).value_or(-1) == 37);
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

}  // namespace tests::cases::case_21
