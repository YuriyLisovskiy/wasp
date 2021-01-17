/**
 * conf/loaders/tests_yaml_settings_loader.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../../src/conf/loaders/yaml_settings_loader.h"

using namespace xw;

class YamlSettingsLoaderTestCase : public ::testing::Test, public conf::YamlSettingsLoader
{

};

TEST_F(YamlSettingsLoaderTestCase, Test_overwrite_config_ScalarNotNull)
{
	YAML::Node test_map(YAML::NodeType::Map);
	test_map["test_scalar"] = "Hello";

	YAML::Node local_test_map(YAML::NodeType::Map);
	local_test_map["test_scalar"] = "World";

	overwrite_scalar_or_remove_if_null(test_map, local_test_map, "test_scalar");

	ASSERT_EQ(test_map["test_scalar"].as<std::string>(""), "World");
}

TEST_F(YamlSettingsLoaderTestCase, Test_overwrite_config_ScalarIsNull)
{
	YAML::Node test_map(YAML::NodeType::Map);
	test_map["test_scalar"] = "Hello";

	YAML::Node local_test_map(YAML::NodeType::Map);
//	local_test_map["test_scalar"] = "World";

	overwrite_scalar_or_remove_if_null(test_map, local_test_map, "test_scalar");

	ASSERT_TRUE(test_map["test_scalar"]);
	ASSERT_TRUE(test_map["test_scalar"].IsNull());
}
