#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_20 {

TEST_CASE("match-case-20") {
    x86::engine engine;
    dummy::memory memory;

    libresolver::pattern::tree tree = tests::pattern::create_tree();
    libresolver::pattern::matcher matcher(tree);
    const libresolver::context& context = matcher.get_context();

    SECTION("example 1") {
        memory.u8_ = {{1997, 1},  {1998, 2},  {1999, 3},  {2000, 4},  {2001, 5},  {2002, 6},  {2003, 7},  {2004, 8},
                      {2005, 9},  {2006, 10}, {2007, 11}, {2008, 12}, {2009, 13}, {2010, 14}, {2011, 15}, {2012, 16},
                      {2013, 17}, {2014, 18}, {2015, 19}, {2016, 20}, {2017, 21}, {2018, 22}, {2019, 23}, {2020, 24},
                      {2021, 25}, {2022, 26}, {2023, 27}, {2024, 28}, {2025, 29}, {2026, 30}, {2027, 31}, {2028, 32},
                      {2029, 33}, {2030, 34}, {2031, 35}, {2032, 36}, {2033, 37}, {2034, 38}, {2035, 39}, {2036, 40},
                      {2037, 41}, {2038, 42}, {2039, 43}, {2040, 44}, {2041, 45}, {2042, 46}, {2043, 47}, {2044, 48},
                      {2045, 49}, {2046, 50}, {2047, 51}};

        memory.u64_ = {{1032, 1010}, {1040, 1020}, {1048, 1030}, {1056, 1040}, {1064, 1050}, {1072, 1060}, {1080, 1070},
                       {1088, 1080}, {1096, 1090}, {1104, 1100}, {1112, 1110}, {1120, 1120}, {1128, 1130}, {1136, 1140},
                       {1144, 1150}, {1152, 1160}, {1160, 1170}, {1168, 1180}, {1176, 1190}, {1184, 1200}, {1192, 1210},
                       {1200, 1220}, {1208, 1230}, {1216, 1240}, {1224, 1250}, {1232, 1260}, {1240, 1270}, {1248, 1280},
                       {1256, 1290}, {1264, 1300}, {1272, 1310}, {1280, 1320}, {1288, 1330}, {1296, 1340}, {1304, 1350},
                       {1312, 1360}, {1320, 1370}, {1328, 1380}, {1336, 1390}, {1344, 1400}, {1352, 1410}, {1360, 1420},
                       {1368, 1430}, {1376, 1440}, {1384, 1450}, {1392, 1460}, {1400, 1470}, {1408, 1480}, {1416, 1490},
                       {1424, 1500}, {1432, 1510}};

        const char bytes[] =
            "lea    -20(%rbx),%eax\n"
            "cmp    $30,%al\n"
            "ja     10\n"
            "movsbl %bl,%eax\n"
            "lea    2000(%rip),%rdi\n"
            "lea    1000(%rip),%rcx\n"
            "sub    $20,%eax\n"
            "cltq\n"
            "movzbl (%rdi,%rax,1),%eax\n"
            "mov    (%rcx,%rax,8),%rax\n"
            "jmpq   *%rax\n";
        auto instructions = engine.assemble_and_disassemble(bytes, 0, 11, 11);
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
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == 10);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == 30);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == static_cast<uint64_t>(-20));
        REQUIRE(context.get_rip(0).value_or(-1) == 24);
        REQUIRE(context.get_rip(1).value_or(-1) == 17);
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
