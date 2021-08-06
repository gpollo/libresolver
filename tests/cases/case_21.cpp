#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_21 {

TEST_CASE("match-case-21") {
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

        memory.u64_ = {{1041, 1010}, {1049, 1020}, {1057, 1030}, {1065, 1040}, {1073, 1050}, {1081, 1060}, {1089, 1070},
                       {1097, 1080}, {1105, 1090}, {1113, 1100}, {1121, 1110}, {1129, 1120}, {1137, 1130}, {1145, 1140},
                       {1153, 1150}, {1161, 1160}, {1169, 1170}, {1177, 1180}, {1185, 1190}, {1193, 1200}, {1201, 1210},
                       {1209, 1220}, {1217, 1230}, {1225, 1240}, {1233, 1250}, {1241, 1260}, {1249, 1270}, {1257, 1280},
                       {1265, 1290}, {1273, 1300}, {1281, 1310}, {1289, 1320}, {1297, 1330}, {1305, 1340}, {1313, 1350},
                       {1321, 1360}, {1329, 1370}, {1337, 1380}, {1345, 1390}, {1353, 1400}, {1361, 1410}, {1369, 1420},
                       {1377, 1430}, {1385, 1440}, {1393, 1450}, {1401, 1460}, {1409, 1470}, {1417, 1480}, {1425, 1490},
                       {1433, 1500}, {1441, 1510}};

        const char bytes[] =
            "lea    -0x14(%rbx),%eax\n"
            "cmp    $0x1E,%al\n"
            "ja     0xA\n"
            "movsbl %bl,%eax\n"
            "lea    0x7D0(%rip),%rdi\n"
            "sub    $0x14,%eax\n"
            "cltq\n"
            "movzbl (%rdi,%rax,1),%eax\n"
            "lea    0x3E8(%rip),%rdi\n"
            "mov    (%rdi,%rax,8),%rax\n"
            "jmpq   *%rax\n";
        auto instructions = engine.assemble_and_disassemble(bytes, 0, 11, 11);
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
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == 10);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == 30);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == static_cast<uint64_t>(-20));
        REQUIRE(context.get_rip(0).value_or(-1) == 33);
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

} /* namespace tests::cases::case_21 */
