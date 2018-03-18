#ifndef VFS_T_TAGS_H
#define VFS_T_TAGS_H

#define UNIT_TEST_TAG           "[unit test]"
#define INTEGRATION_TEST_TAG    "[integration test]"

namespace vfs
{
    namespace test
    {
//        constexpr const char *unit_test_tag = "[unit test]";
//        constexpr const char *integration_test_tag = "[integration test]";

        static inline constexpr const char *unit_test_tag()
        {
            return "[unit test]";
        };

        static inline constexpr const char *integration_test_tag()
        {
            return "[integration test]";
        };
    }
}

#endif
