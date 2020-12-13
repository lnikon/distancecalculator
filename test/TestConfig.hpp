#ifndef TEST_CONFIG_HPP
#define TEST_CONFIG_HPP

#include <string>
#include <cstdlib>
#include <exception>
#include <stdexcept>

/**
 * @brief Environment name that stores project root
 */
const std::string DISC_PROJECT_ROOT_ENV_NAME = "DISC_PROJ_ROOT";

std::string getProjectRootPath()
{
	if (const char* penv = std::getenv(DISC_PROJECT_ROOT_ENV_NAME.data()); penv != nullptr)
	{
		return penv;
	}

	throw std::runtime_error(std::string(DISC_PROJECT_ROOT_ENV_NAME + std::string(" is not set. Can\'t determine test data location")).data());
}

/**
 * @brief Project root path 
 */
const std::string DISC_PROJECT_ROOT = getProjectRootPath();

/**
 * @brief Test root path 
 */
const std::string DISC_TEST_ROOT = DISC_PROJECT_ROOT + "/test";

#endif
